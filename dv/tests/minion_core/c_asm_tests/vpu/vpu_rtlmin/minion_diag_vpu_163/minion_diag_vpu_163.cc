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
			 0x0c600000,                                                  // Leading 1s:                                 /// 00000
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00008
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0000c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00010
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00014
			 0x3f028f5c,                                                  // 0.51                                        /// 00018
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00020
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0002c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00034
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0003c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00040
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00044
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00054
			 0x7fc00001,                                                  // signaling NaN                               /// 00058
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0005c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0006c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00074
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00078
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0007c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00080
			 0x33333333,                                                  // 4 random values                             /// 00084
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00088
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0008c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00090
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00094
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0009c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000a4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000b4
			 0xbf028f5c,                                                  // -0.51                                       /// 000b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x0e000003,                                                  // Trailing 1s:                                /// 000c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000d8
			 0xffc00001,                                                  // -signaling NaN                              /// 000dc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 000e4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00104
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00118
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0012c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00134
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00138
			 0x7f800000,                                                  // inf                                         /// 0013c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00140
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00144
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00148
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00150
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00154
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00158
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00160
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0016c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00170
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00174
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00178
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0017c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00180
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00184
			 0xcb000000,                                                  // -8388608.0                                  /// 00188
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00190
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00194
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00198
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001a0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001b4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001ec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001fc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00200
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00204
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0020c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0021c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00220
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00224
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00228
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0022c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00230
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00238
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00240
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0024c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00250
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00258
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0025c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00260
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00264
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00274
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00278
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0027c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00288
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0028c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00294
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00298
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 002a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 002c8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002e8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 002f8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00300
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00304
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00310
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00314
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00318
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0031c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00320
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00324
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0032c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00334
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00338
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00344
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00348
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0035c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00360
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00364
			 0x3f028f5c,                                                  // 0.51                                        /// 00368
			 0xcb000000,                                                  // -8388608.0                                  /// 0036c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00370
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00374
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00378
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0037c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00388
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00390
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00394
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0xcb000000,                                                  // -8388608.0                                  /// 003b8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 003c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003c4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003d8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003dc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x7f800000,                                                  // inf                                         /// 003f0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00400
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00404
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00408
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00410
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00414
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0041c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00420
			 0x0c700000,                                                  // Leading 1s:                                 /// 00424
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00428
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0042c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00430
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0x0c600000,                                                  // Leading 1s:                                 /// 00438
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00444
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00448
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0044c
			 0xffc00001,                                                  // -signaling NaN                              /// 00450
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00454
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00458
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0045c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00460
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00464
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00468
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0046c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00478
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0047c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00480
			 0x7f800000,                                                  // inf                                         /// 00484
			 0xbf028f5c,                                                  // -0.51                                       /// 00488
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0048c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00498
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004a4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004a8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x7fc00001,                                                  // signaling NaN                               /// 004bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004c0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004e8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004f4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0050c
			 0x7f800000,                                                  // inf                                         /// 00510
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00514
			 0x0c400000,                                                  // Leading 1s:                                 /// 00518
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0051c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00524
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0052c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00530
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00534
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0053c
			 0x3f028f5c,                                                  // 0.51                                        /// 00540
			 0x80011111,                                                  // -9.7958E-41                                 /// 00544
			 0x7fc00001,                                                  // signaling NaN                               /// 00548
			 0x0c600000,                                                  // Leading 1s:                                 /// 0054c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00550
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00564
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00568
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00574
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0057c
			 0xff800000,                                                  // -inf                                        /// 00580
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00588
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0058c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00590
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005a4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005b8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005d4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005dc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005ec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005f0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005fc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00600
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00604
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00608
			 0x0e000003,                                                  // Trailing 1s:                                /// 0060c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00610
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00620
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00624
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00630
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0063c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00640
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x7fc00001,                                                  // signaling NaN                               /// 00648
			 0x80011111,                                                  // -9.7958E-41                                 /// 0064c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00658
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00660
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x0c600000,                                                  // Leading 1s:                                 /// 00668
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0066c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00670
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00678
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00684
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00688
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0068c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00690
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00694
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00698
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0069c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006a0
			 0x00000000,                                                  // zero                                        /// 006a4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006a8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006b0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0xcb000000,                                                  // -8388608.0                                  /// 006b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006d0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006f8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00700
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00708
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00710
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00714
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00720
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00724
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00728
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0072c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00730
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0073c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00740
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00744
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00748
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00754
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0075c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00764
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0076c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00774
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00778
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00780
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0078c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00794
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00798
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007a8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007ac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007c0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007e0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x7f800000,                                                  // inf                                         /// 007ec
			 0x0e000007,                                                  // Trailing 1s:                                /// 007f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007f8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007fc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00800
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00804
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00808
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00810
			 0x55555555,                                                  // 4 random values                             /// 00814
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00818
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00820
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00824
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00828
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0082c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00830
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00834
			 0x33333333,                                                  // 4 random values                             /// 00838
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0083c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00840
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00844
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00848
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00850
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00854
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00860
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00868
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0086c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00870
			 0x00011111,                                                  // 9.7958E-41                                  /// 00874
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0087c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00880
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00884
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0088c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00898
			 0x3f028f5c,                                                  // 0.51                                        /// 0089c
			 0xbf028f5c,                                                  // -0.51                                       /// 008a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008a8
			 0x7fc00001,                                                  // signaling NaN                               /// 008ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008cc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008dc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008ec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x00000000,                                                  // zero                                        /// 00904
			 0xffc00001,                                                  // -signaling NaN                              /// 00908
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0090c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00910
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00918
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0091c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00920
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00924
			 0xcb000000,                                                  // -8388608.0                                  /// 00928
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0092c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00930
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00934
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00938
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0093c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00944
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00950
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00954
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00958
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0095c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00964
			 0x0c600000,                                                  // Leading 1s:                                 /// 00968
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0096c
			 0x33333333,                                                  // 4 random values                             /// 00970
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00974
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00978
			 0x0c780000,                                                  // Leading 1s:                                 /// 0097c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00980
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00988
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0098c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x3f028f5c,                                                  // 0.51                                        /// 009a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009a8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x80011111,                                                  // -9.7958E-41                                 /// 009b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009b4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 009d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009d4
			 0x7fc00001,                                                  // signaling NaN                               /// 009d8
			 0xcb000000,                                                  // -8388608.0                                  /// 009dc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009f0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a00
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a04
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a08
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a0c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a10
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a18
			 0x80000000,                                                  // -zero                                       /// 00a1c
			 0x00000000,                                                  // zero                                        /// 00a20
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a24
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a28
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a2c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a30
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a3c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a40
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a44
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a48
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a4c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a60
			 0x4b000000,                                                  // 8388608.0                                   /// 00a64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a6c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a70
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a74
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a78
			 0x33333333,                                                  // 4 random values                             /// 00a7c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a80
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a84
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a88
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a90
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a94
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a98
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a9c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00aa0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ab0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ab4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ab8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00abc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ac0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ac8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00acc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ad0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ad4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ad8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00adc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ae4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00aec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00af0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00af4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00af8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00afc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b08
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b10
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b14
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b20
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b28
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b2c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b34
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b3c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b44
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b48
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b50
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b58
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b64
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b68
			 0xffc00001,                                                  // -signaling NaN                              /// 00b6c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b78
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b7c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b80
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b84
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b88
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b90
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b94
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ba0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ba8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bb0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bb4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bb8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bbc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bc8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bcc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bd4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bdc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00be0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00be8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bf0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bf4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bfc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c00
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c04
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c0c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c20
			 0xff800000,                                                  // -inf                                        /// 00c24
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c2c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c38
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c3c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c40
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c48
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c4c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c50
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c60
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c64
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c68
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c6c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c70
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c74
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c7c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c80
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c84
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c8c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c94
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c98
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c9c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ca0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ca4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ca8
			 0x80000000,                                                  // -zero                                       /// 00cac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cb0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cb4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cc8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ccc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cd0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cd4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cd8
			 0x7fc00001,                                                  // signaling NaN                               /// 00cdc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ce0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ce4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cf0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cf8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cfc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d00
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d04
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d08
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d0c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d18
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d1c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d20
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d24
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d28
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d30
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d34
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d38
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d3c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d44
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d48
			 0x7f800000,                                                  // inf                                         /// 00d4c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d50
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d54
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0xcb000000,                                                  // -8388608.0                                  /// 00d5c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d64
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d68
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d6c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d78
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d7c
			 0x80000000,                                                  // -zero                                       /// 00d80
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d84
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d88
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d8c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d90
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d94
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x4b000000,                                                  // 8388608.0                                   /// 00d9c
			 0x4b000000,                                                  // 8388608.0                                   /// 00da0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00da8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00db4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00db8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dbc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dc4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dc8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dcc
			 0x33333333,                                                  // 4 random values                             /// 00dd0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00dd4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00dd8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00de4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00df0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00df4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00df8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00dfc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e04
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e10
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e14
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e18
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e24
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e2c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e30
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e34
			 0x80000000,                                                  // -zero                                       /// 00e38
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e3c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e40
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e4c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e50
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e58
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e60
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e64
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e6c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e70
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e74
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e7c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e80
			 0xcb000000,                                                  // -8388608.0                                  /// 00e84
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e88
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e8c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e90
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x4b000000,                                                  // 8388608.0                                   /// 00e98
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e9c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ea0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ea4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ea8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00eac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ebc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ec0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ec4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ec8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0xbf028f5c,                                                  // -0.51                                       /// 00ed0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ed4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ee0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ee8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00eec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ef0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ef4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ef8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00efc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f00
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f04
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f08
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f10
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f14
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f20
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f24
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f28
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f2c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f30
			 0x80000000,                                                  // -zero                                       /// 00f34
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f3c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f50
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f54
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f58
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f5c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f60
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f64
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f68
			 0xcb000000,                                                  // -8388608.0                                  /// 00f6c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f70
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f78
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f7c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f80
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f84
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f90
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f9c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fa0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fa4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fa8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fb4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fb8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fbc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fc0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fc4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fd4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fd8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fdc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fe0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fe4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fe8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0xff800000,                                                  // -inf                                        /// 00ff0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ff4
			 0x00000000,                                                  // zero                                        /// 00ff8
			 0x4b000000                                                  // 8388608.0                                   /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xff85bf6b, /// 0x0
			 0x1adf35d9, /// 0x4
			 0x41a68335, /// 0x8
			 0xb65a1326, /// 0xc
			 0xeb5db9ad, /// 0x10
			 0x97c7a932, /// 0x14
			 0x3737bc02, /// 0x18
			 0xea7ade5f, /// 0x1c
			 0x5a8a1111, /// 0x20
			 0xa90a4a67, /// 0x24
			 0xc589ec50, /// 0x28
			 0xc020f351, /// 0x2c
			 0x474bacc5, /// 0x30
			 0x4864b4cc, /// 0x34
			 0xf223e42b, /// 0x38
			 0x7775084b, /// 0x3c
			 0xb0acdf88, /// 0x40
			 0x2bb6040a, /// 0x44
			 0x4a272405, /// 0x48
			 0x064ebfbc, /// 0x4c
			 0xde3d693d, /// 0x50
			 0x74fe5af4, /// 0x54
			 0xaedfd1d3, /// 0x58
			 0x2361ba8e, /// 0x5c
			 0xc8267735, /// 0x60
			 0xc63cb37c, /// 0x64
			 0x88686508, /// 0x68
			 0x91318828, /// 0x6c
			 0x908532bc, /// 0x70
			 0xc286e76f, /// 0x74
			 0xeb13337d, /// 0x78
			 0x50463b33, /// 0x7c
			 0x75eb9295, /// 0x80
			 0x25fb7355, /// 0x84
			 0x499a7bfa, /// 0x88
			 0x521defeb, /// 0x8c
			 0x7d60cd63, /// 0x90
			 0xc0a70693, /// 0x94
			 0xcf244599, /// 0x98
			 0x4d9dd0f7, /// 0x9c
			 0x45c6f922, /// 0xa0
			 0x1db05e9e, /// 0xa4
			 0xc3152477, /// 0xa8
			 0xedbe3342, /// 0xac
			 0x4bbcc10a, /// 0xb0
			 0x47e785ad, /// 0xb4
			 0x2a74b238, /// 0xb8
			 0x82b9601f, /// 0xbc
			 0x5807bbda, /// 0xc0
			 0x48c8cb0b, /// 0xc4
			 0xd22cc1b0, /// 0xc8
			 0x43224831, /// 0xcc
			 0x9633dc1a, /// 0xd0
			 0xe4704290, /// 0xd4
			 0x09d00143, /// 0xd8
			 0xfe5433ed, /// 0xdc
			 0x72f1975e, /// 0xe0
			 0x98c37dba, /// 0xe4
			 0x4796e226, /// 0xe8
			 0x04f10f5c, /// 0xec
			 0x048baf72, /// 0xf0
			 0x769f4323, /// 0xf4
			 0x33f447d0, /// 0xf8
			 0xc04ed661, /// 0xfc
			 0xa67a7db0, /// 0x100
			 0xf22e29ad, /// 0x104
			 0xd89ad1dc, /// 0x108
			 0xafc222a3, /// 0x10c
			 0xb4141b36, /// 0x110
			 0x81e101fb, /// 0x114
			 0x13ca8151, /// 0x118
			 0xac670c14, /// 0x11c
			 0xc16ecf73, /// 0x120
			 0xb7176102, /// 0x124
			 0x305a23f0, /// 0x128
			 0x509841b7, /// 0x12c
			 0xe0ea3fe0, /// 0x130
			 0x38dc62ca, /// 0x134
			 0x899f80a8, /// 0x138
			 0x213fff65, /// 0x13c
			 0x14710c36, /// 0x140
			 0x36103234, /// 0x144
			 0xa1d41da5, /// 0x148
			 0x98295827, /// 0x14c
			 0xbbacd7f3, /// 0x150
			 0x8b304464, /// 0x154
			 0x621663d8, /// 0x158
			 0x4c298df9, /// 0x15c
			 0x34187df0, /// 0x160
			 0x566f420a, /// 0x164
			 0x1075b34c, /// 0x168
			 0xfd0b6e9d, /// 0x16c
			 0xbd4f0712, /// 0x170
			 0xffc4eeb2, /// 0x174
			 0x3bdcac89, /// 0x178
			 0xf2167c01, /// 0x17c
			 0xabaa2134, /// 0x180
			 0xf719e101, /// 0x184
			 0x1a632790, /// 0x188
			 0xbee6e93b, /// 0x18c
			 0x7875a19d, /// 0x190
			 0x7a5231ca, /// 0x194
			 0x63059056, /// 0x198
			 0xfd416a68, /// 0x19c
			 0xffee447e, /// 0x1a0
			 0xc5d8d095, /// 0x1a4
			 0x722eed18, /// 0x1a8
			 0xd71bcc2d, /// 0x1ac
			 0x23262cd5, /// 0x1b0
			 0x845beddd, /// 0x1b4
			 0xa11e335f, /// 0x1b8
			 0x2b487106, /// 0x1bc
			 0xc6084659, /// 0x1c0
			 0xff745487, /// 0x1c4
			 0x8cab233b, /// 0x1c8
			 0x19bf7985, /// 0x1cc
			 0x733805b7, /// 0x1d0
			 0x52ab11bb, /// 0x1d4
			 0xa3d885c8, /// 0x1d8
			 0x2a976a32, /// 0x1dc
			 0x21326142, /// 0x1e0
			 0xd8627294, /// 0x1e4
			 0x86be8cb3, /// 0x1e8
			 0x9313218f, /// 0x1ec
			 0x9f3f259c, /// 0x1f0
			 0x0dbc0c12, /// 0x1f4
			 0xaad9b2b0, /// 0x1f8
			 0x72f3466d, /// 0x1fc
			 0x7737eb60, /// 0x200
			 0x0bf53fd4, /// 0x204
			 0x266e8632, /// 0x208
			 0x2af8ac14, /// 0x20c
			 0x77ead325, /// 0x210
			 0x3d2e117a, /// 0x214
			 0x4b226bae, /// 0x218
			 0x88c24af7, /// 0x21c
			 0x63cfc141, /// 0x220
			 0x1e857c7b, /// 0x224
			 0x4321033f, /// 0x228
			 0x4a549ce6, /// 0x22c
			 0x369da0f6, /// 0x230
			 0x61d8812a, /// 0x234
			 0x456e5773, /// 0x238
			 0x8e418028, /// 0x23c
			 0x4a87f858, /// 0x240
			 0x74ed366b, /// 0x244
			 0x3a75652c, /// 0x248
			 0x116c10b8, /// 0x24c
			 0xd3c3f39b, /// 0x250
			 0x630d316f, /// 0x254
			 0xa62a6841, /// 0x258
			 0x7873c446, /// 0x25c
			 0x874ad2ae, /// 0x260
			 0x146a355b, /// 0x264
			 0xf9c40b1b, /// 0x268
			 0x08590e13, /// 0x26c
			 0x2dc6b623, /// 0x270
			 0xeb5be53a, /// 0x274
			 0x38b9c1b9, /// 0x278
			 0xa7c37140, /// 0x27c
			 0xc7bd7b3f, /// 0x280
			 0x4fd67148, /// 0x284
			 0x8042cc0e, /// 0x288
			 0x6a70be68, /// 0x28c
			 0xb5286f56, /// 0x290
			 0x25f6179a, /// 0x294
			 0x4695b480, /// 0x298
			 0xd2cf70f3, /// 0x29c
			 0xe24a07a1, /// 0x2a0
			 0xe130a374, /// 0x2a4
			 0x932d1736, /// 0x2a8
			 0x17dddf59, /// 0x2ac
			 0x41f9c804, /// 0x2b0
			 0xa921c9d3, /// 0x2b4
			 0xe5b06815, /// 0x2b8
			 0x3abc743f, /// 0x2bc
			 0xeac0355d, /// 0x2c0
			 0x5598c568, /// 0x2c4
			 0x11fe22a8, /// 0x2c8
			 0xc032d0ce, /// 0x2cc
			 0x5efb2beb, /// 0x2d0
			 0x213f2daa, /// 0x2d4
			 0x72fa3090, /// 0x2d8
			 0xaff013fc, /// 0x2dc
			 0xa9806ce2, /// 0x2e0
			 0xf46a7f5c, /// 0x2e4
			 0x962fdb9c, /// 0x2e8
			 0x10bc32a3, /// 0x2ec
			 0xb3e70fce, /// 0x2f0
			 0x803f8820, /// 0x2f4
			 0x17c1f441, /// 0x2f8
			 0x94e165ac, /// 0x2fc
			 0xfeb6064a, /// 0x300
			 0xd90479b1, /// 0x304
			 0x84d1c60a, /// 0x308
			 0x12299245, /// 0x30c
			 0x39fe03e0, /// 0x310
			 0x1715b8bb, /// 0x314
			 0x927fa723, /// 0x318
			 0x5fa0469f, /// 0x31c
			 0x93b38433, /// 0x320
			 0x3d6709ed, /// 0x324
			 0x205b372e, /// 0x328
			 0xd1a5e4cb, /// 0x32c
			 0x94543cc8, /// 0x330
			 0x52a53a04, /// 0x334
			 0x66954ef7, /// 0x338
			 0x3b92b86d, /// 0x33c
			 0x72bf60b0, /// 0x340
			 0x6d0acb07, /// 0x344
			 0x86fc296e, /// 0x348
			 0xb43c9fa7, /// 0x34c
			 0x28ae881c, /// 0x350
			 0xbdf08407, /// 0x354
			 0xaf13d447, /// 0x358
			 0x7f1be29a, /// 0x35c
			 0x757f7288, /// 0x360
			 0x7e1e05f0, /// 0x364
			 0x0a7f3832, /// 0x368
			 0xfc812954, /// 0x36c
			 0x09fe4fb5, /// 0x370
			 0xe63db3e8, /// 0x374
			 0xb71369e0, /// 0x378
			 0x1d36e7ae, /// 0x37c
			 0xd1443fa7, /// 0x380
			 0x27ad1588, /// 0x384
			 0x043ea62e, /// 0x388
			 0x8998b39a, /// 0x38c
			 0x097be016, /// 0x390
			 0x6b17560e, /// 0x394
			 0x402af3fa, /// 0x398
			 0x6b2c54ea, /// 0x39c
			 0xcc812d8d, /// 0x3a0
			 0xeb33992d, /// 0x3a4
			 0x2a832a0b, /// 0x3a8
			 0xb0a7460b, /// 0x3ac
			 0x8b253ceb, /// 0x3b0
			 0xbe5c9cf1, /// 0x3b4
			 0x78e3a2be, /// 0x3b8
			 0x36f3d470, /// 0x3bc
			 0xa054b377, /// 0x3c0
			 0xfd6538d0, /// 0x3c4
			 0x2dd65b53, /// 0x3c8
			 0x7df255c0, /// 0x3cc
			 0x5ff7bae6, /// 0x3d0
			 0x838d7fbc, /// 0x3d4
			 0x45faa7f5, /// 0x3d8
			 0x11fd9cd4, /// 0x3dc
			 0xf0ef5680, /// 0x3e0
			 0x6631206f, /// 0x3e4
			 0x7449026e, /// 0x3e8
			 0xe10a3d3b, /// 0x3ec
			 0x2c809347, /// 0x3f0
			 0x847051c4, /// 0x3f4
			 0x039389ae, /// 0x3f8
			 0x40ed8b37, /// 0x3fc
			 0x6c49f0b8, /// 0x400
			 0x238a06a4, /// 0x404
			 0x91dcc76d, /// 0x408
			 0xf001c0b2, /// 0x40c
			 0x6294637a, /// 0x410
			 0x48b8d5da, /// 0x414
			 0x30e462f3, /// 0x418
			 0xf8371805, /// 0x41c
			 0x4171454a, /// 0x420
			 0x309ff15e, /// 0x424
			 0x9d9b65f5, /// 0x428
			 0xe961675b, /// 0x42c
			 0x98b484b9, /// 0x430
			 0xd0fba543, /// 0x434
			 0x034f38f2, /// 0x438
			 0xc4eaa2ca, /// 0x43c
			 0xc35b55ab, /// 0x440
			 0xed1b7651, /// 0x444
			 0xfe54c87a, /// 0x448
			 0x86992dca, /// 0x44c
			 0x96192743, /// 0x450
			 0x3b424b19, /// 0x454
			 0x4329a1b0, /// 0x458
			 0x693403bd, /// 0x45c
			 0xc564e2e3, /// 0x460
			 0x4c208a1a, /// 0x464
			 0xeb159e6b, /// 0x468
			 0xae9ecfbb, /// 0x46c
			 0xb3c15ba9, /// 0x470
			 0x334aa65f, /// 0x474
			 0xfc38f9e1, /// 0x478
			 0x57712933, /// 0x47c
			 0xbdfd8eeb, /// 0x480
			 0xca9d913a, /// 0x484
			 0xe01f2a94, /// 0x488
			 0xac4479aa, /// 0x48c
			 0xd96ca055, /// 0x490
			 0x51fdc570, /// 0x494
			 0xfb3e62b1, /// 0x498
			 0xd43e4510, /// 0x49c
			 0xaa13716b, /// 0x4a0
			 0x6995fb70, /// 0x4a4
			 0x23d63a0b, /// 0x4a8
			 0xa512ac0e, /// 0x4ac
			 0xaa609242, /// 0x4b0
			 0x151209a3, /// 0x4b4
			 0x02f4222d, /// 0x4b8
			 0x7f4fbda8, /// 0x4bc
			 0x22a013ad, /// 0x4c0
			 0x521306f2, /// 0x4c4
			 0x4e4f28f4, /// 0x4c8
			 0x7a73e036, /// 0x4cc
			 0x5be794f1, /// 0x4d0
			 0x29b37fef, /// 0x4d4
			 0xdbeec9af, /// 0x4d8
			 0xa95edb56, /// 0x4dc
			 0x78296dac, /// 0x4e0
			 0xa19d0633, /// 0x4e4
			 0xf43846e4, /// 0x4e8
			 0xc4b25836, /// 0x4ec
			 0x339d2183, /// 0x4f0
			 0x42c2a26b, /// 0x4f4
			 0xd1f75fab, /// 0x4f8
			 0xd9fb395f, /// 0x4fc
			 0x53f692e9, /// 0x500
			 0xd29cf9b8, /// 0x504
			 0x9a5225d3, /// 0x508
			 0x5b902f34, /// 0x50c
			 0x2c3d529e, /// 0x510
			 0xa2e0420c, /// 0x514
			 0xfa63d5d9, /// 0x518
			 0x82c57c7f, /// 0x51c
			 0x07849020, /// 0x520
			 0xe69977d5, /// 0x524
			 0x3ea9b4cb, /// 0x528
			 0xa446ec3a, /// 0x52c
			 0x306d053e, /// 0x530
			 0xf853221c, /// 0x534
			 0x9542f506, /// 0x538
			 0x085b92fe, /// 0x53c
			 0x5ebd682d, /// 0x540
			 0x3e6f13ed, /// 0x544
			 0xd5dd193f, /// 0x548
			 0x13beff12, /// 0x54c
			 0x6daacb28, /// 0x550
			 0x77c9eb4e, /// 0x554
			 0xe5ec8075, /// 0x558
			 0x6abc3098, /// 0x55c
			 0x95955715, /// 0x560
			 0x7e8b6504, /// 0x564
			 0x1e6137e1, /// 0x568
			 0x36e1cf8e, /// 0x56c
			 0x8e0aef04, /// 0x570
			 0xb72d7ea6, /// 0x574
			 0x4d32f512, /// 0x578
			 0x56954a15, /// 0x57c
			 0xf0e2a209, /// 0x580
			 0x21c258c0, /// 0x584
			 0x863f5763, /// 0x588
			 0xedf88850, /// 0x58c
			 0xc34f3b55, /// 0x590
			 0x8a4b7c84, /// 0x594
			 0x84d0592e, /// 0x598
			 0x31052951, /// 0x59c
			 0x9308a101, /// 0x5a0
			 0x868d02a5, /// 0x5a4
			 0x931689e6, /// 0x5a8
			 0x85f5b22a, /// 0x5ac
			 0x31f3d367, /// 0x5b0
			 0x3b624066, /// 0x5b4
			 0xbd250f97, /// 0x5b8
			 0x2d8ba3a2, /// 0x5bc
			 0xbe71460b, /// 0x5c0
			 0x0632b499, /// 0x5c4
			 0x38b8904b, /// 0x5c8
			 0x2f6df827, /// 0x5cc
			 0x32422d49, /// 0x5d0
			 0x587c894f, /// 0x5d4
			 0x30d97c69, /// 0x5d8
			 0xb4938ed6, /// 0x5dc
			 0x1dd04610, /// 0x5e0
			 0x3f8025cb, /// 0x5e4
			 0xa7faddd6, /// 0x5e8
			 0x4f249ec2, /// 0x5ec
			 0x71ff3e79, /// 0x5f0
			 0xe2eacf4c, /// 0x5f4
			 0x68d4d670, /// 0x5f8
			 0x0b11a5b1, /// 0x5fc
			 0x914f8308, /// 0x600
			 0x8086a0f4, /// 0x604
			 0xba53fe32, /// 0x608
			 0x27ad1229, /// 0x60c
			 0xda2f9fe2, /// 0x610
			 0x2a76337d, /// 0x614
			 0x7ce5bbda, /// 0x618
			 0xf983a2df, /// 0x61c
			 0xa0e264a6, /// 0x620
			 0xb325c356, /// 0x624
			 0x685607df, /// 0x628
			 0xfe3b4294, /// 0x62c
			 0xa86b2d29, /// 0x630
			 0x66fc5bc3, /// 0x634
			 0x68681a65, /// 0x638
			 0x99150935, /// 0x63c
			 0x3c3d74fb, /// 0x640
			 0xc7d7abdf, /// 0x644
			 0xa2722b9c, /// 0x648
			 0x78426a2c, /// 0x64c
			 0xef84b1fc, /// 0x650
			 0x5b5d4d10, /// 0x654
			 0x638770e0, /// 0x658
			 0xa49e6452, /// 0x65c
			 0xcca97ebf, /// 0x660
			 0x35cd5404, /// 0x664
			 0x9120fa8c, /// 0x668
			 0x76296218, /// 0x66c
			 0x3a1dcbc4, /// 0x670
			 0x54ef77eb, /// 0x674
			 0x1303858e, /// 0x678
			 0x7c827775, /// 0x67c
			 0x74c657ed, /// 0x680
			 0xad8f7847, /// 0x684
			 0x65b5d632, /// 0x688
			 0x4f03b7e1, /// 0x68c
			 0xac5df257, /// 0x690
			 0x0f1365cc, /// 0x694
			 0xc0ee99b7, /// 0x698
			 0x837aa6f3, /// 0x69c
			 0x0301b1eb, /// 0x6a0
			 0x28c0663d, /// 0x6a4
			 0x197f345e, /// 0x6a8
			 0x150fa62d, /// 0x6ac
			 0x98fb19c3, /// 0x6b0
			 0x12a72dda, /// 0x6b4
			 0x540049cc, /// 0x6b8
			 0xe7d7c07c, /// 0x6bc
			 0x98bee323, /// 0x6c0
			 0xe6478763, /// 0x6c4
			 0xc0b0b246, /// 0x6c8
			 0x57cbbb83, /// 0x6cc
			 0x19f93781, /// 0x6d0
			 0x0c69bf8b, /// 0x6d4
			 0xff4c1157, /// 0x6d8
			 0x3458d672, /// 0x6dc
			 0x7a1edb75, /// 0x6e0
			 0xab7ca044, /// 0x6e4
			 0xd015bf2e, /// 0x6e8
			 0x21d5227b, /// 0x6ec
			 0xed54ea39, /// 0x6f0
			 0x45a9fa69, /// 0x6f4
			 0x308d8b78, /// 0x6f8
			 0xa2bf090b, /// 0x6fc
			 0x55aa726e, /// 0x700
			 0x1b8c6939, /// 0x704
			 0x00a1202f, /// 0x708
			 0x4036592d, /// 0x70c
			 0xe04c615b, /// 0x710
			 0xe59585fe, /// 0x714
			 0x69082761, /// 0x718
			 0x1a0de1ce, /// 0x71c
			 0xd22365cb, /// 0x720
			 0x6ad96337, /// 0x724
			 0x9fc3d02b, /// 0x728
			 0xfa847012, /// 0x72c
			 0x89f4d125, /// 0x730
			 0x93144ac9, /// 0x734
			 0x452073f1, /// 0x738
			 0x2607e5ab, /// 0x73c
			 0xb5b76372, /// 0x740
			 0x48e03928, /// 0x744
			 0x8ce23e61, /// 0x748
			 0x9290e09f, /// 0x74c
			 0x70eaf7cf, /// 0x750
			 0x306f31f5, /// 0x754
			 0x1ce8b293, /// 0x758
			 0x6c221054, /// 0x75c
			 0xc5136c38, /// 0x760
			 0x1f2a2b23, /// 0x764
			 0x42be2c82, /// 0x768
			 0x06a95b6a, /// 0x76c
			 0x1b278383, /// 0x770
			 0xb1012893, /// 0x774
			 0x58a0886d, /// 0x778
			 0xb40b2a04, /// 0x77c
			 0x3f68ef69, /// 0x780
			 0x675c96fd, /// 0x784
			 0xc6bc1ce1, /// 0x788
			 0xdcbd08c0, /// 0x78c
			 0xb8837a4e, /// 0x790
			 0x065d287a, /// 0x794
			 0x6c5e0f1e, /// 0x798
			 0x23b5ad87, /// 0x79c
			 0x3c60a0cd, /// 0x7a0
			 0x3850af2a, /// 0x7a4
			 0x83ae150c, /// 0x7a8
			 0x6455c776, /// 0x7ac
			 0x1746adf8, /// 0x7b0
			 0xafb49105, /// 0x7b4
			 0xbef15fa1, /// 0x7b8
			 0xdd8bf739, /// 0x7bc
			 0xa644c830, /// 0x7c0
			 0x6e2cd08c, /// 0x7c4
			 0x115a2dfd, /// 0x7c8
			 0x60448b86, /// 0x7cc
			 0xdbee8d25, /// 0x7d0
			 0xbd0bd822, /// 0x7d4
			 0x427091ff, /// 0x7d8
			 0x3f33af95, /// 0x7dc
			 0x76d6b67f, /// 0x7e0
			 0x1cf63323, /// 0x7e4
			 0xfd02dbc4, /// 0x7e8
			 0x3486a560, /// 0x7ec
			 0x3bbc0d51, /// 0x7f0
			 0x70da472a, /// 0x7f4
			 0x258a0ac1, /// 0x7f8
			 0x3870fe63, /// 0x7fc
			 0x80c9f239, /// 0x800
			 0x4ab00b53, /// 0x804
			 0xea2585d7, /// 0x808
			 0xd5408226, /// 0x80c
			 0xff65f63c, /// 0x810
			 0xe5e01ae6, /// 0x814
			 0x56b552ed, /// 0x818
			 0x7048544f, /// 0x81c
			 0x45c7475b, /// 0x820
			 0x59550dc6, /// 0x824
			 0xeb80d526, /// 0x828
			 0x6b69283a, /// 0x82c
			 0xb7b2673b, /// 0x830
			 0xae98a80f, /// 0x834
			 0xe7c5d84f, /// 0x838
			 0xca739483, /// 0x83c
			 0x53105560, /// 0x840
			 0xfad57859, /// 0x844
			 0x48625c70, /// 0x848
			 0xd1e89d83, /// 0x84c
			 0xa55d30ad, /// 0x850
			 0x3ec248fd, /// 0x854
			 0xf9ffa7ad, /// 0x858
			 0xd7ffec2c, /// 0x85c
			 0x62dcf610, /// 0x860
			 0x1232c06d, /// 0x864
			 0x97fe34d3, /// 0x868
			 0x306eb1da, /// 0x86c
			 0xab6513aa, /// 0x870
			 0x25aa16ed, /// 0x874
			 0x317dc629, /// 0x878
			 0xc02ceabe, /// 0x87c
			 0x3a05eb35, /// 0x880
			 0x54a20704, /// 0x884
			 0x657bd12c, /// 0x888
			 0xfc256732, /// 0x88c
			 0x7075ba81, /// 0x890
			 0x1beebfd4, /// 0x894
			 0x1e847d64, /// 0x898
			 0xd43665f4, /// 0x89c
			 0x13bc3f5a, /// 0x8a0
			 0x9b4f758e, /// 0x8a4
			 0xdf2cd8c7, /// 0x8a8
			 0x019b2f18, /// 0x8ac
			 0x366f336c, /// 0x8b0
			 0xf2b451f7, /// 0x8b4
			 0x1c303103, /// 0x8b8
			 0x1c1e80b0, /// 0x8bc
			 0x5b8040b7, /// 0x8c0
			 0x58444ba5, /// 0x8c4
			 0x4ba8a5f0, /// 0x8c8
			 0x4d5d06bf, /// 0x8cc
			 0x7e76ff31, /// 0x8d0
			 0x8e745d71, /// 0x8d4
			 0x796651aa, /// 0x8d8
			 0x704abe76, /// 0x8dc
			 0xba52ad71, /// 0x8e0
			 0xfbf218b6, /// 0x8e4
			 0x53e49fc1, /// 0x8e8
			 0xee9a2179, /// 0x8ec
			 0xdc06df74, /// 0x8f0
			 0x1751c8cc, /// 0x8f4
			 0xdbde1bf6, /// 0x8f8
			 0x99919eb7, /// 0x8fc
			 0x7c55601b, /// 0x900
			 0x06a316fa, /// 0x904
			 0xe93975aa, /// 0x908
			 0x28612836, /// 0x90c
			 0xb4e43e31, /// 0x910
			 0x0e8a6d75, /// 0x914
			 0xc92ad9c7, /// 0x918
			 0xa91ecb62, /// 0x91c
			 0xeb3bb7d5, /// 0x920
			 0xa59e2eec, /// 0x924
			 0x4b5c5598, /// 0x928
			 0x2c03ff16, /// 0x92c
			 0xeb146815, /// 0x930
			 0x57dfb2ef, /// 0x934
			 0xf4954c78, /// 0x938
			 0x6c962d81, /// 0x93c
			 0x81300693, /// 0x940
			 0xe9b27c7b, /// 0x944
			 0xf2ed8b56, /// 0x948
			 0xe0f26fec, /// 0x94c
			 0xaf442ffc, /// 0x950
			 0x87a5147f, /// 0x954
			 0x818efc5e, /// 0x958
			 0x3d797904, /// 0x95c
			 0x30047f28, /// 0x960
			 0x63d319a6, /// 0x964
			 0xb3b67890, /// 0x968
			 0x0f78cb0a, /// 0x96c
			 0x93cadf0c, /// 0x970
			 0x1905070c, /// 0x974
			 0x8973eb41, /// 0x978
			 0x4b567f2e, /// 0x97c
			 0x367ec916, /// 0x980
			 0x08f65e21, /// 0x984
			 0xde367a47, /// 0x988
			 0xd6b60121, /// 0x98c
			 0xd8bd155f, /// 0x990
			 0x0bd24e48, /// 0x994
			 0x83ba69e0, /// 0x998
			 0xfd7ca1ae, /// 0x99c
			 0x91706230, /// 0x9a0
			 0x1d372309, /// 0x9a4
			 0x33440448, /// 0x9a8
			 0xc38b421f, /// 0x9ac
			 0xbfe0803a, /// 0x9b0
			 0xa2993cbe, /// 0x9b4
			 0x11b991a1, /// 0x9b8
			 0x2f00fc8d, /// 0x9bc
			 0x859ffe7d, /// 0x9c0
			 0xca06612e, /// 0x9c4
			 0xdc62ab69, /// 0x9c8
			 0x10891353, /// 0x9cc
			 0x33a18deb, /// 0x9d0
			 0x22b6a38c, /// 0x9d4
			 0x6b44906f, /// 0x9d8
			 0x3a82d3ed, /// 0x9dc
			 0x9a4804ca, /// 0x9e0
			 0x64381fbf, /// 0x9e4
			 0x1abf013b, /// 0x9e8
			 0xc09615f8, /// 0x9ec
			 0x942a59ac, /// 0x9f0
			 0x3a989516, /// 0x9f4
			 0xf79c4ae2, /// 0x9f8
			 0x75158b51, /// 0x9fc
			 0x656861f4, /// 0xa00
			 0x70763fc4, /// 0xa04
			 0x24031da7, /// 0xa08
			 0x01fb0ea3, /// 0xa0c
			 0x32bc690c, /// 0xa10
			 0x3b49518c, /// 0xa14
			 0xaa6fdda7, /// 0xa18
			 0x1458d97e, /// 0xa1c
			 0xb943e3d7, /// 0xa20
			 0xd46d802d, /// 0xa24
			 0x8e788af4, /// 0xa28
			 0x46857765, /// 0xa2c
			 0x33498243, /// 0xa30
			 0x2a04b53e, /// 0xa34
			 0x0dd2d774, /// 0xa38
			 0xba6e6fbd, /// 0xa3c
			 0x64243e41, /// 0xa40
			 0x88dd774f, /// 0xa44
			 0xddf9e278, /// 0xa48
			 0xd4385ae3, /// 0xa4c
			 0xe456e1bd, /// 0xa50
			 0xff0ff0b8, /// 0xa54
			 0xa037b2a0, /// 0xa58
			 0x25517558, /// 0xa5c
			 0xc326fa6d, /// 0xa60
			 0x0c2c4529, /// 0xa64
			 0x44cc5d0b, /// 0xa68
			 0xba88d00e, /// 0xa6c
			 0x4d31ead5, /// 0xa70
			 0xe3a4daa1, /// 0xa74
			 0x1e32a9e0, /// 0xa78
			 0x173e333e, /// 0xa7c
			 0x8cd65704, /// 0xa80
			 0xd4fa8745, /// 0xa84
			 0x23834d2f, /// 0xa88
			 0xcaa56033, /// 0xa8c
			 0x41c3b73c, /// 0xa90
			 0xc839a16b, /// 0xa94
			 0xbd112ac7, /// 0xa98
			 0xcb24935b, /// 0xa9c
			 0x6c098df0, /// 0xaa0
			 0x0994bc68, /// 0xaa4
			 0x6ecf7507, /// 0xaa8
			 0x24e7c156, /// 0xaac
			 0x4b319d31, /// 0xab0
			 0x41060d9c, /// 0xab4
			 0xce6672a2, /// 0xab8
			 0x0568650d, /// 0xabc
			 0x182ed1f0, /// 0xac0
			 0xf900593f, /// 0xac4
			 0x269bd2de, /// 0xac8
			 0xd462d31f, /// 0xacc
			 0xd18af857, /// 0xad0
			 0xb3d8e546, /// 0xad4
			 0xf9a304f7, /// 0xad8
			 0x05e39a90, /// 0xadc
			 0x808c8b5d, /// 0xae0
			 0xe7b1f7ab, /// 0xae4
			 0xce8015ca, /// 0xae8
			 0xb14019f7, /// 0xaec
			 0x074c8e04, /// 0xaf0
			 0x83d456c2, /// 0xaf4
			 0x65ed5a82, /// 0xaf8
			 0x8818b7db, /// 0xafc
			 0xbef98027, /// 0xb00
			 0x129f84f9, /// 0xb04
			 0xcd10d283, /// 0xb08
			 0x369c3ef5, /// 0xb0c
			 0x7601003f, /// 0xb10
			 0x84ab3e4d, /// 0xb14
			 0xc011caaa, /// 0xb18
			 0xe3b086e1, /// 0xb1c
			 0xdbf89bb7, /// 0xb20
			 0x011186a1, /// 0xb24
			 0xfad94072, /// 0xb28
			 0x81d2d6d0, /// 0xb2c
			 0x0f079aa5, /// 0xb30
			 0x080be635, /// 0xb34
			 0x9cd7a8f9, /// 0xb38
			 0xa228b613, /// 0xb3c
			 0xa8254147, /// 0xb40
			 0x6c345ba1, /// 0xb44
			 0xc702c18c, /// 0xb48
			 0x80d95dbc, /// 0xb4c
			 0x3aebca56, /// 0xb50
			 0x27461604, /// 0xb54
			 0x1805a0e4, /// 0xb58
			 0xc078244b, /// 0xb5c
			 0x6d2264c2, /// 0xb60
			 0xb5d6b237, /// 0xb64
			 0x96d52670, /// 0xb68
			 0xcd278086, /// 0xb6c
			 0x2c4a6cd7, /// 0xb70
			 0x2a33a85f, /// 0xb74
			 0x7a5d6ce0, /// 0xb78
			 0xd591ce24, /// 0xb7c
			 0x68cf609a, /// 0xb80
			 0xc09f9288, /// 0xb84
			 0x048c83d2, /// 0xb88
			 0x92ba232b, /// 0xb8c
			 0xf4073097, /// 0xb90
			 0x024552da, /// 0xb94
			 0xd1387a33, /// 0xb98
			 0x91fa253f, /// 0xb9c
			 0x99107044, /// 0xba0
			 0x3686cf8f, /// 0xba4
			 0xa9acbd40, /// 0xba8
			 0xb5e3ce79, /// 0xbac
			 0xa375aab6, /// 0xbb0
			 0x7ce0db21, /// 0xbb4
			 0xfa7cd02e, /// 0xbb8
			 0x134ba6ac, /// 0xbbc
			 0xd7f3a976, /// 0xbc0
			 0x917c04c5, /// 0xbc4
			 0xf0e2021f, /// 0xbc8
			 0x042421e4, /// 0xbcc
			 0x125865e0, /// 0xbd0
			 0xf884db4a, /// 0xbd4
			 0x9150f11d, /// 0xbd8
			 0x6af44e1f, /// 0xbdc
			 0xfa63ae1d, /// 0xbe0
			 0xaa7e0512, /// 0xbe4
			 0xa071b514, /// 0xbe8
			 0x1fb0f9ce, /// 0xbec
			 0x2322d24d, /// 0xbf0
			 0x8ee4699c, /// 0xbf4
			 0x645afe15, /// 0xbf8
			 0xf2ecd88f, /// 0xbfc
			 0x17dfbe59, /// 0xc00
			 0xf55dd148, /// 0xc04
			 0x4812f59b, /// 0xc08
			 0xeaeb033e, /// 0xc0c
			 0x34cca653, /// 0xc10
			 0xd3dc72ae, /// 0xc14
			 0x6009b3a0, /// 0xc18
			 0x36882f90, /// 0xc1c
			 0x82d7babb, /// 0xc20
			 0x8ec8b8a3, /// 0xc24
			 0xa4e5a1eb, /// 0xc28
			 0x0d52b43e, /// 0xc2c
			 0xb0e6b060, /// 0xc30
			 0x56674501, /// 0xc34
			 0x7dbdf1d2, /// 0xc38
			 0x421bd625, /// 0xc3c
			 0xfbac65b0, /// 0xc40
			 0xe3b834a0, /// 0xc44
			 0x2fce8ae4, /// 0xc48
			 0x9a64e623, /// 0xc4c
			 0xf1e4d7d0, /// 0xc50
			 0x9d47375b, /// 0xc54
			 0x950371da, /// 0xc58
			 0x44f0a8c8, /// 0xc5c
			 0xffb696d6, /// 0xc60
			 0xb1f504e3, /// 0xc64
			 0x60cf217b, /// 0xc68
			 0x3e4e7c77, /// 0xc6c
			 0x58c5cc79, /// 0xc70
			 0x77643cee, /// 0xc74
			 0x7382f584, /// 0xc78
			 0x981fa328, /// 0xc7c
			 0x5712e431, /// 0xc80
			 0x8be06a12, /// 0xc84
			 0x1ecc168e, /// 0xc88
			 0xa212ff5d, /// 0xc8c
			 0xb11ef079, /// 0xc90
			 0x09e973ea, /// 0xc94
			 0x104ed238, /// 0xc98
			 0x35b575a1, /// 0xc9c
			 0xb1827762, /// 0xca0
			 0xa5e68bfa, /// 0xca4
			 0xfe54f4dd, /// 0xca8
			 0x42473cc1, /// 0xcac
			 0x11d4b038, /// 0xcb0
			 0x7281f93d, /// 0xcb4
			 0x561ee17e, /// 0xcb8
			 0xe3c189ad, /// 0xcbc
			 0xb91405e3, /// 0xcc0
			 0x324676ff, /// 0xcc4
			 0xcb787bae, /// 0xcc8
			 0x7b16fed8, /// 0xccc
			 0x38306706, /// 0xcd0
			 0xd25f1b01, /// 0xcd4
			 0x6d7b6686, /// 0xcd8
			 0xdc9197b4, /// 0xcdc
			 0xf7fe199c, /// 0xce0
			 0x00cbd326, /// 0xce4
			 0xb42ea4a7, /// 0xce8
			 0x9a66965d, /// 0xcec
			 0x0c6e1eeb, /// 0xcf0
			 0x47ec3029, /// 0xcf4
			 0x50f38bb5, /// 0xcf8
			 0x1b3fab43, /// 0xcfc
			 0x45e6217b, /// 0xd00
			 0xa35d58f8, /// 0xd04
			 0x4764b4b3, /// 0xd08
			 0x9927b652, /// 0xd0c
			 0x85416d04, /// 0xd10
			 0x225718ee, /// 0xd14
			 0xac8eb662, /// 0xd18
			 0xd731d409, /// 0xd1c
			 0xf1732b76, /// 0xd20
			 0x6a13b863, /// 0xd24
			 0x0e105db0, /// 0xd28
			 0xf6f753c0, /// 0xd2c
			 0x7f2eefce, /// 0xd30
			 0x6ba88e48, /// 0xd34
			 0x912adf25, /// 0xd38
			 0x8a6c682a, /// 0xd3c
			 0xf544e936, /// 0xd40
			 0xe591f9f9, /// 0xd44
			 0x422e0420, /// 0xd48
			 0x97f39b0f, /// 0xd4c
			 0xacf57cab, /// 0xd50
			 0xf5227acb, /// 0xd54
			 0x5666ece8, /// 0xd58
			 0xf9d75382, /// 0xd5c
			 0x664df8cb, /// 0xd60
			 0x319b48ea, /// 0xd64
			 0x55717377, /// 0xd68
			 0xc30f286c, /// 0xd6c
			 0xb5d82faf, /// 0xd70
			 0xcf2a13c7, /// 0xd74
			 0x4238717f, /// 0xd78
			 0x001bd000, /// 0xd7c
			 0x84043fff, /// 0xd80
			 0xa1a80b8b, /// 0xd84
			 0xa09a3f3f, /// 0xd88
			 0x214f8f24, /// 0xd8c
			 0xd04da306, /// 0xd90
			 0x14ee2b0a, /// 0xd94
			 0x59811fe1, /// 0xd98
			 0x8b1b019b, /// 0xd9c
			 0x81a9dd16, /// 0xda0
			 0xd327dfd8, /// 0xda4
			 0x6ae8d979, /// 0xda8
			 0x0fc99bdb, /// 0xdac
			 0x3612a863, /// 0xdb0
			 0xb233e8de, /// 0xdb4
			 0xa04f20c2, /// 0xdb8
			 0x56b992b8, /// 0xdbc
			 0x4a71a34b, /// 0xdc0
			 0xa2337611, /// 0xdc4
			 0x7b760538, /// 0xdc8
			 0xe18fd5ce, /// 0xdcc
			 0x91b217c3, /// 0xdd0
			 0x78279554, /// 0xdd4
			 0x7b99fad9, /// 0xdd8
			 0x11047fb1, /// 0xddc
			 0x52c73bbb, /// 0xde0
			 0x74aa5f7c, /// 0xde4
			 0x0c19c178, /// 0xde8
			 0x5729b7de, /// 0xdec
			 0x2bf4aa23, /// 0xdf0
			 0xfda3af60, /// 0xdf4
			 0x4126faef, /// 0xdf8
			 0x391ad72e, /// 0xdfc
			 0x6d8efc69, /// 0xe00
			 0xab015328, /// 0xe04
			 0x46888083, /// 0xe08
			 0xd8a8038a, /// 0xe0c
			 0x97314349, /// 0xe10
			 0xef9bba89, /// 0xe14
			 0x30330ef8, /// 0xe18
			 0x63a9ec2e, /// 0xe1c
			 0x390cf15e, /// 0xe20
			 0xe50154ff, /// 0xe24
			 0xf86a7225, /// 0xe28
			 0xf8d173db, /// 0xe2c
			 0x4ee1d75f, /// 0xe30
			 0x742da6df, /// 0xe34
			 0xc2d59aa3, /// 0xe38
			 0xb8924cdc, /// 0xe3c
			 0xf95e4387, /// 0xe40
			 0x908b0d1e, /// 0xe44
			 0xd55a5c63, /// 0xe48
			 0x612eb77f, /// 0xe4c
			 0xf3b6a969, /// 0xe50
			 0xc48c03ef, /// 0xe54
			 0x45cbf031, /// 0xe58
			 0x8c112c4e, /// 0xe5c
			 0x85efa6bf, /// 0xe60
			 0xb86a0902, /// 0xe64
			 0xb48e68c5, /// 0xe68
			 0x24328468, /// 0xe6c
			 0x5efd1c93, /// 0xe70
			 0x3ed743f4, /// 0xe74
			 0xb3b37b4b, /// 0xe78
			 0x7b5d206e, /// 0xe7c
			 0xcaa3f857, /// 0xe80
			 0x7cb3aadd, /// 0xe84
			 0x06397209, /// 0xe88
			 0x7f123699, /// 0xe8c
			 0xe3fa8d46, /// 0xe90
			 0x7ee8107f, /// 0xe94
			 0x796222ad, /// 0xe98
			 0x9e8f001e, /// 0xe9c
			 0xeb6b300b, /// 0xea0
			 0xb1026fd8, /// 0xea4
			 0x72d29b8c, /// 0xea8
			 0xd7748644, /// 0xeac
			 0x3b59c8e1, /// 0xeb0
			 0x7657e11a, /// 0xeb4
			 0xa007d11c, /// 0xeb8
			 0x2efcf152, /// 0xebc
			 0x6b6a4658, /// 0xec0
			 0xabb40aaf, /// 0xec4
			 0xfa7066d1, /// 0xec8
			 0x9afb0735, /// 0xecc
			 0xfda148e6, /// 0xed0
			 0xb6cf5294, /// 0xed4
			 0xc3ed719f, /// 0xed8
			 0x79437cf8, /// 0xedc
			 0x1bedfdfd, /// 0xee0
			 0x85ba9fef, /// 0xee4
			 0x1230d3ef, /// 0xee8
			 0xbf63d362, /// 0xeec
			 0x3501c3f7, /// 0xef0
			 0x80ed10ba, /// 0xef4
			 0x265b0b1c, /// 0xef8
			 0x2cd5467c, /// 0xefc
			 0x0dff77c5, /// 0xf00
			 0x016ac486, /// 0xf04
			 0x49825d0a, /// 0xf08
			 0xaed413ed, /// 0xf0c
			 0x637072a2, /// 0xf10
			 0x1cb89026, /// 0xf14
			 0x88303cec, /// 0xf18
			 0x3b5f53fc, /// 0xf1c
			 0x03917ae7, /// 0xf20
			 0xd777041a, /// 0xf24
			 0xd274f8c5, /// 0xf28
			 0x1ef7a7b4, /// 0xf2c
			 0x3eb70906, /// 0xf30
			 0x0efccbd1, /// 0xf34
			 0x55fe371f, /// 0xf38
			 0xaefd3db4, /// 0xf3c
			 0x29d0d3a2, /// 0xf40
			 0x019ebfff, /// 0xf44
			 0xbd517b55, /// 0xf48
			 0x9f1fc6fe, /// 0xf4c
			 0x133652a8, /// 0xf50
			 0x4a0c08ef, /// 0xf54
			 0x2a729869, /// 0xf58
			 0x4b04c61d, /// 0xf5c
			 0xa0a431ee, /// 0xf60
			 0xe016d0a9, /// 0xf64
			 0x748b8de7, /// 0xf68
			 0xd501b65e, /// 0xf6c
			 0x7965673b, /// 0xf70
			 0x6229dc88, /// 0xf74
			 0x58d3da92, /// 0xf78
			 0xd472bdc6, /// 0xf7c
			 0x1ed85726, /// 0xf80
			 0x149ebf9e, /// 0xf84
			 0xb9202f6e, /// 0xf88
			 0x3b74394e, /// 0xf8c
			 0xe3bf5aa3, /// 0xf90
			 0xe208c2a6, /// 0xf94
			 0x6404d309, /// 0xf98
			 0x709142ff, /// 0xf9c
			 0x5f98ad15, /// 0xfa0
			 0x7ddb76af, /// 0xfa4
			 0x9ecd4d65, /// 0xfa8
			 0xf323732c, /// 0xfac
			 0x2a607c23, /// 0xfb0
			 0xe66710e7, /// 0xfb4
			 0xec669e24, /// 0xfb8
			 0xac4fe75e, /// 0xfbc
			 0xad8b41db, /// 0xfc0
			 0xcb0e4486, /// 0xfc4
			 0x1ba925ef, /// 0xfc8
			 0x832c01ea, /// 0xfcc
			 0xba97d9a7, /// 0xfd0
			 0x4fffc37e, /// 0xfd4
			 0x407538af, /// 0xfd8
			 0xa6bfc032, /// 0xfdc
			 0xed6a5a05, /// 0xfe0
			 0x130fa284, /// 0xfe4
			 0xc977ac14, /// 0xfe8
			 0x242e1a6d, /// 0xfec
			 0x1e721620, /// 0xff0
			 0xe99e4c78, /// 0xff4
			 0x7f12e2d3, /// 0xff8
			 0x5cd2679c /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00004
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00008
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x00000000,                                                  // zero                                        /// 00014
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00018
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0001c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00020
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00028
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0002c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00034
			 0xff800000,                                                  // -inf                                        /// 00038
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0003c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0004c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00050
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x00000000,                                                  // zero                                        /// 0005c
			 0x55555555,                                                  // 4 random values                             /// 00060
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00064
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x00011111,                                                  // 9.7958E-41                                  /// 0006c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00070
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00074
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0007c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00080
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00088
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0008c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00090
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00094
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00098
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0009c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000a8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000ac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000d4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000e8
			 0x80011111,                                                  // -9.7958E-41                                 /// 000ec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 000f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00104
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0010c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00110
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00118
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x7fc00001,                                                  // signaling NaN                               /// 00130
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00134
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0013c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00140
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00148
			 0x0c780000,                                                  // Leading 1s:                                 /// 0014c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00154
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00158
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0015c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00160
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00164
			 0xbf028f5c,                                                  // -0.51                                       /// 00168
			 0xbf028f5c,                                                  // -0.51                                       /// 0016c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00170
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00174
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0017c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00180
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00184
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00190
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00194
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00198
			 0x0e000001,                                                  // Trailing 1s:                                /// 0019c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001a8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001b8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001c8
			 0x00000000,                                                  // zero                                        /// 001cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001d0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001d8
			 0xffc00001,                                                  // -signaling NaN                              /// 001dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001e0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x00000000,                                                  // zero                                        /// 001ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00204
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00208
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0020c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00210
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00214
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00218
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00224
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00228
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00230
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00234
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00238
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0023c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00248
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0024c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00254
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00258
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0025c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00260
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00268
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x33333333,                                                  // 4 random values                             /// 00270
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00274
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00278
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0027c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x00000000,                                                  // zero                                        /// 00284
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00288
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0028c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00294
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0029c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002a4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002a8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002b0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002d0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0xbf028f5c,                                                  // -0.51                                       /// 002dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002e0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002e4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00300
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00308
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0030c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00310
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0031c
			 0x00000000,                                                  // zero                                        /// 00320
			 0xbf028f5c,                                                  // -0.51                                       /// 00324
			 0x0c400000,                                                  // Leading 1s:                                 /// 00328
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0032c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00330
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00334
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0033c
			 0x4b000000,                                                  // 8388608.0                                   /// 00340
			 0x3f028f5c,                                                  // 0.51                                        /// 00344
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00348
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00350
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00354
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00358
			 0xcb000000,                                                  // -8388608.0                                  /// 0035c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00360
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00364
			 0x0e000003,                                                  // Trailing 1s:                                /// 00368
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0036c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00370
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00374
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00378
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00384
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0038c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00394
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00398
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0039c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003a0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003a8
			 0xffc00001,                                                  // -signaling NaN                              /// 003ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003c4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x80011111,                                                  // -9.7958E-41                                 /// 003e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00404
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00418
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0041c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00424
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00428
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0042c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00430
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00438
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0043c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00440
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00444
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0044c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00454
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00458
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0045c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00460
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00464
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00468
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0046c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00470
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00474
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00478
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0047c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00480
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00484
			 0x0c400000,                                                  // Leading 1s:                                 /// 00488
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0048c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00490
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00498
			 0x00011111,                                                  // 9.7958E-41                                  /// 0049c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004d4
			 0x55555555,                                                  // 4 random values                             /// 004d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00504
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00508
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0050c
			 0x55555555,                                                  // 4 random values                             /// 00510
			 0x0c700000,                                                  // Leading 1s:                                 /// 00514
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00518
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00520
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00524
			 0x0c780000,                                                  // Leading 1s:                                 /// 00528
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0052c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00530
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00534
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00538
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0053c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00540
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00548
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00550
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00554
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00558
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00560
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00564
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00568
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0056c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00570
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00574
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00578
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0057c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00580
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00588
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0058c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00590
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00594
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00598
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0059c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005b8
			 0x00000000,                                                  // zero                                        /// 005bc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x33333333,                                                  // 4 random values                             /// 005e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00600
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00604
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0060c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00614
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00628
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0062c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00630
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00634
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00638
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00648
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0064c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00654
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00658
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00660
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00664
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00668
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x33333333,                                                  // 4 random values                             /// 00670
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00674
			 0x0e000007,                                                  // Trailing 1s:                                /// 00678
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0067c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00684
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00688
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00694
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00698
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0069c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006b0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006b4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0xbf028f5c,                                                  // -0.51                                       /// 006d4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006dc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006e4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006e8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x0c780000,                                                  // Leading 1s:                                 /// 006f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00700
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0070c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00710
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00714
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00718
			 0x33333333,                                                  // 4 random values                             /// 0071c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00724
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00728
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0072c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00738
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00740
			 0x00000000,                                                  // zero                                        /// 00744
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00748
			 0x0c780000,                                                  // Leading 1s:                                 /// 0074c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00750
			 0x80000000,                                                  // -zero                                       /// 00754
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00760
			 0x0e000007,                                                  // Trailing 1s:                                /// 00764
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00768
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0076c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x0c780000,                                                  // Leading 1s:                                 /// 00774
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00778
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0077c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00780
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00784
			 0x0c400000,                                                  // Leading 1s:                                 /// 00788
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0078c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00790
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0079c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007a4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007cc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007d4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007ec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007f4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007f8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00804
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00808
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0080c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00810
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00818
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0081c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00824
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00828
			 0x3f028f5c,                                                  // 0.51                                        /// 0082c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00834
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00838
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0083c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00844
			 0x33333333,                                                  // 4 random values                             /// 00848
			 0x80011111,                                                  // -9.7958E-41                                 /// 0084c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00850
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00854
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00858
			 0xbf028f5c,                                                  // -0.51                                       /// 0085c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00864
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x33333333,                                                  // 4 random values                             /// 0086c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00874
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00878
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0087c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0088c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00890
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00894
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00898
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0089c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008a8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008c0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008c4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008cc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008dc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008e4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008e8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008f0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00900
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00904
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00908
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0090c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00914
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00918
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0091c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00920
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00924
			 0x00000000,                                                  // zero                                        /// 00928
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0092c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00930
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0093c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00940
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00944
			 0x7fc00001,                                                  // signaling NaN                               /// 00948
			 0x3f028f5c,                                                  // 0.51                                        /// 0094c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00950
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00954
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00958
			 0x0c700000,                                                  // Leading 1s:                                 /// 0095c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00960
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00964
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00968
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0096c
			 0xff800000,                                                  // -inf                                        /// 00970
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00974
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00978
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0097c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00980
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00984
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00988
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0098c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00990
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00994
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0099c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0x80000000,                                                  // -zero                                       /// 009a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009bc
			 0xcb000000,                                                  // -8388608.0                                  /// 009c0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009c8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009d4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009d8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009e4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x3f028f5c,                                                  // 0.51                                        /// 009f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a00
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a08
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a0c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a10
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a14
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a1c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a20
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a28
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a2c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a34
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a40
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a44
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a4c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a50
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a54
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a5c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a60
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a64
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a6c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a70
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a74
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a7c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a80
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a88
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x3f028f5c,                                                  // 0.51                                        /// 00a94
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a9c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00aa0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00aa4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00aac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ab0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ab8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00abc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ac0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ac4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ac8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00acc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ad4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ad8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00adc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ae0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ae8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00af4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b00
			 0x7f800000,                                                  // inf                                         /// 00b04
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0x7fc00001,                                                  // signaling NaN                               /// 00b0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b10
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b14
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b1c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b20
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b24
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b28
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b2c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b34
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b38
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b3c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b40
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b48
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b4c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b50
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b54
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b58
			 0xffc00001,                                                  // -signaling NaN                              /// 00b5c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b60
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b64
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b68
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b6c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b70
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b7c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b80
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b84
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b88
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b94
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b9c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ba0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ba4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ba8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bb0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bbc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bc0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bc4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bc8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bd0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0xbf028f5c,                                                  // -0.51                                       /// 00bd8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bdc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00be0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00be4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00be8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bf0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bf4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bfc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c00
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c0c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c10
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c18
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c1c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c20
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c24
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c28
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c2c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c34
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c3c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c40
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c44
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c48
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c4c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c50
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c58
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c5c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c6c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c70
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c74
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c7c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c80
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c84
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c98
			 0x33333333,                                                  // 4 random values                             /// 00c9c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ca0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ca8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cb0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cb8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cbc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cc0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ccc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cd0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cd4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ce0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cec
			 0xcb000000,                                                  // -8388608.0                                  /// 00cf0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cf4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cf8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d08
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d0c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d10
			 0x80000000,                                                  // -zero                                       /// 00d14
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0xffc00001,                                                  // -signaling NaN                              /// 00d1c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d20
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d28
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d2c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d34
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d38
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d3c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d44
			 0x33333333,                                                  // 4 random values                             /// 00d48
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d4c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x3f028f5c,                                                  // 0.51                                        /// 00d54
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d64
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d70
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d74
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d78
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d80
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d84
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d8c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d90
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d9c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00da0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00da4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00db0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00db4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dbc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dc0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dc4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00dc8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dd8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00de0
			 0xffc00001,                                                  // -signaling NaN                              /// 00de4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00de8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00df0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00df4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00df8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dfc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e00
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x7fc00001,                                                  // signaling NaN                               /// 00e08
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e10
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x00000000,                                                  // zero                                        /// 00e18
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e20
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e28
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e34
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e3c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e40
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e48
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e4c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e50
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e5c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e64
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e6c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e70
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e78
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e7c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e84
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e88
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e90
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e94
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e9c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ea0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ea4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ea8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00eac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00eb0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00eb8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ebc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ec0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ec4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ec8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ed4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ee0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ee4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ee8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ef0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ef4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ef8
			 0x3f028f5c,                                                  // 0.51                                        /// 00efc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f00
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f08
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f0c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f14
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f1c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f20
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f2c
			 0x00000000,                                                  // zero                                        /// 00f30
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f34
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f38
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f40
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f44
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f4c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f50
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f5c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f64
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f68
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f6c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f70
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x4b000000,                                                  // 8388608.0                                   /// 00f78
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f80
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f84
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f88
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f8c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f90
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f98
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f9c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fa0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fa4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fa8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fb8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fbc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fc4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fc8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fcc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fd0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fd4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fd8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x4b000000,                                                  // 8388608.0                                   /// 00fe0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fe4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fec
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ff0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ff4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ff8
			 0x0c7ffff0                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00000
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00004
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00010
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00018
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0001c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00020
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00024
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00028
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00030
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00034
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0003c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00040
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00044
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00048
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0004c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00050
			 0x4b000000,                                                  // 8388608.0                                   /// 00054
			 0x80011111,                                                  // -9.7958E-41                                 /// 00058
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00064
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0006c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00070
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00074
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0007c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00080
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00084
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00088
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0008c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00090
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00094
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00098
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000b4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000bc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000c8
			 0xffc00001,                                                  // -signaling NaN                              /// 000cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 000d8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000dc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000e4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00100
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00104
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00108
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0010c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0011c
			 0xcb000000,                                                  // -8388608.0                                  /// 00120
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00124
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00130
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00138
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0013c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00140
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00144
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00148
			 0xffc00001,                                                  // -signaling NaN                              /// 0014c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00150
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00154
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00158
			 0x0c700000,                                                  // Leading 1s:                                 /// 0015c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00160
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00164
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00168
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00170
			 0x80011111,                                                  // -9.7958E-41                                 /// 00174
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00178
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0017c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00184
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x0c700000,                                                  // Leading 1s:                                 /// 0018c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00190
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00194
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00198
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0019c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001cc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001d4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001ec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 001f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001fc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x4b000000,                                                  // 8388608.0                                   /// 00204
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00208
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00214
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00218
			 0x00000000,                                                  // zero                                        /// 0021c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0022c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00230
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00238
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00240
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x0e000001,                                                  // Trailing 1s:                                /// 00248
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0024c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00250
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0025c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00260
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00264
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00268
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0026c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00274
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00278
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0027c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00280
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00284
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00288
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0028c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00290
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0029c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002a8
			 0x00011111,                                                  // 9.7958E-41                                  /// 002ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002b8
			 0x80011111,                                                  // -9.7958E-41                                 /// 002bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 002d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002d8
			 0x33333333,                                                  // 4 random values                             /// 002dc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002e0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002e4
			 0x80000000,                                                  // -zero                                       /// 002e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002f4
			 0xff800000,                                                  // -inf                                        /// 002f8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002fc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00300
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x00011111,                                                  // 9.7958E-41                                  /// 00308
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0030c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00310
			 0x7f800000,                                                  // inf                                         /// 00314
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00318
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0031c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00320
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0032c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00330
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00338
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00340
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00344
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00348
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00350
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00354
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00358
			 0x80011111,                                                  // -9.7958E-41                                 /// 0035c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00364
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00368
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0036c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00370
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00374
			 0x4b000000,                                                  // 8388608.0                                   /// 00378
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00380
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00384
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00388
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0038c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00390
			 0x80011111,                                                  // -9.7958E-41                                 /// 00394
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0039c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003a8
			 0x7f800000,                                                  // inf                                         /// 003ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003c4
			 0x4b000000,                                                  // 8388608.0                                   /// 003c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 003ec
			 0x4b000000,                                                  // 8388608.0                                   /// 003f0
			 0x4b000000,                                                  // 8388608.0                                   /// 003f4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003f8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00400
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00404
			 0x7f800000,                                                  // inf                                         /// 00408
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0040c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00410
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00414
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00418
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00420
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00424
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00428
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00430
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00434
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00438
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0043c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00440
			 0x7f800000,                                                  // inf                                         /// 00444
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0044c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00450
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00454
			 0x0c400000,                                                  // Leading 1s:                                 /// 00458
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00464
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00468
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0046c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00470
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00474
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0047c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00480
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00484
			 0x55555555,                                                  // 4 random values                             /// 00488
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00490
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0049c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 004a8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004ac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 004b8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004bc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004c0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004c8
			 0xbf028f5c,                                                  // -0.51                                       /// 004cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004e8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004ec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004f8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00500
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x0e000007,                                                  // Trailing 1s:                                /// 00508
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x0e000001,                                                  // Trailing 1s:                                /// 00518
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0051c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00524
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00528
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0052c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00530
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00534
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0053c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00540
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00544
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0054c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00550
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00554
			 0xff800000,                                                  // -inf                                        /// 00558
			 0xbf028f5c,                                                  // -0.51                                       /// 0055c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00564
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0xffc00001,                                                  // -signaling NaN                              /// 0056c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0xffc00001,                                                  // -signaling NaN                              /// 00574
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0057c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00580
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00584
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00588
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0058c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00590
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00594
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00598
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0059c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005a4
			 0x80000000,                                                  // -zero                                       /// 005a8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005ac
			 0x80011111,                                                  // -9.7958E-41                                 /// 005b0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005b4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 005cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005d0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 005e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 005e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x55555555,                                                  // 4 random values                             /// 005f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00600
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00604
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00610
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00620
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00624
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00628
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0062c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00630
			 0x00011111,                                                  // 9.7958E-41                                  /// 00634
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00638
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0063c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0064c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00654
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00658
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0065c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x0e000007,                                                  // Trailing 1s:                                /// 00664
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00668
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00670
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00674
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00678
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0067c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00680
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00688
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0068c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00690
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00694
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00698
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006a4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006b0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006b8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006c0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0xbf028f5c,                                                  // -0.51                                       /// 006cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006e8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006ec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006f8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00700
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00704
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0070c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00714
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00718
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00720
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00724
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00728
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0072c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00734
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0073c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00740
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0074c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00750
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00758
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0075c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00760
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00764
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00768
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0076c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00770
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00774
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0077c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00784
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0078c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00790
			 0x7f800000,                                                  // inf                                         /// 00794
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00798
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0079c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007b0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007bc
			 0x7f800000,                                                  // inf                                         /// 007c0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007c8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007cc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0xff800000,                                                  // -inf                                        /// 007d4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007f8
			 0x0e000003,                                                  // Trailing 1s:                                /// 007fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00800
			 0x4b000000,                                                  // 8388608.0                                   /// 00804
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00808
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00810
			 0x0e000007,                                                  // Trailing 1s:                                /// 00814
			 0x7fc00001,                                                  // signaling NaN                               /// 00818
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0081c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00820
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00824
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00828
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0082c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00834
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0xffc00001,                                                  // -signaling NaN                              /// 0083c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00844
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0084c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00854
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00858
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0085c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00860
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00868
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0086c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00878
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0087c
			 0xff800000,                                                  // -inf                                        /// 00880
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00884
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00888
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x7f800000,                                                  // inf                                         /// 00894
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00898
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0089c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008cc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008e4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 008ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008f8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008fc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00904
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00908
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00910
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00914
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0091c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00930
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00938
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0093c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00940
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00944
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00948
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0094c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00954
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00958
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0095c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0096c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00978
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0097c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00980
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0xffc00001,                                                  // -signaling NaN                              /// 00988
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0098c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00990
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00994
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0099c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009a0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009a4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009a8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 009b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009c8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009e0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009e4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009ec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009f4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a00
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a04
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a0c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a18
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a20
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a24
			 0xff800000,                                                  // -inf                                        /// 00a28
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a2c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a30
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a34
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a3c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a40
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a44
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a48
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a4c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a50
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a54
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x55555555,                                                  // 4 random values                             /// 00a5c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a60
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a64
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a68
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a6c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a74
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a78
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a7c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a80
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a84
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a8c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a94
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a9c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00aa0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00aa8
			 0x00000000,                                                  // zero                                        /// 00aac
			 0x4b000000,                                                  // 8388608.0                                   /// 00ab0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ab4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00abc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ac0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ac8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00acc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ad0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ad4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ad8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00adc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ae0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ae4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ae8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00aec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00af4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x7fc00001,                                                  // signaling NaN                               /// 00b00
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b04
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b08
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b10
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b14
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b18
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b1c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b20
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b24
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b28
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b2c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b34
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x55555555,                                                  // 4 random values                             /// 00b3c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b40
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b44
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b48
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b4c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b5c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b60
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b68
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b6c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b70
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b74
			 0x80000000,                                                  // -zero                                       /// 00b78
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b7c
			 0xbf028f5c,                                                  // -0.51                                       /// 00b80
			 0x55555555,                                                  // 4 random values                             /// 00b84
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0xcb000000,                                                  // -8388608.0                                  /// 00b90
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b94
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b98
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ba0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ba4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x4b000000,                                                  // 8388608.0                                   /// 00bac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bb0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bb4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bb8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bc0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bc4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bc8
			 0x00000000,                                                  // zero                                        /// 00bcc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bd0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bd4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bd8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bdc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00be0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00be4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00be8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bf4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bf8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bfc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c00
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c08
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c18
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c20
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c28
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c30
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c34
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c3c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c40
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c44
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c4c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c50
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c58
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c64
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c68
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c6c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c70
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c74
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c78
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c7c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c80
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x80000000,                                                  // -zero                                       /// 00c88
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c90
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c98
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ca8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cb0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cbc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cc0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cc4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cc8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ccc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cd0
			 0xff800000,                                                  // -inf                                        /// 00cd4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cdc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ce0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ce4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ce8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cf4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cfc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d08
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d10
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d14
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d18
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d1c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d20
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d24
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d28
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d2c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d30
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d34
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d38
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d3c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d40
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d48
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d50
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d58
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d60
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d64
			 0x3f028f5c,                                                  // 0.51                                        /// 00d68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d70
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d7c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d80
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d84
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d88
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d94
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d98
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d9c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00da4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00db4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00db8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dc0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dc4
			 0x33333333,                                                  // 4 random values                             /// 00dc8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dcc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dd0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dd8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ddc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00de0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00de4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00de8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00df0
			 0x80000000,                                                  // -zero                                       /// 00df4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00df8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dfc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e00
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e08
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e0c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e14
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e1c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e24
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e2c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e38
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e3c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e44
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e4c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e50
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e54
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e58
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e5c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e60
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e64
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e6c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e70
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e74
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e7c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e80
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e84
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e94
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e9c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ea0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ea4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00eb0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00eb4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00eb8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ebc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ec0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ec4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ec8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ecc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00edc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ee0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0xff800000,                                                  // -inf                                        /// 00ee8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ef0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ef4
			 0x7f800000,                                                  // inf                                         /// 00ef8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00efc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f00
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f04
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f08
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f0c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f10
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f14
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f18
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f1c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f2c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f30
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f34
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f38
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f3c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f44
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f4c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f50
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f54
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f58
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f60
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f68
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f6c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f7c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f88
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f90
			 0x80000000,                                                  // -zero                                       /// 00f94
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f98
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fa4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fa8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fb0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fb8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fbc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fc8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x4b000000,                                                  // 8388608.0                                   /// 00fd4
			 0x80000000,                                                  // -zero                                       /// 00fd8
			 0x00000000,                                                  // zero                                        /// 00fdc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fe0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fe4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fe8
			 0x3f028f5c,                                                  // 0.51                                        /// 00fec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ff0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ff8
			 0x80008000                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00000
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00004
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00008
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00010
			 0x55555555,                                                  // 4 random values                             /// 00014
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00018
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0001c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00020
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0002c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00030
			 0x0c700000,                                                  // Leading 1s:                                 /// 00034
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0003c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00040
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00044
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00048
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0004c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00050
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00054
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00058
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0005c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00064
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0006c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00070
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00074
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00078
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0007c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00080
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00084
			 0x55555555,                                                  // 4 random values                             /// 00088
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00090
			 0x0c600000,                                                  // Leading 1s:                                 /// 00094
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000a0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000a4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000b4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0xff800000,                                                  // -inf                                        /// 000c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x80011111,                                                  // -9.7958E-41                                 /// 000dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000ec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000f8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00100
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00108
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0010c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00110
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00114
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00118
			 0x00011111,                                                  // 9.7958E-41                                  /// 0011c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00120
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00124
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00128
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0012c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00130
			 0x0c780000,                                                  // Leading 1s:                                 /// 00134
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00138
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0013c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00140
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00144
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00148
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0015c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00160
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00168
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0016c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00174
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00178
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0017c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00180
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00184
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0018c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00190
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00194
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00198
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0019c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001b8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x00000000,                                                  // zero                                        /// 001c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 001d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001e8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001f4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001f8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00200
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00208
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0020c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00210
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0021c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00220
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00224
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00228
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0022c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0xffc00001,                                                  // -signaling NaN                              /// 00234
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0023c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00244
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00248
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0024c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00250
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00254
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00258
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00260
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00264
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00268
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0026c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00270
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00280
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00284
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0028c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00290
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00294
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0029c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002a8
			 0x00000000,                                                  // zero                                        /// 002ac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002c0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002c4
			 0x80000000,                                                  // -zero                                       /// 002c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002dc
			 0x33333333,                                                  // 4 random values                             /// 002e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 002e4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002f0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00304
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00308
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0030c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00310
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00318
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0031c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00320
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00324
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0032c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00330
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0033c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00340
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00350
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00354
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0035c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00360
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00364
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0036c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00370
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00374
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00378
			 0x0c780000,                                                  // Leading 1s:                                 /// 0037c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0xbf028f5c,                                                  // -0.51                                       /// 00384
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00388
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0038c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00394
			 0x80011111,                                                  // -9.7958E-41                                 /// 00398
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0039c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x3f028f5c,                                                  // 0.51                                        /// 003bc
			 0x80000000,                                                  // -zero                                       /// 003c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003d8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003dc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003e0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003ec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00400
			 0xbf028f5c,                                                  // -0.51                                       /// 00404
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0040c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00418
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0041c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00420
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00424
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00428
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0042c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00434
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0043c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00440
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00444
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0044c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00458
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0045c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00468
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0047c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00480
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00488
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00490
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00498
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 004a4
			 0xcb000000,                                                  // -8388608.0                                  /// 004a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0x7fc00001,                                                  // signaling NaN                               /// 004b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 004bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004c4
			 0xff800000,                                                  // -inf                                        /// 004c8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004d8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 004ec
			 0x0e000007,                                                  // Trailing 1s:                                /// 004f0
			 0x0c780000,                                                  // Leading 1s:                                 /// 004f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00508
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0050c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00510
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00514
			 0x55555555,                                                  // 4 random values                             /// 00518
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00520
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00528
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0052c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00530
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00534
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0053c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00540
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00548
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0054c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00550
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00554
			 0x00000000,                                                  // zero                                        /// 00558
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0055c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00560
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00564
			 0x0e000003,                                                  // Trailing 1s:                                /// 00568
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00570
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00578
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0057c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x00011111,                                                  // 9.7958E-41                                  /// 00584
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00588
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0058c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00590
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00594
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0059c
			 0x7f800000,                                                  // inf                                         /// 005a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 005a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 005b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 005c0
			 0x7f800000,                                                  // inf                                         /// 005c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 005c8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005e4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00600
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00608
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0060c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00610
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00614
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0061c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00620
			 0xbf028f5c,                                                  // -0.51                                       /// 00624
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00628
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0062c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00630
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x00000000,                                                  // zero                                        /// 00638
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00644
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0064c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00650
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00654
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00658
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0065c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00660
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00664
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00668
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00670
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00674
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0067c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00680
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00684
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00688
			 0x80000000,                                                  // -zero                                       /// 0068c
			 0xff800000,                                                  // -inf                                        /// 00690
			 0x4b000000,                                                  // 8388608.0                                   /// 00694
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00698
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0069c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x0e000007,                                                  // Trailing 1s:                                /// 006a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006c4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006e0
			 0x80000000,                                                  // -zero                                       /// 006e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006f4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006f8
			 0xbf028f5c,                                                  // -0.51                                       /// 006fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00710
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00714
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00718
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0071c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00720
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00724
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00728
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0072c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00734
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00738
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00740
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00744
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00748
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0074c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00754
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00758
			 0x3f028f5c,                                                  // 0.51                                        /// 0075c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00764
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00778
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x55555555,                                                  // 4 random values                             /// 00780
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00784
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00788
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0078c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00790
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00794
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0079c
			 0x0e000007,                                                  // Trailing 1s:                                /// 007a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007ac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007b8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007c4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007c8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 007d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007e8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007ec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x80000000,                                                  // -zero                                       /// 007f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00800
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00804
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00808
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00810
			 0x00000000,                                                  // zero                                        /// 00814
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00820
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00824
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0082c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00830
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00834
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00840
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00844
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00848
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0084c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00858
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0085c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00860
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00868
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0086c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00870
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00874
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0087c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00880
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00884
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0088c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00894
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00898
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008a0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008a4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008a8
			 0xffc00001,                                                  // -signaling NaN                              /// 008ac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x7f800000,                                                  // inf                                         /// 008bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008c4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008d4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008dc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x00000000,                                                  // zero                                        /// 008e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008ec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00900
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00904
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0090c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0xffc00001,                                                  // -signaling NaN                              /// 00914
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00918
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0091c
			 0x3f028f5c,                                                  // 0.51                                        /// 00920
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00924
			 0x7f800000,                                                  // inf                                         /// 00928
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0092c
			 0x7f800000,                                                  // inf                                         /// 00930
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0xbf028f5c,                                                  // -0.51                                       /// 00940
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00944
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0xbf028f5c,                                                  // -0.51                                       /// 0094c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00950
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00954
			 0x7fc00001,                                                  // signaling NaN                               /// 00958
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0095c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00960
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0096c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00970
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00974
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00978
			 0x7fc00001,                                                  // signaling NaN                               /// 0097c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00980
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00984
			 0xcb000000,                                                  // -8388608.0                                  /// 00988
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0098c
			 0x00000000,                                                  // zero                                        /// 00990
			 0x0c400000,                                                  // Leading 1s:                                 /// 00994
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00998
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0099c
			 0xbf028f5c,                                                  // -0.51                                       /// 009a0
			 0x4b000000,                                                  // 8388608.0                                   /// 009a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009ac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009b4
			 0xffc00001,                                                  // -signaling NaN                              /// 009b8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009c8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009d0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009d4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009dc
			 0x80011111,                                                  // -9.7958E-41                                 /// 009e0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009e4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009ec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009f0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a08
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a0c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a14
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a18
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a1c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a24
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a28
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a2c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x00000000,                                                  // zero                                        /// 00a44
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a4c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a50
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a58
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a60
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a64
			 0xff800000,                                                  // -inf                                        /// 00a68
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a6c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a70
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a74
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a78
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a80
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a88
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a8c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a94
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a9c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ab0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ab4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00abc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ac0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ac4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00acc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ad0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ad4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ad8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00adc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ae0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ae4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ae8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00aec
			 0x0e000007,                                                  // Trailing 1s:                                /// 00af0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00af4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00afc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b00
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b08
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b0c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b10
			 0x7f800000,                                                  // inf                                         /// 00b14
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b18
			 0xffc00001,                                                  // -signaling NaN                              /// 00b1c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b20
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b34
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b3c
			 0x3f028f5c,                                                  // 0.51                                        /// 00b40
			 0xcb000000,                                                  // -8388608.0                                  /// 00b44
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b48
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b4c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b54
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b58
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b60
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b68
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b6c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b70
			 0xff800000,                                                  // -inf                                        /// 00b74
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b78
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b7c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b80
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b84
			 0x80000000,                                                  // -zero                                       /// 00b88
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b8c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b90
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b94
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b98
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ba0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ba8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bb4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bb8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bbc
			 0xffc00001,                                                  // -signaling NaN                              /// 00bc0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bcc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bd0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bd4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00be0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00be8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bf0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bf8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c00
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c08
			 0xffc00001,                                                  // -signaling NaN                              /// 00c0c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c10
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c14
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0xbf028f5c,                                                  // -0.51                                       /// 00c1c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c24
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c28
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c30
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c34
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c38
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c3c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c44
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c48
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c4c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c50
			 0x33333333,                                                  // 4 random values                             /// 00c54
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c58
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c5c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c64
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c6c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c74
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c78
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c7c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c80
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c84
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c8c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c90
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c94
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c9c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ca0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ca4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ca8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cb0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cb4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cbc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cc0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cc4
			 0x33333333,                                                  // 4 random values                             /// 00cc8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cd4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cd8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cdc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ce0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ce4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cf0
			 0x33333333,                                                  // 4 random values                             /// 00cf4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x55555555,                                                  // 4 random values                             /// 00cfc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d00
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d04
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d08
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d0c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d10
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d18
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d1c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d24
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d28
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d30
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d34
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d38
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d3c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d40
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d44
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d48
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d4c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d50
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0xcb000000,                                                  // -8388608.0                                  /// 00d58
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d60
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d64
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d6c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d70
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d78
			 0xbf028f5c,                                                  // -0.51                                       /// 00d7c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d84
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d88
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d8c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0xbf028f5c,                                                  // -0.51                                       /// 00d94
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d9c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00da0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00da8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00db4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0xff800000,                                                  // -inf                                        /// 00dbc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dc0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dc8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dcc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dd0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dd4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ddc
			 0x55555555,                                                  // 4 random values                             /// 00de0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00de4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00de8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00df0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00df4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dfc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e04
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0x80000000,                                                  // -zero                                       /// 00e0c
			 0x3f028f5c,                                                  // 0.51                                        /// 00e10
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e18
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0xff800000,                                                  // -inf                                        /// 00e28
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e2c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e30
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0xbf028f5c,                                                  // -0.51                                       /// 00e3c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e40
			 0xff800000,                                                  // -inf                                        /// 00e44
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e4c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e50
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e54
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e58
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e5c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e68
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e6c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e70
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e74
			 0x7f800000,                                                  // inf                                         /// 00e78
			 0x55555555,                                                  // 4 random values                             /// 00e7c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e80
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e84
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e88
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e8c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e90
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e98
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ea4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00eb4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ebc
			 0x33333333,                                                  // 4 random values                             /// 00ec0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ec4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ec8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ecc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ed0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ed4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ed8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ee0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ee8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00eec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ef0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ef4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ef8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00efc
			 0x7f800000,                                                  // inf                                         /// 00f00
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f04
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f08
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f10
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f14
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f1c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f20
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f34
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f38
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f3c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f48
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f58
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f5c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f60
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f64
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f68
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f6c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f70
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f78
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f80
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f84
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f88
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f8c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f90
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f98
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f9c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fa0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fa8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fb0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fb4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fb8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fc4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fc8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fd0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fd4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fd8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fdc
			 0x4b000000,                                                  // 8388608.0                                   /// 00fe0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ff0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ff4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ff8
			 0x0c7ff800                                                  // Leading 1s:                                 /// last
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
			 0x000007a0,
			 0x000006cc,
			 0x000002b4,
			 0x00000138,
			 0x00000594,
			 0x000005e4,
			 0x000003b8,
			 0x000007bc,

			 /// vpu register f2
			 0x41b00000,
			 0x41a00000,
			 0x41200000,
			 0x41a80000,
			 0x42000000,
			 0x41500000,
			 0x41e80000,
			 0x41700000,

			 /// vpu register f3
			 0x40800000,
			 0x41c80000,
			 0x41f00000,
			 0x40c00000,
			 0x41a80000,
			 0x40000000,
			 0x41b00000,
			 0x40800000,

			 /// vpu register f4
			 0x41500000,
			 0x41980000,
			 0x41c00000,
			 0x41d80000,
			 0x41f00000,
			 0x41c80000,
			 0x41500000,
			 0x41f00000,

			 /// vpu register f5
			 0x40000000,
			 0x41d80000,
			 0x41c80000,
			 0x41400000,
			 0x41000000,
			 0x41800000,
			 0x40c00000,
			 0x41b00000,

			 /// vpu register f6
			 0x40c00000,
			 0x41b00000,
			 0x41200000,
			 0x41300000,
			 0x41f80000,
			 0x41e80000,
			 0x42000000,
			 0x41a80000,

			 /// vpu register f7
			 0x41700000,
			 0x41500000,
			 0x41700000,
			 0x41000000,
			 0x40800000,
			 0x41900000,
			 0x41b80000,
			 0x41000000,

			 /// vpu register f8
			 0x40800000,
			 0x40e00000,
			 0x40800000,
			 0x41880000,
			 0x41800000,
			 0x41e00000,
			 0x41100000,
			 0x41980000,

			 /// vpu register f9
			 0x41e80000,
			 0x41e00000,
			 0x41c80000,
			 0x41980000,
			 0x41d00000,
			 0x41300000,
			 0x40000000,
			 0x41600000,

			 /// vpu register f10
			 0x41b00000,
			 0x41880000,
			 0x41900000,
			 0x41100000,
			 0x41500000,
			 0x40800000,
			 0x41200000,
			 0x41a00000,

			 /// vpu register f11
			 0x41e00000,
			 0x40a00000,
			 0x41600000,
			 0x40400000,
			 0x41e00000,
			 0x3f800000,
			 0x41d00000,
			 0x41c00000,

			 /// vpu register f12
			 0x41d00000,
			 0x41500000,
			 0x41f00000,
			 0x41a80000,
			 0x41d00000,
			 0x41d80000,
			 0x41100000,
			 0x41f00000,

			 /// vpu register f13
			 0x41500000,
			 0x41c80000,
			 0x3f800000,
			 0x41f80000,
			 0x41800000,
			 0x41500000,
			 0x41a80000,
			 0x41e00000,

			 /// vpu register f14
			 0x41880000,
			 0x3f800000,
			 0x41000000,
			 0x41b00000,
			 0x41e80000,
			 0x41d00000,
			 0x41d00000,
			 0x40a00000,

			 /// vpu register f15
			 0x41f80000,
			 0x3f800000,
			 0x41a80000,
			 0x41300000,
			 0x40e00000,
			 0x40a00000,
			 0x41f00000,
			 0x41980000,

			 /// vpu register f16
			 0x40800000,
			 0x40c00000,
			 0x41000000,
			 0x41700000,
			 0x41880000,
			 0x41700000,
			 0x41b00000,
			 0x41400000,

			 /// vpu register f17
			 0x41c80000,
			 0x41700000,
			 0x41200000,
			 0x41500000,
			 0x42000000,
			 0x41200000,
			 0x41700000,
			 0x41b00000,

			 /// vpu register f18
			 0x41f00000,
			 0x41400000,
			 0x41000000,
			 0x41c00000,
			 0x42000000,
			 0x40400000,
			 0x41d00000,
			 0x40c00000,

			 /// vpu register f19
			 0x41a80000,
			 0x41d80000,
			 0x41800000,
			 0x3f800000,
			 0x40800000,
			 0x40a00000,
			 0x41f00000,
			 0x41d00000,

			 /// vpu register f20
			 0x41800000,
			 0x3f800000,
			 0x41800000,
			 0x41d80000,
			 0x41e80000,
			 0x42000000,
			 0x41e80000,
			 0x41880000,

			 /// vpu register f21
			 0x40e00000,
			 0x42000000,
			 0x41980000,
			 0x41700000,
			 0x41c80000,
			 0x41f00000,
			 0x41100000,
			 0x41c00000,

			 /// vpu register f22
			 0x41f80000,
			 0x42000000,
			 0x41c00000,
			 0x41880000,
			 0x40800000,
			 0x41500000,
			 0x41700000,
			 0x41300000,

			 /// vpu register f23
			 0x41c80000,
			 0x41300000,
			 0x40400000,
			 0x41f80000,
			 0x41d80000,
			 0x41f00000,
			 0x41f00000,
			 0x41300000,

			 /// vpu register f24
			 0x41e80000,
			 0x3f800000,
			 0x41800000,
			 0x41b00000,
			 0x40800000,
			 0x41f00000,
			 0x41800000,
			 0x40e00000,

			 /// vpu register f25
			 0x41600000,
			 0x41500000,
			 0x41200000,
			 0x40c00000,
			 0x41e80000,
			 0x40400000,
			 0x41400000,
			 0x41980000,

			 /// vpu register f26
			 0x41e00000,
			 0x41e80000,
			 0x41880000,
			 0x41d80000,
			 0x41c00000,
			 0x41100000,
			 0x41d80000,
			 0x41400000,

			 /// vpu register f27
			 0x40400000,
			 0x3f800000,
			 0x41600000,
			 0x41980000,
			 0x41980000,
			 0x41300000,
			 0x41200000,
			 0x41200000,

			 /// vpu register f28
			 0x41400000,
			 0x41300000,
			 0x41e80000,
			 0x41e80000,
			 0x40e00000,
			 0x41f80000,
			 0x41300000,
			 0x41980000,

			 /// vpu register f29
			 0x41400000,
			 0x3f800000,
			 0x41f80000,
			 0x40c00000,
			 0x40e00000,
			 0x41400000,
			 0x40a00000,
			 0x41980000,

			 /// vpu register f30
			 0x40c00000,
			 0x41e00000,
			 0x41500000,
			 0x41f80000,
			 0x41500000,
			 0x41700000,
			 0x42000000,
			 0x40e00000,

			 /// vpu register f31
			 0x41200000,
			 0x41e00000,
			 0x41000000,
			 0x41980000,
			 0x40000000,
			 0x41c00000,
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
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f21, f1 (x14)\n"                        ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f21, f1 (x12)\n"                        ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f28, f1 (x14)\n"                        ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f2, f1 (x11)\n"                         ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f15, f1 (x11)\n"                        ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f12, f1 (x13)\n"                        ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f17, f1 (x14)\n"                        ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f4, f1 (x15)\n"                         ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f13, f1 (x12)\n"                        ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f20, f1 (x12)\n"                        ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f20, f1 (x11)\n"                        ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f3, f1 (x13)\n"                         ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f11, f1 (x12)\n"                        ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f16, f1 (x11)\n"                        ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f21, f1 (x15)\n"                        ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f20, f1 (x14)\n"                        ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f3, f1 (x12)\n"                         ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f8, f1 (x14)\n"                         ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f20, f1 (x15)\n"                        ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f10, f1 (x12)\n"                        ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f23, f1 (x15)\n"                        ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f21, f1 (x12)\n"                        ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f10, f1 (x15)\n"                        ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f9, f1 (x14)\n"                         ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f16, f1 (x14)\n"                        ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f30, f1 (x15)\n"                        ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f29, f1 (x14)\n"                        ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f12, f1 (x14)\n"                        ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f13, f1 (x15)\n"                        ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f5, f1 (x13)\n"                         ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f19, f1 (x13)\n"                        ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f23, f1 (x12)\n"                        ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f28, f1 (x13)\n"                        ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f28, f1 (x14)\n"                        ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f18, f1 (x14)\n"                        ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f26, f1 (x12)\n"                        ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f29, f1 (x12)\n"                        ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f7, f1 (x14)\n"                         ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f15, f1 (x14)\n"                        ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f20, f1 (x13)\n"                        ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f27, f1 (x13)\n"                        ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f19, f1 (x14)\n"                        ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f28, f1 (x12)\n"                        ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f28, f1 (x14)\n"                        ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f14, f1 (x14)\n"                        ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f13, f1 (x14)\n"                        ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f15, f1 (x12)\n"                        ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f8, f1 (x13)\n"                         ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f6, f1 (x14)\n"                         ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f10, f1 (x12)\n"                        ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f14, f1 (x11)\n"                        ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f22, f1 (x13)\n"                        ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f2, f1 (x15)\n"                         ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f27, f1 (x13)\n"                        ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f19, f1 (x12)\n"                        ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f9, f1 (x14)\n"                         ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f3, f1 (x15)\n"                         ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f4, f1 (x13)\n"                         ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f3, f1 (x13)\n"                         ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f13, f1 (x14)\n"                        ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f9, f1 (x15)\n"                         ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f24, f1 (x14)\n"                        ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f17, f1 (x11)\n"                        ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f6, f1 (x13)\n"                         ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f20, f1 (x13)\n"                        ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f2, f1 (x15)\n"                         ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f10, f1 (x13)\n"                        ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f22, f1 (x13)\n"                        ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f26, f1 (x12)\n"                        ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f21, f1 (x13)\n"                        ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f10, f1 (x11)\n"                        ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f22, f1 (x13)\n"                        ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f20, f1 (x11)\n"                        ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f26, f1 (x15)\n"                        ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f11, f1 (x14)\n"                        ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f28, f1 (x13)\n"                        ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f5, f1 (x13)\n"                         ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f29, f1 (x14)\n"                        ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f11, f1 (x15)\n"                        ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f8, f1 (x14)\n"                         ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f24, f1 (x11)\n"                        ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f30, f1 (x12)\n"                        ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f14, f1 (x14)\n"                        ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f24, f1 (x12)\n"                        ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f20, f1 (x11)\n"                        ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f17, f1 (x15)\n"                        ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f12, f1 (x11)\n"                        ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f14, f1 (x12)\n"                        ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f29, f1 (x13)\n"                        ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f18, f1 (x12)\n"                        ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f7, f1 (x14)\n"                         ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f11, f1 (x14)\n"                        ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f9, f1 (x15)\n"                         ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f19, f1 (x13)\n"                        ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f12, f1 (x14)\n"                        ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f25, f1 (x15)\n"                        ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f25, f1 (x11)\n"                        ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f10, f1 (x11)\n"                        ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapl.pi f15, f1 (x13)\n"                        ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
