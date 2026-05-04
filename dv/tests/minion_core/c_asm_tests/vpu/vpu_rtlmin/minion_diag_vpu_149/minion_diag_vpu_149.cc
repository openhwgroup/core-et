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
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00004
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0xffc00001,                                                  // -signaling NaN                              /// 00014
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x80011111,                                                  // -9.7958E-41                                 /// 0001c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00020
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00028
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0002c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00030
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00034
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0003c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00040
			 0x0c780000,                                                  // Leading 1s:                                 /// 00044
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0004c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00050
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00058
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00060
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00068
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0006c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00074
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x00011111,                                                  // 9.7958E-41                                  /// 0007c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00080
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00084
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00088
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0008c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00090
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00098
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0009c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000b0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0xcb000000,                                                  // -8388608.0                                  /// 000b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000c0
			 0xff800000,                                                  // -inf                                        /// 000c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 000cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000d8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000f4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00100
			 0x0c780000,                                                  // Leading 1s:                                 /// 00104
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0010c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00110
			 0x4b000000,                                                  // 8388608.0                                   /// 00114
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00118
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0011c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00120
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00124
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00128
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00130
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00138
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0013c
			 0x7fc00001,                                                  // signaling NaN                               /// 00140
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00144
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00148
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0014c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0015c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00160
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00168
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0016c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00170
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00178
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00180
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00184
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00188
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0018c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00190
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00198
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x0e000003,                                                  // Trailing 1s:                                /// 001a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001b8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001c4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001d8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00200
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00204
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0020c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00210
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00214
			 0x0e000003,                                                  // Trailing 1s:                                /// 00218
			 0x33333333,                                                  // 4 random values                             /// 0021c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0xff800000,                                                  // -inf                                        /// 00224
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0xbf028f5c,                                                  // -0.51                                       /// 0022c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00230
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00240
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00248
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0024c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00250
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00254
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x4b000000,                                                  // 8388608.0                                   /// 0025c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00260
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00264
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00268
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0026c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00274
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0027c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00280
			 0x7f800000,                                                  // inf                                         /// 00284
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0029c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002a0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x55555555,                                                  // 4 random values                             /// 002a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002b4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 002c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002cc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002d4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0xcb000000,                                                  // -8388608.0                                  /// 002dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002e4
			 0x80000000,                                                  // -zero                                       /// 002e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002fc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00300
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00304
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0030c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00310
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0031c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00320
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00324
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0032c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0x7f800000,                                                  // inf                                         /// 00334
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00338
			 0x80011111,                                                  // -9.7958E-41                                 /// 0033c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00340
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00348
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0034c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00350
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00354
			 0x0e000003,                                                  // Trailing 1s:                                /// 00358
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0035c
			 0x33333333,                                                  // 4 random values                             /// 00360
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00364
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00370
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00374
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00378
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0037c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00380
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00384
			 0x0c700000,                                                  // Leading 1s:                                 /// 00388
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0038c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00390
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0039c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003a0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003a4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003ac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 003b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x7fc00001,                                                  // signaling NaN                               /// 003d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 003f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00400
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00404
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0040c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00410
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00414
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00418
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x55555555,                                                  // 4 random values                             /// 00420
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00424
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00428
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0042c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00434
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00438
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0043c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00440
			 0x00011111,                                                  // 9.7958E-41                                  /// 00444
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00448
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0044c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00450
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00458
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0045c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00460
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00468
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0046c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00474
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00480
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00484
			 0x0c400000,                                                  // Leading 1s:                                 /// 00488
			 0x0e000007,                                                  // Trailing 1s:                                /// 0048c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x33333333,                                                  // 4 random values                             /// 00498
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0049c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004a0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004a8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004b0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004b4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004c4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004cc
			 0x7fc00001,                                                  // signaling NaN                               /// 004d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004dc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004e8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004f4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00500
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00508
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0050c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00510
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00514
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00518
			 0x00011111,                                                  // 9.7958E-41                                  /// 0051c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00524
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00528
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0052c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00530
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00534
			 0x0e000001,                                                  // Trailing 1s:                                /// 00538
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0053c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00540
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00544
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00548
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00550
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00554
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00558
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00560
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00564
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0056c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00570
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00578
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0057c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00584
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00588
			 0x80000000,                                                  // -zero                                       /// 0058c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00590
			 0xffc00001,                                                  // -signaling NaN                              /// 00594
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00598
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0059c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005b4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005ec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00600
			 0xffc00001,                                                  // -signaling NaN                              /// 00604
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0060c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00610
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00618
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0061c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00630
			 0x3f028f5c,                                                  // 0.51                                        /// 00634
			 0x80011111,                                                  // -9.7958E-41                                 /// 00638
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0063c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00640
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00644
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00648
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0064c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00650
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00654
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0065c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00660
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00668
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0066c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00670
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00674
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00678
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0067c
			 0xcb000000,                                                  // -8388608.0                                  /// 00680
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00684
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00688
			 0x33333333,                                                  // 4 random values                             /// 0068c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00690
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00694
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006a8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006ac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006b4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006b8
			 0x0c700000,                                                  // Leading 1s:                                 /// 006bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0e000007,                                                  // Trailing 1s:                                /// 006d4
			 0x55555555,                                                  // 4 random values                             /// 006d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006ec
			 0xff800000,                                                  // -inf                                        /// 006f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006f4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00700
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00710
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00714
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00718
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0071c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00720
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00724
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00728
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00734
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00738
			 0x0e000003,                                                  // Trailing 1s:                                /// 0073c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00740
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00744
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0074c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00750
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00754
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x0e000003,                                                  // Trailing 1s:                                /// 0075c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00760
			 0xcb000000,                                                  // -8388608.0                                  /// 00764
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00768
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0076c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00770
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00778
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0077c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x7f800000,                                                  // inf                                         /// 00784
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0078c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00790
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00794
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00798
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0079c
			 0x0c700000,                                                  // Leading 1s:                                 /// 007a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007a8
			 0x3f028f5c,                                                  // 0.51                                        /// 007ac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x55555555,                                                  // 4 random values                             /// 007b4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007bc
			 0xffc00001,                                                  // -signaling NaN                              /// 007c0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007c4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007d0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00800
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00804
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00808
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00810
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00814
			 0x55555555,                                                  // 4 random values                             /// 00818
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0081c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00820
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0082c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00834
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00838
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0083c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00840
			 0xcb000000,                                                  // -8388608.0                                  /// 00844
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0084c
			 0x33333333,                                                  // 4 random values                             /// 00850
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00860
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00868
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00870
			 0x00000000,                                                  // zero                                        /// 00874
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00878
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0087c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00880
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00884
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0xffc00001,                                                  // -signaling NaN                              /// 0088c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00894
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00898
			 0x4b000000,                                                  // 8388608.0                                   /// 0089c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008ac
			 0x0e000003,                                                  // Trailing 1s:                                /// 008b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008b4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008bc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 008c4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 008e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00900
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00904
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00908
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0090c
			 0x4b000000,                                                  // 8388608.0                                   /// 00910
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00914
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00924
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0092c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00930
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00934
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00938
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0093c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00944
			 0xcb000000,                                                  // -8388608.0                                  /// 00948
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0094c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00958
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0095c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00964
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00968
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0096c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00970
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00974
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00978
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0097c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00980
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00984
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0098c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00990
			 0x3f028f5c,                                                  // 0.51                                        /// 00994
			 0x80011111,                                                  // -9.7958E-41                                 /// 00998
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0099c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009a0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009b8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009c4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009c8
			 0xff800000,                                                  // -inf                                        /// 009cc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009ec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a08
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a0c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a18
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a24
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a28
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a2c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a30
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a38
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a3c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a44
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a48
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a4c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a54
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a58
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a5c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a60
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a64
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a6c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a74
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a78
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a7c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a80
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a84
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a8c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a90
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a94
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a98
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a9c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00aa0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00aa4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00aa8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ab0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ab4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ab8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00abc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ac0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ac4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ad0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ad4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ad8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00adc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ae0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ae4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ae8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00af0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00af4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00afc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b04
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b0c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b10
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b18
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b1c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b24
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b28
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b2c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b38
			 0x3f028f5c,                                                  // 0.51                                        /// 00b3c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b44
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b48
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b4c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b5c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b68
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b70
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b74
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b78
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b88
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b9c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ba0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ba4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ba8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bbc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bc4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bc8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bd0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bd4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bd8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bdc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00be0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00be4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bf8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bfc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c00
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c0c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c14
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c18
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c1c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c28
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c2c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c30
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c34
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c38
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c3c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c40
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c4c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c50
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c58
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c5c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c60
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c64
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c6c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c74
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c78
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c7c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c80
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c84
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c88
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c8c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c90
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c9c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ca0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ca8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cb0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cb4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cc0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cc4
			 0xff800000,                                                  // -inf                                        /// 00cc8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cd0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cd4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cd8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ce0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ce8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cf0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cf8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cfc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d0c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d10
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d18
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d1c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d20
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d24
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d2c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d30
			 0xff800000,                                                  // -inf                                        /// 00d34
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d40
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0xff800000,                                                  // -inf                                        /// 00d48
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d4c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d54
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d58
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d64
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d7c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d80
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d84
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d88
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d8c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d90
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d94
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d98
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d9c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00da4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00db4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00db8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dbc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dc4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dc8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dcc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dd0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dd4
			 0x7f800000,                                                  // inf                                         /// 00dd8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ddc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00de4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00df0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00df4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00df8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00dfc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e00
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e08
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e10
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x55555555,                                                  // 4 random values                             /// 00e1c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e20
			 0x80000000,                                                  // -zero                                       /// 00e24
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e30
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e38
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x55555555,                                                  // 4 random values                             /// 00e44
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e48
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e4c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e54
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e58
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e60
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e68
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e6c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e70
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e74
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e78
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e80
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e84
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e88
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e8c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e90
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e98
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ea4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ea8
			 0x7f800000,                                                  // inf                                         /// 00eac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eb4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ebc
			 0x3f028f5c,                                                  // 0.51                                        /// 00ec0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ec4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ec8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ecc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ed4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ed8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00edc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ee4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00eec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ef0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ef4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ef8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00efc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f04
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f08
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f14
			 0x7fc00001,                                                  // signaling NaN                               /// 00f18
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f24
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f28
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f2c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f30
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f34
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f3c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f44
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f48
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f4c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f50
			 0xff800000,                                                  // -inf                                        /// 00f54
			 0x7f800000,                                                  // inf                                         /// 00f58
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f5c
			 0x33333333,                                                  // 4 random values                             /// 00f60
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f64
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f68
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f70
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f74
			 0x4b000000,                                                  // 8388608.0                                   /// 00f78
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f7c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f84
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f88
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f90
			 0xffc00001,                                                  // -signaling NaN                              /// 00f94
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f98
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f9c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fa0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fa4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fa8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fb0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fb4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fb8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fbc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fc0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fc4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fc8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fcc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fd4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fd8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fdc
			 0x80000000,                                                  // -zero                                       /// 00fe0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fe4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ff4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ff8
			 0x00000080                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00000
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00008
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x7fc00001,                                                  // signaling NaN                               /// 00010
			 0x4b000000,                                                  // 8388608.0                                   /// 00014
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00018
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0001c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00024
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00028
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00030
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00034
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00038
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00044
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00048
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0004c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00050
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00054
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00058
			 0x0c700000,                                                  // Leading 1s:                                 /// 0005c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00064
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00068
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0006c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00070
			 0x33333333,                                                  // 4 random values                             /// 00074
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00080
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00084
			 0xcb000000,                                                  // -8388608.0                                  /// 00088
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0008c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00090
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00094
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00098
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0009c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000a0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000ac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000bc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000c0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000d8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000dc
			 0xcb000000,                                                  // -8388608.0                                  /// 000e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000f0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00100
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00110
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00114
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00118
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0011c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00120
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00128
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0012c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00130
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00134
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00138
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00140
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00148
			 0x0c600000,                                                  // Leading 1s:                                 /// 0014c
			 0x80000000,                                                  // -zero                                       /// 00150
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00154
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0015c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00164
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00168
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0016c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00174
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00178
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0017c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00180
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0018c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00190
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00194
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00198
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0019c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001a0
			 0x55555555,                                                  // 4 random values                             /// 001a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 001a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001bc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001dc
			 0x7f800000,                                                  // inf                                         /// 001e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001e8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001fc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00200
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00204
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00208
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00210
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00214
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00218
			 0x55555555,                                                  // 4 random values                             /// 0021c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00220
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00224
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00228
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00234
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0023c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00244
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0024c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00250
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00254
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00258
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0025c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00264
			 0x33333333,                                                  // 4 random values                             /// 00268
			 0x0e000007,                                                  // Trailing 1s:                                /// 0026c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00270
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0027c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00280
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00284
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0028c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00290
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00294
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00298
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0029c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002b8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002bc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002c4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002c8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002cc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002d8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002dc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002e0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002f0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002fc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00300
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00308
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0030c
			 0x3f028f5c,                                                  // 0.51                                        /// 00310
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00318
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0031c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00320
			 0x80000000,                                                  // -zero                                       /// 00324
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00328
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00330
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00338
			 0x0c700000,                                                  // Leading 1s:                                 /// 0033c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00340
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00344
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00350
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x0e000001,                                                  // Trailing 1s:                                /// 0035c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00360
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00364
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x33333333,                                                  // 4 random values                             /// 0036c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00370
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00378
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0037c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00380
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00384
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00388
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0038c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00390
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00398
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0039c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003a8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003b4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 003c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003cc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003e8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003f8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00404
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00410
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00414
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00418
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0041c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00428
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0042c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00430
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00434
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00438
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00440
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00444
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00448
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0044c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00450
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00454
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00460
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00464
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00468
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0046c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00470
			 0xff800000,                                                  // -inf                                        /// 00474
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00478
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0047c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00480
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00484
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0048c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00490
			 0xcb000000,                                                  // -8388608.0                                  /// 00494
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00498
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0049c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004a4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004a8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004b0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004b4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004b8
			 0x80011111,                                                  // -9.7958E-41                                 /// 004bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 004c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004c8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x3f028f5c,                                                  // 0.51                                        /// 004d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004ec
			 0x33333333,                                                  // 4 random values                             /// 004f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 004f8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00504
			 0x0e000003,                                                  // Trailing 1s:                                /// 00508
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00510
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00514
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00518
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x0c600000,                                                  // Leading 1s:                                 /// 00524
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00528
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0052c
			 0xffc00001,                                                  // -signaling NaN                              /// 00530
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00534
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00538
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00540
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00544
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0054c
			 0xff800000,                                                  // -inf                                        /// 00550
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00554
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00558
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0055c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00560
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00568
			 0x4b000000,                                                  // 8388608.0                                   /// 0056c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00570
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0xff800000,                                                  // -inf                                        /// 00578
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0057c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00584
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00594
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00598
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0059c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005b0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005bc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005c0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005d8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005e4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00600
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00604
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0060c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00610
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0061c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00620
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00624
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0062c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00638
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0063c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00640
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00644
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00648
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0064c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00650
			 0x0c780000,                                                  // Leading 1s:                                 /// 00654
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00658
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0065c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00660
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00664
			 0x3f028f5c,                                                  // 0.51                                        /// 00668
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00670
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00678
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0067c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00680
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00684
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00688
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0068c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00694
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00698
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0xffc00001,                                                  // -signaling NaN                              /// 006a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 006a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006b0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006bc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006c0
			 0x00000000,                                                  // zero                                        /// 006c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006d4
			 0x3f028f5c,                                                  // 0.51                                        /// 006d8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006f4
			 0x7f800000,                                                  // inf                                         /// 006f8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006fc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00700
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00708
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0070c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00714
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0071c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00720
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00724
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00728
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0072c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00734
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x0e000001,                                                  // Trailing 1s:                                /// 00744
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00748
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00750
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00754
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0075c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00760
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00768
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0076c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00770
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00778
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0077c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00780
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x3f028f5c,                                                  // 0.51                                        /// 0078c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00790
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00794
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0079c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007ac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007b4
			 0x00011111,                                                  // 9.7958E-41                                  /// 007b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007bc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007c0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007d0
			 0x55555555,                                                  // 4 random values                             /// 007d4
			 0x7fc00001,                                                  // signaling NaN                               /// 007d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007dc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007e0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00800
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00808
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0080c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00810
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0081c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00820
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00824
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00828
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0082c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00830
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00834
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00838
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0083c
			 0x4b000000,                                                  // 8388608.0                                   /// 00840
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00844
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00848
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0084c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00854
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00858
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0085c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00860
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00864
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00868
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00874
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x0e000001,                                                  // Trailing 1s:                                /// 0087c
			 0xff800000,                                                  // -inf                                        /// 00880
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00890
			 0x80011111,                                                  // -9.7958E-41                                 /// 00894
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00898
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0089c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008b0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008dc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008e0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008e4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00900
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00904
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00908
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0090c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00910
			 0x55555555,                                                  // 4 random values                             /// 00914
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00918
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0091c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00924
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00928
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00930
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00934
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0093c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0094c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00950
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00958
			 0x7fc00001,                                                  // signaling NaN                               /// 0095c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00960
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00968
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0096c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00974
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00978
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0097c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00980
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00984
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00988
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00994
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00998
			 0x0c700000,                                                  // Leading 1s:                                 /// 0099c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x0e000003,                                                  // Trailing 1s:                                /// 009a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x00000000,                                                  // zero                                        /// 009ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009b0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009b4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009c4
			 0xffc00001,                                                  // -signaling NaN                              /// 009c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 009cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009d0
			 0xcb000000,                                                  // -8388608.0                                  /// 009d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009ec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a00
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a08
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a0c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a14
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a1c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a28
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a30
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a38
			 0xbf028f5c,                                                  // -0.51                                       /// 00a3c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a40
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a44
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x4b000000,                                                  // 8388608.0                                   /// 00a4c
			 0x55555555,                                                  // 4 random values                             /// 00a50
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a54
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a58
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a5c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a60
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a64
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a6c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a70
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a74
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a78
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a7c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a80
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a8c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a94
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a9c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00aa0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00aa8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ab4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ab8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00abc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ac0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x00000000,                                                  // zero                                        /// 00acc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ad0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ad4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ad8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00adc
			 0xff800000,                                                  // -inf                                        /// 00ae0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ae4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ae8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00af0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00af4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00afc
			 0x3f028f5c,                                                  // 0.51                                        /// 00b00
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b04
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b08
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b10
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b14
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b18
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x4b000000,                                                  // 8388608.0                                   /// 00b20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b24
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b28
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b2c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b30
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b38
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b3c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b40
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b4c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b50
			 0x80000000,                                                  // -zero                                       /// 00b54
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b58
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b5c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b60
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b64
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b68
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b6c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b70
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b78
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b7c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b80
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b84
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b88
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b90
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b98
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b9c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ba4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bb0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bb4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bb8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bbc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bcc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bd0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bd4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bd8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bdc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00be0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00be4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00be8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bf0
			 0xbf028f5c,                                                  // -0.51                                       /// 00bf4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bf8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c00
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c04
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c0c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c14
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c1c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c2c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c30
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c34
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c38
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c40
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c48
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c4c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c54
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c58
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c5c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c60
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c64
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c68
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c6c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c70
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c74
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c78
			 0xbf028f5c,                                                  // -0.51                                       /// 00c7c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c80
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c84
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c8c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c90
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c94
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c9c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cb0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cb4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cc0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cc4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ccc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cd0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cd4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cdc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ce4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ce8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cf0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cf4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cf8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cfc
			 0x80000000,                                                  // -zero                                       /// 00d00
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d04
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d08
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d14
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d18
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d1c
			 0xbf028f5c,                                                  // -0.51                                       /// 00d20
			 0xcb000000,                                                  // -8388608.0                                  /// 00d24
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d28
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d2c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d34
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d38
			 0x7f800000,                                                  // inf                                         /// 00d3c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d44
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d48
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d4c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d50
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d58
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d60
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d64
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d68
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d6c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d70
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d74
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d78
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d80
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d84
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d88
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d94
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d98
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00da0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00da8
			 0x3f028f5c,                                                  // 0.51                                        /// 00dac
			 0x80011111,                                                  // -9.7958E-41                                 /// 00db0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00dbc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dc0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00dc4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dc8
			 0x7f800000,                                                  // inf                                         /// 00dcc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dd0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dd4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dd8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ddc
			 0x55555555,                                                  // 4 random values                             /// 00de0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x33333333,                                                  // 4 random values                             /// 00de8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00df0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00df4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00df8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e00
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e04
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e08
			 0x55555555,                                                  // 4 random values                             /// 00e0c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e10
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e18
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e1c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e20
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e24
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e28
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e2c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e30
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e38
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e3c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e40
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e44
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e48
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e4c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e50
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e60
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e64
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e68
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e74
			 0x33333333,                                                  // 4 random values                             /// 00e78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e80
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e84
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e88
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e90
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e94
			 0x7fc00001,                                                  // signaling NaN                               /// 00e98
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e9c
			 0x7f800000,                                                  // inf                                         /// 00ea0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ea4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ea8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00eac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00eb0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00eb8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ec0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ec8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ecc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ed0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ed4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ed8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00edc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ee8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ef0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ef4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x4b000000,                                                  // 8388608.0                                   /// 00f00
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f04
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f08
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f0c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f10
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f14
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f1c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f24
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f28
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f2c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f30
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f3c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f44
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f48
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0x3f028f5c,                                                  // 0.51                                        /// 00f50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f54
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f64
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f68
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f6c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f70
			 0xff800000,                                                  // -inf                                        /// 00f74
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f78
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f7c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f88
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f8c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f90
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f9c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fa4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fa8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fb0
			 0x80000000,                                                  // -zero                                       /// 00fb4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fb8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fbc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fc0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x00000000,                                                  // zero                                        /// 00fcc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fd0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fd4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fd8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fe0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fe8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ff0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ff4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ff8
			 0x0c7ffff8                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00000
			 0x0c600000,                                                  // Leading 1s:                                 /// 00004
			 0x33333333,                                                  // 4 random values                             /// 00008
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0000c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00010
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00014
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00020
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00024
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00028
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0002c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00034
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00038
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0003c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00040
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x7fc00001,                                                  // signaling NaN                               /// 00048
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0xcb000000,                                                  // -8388608.0                                  /// 00050
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0005c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00060
			 0x0e000003,                                                  // Trailing 1s:                                /// 00064
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00068
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00070
			 0x80000000,                                                  // -zero                                       /// 00074
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0007c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00084
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00088
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00090
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00094
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00098
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000a0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000b4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000bc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 000cc
			 0x7fc00001,                                                  // signaling NaN                               /// 000d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000d4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000dc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000f0
			 0x3f028f5c,                                                  // 0.51                                        /// 000f4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00100
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00108
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0010c
			 0x7f800000,                                                  // inf                                         /// 00110
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00114
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00118
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0011c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00124
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00128
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0012c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0013c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00148
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0014c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00154
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0015c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x80000000,                                                  // -zero                                       /// 00164
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00168
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00170
			 0xcb000000,                                                  // -8388608.0                                  /// 00174
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00178
			 0x0c700000,                                                  // Leading 1s:                                 /// 0017c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00180
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00188
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00198
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001b0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001b4
			 0xcb000000,                                                  // -8388608.0                                  /// 001b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001c0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001c4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001c8
			 0x0e000007,                                                  // Trailing 1s:                                /// 001cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001d8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 001e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 001e8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001f4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00204
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00208
			 0x80011111,                                                  // -9.7958E-41                                 /// 0020c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00210
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00218
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00224
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00228
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00234
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00240
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00244
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00248
			 0x7f800000,                                                  // inf                                         /// 0024c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00254
			 0x00000000,                                                  // zero                                        /// 00258
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00260
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00264
			 0x7fc00001,                                                  // signaling NaN                               /// 00268
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0026c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00274
			 0x0c400000,                                                  // Leading 1s:                                 /// 00278
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0027c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00284
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00288
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00290
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00294
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00298
			 0x0c400000,                                                  // Leading 1s:                                 /// 0029c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002a0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 002a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002b0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002b8
			 0x00000000,                                                  // zero                                        /// 002bc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 002c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002c8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x80000000,                                                  // -zero                                       /// 002d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 002fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00300
			 0x0c780000,                                                  // Leading 1s:                                 /// 00304
			 0x0e000001,                                                  // Trailing 1s:                                /// 00308
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00310
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00314
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0031c
			 0xff800000,                                                  // -inf                                        /// 00320
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00324
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00328
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00330
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00338
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0033c
			 0x80000000,                                                  // -zero                                       /// 00340
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00344
			 0x7f800000,                                                  // inf                                         /// 00348
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00350
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00354
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00358
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0035c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00360
			 0x0e000007,                                                  // Trailing 1s:                                /// 00364
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00368
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0036c
			 0x3f028f5c,                                                  // 0.51                                        /// 00370
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00378
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00380
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00384
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00388
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0038c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00390
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00394
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00398
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0039c
			 0x0e000003,                                                  // Trailing 1s:                                /// 003a0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003ac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003bc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003cc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003f4
			 0x0e000003,                                                  // Trailing 1s:                                /// 003f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00400
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0xffc00001,                                                  // -signaling NaN                              /// 0040c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00410
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00414
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00418
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0041c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00420
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00424
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0042c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00430
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00434
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00440
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00444
			 0x0e000007,                                                  // Trailing 1s:                                /// 00448
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00454
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0045c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00460
			 0x00011111,                                                  // 9.7958E-41                                  /// 00464
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0046c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00474
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00478
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0047c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00484
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x4b000000,                                                  // 8388608.0                                   /// 0048c
			 0x3f028f5c,                                                  // 0.51                                        /// 00490
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0049c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004b4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004bc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004c4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004c8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004e0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004e4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004f4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004f8
			 0x0c780000,                                                  // Leading 1s:                                 /// 004fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00500
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00504
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00508
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x80000000,                                                  // -zero                                       /// 00510
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00514
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00518
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0051c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00520
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00524
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0052c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00534
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0053c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00540
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00548
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00550
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00554
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00558
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00564
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00568
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00570
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00574
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00578
			 0x0c700000,                                                  // Leading 1s:                                 /// 0057c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00580
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00584
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00588
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0058c
			 0xcb000000,                                                  // -8388608.0                                  /// 00590
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00594
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00598
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0059c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x7f800000,                                                  // inf                                         /// 005ac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005b0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005bc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005d4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005d8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005dc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005fc
			 0x33333333,                                                  // 4 random values                             /// 00600
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00604
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00608
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x00000000,                                                  // zero                                        /// 0061c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00624
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00628
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00630
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00634
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00638
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0063c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00640
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00644
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x80000000,                                                  // -zero                                       /// 00650
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00654
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0065c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x0e000003,                                                  // Trailing 1s:                                /// 00664
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00668
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00670
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0067c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00680
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00684
			 0x7f800000,                                                  // inf                                         /// 00688
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00690
			 0xcb000000,                                                  // -8388608.0                                  /// 00694
			 0x0c780000,                                                  // Leading 1s:                                 /// 00698
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 006b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006b8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006ec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 006f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006f8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00700
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00704
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00708
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0xcb000000,                                                  // -8388608.0                                  /// 00710
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00714
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00718
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0071c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00720
			 0x00011111,                                                  // 9.7958E-41                                  /// 00724
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00728
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00734
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00738
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00748
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0074c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0xcb000000,                                                  // -8388608.0                                  /// 00754
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00760
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00768
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0076c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00770
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00774
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00778
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00780
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00784
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00788
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00790
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0079c
			 0x0c780000,                                                  // Leading 1s:                                 /// 007a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007b0
			 0xbf028f5c,                                                  // -0.51                                       /// 007b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007b8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007c8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007d0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00804
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0080c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00814
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00818
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00824
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00828
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00834
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00838
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0083c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00840
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00844
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00848
			 0x0c400000,                                                  // Leading 1s:                                 /// 0084c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00850
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0085c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00860
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00864
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00868
			 0x00011111,                                                  // 9.7958E-41                                  /// 0086c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00870
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00874
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00878
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0087c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00880
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00884
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00888
			 0x33333333,                                                  // 4 random values                             /// 0088c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00894
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0xcb000000,                                                  // -8388608.0                                  /// 0089c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008a8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 008b4
			 0x00011111,                                                  // 9.7958E-41                                  /// 008b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008c8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008e8
			 0x3f028f5c,                                                  // 0.51                                        /// 008ec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00900
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00908
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00910
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00918
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x33333333,                                                  // 4 random values                             /// 00924
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00928
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0092c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00930
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00934
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00938
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0093c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00940
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00948
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0094c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00950
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00954
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00958
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00960
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00964
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00968
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0096c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00974
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00978
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0097c
			 0x00000000,                                                  // zero                                        /// 00980
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00984
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0098c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00994
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00998
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0099c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009a0
			 0xcb000000,                                                  // -8388608.0                                  /// 009a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009ac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 009cc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009d8
			 0x7fc00001,                                                  // signaling NaN                               /// 009dc
			 0x0e000001,                                                  // Trailing 1s:                                /// 009e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009f8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a00
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a04
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a08
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a14
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x55555555,                                                  // 4 random values                             /// 00a20
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0xff800000,                                                  // -inf                                        /// 00a2c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a30
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a3c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a44
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a4c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a50
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a58
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a5c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a60
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a6c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a70
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a74
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a84
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a8c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a94
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a98
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a9c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00aa8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00aac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ab4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ab8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00abc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ac4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ac8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00acc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ad0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00adc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ae8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00af0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00af4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00afc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b00
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b04
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b08
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b10
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b14
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b18
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b1c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b20
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b24
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b28
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b30
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b38
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b3c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b40
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b44
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b4c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b50
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b58
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b5c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b64
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b68
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b6c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b70
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b74
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b78
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b7c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b80
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0x55555555,                                                  // 4 random values                             /// 00b88
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b8c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b90
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b94
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b98
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b9c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ba0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ba4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ba8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bb0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bb4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bb8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bbc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0xffc00001,                                                  // -signaling NaN                              /// 00bc4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bc8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bd0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bd4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0xffc00001,                                                  // -signaling NaN                              /// 00be4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bf0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bf8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bfc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c00
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c0c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c10
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c18
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c24
			 0x7fc00001,                                                  // signaling NaN                               /// 00c28
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c2c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c30
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c38
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c3c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c40
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c44
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c48
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c4c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c50
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c58
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c5c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c60
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c64
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c68
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c74
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c78
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c7c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c80
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c84
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c88
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x80000000,                                                  // -zero                                       /// 00c9c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ca8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cb4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cb8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cbc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cc0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cc8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cd0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cd4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cd8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cdc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x55555555,                                                  // 4 random values                             /// 00ce4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cf0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d08
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d0c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d10
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d18
			 0x55555555,                                                  // 4 random values                             /// 00d1c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d24
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d28
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d2c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d30
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d34
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d38
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d3c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d40
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d44
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d50
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d54
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d58
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d5c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d60
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d68
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d6c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d74
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d78
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d80
			 0x7f800000,                                                  // inf                                         /// 00d84
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d88
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d8c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d90
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d94
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d98
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d9c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00da0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00da4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00da8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00db0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00db4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00db8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dc8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dcc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00dd0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dd4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dd8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ddc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00de0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00de4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00de8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00df0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00df4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00df8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dfc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e00
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e04
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e08
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e0c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e10
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e14
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e1c
			 0xbf028f5c,                                                  // -0.51                                       /// 00e20
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e24
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e34
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e38
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e3c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e40
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e44
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e48
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e4c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e68
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e6c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e70
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e74
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e78
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e80
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e8c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e90
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e94
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e98
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e9c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ea0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ea4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ea8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00eac
			 0x0c600000,                                                  // Leading 1s:                                 /// 00eb0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0xcb000000,                                                  // -8388608.0                                  /// 00eb8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ec0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ec8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ecc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ed0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ed4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ed8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00edc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ee0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ee4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00eec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ef0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ef4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ef8
			 0x80000000,                                                  // -zero                                       /// 00efc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f00
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f08
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f0c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f10
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f14
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f18
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f1c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f28
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f30
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x7fc00001,                                                  // signaling NaN                               /// 00f38
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f40
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f50
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f58
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f5c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f60
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f68
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f6c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f70
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f74
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f78
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f7c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f80
			 0x4b000000,                                                  // 8388608.0                                   /// 00f84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f8c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f90
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f94
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f98
			 0x7f800000,                                                  // inf                                         /// 00f9c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fa0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fa4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fa8
			 0xffc00001,                                                  // -signaling NaN                              /// 00fac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fb0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fb8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fbc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fc0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fc4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fcc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fd0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fd4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fe4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fe8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ff4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ff8
			 0x0c700000                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x412226dc, /// 0x0
			 0x49fea2a4, /// 0x4
			 0x4b9b3aae, /// 0x8
			 0x977febbd, /// 0xc
			 0x37bb3dbb, /// 0x10
			 0x39390b09, /// 0x14
			 0x9a0e8621, /// 0x18
			 0xc2d07077, /// 0x1c
			 0x91e38a6a, /// 0x20
			 0x4da6ef04, /// 0x24
			 0x961c6966, /// 0x28
			 0xdc117285, /// 0x2c
			 0x327403e5, /// 0x30
			 0xa1b84126, /// 0x34
			 0xa26bb4b1, /// 0x38
			 0xe230f646, /// 0x3c
			 0xfcc0c71f, /// 0x40
			 0x9296d328, /// 0x44
			 0xfde5b13a, /// 0x48
			 0x66cef4fc, /// 0x4c
			 0xe94c9ff1, /// 0x50
			 0xa894c265, /// 0x54
			 0x46394f98, /// 0x58
			 0x31c4b410, /// 0x5c
			 0x63d7afd8, /// 0x60
			 0x5d5db540, /// 0x64
			 0x5170f72b, /// 0x68
			 0x602652c9, /// 0x6c
			 0x23f16e16, /// 0x70
			 0x67562f46, /// 0x74
			 0x8870f48d, /// 0x78
			 0xebc8c422, /// 0x7c
			 0x45d1acdc, /// 0x80
			 0x37acd4e6, /// 0x84
			 0xb1f1feb3, /// 0x88
			 0xaf38906b, /// 0x8c
			 0x91189f83, /// 0x90
			 0x02abc627, /// 0x94
			 0xedc468af, /// 0x98
			 0x0a3a8d04, /// 0x9c
			 0x5b334a22, /// 0xa0
			 0x9f4a7a4f, /// 0xa4
			 0xa8db7253, /// 0xa8
			 0xaaa9fc0e, /// 0xac
			 0x387c0f4e, /// 0xb0
			 0xa0df68f5, /// 0xb4
			 0x9b8c7e32, /// 0xb8
			 0x18ca53d4, /// 0xbc
			 0xf3ca7bc8, /// 0xc0
			 0x8e6d59d2, /// 0xc4
			 0xf591c6c1, /// 0xc8
			 0xf05b5a31, /// 0xcc
			 0xc4a8d221, /// 0xd0
			 0x02bc7e6c, /// 0xd4
			 0x4d704f08, /// 0xd8
			 0x4d4b8edb, /// 0xdc
			 0xb34cb6e1, /// 0xe0
			 0xee844a89, /// 0xe4
			 0xbffa3465, /// 0xe8
			 0x720a4f8f, /// 0xec
			 0xb72c6945, /// 0xf0
			 0xf1fa7638, /// 0xf4
			 0xf559bf06, /// 0xf8
			 0x22cbd26b, /// 0xfc
			 0x225868b5, /// 0x100
			 0xee92f185, /// 0x104
			 0x590ee221, /// 0x108
			 0xfa2e0f65, /// 0x10c
			 0x018f115e, /// 0x110
			 0xfd13fbdd, /// 0x114
			 0xed7a856c, /// 0x118
			 0x8577dd99, /// 0x11c
			 0xbe62f48a, /// 0x120
			 0x9d038116, /// 0x124
			 0x34d3acd6, /// 0x128
			 0x5860f8d0, /// 0x12c
			 0x4ec8ed61, /// 0x130
			 0x35f5097e, /// 0x134
			 0x1eb58935, /// 0x138
			 0x219a33e6, /// 0x13c
			 0xbc46412c, /// 0x140
			 0x61c5807c, /// 0x144
			 0x7a351c29, /// 0x148
			 0xba11f2db, /// 0x14c
			 0xde41d93f, /// 0x150
			 0x17961fbf, /// 0x154
			 0x43d075fb, /// 0x158
			 0x7a5e1541, /// 0x15c
			 0x9319e93f, /// 0x160
			 0xcbb98478, /// 0x164
			 0xb8728413, /// 0x168
			 0xfcb99139, /// 0x16c
			 0x196d3257, /// 0x170
			 0x66f182c3, /// 0x174
			 0x70c1b4f1, /// 0x178
			 0x08c0c2af, /// 0x17c
			 0x9ad3e98a, /// 0x180
			 0x4abd7c85, /// 0x184
			 0x2eb2c65b, /// 0x188
			 0xa9e321d7, /// 0x18c
			 0xf8668ea6, /// 0x190
			 0x24920e50, /// 0x194
			 0x894d6f6b, /// 0x198
			 0x7a7ebcc9, /// 0x19c
			 0xa9183331, /// 0x1a0
			 0x2938353c, /// 0x1a4
			 0xd7614caf, /// 0x1a8
			 0x6d1a43c3, /// 0x1ac
			 0xadb9ecc7, /// 0x1b0
			 0xd99478ba, /// 0x1b4
			 0x978c572b, /// 0x1b8
			 0x461a301a, /// 0x1bc
			 0x9ce06a72, /// 0x1c0
			 0x6e688b66, /// 0x1c4
			 0xcf0eec70, /// 0x1c8
			 0x05b9854a, /// 0x1cc
			 0xa2838ee6, /// 0x1d0
			 0xa5db64cb, /// 0x1d4
			 0x4892c161, /// 0x1d8
			 0x5e14ef16, /// 0x1dc
			 0x1b9e2b0c, /// 0x1e0
			 0x1ebf10ac, /// 0x1e4
			 0x201a8c7a, /// 0x1e8
			 0xc2d2db4d, /// 0x1ec
			 0x7de02fe5, /// 0x1f0
			 0x30abd8b4, /// 0x1f4
			 0x0cd51b68, /// 0x1f8
			 0xc11d5c5e, /// 0x1fc
			 0x4abdf7fb, /// 0x200
			 0x01c3dcfb, /// 0x204
			 0x99ece9fb, /// 0x208
			 0x95f42662, /// 0x20c
			 0x52bc3307, /// 0x210
			 0x6747db81, /// 0x214
			 0x94bca7e3, /// 0x218
			 0xe7b4b294, /// 0x21c
			 0x9f5554d8, /// 0x220
			 0x9bbd35f6, /// 0x224
			 0x69837f8f, /// 0x228
			 0x9967282b, /// 0x22c
			 0xaae435d3, /// 0x230
			 0x2ffc5d6b, /// 0x234
			 0x60de6002, /// 0x238
			 0x2548464b, /// 0x23c
			 0xc3501972, /// 0x240
			 0xef33ff22, /// 0x244
			 0x82b97978, /// 0x248
			 0x79c54f6f, /// 0x24c
			 0xf285824d, /// 0x250
			 0xe0618861, /// 0x254
			 0xdbd4a99e, /// 0x258
			 0xc55d3a8c, /// 0x25c
			 0xddf0340a, /// 0x260
			 0x63661c40, /// 0x264
			 0x24c044d3, /// 0x268
			 0x823a8934, /// 0x26c
			 0x1bd904a8, /// 0x270
			 0x170146ae, /// 0x274
			 0xf0f3d5e4, /// 0x278
			 0x2c6a4dc6, /// 0x27c
			 0x9ac661c4, /// 0x280
			 0xce3472fd, /// 0x284
			 0x051282a8, /// 0x288
			 0x1f311982, /// 0x28c
			 0x2a437dde, /// 0x290
			 0xd9fd2e8c, /// 0x294
			 0xead64798, /// 0x298
			 0x646733c5, /// 0x29c
			 0x2c956f9d, /// 0x2a0
			 0x12a840a0, /// 0x2a4
			 0xd09583fe, /// 0x2a8
			 0x6b951de1, /// 0x2ac
			 0xf2c0f104, /// 0x2b0
			 0x5a049895, /// 0x2b4
			 0xac780cee, /// 0x2b8
			 0xc1489a7a, /// 0x2bc
			 0x78acc503, /// 0x2c0
			 0x9142b73d, /// 0x2c4
			 0x75cb43b3, /// 0x2c8
			 0x19c44463, /// 0x2cc
			 0xe0ee2c0a, /// 0x2d0
			 0x79dfe5ea, /// 0x2d4
			 0x1daeed78, /// 0x2d8
			 0x4050d8ee, /// 0x2dc
			 0x9a99c458, /// 0x2e0
			 0xa70d4c0a, /// 0x2e4
			 0x0d001d5f, /// 0x2e8
			 0xddb2fd81, /// 0x2ec
			 0x03185f92, /// 0x2f0
			 0xe3f4e679, /// 0x2f4
			 0xc60ca69d, /// 0x2f8
			 0xd15652cd, /// 0x2fc
			 0x4ba51738, /// 0x300
			 0xabc833a5, /// 0x304
			 0xee6c132f, /// 0x308
			 0x77413a39, /// 0x30c
			 0x233e43ae, /// 0x310
			 0x741649d9, /// 0x314
			 0x1332be88, /// 0x318
			 0x18af3492, /// 0x31c
			 0x7509ba07, /// 0x320
			 0x0dd04666, /// 0x324
			 0x41c814e8, /// 0x328
			 0x4e18c931, /// 0x32c
			 0x765fbff0, /// 0x330
			 0x2edf8652, /// 0x334
			 0x2165cc35, /// 0x338
			 0x7b2c6a87, /// 0x33c
			 0xd0cd5ea4, /// 0x340
			 0x7c15693d, /// 0x344
			 0xe33a3df0, /// 0x348
			 0xb5ac25f5, /// 0x34c
			 0x7238419e, /// 0x350
			 0x6f929425, /// 0x354
			 0x1dead754, /// 0x358
			 0x593d4ac0, /// 0x35c
			 0xa8c9a640, /// 0x360
			 0x0023530a, /// 0x364
			 0x42c4662b, /// 0x368
			 0x031c2dbe, /// 0x36c
			 0x0a63972a, /// 0x370
			 0x7b66a641, /// 0x374
			 0x07a607da, /// 0x378
			 0x30372586, /// 0x37c
			 0xc17dc316, /// 0x380
			 0xa1baa23d, /// 0x384
			 0x4896ee7c, /// 0x388
			 0xf0576519, /// 0x38c
			 0x9d0590d7, /// 0x390
			 0x2f7cce05, /// 0x394
			 0xbf5e7bfe, /// 0x398
			 0x8e0b6e00, /// 0x39c
			 0x96c1cbd9, /// 0x3a0
			 0xfd49c88e, /// 0x3a4
			 0xa013e6be, /// 0x3a8
			 0x63d5cb7f, /// 0x3ac
			 0x9d1b24ff, /// 0x3b0
			 0x0d16136d, /// 0x3b4
			 0xbcb95fc2, /// 0x3b8
			 0xa6c579df, /// 0x3bc
			 0xed4f3521, /// 0x3c0
			 0xdecca67d, /// 0x3c4
			 0xb81f5345, /// 0x3c8
			 0x19599811, /// 0x3cc
			 0xa7875931, /// 0x3d0
			 0x91254285, /// 0x3d4
			 0xe3fc4359, /// 0x3d8
			 0xf66da3e8, /// 0x3dc
			 0xc079fc27, /// 0x3e0
			 0x458b9f71, /// 0x3e4
			 0x17f75384, /// 0x3e8
			 0x48a1ba3f, /// 0x3ec
			 0x9a864174, /// 0x3f0
			 0x0d294b2a, /// 0x3f4
			 0x2b5332d8, /// 0x3f8
			 0x8c27e0fa, /// 0x3fc
			 0x4472740a, /// 0x400
			 0xd6febdb8, /// 0x404
			 0x6474f5c0, /// 0x408
			 0xdb5d19bf, /// 0x40c
			 0xbb12f9e3, /// 0x410
			 0x23c66f81, /// 0x414
			 0x65bf78e6, /// 0x418
			 0xc256666d, /// 0x41c
			 0x54af5081, /// 0x420
			 0x58a5124b, /// 0x424
			 0xfef381b8, /// 0x428
			 0x0a2f3202, /// 0x42c
			 0x65f81cdf, /// 0x430
			 0x6803d888, /// 0x434
			 0x78dc7979, /// 0x438
			 0x81a10b58, /// 0x43c
			 0xa63cf44f, /// 0x440
			 0xbf1df610, /// 0x444
			 0x14442d2d, /// 0x448
			 0x93e946ea, /// 0x44c
			 0x92f66319, /// 0x450
			 0xcf0aa791, /// 0x454
			 0x5989fc73, /// 0x458
			 0x73735e29, /// 0x45c
			 0x2fed08f1, /// 0x460
			 0x7d3920e0, /// 0x464
			 0x7bb137ed, /// 0x468
			 0x8cbd3e79, /// 0x46c
			 0xe5b24576, /// 0x470
			 0x551bb980, /// 0x474
			 0xad87883d, /// 0x478
			 0x1b873fc3, /// 0x47c
			 0x56d9c055, /// 0x480
			 0xe953b22d, /// 0x484
			 0x9a92b305, /// 0x488
			 0xf97ee360, /// 0x48c
			 0xf305ad46, /// 0x490
			 0xf2536889, /// 0x494
			 0x373edee9, /// 0x498
			 0xd5a17398, /// 0x49c
			 0xf209acee, /// 0x4a0
			 0x484e2c95, /// 0x4a4
			 0x9cc8fa3d, /// 0x4a8
			 0x17cc3329, /// 0x4ac
			 0x6a213d68, /// 0x4b0
			 0x588b7d0b, /// 0x4b4
			 0x0a6d13f2, /// 0x4b8
			 0x850a880d, /// 0x4bc
			 0x4f8c5d08, /// 0x4c0
			 0x3657c1d6, /// 0x4c4
			 0x8e3ad4a9, /// 0x4c8
			 0x69173eeb, /// 0x4cc
			 0xf9680b57, /// 0x4d0
			 0xad809a2f, /// 0x4d4
			 0x15d24863, /// 0x4d8
			 0x049f1712, /// 0x4dc
			 0x17818a74, /// 0x4e0
			 0x3505380e, /// 0x4e4
			 0xfddd8cba, /// 0x4e8
			 0x5665f5df, /// 0x4ec
			 0x121661f4, /// 0x4f0
			 0x503f8e0d, /// 0x4f4
			 0xf0500577, /// 0x4f8
			 0xcfe20cb2, /// 0x4fc
			 0xa00e1cb4, /// 0x500
			 0xd5318638, /// 0x504
			 0x4fb2d5c8, /// 0x508
			 0xc9e148f5, /// 0x50c
			 0x1a73c96e, /// 0x510
			 0xb7b59f3e, /// 0x514
			 0x2f207ab8, /// 0x518
			 0xd0e468cd, /// 0x51c
			 0xc3cefbbd, /// 0x520
			 0x9b12adf5, /// 0x524
			 0x4718dfd9, /// 0x528
			 0xd8325844, /// 0x52c
			 0x0d94e21b, /// 0x530
			 0xb7ee26a3, /// 0x534
			 0x44030bfb, /// 0x538
			 0x5913c88d, /// 0x53c
			 0xad17bc16, /// 0x540
			 0x0be0041e, /// 0x544
			 0x989d48b4, /// 0x548
			 0x1426cb85, /// 0x54c
			 0x6ca30277, /// 0x550
			 0x44765cbd, /// 0x554
			 0x09810b46, /// 0x558
			 0xbec4a919, /// 0x55c
			 0x64ed713e, /// 0x560
			 0x89a122ad, /// 0x564
			 0xbdf04b52, /// 0x568
			 0xb37f567e, /// 0x56c
			 0x19f13953, /// 0x570
			 0x850bc3e4, /// 0x574
			 0x48b54524, /// 0x578
			 0x6b610ca7, /// 0x57c
			 0x91db10a1, /// 0x580
			 0x055be5c5, /// 0x584
			 0x03fd331e, /// 0x588
			 0x66e95740, /// 0x58c
			 0x5b18c345, /// 0x590
			 0x4ef220e2, /// 0x594
			 0x191db9ac, /// 0x598
			 0xfbdfd14d, /// 0x59c
			 0x17827d18, /// 0x5a0
			 0xf533db7b, /// 0x5a4
			 0x392a474c, /// 0x5a8
			 0x571ea225, /// 0x5ac
			 0xe9c7481c, /// 0x5b0
			 0xa1fe122f, /// 0x5b4
			 0x4e88b2fa, /// 0x5b8
			 0x03cf6c24, /// 0x5bc
			 0xca962dda, /// 0x5c0
			 0x6230ec11, /// 0x5c4
			 0xa63450d0, /// 0x5c8
			 0x44e8efc0, /// 0x5cc
			 0x627e125e, /// 0x5d0
			 0xf5198ea7, /// 0x5d4
			 0x230e5bb2, /// 0x5d8
			 0x1cfd97c7, /// 0x5dc
			 0x034e6ef7, /// 0x5e0
			 0xd68d9b0e, /// 0x5e4
			 0xeb0e6d22, /// 0x5e8
			 0x0cd8df1d, /// 0x5ec
			 0xfe35293a, /// 0x5f0
			 0x076d3691, /// 0x5f4
			 0x41502e0e, /// 0x5f8
			 0x7431e7c3, /// 0x5fc
			 0xe1328750, /// 0x600
			 0x2fca85cb, /// 0x604
			 0xf610ab3c, /// 0x608
			 0xcbd798f6, /// 0x60c
			 0x741e6e68, /// 0x610
			 0xc5125bec, /// 0x614
			 0xcd41a5bf, /// 0x618
			 0x383f42fb, /// 0x61c
			 0x46afe2e1, /// 0x620
			 0xcdc68871, /// 0x624
			 0xe077e7ed, /// 0x628
			 0x40e12245, /// 0x62c
			 0x652f65c0, /// 0x630
			 0x14717b47, /// 0x634
			 0x85bb3439, /// 0x638
			 0x6ed27524, /// 0x63c
			 0x57bd957a, /// 0x640
			 0xb6b18666, /// 0x644
			 0x952f942d, /// 0x648
			 0x2f5c72f6, /// 0x64c
			 0xf49a3c0a, /// 0x650
			 0x9184fcb0, /// 0x654
			 0xeb7c5b7d, /// 0x658
			 0x9e26ca77, /// 0x65c
			 0xf2ff6ddc, /// 0x660
			 0x9450f8b7, /// 0x664
			 0x7a09cc24, /// 0x668
			 0x0edc14d3, /// 0x66c
			 0x3dd707e9, /// 0x670
			 0xeb82f752, /// 0x674
			 0x5f82d6a6, /// 0x678
			 0xd89c55c1, /// 0x67c
			 0xe48e154f, /// 0x680
			 0xc84f53af, /// 0x684
			 0x1dc90cd7, /// 0x688
			 0xa3ac6bac, /// 0x68c
			 0xb81ab458, /// 0x690
			 0x4fe96cba, /// 0x694
			 0x8a016035, /// 0x698
			 0xc25d295f, /// 0x69c
			 0x48b241b5, /// 0x6a0
			 0xf89f5486, /// 0x6a4
			 0x7bcc7f70, /// 0x6a8
			 0x0e6133ef, /// 0x6ac
			 0xdd2a4e80, /// 0x6b0
			 0xe6da1872, /// 0x6b4
			 0x52994056, /// 0x6b8
			 0xdefe9447, /// 0x6bc
			 0x26844979, /// 0x6c0
			 0xb06ccc84, /// 0x6c4
			 0x353e4051, /// 0x6c8
			 0xdf61d95a, /// 0x6cc
			 0x1a434615, /// 0x6d0
			 0xe4fc18e8, /// 0x6d4
			 0x94771873, /// 0x6d8
			 0xeb2eff19, /// 0x6dc
			 0xd7611f65, /// 0x6e0
			 0x8d1e6d92, /// 0x6e4
			 0x8e77d394, /// 0x6e8
			 0x3fb2e7db, /// 0x6ec
			 0x96993528, /// 0x6f0
			 0x1aa7ccaf, /// 0x6f4
			 0x022f70b4, /// 0x6f8
			 0x32054b7b, /// 0x6fc
			 0xe97e4c61, /// 0x700
			 0x0dea2b3c, /// 0x704
			 0x074cb634, /// 0x708
			 0xb239b262, /// 0x70c
			 0x75f72f27, /// 0x710
			 0x40e6aac7, /// 0x714
			 0x6b3ae933, /// 0x718
			 0x32416757, /// 0x71c
			 0x8d0e9ff9, /// 0x720
			 0x5a75a784, /// 0x724
			 0x373326ca, /// 0x728
			 0xbaf48a05, /// 0x72c
			 0xbaf36fac, /// 0x730
			 0xb213023f, /// 0x734
			 0xfd247091, /// 0x738
			 0x22e185e6, /// 0x73c
			 0x8cb40432, /// 0x740
			 0x6e0b1562, /// 0x744
			 0x71a5d39e, /// 0x748
			 0x4408323d, /// 0x74c
			 0xeaf8879f, /// 0x750
			 0xfff92f71, /// 0x754
			 0xc6dca9bd, /// 0x758
			 0x6c963e9c, /// 0x75c
			 0xf6951d18, /// 0x760
			 0x6364ce0b, /// 0x764
			 0x68789c8d, /// 0x768
			 0xdd9b0357, /// 0x76c
			 0x137d8cc8, /// 0x770
			 0x502e615e, /// 0x774
			 0x3fa9cbdd, /// 0x778
			 0x362f423e, /// 0x77c
			 0x1296a932, /// 0x780
			 0xb87f5ddc, /// 0x784
			 0x5deef18e, /// 0x788
			 0x789e216f, /// 0x78c
			 0xfdbb45f7, /// 0x790
			 0x9bae10ce, /// 0x794
			 0x057d6ba5, /// 0x798
			 0x4148c433, /// 0x79c
			 0x2968807b, /// 0x7a0
			 0x54bdd7f6, /// 0x7a4
			 0x05b7aec3, /// 0x7a8
			 0x47545179, /// 0x7ac
			 0xc2e8a09c, /// 0x7b0
			 0x715137ae, /// 0x7b4
			 0x7f20185f, /// 0x7b8
			 0xcc7bec04, /// 0x7bc
			 0xe40d031a, /// 0x7c0
			 0x28730703, /// 0x7c4
			 0x01196e47, /// 0x7c8
			 0xd64b2025, /// 0x7cc
			 0x515f003b, /// 0x7d0
			 0x1ae1b6d4, /// 0x7d4
			 0x97fec71a, /// 0x7d8
			 0xde23c913, /// 0x7dc
			 0x4e394aed, /// 0x7e0
			 0x5574a30c, /// 0x7e4
			 0x2b5e1c2e, /// 0x7e8
			 0x2c850aca, /// 0x7ec
			 0xb56fb2e0, /// 0x7f0
			 0x0e17a093, /// 0x7f4
			 0xe3c44804, /// 0x7f8
			 0x26d9431a, /// 0x7fc
			 0x2e8c4c5e, /// 0x800
			 0x3d5322bc, /// 0x804
			 0x38b5f51d, /// 0x808
			 0x661ac546, /// 0x80c
			 0xcd4fcbc6, /// 0x810
			 0xf5081f33, /// 0x814
			 0xebd996b0, /// 0x818
			 0x33924980, /// 0x81c
			 0x147183b2, /// 0x820
			 0x10da3842, /// 0x824
			 0xf2784de2, /// 0x828
			 0xb87ab3c9, /// 0x82c
			 0xd95b894e, /// 0x830
			 0x7dfbeb41, /// 0x834
			 0xbe51103f, /// 0x838
			 0xcb32e153, /// 0x83c
			 0x0691f04e, /// 0x840
			 0x0a1f26a0, /// 0x844
			 0x5d49fd82, /// 0x848
			 0xf44e9563, /// 0x84c
			 0x3ef99148, /// 0x850
			 0xd6cb3cca, /// 0x854
			 0x5aa7bd9b, /// 0x858
			 0xa97f5d0a, /// 0x85c
			 0x30492a87, /// 0x860
			 0x762e9334, /// 0x864
			 0x03e59548, /// 0x868
			 0x53824fbe, /// 0x86c
			 0x95dcee06, /// 0x870
			 0x359cc1e7, /// 0x874
			 0x0202b3de, /// 0x878
			 0x7e7143c0, /// 0x87c
			 0x5aeb1b8f, /// 0x880
			 0x31c51b81, /// 0x884
			 0x804c5cc0, /// 0x888
			 0x9b3696cb, /// 0x88c
			 0xcbeaadf4, /// 0x890
			 0xaeee3d5f, /// 0x894
			 0xd44d2e51, /// 0x898
			 0xff176d70, /// 0x89c
			 0x23a8424e, /// 0x8a0
			 0x481f357c, /// 0x8a4
			 0x49f0adb5, /// 0x8a8
			 0x8f5ddd9d, /// 0x8ac
			 0x552126b2, /// 0x8b0
			 0x37cb376b, /// 0x8b4
			 0x7aa31b46, /// 0x8b8
			 0x49e7ea98, /// 0x8bc
			 0x9e99b6ae, /// 0x8c0
			 0xd4d5713c, /// 0x8c4
			 0x113fc4bc, /// 0x8c8
			 0x08f64108, /// 0x8cc
			 0x66d0af82, /// 0x8d0
			 0x5a327fdc, /// 0x8d4
			 0xedf23f2d, /// 0x8d8
			 0x46fee4f9, /// 0x8dc
			 0x4a6d58e8, /// 0x8e0
			 0x0c30e7f8, /// 0x8e4
			 0x37fdf121, /// 0x8e8
			 0x58aaf8d0, /// 0x8ec
			 0x478a1963, /// 0x8f0
			 0xd91386aa, /// 0x8f4
			 0x3635400d, /// 0x8f8
			 0xbe6dc50b, /// 0x8fc
			 0x8fd6799a, /// 0x900
			 0x24fdd944, /// 0x904
			 0xe3f0fea6, /// 0x908
			 0x5d7b352c, /// 0x90c
			 0x1b901673, /// 0x910
			 0x9d716622, /// 0x914
			 0x6b03db39, /// 0x918
			 0x98b6231f, /// 0x91c
			 0x9d9fb8aa, /// 0x920
			 0xa17cf0c1, /// 0x924
			 0x14e01059, /// 0x928
			 0x3f3a4624, /// 0x92c
			 0xb6f3b787, /// 0x930
			 0x306b40a3, /// 0x934
			 0xb93e10be, /// 0x938
			 0x4629e12c, /// 0x93c
			 0x3e0ddf24, /// 0x940
			 0xff90ffac, /// 0x944
			 0x7bb066d9, /// 0x948
			 0xf470c929, /// 0x94c
			 0x081d1665, /// 0x950
			 0xb0958982, /// 0x954
			 0x6d23346f, /// 0x958
			 0xceb206bd, /// 0x95c
			 0x2a6468f8, /// 0x960
			 0x89f6ce38, /// 0x964
			 0x2aecaf7a, /// 0x968
			 0x801ddd1d, /// 0x96c
			 0x276c29a7, /// 0x970
			 0x13c4769a, /// 0x974
			 0xff060d63, /// 0x978
			 0x04914ec7, /// 0x97c
			 0xb5553966, /// 0x980
			 0xcb2f5887, /// 0x984
			 0xbb5fd6e0, /// 0x988
			 0xd5adc9eb, /// 0x98c
			 0xd3f7b527, /// 0x990
			 0xe946736f, /// 0x994
			 0xa2249fac, /// 0x998
			 0x75f97455, /// 0x99c
			 0x9fba7672, /// 0x9a0
			 0xff2eaf7c, /// 0x9a4
			 0x000abce6, /// 0x9a8
			 0x6f150108, /// 0x9ac
			 0xfea6f334, /// 0x9b0
			 0x67527666, /// 0x9b4
			 0x8eefabf6, /// 0x9b8
			 0xf6875bda, /// 0x9bc
			 0x3fc2da75, /// 0x9c0
			 0x00f33bd4, /// 0x9c4
			 0x259c97a6, /// 0x9c8
			 0xa213956c, /// 0x9cc
			 0x42a25348, /// 0x9d0
			 0xb200988f, /// 0x9d4
			 0x592675a3, /// 0x9d8
			 0x77cd4bc8, /// 0x9dc
			 0xa17eec30, /// 0x9e0
			 0x9d70bb0e, /// 0x9e4
			 0x22d22921, /// 0x9e8
			 0x07da428e, /// 0x9ec
			 0xb7fbe699, /// 0x9f0
			 0x1c0f852e, /// 0x9f4
			 0xdbc2d3a9, /// 0x9f8
			 0xbe6a2296, /// 0x9fc
			 0xd6b9de82, /// 0xa00
			 0x6176e708, /// 0xa04
			 0x778dc51a, /// 0xa08
			 0x9f3aded0, /// 0xa0c
			 0xc2fe3bb5, /// 0xa10
			 0x897a54eb, /// 0xa14
			 0x392b5610, /// 0xa18
			 0xe2eecf2f, /// 0xa1c
			 0xa0f049b6, /// 0xa20
			 0x3a219059, /// 0xa24
			 0x809c1bda, /// 0xa28
			 0x4f3c143b, /// 0xa2c
			 0x211da202, /// 0xa30
			 0xcab56fdd, /// 0xa34
			 0x4d9a7712, /// 0xa38
			 0xc22240ec, /// 0xa3c
			 0xc2ff55f6, /// 0xa40
			 0x0aa3a169, /// 0xa44
			 0x39812363, /// 0xa48
			 0x32da39b0, /// 0xa4c
			 0x3243926e, /// 0xa50
			 0x33c4aad8, /// 0xa54
			 0x045d3332, /// 0xa58
			 0xe50aa4e7, /// 0xa5c
			 0xe8e98a96, /// 0xa60
			 0xb96c7c84, /// 0xa64
			 0xd69c9064, /// 0xa68
			 0x2200497d, /// 0xa6c
			 0x845fc14f, /// 0xa70
			 0xa3ac729f, /// 0xa74
			 0xc519ac10, /// 0xa78
			 0x9566261e, /// 0xa7c
			 0x5e4c62e7, /// 0xa80
			 0x7c6c3ff2, /// 0xa84
			 0xf8e532a9, /// 0xa88
			 0x63a4697b, /// 0xa8c
			 0x133d4188, /// 0xa90
			 0x74577cbe, /// 0xa94
			 0xa1c8ed06, /// 0xa98
			 0x33516fbf, /// 0xa9c
			 0xd7b0d15c, /// 0xaa0
			 0x47af9b57, /// 0xaa4
			 0x61412209, /// 0xaa8
			 0xea9f0a60, /// 0xaac
			 0x01a193ca, /// 0xab0
			 0x4eb94a52, /// 0xab4
			 0xee304efb, /// 0xab8
			 0xa041b62c, /// 0xabc
			 0x3a9e491e, /// 0xac0
			 0x86bd72c6, /// 0xac4
			 0x7501ed7d, /// 0xac8
			 0xab0d3178, /// 0xacc
			 0x1e3fa451, /// 0xad0
			 0x00e1cdb6, /// 0xad4
			 0xd9173af8, /// 0xad8
			 0x816b4bc4, /// 0xadc
			 0xfd822c2b, /// 0xae0
			 0xd9b23bd9, /// 0xae4
			 0x87dec69f, /// 0xae8
			 0x982cdace, /// 0xaec
			 0xf8ddd7f3, /// 0xaf0
			 0x37cb25cb, /// 0xaf4
			 0x7b22c171, /// 0xaf8
			 0xf4614614, /// 0xafc
			 0x5fbb0507, /// 0xb00
			 0xa9730ced, /// 0xb04
			 0x7bfd7bee, /// 0xb08
			 0x0c4ca6e6, /// 0xb0c
			 0xd758d9a2, /// 0xb10
			 0xf2cdeadb, /// 0xb14
			 0x72917bcf, /// 0xb18
			 0xa61f5e47, /// 0xb1c
			 0x4e5a4f46, /// 0xb20
			 0xf46600a7, /// 0xb24
			 0x9adf7d2f, /// 0xb28
			 0xf1afc6a7, /// 0xb2c
			 0xe9a04a9f, /// 0xb30
			 0x57c22ed2, /// 0xb34
			 0xc76745f3, /// 0xb38
			 0x9e5456dd, /// 0xb3c
			 0xcb1e7053, /// 0xb40
			 0x1a8cd136, /// 0xb44
			 0xd5a90bf3, /// 0xb48
			 0x37ca6dcc, /// 0xb4c
			 0x3fa18128, /// 0xb50
			 0x56e25617, /// 0xb54
			 0xf80fbc75, /// 0xb58
			 0x803588ed, /// 0xb5c
			 0x08385781, /// 0xb60
			 0x64132059, /// 0xb64
			 0x32f162df, /// 0xb68
			 0x93625e9d, /// 0xb6c
			 0xeac55ef4, /// 0xb70
			 0x8590ece3, /// 0xb74
			 0x65814a5a, /// 0xb78
			 0x62ab2954, /// 0xb7c
			 0x1d6eaed8, /// 0xb80
			 0x269b0fd3, /// 0xb84
			 0x75591e3b, /// 0xb88
			 0x0df0ec1d, /// 0xb8c
			 0x3cfd1fa2, /// 0xb90
			 0x8c38071f, /// 0xb94
			 0x8b429fb3, /// 0xb98
			 0xb46f9a1b, /// 0xb9c
			 0x2d4ef14c, /// 0xba0
			 0xa59131ec, /// 0xba4
			 0x2a849a54, /// 0xba8
			 0x596e2191, /// 0xbac
			 0x0edeee25, /// 0xbb0
			 0xdab85bdd, /// 0xbb4
			 0x0de55150, /// 0xbb8
			 0xf6eae97c, /// 0xbbc
			 0xd557db6b, /// 0xbc0
			 0x94b8586e, /// 0xbc4
			 0xeb8b7614, /// 0xbc8
			 0x9675a519, /// 0xbcc
			 0x57d1474d, /// 0xbd0
			 0x47e60154, /// 0xbd4
			 0x6fe6ca40, /// 0xbd8
			 0xc7a2cb89, /// 0xbdc
			 0xffd71915, /// 0xbe0
			 0x0146280e, /// 0xbe4
			 0x1920512b, /// 0xbe8
			 0x16e44a3a, /// 0xbec
			 0xd42f49ad, /// 0xbf0
			 0xa9949472, /// 0xbf4
			 0x67527594, /// 0xbf8
			 0x8ff51fd4, /// 0xbfc
			 0x4b124388, /// 0xc00
			 0x95b088a0, /// 0xc04
			 0x03a6727f, /// 0xc08
			 0x6dda7528, /// 0xc0c
			 0xfc8a59f4, /// 0xc10
			 0x0bc29f2e, /// 0xc14
			 0xf9589a4d, /// 0xc18
			 0x63248800, /// 0xc1c
			 0xaffe6c60, /// 0xc20
			 0xbe2f51f2, /// 0xc24
			 0x5ce52d0b, /// 0xc28
			 0x6b7b14c6, /// 0xc2c
			 0xe8436f1a, /// 0xc30
			 0xea1ddd09, /// 0xc34
			 0x61acc859, /// 0xc38
			 0x7841dc11, /// 0xc3c
			 0x0313f715, /// 0xc40
			 0xf3c02f1f, /// 0xc44
			 0x33a4d784, /// 0xc48
			 0x413344f8, /// 0xc4c
			 0xff907e6d, /// 0xc50
			 0xad5237ca, /// 0xc54
			 0xdaeb37a8, /// 0xc58
			 0x031a54d0, /// 0xc5c
			 0xa47bcd1d, /// 0xc60
			 0xb9d6afa6, /// 0xc64
			 0x5d298079, /// 0xc68
			 0xefcebe51, /// 0xc6c
			 0x744eae36, /// 0xc70
			 0x8ef4c26e, /// 0xc74
			 0xd04b6c12, /// 0xc78
			 0x5a780947, /// 0xc7c
			 0x8e32e816, /// 0xc80
			 0x0bb2b831, /// 0xc84
			 0x7eb58ae1, /// 0xc88
			 0x00d450c0, /// 0xc8c
			 0xf02d0adf, /// 0xc90
			 0x37779a72, /// 0xc94
			 0xc143c324, /// 0xc98
			 0x53db4dec, /// 0xc9c
			 0x9dc51401, /// 0xca0
			 0x767766d5, /// 0xca4
			 0x39276928, /// 0xca8
			 0xa78c8049, /// 0xcac
			 0xfad278a5, /// 0xcb0
			 0xf5c7bff2, /// 0xcb4
			 0x5ce24978, /// 0xcb8
			 0x22f609f4, /// 0xcbc
			 0x82d9a274, /// 0xcc0
			 0x67842ad9, /// 0xcc4
			 0xe904fdc5, /// 0xcc8
			 0x331f1076, /// 0xccc
			 0x5079681f, /// 0xcd0
			 0xd315f430, /// 0xcd4
			 0x6e8ca772, /// 0xcd8
			 0x103bafbb, /// 0xcdc
			 0xb69a4e58, /// 0xce0
			 0x9b50c1ae, /// 0xce4
			 0x96157679, /// 0xce8
			 0x0f326ae9, /// 0xcec
			 0xb15fdeb6, /// 0xcf0
			 0x906d95da, /// 0xcf4
			 0x10da557b, /// 0xcf8
			 0x458e19f1, /// 0xcfc
			 0x467df0f4, /// 0xd00
			 0xbd0a1ede, /// 0xd04
			 0x7384f2de, /// 0xd08
			 0xe70467cb, /// 0xd0c
			 0x8629e490, /// 0xd10
			 0x6dcb7308, /// 0xd14
			 0x58c6477b, /// 0xd18
			 0x3cbdb8cd, /// 0xd1c
			 0x178fead8, /// 0xd20
			 0x68129764, /// 0xd24
			 0x04efd3f3, /// 0xd28
			 0x1334801e, /// 0xd2c
			 0xb11e0b88, /// 0xd30
			 0x72dba143, /// 0xd34
			 0x815989e9, /// 0xd38
			 0x2185d180, /// 0xd3c
			 0xc542489c, /// 0xd40
			 0x03dcdff7, /// 0xd44
			 0xa7ca596e, /// 0xd48
			 0x428e3e3f, /// 0xd4c
			 0x75b09482, /// 0xd50
			 0xbd581865, /// 0xd54
			 0x0a67ecfa, /// 0xd58
			 0xe035ed6a, /// 0xd5c
			 0x4a328eec, /// 0xd60
			 0xfd1f5eff, /// 0xd64
			 0xe130653b, /// 0xd68
			 0x3c51c39b, /// 0xd6c
			 0x14aa0e40, /// 0xd70
			 0x05528ff5, /// 0xd74
			 0x06bd2063, /// 0xd78
			 0x3df2a609, /// 0xd7c
			 0xc23c42c8, /// 0xd80
			 0x08392cf0, /// 0xd84
			 0xeb9586af, /// 0xd88
			 0xc288921b, /// 0xd8c
			 0x84162e10, /// 0xd90
			 0xca7f2228, /// 0xd94
			 0xf6de4a92, /// 0xd98
			 0x22a9bc87, /// 0xd9c
			 0x6d85fb4c, /// 0xda0
			 0x0a975976, /// 0xda4
			 0xc8400683, /// 0xda8
			 0x3a8a3568, /// 0xdac
			 0x3352c5b6, /// 0xdb0
			 0xaddef189, /// 0xdb4
			 0x6e099760, /// 0xdb8
			 0xeb28d415, /// 0xdbc
			 0x3b45093c, /// 0xdc0
			 0x66955ddb, /// 0xdc4
			 0x38057e44, /// 0xdc8
			 0x8235334d, /// 0xdcc
			 0xb96bda94, /// 0xdd0
			 0xf7c74151, /// 0xdd4
			 0x5cac9979, /// 0xdd8
			 0xff11d90c, /// 0xddc
			 0xea3d18be, /// 0xde0
			 0xf0f84277, /// 0xde4
			 0x37fb858a, /// 0xde8
			 0x1f922608, /// 0xdec
			 0xa5d26d7d, /// 0xdf0
			 0x04084411, /// 0xdf4
			 0xab2acd53, /// 0xdf8
			 0xf36310cf, /// 0xdfc
			 0xf0985b44, /// 0xe00
			 0xe8849ac7, /// 0xe04
			 0x0b660717, /// 0xe08
			 0x7731a28a, /// 0xe0c
			 0xe11494c0, /// 0xe10
			 0xc9b6bbc4, /// 0xe14
			 0x4441c6b2, /// 0xe18
			 0xee7a47c1, /// 0xe1c
			 0xea8955bf, /// 0xe20
			 0x8a932136, /// 0xe24
			 0xe62823ad, /// 0xe28
			 0x0351e9a9, /// 0xe2c
			 0x3b798b29, /// 0xe30
			 0xc2b63187, /// 0xe34
			 0xffcdcd9e, /// 0xe38
			 0x751c8cdb, /// 0xe3c
			 0xe9e98d0f, /// 0xe40
			 0xd15b4471, /// 0xe44
			 0xbcbf754e, /// 0xe48
			 0xf93ca82a, /// 0xe4c
			 0x1b63ecdd, /// 0xe50
			 0x52be0f27, /// 0xe54
			 0xa27b07d7, /// 0xe58
			 0xa6f86095, /// 0xe5c
			 0x195fb540, /// 0xe60
			 0xde472d64, /// 0xe64
			 0xae30a8d1, /// 0xe68
			 0xfb68fbea, /// 0xe6c
			 0xde86a95b, /// 0xe70
			 0x6627545b, /// 0xe74
			 0x66547edf, /// 0xe78
			 0xf267ae2b, /// 0xe7c
			 0x032d37e6, /// 0xe80
			 0xaa8b6a29, /// 0xe84
			 0xcaceab9b, /// 0xe88
			 0x1c84e480, /// 0xe8c
			 0x43663e89, /// 0xe90
			 0xf1ab04a6, /// 0xe94
			 0x1c1b7a78, /// 0xe98
			 0x15b3b98a, /// 0xe9c
			 0x6d0c93c1, /// 0xea0
			 0x9c015b2b, /// 0xea4
			 0xccbac34b, /// 0xea8
			 0xbe15528d, /// 0xeac
			 0x30118a94, /// 0xeb0
			 0x2ad7067e, /// 0xeb4
			 0x8f822fbc, /// 0xeb8
			 0xa25e998b, /// 0xebc
			 0x7dbabf7c, /// 0xec0
			 0xc211e799, /// 0xec4
			 0xad2dbf71, /// 0xec8
			 0x21cd0da7, /// 0xecc
			 0x476a46bf, /// 0xed0
			 0xabf9ce65, /// 0xed4
			 0x8c50192e, /// 0xed8
			 0xc3548c94, /// 0xedc
			 0x82365146, /// 0xee0
			 0x1e6e345f, /// 0xee4
			 0xf71c9ceb, /// 0xee8
			 0xc15df6f7, /// 0xeec
			 0x043d2d42, /// 0xef0
			 0x8d4f9ad7, /// 0xef4
			 0x4eec5c9c, /// 0xef8
			 0xd3799594, /// 0xefc
			 0x66f9875a, /// 0xf00
			 0xc355bfb2, /// 0xf04
			 0x6e79a760, /// 0xf08
			 0x953127c9, /// 0xf0c
			 0xb3cfb0b1, /// 0xf10
			 0x54b63376, /// 0xf14
			 0xbb600264, /// 0xf18
			 0x78f24ac0, /// 0xf1c
			 0xb174696e, /// 0xf20
			 0xac88f9be, /// 0xf24
			 0x3c6ea29c, /// 0xf28
			 0xbbaa7504, /// 0xf2c
			 0x1a41aac0, /// 0xf30
			 0x25414c39, /// 0xf34
			 0x451309ce, /// 0xf38
			 0xd2e34356, /// 0xf3c
			 0x3536a917, /// 0xf40
			 0x1880cf6f, /// 0xf44
			 0x7b34575a, /// 0xf48
			 0x68ff2a05, /// 0xf4c
			 0x94d8a8aa, /// 0xf50
			 0xb466faed, /// 0xf54
			 0xe74ad289, /// 0xf58
			 0x099a8a8f, /// 0xf5c
			 0x3ba9c55d, /// 0xf60
			 0xc30b6c9a, /// 0xf64
			 0x4cd9fc37, /// 0xf68
			 0x763fc13c, /// 0xf6c
			 0xa3c621ec, /// 0xf70
			 0x94ae465e, /// 0xf74
			 0x901c5c86, /// 0xf78
			 0xa44029df, /// 0xf7c
			 0x83a7ab69, /// 0xf80
			 0x6c8ad064, /// 0xf84
			 0x1530ac7b, /// 0xf88
			 0xbcd4ad64, /// 0xf8c
			 0xe3e9b7dc, /// 0xf90
			 0xe0479e65, /// 0xf94
			 0x404a80f8, /// 0xf98
			 0x9249740e, /// 0xf9c
			 0x81e7d178, /// 0xfa0
			 0xc711e145, /// 0xfa4
			 0xbdeebc40, /// 0xfa8
			 0xf837a2cd, /// 0xfac
			 0xd70c047c, /// 0xfb0
			 0xde282623, /// 0xfb4
			 0x597581d0, /// 0xfb8
			 0x2b2bfbdb, /// 0xfbc
			 0x97ddcd16, /// 0xfc0
			 0x6163c91b, /// 0xfc4
			 0x62330678, /// 0xfc8
			 0x68398b3a, /// 0xfcc
			 0xdd58cca5, /// 0xfd0
			 0x94c93793, /// 0xfd4
			 0xb3a35717, /// 0xfd8
			 0xe4059e0a, /// 0xfdc
			 0x18176857, /// 0xfe0
			 0x914d2cc2, /// 0xfe4
			 0x68d1beba, /// 0xfe8
			 0x4700b58d, /// 0xfec
			 0x695f4f6a, /// 0xff0
			 0xc7a18592, /// 0xff4
			 0xa70692d5, /// 0xff8
			 0x0a2669c3 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x550b8f3d, /// 0x0
			 0x049cd7b2, /// 0x4
			 0x28516e4e, /// 0x8
			 0xa2598c92, /// 0xc
			 0x2d588c56, /// 0x10
			 0x4acbeeaf, /// 0x14
			 0xd5f6eec9, /// 0x18
			 0xc2dd800b, /// 0x1c
			 0x7ceb27f3, /// 0x20
			 0xf2021039, /// 0x24
			 0xf0b062e6, /// 0x28
			 0xcea9ed19, /// 0x2c
			 0x466c115b, /// 0x30
			 0x1da60b9c, /// 0x34
			 0x7bed3295, /// 0x38
			 0xde850e1d, /// 0x3c
			 0x3042d469, /// 0x40
			 0x3b17c448, /// 0x44
			 0x336d885b, /// 0x48
			 0x9a338a47, /// 0x4c
			 0xa7bbe02c, /// 0x50
			 0xe26c9ea7, /// 0x54
			 0x811039a1, /// 0x58
			 0xa4b3a469, /// 0x5c
			 0x351b407d, /// 0x60
			 0x9cd2e9b0, /// 0x64
			 0xca038e9d, /// 0x68
			 0xc2e0efb5, /// 0x6c
			 0xb80bd8d2, /// 0x70
			 0xd4202007, /// 0x74
			 0x2a7c24f0, /// 0x78
			 0x9398f3d4, /// 0x7c
			 0x67b3bc6c, /// 0x80
			 0x432193a2, /// 0x84
			 0xc7570be9, /// 0x88
			 0x69a1fb47, /// 0x8c
			 0xeed96330, /// 0x90
			 0x33307aad, /// 0x94
			 0x9e1717ca, /// 0x98
			 0x76ad45af, /// 0x9c
			 0xf2e54f9d, /// 0xa0
			 0xf4ee1ca6, /// 0xa4
			 0x3c0a6337, /// 0xa8
			 0xc13fce36, /// 0xac
			 0x8d8733b4, /// 0xb0
			 0x825791f5, /// 0xb4
			 0x141c9003, /// 0xb8
			 0x9b5c620c, /// 0xbc
			 0x62aa829c, /// 0xc0
			 0x708ca10d, /// 0xc4
			 0x865e73bb, /// 0xc8
			 0xa3f4e5f9, /// 0xcc
			 0xc19bd9e8, /// 0xd0
			 0x564a4cab, /// 0xd4
			 0x58f970c1, /// 0xd8
			 0xdc474c5d, /// 0xdc
			 0x95a3a30d, /// 0xe0
			 0xbd247106, /// 0xe4
			 0x9054cccc, /// 0xe8
			 0x12922461, /// 0xec
			 0x6949caf5, /// 0xf0
			 0xe4ae0189, /// 0xf4
			 0x5e7d886d, /// 0xf8
			 0xf674bdb8, /// 0xfc
			 0x0607c073, /// 0x100
			 0xb6540bbd, /// 0x104
			 0x41c1e384, /// 0x108
			 0xe68ed603, /// 0x10c
			 0x53fa6948, /// 0x110
			 0xdd6ee286, /// 0x114
			 0x12daf6d3, /// 0x118
			 0xb7835fb2, /// 0x11c
			 0x8e18bf19, /// 0x120
			 0xdfd7ec71, /// 0x124
			 0x54887ae6, /// 0x128
			 0xb9cb6355, /// 0x12c
			 0x906337f7, /// 0x130
			 0xca71b81f, /// 0x134
			 0x716a35c5, /// 0x138
			 0x905fcf22, /// 0x13c
			 0x128752db, /// 0x140
			 0x447f75c3, /// 0x144
			 0x591b94bb, /// 0x148
			 0x2c2dc7e8, /// 0x14c
			 0x0534b729, /// 0x150
			 0x1b9acff0, /// 0x154
			 0xed177a93, /// 0x158
			 0xeacf7e85, /// 0x15c
			 0xb9c4ee6b, /// 0x160
			 0xda35d827, /// 0x164
			 0x8ea86811, /// 0x168
			 0xe4946806, /// 0x16c
			 0xc1feb04e, /// 0x170
			 0x475ef576, /// 0x174
			 0xdef423d9, /// 0x178
			 0xc726af64, /// 0x17c
			 0xd4627ff6, /// 0x180
			 0x42ea8d24, /// 0x184
			 0x3a34f673, /// 0x188
			 0xd623565a, /// 0x18c
			 0x0605a868, /// 0x190
			 0x37c3a4ad, /// 0x194
			 0xfd83ea2f, /// 0x198
			 0x0c77ea97, /// 0x19c
			 0xa3bf5f34, /// 0x1a0
			 0x252451bf, /// 0x1a4
			 0x7b9ba652, /// 0x1a8
			 0x15ba4696, /// 0x1ac
			 0x97248b00, /// 0x1b0
			 0x823f6335, /// 0x1b4
			 0xabfe2d8b, /// 0x1b8
			 0x5eac7e48, /// 0x1bc
			 0xbe4bae85, /// 0x1c0
			 0xe699d577, /// 0x1c4
			 0xcc3d77d0, /// 0x1c8
			 0x05f20508, /// 0x1cc
			 0xd58164d5, /// 0x1d0
			 0x70558876, /// 0x1d4
			 0xf4abf80c, /// 0x1d8
			 0x7863cb91, /// 0x1dc
			 0x8dd38183, /// 0x1e0
			 0x8cfca2fe, /// 0x1e4
			 0xe6d9aef8, /// 0x1e8
			 0xf1aa9cb4, /// 0x1ec
			 0x8ea27c95, /// 0x1f0
			 0xd3ac39c8, /// 0x1f4
			 0xf60c2ef2, /// 0x1f8
			 0xcce041f7, /// 0x1fc
			 0x9d0a63f4, /// 0x200
			 0xe191821d, /// 0x204
			 0x1adf3de6, /// 0x208
			 0xa5aa32ed, /// 0x20c
			 0x78c8b0a0, /// 0x210
			 0xecfe6cf7, /// 0x214
			 0xf9ef377d, /// 0x218
			 0xf6f73ffd, /// 0x21c
			 0x550466fd, /// 0x220
			 0xd119b85a, /// 0x224
			 0xb5ba4709, /// 0x228
			 0x17ce4f2d, /// 0x22c
			 0x5330299a, /// 0x230
			 0x62209c9e, /// 0x234
			 0x39ab09ff, /// 0x238
			 0x63d06e1b, /// 0x23c
			 0x75aa211d, /// 0x240
			 0x4b35d434, /// 0x244
			 0x967f4d57, /// 0x248
			 0xece78758, /// 0x24c
			 0x9b80ac66, /// 0x250
			 0x3817f344, /// 0x254
			 0x2e4f1520, /// 0x258
			 0x02866bb3, /// 0x25c
			 0x48c0188d, /// 0x260
			 0x42ba58af, /// 0x264
			 0x1cd5a071, /// 0x268
			 0x7ae8d84f, /// 0x26c
			 0xc6c9af3e, /// 0x270
			 0x8f601b2f, /// 0x274
			 0xf4c66678, /// 0x278
			 0x9e57fa46, /// 0x27c
			 0x3f785e6c, /// 0x280
			 0xb923f20b, /// 0x284
			 0x4f280a6a, /// 0x288
			 0x3c480581, /// 0x28c
			 0x49ca9947, /// 0x290
			 0xace5e873, /// 0x294
			 0x8b8f5e78, /// 0x298
			 0x6a792e26, /// 0x29c
			 0x63f8c0c0, /// 0x2a0
			 0x899d50ee, /// 0x2a4
			 0xf2b832a3, /// 0x2a8
			 0x90947aed, /// 0x2ac
			 0xa2513431, /// 0x2b0
			 0x48bf0612, /// 0x2b4
			 0x636920d3, /// 0x2b8
			 0x8ac3df20, /// 0x2bc
			 0xf2d06169, /// 0x2c0
			 0xff371058, /// 0x2c4
			 0xe89c722f, /// 0x2c8
			 0x72418e30, /// 0x2cc
			 0x679e78c1, /// 0x2d0
			 0xf457f480, /// 0x2d4
			 0xa97493c5, /// 0x2d8
			 0x069ba598, /// 0x2dc
			 0x64b73c57, /// 0x2e0
			 0x8c78d60b, /// 0x2e4
			 0x32151d26, /// 0x2e8
			 0x17731bbe, /// 0x2ec
			 0x0450ab78, /// 0x2f0
			 0xedb07b43, /// 0x2f4
			 0x926474f3, /// 0x2f8
			 0x19acfdfd, /// 0x2fc
			 0xa3823cdf, /// 0x300
			 0x36507678, /// 0x304
			 0x5549e835, /// 0x308
			 0x9a48a3b7, /// 0x30c
			 0x537a9548, /// 0x310
			 0xb415b56c, /// 0x314
			 0x5a1d01fa, /// 0x318
			 0x5fbfc451, /// 0x31c
			 0x0bc06726, /// 0x320
			 0x773d5ce2, /// 0x324
			 0x158957c7, /// 0x328
			 0xcd84ae5f, /// 0x32c
			 0xc848f61b, /// 0x330
			 0x85cdb93f, /// 0x334
			 0x14076e3b, /// 0x338
			 0x4cb553f4, /// 0x33c
			 0x1f6f3ca2, /// 0x340
			 0xe9321c42, /// 0x344
			 0xd977fc92, /// 0x348
			 0x0799948d, /// 0x34c
			 0x4cf2229e, /// 0x350
			 0x5a062b42, /// 0x354
			 0xa222e2bc, /// 0x358
			 0x1fd3352a, /// 0x35c
			 0x5714fa3b, /// 0x360
			 0x52183624, /// 0x364
			 0xb439b51e, /// 0x368
			 0xf691dd10, /// 0x36c
			 0x13b9efee, /// 0x370
			 0x6418d4f1, /// 0x374
			 0x41979fb5, /// 0x378
			 0xfa1e466d, /// 0x37c
			 0x092a5ef0, /// 0x380
			 0x017c7f2c, /// 0x384
			 0x1551683b, /// 0x388
			 0x86b83c30, /// 0x38c
			 0xdb808964, /// 0x390
			 0xe7fd54c8, /// 0x394
			 0x3672bdf2, /// 0x398
			 0xf4a0e405, /// 0x39c
			 0xea23290e, /// 0x3a0
			 0x380fd82a, /// 0x3a4
			 0x561ebfac, /// 0x3a8
			 0x9338bcc4, /// 0x3ac
			 0xbf7fb4e7, /// 0x3b0
			 0x17af4b03, /// 0x3b4
			 0xe1f2ea4b, /// 0x3b8
			 0x2cbadde7, /// 0x3bc
			 0x4507242c, /// 0x3c0
			 0x6698eeff, /// 0x3c4
			 0xe5e58c07, /// 0x3c8
			 0x07eb4253, /// 0x3cc
			 0xe802dd91, /// 0x3d0
			 0xf3fa6659, /// 0x3d4
			 0x3dc0fa56, /// 0x3d8
			 0x15e73383, /// 0x3dc
			 0xc90d8fcf, /// 0x3e0
			 0x9d3b2543, /// 0x3e4
			 0x4b350e76, /// 0x3e8
			 0x5d1ef568, /// 0x3ec
			 0x0fe7b77c, /// 0x3f0
			 0xc4f61f51, /// 0x3f4
			 0x7ae55e29, /// 0x3f8
			 0xb9e422a0, /// 0x3fc
			 0x9b4a8c77, /// 0x400
			 0xe8cfa3bf, /// 0x404
			 0x48cdf53c, /// 0x408
			 0x6475a505, /// 0x40c
			 0x2a728b82, /// 0x410
			 0xf1cd12e2, /// 0x414
			 0x3f34b6e4, /// 0x418
			 0x10a19418, /// 0x41c
			 0xa222720e, /// 0x420
			 0x48ab6e9a, /// 0x424
			 0x543905b3, /// 0x428
			 0xa5376018, /// 0x42c
			 0xb4a88216, /// 0x430
			 0x0175fbef, /// 0x434
			 0x9711c691, /// 0x438
			 0xddc8acbc, /// 0x43c
			 0x5811772a, /// 0x440
			 0x24572225, /// 0x444
			 0x7abb584b, /// 0x448
			 0xa1f41103, /// 0x44c
			 0x48741049, /// 0x450
			 0x724c4535, /// 0x454
			 0xf74e54f5, /// 0x458
			 0x602ffba3, /// 0x45c
			 0xfd9122e5, /// 0x460
			 0xab3e69c9, /// 0x464
			 0x71f4c373, /// 0x468
			 0x74f6f0af, /// 0x46c
			 0x9e4c5a56, /// 0x470
			 0x502594fa, /// 0x474
			 0xdf4d4067, /// 0x478
			 0x36052ff7, /// 0x47c
			 0xb562d08c, /// 0x480
			 0x782216af, /// 0x484
			 0x93693a13, /// 0x488
			 0xdefae6e1, /// 0x48c
			 0x4ccc3ed7, /// 0x490
			 0xc219034a, /// 0x494
			 0xed99b15b, /// 0x498
			 0xade3a4a7, /// 0x49c
			 0x6abdf9f1, /// 0x4a0
			 0x86b83827, /// 0x4a4
			 0x422ad0cc, /// 0x4a8
			 0x6218736e, /// 0x4ac
			 0xd74c92d5, /// 0x4b0
			 0x4a7b5a0b, /// 0x4b4
			 0x7440cd90, /// 0x4b8
			 0x5d1f0834, /// 0x4bc
			 0x2224b483, /// 0x4c0
			 0xc6d8e315, /// 0x4c4
			 0x44a68f78, /// 0x4c8
			 0x7cd3b280, /// 0x4cc
			 0xad48d464, /// 0x4d0
			 0x6ab54109, /// 0x4d4
			 0x070dbf97, /// 0x4d8
			 0x49e61096, /// 0x4dc
			 0x490b1a76, /// 0x4e0
			 0x737892dc, /// 0x4e4
			 0x3edf3ff2, /// 0x4e8
			 0xe439cab7, /// 0x4ec
			 0x3c090ac5, /// 0x4f0
			 0x217813a6, /// 0x4f4
			 0x3dabaf0f, /// 0x4f8
			 0xb106032f, /// 0x4fc
			 0x278ae920, /// 0x500
			 0x4797b9e9, /// 0x504
			 0x0cad9788, /// 0x508
			 0xc247bdf2, /// 0x50c
			 0x8c36669f, /// 0x510
			 0xfe5cc5cc, /// 0x514
			 0xd1399f91, /// 0x518
			 0xfae7eba4, /// 0x51c
			 0xdadd83e5, /// 0x520
			 0xbff7e9f2, /// 0x524
			 0x3467f5ff, /// 0x528
			 0x763dbfe9, /// 0x52c
			 0x1fcc5b8e, /// 0x530
			 0x16ae90b6, /// 0x534
			 0xcb9f3ec8, /// 0x538
			 0x8939ce52, /// 0x53c
			 0x43ef83d9, /// 0x540
			 0x59aea01f, /// 0x544
			 0xa8c60336, /// 0x548
			 0x56bea9b5, /// 0x54c
			 0x1c9fa2c7, /// 0x550
			 0x38637109, /// 0x554
			 0x29330f38, /// 0x558
			 0x6dad70fc, /// 0x55c
			 0x3e67abab, /// 0x560
			 0x09b71504, /// 0x564
			 0x49e7b990, /// 0x568
			 0xbad9bdff, /// 0x56c
			 0xa8ce39da, /// 0x570
			 0x0984ca90, /// 0x574
			 0xef58f579, /// 0x578
			 0xbfb129ac, /// 0x57c
			 0x7441a060, /// 0x580
			 0x13befe59, /// 0x584
			 0x93dac66d, /// 0x588
			 0xb134a515, /// 0x58c
			 0x94198317, /// 0x590
			 0x25ac2843, /// 0x594
			 0xce70fc22, /// 0x598
			 0x61bc48ad, /// 0x59c
			 0x1510c844, /// 0x5a0
			 0x1ee407a4, /// 0x5a4
			 0x2d2f6fb9, /// 0x5a8
			 0x4f9880ac, /// 0x5ac
			 0x5f98ff1f, /// 0x5b0
			 0xa4ee0849, /// 0x5b4
			 0xcab8f3df, /// 0x5b8
			 0x9a6fa244, /// 0x5bc
			 0xe5169257, /// 0x5c0
			 0x20013fea, /// 0x5c4
			 0x580d2225, /// 0x5c8
			 0x81cec932, /// 0x5cc
			 0xf9aa9bdd, /// 0x5d0
			 0xb4e86832, /// 0x5d4
			 0x1b57fe87, /// 0x5d8
			 0x1a164fb1, /// 0x5dc
			 0x035cc992, /// 0x5e0
			 0x68ddad71, /// 0x5e4
			 0x63d5ac38, /// 0x5e8
			 0x69fd44e9, /// 0x5ec
			 0x20129feb, /// 0x5f0
			 0x5a82f336, /// 0x5f4
			 0x2f32326e, /// 0x5f8
			 0x209c1d3e, /// 0x5fc
			 0x41c7e2a9, /// 0x600
			 0x259308cd, /// 0x604
			 0x05d7f41a, /// 0x608
			 0xc51c6de0, /// 0x60c
			 0x1ef21633, /// 0x610
			 0x73697dd8, /// 0x614
			 0x881a5c1e, /// 0x618
			 0xfd551dd8, /// 0x61c
			 0x60981d3b, /// 0x620
			 0xcf3cfa5e, /// 0x624
			 0x2bf4b5be, /// 0x628
			 0x56e61d6d, /// 0x62c
			 0xae435e15, /// 0x630
			 0x909f8aa5, /// 0x634
			 0x53eae962, /// 0x638
			 0x30d41c74, /// 0x63c
			 0xb6b140e6, /// 0x640
			 0x94b9ab90, /// 0x644
			 0x225f13d3, /// 0x648
			 0xd919b921, /// 0x64c
			 0xc1a05de7, /// 0x650
			 0x869ee3f4, /// 0x654
			 0x5969cc06, /// 0x658
			 0x86c8bf95, /// 0x65c
			 0x1464fcd4, /// 0x660
			 0x612c027f, /// 0x664
			 0x0e514d02, /// 0x668
			 0xfd9d7f3b, /// 0x66c
			 0x17313b15, /// 0x670
			 0x8cf568dd, /// 0x674
			 0x51741b14, /// 0x678
			 0xe2ae173a, /// 0x67c
			 0x53c89c65, /// 0x680
			 0xa042dc2b, /// 0x684
			 0x473f6f91, /// 0x688
			 0x5683a269, /// 0x68c
			 0x4cded578, /// 0x690
			 0x67175f8a, /// 0x694
			 0x2ffc3dcc, /// 0x698
			 0x078a451c, /// 0x69c
			 0x1f333773, /// 0x6a0
			 0xa50bb7ca, /// 0x6a4
			 0x6e1eefbe, /// 0x6a8
			 0x395af984, /// 0x6ac
			 0x8eabbda2, /// 0x6b0
			 0x7f26edad, /// 0x6b4
			 0x4296ac94, /// 0x6b8
			 0x484fab30, /// 0x6bc
			 0xf0373eed, /// 0x6c0
			 0x70216d17, /// 0x6c4
			 0xaf183ff0, /// 0x6c8
			 0x5f873a60, /// 0x6cc
			 0x4e3102ad, /// 0x6d0
			 0x7d42b0c9, /// 0x6d4
			 0x34e67877, /// 0x6d8
			 0xe8edd0fe, /// 0x6dc
			 0x6bf6cf1b, /// 0x6e0
			 0x23617896, /// 0x6e4
			 0x7d1e44b8, /// 0x6e8
			 0xb8868d8c, /// 0x6ec
			 0x31643465, /// 0x6f0
			 0x72cca81c, /// 0x6f4
			 0x96315cc3, /// 0x6f8
			 0x73f9edd6, /// 0x6fc
			 0xa5de8153, /// 0x700
			 0xd1bf7699, /// 0x704
			 0x3ae304e8, /// 0x708
			 0x40d34330, /// 0x70c
			 0xb962365d, /// 0x710
			 0x5088b812, /// 0x714
			 0xb98b2fff, /// 0x718
			 0x8ca933df, /// 0x71c
			 0xef0d31b7, /// 0x720
			 0x84591743, /// 0x724
			 0x920b3346, /// 0x728
			 0x56fc8cef, /// 0x72c
			 0x387acdad, /// 0x730
			 0x61e6db82, /// 0x734
			 0x35be0a0c, /// 0x738
			 0xdd12df39, /// 0x73c
			 0x75610d11, /// 0x740
			 0x69ebea66, /// 0x744
			 0x71b741f2, /// 0x748
			 0x50c354e8, /// 0x74c
			 0xd8d9df8f, /// 0x750
			 0xcc015b0e, /// 0x754
			 0xdb9e1ac0, /// 0x758
			 0xe852252a, /// 0x75c
			 0x7d0685df, /// 0x760
			 0xcedb0327, /// 0x764
			 0xed359d3a, /// 0x768
			 0x1aed154c, /// 0x76c
			 0x21033e65, /// 0x770
			 0xe8787816, /// 0x774
			 0x6fcd7691, /// 0x778
			 0x920e0f79, /// 0x77c
			 0x71113d8f, /// 0x780
			 0xd75b91f7, /// 0x784
			 0x50c30cd8, /// 0x788
			 0x5ea01e3d, /// 0x78c
			 0x08acaf4c, /// 0x790
			 0xbb02adef, /// 0x794
			 0x1cdfc77e, /// 0x798
			 0xd593933d, /// 0x79c
			 0x04a1baa1, /// 0x7a0
			 0x196fcb6f, /// 0x7a4
			 0xc1e494fb, /// 0x7a8
			 0xf3b3a7d1, /// 0x7ac
			 0x07ac1b18, /// 0x7b0
			 0x1f719c46, /// 0x7b4
			 0x81cd089e, /// 0x7b8
			 0xba3701aa, /// 0x7bc
			 0x0336c3d4, /// 0x7c0
			 0x14af4e60, /// 0x7c4
			 0xaff03c87, /// 0x7c8
			 0x2bba84b0, /// 0x7cc
			 0xc455961e, /// 0x7d0
			 0x132c9ff4, /// 0x7d4
			 0xca97f28a, /// 0x7d8
			 0x0ddc16f9, /// 0x7dc
			 0x90dbb9a4, /// 0x7e0
			 0xbbd7fb00, /// 0x7e4
			 0x3f379a31, /// 0x7e8
			 0x54b91433, /// 0x7ec
			 0x37dcd633, /// 0x7f0
			 0x22ddb0ab, /// 0x7f4
			 0x5e3c107d, /// 0x7f8
			 0x6cc552c9, /// 0x7fc
			 0x44fd083e, /// 0x800
			 0xff58c24e, /// 0x804
			 0xd8e65e83, /// 0x808
			 0x0d7b11cf, /// 0x80c
			 0x7092eff3, /// 0x810
			 0x937029ef, /// 0x814
			 0x9b142264, /// 0x818
			 0x0819b70c, /// 0x81c
			 0x0ded4b30, /// 0x820
			 0xde9f4930, /// 0x824
			 0xd482b910, /// 0x828
			 0xb033f0b4, /// 0x82c
			 0xe368c32d, /// 0x830
			 0xec7d80e1, /// 0x834
			 0x0f961d61, /// 0x838
			 0xb2422c5c, /// 0x83c
			 0x43cef6cd, /// 0x840
			 0x1656aa88, /// 0x844
			 0x5ec72681, /// 0x848
			 0xbf0a3ea8, /// 0x84c
			 0x5077f0b4, /// 0x850
			 0x5b4b3954, /// 0x854
			 0x75a49aae, /// 0x858
			 0xbe43cf9e, /// 0x85c
			 0xcbaf4a23, /// 0x860
			 0xa5753c5d, /// 0x864
			 0xbcffc5d9, /// 0x868
			 0x30374ba3, /// 0x86c
			 0x07f0a8e9, /// 0x870
			 0xec8c9920, /// 0x874
			 0x0f8e4ac1, /// 0x878
			 0x8a1f7206, /// 0x87c
			 0xca604bb3, /// 0x880
			 0x38c93881, /// 0x884
			 0xe06986cc, /// 0x888
			 0xdb4a0b8e, /// 0x88c
			 0x3ca4b732, /// 0x890
			 0xa610855b, /// 0x894
			 0x5bdb789f, /// 0x898
			 0x8db9a154, /// 0x89c
			 0x6e45ee61, /// 0x8a0
			 0xb2e0ae36, /// 0x8a4
			 0xe6525117, /// 0x8a8
			 0x013fc5d1, /// 0x8ac
			 0x41c64c2d, /// 0x8b0
			 0x76980709, /// 0x8b4
			 0x28c8b50a, /// 0x8b8
			 0x27d6d3ee, /// 0x8bc
			 0xac2775d2, /// 0x8c0
			 0x5ce945c2, /// 0x8c4
			 0x231a6c64, /// 0x8c8
			 0x804781ba, /// 0x8cc
			 0xb2a4028f, /// 0x8d0
			 0x62fcd44c, /// 0x8d4
			 0xa1c0c8b6, /// 0x8d8
			 0xb779a227, /// 0x8dc
			 0xfef150c0, /// 0x8e0
			 0xaec187f9, /// 0x8e4
			 0xe12ad6b1, /// 0x8e8
			 0x6ba32f4e, /// 0x8ec
			 0x67c73962, /// 0x8f0
			 0xee9b3746, /// 0x8f4
			 0x342e6359, /// 0x8f8
			 0x32a41522, /// 0x8fc
			 0x77fd3ffa, /// 0x900
			 0xe207aa31, /// 0x904
			 0x17e127ff, /// 0x908
			 0xcc035656, /// 0x90c
			 0x516e3f0a, /// 0x910
			 0x4d2209a2, /// 0x914
			 0x54f42f22, /// 0x918
			 0xe66ab3f2, /// 0x91c
			 0xfb046b7f, /// 0x920
			 0xa287b62e, /// 0x924
			 0x56d5198c, /// 0x928
			 0x72119383, /// 0x92c
			 0x7eea6d6f, /// 0x930
			 0xcbc18ef4, /// 0x934
			 0xda51d620, /// 0x938
			 0xb5b43fa9, /// 0x93c
			 0x2da6456f, /// 0x940
			 0x3875cf86, /// 0x944
			 0x5ddf31d3, /// 0x948
			 0xa47acacc, /// 0x94c
			 0xf9624609, /// 0x950
			 0xcadb8a34, /// 0x954
			 0x3443d435, /// 0x958
			 0x78f0343b, /// 0x95c
			 0x1a64b31f, /// 0x960
			 0x2e706d8f, /// 0x964
			 0x62b3082c, /// 0x968
			 0xeb3847d4, /// 0x96c
			 0x44561347, /// 0x970
			 0x88fe1bc6, /// 0x974
			 0xfa32b166, /// 0x978
			 0x9b8c2400, /// 0x97c
			 0x90eb21c4, /// 0x980
			 0xccbe6721, /// 0x984
			 0x1c18071c, /// 0x988
			 0xfabb1a45, /// 0x98c
			 0x4af44960, /// 0x990
			 0x3fa2f5e9, /// 0x994
			 0xe7fa9644, /// 0x998
			 0xa42b9425, /// 0x99c
			 0x3c1a9ecc, /// 0x9a0
			 0x7b8592c1, /// 0x9a4
			 0x1313df51, /// 0x9a8
			 0x578948d9, /// 0x9ac
			 0xeafdd007, /// 0x9b0
			 0xcba4ce04, /// 0x9b4
			 0x9fb90f8e, /// 0x9b8
			 0x452d0c06, /// 0x9bc
			 0x3c2f4832, /// 0x9c0
			 0x4bf3547c, /// 0x9c4
			 0xaff170e2, /// 0x9c8
			 0xaf4632c0, /// 0x9cc
			 0x3e330091, /// 0x9d0
			 0x9413f4de, /// 0x9d4
			 0x7a0379a1, /// 0x9d8
			 0x69165a28, /// 0x9dc
			 0xe6049ccb, /// 0x9e0
			 0x49f12d1e, /// 0x9e4
			 0x0deccb24, /// 0x9e8
			 0xe2ff39e6, /// 0x9ec
			 0x5cb4071e, /// 0x9f0
			 0x7517a401, /// 0x9f4
			 0x2dd5d964, /// 0x9f8
			 0x628142ef, /// 0x9fc
			 0xb1526d7b, /// 0xa00
			 0xad8dedf7, /// 0xa04
			 0xf4b0f2db, /// 0xa08
			 0xdbdf6561, /// 0xa0c
			 0x69e30011, /// 0xa10
			 0xcaf0d98a, /// 0xa14
			 0xe6db9769, /// 0xa18
			 0x9e44b4ce, /// 0xa1c
			 0x14de1ca6, /// 0xa20
			 0x0b5e0ba4, /// 0xa24
			 0x15abb95d, /// 0xa28
			 0x613a2951, /// 0xa2c
			 0xb205ae3c, /// 0xa30
			 0x4655da81, /// 0xa34
			 0xa333b3c4, /// 0xa38
			 0x40a3b9a3, /// 0xa3c
			 0xee4a3c33, /// 0xa40
			 0x7875fba7, /// 0xa44
			 0xbd8133eb, /// 0xa48
			 0xb05a6b73, /// 0xa4c
			 0xf771af5c, /// 0xa50
			 0x2a870ed9, /// 0xa54
			 0x129a5365, /// 0xa58
			 0x5a8f1dc3, /// 0xa5c
			 0x595435f3, /// 0xa60
			 0xea9707f4, /// 0xa64
			 0x0f3d99ea, /// 0xa68
			 0xd012b019, /// 0xa6c
			 0x3e645d38, /// 0xa70
			 0xfa3aa085, /// 0xa74
			 0xaae3fea3, /// 0xa78
			 0xc8fef9e9, /// 0xa7c
			 0x8ce1b95f, /// 0xa80
			 0xffb6bb4d, /// 0xa84
			 0x3fc17b3f, /// 0xa88
			 0x0725dbc0, /// 0xa8c
			 0xa47b1c13, /// 0xa90
			 0xa076b01c, /// 0xa94
			 0xcc53574e, /// 0xa98
			 0xc6d678ab, /// 0xa9c
			 0xda99febf, /// 0xaa0
			 0x3d67130b, /// 0xaa4
			 0x8f24be86, /// 0xaa8
			 0x195ff988, /// 0xaac
			 0x499f6f4e, /// 0xab0
			 0x04c8cab7, /// 0xab4
			 0x1202920a, /// 0xab8
			 0x1fe3199c, /// 0xabc
			 0x6a8eadee, /// 0xac0
			 0x546d74c9, /// 0xac4
			 0x67146ddf, /// 0xac8
			 0xac7d91a9, /// 0xacc
			 0x4f188451, /// 0xad0
			 0xb5f05857, /// 0xad4
			 0x0d3b6d76, /// 0xad8
			 0xe3c6125b, /// 0xadc
			 0x90d719bf, /// 0xae0
			 0xe4afaafc, /// 0xae4
			 0x5986a106, /// 0xae8
			 0xb2000ee5, /// 0xaec
			 0x4e46f662, /// 0xaf0
			 0xe1da4490, /// 0xaf4
			 0x74ba5d2e, /// 0xaf8
			 0x72832379, /// 0xafc
			 0xeb8097c6, /// 0xb00
			 0xd08b00dc, /// 0xb04
			 0x5ce64143, /// 0xb08
			 0xf3bf8926, /// 0xb0c
			 0x784ecff5, /// 0xb10
			 0x0e506484, /// 0xb14
			 0x7aa5947b, /// 0xb18
			 0xb762fa6a, /// 0xb1c
			 0x9e456123, /// 0xb20
			 0xaac13bda, /// 0xb24
			 0xbfe5c189, /// 0xb28
			 0x50695511, /// 0xb2c
			 0x7a83df6c, /// 0xb30
			 0x394558b9, /// 0xb34
			 0x10496fad, /// 0xb38
			 0xbd98c5aa, /// 0xb3c
			 0x9765c018, /// 0xb40
			 0xa1357534, /// 0xb44
			 0x782884fc, /// 0xb48
			 0x4d1f10e1, /// 0xb4c
			 0x6e20c412, /// 0xb50
			 0x1be8b90f, /// 0xb54
			 0xb9e0bbcf, /// 0xb58
			 0x4b720f6b, /// 0xb5c
			 0xaa870332, /// 0xb60
			 0xff54ef30, /// 0xb64
			 0x1ad807b7, /// 0xb68
			 0xc2c1e42f, /// 0xb6c
			 0x3f8911e2, /// 0xb70
			 0x0a2d2484, /// 0xb74
			 0xecfa09a0, /// 0xb78
			 0x803f5dbf, /// 0xb7c
			 0x86a8a14c, /// 0xb80
			 0x1a26c66e, /// 0xb84
			 0x392387a5, /// 0xb88
			 0xdc3de0d5, /// 0xb8c
			 0x2e0551a6, /// 0xb90
			 0x1dcc891e, /// 0xb94
			 0x10a5e0ca, /// 0xb98
			 0xed22779a, /// 0xb9c
			 0xe82eb5f6, /// 0xba0
			 0x50e5b54f, /// 0xba4
			 0x142fbe8f, /// 0xba8
			 0x513a2b20, /// 0xbac
			 0x52131729, /// 0xbb0
			 0x5516e533, /// 0xbb4
			 0xbccec74a, /// 0xbb8
			 0xf8e83b9e, /// 0xbbc
			 0x5904a9a1, /// 0xbc0
			 0x4748dc17, /// 0xbc4
			 0x2108c88a, /// 0xbc8
			 0xe26aab4e, /// 0xbcc
			 0xb969361e, /// 0xbd0
			 0x408a3f38, /// 0xbd4
			 0xbe3159ea, /// 0xbd8
			 0x09edfa0b, /// 0xbdc
			 0xcde7795a, /// 0xbe0
			 0xb70f6c97, /// 0xbe4
			 0xa4c57d01, /// 0xbe8
			 0xa3d80a93, /// 0xbec
			 0x43bc2ff8, /// 0xbf0
			 0x19d8aa05, /// 0xbf4
			 0x683c8387, /// 0xbf8
			 0x5f84bec3, /// 0xbfc
			 0x90e5374f, /// 0xc00
			 0x3a6e2376, /// 0xc04
			 0x6b666b64, /// 0xc08
			 0x218990e9, /// 0xc0c
			 0xa931197a, /// 0xc10
			 0x7e56b782, /// 0xc14
			 0x129087f7, /// 0xc18
			 0x2a38a560, /// 0xc1c
			 0x758b7ecd, /// 0xc20
			 0xd0116353, /// 0xc24
			 0x3ec468d1, /// 0xc28
			 0x6133e445, /// 0xc2c
			 0x008afab8, /// 0xc30
			 0x24260d44, /// 0xc34
			 0xf041d270, /// 0xc38
			 0x6a48d599, /// 0xc3c
			 0x7c01b3a9, /// 0xc40
			 0x47893327, /// 0xc44
			 0xbfea783f, /// 0xc48
			 0x7b3a4467, /// 0xc4c
			 0xb5d49c02, /// 0xc50
			 0x1f5671bb, /// 0xc54
			 0x5f4d4aa9, /// 0xc58
			 0xf8a74bf3, /// 0xc5c
			 0xbad83a7a, /// 0xc60
			 0x396a0016, /// 0xc64
			 0x7cd4b354, /// 0xc68
			 0x7176e5ec, /// 0xc6c
			 0x2686e920, /// 0xc70
			 0x8fa23ff8, /// 0xc74
			 0x51c48b3f, /// 0xc78
			 0x1ea280d0, /// 0xc7c
			 0xede7d5e6, /// 0xc80
			 0x2f61de4b, /// 0xc84
			 0xadba9968, /// 0xc88
			 0x7845d351, /// 0xc8c
			 0x476621b4, /// 0xc90
			 0xb69af03a, /// 0xc94
			 0x56325a10, /// 0xc98
			 0x36c071e3, /// 0xc9c
			 0x30d834ef, /// 0xca0
			 0xea499e6a, /// 0xca4
			 0xbbfb1535, /// 0xca8
			 0xbf84aebe, /// 0xcac
			 0x46eac3f0, /// 0xcb0
			 0x64ead402, /// 0xcb4
			 0x28f20699, /// 0xcb8
			 0xec0f139b, /// 0xcbc
			 0xa099cd9a, /// 0xcc0
			 0xf30ae3c6, /// 0xcc4
			 0x3dee297e, /// 0xcc8
			 0x6eb3d5cc, /// 0xccc
			 0x1bb5555f, /// 0xcd0
			 0x9035e5b6, /// 0xcd4
			 0x47c4eb77, /// 0xcd8
			 0x2d815d14, /// 0xcdc
			 0x0c4854e9, /// 0xce0
			 0x4dd88b22, /// 0xce4
			 0xc5f39eb5, /// 0xce8
			 0x76566b11, /// 0xcec
			 0x4d406710, /// 0xcf0
			 0x2499acc7, /// 0xcf4
			 0x2cb34564, /// 0xcf8
			 0xbd81b304, /// 0xcfc
			 0xf3c2baf1, /// 0xd00
			 0x0760b139, /// 0xd04
			 0x3d86a67e, /// 0xd08
			 0x6ac60f52, /// 0xd0c
			 0x5af1c2e8, /// 0xd10
			 0x3085119d, /// 0xd14
			 0x031508d2, /// 0xd18
			 0xa1c198ba, /// 0xd1c
			 0x3c2cf133, /// 0xd20
			 0x6fd0b109, /// 0xd24
			 0xa1e84e60, /// 0xd28
			 0xdae6c530, /// 0xd2c
			 0xb83bda80, /// 0xd30
			 0x45a2b026, /// 0xd34
			 0xe13b5f13, /// 0xd38
			 0x72d6dc86, /// 0xd3c
			 0x70e12fc7, /// 0xd40
			 0x69fb8b15, /// 0xd44
			 0xd2934fe4, /// 0xd48
			 0x8f8f0e06, /// 0xd4c
			 0x7f222f5a, /// 0xd50
			 0x05b2237c, /// 0xd54
			 0x24598d6d, /// 0xd58
			 0x05a4fa1a, /// 0xd5c
			 0x39980d04, /// 0xd60
			 0xe8e4ffaa, /// 0xd64
			 0x54416acc, /// 0xd68
			 0x3932fac0, /// 0xd6c
			 0x0b39abc1, /// 0xd70
			 0xd5de6e68, /// 0xd74
			 0x1f12a7a5, /// 0xd78
			 0xc68f4f40, /// 0xd7c
			 0x0e123a60, /// 0xd80
			 0xcca34be4, /// 0xd84
			 0x918edb1c, /// 0xd88
			 0xe20ac491, /// 0xd8c
			 0xa6a548f6, /// 0xd90
			 0x11e50195, /// 0xd94
			 0xc6f94b2f, /// 0xd98
			 0x63eca0bd, /// 0xd9c
			 0xe273f901, /// 0xda0
			 0x86b355df, /// 0xda4
			 0xfa5f7f98, /// 0xda8
			 0xbed7def5, /// 0xdac
			 0x5bfcde27, /// 0xdb0
			 0x99b77295, /// 0xdb4
			 0x5a4ee814, /// 0xdb8
			 0x08c7f9d8, /// 0xdbc
			 0xaecacb35, /// 0xdc0
			 0x27356fab, /// 0xdc4
			 0x8458f7b6, /// 0xdc8
			 0xb4b61c7d, /// 0xdcc
			 0xde36632a, /// 0xdd0
			 0x24490c53, /// 0xdd4
			 0xf55e10d4, /// 0xdd8
			 0x34272400, /// 0xddc
			 0x78566269, /// 0xde0
			 0xb7f27a12, /// 0xde4
			 0xb4907439, /// 0xde8
			 0x99dec550, /// 0xdec
			 0xa418f4ec, /// 0xdf0
			 0x4837613f, /// 0xdf4
			 0x7e9c65b9, /// 0xdf8
			 0xd802980b, /// 0xdfc
			 0x628059d0, /// 0xe00
			 0x80da8ee8, /// 0xe04
			 0x641b7458, /// 0xe08
			 0x5c0035b1, /// 0xe0c
			 0x58fe389f, /// 0xe10
			 0x4e55606a, /// 0xe14
			 0xbdf132e4, /// 0xe18
			 0xda70f984, /// 0xe1c
			 0x9b73bbd3, /// 0xe20
			 0x7b7f7fd7, /// 0xe24
			 0x06d71a50, /// 0xe28
			 0xa7c254a5, /// 0xe2c
			 0xe2a22502, /// 0xe30
			 0x4cec4f31, /// 0xe34
			 0x5cdd547f, /// 0xe38
			 0x37e2837c, /// 0xe3c
			 0x8f195f6d, /// 0xe40
			 0x1d52f92a, /// 0xe44
			 0x58d6f38b, /// 0xe48
			 0x38f3771d, /// 0xe4c
			 0xd70c880c, /// 0xe50
			 0x2d2531ae, /// 0xe54
			 0x27fb6a3d, /// 0xe58
			 0x1f31c41e, /// 0xe5c
			 0x35c4e0cb, /// 0xe60
			 0xee50c7ce, /// 0xe64
			 0xd01b981b, /// 0xe68
			 0x3451e2c2, /// 0xe6c
			 0xa602e0b0, /// 0xe70
			 0xade6876a, /// 0xe74
			 0xaa1a16ac, /// 0xe78
			 0xe69bca83, /// 0xe7c
			 0xef77e4a9, /// 0xe80
			 0x095bc7f6, /// 0xe84
			 0x7c6aab84, /// 0xe88
			 0x6f111762, /// 0xe8c
			 0xff672e4c, /// 0xe90
			 0xc7c818e8, /// 0xe94
			 0xa4034e66, /// 0xe98
			 0x3ff6b4af, /// 0xe9c
			 0xf5c49cad, /// 0xea0
			 0x127a243a, /// 0xea4
			 0x5b82d2fe, /// 0xea8
			 0x9e6dadf5, /// 0xeac
			 0x51a1b4a1, /// 0xeb0
			 0x4393ee8e, /// 0xeb4
			 0x30c99fe5, /// 0xeb8
			 0x99e079a7, /// 0xebc
			 0xd691a591, /// 0xec0
			 0xbc3d1002, /// 0xec4
			 0x87eb45db, /// 0xec8
			 0x354dfe42, /// 0xecc
			 0xfc0d5f77, /// 0xed0
			 0x42a28fa5, /// 0xed4
			 0x588bcf23, /// 0xed8
			 0x71f46d8a, /// 0xedc
			 0x710257ec, /// 0xee0
			 0x783b3801, /// 0xee4
			 0x5250de59, /// 0xee8
			 0x597426dd, /// 0xeec
			 0x180285c2, /// 0xef0
			 0xc5f750ea, /// 0xef4
			 0xcf9e5f6d, /// 0xef8
			 0x8428726b, /// 0xefc
			 0xd11a97ba, /// 0xf00
			 0xc12bebcb, /// 0xf04
			 0x5a15053b, /// 0xf08
			 0xe6c02e28, /// 0xf0c
			 0x197b1c8b, /// 0xf10
			 0x58b23ce2, /// 0xf14
			 0xf792709b, /// 0xf18
			 0x26e6bd0d, /// 0xf1c
			 0xa465ba71, /// 0xf20
			 0x9b8e2d88, /// 0xf24
			 0xd675d73e, /// 0xf28
			 0xe81cbf07, /// 0xf2c
			 0x59f9fcd2, /// 0xf30
			 0x294fe40c, /// 0xf34
			 0x9b1b2d95, /// 0xf38
			 0xd8ab939b, /// 0xf3c
			 0x16374998, /// 0xf40
			 0x05d193cc, /// 0xf44
			 0x7cd38832, /// 0xf48
			 0x786c24fb, /// 0xf4c
			 0x40913dd6, /// 0xf50
			 0x7c13270c, /// 0xf54
			 0x8e5f9ba7, /// 0xf58
			 0xa10a16cf, /// 0xf5c
			 0x03890c3e, /// 0xf60
			 0x8e957aae, /// 0xf64
			 0xd54de5d4, /// 0xf68
			 0x2b27b2d6, /// 0xf6c
			 0x0211094e, /// 0xf70
			 0xdb37d554, /// 0xf74
			 0xe807a831, /// 0xf78
			 0xd557d444, /// 0xf7c
			 0xc151551e, /// 0xf80
			 0xa29f81cd, /// 0xf84
			 0xbea358db, /// 0xf88
			 0x9dee8bda, /// 0xf8c
			 0x5511e429, /// 0xf90
			 0x0e429864, /// 0xf94
			 0x05665219, /// 0xf98
			 0x078ebfdb, /// 0xf9c
			 0x8daaeced, /// 0xfa0
			 0xe0b29d0a, /// 0xfa4
			 0x4340a39f, /// 0xfa8
			 0xb949878f, /// 0xfac
			 0x0df115c2, /// 0xfb0
			 0xa4862532, /// 0xfb4
			 0x7d0cf5dc, /// 0xfb8
			 0xf582cbb0, /// 0xfbc
			 0xae91b625, /// 0xfc0
			 0x22a97f94, /// 0xfc4
			 0xbce16452, /// 0xfc8
			 0xa12dcf2c, /// 0xfcc
			 0x5cc9271e, /// 0xfd0
			 0x1dc4af50, /// 0xfd4
			 0x8766441b, /// 0xfd8
			 0x06fb4ebc, /// 0xfdc
			 0xefd28190, /// 0xfe0
			 0x4446f6ef, /// 0xfe4
			 0x1d74edf3, /// 0xfe8
			 0xbddca9f1, /// 0xfec
			 0x3746fe01, /// 0xff0
			 0xfe29d477, /// 0xff4
			 0x03869175, /// 0xff8
			 0x12e791d4 /// last
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
			 0x00000344,
			 0x000004ec,
			 0x0000074c,
			 0x0000044c,
			 0x00000410,
			 0x000003d8,
			 0x000004e0,
			 0x000004a8,

			 /// vpu register f2
			 0x41d80000,
			 0x41300000,
			 0x41900000,
			 0x41d80000,
			 0x41500000,
			 0x41e00000,
			 0x41300000,
			 0x41a00000,

			 /// vpu register f3
			 0x41200000,
			 0x41a00000,
			 0x41500000,
			 0x41e80000,
			 0x41500000,
			 0x3f800000,
			 0x41700000,
			 0x41a00000,

			 /// vpu register f4
			 0x41200000,
			 0x40000000,
			 0x41700000,
			 0x41f00000,
			 0x41100000,
			 0x41100000,
			 0x41800000,
			 0x41a00000,

			 /// vpu register f5
			 0x41900000,
			 0x41880000,
			 0x41600000,
			 0x41f00000,
			 0x41b00000,
			 0x41500000,
			 0x41e80000,
			 0x40400000,

			 /// vpu register f6
			 0x42000000,
			 0x41980000,
			 0x40000000,
			 0x41600000,
			 0x41700000,
			 0x41700000,
			 0x42000000,
			 0x40800000,

			 /// vpu register f7
			 0x41900000,
			 0x41c00000,
			 0x41100000,
			 0x41000000,
			 0x41300000,
			 0x40400000,
			 0x41d00000,
			 0x40e00000,

			 /// vpu register f8
			 0x41200000,
			 0x41a00000,
			 0x41900000,
			 0x41700000,
			 0x41a80000,
			 0x41d00000,
			 0x41900000,
			 0x41980000,

			 /// vpu register f9
			 0x41700000,
			 0x41a00000,
			 0x40e00000,
			 0x40800000,
			 0x41b00000,
			 0x41100000,
			 0x40c00000,
			 0x41b00000,

			 /// vpu register f10
			 0x41500000,
			 0x40e00000,
			 0x41300000,
			 0x41d00000,
			 0x41c00000,
			 0x41d00000,
			 0x40000000,
			 0x41c00000,

			 /// vpu register f11
			 0x41800000,
			 0x3f800000,
			 0x41f80000,
			 0x41b00000,
			 0x41c80000,
			 0x40000000,
			 0x41e00000,
			 0x40800000,

			 /// vpu register f12
			 0x41e00000,
			 0x40000000,
			 0x41e00000,
			 0x41c00000,
			 0x40800000,
			 0x41a00000,
			 0x42000000,
			 0x41c00000,

			 /// vpu register f13
			 0x41600000,
			 0x41c80000,
			 0x40c00000,
			 0x41e80000,
			 0x41f00000,
			 0x41000000,
			 0x41c80000,
			 0x41e00000,

			 /// vpu register f14
			 0x41100000,
			 0x40e00000,
			 0x41400000,
			 0x41c80000,
			 0x41400000,
			 0x41600000,
			 0x41200000,
			 0x41500000,

			 /// vpu register f15
			 0x40800000,
			 0x41c80000,
			 0x41880000,
			 0x40400000,
			 0x41300000,
			 0x41980000,
			 0x41d80000,
			 0x41f00000,

			 /// vpu register f16
			 0x41500000,
			 0x41100000,
			 0x41e00000,
			 0x41e80000,
			 0x41600000,
			 0x41a00000,
			 0x40000000,
			 0x42000000,

			 /// vpu register f17
			 0x41200000,
			 0x41980000,
			 0x41800000,
			 0x41b80000,
			 0x3f800000,
			 0x41600000,
			 0x41e00000,
			 0x41100000,

			 /// vpu register f18
			 0x41b00000,
			 0x41000000,
			 0x41f80000,
			 0x41b80000,
			 0x41600000,
			 0x41900000,
			 0x41b00000,
			 0x41800000,

			 /// vpu register f19
			 0x40c00000,
			 0x41700000,
			 0x40c00000,
			 0x42000000,
			 0x41700000,
			 0x41f00000,
			 0x41c00000,
			 0x41b80000,

			 /// vpu register f20
			 0x40a00000,
			 0x41a00000,
			 0x41000000,
			 0x41700000,
			 0x41a00000,
			 0x41c80000,
			 0x41900000,
			 0x40c00000,

			 /// vpu register f21
			 0x41e80000,
			 0x41e00000,
			 0x42000000,
			 0x41c00000,
			 0x41b00000,
			 0x41700000,
			 0x41b80000,
			 0x41d00000,

			 /// vpu register f22
			 0x41800000,
			 0x41300000,
			 0x41100000,
			 0x41e80000,
			 0x40800000,
			 0x41c00000,
			 0x41e80000,
			 0x41700000,

			 /// vpu register f23
			 0x41700000,
			 0x41700000,
			 0x41b00000,
			 0x41980000,
			 0x40400000,
			 0x41980000,
			 0x41e80000,
			 0x41d00000,

			 /// vpu register f24
			 0x40800000,
			 0x40000000,
			 0x41200000,
			 0x40c00000,
			 0x41400000,
			 0x42000000,
			 0x41c80000,
			 0x40a00000,

			 /// vpu register f25
			 0x41e00000,
			 0x3f800000,
			 0x41980000,
			 0x41b00000,
			 0x41a80000,
			 0x41200000,
			 0x41980000,
			 0x41200000,

			 /// vpu register f26
			 0x41100000,
			 0x40800000,
			 0x41d00000,
			 0x40a00000,
			 0x41700000,
			 0x41d00000,
			 0x41b80000,
			 0x41200000,

			 /// vpu register f27
			 0x40800000,
			 0x40000000,
			 0x41600000,
			 0x41900000,
			 0x40e00000,
			 0x41b00000,
			 0x40000000,
			 0x41f00000,

			 /// vpu register f28
			 0x41800000,
			 0x41e00000,
			 0x41800000,
			 0x41700000,
			 0x41500000,
			 0x41e80000,
			 0x41d00000,
			 0x40e00000,

			 /// vpu register f29
			 0x41d80000,
			 0x40000000,
			 0x41400000,
			 0x41600000,
			 0x40400000,
			 0x41600000,
			 0x40800000,
			 0x41d80000,

			 /// vpu register f30
			 0x41c80000,
			 0x41c80000,
			 0x41880000,
			 0x41800000,
			 0x41b80000,
			 0x41600000,
			 0x41600000,
			 0x41300000,

			 /// vpu register f31
			 0x41c00000,
			 0x41880000,
			 0x41f00000,
			 0x41c80000,
			 0x41a80000,
			 0x41d00000,
			 0x41f80000,
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
		"fsatu8.pi f9, f23\n"                                 ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f29, f24\n"                                ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f14, f15\n"                                ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f12, f8\n"                                 ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f15, f7\n"                                 ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f19, f22\n"                                ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f4, f25\n"                                 ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f14, f1\n"                                 ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f10, f1\n"                                 ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f5, f15\n"                                 ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f28, f14\n"                                ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f16, f12\n"                                ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f20, f16\n"                                ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f25, f21\n"                                ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f29, f8\n"                                 ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f21, f20\n"                                ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f10, f1\n"                                 ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f2, f8\n"                                  ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f24, f1\n"                                 ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f24, f17\n"                                ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f13, f17\n"                                ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f9, f13\n"                                 ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f13, f26\n"                                ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f3, f20\n"                                 ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f17, f13\n"                                ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f14, f9\n"                                 ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f18, f11\n"                                ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f4, f15\n"                                 ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f4, f27\n"                                 ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f17, f4\n"                                 ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f23, f22\n"                                ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f3, f4\n"                                  ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f28, f8\n"                                 ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f24, f7\n"                                 ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f6, f23\n"                                 ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f2, f22\n"                                 ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f15, f21\n"                                ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f18, f19\n"                                ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f25, f3\n"                                 ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f16, f7\n"                                 ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f26, f11\n"                                ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f15, f24\n"                                ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f28, f14\n"                                ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f12, f21\n"                                ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f28, f13\n"                                ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f26, f28\n"                                ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f17, f0\n"                                 ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f14, f13\n"                                ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f12, f24\n"                                ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f5, f6\n"                                  ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f4, f1\n"                                  ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f17, f13\n"                                ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f12, f29\n"                                ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f19, f11\n"                                ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f5, f12\n"                                 ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f4, f29\n"                                 ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f17, f15\n"                                ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f24, f21\n"                                ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f11, f17\n"                                ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f2, f0\n"                                  ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f11, f15\n"                                ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f8, f30\n"                                 ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f25, f16\n"                                ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f29, f16\n"                                ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f26, f13\n"                                ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f17, f1\n"                                 ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f23, f26\n"                                ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f16, f22\n"                                ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f16, f2\n"                                 ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f21, f11\n"                                ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f5, f20\n"                                 ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f25, f15\n"                                ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f10, f13\n"                                ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f13, f4\n"                                 ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f8, f19\n"                                 ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f6, f26\n"                                 ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f23, f2\n"                                 ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f4, f13\n"                                 ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f23, f10\n"                                ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f9, f21\n"                                 ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f30, f5\n"                                 ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f2, f20\n"                                 ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f27, f23\n"                                ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f28, f3\n"                                 ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f15, f3\n"                                 ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f14, f17\n"                                ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f7, f17\n"                                 ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f5, f18\n"                                 ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f17, f19\n"                                ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f20, f11\n"                                ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f11, f0\n"                                 ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f28, f25\n"                                ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f8, f3\n"                                  ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f30, f29\n"                                ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f26, f29\n"                                ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f8, f9\n"                                  ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f6, f14\n"                                 ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f7, f15\n"                                 ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsatu8.pi f20, f10\n"                                ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
