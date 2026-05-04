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
			 0x3f028f5c,                                                  // 0.51                                        /// 00000
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00004
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00008
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0000c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00010
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00014
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00018
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00020
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00024
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00038
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00048
			 0x7fc00001,                                                  // signaling NaN                               /// 0004c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00050
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00054
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00058
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00060
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00064
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00068
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0006c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00070
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00074
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00078
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0007c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00080
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00088
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0008c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00090
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00094
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000a0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 000b8
			 0x33333333,                                                  // 4 random values                             /// 000bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0xbf028f5c,                                                  // -0.51                                       /// 000c4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000cc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000d4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000ec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000f8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00100
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00104
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00108
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0010c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00114
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0011c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00128
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x33333333,                                                  // 4 random values                             /// 00134
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0013c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00140
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00144
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00148
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0014c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00150
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00158
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0015c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00160
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00164
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0016c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00170
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00174
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00188
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0018c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00194
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00198
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001a4
			 0xbf028f5c,                                                  // -0.51                                       /// 001a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x80000000,                                                  // -zero                                       /// 001c4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001cc
			 0xff800000,                                                  // -inf                                        /// 001d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001f4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00200
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0020c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00210
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00214
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00218
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0021c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00220
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00224
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00228
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0022c
			 0xcb000000,                                                  // -8388608.0                                  /// 00230
			 0x80011111,                                                  // -9.7958E-41                                 /// 00234
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0023c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00240
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00244
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00248
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0024c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00254
			 0x0c400000,                                                  // Leading 1s:                                 /// 00258
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0025c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00260
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00264
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0026c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00270
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00274
			 0x80011111,                                                  // -9.7958E-41                                 /// 00278
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00284
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00288
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00298
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0029c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002a4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002b0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x0e000007,                                                  // Trailing 1s:                                /// 002cc
			 0xbf028f5c,                                                  // -0.51                                       /// 002d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002d8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0xff800000,                                                  // -inf                                        /// 002f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00300
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00304
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00308
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0030c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00310
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00318
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0031c
			 0x3f028f5c,                                                  // 0.51                                        /// 00320
			 0x0e000003,                                                  // Trailing 1s:                                /// 00324
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00328
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00334
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0033c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x33333333,                                                  // 4 random values                             /// 00344
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00348
			 0x7f800000,                                                  // inf                                         /// 0034c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00350
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00358
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0035c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00360
			 0x0e000001,                                                  // Trailing 1s:                                /// 00364
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0036c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0037c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00380
			 0x80000000,                                                  // -zero                                       /// 00384
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00390
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00394
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003ac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003c8
			 0x55555555,                                                  // 4 random values                             /// 003cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 003f0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00400
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00408
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0040c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00410
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00418
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0041c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00420
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00424
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0042c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00434
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00438
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0043c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00440
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00444
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0044c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0045c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00464
			 0x3f028f5c,                                                  // 0.51                                        /// 00468
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0046c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00470
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00478
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0047c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00480
			 0x0c780000,                                                  // Leading 1s:                                 /// 00484
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00488
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0048c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00490
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00498
			 0x55555555,                                                  // 4 random values                             /// 0049c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004a0
			 0x00000000,                                                  // zero                                        /// 004a4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004ac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004b8
			 0xffc00001,                                                  // -signaling NaN                              /// 004bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 004c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 004c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 004d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004e0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004ec
			 0x00000000,                                                  // zero                                        /// 004f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00500
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00508
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0050c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00510
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00514
			 0xcb000000,                                                  // -8388608.0                                  /// 00518
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00520
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00524
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00528
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0052c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00530
			 0x0e000007,                                                  // Trailing 1s:                                /// 00534
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0053c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00540
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00544
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0xcb000000,                                                  // -8388608.0                                  /// 0054c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00550
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00554
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00558
			 0x4b000000,                                                  // 8388608.0                                   /// 0055c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00560
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00564
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00568
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00574
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0057c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00580
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00584
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0058c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x4b000000,                                                  // 8388608.0                                   /// 00594
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00598
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0059c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005a4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 005ac
			 0x3f028f5c,                                                  // 0.51                                        /// 005b0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005bc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005c0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005d0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00608
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00610
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00614
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00618
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0061c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00624
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00628
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00630
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00634
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0063c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00648
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0064c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00650
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00654
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00658
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0065c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00660
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00664
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0066c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00674
			 0x4b000000,                                                  // 8388608.0                                   /// 00678
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0068c
			 0xffc00001,                                                  // -signaling NaN                              /// 00690
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00694
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00698
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0069c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006c0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006d4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006e0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006e4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006e8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006f0
			 0x7f800000,                                                  // inf                                         /// 006f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006f8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00704
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00714
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00718
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0071c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00728
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00734
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0073c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00740
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00744
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00748
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0074c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00750
			 0x0e000007,                                                  // Trailing 1s:                                /// 00754
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00758
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0075c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00764
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00778
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00780
			 0x7fc00001,                                                  // signaling NaN                               /// 00784
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00788
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0078c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00798
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0079c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007c4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007dc
			 0x33333333,                                                  // 4 random values                             /// 007e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007ec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00808
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0080c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00810
			 0x0e000001,                                                  // Trailing 1s:                                /// 00814
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00818
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00824
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0xff800000,                                                  // -inf                                        /// 0082c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00830
			 0x55555555,                                                  // 4 random values                             /// 00834
			 0x00011111,                                                  // 9.7958E-41                                  /// 00838
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00840
			 0x0e000007,                                                  // Trailing 1s:                                /// 00844
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00848
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0084c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00850
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00858
			 0xffc00001,                                                  // -signaling NaN                              /// 0085c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00868
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0086c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00870
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00874
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00878
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00880
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00888
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0xbf028f5c,                                                  // -0.51                                       /// 00894
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00898
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0089c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008d4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008e0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00900
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00904
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0090c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00910
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00914
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00918
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0091c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00924
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00928
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0092c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00934
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00938
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00940
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00948
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0094c
			 0x7fc00001,                                                  // signaling NaN                               /// 00950
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00954
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x0e000003,                                                  // Trailing 1s:                                /// 0095c
			 0x4b000000,                                                  // 8388608.0                                   /// 00960
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00964
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00970
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00974
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00978
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0097c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00980
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00984
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0098c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00990
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00994
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0099c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009a0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009c4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009cc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009d4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009d8
			 0x3f028f5c,                                                  // 0.51                                        /// 009dc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009e4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009ec
			 0xbf028f5c,                                                  // -0.51                                       /// 009f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a00
			 0xbf028f5c,                                                  // -0.51                                       /// 00a04
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a0c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a10
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a18
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a1c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a24
			 0xff800000,                                                  // -inf                                        /// 00a28
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a2c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a34
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a38
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a44
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a48
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a4c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a54
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a58
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a5c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a60
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a6c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a70
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a74
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a80
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a84
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a88
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a8c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a90
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a94
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a9c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00aa4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00aac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ab0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ab4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00abc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ac0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ac4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x4b000000,                                                  // 8388608.0                                   /// 00ad0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ad4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ad8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00adc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ae0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ae8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00aec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00af4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00af8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00afc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b00
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b04
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b08
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b0c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b10
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b14
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b20
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b24
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b28
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b34
			 0xff800000,                                                  // -inf                                        /// 00b38
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b3c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b40
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b44
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b48
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b4c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b50
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b58
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b5c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b60
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b64
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b68
			 0xffc00001,                                                  // -signaling NaN                              /// 00b6c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b70
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b74
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b78
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b7c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b84
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b88
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b94
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b98
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ba0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ba4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bb0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bb8
			 0x7fc00001,                                                  // signaling NaN                               /// 00bbc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bc4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bcc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bd0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bd8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bdc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00be0
			 0xff800000,                                                  // -inf                                        /// 00be4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bf8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bfc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c00
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c04
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c08
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c0c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c10
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c20
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c24
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c34
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c38
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c3c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c40
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c44
			 0xcb000000,                                                  // -8388608.0                                  /// 00c48
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c50
			 0xcb000000,                                                  // -8388608.0                                  /// 00c54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c58
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c5c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c60
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c64
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c74
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c7c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c80
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c84
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c88
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c8c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c94
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ca8
			 0xbf028f5c,                                                  // -0.51                                       /// 00cac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cb4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cb8
			 0xcb000000,                                                  // -8388608.0                                  /// 00cbc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cc0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cc4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cc8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ccc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cd4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cdc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ce4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cf0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cf8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cfc
			 0x7fc00001,                                                  // signaling NaN                               /// 00d00
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d04
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d08
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d0c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d10
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d14
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d24
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d28
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d30
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d38
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d3c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d48
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d4c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d58
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d64
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d68
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d6c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d70
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x55555555,                                                  // 4 random values                             /// 00d78
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d80
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d84
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d88
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d8c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d90
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d94
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00da0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00da4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00da8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00db0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00db4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dbc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dc0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dc8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dcc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dd0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dd8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ddc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00de0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00de4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00de8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dec
			 0x0e000007,                                                  // Trailing 1s:                                /// 00df0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00df4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00df8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00dfc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e00
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e10
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e14
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e18
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e1c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e20
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e24
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e2c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e30
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e34
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e3c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e40
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e44
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e48
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e4c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e50
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e54
			 0x80000000,                                                  // -zero                                       /// 00e58
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e64
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e68
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e7c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e84
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e88
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e8c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e90
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e94
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e98
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e9c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ea4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ea8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00eb0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00eb4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00eb8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ebc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ec0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ec4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ec8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ecc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ed4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00edc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ee0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ee4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ee8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ef0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ef4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ef8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00efc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f00
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f08
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f10
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f14
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f18
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f1c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f24
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f2c
			 0x00000000,                                                  // zero                                        /// 00f30
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f38
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f3c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f40
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f44
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f4c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f50
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f54
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f58
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f5c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f60
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f64
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f6c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0xffc00001,                                                  // -signaling NaN                              /// 00f78
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f80
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f88
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f90
			 0x00000000,                                                  // zero                                        /// 00f94
			 0x7f800000,                                                  // inf                                         /// 00f98
			 0xffc00001,                                                  // -signaling NaN                              /// 00f9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fa4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fa8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fb0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fb4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fb8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fbc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fc0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fc8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fd0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fd8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fe0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fe4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fe8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ff4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0xff7ffffe // max norm - 3ulp                               // max norm -ve                                /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00000
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x0e000003,                                                  // Trailing 1s:                                /// 00008
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0000c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00010
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00018
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00024
			 0x80011111,                                                  // -9.7958E-41                                 /// 00028
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0002c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00030
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00034
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00038
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0003c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00040
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00044
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00048
			 0xbf028f5c,                                                  // -0.51                                       /// 0004c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00050
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00054
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00058
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0005c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x0c700000,                                                  // Leading 1s:                                 /// 00064
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00068
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0006c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00078
			 0x00000000,                                                  // zero                                        /// 0007c
			 0x3f028f5c,                                                  // 0.51                                        /// 00080
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00084
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00088
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00090
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00094
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00098
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0009c
			 0x55555555,                                                  // 4 random values                             /// 000a0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000b0
			 0x3f028f5c,                                                  // 0.51                                        /// 000b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000b8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000d0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000d8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000ec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000f0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00100
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00108
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0010c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00110
			 0xbf028f5c,                                                  // -0.51                                       /// 00114
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0011c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00120
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00128
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0012c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00130
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00134
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00140
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x00000000,                                                  // zero                                        /// 00148
			 0x55555555,                                                  // 4 random values                             /// 0014c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00154
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00158
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0015c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00168
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x0c400000,                                                  // Leading 1s:                                 /// 00178
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0017c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00180
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00184
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0018c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00190
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00194
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00198
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0019c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x00000000,                                                  // zero                                        /// 001a4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001c8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0xbf028f5c,                                                  // -0.51                                       /// 001dc
			 0xff800000,                                                  // -inf                                        /// 001e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001e4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001ec
			 0x80000000,                                                  // -zero                                       /// 001f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 001fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00200
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0020c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00214
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0021c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00220
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00228
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0022c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00230
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00234
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00238
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x0c780000,                                                  // Leading 1s:                                 /// 00244
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00254
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00258
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0025c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00260
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00268
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00270
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00274
			 0xffc00001,                                                  // -signaling NaN                              /// 00278
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00280
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00284
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0028c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00294
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0029c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002a4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002ac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002b0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002bc
			 0x00000000,                                                  // zero                                        /// 002c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002c4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002d0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 002e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002f8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00314
			 0x0c780000,                                                  // Leading 1s:                                 /// 00318
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0031c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00324
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00328
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0032c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00330
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00338
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00340
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00344
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00350
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00358
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00360
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00364
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00368
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00374
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00378
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0037c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00380
			 0xff800000,                                                  // -inf                                        /// 00384
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00388
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0038c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00390
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00394
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00398
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0039c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003a8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003b0
			 0x3f028f5c,                                                  // 0.51                                        /// 003b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x00000000,                                                  // zero                                        /// 003cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003d8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003f0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00400
			 0x00000000,                                                  // zero                                        /// 00404
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0040c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00410
			 0x0c400000,                                                  // Leading 1s:                                 /// 00414
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0041c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00428
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0042c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00430
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00434
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0c400000,                                                  // Leading 1s:                                 /// 0043c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00440
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00444
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0044c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00454
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00458
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0045c
			 0xffc00001,                                                  // -signaling NaN                              /// 00460
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00464
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00470
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00474
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00484
			 0x0c400000,                                                  // Leading 1s:                                 /// 00488
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0048c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00494
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x0e000007,                                                  // Trailing 1s:                                /// 0049c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004a0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004b0
			 0xff800000,                                                  // -inf                                        /// 004b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 004b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004bc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 004c4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004e0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004f8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004fc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00500
			 0x3f028f5c,                                                  // 0.51                                        /// 00504
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0050c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00510
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00518
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0051c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00520
			 0x0e000003,                                                  // Trailing 1s:                                /// 00524
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00528
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0052c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00530
			 0xff800000,                                                  // -inf                                        /// 00534
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00544
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00548
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00550
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00554
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00558
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0055c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00560
			 0x0e000003,                                                  // Trailing 1s:                                /// 00564
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00568
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0056c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00570
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00578
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00584
			 0x4b000000,                                                  // 8388608.0                                   /// 00588
			 0x0e000003,                                                  // Trailing 1s:                                /// 0058c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00590
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00598
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0059c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005ac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005c0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x80011111,                                                  // -9.7958E-41                                 /// 005d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005d8
			 0x80011111,                                                  // -9.7958E-41                                 /// 005dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 005e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00600
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00604
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00608
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0060c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00614
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00618
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0061c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00620
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0xbf028f5c,                                                  // -0.51                                       /// 00628
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0062c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00630
			 0x3f028f5c,                                                  // 0.51                                        /// 00634
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00638
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0063c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00640
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00644
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00648
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00650
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00658
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0065c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00660
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00668
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0066c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00670
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00674
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x3f028f5c,                                                  // 0.51                                        /// 0067c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00680
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0068c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00690
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00694
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00698
			 0x55555555,                                                  // 4 random values                             /// 0069c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006b0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006dc
			 0x55555555,                                                  // 4 random values                             /// 006e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006ec
			 0xffc00001,                                                  // -signaling NaN                              /// 006f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006f4
			 0x7f800000,                                                  // inf                                         /// 006f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00700
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00704
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0xffc00001,                                                  // -signaling NaN                              /// 0070c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00710
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0xcb000000,                                                  // -8388608.0                                  /// 00718
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0071c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00720
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00728
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0072c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00730
			 0x0c400000,                                                  // Leading 1s:                                 /// 00734
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00738
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00744
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00750
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00758
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00760
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00768
			 0xff800000,                                                  // -inf                                        /// 0076c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00774
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00780
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00784
			 0x33333333,                                                  // 4 random values                             /// 00788
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00790
			 0x0e000003,                                                  // Trailing 1s:                                /// 00794
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00798
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007a4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0xffc00001,                                                  // -signaling NaN                              /// 007ac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007bc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 007dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007e4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007f4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007f8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00800
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00804
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00808
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0080c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00810
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00814
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00818
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0081c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00820
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00824
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00828
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0082c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00830
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00834
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0083c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00840
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00844
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0084c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00850
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00854
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0085c
			 0xbf028f5c,                                                  // -0.51                                       /// 00860
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00868
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0086c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00870
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00878
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0087c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00880
			 0x80011111,                                                  // -9.7958E-41                                 /// 00884
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00890
			 0x7f800000,                                                  // inf                                         /// 00894
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00898
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0xffc00001,                                                  // -signaling NaN                              /// 008b0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x0c780000,                                                  // Leading 1s:                                 /// 008b8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008c4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008c8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008d0
			 0xff800000,                                                  // -inf                                        /// 008d4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008dc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008ec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x0c780000,                                                  // Leading 1s:                                 /// 00904
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00908
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0090c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00910
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00914
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00918
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0091c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00924
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00928
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0092c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00930
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00934
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00938
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0093c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00940
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0094c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00950
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00954
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00960
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00964
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00968
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00970
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00978
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0097c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00980
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00984
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00988
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00990
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00994
			 0x0e000003,                                                  // Trailing 1s:                                /// 00998
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0099c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009a4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009c0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009d4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009d8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009dc
			 0x80011111,                                                  // -9.7958E-41                                 /// 009e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x80000000,                                                  // -zero                                       /// 009f8
			 0x33333333,                                                  // 4 random values                             /// 009fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a04
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a0c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a10
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a14
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a1c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a20
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a24
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a2c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a30
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a38
			 0x33333333,                                                  // 4 random values                             /// 00a3c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a40
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a48
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a58
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a5c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a60
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a64
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a74
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a7c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a80
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a94
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00aa0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00aac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ab0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ab4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ac4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ac8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00acc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ad4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ad8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00adc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ae0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ae8
			 0x33333333,                                                  // 4 random values                             /// 00aec
			 0x80000000,                                                  // -zero                                       /// 00af0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00afc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b00
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b08
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b10
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b14
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b18
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b1c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x7fc00001,                                                  // signaling NaN                               /// 00b30
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b34
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b40
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b4c
			 0xbf028f5c,                                                  // -0.51                                       /// 00b50
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b54
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b58
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b60
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b64
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b68
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b6c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b74
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b78
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b88
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b8c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b90
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b9c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ba0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ba8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bac
			 0xcb000000,                                                  // -8388608.0                                  /// 00bb0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bb8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bbc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bc0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bc4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bc8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bd4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bd8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bdc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00be4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bf0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x4b000000,                                                  // 8388608.0                                   /// 00bfc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c00
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c04
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c08
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c0c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c10
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c14
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c20
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c2c
			 0xff800000,                                                  // -inf                                        /// 00c30
			 0x00000000,                                                  // zero                                        /// 00c34
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c38
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c40
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c44
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c4c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c50
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c54
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c58
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c5c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c60
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c70
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c74
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c78
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c7c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c80
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c84
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c88
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c8c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c90
			 0xff800000,                                                  // -inf                                        /// 00c94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c9c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ca0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ca4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cb0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cb4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cb8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cc4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cc8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ccc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cd4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cdc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0xff800000,                                                  // -inf                                        /// 00ce8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cec
			 0xffc00001,                                                  // -signaling NaN                              /// 00cf0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cf4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cf8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d00
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d04
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d10
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d18
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d1c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d20
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d2c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d38
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d3c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d40
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d44
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d48
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d4c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d50
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d54
			 0xffc00001,                                                  // -signaling NaN                              /// 00d58
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d6c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d70
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d74
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d78
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d7c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d80
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d84
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d88
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d90
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d94
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d9c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00da0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00da4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00da8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00db0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00db8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dc0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dc4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dc8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dcc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dd0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dd4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dd8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ddc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00de0
			 0x7fc00001,                                                  // signaling NaN                               /// 00de4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00de8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00df0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00df4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00df8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e00
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e04
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e0c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e10
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e18
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e20
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e24
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e28
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e2c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e30
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e34
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e38
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e3c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e40
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e44
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e4c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e54
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e5c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e70
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e78
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e80
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e88
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e94
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e98
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x55555555,                                                  // 4 random values                             /// 00ea0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ea4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ea8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00eb0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00eb4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00eb8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ebc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ec0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ec4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ecc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ed4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ed8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00edc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ee0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x7f800000,                                                  // inf                                         /// 00ee8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00eec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ef0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ef4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ef8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f00
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f08
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f0c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f10
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f1c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f20
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f24
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f28
			 0xcb000000,                                                  // -8388608.0                                  /// 00f2c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f30
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f34
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f3c
			 0x00000000,                                                  // zero                                        /// 00f40
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f44
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f48
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f50
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f54
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0xff800000,                                                  // -inf                                        /// 00f64
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f70
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f74
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f78
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f7c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x3f028f5c,                                                  // 0.51                                        /// 00f84
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f8c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f94
			 0x80000000,                                                  // -zero                                       /// 00f98
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f9c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fa0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fa4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fa8
			 0xffc00001,                                                  // -signaling NaN                              /// 00fac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fb0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fb4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fb8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fbc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fc4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fcc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fd0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fdc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fe8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ff0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ff4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ff8
			 0xff800000                                                  // -inf                                        /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00004
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00008
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0000c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00010
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00014
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00018
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0001c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00020
			 0xffc00001,                                                  // -signaling NaN                              /// 00024
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00028
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0002c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00030
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00034
			 0x00011111,                                                  // 9.7958E-41                                  /// 00038
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0003c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00040
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00058
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0005c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00060
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00064
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00068
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00074
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x55555555,                                                  // 4 random values                             /// 0007c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00080
			 0xcb000000,                                                  // -8388608.0                                  /// 00084
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00088
			 0x7f800000,                                                  // inf                                         /// 0008c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00090
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00094
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00098
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0009c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000a4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000b8
			 0x80000000,                                                  // -zero                                       /// 000bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000c8
			 0x55555555,                                                  // 4 random values                             /// 000cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000e8
			 0x7fc00001,                                                  // signaling NaN                               /// 000ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000f0
			 0x7fc00001,                                                  // signaling NaN                               /// 000f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000f8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00100
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00108
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0010c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00118
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0011c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00124
			 0x80011111,                                                  // -9.7958E-41                                 /// 00128
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0012c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00130
			 0x0c780000,                                                  // Leading 1s:                                 /// 00134
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00138
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0013c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00140
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00144
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00148
			 0x0c400000,                                                  // Leading 1s:                                 /// 0014c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0xbf028f5c,                                                  // -0.51                                       /// 00154
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00158
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0015c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00160
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00164
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00168
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0016c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00170
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00174
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00178
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0017c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00180
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00184
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00188
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0018c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 001d8
			 0xbf028f5c,                                                  // -0.51                                       /// 001dc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001e8
			 0x80000000,                                                  // -zero                                       /// 001ec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00200
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00204
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00208
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00210
			 0x3f028f5c,                                                  // 0.51                                        /// 00214
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00218
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00228
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0022c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00230
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00234
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0023c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00240
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00244
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00248
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0024c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00250
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x80000000,                                                  // -zero                                       /// 00258
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0025c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00260
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00268
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00270
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00274
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0027c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00280
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00288
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00290
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00294
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00298
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0029c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002a8
			 0x7f800000,                                                  // inf                                         /// 002ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002d0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002d4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002d8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002dc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002e4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002e8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00300
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00304
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00308
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0030c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00314
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00324
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0xffc00001,                                                  // -signaling NaN                              /// 0032c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00330
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00334
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00338
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0033c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00344
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00348
			 0x55555555,                                                  // 4 random values                             /// 0034c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00350
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00358
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00360
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00364
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00368
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0036c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00370
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00374
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00378
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00380
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00388
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0038c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00394
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00398
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0039c
			 0x0c780000,                                                  // Leading 1s:                                 /// 003a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003a8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003ac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003c4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003c8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x80000000,                                                  // -zero                                       /// 003d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003e0
			 0x33333333,                                                  // 4 random values                             /// 003e4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003f4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003f8
			 0x55555555,                                                  // 4 random values                             /// 003fc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00400
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00408
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0040c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00418
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0041c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00420
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00424
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00428
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00430
			 0xffc00001,                                                  // -signaling NaN                              /// 00434
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00440
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00444
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0044c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00450
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00454
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00464
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00468
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0046c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00470
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00474
			 0x0c400000,                                                  // Leading 1s:                                 /// 00478
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00480
			 0x0c400000,                                                  // Leading 1s:                                 /// 00484
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0048c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00490
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00494
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00498
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0049c
			 0x7fc00001,                                                  // signaling NaN                               /// 004a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004a8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004d0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004d4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004d8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00500
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x0e000007,                                                  // Trailing 1s:                                /// 00508
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00514
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00518
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00520
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00524
			 0x0e000007,                                                  // Trailing 1s:                                /// 00528
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00538
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00540
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00548
			 0xcb000000,                                                  // -8388608.0                                  /// 0054c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00550
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00554
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00558
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0xff800000,                                                  // -inf                                        /// 00560
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00564
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00568
			 0x0c780000,                                                  // Leading 1s:                                 /// 0056c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00570
			 0x0e000007,                                                  // Trailing 1s:                                /// 00574
			 0xbf028f5c,                                                  // -0.51                                       /// 00578
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0057c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00580
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00590
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x0e000001,                                                  // Trailing 1s:                                /// 0059c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005b4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005b8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005bc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x3f028f5c,                                                  // 0.51                                        /// 005c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 005cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005f4
			 0x00000000,                                                  // zero                                        /// 005f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0xff800000,                                                  // -inf                                        /// 00604
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0060c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00610
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00614
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x7fc00001,                                                  // signaling NaN                               /// 0061c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00620
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00624
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00628
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0062c
			 0xffc00001,                                                  // -signaling NaN                              /// 00630
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00634
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00638
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00640
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00644
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00648
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0064c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00650
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00654
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0065c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00664
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0066c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00674
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00678
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0067c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00688
			 0xffc00001,                                                  // -signaling NaN                              /// 0068c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00690
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00694
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006a0
			 0x3f028f5c,                                                  // 0.51                                        /// 006a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006b4
			 0x3f028f5c,                                                  // 0.51                                        /// 006b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006bc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 006d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00704
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00708
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0070c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00710
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00714
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00718
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x80011111,                                                  // -9.7958E-41                                 /// 00724
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00728
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0072c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00730
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00734
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00738
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00740
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00744
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0074c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00750
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00754
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00758
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0075c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0076c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0077c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00780
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00788
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0078c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00790
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00794
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007a8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007b8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007bc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007cc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 007dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007e8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00804
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00808
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0080c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00810
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00814
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00818
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00820
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0082c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00830
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00834
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0083c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00848
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x55555555,                                                  // 4 random values                             /// 00850
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00858
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0085c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00860
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00864
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00868
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0086c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00878
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00880
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00884
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0088c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00890
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00894
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00898
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0089c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008a8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008c4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008cc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008d8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008e4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008ec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008f8
			 0xbf028f5c,                                                  // -0.51                                       /// 008fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00904
			 0x0c780000,                                                  // Leading 1s:                                 /// 00908
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0090c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00910
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00914
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00918
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0091c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00920
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00924
			 0x0c400000,                                                  // Leading 1s:                                 /// 00928
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0092c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00930
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00934
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0093c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00940
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00944
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00948
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0094c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00950
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00954
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0095c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00960
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00964
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x7f800000,                                                  // inf                                         /// 0096c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00970
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00974
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00978
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0xbf028f5c,                                                  // -0.51                                       /// 00980
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0098c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00990
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x33333333,                                                  // 4 random values                             /// 00998
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009a0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009bc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009dc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009e8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009ec
			 0xbf028f5c,                                                  // -0.51                                       /// 009f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 009f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a00
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a04
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a08
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a18
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a1c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a24
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a2c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a38
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a3c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a40
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a44
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a4c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a50
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a58
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a5c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a60
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a68
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a70
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a74
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a7c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a80
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a88
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a90
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a94
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a98
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a9c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00aa0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aa4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00aa8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00aac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ab0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ab4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ab8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00abc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00acc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ad4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00adc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ae0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ae8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00aec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00af0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00af4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00af8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00afc
			 0xffc00001,                                                  // -signaling NaN                              /// 00b00
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b04
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b0c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b10
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b14
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b18
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b1c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b20
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b24
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b28
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b2c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b30
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b34
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b38
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b3c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b40
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b44
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b50
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b54
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b58
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b5c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b60
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b64
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b6c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b70
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b74
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b78
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b7c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b84
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0xcb000000,                                                  // -8388608.0                                  /// 00b90
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b98
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b9c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bb8
			 0xcb000000,                                                  // -8388608.0                                  /// 00bbc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bc4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bc8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bcc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bd0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bd4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bd8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bdc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00be4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00be8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bf0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c00
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c04
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c08
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c10
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c28
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c2c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x00000000,                                                  // zero                                        /// 00c34
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c38
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c3c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c44
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c48
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c54
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c58
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c60
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c64
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c6c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c70
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c78
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c84
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c88
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c90
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c9c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ca0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ca4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ca8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cb0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cb8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cc4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cc8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cd0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cd4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cd8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cdc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ce0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ce8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cf0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cf8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cfc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d00
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d0c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d10
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d14
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d18
			 0x3f028f5c,                                                  // 0.51                                        /// 00d1c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d20
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d24
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d28
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d30
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d34
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d38
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d3c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d40
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d44
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d48
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d4c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d50
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d54
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d58
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d64
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d6c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d78
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d7c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d80
			 0x7f800000,                                                  // inf                                         /// 00d84
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d8c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d94
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d98
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d9c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00da8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dac
			 0x0e000003,                                                  // Trailing 1s:                                /// 00db0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dc0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dc4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dc8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dcc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dd0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00dd4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dd8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00dec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00df0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dfc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e00
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e08
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e10
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e18
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e1c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e20
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e28
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e30
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x55555555,                                                  // 4 random values                             /// 00e38
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e3c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e40
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e44
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e48
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e4c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e50
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e5c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e68
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e6c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e70
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e78
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e7c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0xffc00001,                                                  // -signaling NaN                              /// 00e84
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e8c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e94
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e9c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ea0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ea4
			 0x00000000,                                                  // zero                                        /// 00ea8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00eac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00eb0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ebc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ec0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ec4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ec8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ecc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ed0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ed4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00edc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ee0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ee4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ee8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ef0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ef4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ef8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f00
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f04
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f08
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f10
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f14
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f18
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f20
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f24
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f28
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f30
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f34
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f38
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f48
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f4c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f54
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f58
			 0x4b000000,                                                  // 8388608.0                                   /// 00f5c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f68
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f74
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f78
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f7c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f80
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f84
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f88
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f8c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f90
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f94
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f98
			 0xbf028f5c,                                                  // -0.51                                       /// 00f9c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fa4
			 0xff800000,                                                  // -inf                                        /// 00fa8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fb4
			 0x00000000,                                                  // zero                                        /// 00fb8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fbc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fc0
			 0x00000000,                                                  // zero                                        /// 00fc4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fd0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fd4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fe0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fe4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ff0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ff8
			 0x0e003fff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00000
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00004
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00010
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00014
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00018
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0001c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00020
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00028
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0002c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00030
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00034
			 0x0c780000,                                                  // Leading 1s:                                 /// 00038
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00040
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00048
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0004c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00050
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00054
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00058
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0005c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00060
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00064
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0006c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00070
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0007c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00080
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00084
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0008c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x0c600000,                                                  // Leading 1s:                                 /// 00098
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000c0
			 0x7f800000,                                                  // inf                                         /// 000c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000c8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000dc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000e8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 000f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x00000000,                                                  // zero                                        /// 000fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00108
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00110
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00114
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00118
			 0x0e000007,                                                  // Trailing 1s:                                /// 0011c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00120
			 0xcb000000,                                                  // -8388608.0                                  /// 00124
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0012c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00130
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00134
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00138
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0013c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00140
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00144
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00150
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00154
			 0x0e000001,                                                  // Trailing 1s:                                /// 00158
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0015c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00160
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00164
			 0x80000000,                                                  // -zero                                       /// 00168
			 0x55555555,                                                  // 4 random values                             /// 0016c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00170
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00174
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00178
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0017c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00180
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00184
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0018c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00190
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00194
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00198
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0019c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001a8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001c0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001c8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001d0
			 0x80000000,                                                  // -zero                                       /// 001d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001d8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001dc
			 0x33333333,                                                  // 4 random values                             /// 001e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001ec
			 0xffc00001,                                                  // -signaling NaN                              /// 001f0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001f8
			 0x0c400000,                                                  // Leading 1s:                                 /// 001fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00200
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00208
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0020c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00210
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00214
			 0x0c400000,                                                  // Leading 1s:                                 /// 00218
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0021c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00224
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00228
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0022c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00230
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00234
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00238
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00240
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00244
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0024c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00250
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0025c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00260
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00264
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00268
			 0x80000000,                                                  // -zero                                       /// 0026c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00270
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00274
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0027c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00280
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00284
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00290
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00294
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00298
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002a0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002ac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 002bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 002d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002e0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002e4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002f0
			 0x0c400000,                                                  // Leading 1s:                                 /// 002f4
			 0xcb000000,                                                  // -8388608.0                                  /// 002f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00300
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00308
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00310
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00314
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00318
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0031c
			 0x7fc00001,                                                  // signaling NaN                               /// 00320
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00328
			 0xbf028f5c,                                                  // -0.51                                       /// 0032c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00330
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00334
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00338
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0033c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00348
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0034c
			 0x80000000,                                                  // -zero                                       /// 00350
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0035c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00364
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00370
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00374
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00378
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0037c
			 0x3f028f5c,                                                  // 0.51                                        /// 00380
			 0xffc00001,                                                  // -signaling NaN                              /// 00384
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00388
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00390
			 0x7f800000,                                                  // inf                                         /// 00394
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00398
			 0x55555555,                                                  // 4 random values                             /// 0039c
			 0xbf028f5c,                                                  // -0.51                                       /// 003a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003a4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x55555555,                                                  // 4 random values                             /// 003c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003cc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003d4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00400
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00404
			 0x0c700000,                                                  // Leading 1s:                                 /// 00408
			 0x0c780000,                                                  // Leading 1s:                                 /// 0040c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00410
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00418
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00420
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00428
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00434
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00440
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00448
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0044c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00454
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00464
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0046c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00470
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00474
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x80000000,                                                  // -zero                                       /// 0047c
			 0xbf028f5c,                                                  // -0.51                                       /// 00480
			 0x0e000007,                                                  // Trailing 1s:                                /// 00484
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00488
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0048c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00490
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0xcb000000,                                                  // -8388608.0                                  /// 00498
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0049c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004ac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004b4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004b8
			 0x3f028f5c,                                                  // 0.51                                        /// 004bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004c0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004d4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004d8
			 0x33333333,                                                  // 4 random values                             /// 004dc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004f0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00500
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00508
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0050c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00510
			 0x55555555,                                                  // 4 random values                             /// 00514
			 0x0c780000,                                                  // Leading 1s:                                 /// 00518
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0051c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00520
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00524
			 0x0c400000,                                                  // Leading 1s:                                 /// 00528
			 0x0c700000,                                                  // Leading 1s:                                 /// 0052c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00530
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00538
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0053c
			 0xffc00001,                                                  // -signaling NaN                              /// 00540
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00548
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0054c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00550
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00554
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00558
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0055c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00560
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00568
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0056c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00570
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00574
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00578
			 0x0e000007,                                                  // Trailing 1s:                                /// 0057c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00580
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00584
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0058c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00594
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00598
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0059c
			 0x0e000007,                                                  // Trailing 1s:                                /// 005a0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005b0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005b4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005d0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005ec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005f0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00600
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00604
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00608
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0060c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00610
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00614
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0061c
			 0x33333333,                                                  // 4 random values                             /// 00620
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00624
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0062c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00630
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00634
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0063c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00640
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00644
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0064c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00650
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00654
			 0x00011111,                                                  // 9.7958E-41                                  /// 00658
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00664
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00668
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0066c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00670
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00674
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00680
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00690
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00694
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0069c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006a0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0xcb000000,                                                  // -8388608.0                                  /// 006a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006bc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 006cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x55555555,                                                  // 4 random values                             /// 006d4
			 0xffc00001,                                                  // -signaling NaN                              /// 006d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0xff800000,                                                  // -inf                                        /// 006f0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006f8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00700
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00704
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00708
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0070c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00710
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00714
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00720
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00724
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00728
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0072c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0e000003,                                                  // Trailing 1s:                                /// 0073c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00740
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00744
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00748
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0074c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00750
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00754
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00758
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0075c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00760
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00764
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00768
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00778
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0077c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00780
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00788
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0078c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00790
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00794
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00798
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007a0
			 0x0c400000,                                                  // Leading 1s:                                 /// 007a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007b8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007c8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007dc
			 0xff800000,                                                  // -inf                                        /// 007e0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007ec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007f0
			 0xbf028f5c,                                                  // -0.51                                       /// 007f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x0c700000,                                                  // Leading 1s:                                 /// 00804
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00808
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0080c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0xbf028f5c,                                                  // -0.51                                       /// 00814
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00818
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00820
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00824
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0082c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00830
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00834
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00838
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00848
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0084c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00850
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00854
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0085c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00860
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00864
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00868
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0086c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00870
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00874
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00878
			 0x0c780000,                                                  // Leading 1s:                                 /// 0087c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00880
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00888
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00890
			 0x0e000003,                                                  // Trailing 1s:                                /// 00894
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0089c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 008ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008b8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008c0
			 0x7fc00001,                                                  // signaling NaN                               /// 008c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008d8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008fc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00904
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0090c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00910
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00914
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00918
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00920
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00924
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00928
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0092c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00930
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00934
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00938
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0093c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00948
			 0x0e000001,                                                  // Trailing 1s:                                /// 0094c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00950
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00954
			 0x0c400000,                                                  // Leading 1s:                                 /// 00958
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0095c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00960
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x0c700000,                                                  // Leading 1s:                                 /// 00968
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0096c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00970
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00974
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00978
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00980
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00984
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00988
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0098c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00990
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00994
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00998
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0099c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009c4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x33333333,                                                  // 4 random values                             /// 009cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009e8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x7fc00001,                                                  // signaling NaN                               /// 009f8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a00
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a04
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a0c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a14
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a18
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a20
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a30
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a34
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a38
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a3c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a40
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a44
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a48
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a54
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a58
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a5c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a70
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a74
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a78
			 0xbf028f5c,                                                  // -0.51                                       /// 00a7c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a80
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a84
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a88
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a8c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a90
			 0xff800000,                                                  // -inf                                        /// 00a94
			 0x80000000,                                                  // -zero                                       /// 00a98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a9c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00aa0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00aac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ab0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ab4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ab8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00abc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ac0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ac4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ac8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00acc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ad0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ad4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ad8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00adc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ae0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ae4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ae8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00aec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00af4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00af8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b00
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b04
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b08
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b0c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b10
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b20
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b24
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b2c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b34
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x55555555,                                                  // 4 random values                             /// 00b48
			 0x55555555,                                                  // 4 random values                             /// 00b4c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b50
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b58
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b64
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b70
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b74
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b78
			 0x7fc00001,                                                  // signaling NaN                               /// 00b7c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b80
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b84
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b88
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b8c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b90
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b94
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0xffc00001,                                                  // -signaling NaN                              /// 00b9c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ba0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ba4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ba8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bb0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bb4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bb8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bbc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bc0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bc8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bd0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bd4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bd8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bdc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00be0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00be8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bf0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bf4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bf8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bfc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c00
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c14
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c1c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c20
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c24
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c28
			 0xff800000,                                                  // -inf                                        /// 00c2c
			 0x4b000000,                                                  // 8388608.0                                   /// 00c30
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c3c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c44
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c48
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c4c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c54
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c60
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c64
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c68
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c70
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c78
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c7c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c80
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c84
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c88
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c90
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c94
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c9c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ca8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cb4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cb8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cbc
			 0x4b000000,                                                  // 8388608.0                                   /// 00cc0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cc8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cd0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cd4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cdc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ce0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ce8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cf0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cf4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cf8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d00
			 0xff800000,                                                  // -inf                                        /// 00d04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d0c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d10
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d14
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d20
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d2c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d40
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d4c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d50
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d5c
			 0x00000000,                                                  // zero                                        /// 00d60
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d68
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d70
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d84
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0xffc00001,                                                  // -signaling NaN                              /// 00d8c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d90
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d94
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d9c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00da4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00da8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00db4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dbc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dc0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x7f800000,                                                  // inf                                         /// 00dcc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dd8
			 0x33333333,                                                  // 4 random values                             /// 00ddc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00de8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00df0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0xbf028f5c,                                                  // -0.51                                       /// 00df8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dfc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e00
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e08
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e0c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e1c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e20
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e2c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e30
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e34
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e38
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e3c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e44
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e48
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e4c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e50
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e58
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e60
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e64
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e68
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e70
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e74
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e78
			 0xcb000000,                                                  // -8388608.0                                  /// 00e7c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e84
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e8c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e90
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e94
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e98
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ea0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ea4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ea8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00eb0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x4b000000,                                                  // 8388608.0                                   /// 00eb8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ebc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ec0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ec8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ecc
			 0xffc00001,                                                  // -signaling NaN                              /// 00ed0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ed8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ee0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ee8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00eec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ef0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ef4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ef8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f00
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f04
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f08
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f10
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f20
			 0xff800000,                                                  // -inf                                        /// 00f24
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f28
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f2c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f3c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f40
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f44
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f48
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f4c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f58
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f60
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f64
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f6c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f70
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f78
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f7c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f80
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f84
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f88
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f8c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f90
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f94
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f9c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fa8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fb0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fb4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fb8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fbc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x55555555,                                                  // 4 random values                             /// 00fc8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0x3f028f5c,                                                  // 0.51                                        /// 00fd0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fd4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fd8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fe0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ff0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ff8
			 0x0c600000                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x424e03a2, /// 0x0
			 0x48159cc7, /// 0x4
			 0xfe56773e, /// 0x8
			 0x71c377ca, /// 0xc
			 0x9b04cdeb, /// 0x10
			 0x6da9660d, /// 0x14
			 0x4c08ea0b, /// 0x18
			 0x387bce30, /// 0x1c
			 0x95a2f25c, /// 0x20
			 0xd959e5bc, /// 0x24
			 0xab83f4fd, /// 0x28
			 0xaec0b403, /// 0x2c
			 0x9b5aadaa, /// 0x30
			 0x88323e57, /// 0x34
			 0xdf06643d, /// 0x38
			 0xe3f1fe19, /// 0x3c
			 0x6d48ba43, /// 0x40
			 0x24381743, /// 0x44
			 0xade70bef, /// 0x48
			 0x646eb169, /// 0x4c
			 0x59a22ee2, /// 0x50
			 0xb85119fc, /// 0x54
			 0x536f9d64, /// 0x58
			 0xd1a7c336, /// 0x5c
			 0x27474af5, /// 0x60
			 0x4b70581f, /// 0x64
			 0x1ceb03cc, /// 0x68
			 0xcf313f95, /// 0x6c
			 0x15f72de6, /// 0x70
			 0xe3d23cee, /// 0x74
			 0xb3d54f38, /// 0x78
			 0x85194fb6, /// 0x7c
			 0x044ac4ce, /// 0x80
			 0x487a7194, /// 0x84
			 0xa5d34016, /// 0x88
			 0xb3d81066, /// 0x8c
			 0x7c02d393, /// 0x90
			 0x3bb8434d, /// 0x94
			 0x698c1f7b, /// 0x98
			 0xca723026, /// 0x9c
			 0xb328735c, /// 0xa0
			 0x3377ed83, /// 0xa4
			 0xb7216b74, /// 0xa8
			 0x82262061, /// 0xac
			 0x80c3494e, /// 0xb0
			 0x85403d51, /// 0xb4
			 0x9536082c, /// 0xb8
			 0x967d1b3c, /// 0xbc
			 0xaab33695, /// 0xc0
			 0xba6a6442, /// 0xc4
			 0x136bc1f0, /// 0xc8
			 0x3aa7024d, /// 0xcc
			 0xd9f42772, /// 0xd0
			 0x07288d92, /// 0xd4
			 0xb8141e1b, /// 0xd8
			 0x7b38bba6, /// 0xdc
			 0x041039a6, /// 0xe0
			 0xfa66a101, /// 0xe4
			 0xc18276b3, /// 0xe8
			 0x52b3c13b, /// 0xec
			 0x5c624f0a, /// 0xf0
			 0x7cce388e, /// 0xf4
			 0xa012d3ed, /// 0xf8
			 0x9f54523e, /// 0xfc
			 0xfe62dcea, /// 0x100
			 0x253be83a, /// 0x104
			 0x43f78995, /// 0x108
			 0xdaef86b4, /// 0x10c
			 0xaa93a72d, /// 0x110
			 0xdd60f191, /// 0x114
			 0x9b2dc8d1, /// 0x118
			 0x5fd38626, /// 0x11c
			 0x747cf4e4, /// 0x120
			 0xb1f6d9d1, /// 0x124
			 0x0ff7b78d, /// 0x128
			 0x0099175c, /// 0x12c
			 0x6a252070, /// 0x130
			 0xfbc761ce, /// 0x134
			 0x9a1d25d2, /// 0x138
			 0x3c464236, /// 0x13c
			 0x7dd8b0b0, /// 0x140
			 0xdcca7345, /// 0x144
			 0x51559568, /// 0x148
			 0x82320bbd, /// 0x14c
			 0x1e44899c, /// 0x150
			 0x2e88f234, /// 0x154
			 0x0b8f23e4, /// 0x158
			 0x39946479, /// 0x15c
			 0xd5aec156, /// 0x160
			 0x72b957db, /// 0x164
			 0x316e0418, /// 0x168
			 0x029c2e19, /// 0x16c
			 0x2b0a7760, /// 0x170
			 0x7e3019a0, /// 0x174
			 0x6a85ea4c, /// 0x178
			 0x325f8587, /// 0x17c
			 0x09a81e8e, /// 0x180
			 0xb4bacaf2, /// 0x184
			 0x36dc9db5, /// 0x188
			 0x1d87f75e, /// 0x18c
			 0x037fa082, /// 0x190
			 0x92138ae7, /// 0x194
			 0x21a578a5, /// 0x198
			 0xef93747f, /// 0x19c
			 0x34619770, /// 0x1a0
			 0x6bfd0d4a, /// 0x1a4
			 0xddf2d1cf, /// 0x1a8
			 0xae5d0295, /// 0x1ac
			 0xca3785e3, /// 0x1b0
			 0x294ae280, /// 0x1b4
			 0x629e2370, /// 0x1b8
			 0xf4a86df7, /// 0x1bc
			 0xceed51c9, /// 0x1c0
			 0xcca62d0b, /// 0x1c4
			 0x8c573080, /// 0x1c8
			 0xa2dcc0f9, /// 0x1cc
			 0x29427e7e, /// 0x1d0
			 0xbf7137f5, /// 0x1d4
			 0x10ddc614, /// 0x1d8
			 0x643afd0d, /// 0x1dc
			 0x117ae703, /// 0x1e0
			 0x3b4fefb8, /// 0x1e4
			 0xc49afc0d, /// 0x1e8
			 0xcd436fda, /// 0x1ec
			 0xace21fe9, /// 0x1f0
			 0x6b93704f, /// 0x1f4
			 0x80b6e63c, /// 0x1f8
			 0x772e4943, /// 0x1fc
			 0x0280a88c, /// 0x200
			 0xec78b1af, /// 0x204
			 0x41cf3b2a, /// 0x208
			 0x656bf351, /// 0x20c
			 0xd5b15c85, /// 0x210
			 0x9b19457d, /// 0x214
			 0xfa98c272, /// 0x218
			 0xa390bffa, /// 0x21c
			 0xabfc261d, /// 0x220
			 0x96aaa4d8, /// 0x224
			 0xa8a70fae, /// 0x228
			 0xb4e1ca23, /// 0x22c
			 0x7e6d1c21, /// 0x230
			 0x5a4e9e47, /// 0x234
			 0x9947f7aa, /// 0x238
			 0x89af7287, /// 0x23c
			 0x736b2c2b, /// 0x240
			 0xb2908370, /// 0x244
			 0x721a0409, /// 0x248
			 0xcdc05a36, /// 0x24c
			 0x214b9f35, /// 0x250
			 0xc6147103, /// 0x254
			 0x439751e4, /// 0x258
			 0x310c836d, /// 0x25c
			 0xf99cd877, /// 0x260
			 0xde74b8f1, /// 0x264
			 0x4a84753b, /// 0x268
			 0xeca75b55, /// 0x26c
			 0x521d3c22, /// 0x270
			 0x3da78492, /// 0x274
			 0x789602c5, /// 0x278
			 0xeb4ca2fb, /// 0x27c
			 0x4dd272e0, /// 0x280
			 0xdcb0fa44, /// 0x284
			 0xf3783988, /// 0x288
			 0xdd13c5f6, /// 0x28c
			 0x9c207bee, /// 0x290
			 0xd6610bac, /// 0x294
			 0xc71eaf6e, /// 0x298
			 0x9a12b914, /// 0x29c
			 0x8d3f9a13, /// 0x2a0
			 0x0f8fc7b4, /// 0x2a4
			 0xd479522a, /// 0x2a8
			 0x0737b7c4, /// 0x2ac
			 0x03e5194d, /// 0x2b0
			 0x551a164c, /// 0x2b4
			 0x86204e15, /// 0x2b8
			 0x2d30c63f, /// 0x2bc
			 0x2a3ca9b2, /// 0x2c0
			 0xe1fcc8cc, /// 0x2c4
			 0x7d8aef83, /// 0x2c8
			 0x0a2b7463, /// 0x2cc
			 0x870f3ebd, /// 0x2d0
			 0x887c6017, /// 0x2d4
			 0xac752274, /// 0x2d8
			 0xf71d7764, /// 0x2dc
			 0x2060f9ef, /// 0x2e0
			 0xd2747d05, /// 0x2e4
			 0xd6791822, /// 0x2e8
			 0x198f1ccb, /// 0x2ec
			 0x8243cea8, /// 0x2f0
			 0xabde4cfa, /// 0x2f4
			 0x07c20de9, /// 0x2f8
			 0xe2528e90, /// 0x2fc
			 0x0119ed35, /// 0x300
			 0x993f40de, /// 0x304
			 0x6e269834, /// 0x308
			 0x7d042b05, /// 0x30c
			 0x3d7c3e22, /// 0x310
			 0x3b81203d, /// 0x314
			 0xcbd41dcb, /// 0x318
			 0x7c8e70fc, /// 0x31c
			 0x6891834d, /// 0x320
			 0xe4b65e64, /// 0x324
			 0xca50f322, /// 0x328
			 0x4c6c34a1, /// 0x32c
			 0x81e2bd99, /// 0x330
			 0x9cb3e68b, /// 0x334
			 0x8dd414fe, /// 0x338
			 0x67f1a099, /// 0x33c
			 0xbbda9d68, /// 0x340
			 0x40c11f39, /// 0x344
			 0x3e530709, /// 0x348
			 0xa897e73d, /// 0x34c
			 0xe5f2759e, /// 0x350
			 0x1e7b0cd2, /// 0x354
			 0x2376f87d, /// 0x358
			 0x6d22dacd, /// 0x35c
			 0x393ca6bc, /// 0x360
			 0x4ca77bab, /// 0x364
			 0x18460915, /// 0x368
			 0x70f254be, /// 0x36c
			 0xe1d3e60d, /// 0x370
			 0x203f642d, /// 0x374
			 0x880fdbb9, /// 0x378
			 0x0c03f94c, /// 0x37c
			 0x4b953bc6, /// 0x380
			 0xeda24170, /// 0x384
			 0x0ed392cc, /// 0x388
			 0x743642f0, /// 0x38c
			 0xc0817cb3, /// 0x390
			 0xce8d8d8b, /// 0x394
			 0x0e0d6262, /// 0x398
			 0x2bf2cf16, /// 0x39c
			 0x41de8652, /// 0x3a0
			 0x654f09a1, /// 0x3a4
			 0xc18490ac, /// 0x3a8
			 0x5f869c1e, /// 0x3ac
			 0x80ac91ea, /// 0x3b0
			 0xe01cafd8, /// 0x3b4
			 0x82bb3d61, /// 0x3b8
			 0x82928d27, /// 0x3bc
			 0x3a3ac1b7, /// 0x3c0
			 0x0f1b7680, /// 0x3c4
			 0xb134e0dc, /// 0x3c8
			 0xc09d661d, /// 0x3cc
			 0x1e7bddf1, /// 0x3d0
			 0x6cc9c9a9, /// 0x3d4
			 0x4974fccf, /// 0x3d8
			 0x8430f6ee, /// 0x3dc
			 0xa8b9ffbb, /// 0x3e0
			 0xf50fdcbd, /// 0x3e4
			 0xa8588df1, /// 0x3e8
			 0xca46e295, /// 0x3ec
			 0x89be0aee, /// 0x3f0
			 0xbd11d16a, /// 0x3f4
			 0x750979cb, /// 0x3f8
			 0xc21a32f1, /// 0x3fc
			 0x9310149c, /// 0x400
			 0x7215c892, /// 0x404
			 0xf763eb5c, /// 0x408
			 0x64a7d0ea, /// 0x40c
			 0x76472bbc, /// 0x410
			 0x200f674a, /// 0x414
			 0x40a03fb0, /// 0x418
			 0x5a753e88, /// 0x41c
			 0x40bbfddc, /// 0x420
			 0x9dd826e6, /// 0x424
			 0x44b47578, /// 0x428
			 0x6752b5e1, /// 0x42c
			 0x1e481f36, /// 0x430
			 0x41789f23, /// 0x434
			 0x398431cc, /// 0x438
			 0x08f3ba78, /// 0x43c
			 0x1dd1baa6, /// 0x440
			 0xfbe49bce, /// 0x444
			 0x6a9e773d, /// 0x448
			 0xb4e9ecd0, /// 0x44c
			 0xf8c43ee2, /// 0x450
			 0xbeb2d5c5, /// 0x454
			 0x55639b5e, /// 0x458
			 0xc5f286d3, /// 0x45c
			 0x3f1fdb1c, /// 0x460
			 0xc03a05b9, /// 0x464
			 0x65bbf102, /// 0x468
			 0x766e7d99, /// 0x46c
			 0x649d3d51, /// 0x470
			 0x9e981828, /// 0x474
			 0xe5f069bd, /// 0x478
			 0x093cdbcc, /// 0x47c
			 0x69903fc3, /// 0x480
			 0xfa3709ca, /// 0x484
			 0x9bbbcea6, /// 0x488
			 0x0e496f09, /// 0x48c
			 0xdd16737f, /// 0x490
			 0xe6cac99d, /// 0x494
			 0xf6bd0de0, /// 0x498
			 0xa17d11f2, /// 0x49c
			 0x912d6d22, /// 0x4a0
			 0x6ddd64e0, /// 0x4a4
			 0xb033cd47, /// 0x4a8
			 0xbbcdb411, /// 0x4ac
			 0x6d58432f, /// 0x4b0
			 0xdfeb60fe, /// 0x4b4
			 0x0bcdcdfa, /// 0x4b8
			 0x3b99ce5a, /// 0x4bc
			 0x14e2deca, /// 0x4c0
			 0x0c3c9f10, /// 0x4c4
			 0x3b87069d, /// 0x4c8
			 0x90acf029, /// 0x4cc
			 0xced0f2c4, /// 0x4d0
			 0x2183ac9e, /// 0x4d4
			 0x7c774887, /// 0x4d8
			 0xabea1989, /// 0x4dc
			 0x1997803b, /// 0x4e0
			 0x4dba073b, /// 0x4e4
			 0xf1decc31, /// 0x4e8
			 0xcb10d8b0, /// 0x4ec
			 0xdbecec05, /// 0x4f0
			 0x811e5753, /// 0x4f4
			 0xd6b349fd, /// 0x4f8
			 0xa2feb374, /// 0x4fc
			 0x92c626eb, /// 0x500
			 0x8617a9a2, /// 0x504
			 0x1c0b1d61, /// 0x508
			 0x2d5c7427, /// 0x50c
			 0x4677e8bb, /// 0x510
			 0xcbd6f3cb, /// 0x514
			 0xc17c188b, /// 0x518
			 0x406cc6e1, /// 0x51c
			 0xdd49aad9, /// 0x520
			 0x7b491c34, /// 0x524
			 0x69df4118, /// 0x528
			 0x47d6acb5, /// 0x52c
			 0x54845fd6, /// 0x530
			 0xd9afe16b, /// 0x534
			 0xc1a4486f, /// 0x538
			 0x6adbff6f, /// 0x53c
			 0x3dcf64b3, /// 0x540
			 0x46adbeb1, /// 0x544
			 0x15ddf232, /// 0x548
			 0x25c3f641, /// 0x54c
			 0xcd396f82, /// 0x550
			 0xb8f73499, /// 0x554
			 0xcc62b24d, /// 0x558
			 0xdb1ef9b5, /// 0x55c
			 0x76ff38b8, /// 0x560
			 0x72b0b960, /// 0x564
			 0xd4e9695f, /// 0x568
			 0x9ca8de5e, /// 0x56c
			 0xe90ef3cc, /// 0x570
			 0xe0927ce9, /// 0x574
			 0x0ae6c63c, /// 0x578
			 0x00961d52, /// 0x57c
			 0xe3dd7c23, /// 0x580
			 0xeaf9425d, /// 0x584
			 0x8c5c556d, /// 0x588
			 0x00320c3d, /// 0x58c
			 0x43d4d966, /// 0x590
			 0x158271a1, /// 0x594
			 0x90af0247, /// 0x598
			 0xcd0a8911, /// 0x59c
			 0x07af5cfa, /// 0x5a0
			 0x78120137, /// 0x5a4
			 0xbfc92ef4, /// 0x5a8
			 0xc2512fcc, /// 0x5ac
			 0xa2c44e1f, /// 0x5b0
			 0xc56b47c9, /// 0x5b4
			 0x31c30ec1, /// 0x5b8
			 0x656fb92f, /// 0x5bc
			 0x1a263e0c, /// 0x5c0
			 0x4d051c9c, /// 0x5c4
			 0x774e9e46, /// 0x5c8
			 0xbdbc2f5b, /// 0x5cc
			 0x2b37436d, /// 0x5d0
			 0x9c5e7d54, /// 0x5d4
			 0x92a73ba8, /// 0x5d8
			 0x388bc1b3, /// 0x5dc
			 0xb1542a44, /// 0x5e0
			 0xa3ce08be, /// 0x5e4
			 0xe9efce45, /// 0x5e8
			 0xd110592b, /// 0x5ec
			 0x7a65759b, /// 0x5f0
			 0x9dc41e57, /// 0x5f4
			 0x6a2d3875, /// 0x5f8
			 0x1d2f898e, /// 0x5fc
			 0xbf7f6d8f, /// 0x600
			 0x65981be7, /// 0x604
			 0x63735ad3, /// 0x608
			 0x049e53db, /// 0x60c
			 0x51de6632, /// 0x610
			 0x3cc6a16f, /// 0x614
			 0xf569c522, /// 0x618
			 0xdad93e75, /// 0x61c
			 0x368f144b, /// 0x620
			 0x61f10073, /// 0x624
			 0x41f320b3, /// 0x628
			 0x5bfc171b, /// 0x62c
			 0x3b4d7cdf, /// 0x630
			 0x20e63bc1, /// 0x634
			 0xcdcf3b2a, /// 0x638
			 0xab71df29, /// 0x63c
			 0x0271f18e, /// 0x640
			 0xb4e869f2, /// 0x644
			 0x9a7144dc, /// 0x648
			 0x0c54c8c8, /// 0x64c
			 0x9da09ae0, /// 0x650
			 0x4c1173ec, /// 0x654
			 0x361847df, /// 0x658
			 0x01543830, /// 0x65c
			 0x90ebb892, /// 0x660
			 0x97bb5331, /// 0x664
			 0x54b808e4, /// 0x668
			 0xc303fc70, /// 0x66c
			 0xa8ef7c7c, /// 0x670
			 0x81e460c1, /// 0x674
			 0x9f728654, /// 0x678
			 0x9143b99f, /// 0x67c
			 0xec8ab184, /// 0x680
			 0x5ea08dec, /// 0x684
			 0x57f06070, /// 0x688
			 0xe0bd93a4, /// 0x68c
			 0xe70035e5, /// 0x690
			 0x3dab4b69, /// 0x694
			 0x8898be6d, /// 0x698
			 0x408e67ea, /// 0x69c
			 0x8363e014, /// 0x6a0
			 0x89ab5672, /// 0x6a4
			 0x28d4fe24, /// 0x6a8
			 0xad9e1748, /// 0x6ac
			 0xd7962598, /// 0x6b0
			 0x841e519f, /// 0x6b4
			 0xc71706d6, /// 0x6b8
			 0xde609487, /// 0x6bc
			 0xdc04699a, /// 0x6c0
			 0x0acf56aa, /// 0x6c4
			 0x325fe24e, /// 0x6c8
			 0x036f651c, /// 0x6cc
			 0x9d51b82b, /// 0x6d0
			 0x5a5aa4da, /// 0x6d4
			 0x8850322e, /// 0x6d8
			 0x5f084134, /// 0x6dc
			 0x2597deb3, /// 0x6e0
			 0x91498864, /// 0x6e4
			 0x2cb5739f, /// 0x6e8
			 0xe7eb44d9, /// 0x6ec
			 0x5d4d69bb, /// 0x6f0
			 0xe4542f08, /// 0x6f4
			 0xbeeaefec, /// 0x6f8
			 0x4bab8add, /// 0x6fc
			 0x85ec672f, /// 0x700
			 0xce374386, /// 0x704
			 0x7f6f316a, /// 0x708
			 0x04c0b8f5, /// 0x70c
			 0x5cadb21a, /// 0x710
			 0x8dad07a5, /// 0x714
			 0x25f78a62, /// 0x718
			 0xa06611dd, /// 0x71c
			 0xbb8fcf87, /// 0x720
			 0xbfb4736d, /// 0x724
			 0xd8889ced, /// 0x728
			 0xf04aafdb, /// 0x72c
			 0x98703321, /// 0x730
			 0x40f6973d, /// 0x734
			 0x9e5a8495, /// 0x738
			 0xd1a329f8, /// 0x73c
			 0x1b7cf2f1, /// 0x740
			 0x3f698b92, /// 0x744
			 0x2c87a911, /// 0x748
			 0xe357ccfb, /// 0x74c
			 0x92a064f0, /// 0x750
			 0xe0fae527, /// 0x754
			 0xfd1988d5, /// 0x758
			 0xb48d01ab, /// 0x75c
			 0xdad02336, /// 0x760
			 0x3b202947, /// 0x764
			 0x834abd17, /// 0x768
			 0x72013276, /// 0x76c
			 0x724b6e11, /// 0x770
			 0x3845d7ad, /// 0x774
			 0x4dcfbaeb, /// 0x778
			 0xc85b9ca3, /// 0x77c
			 0xb39741b4, /// 0x780
			 0xf6d0a888, /// 0x784
			 0x7e431b73, /// 0x788
			 0xaf9f9152, /// 0x78c
			 0x2fa3ccb1, /// 0x790
			 0x75299119, /// 0x794
			 0x4e6d58a0, /// 0x798
			 0x9f922e0d, /// 0x79c
			 0xe2c510fe, /// 0x7a0
			 0xd6843f86, /// 0x7a4
			 0xb53c3c06, /// 0x7a8
			 0x044ab57f, /// 0x7ac
			 0x19040ed7, /// 0x7b0
			 0x8e38178f, /// 0x7b4
			 0xee002874, /// 0x7b8
			 0x7125daad, /// 0x7bc
			 0x759d180b, /// 0x7c0
			 0x541920b4, /// 0x7c4
			 0x0b669f00, /// 0x7c8
			 0x2d68ecd9, /// 0x7cc
			 0xf08c22db, /// 0x7d0
			 0xeb737d14, /// 0x7d4
			 0x32f5a968, /// 0x7d8
			 0x4b551d3e, /// 0x7dc
			 0x25c23bda, /// 0x7e0
			 0x23dc8ad7, /// 0x7e4
			 0xf8edf8a2, /// 0x7e8
			 0xbf8d3071, /// 0x7ec
			 0x73c8ea52, /// 0x7f0
			 0xb254ae3e, /// 0x7f4
			 0xc01dc03a, /// 0x7f8
			 0xae136058, /// 0x7fc
			 0x95071ae1, /// 0x800
			 0x8c6e13dc, /// 0x804
			 0xeec2b45f, /// 0x808
			 0x3f01949a, /// 0x80c
			 0x2bb52b2c, /// 0x810
			 0x9c59dd5a, /// 0x814
			 0x9ec5d0ad, /// 0x818
			 0x215766e9, /// 0x81c
			 0x53e30045, /// 0x820
			 0x08247d33, /// 0x824
			 0x6a8e49fb, /// 0x828
			 0x207fc0ed, /// 0x82c
			 0x2e5e9b7f, /// 0x830
			 0x771dd739, /// 0x834
			 0xabef775d, /// 0x838
			 0xc838981d, /// 0x83c
			 0x8cdf0d89, /// 0x840
			 0xdaa439a3, /// 0x844
			 0x06beb6db, /// 0x848
			 0x72f86030, /// 0x84c
			 0xb2d1c253, /// 0x850
			 0xb7d93410, /// 0x854
			 0x9aa8dfda, /// 0x858
			 0x118e4939, /// 0x85c
			 0xcdc3bd99, /// 0x860
			 0xd92a0615, /// 0x864
			 0xcab6cd89, /// 0x868
			 0x57adcbee, /// 0x86c
			 0x78d58b30, /// 0x870
			 0xfa2ed33c, /// 0x874
			 0xbf31d0f7, /// 0x878
			 0xb2d370aa, /// 0x87c
			 0x5f66829f, /// 0x880
			 0x672bbadb, /// 0x884
			 0x25c81bc0, /// 0x888
			 0x50b72eba, /// 0x88c
			 0x8d944c5e, /// 0x890
			 0x58876820, /// 0x894
			 0x8a444e6e, /// 0x898
			 0xdb23ee2e, /// 0x89c
			 0x85d8e220, /// 0x8a0
			 0xb7d40461, /// 0x8a4
			 0xf6abd6e1, /// 0x8a8
			 0x4baa5297, /// 0x8ac
			 0x4015a36b, /// 0x8b0
			 0x8510ea79, /// 0x8b4
			 0x9b0bcb05, /// 0x8b8
			 0xd24e9c27, /// 0x8bc
			 0x537cb30f, /// 0x8c0
			 0x8e458379, /// 0x8c4
			 0x7bcc1fa7, /// 0x8c8
			 0x8a8c29dc, /// 0x8cc
			 0x6d65e5d6, /// 0x8d0
			 0x9fe4e648, /// 0x8d4
			 0x63b50986, /// 0x8d8
			 0x1922fc32, /// 0x8dc
			 0x921ab877, /// 0x8e0
			 0x4d3b90bf, /// 0x8e4
			 0xfe817be6, /// 0x8e8
			 0x7ac7a5b1, /// 0x8ec
			 0xeda5d86c, /// 0x8f0
			 0xa05fa2c4, /// 0x8f4
			 0x675406a9, /// 0x8f8
			 0x0bc7c732, /// 0x8fc
			 0xae495e97, /// 0x900
			 0x4d3a7b9e, /// 0x904
			 0x0c46152d, /// 0x908
			 0x32be309e, /// 0x90c
			 0x4f4a31ee, /// 0x910
			 0x2f925c5f, /// 0x914
			 0xfe2205d8, /// 0x918
			 0x844d02e4, /// 0x91c
			 0x18f28a1c, /// 0x920
			 0x4a31e1f3, /// 0x924
			 0x54708889, /// 0x928
			 0x3768be7a, /// 0x92c
			 0xcde82831, /// 0x930
			 0x6220bb44, /// 0x934
			 0xeb151efe, /// 0x938
			 0xe2c316cf, /// 0x93c
			 0x7b13a473, /// 0x940
			 0x6926bbbf, /// 0x944
			 0x6d2251fb, /// 0x948
			 0x95ed1d67, /// 0x94c
			 0x0cf56b7a, /// 0x950
			 0xe564e460, /// 0x954
			 0xf108c87f, /// 0x958
			 0x966ac194, /// 0x95c
			 0x571d036a, /// 0x960
			 0x565cd1a0, /// 0x964
			 0x009d3f3a, /// 0x968
			 0x1e7cd58d, /// 0x96c
			 0xbdd5f1de, /// 0x970
			 0x172c226c, /// 0x974
			 0x308be87c, /// 0x978
			 0xdef3fde4, /// 0x97c
			 0x6e6b5296, /// 0x980
			 0x486f872e, /// 0x984
			 0x27d01022, /// 0x988
			 0x789f80ff, /// 0x98c
			 0x773b3bf9, /// 0x990
			 0x7900698b, /// 0x994
			 0x39c4da7a, /// 0x998
			 0x9d9145d1, /// 0x99c
			 0x12d08d23, /// 0x9a0
			 0x1a55af9c, /// 0x9a4
			 0xac4a8447, /// 0x9a8
			 0xd27dfc30, /// 0x9ac
			 0xe02b4628, /// 0x9b0
			 0x8306f4ea, /// 0x9b4
			 0x367fa76d, /// 0x9b8
			 0x6b793751, /// 0x9bc
			 0x08fd5b21, /// 0x9c0
			 0x482e2827, /// 0x9c4
			 0xef8bdaa8, /// 0x9c8
			 0x9cf4862e, /// 0x9cc
			 0x3325705b, /// 0x9d0
			 0xec2ec47f, /// 0x9d4
			 0xb82d8e35, /// 0x9d8
			 0x249d2634, /// 0x9dc
			 0x3ad3153c, /// 0x9e0
			 0xabfb49ec, /// 0x9e4
			 0x6bc08f57, /// 0x9e8
			 0xef9e6d86, /// 0x9ec
			 0x737a73d8, /// 0x9f0
			 0x13c1b7e1, /// 0x9f4
			 0xb0824d77, /// 0x9f8
			 0x26b885e1, /// 0x9fc
			 0x357aefa5, /// 0xa00
			 0xe94ee9ee, /// 0xa04
			 0x2cf5ad91, /// 0xa08
			 0x0e0fe798, /// 0xa0c
			 0xc1a1dd5d, /// 0xa10
			 0x7335e368, /// 0xa14
			 0x92a6bf39, /// 0xa18
			 0x4517242c, /// 0xa1c
			 0x66530c45, /// 0xa20
			 0xa557fba8, /// 0xa24
			 0x2d7672f7, /// 0xa28
			 0x191374dc, /// 0xa2c
			 0x3042d35d, /// 0xa30
			 0x64816630, /// 0xa34
			 0xb67cd1a5, /// 0xa38
			 0xaf0a36e6, /// 0xa3c
			 0x29f19718, /// 0xa40
			 0xccd2e398, /// 0xa44
			 0x53355a09, /// 0xa48
			 0x60cf5a9e, /// 0xa4c
			 0x6026f2fd, /// 0xa50
			 0xbba5bfa9, /// 0xa54
			 0x51787d2e, /// 0xa58
			 0xa2301799, /// 0xa5c
			 0xaa23da26, /// 0xa60
			 0x9b94f040, /// 0xa64
			 0xc09d4d59, /// 0xa68
			 0x2889f446, /// 0xa6c
			 0x3f8afc58, /// 0xa70
			 0x55f71377, /// 0xa74
			 0x2d460bf9, /// 0xa78
			 0x9e6709bc, /// 0xa7c
			 0x610e5664, /// 0xa80
			 0x91f8bfeb, /// 0xa84
			 0xa097fd73, /// 0xa88
			 0x6798ffe2, /// 0xa8c
			 0x6668b2ed, /// 0xa90
			 0x95ef82ed, /// 0xa94
			 0x742a9c5b, /// 0xa98
			 0x94cdb086, /// 0xa9c
			 0x250621b9, /// 0xaa0
			 0x656ffdae, /// 0xaa4
			 0x781e4bd6, /// 0xaa8
			 0x9b7292b7, /// 0xaac
			 0x435888cc, /// 0xab0
			 0xef1e0483, /// 0xab4
			 0x0dced00b, /// 0xab8
			 0x403c0c13, /// 0xabc
			 0xcd2a9cb3, /// 0xac0
			 0xd2a329de, /// 0xac4
			 0x09431529, /// 0xac8
			 0xaa869350, /// 0xacc
			 0x4c7e47d5, /// 0xad0
			 0xbcf797a1, /// 0xad4
			 0x218e8634, /// 0xad8
			 0x88f951ee, /// 0xadc
			 0xb3cf9d72, /// 0xae0
			 0x7974bb18, /// 0xae4
			 0x87ad7b12, /// 0xae8
			 0x1c98ff1e, /// 0xaec
			 0x6e0944a4, /// 0xaf0
			 0x9d0febd1, /// 0xaf4
			 0x0d76e2f8, /// 0xaf8
			 0xa6296429, /// 0xafc
			 0xeaf4c826, /// 0xb00
			 0x850402f8, /// 0xb04
			 0xe4003c92, /// 0xb08
			 0x0c8d2437, /// 0xb0c
			 0x8bef4cc9, /// 0xb10
			 0x971bada0, /// 0xb14
			 0x33e02eef, /// 0xb18
			 0x91961d89, /// 0xb1c
			 0x7a37d161, /// 0xb20
			 0x44d70b4b, /// 0xb24
			 0xec5d9c7a, /// 0xb28
			 0xf18c8775, /// 0xb2c
			 0xda0d4451, /// 0xb30
			 0xc03564f1, /// 0xb34
			 0xca1add63, /// 0xb38
			 0xbdc60701, /// 0xb3c
			 0xc24f0bf5, /// 0xb40
			 0xaf9e8ac7, /// 0xb44
			 0xd8390029, /// 0xb48
			 0x1adec5c7, /// 0xb4c
			 0xb34f4058, /// 0xb50
			 0x45a3adfa, /// 0xb54
			 0xe3ad6e21, /// 0xb58
			 0x8218958f, /// 0xb5c
			 0xe3e4267c, /// 0xb60
			 0xcb46f5c0, /// 0xb64
			 0x89772622, /// 0xb68
			 0x6f686631, /// 0xb6c
			 0xc906a4a9, /// 0xb70
			 0x7e3fabc3, /// 0xb74
			 0x029adf52, /// 0xb78
			 0xc33356a1, /// 0xb7c
			 0x36406d06, /// 0xb80
			 0x3ac223be, /// 0xb84
			 0x758bde10, /// 0xb88
			 0xb2192931, /// 0xb8c
			 0x95e0b8c6, /// 0xb90
			 0x403e9869, /// 0xb94
			 0xae9c273a, /// 0xb98
			 0x41cd4b14, /// 0xb9c
			 0x5a65a494, /// 0xba0
			 0x7f3309ed, /// 0xba4
			 0xc2caa892, /// 0xba8
			 0x9e88c507, /// 0xbac
			 0x2952e2e2, /// 0xbb0
			 0xf849322d, /// 0xbb4
			 0x19b26c32, /// 0xbb8
			 0x52fcd006, /// 0xbbc
			 0x807ce95a, /// 0xbc0
			 0x75440f6d, /// 0xbc4
			 0x69baeb8f, /// 0xbc8
			 0xb6fef803, /// 0xbcc
			 0x1c871d94, /// 0xbd0
			 0xb11b7982, /// 0xbd4
			 0xbf195948, /// 0xbd8
			 0x89687d8b, /// 0xbdc
			 0xa95719c6, /// 0xbe0
			 0xa940f12c, /// 0xbe4
			 0xa7722dab, /// 0xbe8
			 0xdbbe1cc9, /// 0xbec
			 0x4a5e0fad, /// 0xbf0
			 0x8f74ea4a, /// 0xbf4
			 0x23f61902, /// 0xbf8
			 0x81050523, /// 0xbfc
			 0x074579e5, /// 0xc00
			 0x71c9f326, /// 0xc04
			 0x47a8163f, /// 0xc08
			 0xd5424a00, /// 0xc0c
			 0x5f2e8cc0, /// 0xc10
			 0xc64d67d0, /// 0xc14
			 0xdcc3c477, /// 0xc18
			 0x3e38aac1, /// 0xc1c
			 0x3e26993e, /// 0xc20
			 0x8604bade, /// 0xc24
			 0xc6997442, /// 0xc28
			 0xec019ae8, /// 0xc2c
			 0x8789ed48, /// 0xc30
			 0x360b68e0, /// 0xc34
			 0xa87849e2, /// 0xc38
			 0x2611eae9, /// 0xc3c
			 0x58f2d972, /// 0xc40
			 0xde250285, /// 0xc44
			 0x6e429e1b, /// 0xc48
			 0x42edeebe, /// 0xc4c
			 0x7ea3180d, /// 0xc50
			 0xd78c7960, /// 0xc54
			 0x69d36d1a, /// 0xc58
			 0x834e2670, /// 0xc5c
			 0x10885cd8, /// 0xc60
			 0xbc0be689, /// 0xc64
			 0xc14aaf60, /// 0xc68
			 0x521d5869, /// 0xc6c
			 0x1863a4e0, /// 0xc70
			 0xcfbad9bb, /// 0xc74
			 0x9360f137, /// 0xc78
			 0xfa5b0e56, /// 0xc7c
			 0xd593a4af, /// 0xc80
			 0x26e9ed8b, /// 0xc84
			 0xc6ceefc1, /// 0xc88
			 0xdf0ecbc8, /// 0xc8c
			 0x745375fb, /// 0xc90
			 0x903a191a, /// 0xc94
			 0x79b21512, /// 0xc98
			 0xf56e7a63, /// 0xc9c
			 0x3a0aca22, /// 0xca0
			 0xbc216ea0, /// 0xca4
			 0x41757d9a, /// 0xca8
			 0x25f7fe6f, /// 0xcac
			 0x59a766dd, /// 0xcb0
			 0x814c80b1, /// 0xcb4
			 0xa39f4727, /// 0xcb8
			 0xaad15c13, /// 0xcbc
			 0xc3457b26, /// 0xcc0
			 0x193fa534, /// 0xcc4
			 0xd92dbdce, /// 0xcc8
			 0x0438b885, /// 0xccc
			 0x00d55ab7, /// 0xcd0
			 0x59e5b8cd, /// 0xcd4
			 0xe69c4d83, /// 0xcd8
			 0x01b5c0bd, /// 0xcdc
			 0x323040e7, /// 0xce0
			 0xcb631b24, /// 0xce4
			 0xbb31b666, /// 0xce8
			 0x42f91c1d, /// 0xcec
			 0x8dab429e, /// 0xcf0
			 0xd1d0e1a1, /// 0xcf4
			 0x9fad988e, /// 0xcf8
			 0xc1ddf2ad, /// 0xcfc
			 0x510bbec0, /// 0xd00
			 0x3a7b2562, /// 0xd04
			 0xb264b097, /// 0xd08
			 0xea297d03, /// 0xd0c
			 0x88b29dac, /// 0xd10
			 0xf6774fbb, /// 0xd14
			 0xd5c466a6, /// 0xd18
			 0x0a3886bf, /// 0xd1c
			 0x533d96bf, /// 0xd20
			 0xdc2f6d39, /// 0xd24
			 0xd1771095, /// 0xd28
			 0x9ac26fca, /// 0xd2c
			 0xca47e45b, /// 0xd30
			 0x51a116b0, /// 0xd34
			 0xa89fe242, /// 0xd38
			 0x54d270f6, /// 0xd3c
			 0xff2d1126, /// 0xd40
			 0x327d9509, /// 0xd44
			 0xb2319c44, /// 0xd48
			 0x711b550d, /// 0xd4c
			 0xc1b995de, /// 0xd50
			 0xed999f0e, /// 0xd54
			 0x598ccaf3, /// 0xd58
			 0xad5b12d1, /// 0xd5c
			 0xcbc33234, /// 0xd60
			 0x9aeaae96, /// 0xd64
			 0x5eda7374, /// 0xd68
			 0x063a8608, /// 0xd6c
			 0xef87d28c, /// 0xd70
			 0x032bb127, /// 0xd74
			 0xb496435d, /// 0xd78
			 0x1e798812, /// 0xd7c
			 0x60f01f2b, /// 0xd80
			 0x7c65edc6, /// 0xd84
			 0x0e69591c, /// 0xd88
			 0x9db28af1, /// 0xd8c
			 0xcfd51257, /// 0xd90
			 0xdfc1acf0, /// 0xd94
			 0xe231edae, /// 0xd98
			 0xaf2cd63b, /// 0xd9c
			 0x2feb8efb, /// 0xda0
			 0x7da5f3c4, /// 0xda4
			 0x1b671cac, /// 0xda8
			 0x25ec32d9, /// 0xdac
			 0xc1de5f17, /// 0xdb0
			 0xc468e0a8, /// 0xdb4
			 0x92933feb, /// 0xdb8
			 0x8b03b450, /// 0xdbc
			 0x5a24cfe1, /// 0xdc0
			 0xc65bd800, /// 0xdc4
			 0x71723d40, /// 0xdc8
			 0x03448826, /// 0xdcc
			 0xa6392539, /// 0xdd0
			 0xdd0df4bc, /// 0xdd4
			 0x54374dd5, /// 0xdd8
			 0x29d49e02, /// 0xddc
			 0x78fe24a0, /// 0xde0
			 0x9ed6e07f, /// 0xde4
			 0x521a31d8, /// 0xde8
			 0xb1cc1e89, /// 0xdec
			 0xff078071, /// 0xdf0
			 0x83660a1f, /// 0xdf4
			 0xe1396dcd, /// 0xdf8
			 0xab04acfa, /// 0xdfc
			 0xf17ccde7, /// 0xe00
			 0x21af277e, /// 0xe04
			 0xd63c0ae5, /// 0xe08
			 0x9dcee0e8, /// 0xe0c
			 0x80efcca8, /// 0xe10
			 0x240b4f80, /// 0xe14
			 0x16f1aeae, /// 0xe18
			 0x12d9891a, /// 0xe1c
			 0x60a0fac8, /// 0xe20
			 0x37c79c05, /// 0xe24
			 0x9cde4722, /// 0xe28
			 0xd0ec5a08, /// 0xe2c
			 0x624eb0a7, /// 0xe30
			 0x74906738, /// 0xe34
			 0xe713d9dc, /// 0xe38
			 0x932554bc, /// 0xe3c
			 0xa30121af, /// 0xe40
			 0x2bd7dc68, /// 0xe44
			 0xb004112b, /// 0xe48
			 0x6cfba9a5, /// 0xe4c
			 0xb82748db, /// 0xe50
			 0x9282b849, /// 0xe54
			 0x88f9308c, /// 0xe58
			 0xb10b5c7b, /// 0xe5c
			 0xfafde808, /// 0xe60
			 0xb16b396d, /// 0xe64
			 0x562598da, /// 0xe68
			 0x2f53bcd7, /// 0xe6c
			 0xe2e2aedd, /// 0xe70
			 0xba87fd83, /// 0xe74
			 0x3d04344b, /// 0xe78
			 0x4ce2a07f, /// 0xe7c
			 0xf304db0e, /// 0xe80
			 0xe335326d, /// 0xe84
			 0x5a05766b, /// 0xe88
			 0x87ff8399, /// 0xe8c
			 0x13eedf39, /// 0xe90
			 0xa39e7241, /// 0xe94
			 0xd49c7593, /// 0xe98
			 0xe7f7fab4, /// 0xe9c
			 0x4ed0767b, /// 0xea0
			 0x2a4bfb11, /// 0xea4
			 0x676dec85, /// 0xea8
			 0xbe572906, /// 0xeac
			 0xd53354da, /// 0xeb0
			 0xade67515, /// 0xeb4
			 0xf7e13e18, /// 0xeb8
			 0x4aff039e, /// 0xebc
			 0x4a03fbd3, /// 0xec0
			 0xf67049f5, /// 0xec4
			 0x4ded94ef, /// 0xec8
			 0x686576e6, /// 0xecc
			 0x566873c9, /// 0xed0
			 0x9b9e0e81, /// 0xed4
			 0xf17c6757, /// 0xed8
			 0xe3b7f812, /// 0xedc
			 0x8999729d, /// 0xee0
			 0x0de97a73, /// 0xee4
			 0x5b0a7fcc, /// 0xee8
			 0xade5dd66, /// 0xeec
			 0x3710b032, /// 0xef0
			 0xbc4dd895, /// 0xef4
			 0xf8342f67, /// 0xef8
			 0x485e51a5, /// 0xefc
			 0x95dcc32f, /// 0xf00
			 0x4b8c9322, /// 0xf04
			 0x6db91078, /// 0xf08
			 0x6ad6edd3, /// 0xf0c
			 0x088face9, /// 0xf10
			 0x76a164ed, /// 0xf14
			 0xa436b2fa, /// 0xf18
			 0x81de9724, /// 0xf1c
			 0xe8c5c314, /// 0xf20
			 0xb3ef661a, /// 0xf24
			 0xd7e60a21, /// 0xf28
			 0x7cf220fe, /// 0xf2c
			 0x93f7b18d, /// 0xf30
			 0xef4b9c2b, /// 0xf34
			 0xd558e042, /// 0xf38
			 0xe4b44b8b, /// 0xf3c
			 0x8875ab91, /// 0xf40
			 0xfadf93cc, /// 0xf44
			 0xd7293b08, /// 0xf48
			 0x1bf6e856, /// 0xf4c
			 0x1cb370c2, /// 0xf50
			 0xa8c5265e, /// 0xf54
			 0xda48ecca, /// 0xf58
			 0x0c7cca7f, /// 0xf5c
			 0x843b3152, /// 0xf60
			 0x0e74cd3c, /// 0xf64
			 0x9c6837ae, /// 0xf68
			 0x5ac81bd7, /// 0xf6c
			 0x52729d72, /// 0xf70
			 0xd2d55f04, /// 0xf74
			 0xce86949f, /// 0xf78
			 0x927a065f, /// 0xf7c
			 0x12af1366, /// 0xf80
			 0xdffe9fa3, /// 0xf84
			 0x416f6b4d, /// 0xf88
			 0xf1edef32, /// 0xf8c
			 0xf2c7811e, /// 0xf90
			 0xd534ab90, /// 0xf94
			 0x499cc108, /// 0xf98
			 0x785aec94, /// 0xf9c
			 0x1bb937b1, /// 0xfa0
			 0xb8d2b6dd, /// 0xfa4
			 0xdf5b7e84, /// 0xfa8
			 0xc34c8e7f, /// 0xfac
			 0x05255408, /// 0xfb0
			 0x73fbdee7, /// 0xfb4
			 0x27a0a894, /// 0xfb8
			 0xf26a4ffe, /// 0xfbc
			 0xcec47988, /// 0xfc0
			 0x69e99df7, /// 0xfc4
			 0x4e2564cb, /// 0xfc8
			 0x37440349, /// 0xfcc
			 0xea771d3e, /// 0xfd0
			 0x5b296819, /// 0xfd4
			 0x5a1ab12c, /// 0xfd8
			 0x4da44fd9, /// 0xfdc
			 0x16173b8a, /// 0xfe0
			 0xc04afacb, /// 0xfe4
			 0x9c037c6d, /// 0xfe8
			 0xf8f60ded, /// 0xfec
			 0xfcce1b69, /// 0xff0
			 0xfbef237f, /// 0xff4
			 0x7e1ae1de, /// 0xff8
			 0x84f5c309 /// last
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
			 0x00000638,
			 0x0000058c,
			 0x0000012c,
			 0x000000a8,
			 0x00000034,
			 0x000006d8,
			 0x0000007c,
			 0x000002c0,

			 /// vpu register f2
			 0x41880000,
			 0x41300000,
			 0x40400000,
			 0x41200000,
			 0x41400000,
			 0x41000000,
			 0x41a00000,
			 0x41e80000,

			 /// vpu register f3
			 0x41c80000,
			 0x41a80000,
			 0x3f800000,
			 0x41700000,
			 0x40e00000,
			 0x42000000,
			 0x41980000,
			 0x41600000,

			 /// vpu register f4
			 0x40c00000,
			 0x41b00000,
			 0x41980000,
			 0x40c00000,
			 0x41c80000,
			 0x41500000,
			 0x40800000,
			 0x41d80000,

			 /// vpu register f5
			 0x41980000,
			 0x41100000,
			 0x41b80000,
			 0x41b80000,
			 0x42000000,
			 0x41600000,
			 0x3f800000,
			 0x41000000,

			 /// vpu register f6
			 0x41a00000,
			 0x41c80000,
			 0x40a00000,
			 0x40c00000,
			 0x40a00000,
			 0x41f00000,
			 0x41980000,
			 0x41b80000,

			 /// vpu register f7
			 0x41b80000,
			 0x41000000,
			 0x41e00000,
			 0x41900000,
			 0x41f00000,
			 0x41880000,
			 0x40a00000,
			 0x41400000,

			 /// vpu register f8
			 0x42000000,
			 0x41700000,
			 0x41a00000,
			 0x41e00000,
			 0x41000000,
			 0x41a80000,
			 0x41300000,
			 0x41b80000,

			 /// vpu register f9
			 0x41100000,
			 0x40800000,
			 0x41f80000,
			 0x40e00000,
			 0x41b80000,
			 0x41980000,
			 0x41000000,
			 0x41c80000,

			 /// vpu register f10
			 0x42000000,
			 0x41f00000,
			 0x41b80000,
			 0x41b80000,
			 0x41980000,
			 0x41f00000,
			 0x41700000,
			 0x41b00000,

			 /// vpu register f11
			 0x41800000,
			 0x41880000,
			 0x41a00000,
			 0x41a00000,
			 0x41880000,
			 0x41700000,
			 0x41200000,
			 0x41900000,

			 /// vpu register f12
			 0x41a80000,
			 0x41400000,
			 0x41c00000,
			 0x40800000,
			 0x41a00000,
			 0x41b80000,
			 0x41980000,
			 0x41c80000,

			 /// vpu register f13
			 0x41300000,
			 0x3f800000,
			 0x40400000,
			 0x41a80000,
			 0x40a00000,
			 0x40a00000,
			 0x41600000,
			 0x41a00000,

			 /// vpu register f14
			 0x41800000,
			 0x41d80000,
			 0x41b00000,
			 0x41f00000,
			 0x41b00000,
			 0x40c00000,
			 0x41d00000,
			 0x41600000,

			 /// vpu register f15
			 0x41300000,
			 0x41c80000,
			 0x41880000,
			 0x41300000,
			 0x41c00000,
			 0x41100000,
			 0x41880000,
			 0x41300000,

			 /// vpu register f16
			 0x41d80000,
			 0x41100000,
			 0x41980000,
			 0x41c80000,
			 0x41e80000,
			 0x41f80000,
			 0x40000000,
			 0x41800000,

			 /// vpu register f17
			 0x41f00000,
			 0x41300000,
			 0x41a00000,
			 0x40000000,
			 0x41980000,
			 0x41d00000,
			 0x41a00000,
			 0x40c00000,

			 /// vpu register f18
			 0x40a00000,
			 0x41500000,
			 0x41d00000,
			 0x41e00000,
			 0x41c00000,
			 0x41000000,
			 0x41d80000,
			 0x41500000,

			 /// vpu register f19
			 0x41a00000,
			 0x41880000,
			 0x41b80000,
			 0x40c00000,
			 0x41a00000,
			 0x41e00000,
			 0x41e80000,
			 0x41b00000,

			 /// vpu register f20
			 0x41000000,
			 0x41400000,
			 0x41b80000,
			 0x41500000,
			 0x41b80000,
			 0x41800000,
			 0x41c80000,
			 0x3f800000,

			 /// vpu register f21
			 0x41000000,
			 0x41d00000,
			 0x41a80000,
			 0x40400000,
			 0x41700000,
			 0x41800000,
			 0x40c00000,
			 0x41100000,

			 /// vpu register f22
			 0x41c80000,
			 0x41a00000,
			 0x41000000,
			 0x41d00000,
			 0x40000000,
			 0x42000000,
			 0x40e00000,
			 0x41000000,

			 /// vpu register f23
			 0x41e80000,
			 0x41400000,
			 0x41900000,
			 0x40400000,
			 0x40c00000,
			 0x41c80000,
			 0x40a00000,
			 0x41e80000,

			 /// vpu register f24
			 0x41300000,
			 0x41e00000,
			 0x41980000,
			 0x41800000,
			 0x41a80000,
			 0x41300000,
			 0x40800000,
			 0x41e00000,

			 /// vpu register f25
			 0x3f800000,
			 0x41b80000,
			 0x41a00000,
			 0x41c00000,
			 0x41200000,
			 0x41200000,
			 0x41d80000,
			 0x41800000,

			 /// vpu register f26
			 0x41700000,
			 0x40800000,
			 0x41980000,
			 0x41300000,
			 0x40a00000,
			 0x40c00000,
			 0x41880000,
			 0x41000000,

			 /// vpu register f27
			 0x41e80000,
			 0x41c00000,
			 0x40c00000,
			 0x41d80000,
			 0x41700000,
			 0x41d00000,
			 0x41300000,
			 0x3f800000,

			 /// vpu register f28
			 0x41c80000,
			 0x40c00000,
			 0x3f800000,
			 0x41a00000,
			 0x40a00000,
			 0x41000000,
			 0x41300000,
			 0x41500000,

			 /// vpu register f29
			 0x41d00000,
			 0x41e80000,
			 0x41000000,
			 0x41b00000,
			 0x41000000,
			 0x41a00000,
			 0x41e80000,
			 0x40800000,

			 /// vpu register f30
			 0x3f800000,
			 0x40c00000,
			 0x41100000,
			 0x41500000,
			 0x41300000,
			 0x41700000,
			 0x41880000,
			 0x40a00000,

			 /// vpu register f31
			 0x41b80000,
			 0x41d00000,
			 0x41880000,
			 0x41c00000,
			 0x41f80000,
			 0x41e00000,
			 0x41a80000,
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
		"famoswapg.pi f30, f1 (x15)\n"                        ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f8, f1 (x14)\n"                         ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f10, f1 (x15)\n"                        ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f23, f1 (x15)\n"                        ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f7, f1 (x11)\n"                         ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f24, f1 (x14)\n"                        ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f9, f1 (x11)\n"                         ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f22, f1 (x13)\n"                        ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f23, f1 (x12)\n"                        ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f23, f1 (x12)\n"                        ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f30, f1 (x15)\n"                        ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f19, f1 (x13)\n"                        ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f13, f1 (x12)\n"                        ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f22, f1 (x13)\n"                        ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f9, f1 (x13)\n"                         ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f8, f1 (x14)\n"                         ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f21, f1 (x15)\n"                        ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f29, f1 (x15)\n"                        ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f27, f1 (x13)\n"                        ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f4, f1 (x13)\n"                         ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f23, f1 (x11)\n"                        ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f2, f1 (x12)\n"                         ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f13, f1 (x12)\n"                        ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f11, f1 (x14)\n"                        ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f12, f1 (x14)\n"                        ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f3, f1 (x15)\n"                         ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f9, f1 (x15)\n"                         ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f26, f1 (x13)\n"                        ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f7, f1 (x13)\n"                         ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f14, f1 (x13)\n"                        ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f13, f1 (x11)\n"                        ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f9, f1 (x11)\n"                         ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f13, f1 (x14)\n"                        ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f8, f1 (x11)\n"                         ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f15, f1 (x12)\n"                        ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f9, f1 (x15)\n"                         ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f26, f1 (x15)\n"                        ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f26, f1 (x12)\n"                        ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f18, f1 (x14)\n"                        ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f16, f1 (x15)\n"                        ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f30, f1 (x15)\n"                        ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f25, f1 (x14)\n"                        ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f25, f1 (x13)\n"                        ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f26, f1 (x14)\n"                        ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f18, f1 (x11)\n"                        ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f13, f1 (x13)\n"                        ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f30, f1 (x12)\n"                        ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f4, f1 (x12)\n"                         ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f27, f1 (x12)\n"                        ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f3, f1 (x14)\n"                         ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f28, f1 (x14)\n"                        ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f10, f1 (x11)\n"                        ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f9, f1 (x15)\n"                         ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f7, f1 (x12)\n"                         ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f13, f1 (x15)\n"                        ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f29, f1 (x14)\n"                        ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f15, f1 (x12)\n"                        ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f29, f1 (x12)\n"                        ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f30, f1 (x11)\n"                        ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f4, f1 (x15)\n"                         ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f7, f1 (x15)\n"                         ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f17, f1 (x14)\n"                        ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f8, f1 (x14)\n"                         ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f6, f1 (x12)\n"                         ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f30, f1 (x12)\n"                        ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f3, f1 (x11)\n"                         ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f8, f1 (x13)\n"                         ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f21, f1 (x13)\n"                        ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f19, f1 (x15)\n"                        ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f28, f1 (x12)\n"                        ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f14, f1 (x14)\n"                        ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f8, f1 (x11)\n"                         ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f13, f1 (x11)\n"                        ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f5, f1 (x15)\n"                         ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f8, f1 (x14)\n"                         ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f18, f1 (x14)\n"                        ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f20, f1 (x15)\n"                        ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f2, f1 (x11)\n"                         ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f6, f1 (x14)\n"                         ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f13, f1 (x11)\n"                        ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f27, f1 (x11)\n"                        ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f13, f1 (x14)\n"                        ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f2, f1 (x11)\n"                         ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f7, f1 (x13)\n"                         ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f11, f1 (x12)\n"                        ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f2, f1 (x13)\n"                         ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f12, f1 (x13)\n"                        ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f14, f1 (x13)\n"                        ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f29, f1 (x14)\n"                        ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f14, f1 (x14)\n"                        ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f18, f1 (x12)\n"                        ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f16, f1 (x13)\n"                        ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f12, f1 (x15)\n"                        ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f30, f1 (x14)\n"                        ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f5, f1 (x14)\n"                         ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f2, f1 (x15)\n"                         ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f27, f1 (x11)\n"                        ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f23, f1 (x11)\n"                        ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoswapg.pi f30, f1 (x13)\n"                        ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
