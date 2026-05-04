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
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00000
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00008
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0000c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00010
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00018
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0001c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00024
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00028
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0002c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00030
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00034
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0003c
			 0x7f800000,                                                  // inf                                         /// 00040
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00044
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0xbf028f5c,                                                  // -0.51                                       /// 00050
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00054
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00060
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00064
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00074
			 0x80011111,                                                  // -9.7958E-41                                 /// 00078
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x7f800000,                                                  // inf                                         /// 00080
			 0x0c780000,                                                  // Leading 1s:                                 /// 00084
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00088
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0008c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00090
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00094
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00098
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0009c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000a4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000cc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000d0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000e8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000f4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000f8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00100
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00104
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00108
			 0x00011111,                                                  // 9.7958E-41                                  /// 0010c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00110
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00114
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0011c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00124
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00128
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00130
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00134
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00138
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0013c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00140
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00148
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0014c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00150
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00158
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0015c
			 0x80000000,                                                  // -zero                                       /// 00160
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00168
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00170
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00174
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00178
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0017c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00180
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00188
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0018c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00194
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00198
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x0e000003,                                                  // Trailing 1s:                                /// 001a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001a4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 001c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001cc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001dc
			 0x7fc00001,                                                  // signaling NaN                               /// 001e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 001e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001f0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00200
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00208
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0xffc00001,                                                  // -signaling NaN                              /// 00214
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x7fc00001,                                                  // signaling NaN                               /// 0021c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00220
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00224
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x0c780000,                                                  // Leading 1s:                                 /// 0022c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00230
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00234
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00238
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0023c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00240
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0024c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00250
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0025c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00260
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00264
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0026c
			 0xff800000,                                                  // -inf                                        /// 00270
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0c780000,                                                  // Leading 1s:                                 /// 00278
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0027c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00280
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00290
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00294
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00298
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0029c
			 0x33333333,                                                  // 4 random values                             /// 002a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002ac
			 0x55555555,                                                  // 4 random values                             /// 002b0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x80000000,                                                  // -zero                                       /// 002c0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002c8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 002d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 002e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002ec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002f0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00300
			 0x0c600000,                                                  // Leading 1s:                                 /// 00304
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0030c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00314
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0031c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00320
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00324
			 0x3f028f5c,                                                  // 0.51                                        /// 00328
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0032c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00330
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00334
			 0x33333333,                                                  // 4 random values                             /// 00338
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00344
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00348
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00354
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00358
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0035c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00360
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00364
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0xbf028f5c,                                                  // -0.51                                       /// 0036c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0037c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00380
			 0x0e000007,                                                  // Trailing 1s:                                /// 00384
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00388
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00398
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0039c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003bc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0xff800000,                                                  // -inf                                        /// 003e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003f8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0x55555555,                                                  // 4 random values                             /// 00400
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00404
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00408
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0041c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00420
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0042c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00430
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00434
			 0xcb000000,                                                  // -8388608.0                                  /// 00438
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0043c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00440
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0044c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00450
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00454
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00458
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0045c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00464
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00468
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00470
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00474
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00478
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0047c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00480
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00488
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0048c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00494
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00498
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x00000000,                                                  // zero                                        /// 004a0
			 0x4b000000,                                                  // 8388608.0                                   /// 004a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004ac
			 0x7f800000,                                                  // inf                                         /// 004b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004b4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 004d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 004e8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0xcb000000,                                                  // -8388608.0                                  /// 004f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00500
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00504
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00508
			 0x3f028f5c,                                                  // 0.51                                        /// 0050c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x0c700000,                                                  // Leading 1s:                                 /// 00514
			 0x55555555,                                                  // 4 random values                             /// 00518
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0051c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00520
			 0xffc00001,                                                  // -signaling NaN                              /// 00524
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0052c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00534
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00538
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00540
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00544
			 0x7f800000,                                                  // inf                                         /// 00548
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00554
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00558
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0055c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00560
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00564
			 0x0e000007,                                                  // Trailing 1s:                                /// 00568
			 0x4b000000,                                                  // 8388608.0                                   /// 0056c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00570
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00578
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0057c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00584
			 0x0e000007,                                                  // Trailing 1s:                                /// 00588
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0058c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00590
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00594
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00598
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0059c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x7f800000,                                                  // inf                                         /// 005b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005c0
			 0x3f028f5c,                                                  // 0.51                                        /// 005c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005e0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005f8
			 0xcb000000,                                                  // -8388608.0                                  /// 005fc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00604
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00608
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0060c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00610
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00614
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00618
			 0x80000000,                                                  // -zero                                       /// 0061c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00620
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00624
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0062c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00630
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00634
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0063c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00640
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00644
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00648
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0064c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00650
			 0x80011111,                                                  // -9.7958E-41                                 /// 00654
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00658
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0065c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00660
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00664
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0xffc00001,                                                  // -signaling NaN                              /// 00670
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00674
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00680
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00684
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00688
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0068c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00690
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00694
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0069c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 006a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x7f800000,                                                  // inf                                         /// 006b4
			 0x0c780000,                                                  // Leading 1s:                                 /// 006b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006c0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006cc
			 0xbf028f5c,                                                  // -0.51                                       /// 006d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006dc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006e8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006f0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006f4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00704
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0070c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00710
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00714
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00718
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0071c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00720
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00728
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00730
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00734
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00738
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x00000000,                                                  // zero                                        /// 00740
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00744
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00748
			 0xff800000,                                                  // -inf                                        /// 0074c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00750
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00754
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0075c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00760
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00764
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00768
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0076c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00774
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0077c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00780
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00788
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0079c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007a4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 007cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007d0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x7f800000,                                                  // inf                                         /// 007d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007e8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007f4
			 0x7fc00001,                                                  // signaling NaN                               /// 007f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007fc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00800
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0080c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00810
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00818
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00820
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00828
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0082c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00830
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00834
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00840
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00848
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0084c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00850
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00854
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0085c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00860
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00864
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00868
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00878
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0087c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00880
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00884
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00888
			 0x0c600000,                                                  // Leading 1s:                                 /// 0088c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00890
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00894
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00898
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x7fc00001,                                                  // signaling NaN                               /// 008c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008cc
			 0x80000000,                                                  // -zero                                       /// 008d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008d8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008e4
			 0x00000000,                                                  // zero                                        /// 008e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008f8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00904
			 0x7fc00001,                                                  // signaling NaN                               /// 00908
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00910
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00914
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00918
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0091c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00920
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0092c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00930
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00938
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00940
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x0c400000,                                                  // Leading 1s:                                 /// 00948
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0094c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00950
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00954
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00958
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00960
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00968
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0096c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00970
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00974
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00978
			 0x00011111,                                                  // 9.7958E-41                                  /// 0097c
			 0x7f800000,                                                  // inf                                         /// 00980
			 0x4b000000,                                                  // 8388608.0                                   /// 00984
			 0x33333333,                                                  // 4 random values                             /// 00988
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0098c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00990
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00998
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0099c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009a4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009a8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009b4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009c0
			 0xffc00001,                                                  // -signaling NaN                              /// 009c4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 009e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009e8
			 0x33333333,                                                  // 4 random values                             /// 009ec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009f4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a00
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a04
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a14
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a20
			 0x3f028f5c,                                                  // 0.51                                        /// 00a24
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a28
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a2c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a38
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a3c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a44
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a48
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a4c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a50
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a54
			 0x4b000000,                                                  // 8388608.0                                   /// 00a58
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a5c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a60
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a68
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a70
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a74
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a80
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a84
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a88
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a94
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a9c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00aa4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00aa8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00aac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ab0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ab4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ab8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00abc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ac8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ad0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ad8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00adc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ae0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ae8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00aec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00af0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0xcb000000,                                                  // -8388608.0                                  /// 00af8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00afc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b00
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b04
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b0c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x55555555,                                                  // 4 random values                             /// 00b14
			 0xffc00001,                                                  // -signaling NaN                              /// 00b18
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b1c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b20
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b24
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b2c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b30
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b3c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b40
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b50
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b54
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b58
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b60
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b64
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b6c
			 0x00000000,                                                  // zero                                        /// 00b70
			 0x4b000000,                                                  // 8388608.0                                   /// 00b74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b7c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b80
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b84
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b88
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b8c
			 0x4b000000,                                                  // 8388608.0                                   /// 00b90
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b94
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b9c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ba0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ba4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ba8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bac
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bb0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bb8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bbc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bc0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bc4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bc8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bd8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00be4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00be8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bf0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bf4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bf8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bfc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c04
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c08
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c0c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c10
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c18
			 0xcb000000,                                                  // -8388608.0                                  /// 00c1c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c20
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c24
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c2c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c30
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c34
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c38
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c40
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c44
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c48
			 0xbf028f5c,                                                  // -0.51                                       /// 00c4c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c50
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c58
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c68
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c6c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c70
			 0x55555555,                                                  // 4 random values                             /// 00c74
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c78
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c80
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c84
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c90
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c94
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c98
			 0xff800000,                                                  // -inf                                        /// 00c9c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ca0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ca4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ca8
			 0x7fc00001,                                                  // signaling NaN                               /// 00cac
			 0xff800000,                                                  // -inf                                        /// 00cb0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0xbf028f5c,                                                  // -0.51                                       /// 00cb8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cbc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cc0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cc8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ccc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cd4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cd8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ce0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ce8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cf0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cf4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cf8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cfc
			 0x55555555,                                                  // 4 random values                             /// 00d00
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d04
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d08
			 0x00000000,                                                  // zero                                        /// 00d0c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d10
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0xffc00001,                                                  // -signaling NaN                              /// 00d1c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d28
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d30
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d38
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d3c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d40
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d50
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d58
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d5c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d60
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d64
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d68
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d6c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d70
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d78
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d7c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d80
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d84
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d90
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d98
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00da0
			 0xbf028f5c,                                                  // -0.51                                       /// 00da4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00da8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00db0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00db4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00db8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dc0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dc4
			 0xffc00001,                                                  // -signaling NaN                              /// 00dc8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dcc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dd0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dd4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x7f800000,                                                  // inf                                         /// 00ddc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00de0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00de4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00de8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00dec
			 0x0c400000,                                                  // Leading 1s:                                 /// 00df0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00df4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dfc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e0c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e10
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e14
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e18
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e1c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e20
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e24
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e28
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e2c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e30
			 0xff800000,                                                  // -inf                                        /// 00e34
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e3c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e40
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e48
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e50
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x3f028f5c,                                                  // 0.51                                        /// 00e58
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e68
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e6c
			 0xcb000000,                                                  // -8388608.0                                  /// 00e70
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e74
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e78
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e80
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e84
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e88
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e8c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e90
			 0x55555555,                                                  // 4 random values                             /// 00e94
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e98
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e9c
			 0x00000000,                                                  // zero                                        /// 00ea0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ea4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ea8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00eac
			 0x0c700000,                                                  // Leading 1s:                                 /// 00eb0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00eb4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ebc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ec4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ec8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ecc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ed0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ed4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00edc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ee4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ee8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00eec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ef0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ef8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00efc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f04
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f18
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f1c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f20
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f24
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f28
			 0x4b000000,                                                  // 8388608.0                                   /// 00f2c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f30
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f34
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f38
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f44
			 0x00000000,                                                  // zero                                        /// 00f48
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f4c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f54
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f5c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f60
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x7f800000,                                                  // inf                                         /// 00f68
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f6c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f70
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f74
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f7c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f80
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f84
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f8c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f90
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f94
			 0x7f800000,                                                  // inf                                         /// 00f98
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f9c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fa8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fb0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fb8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fbc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fc8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fd0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fdc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fe0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fe4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ff8
			 0x7f7ffffe // max norm - 3ulp                               // max norm +ve                                /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x260d319c, /// 0x0
			 0x69c60882, /// 0x4
			 0xffffbb06, /// 0x8
			 0x0d0185fd, /// 0xc
			 0x2b1345b6, /// 0x10
			 0xb24de345, /// 0x14
			 0x4f245f56, /// 0x18
			 0x34c90063, /// 0x1c
			 0x19d7eda9, /// 0x20
			 0xfbe8f28d, /// 0x24
			 0x8c691c4b, /// 0x28
			 0xeac390bc, /// 0x2c
			 0xdcd228a5, /// 0x30
			 0x20e961de, /// 0x34
			 0xe5e9fd64, /// 0x38
			 0x0272a557, /// 0x3c
			 0x75b8e632, /// 0x40
			 0xeaa6f276, /// 0x44
			 0x792a75ec, /// 0x48
			 0x94e12580, /// 0x4c
			 0xfaee6ab9, /// 0x50
			 0xb7ad4813, /// 0x54
			 0xf433900e, /// 0x58
			 0xf35fae07, /// 0x5c
			 0x59941e84, /// 0x60
			 0x4fdaa33c, /// 0x64
			 0x31ec23bd, /// 0x68
			 0x67af0ea7, /// 0x6c
			 0x579cc142, /// 0x70
			 0xe31aa03c, /// 0x74
			 0xb1d51b06, /// 0x78
			 0x97309350, /// 0x7c
			 0xfb040958, /// 0x80
			 0x469f5f77, /// 0x84
			 0x03b49ac3, /// 0x88
			 0x468c89b2, /// 0x8c
			 0x56ca1c87, /// 0x90
			 0xf80ea29e, /// 0x94
			 0x7be82e0c, /// 0x98
			 0x1922446c, /// 0x9c
			 0xf3e7b484, /// 0xa0
			 0x07b91ad2, /// 0xa4
			 0xea8f55ac, /// 0xa8
			 0x1c54866d, /// 0xac
			 0x24ce188d, /// 0xb0
			 0x07e89a31, /// 0xb4
			 0x35d28af9, /// 0xb8
			 0xb62c8a65, /// 0xbc
			 0xcaa271c4, /// 0xc0
			 0x212af484, /// 0xc4
			 0x7d7c0e7c, /// 0xc8
			 0x8c9cf5a5, /// 0xcc
			 0x7ab502df, /// 0xd0
			 0x2361fc40, /// 0xd4
			 0x0107da49, /// 0xd8
			 0x5f5014ca, /// 0xdc
			 0xd44bca5a, /// 0xe0
			 0x96c3926b, /// 0xe4
			 0x556b152c, /// 0xe8
			 0x7d66a01a, /// 0xec
			 0x7d60de42, /// 0xf0
			 0x81480810, /// 0xf4
			 0x79963af3, /// 0xf8
			 0xbd9ffbd3, /// 0xfc
			 0x9c15a776, /// 0x100
			 0x15f3143c, /// 0x104
			 0xce183934, /// 0x108
			 0x9ecb95fa, /// 0x10c
			 0x636f9586, /// 0x110
			 0x602e00ca, /// 0x114
			 0x5fbadd5c, /// 0x118
			 0xea2fcdb5, /// 0x11c
			 0xe9221e23, /// 0x120
			 0x14bf996b, /// 0x124
			 0xa4c24d31, /// 0x128
			 0x127e1687, /// 0x12c
			 0xadb8a9fc, /// 0x130
			 0xf018e202, /// 0x134
			 0x4c400e6c, /// 0x138
			 0x1f7655f7, /// 0x13c
			 0xe26a773c, /// 0x140
			 0x9bdf0aed, /// 0x144
			 0x58d5783b, /// 0x148
			 0x0d8ae493, /// 0x14c
			 0xd6a76ba7, /// 0x150
			 0xeb066ce9, /// 0x154
			 0x65953983, /// 0x158
			 0x5ae423c7, /// 0x15c
			 0xb8b9b3ec, /// 0x160
			 0x0bd5759e, /// 0x164
			 0x41fabb1b, /// 0x168
			 0x2f9870be, /// 0x16c
			 0xfb9f1f8f, /// 0x170
			 0x639466bf, /// 0x174
			 0x301ccb3b, /// 0x178
			 0xbf2464e6, /// 0x17c
			 0x8a1a851e, /// 0x180
			 0xd8053c3b, /// 0x184
			 0x6d4cf66a, /// 0x188
			 0x426e153a, /// 0x18c
			 0x90b823cd, /// 0x190
			 0xc0f9bef6, /// 0x194
			 0x5dff3837, /// 0x198
			 0x9fca62d0, /// 0x19c
			 0x48b9e963, /// 0x1a0
			 0xf364ddf1, /// 0x1a4
			 0xef6e8642, /// 0x1a8
			 0x1cf4e61a, /// 0x1ac
			 0xccb67c2f, /// 0x1b0
			 0x0d29c07d, /// 0x1b4
			 0xbd893de3, /// 0x1b8
			 0xf41c1cef, /// 0x1bc
			 0xe7f3a216, /// 0x1c0
			 0x1f6ee387, /// 0x1c4
			 0xee77340b, /// 0x1c8
			 0x1ca3950b, /// 0x1cc
			 0x273bc465, /// 0x1d0
			 0x41d12024, /// 0x1d4
			 0xfce775f2, /// 0x1d8
			 0x2112ef34, /// 0x1dc
			 0x99e0301e, /// 0x1e0
			 0xe22390ee, /// 0x1e4
			 0x7c8717e2, /// 0x1e8
			 0x4cd210ba, /// 0x1ec
			 0x3eb28c07, /// 0x1f0
			 0xb6bda079, /// 0x1f4
			 0x5f58e7b1, /// 0x1f8
			 0x1b9fb08f, /// 0x1fc
			 0x21a0b58a, /// 0x200
			 0xfc3d9a4c, /// 0x204
			 0x688ab2d4, /// 0x208
			 0x38394b5f, /// 0x20c
			 0x7a1bc77f, /// 0x210
			 0x0c14b19c, /// 0x214
			 0x0d70e7a2, /// 0x218
			 0x5e5a46f5, /// 0x21c
			 0x05f07e36, /// 0x220
			 0xdfa9086c, /// 0x224
			 0x8a75ad04, /// 0x228
			 0xd77b417c, /// 0x22c
			 0x15a809e9, /// 0x230
			 0x0653e343, /// 0x234
			 0x2ca2f3b8, /// 0x238
			 0x2298dd69, /// 0x23c
			 0xccc23381, /// 0x240
			 0x88aa22d3, /// 0x244
			 0x89ba048a, /// 0x248
			 0xad7f0c10, /// 0x24c
			 0xb8cf51f4, /// 0x250
			 0x2edebd8f, /// 0x254
			 0x78b280e2, /// 0x258
			 0x0410e9a6, /// 0x25c
			 0x69ab3d99, /// 0x260
			 0x98889728, /// 0x264
			 0x47f59fd2, /// 0x268
			 0x985b05da, /// 0x26c
			 0x0bfbf39a, /// 0x270
			 0x3f2730af, /// 0x274
			 0x971ad551, /// 0x278
			 0xae0e27fb, /// 0x27c
			 0x31c1901e, /// 0x280
			 0xaa3297f3, /// 0x284
			 0x88291b5c, /// 0x288
			 0x32110eee, /// 0x28c
			 0xc2d36657, /// 0x290
			 0x0fc7c005, /// 0x294
			 0x22bceb55, /// 0x298
			 0x2e6c36d6, /// 0x29c
			 0x5ed52b5f, /// 0x2a0
			 0x04829041, /// 0x2a4
			 0x380a4eaf, /// 0x2a8
			 0x828cdb1a, /// 0x2ac
			 0xd149e38b, /// 0x2b0
			 0xcb842bd1, /// 0x2b4
			 0x10d31a6b, /// 0x2b8
			 0xc64591c0, /// 0x2bc
			 0x8bb33376, /// 0x2c0
			 0xca8ca741, /// 0x2c4
			 0x63cc4b7a, /// 0x2c8
			 0xc937fa60, /// 0x2cc
			 0x924965e0, /// 0x2d0
			 0x76cd5356, /// 0x2d4
			 0xe0ee2c6d, /// 0x2d8
			 0x0ff792f1, /// 0x2dc
			 0x52c8becc, /// 0x2e0
			 0x4b440d6f, /// 0x2e4
			 0xac63b445, /// 0x2e8
			 0x6d156f71, /// 0x2ec
			 0x846bce61, /// 0x2f0
			 0xf1f15978, /// 0x2f4
			 0x46814905, /// 0x2f8
			 0x09625093, /// 0x2fc
			 0xcd6b003c, /// 0x300
			 0xd80976ed, /// 0x304
			 0xa0237f1b, /// 0x308
			 0xc0c5f56b, /// 0x30c
			 0x4c9c04fa, /// 0x310
			 0x3345ffac, /// 0x314
			 0x2bd86cbb, /// 0x318
			 0x93ce3b32, /// 0x31c
			 0x36026c17, /// 0x320
			 0x3eb74458, /// 0x324
			 0x28b4c61a, /// 0x328
			 0xbc3a2625, /// 0x32c
			 0x15d289de, /// 0x330
			 0xf3fb6e20, /// 0x334
			 0xcafc7515, /// 0x338
			 0x3db0a77a, /// 0x33c
			 0x02775f24, /// 0x340
			 0x0efd217c, /// 0x344
			 0x286271cd, /// 0x348
			 0x391fc50f, /// 0x34c
			 0x963582a0, /// 0x350
			 0x09977e18, /// 0x354
			 0x7e7b9332, /// 0x358
			 0xeb62dc26, /// 0x35c
			 0x04151e89, /// 0x360
			 0xb9a57345, /// 0x364
			 0x85968760, /// 0x368
			 0x99b6d9cb, /// 0x36c
			 0xd3c2b42e, /// 0x370
			 0xb634829e, /// 0x374
			 0xfb1f01d0, /// 0x378
			 0xb8838891, /// 0x37c
			 0xebe55dc7, /// 0x380
			 0xef26d83e, /// 0x384
			 0xd9011703, /// 0x388
			 0x56d2e2ee, /// 0x38c
			 0x1a4b262f, /// 0x390
			 0xbea2f472, /// 0x394
			 0x906f6e46, /// 0x398
			 0x3c123716, /// 0x39c
			 0xc43aa80b, /// 0x3a0
			 0xd6bdf1e0, /// 0x3a4
			 0x56d19002, /// 0x3a8
			 0x37d2a1bb, /// 0x3ac
			 0x74754648, /// 0x3b0
			 0xbc2199fc, /// 0x3b4
			 0xbc8d6ee7, /// 0x3b8
			 0x9656669f, /// 0x3bc
			 0x0c355e36, /// 0x3c0
			 0xe5907e16, /// 0x3c4
			 0x0f31c992, /// 0x3c8
			 0xf8d961f1, /// 0x3cc
			 0xb685c27b, /// 0x3d0
			 0xa9390617, /// 0x3d4
			 0xb10d6228, /// 0x3d8
			 0x265c5c35, /// 0x3dc
			 0xb7520691, /// 0x3e0
			 0xde53bfcf, /// 0x3e4
			 0xf1fb6b8a, /// 0x3e8
			 0x414f2df5, /// 0x3ec
			 0x63968f40, /// 0x3f0
			 0xa567f4f1, /// 0x3f4
			 0x7a89d738, /// 0x3f8
			 0xbdcbf546, /// 0x3fc
			 0x8f3804cc, /// 0x400
			 0x60403f93, /// 0x404
			 0xb5266c05, /// 0x408
			 0xb7529bfe, /// 0x40c
			 0x2f0f41e4, /// 0x410
			 0x8fd51d71, /// 0x414
			 0xd216b4c1, /// 0x418
			 0x7846082b, /// 0x41c
			 0x517732ac, /// 0x420
			 0xb3a22cc9, /// 0x424
			 0x3b168067, /// 0x428
			 0x7a7a00f2, /// 0x42c
			 0x1bff716c, /// 0x430
			 0xa4c625f2, /// 0x434
			 0xf036a580, /// 0x438
			 0x9b2fa75e, /// 0x43c
			 0xef76ca1f, /// 0x440
			 0xdc70a127, /// 0x444
			 0x5141fca7, /// 0x448
			 0x2b7480a8, /// 0x44c
			 0xc89adba5, /// 0x450
			 0x5709471c, /// 0x454
			 0x0ba6f78d, /// 0x458
			 0x990380cf, /// 0x45c
			 0x6b79e0be, /// 0x460
			 0x068e61ce, /// 0x464
			 0xe0863794, /// 0x468
			 0x5b8a4d5a, /// 0x46c
			 0xca92027d, /// 0x470
			 0xf5cbaa29, /// 0x474
			 0x7d7a07ed, /// 0x478
			 0x16a18857, /// 0x47c
			 0x3ce510a9, /// 0x480
			 0x936cae54, /// 0x484
			 0xa41af286, /// 0x488
			 0xb4eebc34, /// 0x48c
			 0xd30e06a6, /// 0x490
			 0xed039710, /// 0x494
			 0x8fc3df89, /// 0x498
			 0xb13ae130, /// 0x49c
			 0x065e6984, /// 0x4a0
			 0xdb536b86, /// 0x4a4
			 0x245ad383, /// 0x4a8
			 0xbbde760d, /// 0x4ac
			 0x431b8a93, /// 0x4b0
			 0x753b2044, /// 0x4b4
			 0xc32bb069, /// 0x4b8
			 0xc8a05901, /// 0x4bc
			 0x17cd1399, /// 0x4c0
			 0x05aead5d, /// 0x4c4
			 0x8f53fe79, /// 0x4c8
			 0x1357ad95, /// 0x4cc
			 0xb601bfe7, /// 0x4d0
			 0xed3e4d3b, /// 0x4d4
			 0x3b5c29ab, /// 0x4d8
			 0x89d66d4b, /// 0x4dc
			 0x2ee26730, /// 0x4e0
			 0x1a102e0a, /// 0x4e4
			 0x1f89296c, /// 0x4e8
			 0x3c75c2c2, /// 0x4ec
			 0x8a987d98, /// 0x4f0
			 0x91d8f8a2, /// 0x4f4
			 0xbb8e3210, /// 0x4f8
			 0xf6611e4e, /// 0x4fc
			 0x0e62e08c, /// 0x500
			 0x33b3ce00, /// 0x504
			 0xb77aacc0, /// 0x508
			 0xa70af229, /// 0x50c
			 0xe79e631b, /// 0x510
			 0x5cd5bdf7, /// 0x514
			 0x57aff74f, /// 0x518
			 0xebd3926c, /// 0x51c
			 0x67f07fb1, /// 0x520
			 0xad8e03ca, /// 0x524
			 0xa5c8d93c, /// 0x528
			 0x97c23540, /// 0x52c
			 0xb32302aa, /// 0x530
			 0x32b3869e, /// 0x534
			 0x11195744, /// 0x538
			 0x1facc350, /// 0x53c
			 0x8b19925e, /// 0x540
			 0xe1c83ef1, /// 0x544
			 0xd9bae1a2, /// 0x548
			 0x9fe05db3, /// 0x54c
			 0x71a8e6c9, /// 0x550
			 0xe999d143, /// 0x554
			 0x6a2051b8, /// 0x558
			 0x50704080, /// 0x55c
			 0x09c0ba24, /// 0x560
			 0x2b246ddc, /// 0x564
			 0x7ea82cc9, /// 0x568
			 0xdad6f54b, /// 0x56c
			 0x0137fdfe, /// 0x570
			 0x0b7fee8d, /// 0x574
			 0x279cc4c3, /// 0x578
			 0xa13802c0, /// 0x57c
			 0x44850dcd, /// 0x580
			 0xe6a480da, /// 0x584
			 0x50feabd3, /// 0x588
			 0x5fd5bdcb, /// 0x58c
			 0x3d87102e, /// 0x590
			 0x26805492, /// 0x594
			 0x480bd505, /// 0x598
			 0x5220c239, /// 0x59c
			 0xb37ba271, /// 0x5a0
			 0x5d93b6ae, /// 0x5a4
			 0xd0ad27ae, /// 0x5a8
			 0x8bc77677, /// 0x5ac
			 0x4e3896db, /// 0x5b0
			 0x41220247, /// 0x5b4
			 0x2ada61b6, /// 0x5b8
			 0x4446c8dc, /// 0x5bc
			 0x66f3ffbc, /// 0x5c0
			 0x92fdaecf, /// 0x5c4
			 0xb9eaffa8, /// 0x5c8
			 0xece051cb, /// 0x5cc
			 0x444bd268, /// 0x5d0
			 0xb187ffad, /// 0x5d4
			 0x2c935a88, /// 0x5d8
			 0x67cf1174, /// 0x5dc
			 0x4e761f98, /// 0x5e0
			 0xf04519b4, /// 0x5e4
			 0xf6311661, /// 0x5e8
			 0x00daa01e, /// 0x5ec
			 0x85c71523, /// 0x5f0
			 0x2d1b0e8b, /// 0x5f4
			 0xfea08fc6, /// 0x5f8
			 0xff2b1647, /// 0x5fc
			 0xadb7686a, /// 0x600
			 0x6a8de9c0, /// 0x604
			 0x79961801, /// 0x608
			 0xad3fa3e6, /// 0x60c
			 0xe8af4090, /// 0x610
			 0xc818ddc5, /// 0x614
			 0x6154c0cd, /// 0x618
			 0x3933c6dd, /// 0x61c
			 0x6c626b54, /// 0x620
			 0xad2dea2d, /// 0x624
			 0x6ba9d3a4, /// 0x628
			 0x82c30fc9, /// 0x62c
			 0x91b44e76, /// 0x630
			 0xa5948731, /// 0x634
			 0x6d3aff23, /// 0x638
			 0xd3496819, /// 0x63c
			 0xe1149c1b, /// 0x640
			 0xdc6607ba, /// 0x644
			 0x90d8c5b0, /// 0x648
			 0x7cb6b7d2, /// 0x64c
			 0x1fdb7478, /// 0x650
			 0xb23ec1e3, /// 0x654
			 0xa2798c8e, /// 0x658
			 0x9c604a82, /// 0x65c
			 0x611d7b86, /// 0x660
			 0xbb2caf44, /// 0x664
			 0x41b7b364, /// 0x668
			 0xe8d8b233, /// 0x66c
			 0xac9e1c00, /// 0x670
			 0x2be7f1bd, /// 0x674
			 0x60a898ee, /// 0x678
			 0x2475642f, /// 0x67c
			 0x8163e18c, /// 0x680
			 0xe4969d88, /// 0x684
			 0x8fee7dc4, /// 0x688
			 0x1c352215, /// 0x68c
			 0x28159f08, /// 0x690
			 0x938597ba, /// 0x694
			 0x572fae93, /// 0x698
			 0x4ddf9cdd, /// 0x69c
			 0x73f595c8, /// 0x6a0
			 0x8f804242, /// 0x6a4
			 0x65b22f18, /// 0x6a8
			 0xe0a35969, /// 0x6ac
			 0xe556d1ee, /// 0x6b0
			 0x5b001a6c, /// 0x6b4
			 0x35108f83, /// 0x6b8
			 0xb85fc91f, /// 0x6bc
			 0x8d4ee06d, /// 0x6c0
			 0xfa892101, /// 0x6c4
			 0x8b5454dd, /// 0x6c8
			 0x333a3082, /// 0x6cc
			 0x2bd3a256, /// 0x6d0
			 0xe612c72c, /// 0x6d4
			 0x24b3ca35, /// 0x6d8
			 0xe1f631df, /// 0x6dc
			 0xd69a8a7b, /// 0x6e0
			 0x2dd50a55, /// 0x6e4
			 0x2eb12bcf, /// 0x6e8
			 0x74c57761, /// 0x6ec
			 0x0e1e6844, /// 0x6f0
			 0xb3127bcd, /// 0x6f4
			 0xdd356bed, /// 0x6f8
			 0x7067b329, /// 0x6fc
			 0xd512058c, /// 0x700
			 0xddfe6cb9, /// 0x704
			 0xdbac8741, /// 0x708
			 0x2cbb955f, /// 0x70c
			 0x41346000, /// 0x710
			 0xd0f235b1, /// 0x714
			 0xdd9283a0, /// 0x718
			 0xf4073806, /// 0x71c
			 0x2119bd5f, /// 0x720
			 0x6befacf0, /// 0x724
			 0x070bebd8, /// 0x728
			 0x5663f0fd, /// 0x72c
			 0x8349a323, /// 0x730
			 0x0f4f934e, /// 0x734
			 0x06bbb0f6, /// 0x738
			 0xb3fedb7a, /// 0x73c
			 0x8c4076a1, /// 0x740
			 0xea99c362, /// 0x744
			 0x12d23bc6, /// 0x748
			 0x1b806283, /// 0x74c
			 0xdd4e904e, /// 0x750
			 0x0da657b2, /// 0x754
			 0x9b8afaef, /// 0x758
			 0xb329a465, /// 0x75c
			 0x157b712b, /// 0x760
			 0x9fed83e6, /// 0x764
			 0xdec40f84, /// 0x768
			 0xd1967c16, /// 0x76c
			 0x7ee4cfb0, /// 0x770
			 0x5e10dcfa, /// 0x774
			 0xde1c2221, /// 0x778
			 0xe6d1f727, /// 0x77c
			 0xb3bcb11c, /// 0x780
			 0x1f83c363, /// 0x784
			 0x5ac01651, /// 0x788
			 0xb97e107f, /// 0x78c
			 0x375ae6d8, /// 0x790
			 0xde6a4fa9, /// 0x794
			 0xa43f99dc, /// 0x798
			 0xd9bedb64, /// 0x79c
			 0xbe8c0564, /// 0x7a0
			 0xc6fed35f, /// 0x7a4
			 0x77c03bf5, /// 0x7a8
			 0xa05eb002, /// 0x7ac
			 0x246e26f4, /// 0x7b0
			 0xbcf880f4, /// 0x7b4
			 0x847f4ed4, /// 0x7b8
			 0x0fec5c88, /// 0x7bc
			 0x413f2541, /// 0x7c0
			 0xc248aca4, /// 0x7c4
			 0x55bb7b13, /// 0x7c8
			 0x16e5120e, /// 0x7cc
			 0x33ddc002, /// 0x7d0
			 0x3e2d11bd, /// 0x7d4
			 0xa3162932, /// 0x7d8
			 0x678df5c3, /// 0x7dc
			 0xb4f82c89, /// 0x7e0
			 0x8dbec1d0, /// 0x7e4
			 0xa171388d, /// 0x7e8
			 0x377ebb53, /// 0x7ec
			 0x3dceec7e, /// 0x7f0
			 0x81d1e513, /// 0x7f4
			 0x1a3b4e43, /// 0x7f8
			 0x62abe3ab, /// 0x7fc
			 0xc473e6bf, /// 0x800
			 0x1c3a9ec5, /// 0x804
			 0x8b959852, /// 0x808
			 0xc41ade8e, /// 0x80c
			 0x36ddf82f, /// 0x810
			 0x69e9f36a, /// 0x814
			 0xac46df4e, /// 0x818
			 0x12576cd8, /// 0x81c
			 0xaec3ab9f, /// 0x820
			 0xbfaab896, /// 0x824
			 0x244ef34a, /// 0x828
			 0x57e58d99, /// 0x82c
			 0x3246af0d, /// 0x830
			 0xfa4d20db, /// 0x834
			 0x290c4a12, /// 0x838
			 0x99feb112, /// 0x83c
			 0x3c657a54, /// 0x840
			 0xc71f1870, /// 0x844
			 0xf644415e, /// 0x848
			 0x8b6c812c, /// 0x84c
			 0xe0d044b1, /// 0x850
			 0x70ec2693, /// 0x854
			 0x536093f2, /// 0x858
			 0x9ebac44f, /// 0x85c
			 0x681f629e, /// 0x860
			 0x156b80de, /// 0x864
			 0x2d58679b, /// 0x868
			 0x1698c696, /// 0x86c
			 0x67e6f9d6, /// 0x870
			 0x80a7bb20, /// 0x874
			 0xd6e05846, /// 0x878
			 0x328bf0ae, /// 0x87c
			 0x19a30603, /// 0x880
			 0x1c24ac42, /// 0x884
			 0xdd6fdf9a, /// 0x888
			 0x82738b4d, /// 0x88c
			 0x3023c657, /// 0x890
			 0x79b4332c, /// 0x894
			 0xd3490f2c, /// 0x898
			 0xa0d68ca1, /// 0x89c
			 0x228cdb85, /// 0x8a0
			 0xbc8c3bef, /// 0x8a4
			 0x1ab2d87d, /// 0x8a8
			 0x717cb2be, /// 0x8ac
			 0xcae80a0b, /// 0x8b0
			 0xcf07f840, /// 0x8b4
			 0x63856a02, /// 0x8b8
			 0x35954d34, /// 0x8bc
			 0x39ee13d9, /// 0x8c0
			 0xc0b81da1, /// 0x8c4
			 0x907ba6c5, /// 0x8c8
			 0x3c04aee2, /// 0x8cc
			 0x238c2104, /// 0x8d0
			 0x6fcc7bee, /// 0x8d4
			 0x4533c341, /// 0x8d8
			 0x99e8d12a, /// 0x8dc
			 0x89255d70, /// 0x8e0
			 0x7e2cc31a, /// 0x8e4
			 0x48062a3b, /// 0x8e8
			 0x649becd6, /// 0x8ec
			 0xf43e1b5b, /// 0x8f0
			 0x36569cac, /// 0x8f4
			 0xb44b79ed, /// 0x8f8
			 0xcab18876, /// 0x8fc
			 0xccd19350, /// 0x900
			 0xa7e015c5, /// 0x904
			 0xc5116e10, /// 0x908
			 0x767b6f39, /// 0x90c
			 0x5400fa8b, /// 0x910
			 0xc533f0c8, /// 0x914
			 0xa09c2618, /// 0x918
			 0x0a115ec5, /// 0x91c
			 0x8e4d0410, /// 0x920
			 0x84991492, /// 0x924
			 0x71d07006, /// 0x928
			 0x15e079db, /// 0x92c
			 0x713d81cc, /// 0x930
			 0xdd05762a, /// 0x934
			 0x70a6486e, /// 0x938
			 0x49a0219b, /// 0x93c
			 0x8bc5cfd4, /// 0x940
			 0xace4ae0f, /// 0x944
			 0xe4c174b4, /// 0x948
			 0x1deba2e4, /// 0x94c
			 0x2edcb186, /// 0x950
			 0xb1ddcb23, /// 0x954
			 0xff008e1d, /// 0x958
			 0xabc5396c, /// 0x95c
			 0x9ed7a8e8, /// 0x960
			 0x711ee09b, /// 0x964
			 0x7a0d6719, /// 0x968
			 0x35da8d85, /// 0x96c
			 0xce9f6945, /// 0x970
			 0xfe8b3c10, /// 0x974
			 0x9b5eebf3, /// 0x978
			 0x19c6d806, /// 0x97c
			 0xe50f990f, /// 0x980
			 0x8496efcd, /// 0x984
			 0x6c06d540, /// 0x988
			 0x99d8c9fe, /// 0x98c
			 0x9b309539, /// 0x990
			 0x97e5797b, /// 0x994
			 0x0df608f3, /// 0x998
			 0x8c74c2cf, /// 0x99c
			 0x47ff6c8c, /// 0x9a0
			 0x77fbb29f, /// 0x9a4
			 0xd42282ba, /// 0x9a8
			 0xa6766c35, /// 0x9ac
			 0x95287eff, /// 0x9b0
			 0x0684b1ed, /// 0x9b4
			 0xc6ebde93, /// 0x9b8
			 0x6a83a6cb, /// 0x9bc
			 0x0c1e3e74, /// 0x9c0
			 0x7846ffd6, /// 0x9c4
			 0xc4ed35d7, /// 0x9c8
			 0xff7f32d3, /// 0x9cc
			 0x51bfaadc, /// 0x9d0
			 0xda9bf1fe, /// 0x9d4
			 0x0e30942c, /// 0x9d8
			 0x5e40261d, /// 0x9dc
			 0x28e897a0, /// 0x9e0
			 0x7c973ab7, /// 0x9e4
			 0xfbfff53c, /// 0x9e8
			 0x68b86972, /// 0x9ec
			 0xd092f3dc, /// 0x9f0
			 0x45777cd4, /// 0x9f4
			 0x1d226bae, /// 0x9f8
			 0xc3caffa2, /// 0x9fc
			 0xcca9a510, /// 0xa00
			 0x7dcd59be, /// 0xa04
			 0x9eea411e, /// 0xa08
			 0x1f9705d7, /// 0xa0c
			 0xc84325cb, /// 0xa10
			 0x27bd2e16, /// 0xa14
			 0x7d0bc327, /// 0xa18
			 0xd025dfd5, /// 0xa1c
			 0x1205b71a, /// 0xa20
			 0xfac478fc, /// 0xa24
			 0x10ae04c8, /// 0xa28
			 0xe5be2bb4, /// 0xa2c
			 0x9ccdaaf7, /// 0xa30
			 0xdd819d1b, /// 0xa34
			 0x017ab204, /// 0xa38
			 0x6c6eb6fb, /// 0xa3c
			 0xe9f7a1c3, /// 0xa40
			 0xd7db2ac8, /// 0xa44
			 0xa29e1513, /// 0xa48
			 0x89098175, /// 0xa4c
			 0x4b18d686, /// 0xa50
			 0x16dab115, /// 0xa54
			 0xa9c350ac, /// 0xa58
			 0xab534171, /// 0xa5c
			 0xef9829ed, /// 0xa60
			 0xa6b8d050, /// 0xa64
			 0x3d618667, /// 0xa68
			 0xe083cc6c, /// 0xa6c
			 0xae22f5cf, /// 0xa70
			 0x8e948844, /// 0xa74
			 0x6727ef17, /// 0xa78
			 0x274204ba, /// 0xa7c
			 0x86869c45, /// 0xa80
			 0x6e0b7c18, /// 0xa84
			 0xcfb3e07b, /// 0xa88
			 0xda226a8e, /// 0xa8c
			 0x2562aaa5, /// 0xa90
			 0x627ca54a, /// 0xa94
			 0xaab3c718, /// 0xa98
			 0xc2d024a6, /// 0xa9c
			 0xca5a775b, /// 0xaa0
			 0x95c669e9, /// 0xaa4
			 0x818d1d9f, /// 0xaa8
			 0x67d08658, /// 0xaac
			 0x7f8ba7f7, /// 0xab0
			 0x38db9247, /// 0xab4
			 0x67ad1da7, /// 0xab8
			 0x2409c8a7, /// 0xabc
			 0xeb7618df, /// 0xac0
			 0xe6dc2468, /// 0xac4
			 0xa7c7abd9, /// 0xac8
			 0x270dc435, /// 0xacc
			 0x726e6b53, /// 0xad0
			 0x85aa00db, /// 0xad4
			 0x800e4f85, /// 0xad8
			 0x308482af, /// 0xadc
			 0x0d18dc08, /// 0xae0
			 0xd71af853, /// 0xae4
			 0x100e61ba, /// 0xae8
			 0xf33fbec3, /// 0xaec
			 0x4d2763fe, /// 0xaf0
			 0x75648992, /// 0xaf4
			 0xdee8287a, /// 0xaf8
			 0xa6b07a82, /// 0xafc
			 0x9f6f4704, /// 0xb00
			 0xe2a91369, /// 0xb04
			 0xe6992792, /// 0xb08
			 0x2791dd8d, /// 0xb0c
			 0xd51223d4, /// 0xb10
			 0x33d7fc0a, /// 0xb14
			 0x015f466c, /// 0xb18
			 0xe0bc53f7, /// 0xb1c
			 0xd78ef846, /// 0xb20
			 0x5fb18b76, /// 0xb24
			 0x02e3e5a1, /// 0xb28
			 0x4cdd4b8d, /// 0xb2c
			 0xa2e692ce, /// 0xb30
			 0x607b175d, /// 0xb34
			 0x0d1e174b, /// 0xb38
			 0x813c2a24, /// 0xb3c
			 0xa85312bf, /// 0xb40
			 0x269ef35c, /// 0xb44
			 0x0410ceaf, /// 0xb48
			 0x3090214a, /// 0xb4c
			 0x6f62dd99, /// 0xb50
			 0x48d67653, /// 0xb54
			 0x2997c471, /// 0xb58
			 0x43a9403f, /// 0xb5c
			 0x4bce5528, /// 0xb60
			 0x760a1809, /// 0xb64
			 0x441a67de, /// 0xb68
			 0xc25d7379, /// 0xb6c
			 0x6e69df8d, /// 0xb70
			 0xde8b1493, /// 0xb74
			 0x36d033d2, /// 0xb78
			 0xb2766cff, /// 0xb7c
			 0x42bd737f, /// 0xb80
			 0x3b4c0063, /// 0xb84
			 0xc720b746, /// 0xb88
			 0xaf611686, /// 0xb8c
			 0x5fcb0201, /// 0xb90
			 0xb1a0ca1b, /// 0xb94
			 0x93937e99, /// 0xb98
			 0x4b2df123, /// 0xb9c
			 0xc522c777, /// 0xba0
			 0xa4e4fde5, /// 0xba4
			 0x9868c664, /// 0xba8
			 0x68e735b3, /// 0xbac
			 0xc118026d, /// 0xbb0
			 0xf23d96b6, /// 0xbb4
			 0x2da5fde6, /// 0xbb8
			 0xcd6d8e97, /// 0xbbc
			 0x35d338b3, /// 0xbc0
			 0xcf47acae, /// 0xbc4
			 0x0d7a0fa9, /// 0xbc8
			 0x497281a4, /// 0xbcc
			 0x6d1758b9, /// 0xbd0
			 0x096d035e, /// 0xbd4
			 0x9301958e, /// 0xbd8
			 0x482c10f8, /// 0xbdc
			 0x9b8c7cb3, /// 0xbe0
			 0x8ff10ae7, /// 0xbe4
			 0x2daade0a, /// 0xbe8
			 0x249313e5, /// 0xbec
			 0x1161c2ab, /// 0xbf0
			 0x16008a78, /// 0xbf4
			 0x415b6d73, /// 0xbf8
			 0x05fe3ede, /// 0xbfc
			 0xd484f175, /// 0xc00
			 0x73b0a2f8, /// 0xc04
			 0xd3bb529b, /// 0xc08
			 0x5d32070f, /// 0xc0c
			 0x46ce0965, /// 0xc10
			 0x10b81547, /// 0xc14
			 0x6903a833, /// 0xc18
			 0xea9828e7, /// 0xc1c
			 0x3b93dcfc, /// 0xc20
			 0xa53fbc73, /// 0xc24
			 0x2d0feab3, /// 0xc28
			 0x4553228f, /// 0xc2c
			 0xe08079d2, /// 0xc30
			 0x02055163, /// 0xc34
			 0x97c9f8dc, /// 0xc38
			 0xa42d01a6, /// 0xc3c
			 0xbf49d2fc, /// 0xc40
			 0xbd0e7972, /// 0xc44
			 0xdc772f94, /// 0xc48
			 0x746f3219, /// 0xc4c
			 0xc23c2c90, /// 0xc50
			 0x2e4bafd8, /// 0xc54
			 0x59598bd3, /// 0xc58
			 0xecb5097d, /// 0xc5c
			 0xc95f7b60, /// 0xc60
			 0x664d494e, /// 0xc64
			 0x62b5e244, /// 0xc68
			 0x6ecd617e, /// 0xc6c
			 0x630d3abe, /// 0xc70
			 0x9045f7f3, /// 0xc74
			 0x7669d58d, /// 0xc78
			 0xf4d2cb28, /// 0xc7c
			 0x212a6feb, /// 0xc80
			 0x0fe222bf, /// 0xc84
			 0x5c4831cb, /// 0xc88
			 0xf8247921, /// 0xc8c
			 0xba97e69b, /// 0xc90
			 0xc377d3bd, /// 0xc94
			 0xcf90d15e, /// 0xc98
			 0x229fb9c9, /// 0xc9c
			 0x36761d6a, /// 0xca0
			 0x15c22f4b, /// 0xca4
			 0x8573ccef, /// 0xca8
			 0xe7808017, /// 0xcac
			 0xbb7a558a, /// 0xcb0
			 0x64dc77ae, /// 0xcb4
			 0x5db37061, /// 0xcb8
			 0x037a20a1, /// 0xcbc
			 0xf9a3da12, /// 0xcc0
			 0x11396001, /// 0xcc4
			 0x1982d256, /// 0xcc8
			 0x188bedc6, /// 0xccc
			 0xe0031ac1, /// 0xcd0
			 0xfed15c69, /// 0xcd4
			 0x1561e47e, /// 0xcd8
			 0x733f988c, /// 0xcdc
			 0x3382da10, /// 0xce0
			 0x77497671, /// 0xce4
			 0x8caf336b, /// 0xce8
			 0xf9816faf, /// 0xcec
			 0x75760176, /// 0xcf0
			 0x6f6fccdd, /// 0xcf4
			 0x2f7a6512, /// 0xcf8
			 0x079cf948, /// 0xcfc
			 0xd7914f81, /// 0xd00
			 0x6fde9a39, /// 0xd04
			 0x5a72ac29, /// 0xd08
			 0x970ea597, /// 0xd0c
			 0x2b2ab3fa, /// 0xd10
			 0xa55f30f0, /// 0xd14
			 0xa1c73b14, /// 0xd18
			 0x363112ee, /// 0xd1c
			 0x57362422, /// 0xd20
			 0xcdcc9246, /// 0xd24
			 0x2b0bee63, /// 0xd28
			 0x973f54c7, /// 0xd2c
			 0x50a381fc, /// 0xd30
			 0x8b61828d, /// 0xd34
			 0x6a9794c4, /// 0xd38
			 0xed1fc152, /// 0xd3c
			 0xb76f6dc9, /// 0xd40
			 0xc7466f9e, /// 0xd44
			 0xbca94578, /// 0xd48
			 0x3b62c523, /// 0xd4c
			 0x16945fc6, /// 0xd50
			 0xf4acbcd7, /// 0xd54
			 0x6280939c, /// 0xd58
			 0x66278cfd, /// 0xd5c
			 0xf2e72ce8, /// 0xd60
			 0xf6d27123, /// 0xd64
			 0x0f561d5d, /// 0xd68
			 0x78be544b, /// 0xd6c
			 0x4a10e66f, /// 0xd70
			 0xc3da81f4, /// 0xd74
			 0x839f7623, /// 0xd78
			 0xd6390749, /// 0xd7c
			 0x2d7c9875, /// 0xd80
			 0x02c20518, /// 0xd84
			 0x35051b4f, /// 0xd88
			 0x018984fb, /// 0xd8c
			 0x20183280, /// 0xd90
			 0x473a40a3, /// 0xd94
			 0xa7749cad, /// 0xd98
			 0x41b2c721, /// 0xd9c
			 0xb129e867, /// 0xda0
			 0xaf53b7d4, /// 0xda4
			 0xc2edace9, /// 0xda8
			 0xe7180ace, /// 0xdac
			 0x1ea05653, /// 0xdb0
			 0x74db2b57, /// 0xdb4
			 0x15179132, /// 0xdb8
			 0xdedcd68e, /// 0xdbc
			 0xec9f477b, /// 0xdc0
			 0xbded9592, /// 0xdc4
			 0xb6a23c76, /// 0xdc8
			 0x3a3539b0, /// 0xdcc
			 0x0ac54db4, /// 0xdd0
			 0xd9d2bc58, /// 0xdd4
			 0xe5f140aa, /// 0xdd8
			 0x54014b69, /// 0xddc
			 0xc99d718c, /// 0xde0
			 0xf98d16e9, /// 0xde4
			 0x59c1c24e, /// 0xde8
			 0x957576f3, /// 0xdec
			 0x67ac64d2, /// 0xdf0
			 0x8473ca94, /// 0xdf4
			 0xd1f130c0, /// 0xdf8
			 0x426dc2ed, /// 0xdfc
			 0x09c068a2, /// 0xe00
			 0x3bae5665, /// 0xe04
			 0x12500376, /// 0xe08
			 0xb10e3a74, /// 0xe0c
			 0xe27b893c, /// 0xe10
			 0x06baf83f, /// 0xe14
			 0xf8f758e6, /// 0xe18
			 0x8f3c9a17, /// 0xe1c
			 0xec1e65d8, /// 0xe20
			 0x1d201c79, /// 0xe24
			 0xb3457140, /// 0xe28
			 0x2aa3ccd4, /// 0xe2c
			 0xe13865e0, /// 0xe30
			 0x93e9db93, /// 0xe34
			 0xfd443b8c, /// 0xe38
			 0x9bc37064, /// 0xe3c
			 0x11ffe912, /// 0xe40
			 0xb022fe21, /// 0xe44
			 0x521e0e27, /// 0xe48
			 0x6149d9fc, /// 0xe4c
			 0x33de7862, /// 0xe50
			 0x807cfeeb, /// 0xe54
			 0xa5711b78, /// 0xe58
			 0x44032497, /// 0xe5c
			 0x5e457964, /// 0xe60
			 0x754692da, /// 0xe64
			 0x3ed54f39, /// 0xe68
			 0x4d3c4acf, /// 0xe6c
			 0xd427e912, /// 0xe70
			 0xebfb0c6a, /// 0xe74
			 0xe5fc5071, /// 0xe78
			 0xb5c379bb, /// 0xe7c
			 0x37109304, /// 0xe80
			 0x52e249a4, /// 0xe84
			 0x7a92d669, /// 0xe88
			 0xba27b73b, /// 0xe8c
			 0xf1cfec09, /// 0xe90
			 0xaa62da44, /// 0xe94
			 0x7316bf65, /// 0xe98
			 0xf74b48af, /// 0xe9c
			 0xc22ef89a, /// 0xea0
			 0xc0308aed, /// 0xea4
			 0xd5cbfb75, /// 0xea8
			 0x663f7540, /// 0xeac
			 0xb1687b5a, /// 0xeb0
			 0xc375371e, /// 0xeb4
			 0x77ca8b3f, /// 0xeb8
			 0x47e38f0c, /// 0xebc
			 0xf13206e3, /// 0xec0
			 0xe260c2e2, /// 0xec4
			 0x536b8f24, /// 0xec8
			 0xac9c2534, /// 0xecc
			 0x2ed6ed0c, /// 0xed0
			 0xb91e9898, /// 0xed4
			 0x00ccf94d, /// 0xed8
			 0x604f1795, /// 0xedc
			 0xc9dc1cbf, /// 0xee0
			 0x553c7218, /// 0xee4
			 0x9073a23b, /// 0xee8
			 0x389e4f43, /// 0xeec
			 0xbbb4f489, /// 0xef0
			 0x9673b11d, /// 0xef4
			 0x4b251b4c, /// 0xef8
			 0xf3a86624, /// 0xefc
			 0xa073cfa9, /// 0xf00
			 0x16aef203, /// 0xf04
			 0x4df64329, /// 0xf08
			 0xf3061343, /// 0xf0c
			 0x10932517, /// 0xf10
			 0xe62fed90, /// 0xf14
			 0xa1434600, /// 0xf18
			 0x30f506a0, /// 0xf1c
			 0xa5683278, /// 0xf20
			 0xc1d391bc, /// 0xf24
			 0x62826f73, /// 0xf28
			 0x6814e1e7, /// 0xf2c
			 0x30c15bc6, /// 0xf30
			 0x420068de, /// 0xf34
			 0x2e894a1e, /// 0xf38
			 0x2b234962, /// 0xf3c
			 0xe2ee0ad6, /// 0xf40
			 0x05825d22, /// 0xf44
			 0xff73b16d, /// 0xf48
			 0xcd803c49, /// 0xf4c
			 0x6d09d7d1, /// 0xf50
			 0x6ceabe84, /// 0xf54
			 0xe9624d98, /// 0xf58
			 0xf5cf4ad3, /// 0xf5c
			 0x992f50bd, /// 0xf60
			 0xfa09e8b5, /// 0xf64
			 0x18da4c05, /// 0xf68
			 0x46566131, /// 0xf6c
			 0x9cacb667, /// 0xf70
			 0x6d157cb3, /// 0xf74
			 0xa39d7c35, /// 0xf78
			 0x97a9923d, /// 0xf7c
			 0xf5d24624, /// 0xf80
			 0x4ab6ab30, /// 0xf84
			 0x694972b3, /// 0xf88
			 0x63714d87, /// 0xf8c
			 0x51bb5a73, /// 0xf90
			 0x6bfe7adc, /// 0xf94
			 0xbfe6a4ae, /// 0xf98
			 0xcbbeb452, /// 0xf9c
			 0xe9a47394, /// 0xfa0
			 0xeacb165e, /// 0xfa4
			 0x7047af52, /// 0xfa8
			 0x56516205, /// 0xfac
			 0x5df324e5, /// 0xfb0
			 0xed46a423, /// 0xfb4
			 0x3736ce30, /// 0xfb8
			 0xfaf32639, /// 0xfbc
			 0x1e354f1c, /// 0xfc0
			 0xd5961a8e, /// 0xfc4
			 0x11819338, /// 0xfc8
			 0x90903942, /// 0xfcc
			 0x65196cf2, /// 0xfd0
			 0x466be381, /// 0xfd4
			 0x54007bd7, /// 0xfd8
			 0xbcee29d3, /// 0xfdc
			 0x466850bc, /// 0xfe0
			 0xe2a8a9eb, /// 0xfe4
			 0xe8b81cc3, /// 0xfe8
			 0xbef808c8, /// 0xfec
			 0x49918637, /// 0xff0
			 0x00edea89, /// 0xff4
			 0x75ce0b2a, /// 0xff8
			 0xfa62450c /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00000
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00004
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0xffc00001,                                                  // -signaling NaN                              /// 0000c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00010
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00014
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00018
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0001c
			 0xffc00001,                                                  // -signaling NaN                              /// 00020
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00028
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00030
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00038
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0003c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00040
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00044
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00048
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00050
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00054
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00064
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00068
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00074
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00078
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0007c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00080
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x7f800000,                                                  // inf                                         /// 00088
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0008c
			 0x3f028f5c,                                                  // 0.51                                        /// 00090
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0009c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000a0
			 0xffc00001,                                                  // -signaling NaN                              /// 000a4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000b4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000d0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 000f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000fc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00100
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00104
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0xbf028f5c,                                                  // -0.51                                       /// 00118
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0011c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00120
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00124
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00128
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0012c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00134
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0013c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00140
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00148
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0014c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00150
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00154
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0015c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00160
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00164
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0016c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00170
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00174
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00178
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0017c
			 0xff800000,                                                  // -inf                                        /// 00180
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00188
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0018c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00190
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00194
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00198
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x4b000000,                                                  // 8388608.0                                   /// 001a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001b0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001cc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 001fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00200
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00204
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00208
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0020c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x7f800000,                                                  // inf                                         /// 00214
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00228
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00234
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00238
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0023c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00240
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00244
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00254
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00258
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0025c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00260
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00264
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00268
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0026c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00270
			 0x0e000007,                                                  // Trailing 1s:                                /// 00274
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00278
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0027c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00280
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00284
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00288
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00290
			 0x80011111,                                                  // -9.7958E-41                                 /// 00294
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00298
			 0xbf028f5c,                                                  // -0.51                                       /// 0029c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x7fc00001,                                                  // signaling NaN                               /// 002a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002b4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002b8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x33333333,                                                  // 4 random values                             /// 002c0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002d8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002e4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002f0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00300
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0xffc00001,                                                  // -signaling NaN                              /// 00308
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0030c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00310
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0031c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00320
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0032c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00330
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00334
			 0x0e000007,                                                  // Trailing 1s:                                /// 00338
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0033c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00344
			 0x0c600000,                                                  // Leading 1s:                                 /// 00348
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0034c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00350
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00358
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0035c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00360
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00364
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x0c600000,                                                  // Leading 1s:                                 /// 0036c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00370
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00374
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00378
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00384
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00388
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0038c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00390
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00394
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00398
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0039c
			 0xbf028f5c,                                                  // -0.51                                       /// 003a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 003ac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 003bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003c8
			 0xcb000000,                                                  // -8388608.0                                  /// 003cc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003f8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00400
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00404
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0040c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00410
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00418
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0041c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00424
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00428
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0042c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00430
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x7fc00001,                                                  // signaling NaN                               /// 0043c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00440
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00448
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0044c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00454
			 0xcb000000,                                                  // -8388608.0                                  /// 00458
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0045c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00460
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00464
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x00011111,                                                  // 9.7958E-41                                  /// 0046c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00474
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00478
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0047c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00484
			 0x33333333,                                                  // 4 random values                             /// 00488
			 0x0c600000,                                                  // Leading 1s:                                 /// 0048c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00490
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00494
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00498
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0049c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004a8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 004b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004e0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00504
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00508
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x7f800000,                                                  // inf                                         /// 00510
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0051c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00520
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00524
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00528
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0052c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00530
			 0x0e000003,                                                  // Trailing 1s:                                /// 00534
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0053c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00540
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00544
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00548
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0054c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00550
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00560
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00564
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x3f028f5c,                                                  // 0.51                                        /// 0056c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00578
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0057c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00580
			 0x3f028f5c,                                                  // 0.51                                        /// 00584
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00588
			 0x0c400000,                                                  // Leading 1s:                                 /// 0058c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00594
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00598
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0059c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005a8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005b0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005b4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005bc
			 0x7f800000,                                                  // inf                                         /// 005c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005d0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005e0
			 0x7fc00001,                                                  // signaling NaN                               /// 005e4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005f8
			 0x00000000,                                                  // zero                                        /// 005fc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00600
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00604
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00608
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0060c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00610
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00614
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0061c
			 0xcb000000,                                                  // -8388608.0                                  /// 00620
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0062c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00630
			 0x80000000,                                                  // -zero                                       /// 00634
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0063c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00640
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00644
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00648
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0064c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00650
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00654
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0065c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00660
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00664
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00668
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0066c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00670
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00674
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00680
			 0x4b000000,                                                  // 8388608.0                                   /// 00684
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00688
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00690
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00698
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006a8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006b8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006d0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006ec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00700
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00704
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00708
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00718
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00728
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00730
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00734
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0073c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00740
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0074c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00750
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00754
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0075c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00760
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00764
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00768
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00774
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00778
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0077c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00780
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00784
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00788
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0078c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00790
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00798
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0079c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007c0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007d4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007e4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x0c780000,                                                  // Leading 1s:                                 /// 007ec
			 0x00000000,                                                  // zero                                        /// 007f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00804
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00808
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00810
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00818
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0081c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0082c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00834
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0083c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00840
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00844
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00848
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0084c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00854
			 0x80011111,                                                  // -9.7958E-41                                 /// 00858
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0085c
			 0xcb000000,                                                  // -8388608.0                                  /// 00860
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00864
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00868
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0086c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00870
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00874
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00878
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0087c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00880
			 0x00000000,                                                  // zero                                        /// 00884
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00888
			 0x0c700000,                                                  // Leading 1s:                                 /// 0088c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00890
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00894
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0089c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008a0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008c0
			 0x7f800000,                                                  // inf                                         /// 008c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008cc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008ec
			 0xff800000,                                                  // -inf                                        /// 008f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00900
			 0x0c780000,                                                  // Leading 1s:                                 /// 00904
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00908
			 0xcb000000,                                                  // -8388608.0                                  /// 0090c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00914
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00918
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0091c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00920
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00928
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0092c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00930
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00934
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0093c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00940
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00944
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0094c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00954
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00960
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00964
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00968
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0096c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00970
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00974
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00980
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00988
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00990
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00998
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009b0
			 0xffc00001,                                                  // -signaling NaN                              /// 009b4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 009d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009d8
			 0xff800000,                                                  // -inf                                        /// 009dc
			 0x33333333,                                                  // 4 random values                             /// 009e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a00
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a04
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a08
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a0c
			 0xcb000000,                                                  // -8388608.0                                  /// 00a10
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a14
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a1c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a20
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a24
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a30
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a34
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a38
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a3c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a40
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a48
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a4c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a54
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a58
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a68
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x7fc00001,                                                  // signaling NaN                               /// 00a70
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a74
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a7c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a80
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a84
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a88
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a8c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a90
			 0x3f028f5c,                                                  // 0.51                                        /// 00a94
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a9c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00aa0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aa8
			 0x7fc00001,                                                  // signaling NaN                               /// 00aac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ab4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ab8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00abc
			 0xcb000000,                                                  // -8388608.0                                  /// 00ac0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ac4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ac8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ad0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ad4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ad8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00adc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ae0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ae4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00af0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00af4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00af8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00afc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b00
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b04
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b08
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b0c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b10
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b18
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b20
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b24
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b28
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b2c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b30
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b34
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x55555555,                                                  // 4 random values                             /// 00b3c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b44
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b48
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b4c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b50
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b58
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b60
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b64
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b70
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b74
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b78
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b80
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b84
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b88
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b8c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b98
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b9c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ba0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ba4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ba8
			 0xff800000,                                                  // -inf                                        /// 00bac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bb0
			 0x7fc00001,                                                  // signaling NaN                               /// 00bb4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bb8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bbc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bc0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bc4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bc8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bcc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bd0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bd4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00be0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00be4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00be8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bec
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bf0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x7fc00001,                                                  // signaling NaN                               /// 00bf8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c00
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c04
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c0c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c18
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c24
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c28
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c2c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c30
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c34
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c38
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c3c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c40
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c44
			 0x00000000,                                                  // zero                                        /// 00c48
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x00000000,                                                  // zero                                        /// 00c54
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c5c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c68
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c6c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c70
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c74
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c7c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c80
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c88
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c94
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c98
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ca4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ca8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cb8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cbc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cc4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cc8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ccc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cd0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cd4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cd8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cdc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ce0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ce4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ce8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cf4
			 0x7f800000,                                                  // inf                                         /// 00cf8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cfc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d00
			 0x7f800000,                                                  // inf                                         /// 00d04
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d08
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d10
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d20
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d24
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d28
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d2c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d30
			 0x55555555,                                                  // 4 random values                             /// 00d34
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d38
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d48
			 0xbf028f5c,                                                  // -0.51                                       /// 00d4c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d50
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d54
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d5c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d60
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d64
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d68
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d6c
			 0x7f800000,                                                  // inf                                         /// 00d70
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d74
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d78
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d7c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x00000000,                                                  // zero                                        /// 00d84
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d88
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d8c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d98
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d9c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00da0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00da4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00da8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x3f028f5c,                                                  // 0.51                                        /// 00db4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00db8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00dc4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00dcc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dd0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00dd4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00dd8
			 0x4b000000,                                                  // 8388608.0                                   /// 00ddc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00de0
			 0x3f028f5c,                                                  // 0.51                                        /// 00de4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00de8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00df0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e00
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e08
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e0c
			 0x80000000,                                                  // -zero                                       /// 00e10
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e14
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e1c
			 0x3f028f5c,                                                  // 0.51                                        /// 00e20
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e28
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e2c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e30
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e3c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e40
			 0x3f028f5c,                                                  // 0.51                                        /// 00e44
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e50
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e54
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e58
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e5c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e64
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e68
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e6c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e74
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e78
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e7c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e80
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e84
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e88
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e8c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e90
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e94
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e98
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e9c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ea0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ea4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ea8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00eac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00eb8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ebc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ec4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ec8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ecc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ed0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ed4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ed8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00edc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ee0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ee4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ee8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00eec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ef4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ef8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f00
			 0x4b000000,                                                  // 8388608.0                                   /// 00f04
			 0x80000000,                                                  // -zero                                       /// 00f08
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f0c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f10
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f1c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f20
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f24
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f30
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f38
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0xff800000,                                                  // -inf                                        /// 00f44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f4c
			 0x33333333,                                                  // 4 random values                             /// 00f50
			 0x3f028f5c,                                                  // 0.51                                        /// 00f54
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f58
			 0x80000000,                                                  // -zero                                       /// 00f5c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f60
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f64
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f68
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f6c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f70
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f74
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f7c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f88
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f8c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f90
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f94
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f9c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fa8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fac
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fb0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fb4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fb8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fbc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fc0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fc4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fcc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fd0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fd4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fd8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fdc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fe0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fe4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fe8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fec
			 0x80000000,                                                  // -zero                                       /// 00ff0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ff4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x00800003                                                  // none of the mantissa set to +3ulp           /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00000
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00004
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00008
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0000c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00010
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00014
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00020
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00024
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00028
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0002c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00030
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00034
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00038
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0003c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00040
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00044
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00048
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00050
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0xff800000,                                                  // -inf                                        /// 0005c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00068
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0006c
			 0x55555555,                                                  // 4 random values                             /// 00070
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00074
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00078
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00080
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00088
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00090
			 0x80011111,                                                  // -9.7958E-41                                 /// 00094
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00098
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0009c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000a0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000c0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000e0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000f4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000fc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00100
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0010c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00110
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00114
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00118
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00120
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00128
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0012c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00130
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00134
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0013c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00144
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0014c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00150
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00154
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00158
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00160
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00164
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00168
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0016c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00174
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00178
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0017c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00180
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00184
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00188
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00190
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x0c600000,                                                  // Leading 1s:                                 /// 0019c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001ac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x3f028f5c,                                                  // 0.51                                        /// 001c0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001c4
			 0x0e000003,                                                  // Trailing 1s:                                /// 001c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001d4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 001dc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001e4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0xff800000,                                                  // -inf                                        /// 001f0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001f8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00208
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00210
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00218
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0021c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00224
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0022c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0023c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00240
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00244
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00248
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00250
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00254
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00258
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0025c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x0c600000,                                                  // Leading 1s:                                 /// 00268
			 0x7fc00001,                                                  // signaling NaN                               /// 0026c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00270
			 0x0e000003,                                                  // Trailing 1s:                                /// 00274
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0027c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00280
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00288
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00290
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00294
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00298
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0029c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002c4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 002cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002d4
			 0xffc00001,                                                  // -signaling NaN                              /// 002d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002e0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00300
			 0x55555555,                                                  // 4 random values                             /// 00304
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0030c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00310
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00314
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x00011111,                                                  // 9.7958E-41                                  /// 00324
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00328
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0032c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00330
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00334
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00338
			 0x0e000001,                                                  // Trailing 1s:                                /// 0033c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00344
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00348
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0034c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00350
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0035c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00360
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00364
			 0x0e000007,                                                  // Trailing 1s:                                /// 00368
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0036c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00370
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00378
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00384
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00388
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0038c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00390
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0039c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003b0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003cc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003d0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x00011111,                                                  // 9.7958E-41                                  /// 003f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00414
			 0x55555555,                                                  // 4 random values                             /// 00418
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0041c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00420
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00424
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00428
			 0xcb000000,                                                  // -8388608.0                                  /// 0042c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00434
			 0xff800000,                                                  // -inf                                        /// 00438
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0043c
			 0x7fc00001,                                                  // signaling NaN                               /// 00440
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00444
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00448
			 0x0c700000,                                                  // Leading 1s:                                 /// 0044c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00450
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00454
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00458
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0045c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00460
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00468
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0046c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00474
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00478
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0047c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00480
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00488
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0048c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00490
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00498
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0049c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004a8
			 0x3f028f5c,                                                  // 0.51                                        /// 004ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004b4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004b8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 004d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x3f028f5c,                                                  // 0.51                                        /// 004f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x0c700000,                                                  // Leading 1s:                                 /// 004f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00500
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00508
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0050c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00510
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00514
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00518
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0051c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00520
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00524
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00528
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0052c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00530
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00534
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00538
			 0x0e000003,                                                  // Trailing 1s:                                /// 0053c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00540
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00544
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0054c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00550
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00554
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00560
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x0e000003,                                                  // Trailing 1s:                                /// 00568
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0056c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00574
			 0x4b000000,                                                  // 8388608.0                                   /// 00578
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0057c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00580
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00584
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00588
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00594
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00598
			 0x55555555,                                                  // 4 random values                             /// 0059c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005a0
			 0x55555555,                                                  // 4 random values                             /// 005a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 005c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 005c8
			 0xbf028f5c,                                                  // -0.51                                       /// 005cc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005d8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 005e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005f4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005f8
			 0xff800000,                                                  // -inf                                        /// 005fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00600
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00604
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00608
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00610
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00614
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00618
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x33333333,                                                  // 4 random values                             /// 00620
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00624
			 0x7f800000,                                                  // inf                                         /// 00628
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00634
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00640
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00644
			 0x0c600000,                                                  // Leading 1s:                                 /// 00648
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0064c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00650
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00654
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00660
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00670
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00674
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00678
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0067c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00680
			 0x80011111,                                                  // -9.7958E-41                                 /// 00684
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00688
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x3f028f5c,                                                  // 0.51                                        /// 00690
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00694
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0069c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006c0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006c4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006c8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006d4
			 0x00011111,                                                  // 9.7958E-41                                  /// 006d8
			 0xbf028f5c,                                                  // -0.51                                       /// 006dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00700
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00704
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00708
			 0x0c780000,                                                  // Leading 1s:                                 /// 0070c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00710
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00714
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00724
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00728
			 0x33333333,                                                  // 4 random values                             /// 0072c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00730
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00734
			 0x3f028f5c,                                                  // 0.51                                        /// 00738
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00740
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00748
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0074c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00750
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0075c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00760
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00764
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0076c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0077c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00780
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00788
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0078c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00794
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00798
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0079c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007b8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007c8
			 0xcb000000,                                                  // -8388608.0                                  /// 007cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007d0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007e4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00800
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00804
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00808
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0080c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00810
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00814
			 0x55555555,                                                  // 4 random values                             /// 00818
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00820
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x0c600000,                                                  // Leading 1s:                                 /// 0082c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00830
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00834
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00838
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0083c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00840
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00844
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00848
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0084c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00850
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00854
			 0x00011111,                                                  // 9.7958E-41                                  /// 00858
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00860
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00868
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0086c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00870
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00878
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00884
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0088c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00890
			 0xbf028f5c,                                                  // -0.51                                       /// 00894
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0089c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008a4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 008b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008d0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00900
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00904
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00908
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0090c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00910
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00914
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00920
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00924
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00928
			 0x55555555,                                                  // 4 random values                             /// 0092c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00930
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00934
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00940
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00944
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00948
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0094c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00958
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00960
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00964
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00968
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0096c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00970
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00974
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00978
			 0x7fc00001,                                                  // signaling NaN                               /// 0097c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00980
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00984
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00988
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00994
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00998
			 0x0c600000,                                                  // Leading 1s:                                 /// 0099c
			 0x3f028f5c,                                                  // 0.51                                        /// 009a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009b8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009c8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 009f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a00
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a04
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a08
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a10
			 0x80000000,                                                  // -zero                                       /// 00a14
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a18
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a1c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a20
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a24
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a2c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a30
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a34
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a38
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a4c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a54
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a58
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a5c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x55555555,                                                  // 4 random values                             /// 00a64
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a68
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a6c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a70
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a74
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a80
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a84
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a9c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00aa8
			 0x3f028f5c,                                                  // 0.51                                        /// 00aac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ab4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ab8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00abc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ac4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ac8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ad0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ad4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00adc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ae0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ae4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ae8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00aec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00af0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00af4
			 0xbf028f5c,                                                  // -0.51                                       /// 00af8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00afc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b04
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b08
			 0xbf028f5c,                                                  // -0.51                                       /// 00b0c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b14
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b1c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b20
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b24
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b28
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b2c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b30
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b34
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b40
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b44
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b48
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b4c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b50
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b58
			 0x4b000000,                                                  // 8388608.0                                   /// 00b5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b60
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b64
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b68
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b6c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b70
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b74
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b78
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b80
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b84
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b8c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b90
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b94
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b98
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ba0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ba4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ba8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bb0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bb4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bb8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bc0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bc4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bc8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bcc
			 0x4b000000,                                                  // 8388608.0                                   /// 00bd0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bd4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bd8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bdc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0xffc00001,                                                  // -signaling NaN                              /// 00be8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bf0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bf4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c00
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c0c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c14
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c18
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c24
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c2c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c30
			 0x55555555,                                                  // 4 random values                             /// 00c34
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c38
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c44
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c48
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c4c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c50
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c54
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c58
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0xff800000,                                                  // -inf                                        /// 00c64
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c68
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c70
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c78
			 0x4b000000,                                                  // 8388608.0                                   /// 00c7c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c80
			 0x3f028f5c,                                                  // 0.51                                        /// 00c84
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c88
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c8c
			 0x00000000,                                                  // zero                                        /// 00c90
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c94
			 0x33333333,                                                  // 4 random values                             /// 00c98
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ca4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ca8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cb0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cb4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cbc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cc0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cc4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cc8
			 0x80000000,                                                  // -zero                                       /// 00ccc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cd0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cd4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cdc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ce0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ce4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ce8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cf0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d04
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d08
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d0c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d14
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x00000000,                                                  // zero                                        /// 00d1c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d20
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d24
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0xffc00001,                                                  // -signaling NaN                              /// 00d2c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d34
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d38
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d3c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x00000000,                                                  // zero                                        /// 00d44
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d48
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d4c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d54
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d58
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d5c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d60
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d64
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d68
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d6c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d70
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d74
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d78
			 0x55555555,                                                  // 4 random values                             /// 00d7c
			 0x33333333,                                                  // 4 random values                             /// 00d80
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d84
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d88
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00da0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00da4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x33333333,                                                  // 4 random values                             /// 00db0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00db4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00db8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00dc0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dc4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dcc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dd0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dd4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00dd8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ddc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00de0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x7f800000,                                                  // inf                                         /// 00de8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00df0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00df4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00df8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e04
			 0x00000000,                                                  // zero                                        /// 00e08
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e0c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e10
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e14
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e20
			 0x3f028f5c,                                                  // 0.51                                        /// 00e24
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e28
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e2c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e30
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e34
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e3c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e44
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e48
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e4c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e50
			 0xbf028f5c,                                                  // -0.51                                       /// 00e54
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e58
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e5c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e60
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e64
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e68
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e6c
			 0xff800000,                                                  // -inf                                        /// 00e70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e74
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e88
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e8c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e90
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e94
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ea0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ea4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ea8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00eb0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00eb4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00eb8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ebc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ec0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ec4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ecc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ed0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ed8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00edc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ef0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ef4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ef8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00efc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f00
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f04
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f0c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f14
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f1c
			 0x55555555,                                                  // 4 random values                             /// 00f20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f24
			 0xbf028f5c,                                                  // -0.51                                       /// 00f28
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f2c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f30
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f34
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f38
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f3c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f44
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f48
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f4c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f50
			 0x33333333,                                                  // 4 random values                             /// 00f54
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f58
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f6c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f70
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f74
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f7c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f84
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f88
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x7f800000,                                                  // inf                                         /// 00f98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fa4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fa8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fb0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fb8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fbc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fc0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fc8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fd4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fdc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fe0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fe4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fe8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x4b000000,                                                  // 8388608.0                                   /// 00ff0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x80002000                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00000
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00004
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00008
			 0x80011111,                                                  // -9.7958E-41                                 /// 0000c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00010
			 0x80011111,                                                  // -9.7958E-41                                 /// 00014
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00018
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0001c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00020
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00024
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00028
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0002c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00030
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00034
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0003c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00040
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00044
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00048
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0004c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00050
			 0x0c400000,                                                  // Leading 1s:                                 /// 00054
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0005c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00060
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00064
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00068
			 0x0e000007,                                                  // Trailing 1s:                                /// 0006c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00070
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00074
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00078
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0007c
			 0x7fc00001,                                                  // signaling NaN                               /// 00080
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00084
			 0xffc00001,                                                  // -signaling NaN                              /// 00088
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0008c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00090
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00098
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0009c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000a4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000b0
			 0x0c780000,                                                  // Leading 1s:                                 /// 000b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000c0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000d0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000d4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000f4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00100
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00104
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00108
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0010c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00110
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00114
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00120
			 0x33333333,                                                  // 4 random values                             /// 00124
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00128
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00130
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00134
			 0x0e000007,                                                  // Trailing 1s:                                /// 00138
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0013c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00140
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00144
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00150
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00154
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00158
			 0x0c600000,                                                  // Leading 1s:                                 /// 0015c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00164
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00168
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0016c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0017c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00184
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00188
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0018c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00190
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00194
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00198
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0019c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001a8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001ac
			 0x33333333,                                                  // 4 random values                             /// 001b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001d4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001f8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00200
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00204
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00208
			 0xcb000000,                                                  // -8388608.0                                  /// 0020c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00210
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00214
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00218
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0021c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00220
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00224
			 0x7f800000,                                                  // inf                                         /// 00228
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x80000000,                                                  // -zero                                       /// 00230
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00234
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00238
			 0x0c700000,                                                  // Leading 1s:                                 /// 0023c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00240
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0024c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00258
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00260
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00264
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00268
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0026c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00274
			 0x0e000001,                                                  // Trailing 1s:                                /// 00278
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00280
			 0x80000000,                                                  // -zero                                       /// 00284
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00288
			 0x0e000001,                                                  // Trailing 1s:                                /// 0028c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00298
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0xff800000,                                                  // -inf                                        /// 002a0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002a4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002c0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002c8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002cc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002dc
			 0xff800000,                                                  // -inf                                        /// 002e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002e4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002f0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002fc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00300
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00304
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00308
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0030c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00310
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00318
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0031c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00328
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0032c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00330
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00334
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00338
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0033c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00340
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00354
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00358
			 0x33333333,                                                  // 4 random values                             /// 0035c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00368
			 0x00011111,                                                  // 9.7958E-41                                  /// 0036c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00370
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00374
			 0x7fc00001,                                                  // signaling NaN                               /// 00378
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0037c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00380
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00388
			 0x0c600000,                                                  // Leading 1s:                                 /// 0038c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00390
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00394
			 0x0e000001,                                                  // Trailing 1s:                                /// 00398
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0039c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003bc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0xcb000000,                                                  // -8388608.0                                  /// 003d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00400
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00404
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00408
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0040c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00414
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0041c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00420
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00424
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00428
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00430
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0043c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00440
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00444
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00448
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0044c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00454
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0045c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00460
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00464
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0046c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00470
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00478
			 0x0e000001,                                                  // Trailing 1s:                                /// 0047c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00480
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00484
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00488
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00490
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00494
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00498
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004ac
			 0xff800000,                                                  // -inf                                        /// 004b0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 004dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004e0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004e4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004e8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004f0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004f8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004fc
			 0x4b000000,                                                  // 8388608.0                                   /// 00500
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00504
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00508
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0050c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00510
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00514
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0051c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00520
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00524
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00534
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00538
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0053c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00548
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0054c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00550
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00554
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00558
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0055c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00564
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0056c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00570
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00580
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00584
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0058c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00594
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0059c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005a0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005a8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005ac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005d4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005e0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0xbf028f5c,                                                  // -0.51                                       /// 005e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005ec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005f0
			 0x55555555,                                                  // 4 random values                             /// 005f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005fc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00600
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00604
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0060c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00618
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0061c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00620
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00624
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00628
			 0x0c400000,                                                  // Leading 1s:                                 /// 0062c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0063c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00640
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00648
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0064c
			 0xffc00001,                                                  // -signaling NaN                              /// 00650
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00654
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00658
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0065c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00660
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00668
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0066c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00670
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0067c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00688
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0068c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00690
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00698
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0069c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006a8
			 0xcb000000,                                                  // -8388608.0                                  /// 006ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006b0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006b8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006c4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 006cc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006d4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x80011111,                                                  // -9.7958E-41                                 /// 006e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00700
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0e000001,                                                  // Trailing 1s:                                /// 00708
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00710
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00718
			 0x0c780000,                                                  // Leading 1s:                                 /// 0071c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00720
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00728
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0072c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00730
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00734
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x7fc00001,                                                  // signaling NaN                               /// 0073c
			 0x33333333,                                                  // 4 random values                             /// 00740
			 0x80000000,                                                  // -zero                                       /// 00744
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0xcb000000,                                                  // -8388608.0                                  /// 0074c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00750
			 0xff800000,                                                  // -inf                                        /// 00754
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00758
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0075c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00760
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00764
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00768
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0076c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00778
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0077c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00780
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0078c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00794
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0079c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007a8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007c0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007c4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 007d8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007e4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 007f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007f8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00800
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00804
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0080c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00810
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00814
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0081c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00820
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0082c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00830
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00838
			 0x80011111,                                                  // -9.7958E-41                                 /// 0083c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00840
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00848
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0084c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00850
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00854
			 0xff800000,                                                  // -inf                                        /// 00858
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00864
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00868
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0086c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00870
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00878
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0087c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00884
			 0x7f800000,                                                  // inf                                         /// 00888
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00894
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00898
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008a8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 008b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008c4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008f8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00900
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00904
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00908
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0090c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00910
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00914
			 0x33333333,                                                  // 4 random values                             /// 00918
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x0e000007,                                                  // Trailing 1s:                                /// 00924
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00928
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0092c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x4b000000,                                                  // 8388608.0                                   /// 00940
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00944
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00948
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0094c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00950
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00958
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00960
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00968
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00970
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00974
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00978
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0097c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00980
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00988
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0098c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00990
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00994
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0099c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009a4
			 0xbf028f5c,                                                  // -0.51                                       /// 009a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009b0
			 0x7fc00001,                                                  // signaling NaN                               /// 009b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009b8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009c0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009c4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009cc
			 0x0e000003,                                                  // Trailing 1s:                                /// 009d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a00
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a04
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a08
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a0c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a10
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a14
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a18
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a1c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a20
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a24
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a28
			 0xcb000000,                                                  // -8388608.0                                  /// 00a2c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a34
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a38
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a3c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a40
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a44
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a4c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a50
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a54
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a5c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a60
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a64
			 0x55555555,                                                  // 4 random values                             /// 00a68
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a6c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a70
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a74
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a80
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a84
			 0xff800000,                                                  // -inf                                        /// 00a88
			 0xffc00001,                                                  // -signaling NaN                              /// 00a8c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a90
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a94
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a98
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a9c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00aa0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00aa4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00aac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ab4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00abc
			 0x3f028f5c,                                                  // 0.51                                        /// 00ac0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00acc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x7f800000,                                                  // inf                                         /// 00ad4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ae4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00aec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00af8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00afc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b04
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b0c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b10
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b1c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b20
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b24
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b28
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b2c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b34
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b38
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b3c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b40
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b44
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b48
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b4c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b54
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b58
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b5c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b64
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b68
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b6c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b70
			 0xff800000,                                                  // -inf                                        /// 00b74
			 0x3f028f5c,                                                  // 0.51                                        /// 00b78
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b7c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b84
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b88
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b8c
			 0x7fc00001,                                                  // signaling NaN                               /// 00b90
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b98
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x3f028f5c,                                                  // 0.51                                        /// 00ba0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bb0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bb8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bbc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bc0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bc4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bc8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bcc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bd0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bd4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bd8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00be4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00be8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bec
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bf0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bfc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c10
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c18
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c1c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c20
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c28
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c38
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c3c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c40
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c44
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c50
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c54
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c58
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c5c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c60
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c64
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c68
			 0xbf028f5c,                                                  // -0.51                                       /// 00c6c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c70
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c78
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c7c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c80
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c84
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c88
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c94
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c9c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ca0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ca4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ca8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0xbf028f5c,                                                  // -0.51                                       /// 00cb4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cb8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cbc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cc8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cd0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cd4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cd8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cdc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ce0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ce4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ce8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cf4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cf8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cfc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d04
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d10
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x3f028f5c,                                                  // 0.51                                        /// 00d1c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d30
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d34
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d3c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d40
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d48
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d50
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x80000000,                                                  // -zero                                       /// 00d58
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d60
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d64
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d6c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d78
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d80
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d84
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d88
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d8c
			 0x80000000,                                                  // -zero                                       /// 00d90
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d9c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00da0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x7fc00001,                                                  // signaling NaN                               /// 00dac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00db4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00db8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dbc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dc0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dc4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dc8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dcc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dd0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dd8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00de4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00de8
			 0xbf028f5c,                                                  // -0.51                                       /// 00dec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00df0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x7fc00001,                                                  // signaling NaN                               /// 00df8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e00
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e04
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e0c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e14
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e18
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x00000000,                                                  // zero                                        /// 00e28
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e2c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e30
			 0x7f800000,                                                  // inf                                         /// 00e34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e38
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e3c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e40
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e44
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e50
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e54
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e58
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e60
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e64
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e6c
			 0x3f028f5c,                                                  // 0.51                                        /// 00e70
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e74
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e7c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e80
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e84
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e8c
			 0x33333333,                                                  // 4 random values                             /// 00e90
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e94
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e98
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ea0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ea8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00eac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00eb4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ec0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ec8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ecc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ed0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ed4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ed8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00edc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ee0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ee8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ef0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ef8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00efc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f00
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f04
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f0c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f10
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f18
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f1c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f20
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f24
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0xff800000,                                                  // -inf                                        /// 00f2c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f30
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f34
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f38
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f40
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f50
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f5c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f64
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f68
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f6c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f70
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f74
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f78
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f88
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f94
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f98
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f9c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fa4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0xbf028f5c,                                                  // -0.51                                       /// 00fac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fb4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fb8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fc0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fc8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fcc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fd0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fd4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fd8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0xcb000000,                                                  // -8388608.0                                  /// 00fe4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fe8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ff0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ff8
			 0x00000000                                                  // zero                                        /// last
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
			 0x0000034c,
			 0x00000350,
			 0x00000328,
			 0x00000748,
			 0x00000180,
			 0x00000224,
			 0x00000754,
			 0x00000500,

			 /// vpu register f2
			 0x41500000,
			 0x40800000,
			 0x41000000,
			 0x41f80000,
			 0x40800000,
			 0x41400000,
			 0x40a00000,
			 0x41f00000,

			 /// vpu register f3
			 0x41d80000,
			 0x41000000,
			 0x41900000,
			 0x40400000,
			 0x40a00000,
			 0x41a80000,
			 0x40800000,
			 0x41200000,

			 /// vpu register f4
			 0x41e80000,
			 0x41500000,
			 0x42000000,
			 0x41d00000,
			 0x40800000,
			 0x40400000,
			 0x40e00000,
			 0x41600000,

			 /// vpu register f5
			 0x40a00000,
			 0x41e00000,
			 0x41880000,
			 0x41800000,
			 0x41e00000,
			 0x42000000,
			 0x41b00000,
			 0x41200000,

			 /// vpu register f6
			 0x41400000,
			 0x41200000,
			 0x41900000,
			 0x41980000,
			 0x41e00000,
			 0x40000000,
			 0x41a00000,
			 0x41700000,

			 /// vpu register f7
			 0x40e00000,
			 0x40800000,
			 0x41e00000,
			 0x40a00000,
			 0x41900000,
			 0x41b00000,
			 0x40800000,
			 0x41200000,

			 /// vpu register f8
			 0x40a00000,
			 0x41200000,
			 0x41100000,
			 0x40000000,
			 0x40e00000,
			 0x41b80000,
			 0x41e80000,
			 0x41800000,

			 /// vpu register f9
			 0x40000000,
			 0x41b00000,
			 0x41100000,
			 0x41600000,
			 0x41900000,
			 0x40e00000,
			 0x40e00000,
			 0x41700000,

			 /// vpu register f10
			 0x41000000,
			 0x41100000,
			 0x41000000,
			 0x41e80000,
			 0x41700000,
			 0x41b80000,
			 0x41e80000,
			 0x41700000,

			 /// vpu register f11
			 0x41100000,
			 0x40a00000,
			 0x41300000,
			 0x41f00000,
			 0x41c00000,
			 0x41e80000,
			 0x40e00000,
			 0x41f00000,

			 /// vpu register f12
			 0x41c00000,
			 0x41200000,
			 0x40c00000,
			 0x41a80000,
			 0x41a00000,
			 0x41100000,
			 0x41d00000,
			 0x41700000,

			 /// vpu register f13
			 0x41400000,
			 0x41200000,
			 0x41400000,
			 0x41100000,
			 0x3f800000,
			 0x40e00000,
			 0x41400000,
			 0x41980000,

			 /// vpu register f14
			 0x40400000,
			 0x41c80000,
			 0x40800000,
			 0x41c80000,
			 0x41e80000,
			 0x41f80000,
			 0x41300000,
			 0x41400000,

			 /// vpu register f15
			 0x41900000,
			 0x41880000,
			 0x40800000,
			 0x41900000,
			 0x40c00000,
			 0x41600000,
			 0x41e80000,
			 0x40400000,

			 /// vpu register f16
			 0x41500000,
			 0x41880000,
			 0x40000000,
			 0x40a00000,
			 0x41300000,
			 0x41880000,
			 0x42000000,
			 0x41200000,

			 /// vpu register f17
			 0x41100000,
			 0x41980000,
			 0x41100000,
			 0x41f00000,
			 0x41b80000,
			 0x41d80000,
			 0x41f00000,
			 0x41a00000,

			 /// vpu register f18
			 0x41a80000,
			 0x41b80000,
			 0x41c80000,
			 0x40000000,
			 0x41880000,
			 0x40c00000,
			 0x41c00000,
			 0x41200000,

			 /// vpu register f19
			 0x41a80000,
			 0x41880000,
			 0x40400000,
			 0x42000000,
			 0x41f80000,
			 0x41c00000,
			 0x41c80000,
			 0x41f00000,

			 /// vpu register f20
			 0x41f00000,
			 0x41b00000,
			 0x40400000,
			 0x40c00000,
			 0x40c00000,
			 0x41f00000,
			 0x41200000,
			 0x41f80000,

			 /// vpu register f21
			 0x41300000,
			 0x3f800000,
			 0x3f800000,
			 0x3f800000,
			 0x40800000,
			 0x41600000,
			 0x41000000,
			 0x41880000,

			 /// vpu register f22
			 0x41500000,
			 0x41000000,
			 0x41900000,
			 0x41200000,
			 0x40400000,
			 0x41200000,
			 0x40800000,
			 0x41300000,

			 /// vpu register f23
			 0x41c00000,
			 0x40400000,
			 0x41200000,
			 0x40c00000,
			 0x41d80000,
			 0x41b80000,
			 0x3f800000,
			 0x41880000,

			 /// vpu register f24
			 0x41400000,
			 0x41f80000,
			 0x41a80000,
			 0x41c80000,
			 0x41e80000,
			 0x41300000,
			 0x41000000,
			 0x41100000,

			 /// vpu register f25
			 0x41d00000,
			 0x41e80000,
			 0x41300000,
			 0x41c80000,
			 0x41c00000,
			 0x41c00000,
			 0x40000000,
			 0x40800000,

			 /// vpu register f26
			 0x41500000,
			 0x40e00000,
			 0x41c80000,
			 0x41e80000,
			 0x41880000,
			 0x3f800000,
			 0x41500000,
			 0x41b00000,

			 /// vpu register f27
			 0x41b80000,
			 0x40c00000,
			 0x41f00000,
			 0x41980000,
			 0x41100000,
			 0x42000000,
			 0x41c80000,
			 0x41c80000,

			 /// vpu register f28
			 0x41d80000,
			 0x40400000,
			 0x41d80000,
			 0x41d00000,
			 0x41900000,
			 0x41d00000,
			 0x41c00000,
			 0x40a00000,

			 /// vpu register f29
			 0x41f00000,
			 0x41000000,
			 0x41300000,
			 0x41980000,
			 0x41b00000,
			 0x41600000,
			 0x42000000,
			 0x41a80000,

			 /// vpu register f30
			 0x40c00000,
			 0x40e00000,
			 0x42000000,
			 0x41e80000,
			 0x41000000,
			 0x41e00000,
			 0x40a00000,
			 0x41700000,

			 /// vpu register f31
			 0x40e00000,
			 0x41e00000,
			 0x41980000,
			 0x41980000,
			 0x41d00000,
			 0x40e00000,
			 0x41e00000,
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
		"fadd.ps f4, f30, f5, rne\n"                          ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f7, f9, f24, rne\n"                          ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f18, f27, f1, rmm\n"                         ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f7, f10, f17, dyn\n"                         ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f16, f15, f30, rup\n"                        ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f21, f3, f29, rmm\n"                         ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f24, f21, f29, rdn\n"                        ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f2, f28, f16, rdn\n"                         ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f20, f5, f8, dyn\n"                          ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f16, f29, f7, rup\n"                         ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f12, f17, f30, rne\n"                        ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f18, f0, f12, dyn\n"                         ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f21, f30, f10, rne\n"                        ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f20, f22, f9, rdn\n"                         ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f19, f10, f27, rup\n"                        ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f11, f11, f10, rdn\n"                        ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f23, f8, f0, rup\n"                          ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f3, f30, f2, rdn\n"                          ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f22, f20, f5, rup\n"                         ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f13, f14, f14, dyn\n"                        ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f19, f14, f24, dyn\n"                        ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f13, f18, f12, rne\n"                        ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f26, f26, f4, rdn\n"                         ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f6, f12, f4, rup\n"                          ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f30, f6, f5, rne\n"                          ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f18, f11, f16, dyn\n"                        ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f14, f0, f18, dyn\n"                         ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f28, f17, f10, rne\n"                        ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f4, f3, f3, rne\n"                           ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f29, f20, f19, rne\n"                        ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f4, f22, f25, rup\n"                         ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f16, f5, f15, rtz\n"                         ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f6, f28, f27, rdn\n"                         ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f8, f19, f27, dyn\n"                         ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f30, f3, f3, rmm\n"                          ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f22, f7, f26, rne\n"                         ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f23, f4, f13, rtz\n"                         ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f22, f3, f24, rdn\n"                         ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f6, f1, f7, rne\n"                           ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f11, f11, f19, rup\n"                        ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f29, f14, f23, rtz\n"                        ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f28, f25, f28, rmm\n"                        ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f19, f0, f27, rmm\n"                         ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f8, f6, f27, rne\n"                          ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f3, f3, f18, dyn\n"                          ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f18, f28, f15, rtz\n"                        ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f3, f11, f18, dyn\n"                         ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f9, f26, f18, rup\n"                         ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f4, f1, f27, rmm\n"                          ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f26, f16, f27, rmm\n"                        ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f12, f5, f17, dyn\n"                         ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f7, f12, f30, rup\n"                         ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f22, f22, f19, dyn\n"                        ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f18, f28, f10, rmm\n"                        ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f30, f10, f13, rne\n"                        ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f25, f4, f0, rmm\n"                          ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f13, f28, f9, rup\n"                         ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f10, f14, f23, rtz\n"                        ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f3, f3, f6, rne\n"                           ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f16, f7, f7, dyn\n"                          ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f4, f8, f17, rtz\n"                          ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f29, f23, f16, dyn\n"                        ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f13, f20, f4, rne\n"                         ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f8, f15, f3, rup\n"                          ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f10, f19, f26, rmm\n"                        ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f25, f4, f30, rtz\n"                         ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f6, f20, f15, rmm\n"                         ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f11, f25, f10, rdn\n"                        ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f3, f22, f20, rdn\n"                         ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f14, f10, f7, dyn\n"                         ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f26, f7, f16, dyn\n"                         ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f4, f5, f26, rup\n"                          ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f23, f28, f2, rdn\n"                         ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f10, f18, f29, rmm\n"                        ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f19, f21, f24, rtz\n"                        ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f19, f1, f28, rup\n"                         ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f9, f18, f25, dyn\n"                         ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f26, f18, f19, rup\n"                        ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f15, f14, f21, rup\n"                        ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f20, f18, f29, rup\n"                        ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f27, f30, f3, rdn\n"                         ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f11, f3, f11, rup\n"                         ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f3, f1, f25, rmm\n"                          ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f23, f24, f8, rne\n"                         ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f9, f11, f24, rmm\n"                         ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f24, f26, f18, rtz\n"                        ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f6, f17, f15, rne\n"                         ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f16, f13, f2, rne\n"                         ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f11, f11, f13, dyn\n"                        ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f9, f29, f10, dyn\n"                         ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f9, f17, f21, rne\n"                         ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f11, f23, f6, rup\n"                         ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f15, f4, f7, rmm\n"                          ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f13, f29, f16, rmm\n"                        ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f30, f8, f27, rne\n"                         ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f9, f4, f7, rne\n"                           ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f29, f26, f22, rup\n"                        ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f5, f14, f4, rtz\n"                          ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.ps f13, f8, f22, rmm\n"                         ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
