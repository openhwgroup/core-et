/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//////////////////////////////////////////////////////////////////////// 
////// VPU FOCUS TEST: seed = 0x1  = 1
////// VPU FOCUS TEST: isa_focus = vpu_custom
////// VPU FOCUS TEST: uarch_focus = fsin
////// VPU FOCUS TEST: cvg_choice = 0
//////////////////////////////////////////////////////////////////////// 
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
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x3f028f5c,                                                  // 0.51                                        /// 00004
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00008
			 0x4b000000,                                                  // 8388608.0                                   /// 0000c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00010
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00014
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00018
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0001c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00020
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00024
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0002c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00034
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00038
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00040
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00044
			 0xbf028f5c,                                                  // -0.51                                       /// 00048
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0004c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00050
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00054
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00058
			 0x33333333,                                                  // 4 random values                             /// 0005c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00068
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0006c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00074
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00078
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0007c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x00011111,                                                  // 9.7958E-41                                  /// 00084
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00088
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0008c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00090
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00094
			 0xbf028f5c,                                                  // -0.51                                       /// 00098
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0009c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000ac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000b0
			 0xbf028f5c,                                                  // -0.51                                       /// 000b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000cc
			 0x33333333,                                                  // 4 random values                             /// 000d0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000d8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000e8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000fc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00100
			 0x0e000007,                                                  // Trailing 1s:                                /// 00104
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00108
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0010c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00110
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00114
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00118
			 0x7fc00001,                                                  // signaling NaN                               /// 0011c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00120
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0xbf028f5c,                                                  // -0.51                                       /// 00128
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00130
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00134
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00138
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0013c
			 0xcb000000,                                                  // -8388608.0                                  /// 00140
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00148
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00158
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0015c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00160
			 0x0c780000,                                                  // Leading 1s:                                 /// 00164
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00168
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00170
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00174
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0017c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00180
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00184
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0xcb000000,                                                  // -8388608.0                                  /// 0018c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00190
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00194
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00198
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001a4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001c0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001d0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001d8
			 0x7fc00001,                                                  // signaling NaN                               /// 001dc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001ec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001fc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00200
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00204
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00208
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00218
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0021c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0022c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00230
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00244
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00248
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0024c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00254
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00258
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0025c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00260
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00264
			 0x55555555,                                                  // 4 random values                             /// 00268
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0026c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00274
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0027c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00280
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00288
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0028c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00290
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00294
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00298
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0029c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002a8
			 0x33333333,                                                  // 4 random values                             /// 002ac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002b0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002d4
			 0xff800000,                                                  // -inf                                        /// 002d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002ec
			 0x00011111,                                                  // 9.7958E-41                                  /// 002f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002f4
			 0x80011111,                                                  // -9.7958E-41                                 /// 002f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00300
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00304
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0030c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00310
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0031c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00320
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00324
			 0x0c400000,                                                  // Leading 1s:                                 /// 00328
			 0x0c700000,                                                  // Leading 1s:                                 /// 0032c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00330
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00334
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0033c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00340
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00344
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00348
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0034c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00350
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00358
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0035c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00360
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00364
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00368
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0036c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00370
			 0x33333333,                                                  // 4 random values                             /// 00374
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00378
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0037c
			 0x00000000,                                                  // zero                                        /// 00380
			 0x55555555,                                                  // 4 random values                             /// 00384
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00388
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0038c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00390
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00394
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00398
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003a4
			 0x00000000,                                                  // zero                                        /// 003a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003ac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003bc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003dc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003e0
			 0x7f800000,                                                  // inf                                         /// 003e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003f8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00400
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00408
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0040c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00414
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00418
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0041c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00420
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0042c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00434
			 0x3f028f5c,                                                  // 0.51                                        /// 00438
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0043c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00440
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00444
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0044c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00454
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00458
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00460
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00468
			 0x4b000000,                                                  // 8388608.0                                   /// 0046c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00470
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00478
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00484
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0048c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00490
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00494
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004b4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004b8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x33333333,                                                  // 4 random values                             /// 004c4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004d8
			 0xff800000,                                                  // -inf                                        /// 004dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004f8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00500
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00504
			 0xcb000000,                                                  // -8388608.0                                  /// 00508
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0xbf028f5c,                                                  // -0.51                                       /// 00510
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00514
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00518
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0051c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00520
			 0x00011111,                                                  // 9.7958E-41                                  /// 00524
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0052c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00530
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00538
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0053c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00540
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0054c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00550
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00558
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0055c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00564
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00568
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0056c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00570
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00578
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0057c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00580
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00588
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0058c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00594
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00598
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005a4
			 0x3f028f5c,                                                  // 0.51                                        /// 005a8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005b8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005bc
			 0x3f028f5c,                                                  // 0.51                                        /// 005c0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005d4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005d8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005fc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00600
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00604
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00608
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00610
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0061c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00620
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00628
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0062c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x33333333,                                                  // 4 random values                             /// 00638
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00640
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00654
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00658
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0065c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00660
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00664
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00668
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0066c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00670
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0067c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00680
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00684
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00688
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0068c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00690
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00694
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00698
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0069c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006a8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006b8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006c8
			 0x55555555,                                                  // 4 random values                             /// 006cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 006e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006f0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00700
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00704
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0070c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00710
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00714
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00718
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0071c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00724
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00728
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0072c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00730
			 0x0c600000,                                                  // Leading 1s:                                 /// 00734
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00738
			 0x7fc00001,                                                  // signaling NaN                               /// 0073c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00740
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00744
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00748
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00750
			 0x0e000003,                                                  // Trailing 1s:                                /// 00754
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00758
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00760
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00768
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0076c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00770
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0077c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00780
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00784
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00788
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00790
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00794
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00798
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0079c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007b4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007c8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007d8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00800
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00804
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00808
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0080c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00818
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00820
			 0x0c600000,                                                  // Leading 1s:                                 /// 00824
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00828
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0082c
			 0x4b000000,                                                  // 8388608.0                                   /// 00830
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00834
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00844
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00848
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0084c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00850
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00854
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00858
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00860
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00864
			 0x55555555,                                                  // 4 random values                             /// 00868
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0086c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00870
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00874
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0087c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00888
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0088c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00894
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00898
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0089c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008bc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x3f028f5c,                                                  // 0.51                                        /// 008d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008f0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00900
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00904
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x33333333,                                                  // 4 random values                             /// 0090c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00910
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00914
			 0xcb000000,                                                  // -8388608.0                                  /// 00918
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00924
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00930
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00934
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00948
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0094c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00950
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00958
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00960
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00968
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0096c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00970
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00974
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0097c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00980
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00984
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0098c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00998
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0099c
			 0x7fc00001,                                                  // signaling NaN                               /// 009a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009a8
			 0x3f028f5c,                                                  // 0.51                                        /// 009ac
			 0xbf028f5c,                                                  // -0.51                                       /// 009b0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009b8
			 0x80000000,                                                  // -zero                                       /// 009bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009d8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009e4
			 0xcb000000,                                                  // -8388608.0                                  /// 009e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a00
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a04
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a14
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a1c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a20
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a24
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a28
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a2c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a30
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a3c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a44
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a48
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a4c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a50
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a54
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a5c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a60
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a68
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a70
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a74
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a7c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a80
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a88
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a90
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a94
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00aa0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00aa4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00aac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ab0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ab4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ab8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00abc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ac4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ac8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00acc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ad8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00adc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ae0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ae4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ae8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00aec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00af0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00af4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00af8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00afc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b00
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b04
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b08
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b0c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b10
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b14
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b18
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b1c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b20
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b24
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b2c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b30
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b34
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b38
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b4c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b54
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b58
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b5c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b60
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b64
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b68
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b6c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b74
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b78
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b7c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b84
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b88
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b8c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b90
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b94
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b98
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b9c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ba0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ba4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ba8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bb0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bc0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bc4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bcc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bd0
			 0xbf028f5c,                                                  // -0.51                                       /// 00bd4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bd8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00be0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00be4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00be8
			 0x7f800000,                                                  // inf                                         /// 00bec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bfc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c08
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c14
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c18
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c1c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c20
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c28
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c2c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c30
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c34
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c3c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c44
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c48
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c50
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c54
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c58
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c5c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c60
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c68
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c6c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c74
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c78
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c84
			 0xbf028f5c,                                                  // -0.51                                       /// 00c88
			 0xff800000,                                                  // -inf                                        /// 00c8c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c98
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c9c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ca0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ca8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cb8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cc4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cd0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cd4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0xbf028f5c,                                                  // -0.51                                       /// 00cdc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ce0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ce4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cf0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cf4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cf8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cfc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d00
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d04
			 0x33333333,                                                  // 4 random values                             /// 00d08
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d0c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0xcb000000,                                                  // -8388608.0                                  /// 00d14
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d1c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d20
			 0xff800000,                                                  // -inf                                        /// 00d24
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d30
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d34
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d38
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d3c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d44
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d48
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d4c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d50
			 0x55555555,                                                  // 4 random values                             /// 00d54
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d58
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d60
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d64
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d74
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d78
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d80
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d84
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d88
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d8c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d90
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d98
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d9c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00da8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00dac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00db4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00db8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dbc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dc0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dc4
			 0x7f800000,                                                  // inf                                         /// 00dc8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x7fc00001,                                                  // signaling NaN                               /// 00dd0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dd8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ddc
			 0xbf028f5c,                                                  // -0.51                                       /// 00de0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00de8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x0e000001,                                                  // Trailing 1s:                                /// 00df0
			 0xffc00001,                                                  // -signaling NaN                              /// 00df4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00df8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dfc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e00
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e04
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e08
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e0c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e10
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e18
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e20
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e24
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e28
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e30
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e34
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e38
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e40
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e44
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e50
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e54
			 0x33333333,                                                  // 4 random values                             /// 00e58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e5c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e64
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e68
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e6c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e70
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e78
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e7c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e80
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e84
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e90
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e98
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0xcb000000,                                                  // -8388608.0                                  /// 00ea0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ea4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ea8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00eac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00eb0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00eb4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00eb8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ebc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ec0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ec8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ed0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ed8
			 0x00000000,                                                  // zero                                        /// 00edc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ee0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ee4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ee8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ef4
			 0x33333333,                                                  // 4 random values                             /// 00ef8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00efc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f00
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f08
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f0c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f10
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f14
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f18
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f20
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f2c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f30
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f34
			 0x3f028f5c,                                                  // 0.51                                        /// 00f38
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f3c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f44
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f48
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f4c
			 0x7f800000,                                                  // inf                                         /// 00f50
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f58
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f5c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f64
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f68
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f6c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f7c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f88
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f94
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f98
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f9c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fa0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fa4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fa8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fb0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fb4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fb8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fbc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fc0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fc4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fc8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fcc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fd0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fd4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fd8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fdc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fe0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ff0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ff8
			 0x80000800                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00004
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00008
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00010
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x4b000000,                                                  // 8388608.0                                   /// 00018
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0001c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00024
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0002c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00030
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00034
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00040
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00048
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0004c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00050
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00054
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0005c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00060
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00068
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0006c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00070
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00078
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00080
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00084
			 0x0c600000,                                                  // Leading 1s:                                 /// 00088
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0008c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00090
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00094
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00098
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0009c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000a8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000bc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000c0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000c8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000cc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000d4
			 0xffc00001,                                                  // -signaling NaN                              /// 000d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000e0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00100
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0010c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00114
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00118
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0011c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00120
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00124
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x0c600000,                                                  // Leading 1s:                                 /// 0012c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x7f800000,                                                  // inf                                         /// 00138
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0013c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00140
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00144
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00148
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0014c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00150
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00154
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00158
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0015c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00160
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00168
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00170
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00174
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00178
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0017c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00180
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00188
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0018c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00194
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0xff800000,                                                  // -inf                                        /// 0019c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001a8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001ac
			 0x3f028f5c,                                                  // 0.51                                        /// 001b0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001b4
			 0xff800000,                                                  // -inf                                        /// 001b8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x7fc00001,                                                  // signaling NaN                               /// 001c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001d0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001d8
			 0x7f800000,                                                  // inf                                         /// 001dc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001e4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001ec
			 0xffc00001,                                                  // -signaling NaN                              /// 001f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 001f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0020c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00210
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00214
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00220
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00224
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00228
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0022c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00230
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00234
			 0x80011111,                                                  // -9.7958E-41                                 /// 00238
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0023c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00240
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00248
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0024c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00250
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00254
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00258
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00260
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00264
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00270
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00274
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00278
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0027c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00280
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00284
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00288
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0028c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00290
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x55555555,                                                  // 4 random values                             /// 0029c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002a0
			 0x0c400000,                                                  // Leading 1s:                                 /// 002a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002ac
			 0x0e000003,                                                  // Trailing 1s:                                /// 002b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002cc
			 0x80000000,                                                  // -zero                                       /// 002d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002e4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002f0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00304
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0030c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00314
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00318
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0031c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00320
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00328
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0032c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00330
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00334
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00338
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0033c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00340
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00344
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00354
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00358
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0035c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00364
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0036c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00374
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00378
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0037c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00380
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00384
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00388
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00398
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0039c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003a8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003bc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003d4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003ec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003f0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003f4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 003fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00400
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00404
			 0x0e000007,                                                  // Trailing 1s:                                /// 00408
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0040c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00410
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00414
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00420
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00424
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x4b000000,                                                  // 8388608.0                                   /// 0042c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00434
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00440
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00444
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00448
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0044c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00450
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00454
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00458
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0045c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00460
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00464
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00468
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0046c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00470
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00478
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0047c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00488
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0048c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00490
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00494
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0xffc00001,                                                  // -signaling NaN                              /// 0049c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004a8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004c0
			 0x33333333,                                                  // 4 random values                             /// 004c4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004c8
			 0xcb000000,                                                  // -8388608.0                                  /// 004cc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004d0
			 0x7f800000,                                                  // inf                                         /// 004d4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004e0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004e4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004ec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 004f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00500
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00504
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00508
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00510
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00514
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00518
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0051c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00534
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00538
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0053c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00540
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x55555555,                                                  // 4 random values                             /// 0054c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00550
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00554
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00558
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0055c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00564
			 0x80000000,                                                  // -zero                                       /// 00568
			 0x0e000003,                                                  // Trailing 1s:                                /// 0056c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00574
			 0x0c780000,                                                  // Leading 1s:                                 /// 00578
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0057c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00580
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00584
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0058c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00590
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00594
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00598
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0059c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005a8
			 0x0c780000,                                                  // Leading 1s:                                 /// 005ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005b0
			 0x7fc00001,                                                  // signaling NaN                               /// 005b4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005b8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0xff800000,                                                  // -inf                                        /// 005c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005c8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005d4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 005dc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005e0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005e8
			 0x55555555,                                                  // 4 random values                             /// 005ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00608
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0060c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00610
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00614
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00620
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00624
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00628
			 0x0c780000,                                                  // Leading 1s:                                 /// 0062c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00630
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00638
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00640
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00644
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00648
			 0x7f800000,                                                  // inf                                         /// 0064c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00658
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0065c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00660
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00664
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00668
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00670
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00674
			 0x00011111,                                                  // 9.7958E-41                                  /// 00678
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0067c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00680
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x4b000000,                                                  // 8388608.0                                   /// 00688
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00690
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00694
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x80000000,                                                  // -zero                                       /// 0069c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006a0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006a4
			 0x7fc00001,                                                  // signaling NaN                               /// 006a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006b0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006b4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006b8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006dc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 006e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006e8
			 0x0c780000,                                                  // Leading 1s:                                 /// 006ec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006f4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00700
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0070c
			 0xcb000000,                                                  // -8388608.0                                  /// 00710
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00714
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0071c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00724
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00728
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0072c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00730
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00734
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0073c
			 0x80000000,                                                  // -zero                                       /// 00740
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00744
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00748
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0074c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00750
			 0x0c600000,                                                  // Leading 1s:                                 /// 00754
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00758
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0075c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00768
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0076c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00770
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0077c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00784
			 0x0c400000,                                                  // Leading 1s:                                 /// 00788
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00790
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00798
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0079c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007a8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007b4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007b8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007c4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007c8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 007e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x3f028f5c,                                                  // 0.51                                        /// 007f0
			 0x00000000,                                                  // zero                                        /// 007f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007f8
			 0x7f800000,                                                  // inf                                         /// 007fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00800
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00804
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00808
			 0x55555555,                                                  // 4 random values                             /// 0080c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00810
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00814
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00818
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0081c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00820
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00824
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00828
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00830
			 0xff800000,                                                  // -inf                                        /// 00834
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00854
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00858
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0085c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00860
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0086c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00874
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00878
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0087c
			 0x33333333,                                                  // 4 random values                             /// 00880
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00884
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00888
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00890
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00898
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008b0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008b4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008b8
			 0xff800000,                                                  // -inf                                        /// 008bc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008c0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008cc
			 0x00000000,                                                  // zero                                        /// 008d0
			 0x0e000007,                                                  // Trailing 1s:                                /// 008d4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 008dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008e8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008ec
			 0x80000000,                                                  // -zero                                       /// 008f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008f8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00900
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00908
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0090c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00910
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00914
			 0xff800000,                                                  // -inf                                        /// 00918
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0091c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00920
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00924
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00928
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00930
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00934
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00938
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0093c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00940
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00944
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00948
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00950
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00958
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0095c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00960
			 0x0c600000,                                                  // Leading 1s:                                 /// 00964
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0096c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00974
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00978
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00980
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00984
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00988
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0098c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x55555555,                                                  // 4 random values                             /// 00994
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00998
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0099c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009a0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009a4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009b0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009b4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009bc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009c0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009c8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009d8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009e4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009ec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009f8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a00
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a04
			 0x7fc00001,                                                  // signaling NaN                               /// 00a08
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a10
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a1c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a20
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a28
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a2c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a30
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a3c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a40
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a50
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a54
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a58
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a5c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a60
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a64
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0xbf028f5c,                                                  // -0.51                                       /// 00a6c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a74
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a78
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a80
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a84
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a88
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a8c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a90
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a94
			 0x7fc00001,                                                  // signaling NaN                               /// 00a98
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a9c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00aa0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00aa4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00aa8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00aac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ab0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ab4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ab8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00abc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ac0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ac4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ac8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00acc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ad0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ad4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ad8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00adc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ae0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ae4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ae8
			 0x00000000,                                                  // zero                                        /// 00aec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00af0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00af4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x7fc00001,                                                  // signaling NaN                               /// 00b00
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b04
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b08
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x7f800000,                                                  // inf                                         /// 00b10
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b14
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b1c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b20
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b24
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b2c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b38
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b3c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b40
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b44
			 0x55555555,                                                  // 4 random values                             /// 00b48
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b4c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b50
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b5c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b68
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b6c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x4b000000,                                                  // 8388608.0                                   /// 00b74
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b78
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b7c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b80
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b84
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b8c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b90
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b94
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b98
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b9c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ba0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ba4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ba8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bb0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bbc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bc0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bc4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bc8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bcc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bd4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bd8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bdc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00be4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00be8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bf0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bf4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bfc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c04
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c08
			 0x7f800000,                                                  // inf                                         /// 00c0c
			 0x80000000,                                                  // -zero                                       /// 00c10
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c18
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c1c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c20
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c24
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c28
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c2c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c34
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c38
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c58
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c5c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x80000000,                                                  // -zero                                       /// 00c64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c74
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c78
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c8c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c90
			 0xff800000,                                                  // -inf                                        /// 00c94
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c98
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c9c
			 0xbf028f5c,                                                  // -0.51                                       /// 00ca0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ca4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ca8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cb8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cbc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cc0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cc4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ccc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cd0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cd4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cdc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ce0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ce4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ce8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cf0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cf4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cf8
			 0x4b000000,                                                  // 8388608.0                                   /// 00cfc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d04
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d08
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d0c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d10
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d14
			 0x80000000,                                                  // -zero                                       /// 00d18
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d1c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d24
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d28
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d2c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d30
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d38
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d3c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d40
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d44
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d48
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d4c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d50
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x55555555,                                                  // 4 random values                             /// 00d5c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d60
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d64
			 0x55555555,                                                  // 4 random values                             /// 00d68
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d6c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d70
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d74
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d7c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d80
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d88
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d94
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d9c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00da0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00da4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00da8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00db0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00db4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dc4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dc8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dcc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dd0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dd4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dd8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ddc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00de0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00de4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00df0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00df4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00df8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dfc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e00
			 0xcb000000,                                                  // -8388608.0                                  /// 00e04
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e0c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e14
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e18
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e1c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e34
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e38
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e40
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e44
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e4c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e50
			 0x55555555,                                                  // 4 random values                             /// 00e54
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e5c
			 0xcb000000,                                                  // -8388608.0                                  /// 00e60
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e64
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x7fc00001,                                                  // signaling NaN                               /// 00e70
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e74
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e78
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e80
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e90
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ea0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ea4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ea8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00eac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00eb0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00eb8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ebc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ec8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ecc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ed4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ed8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ee0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ee4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ef0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ef8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00efc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f08
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f10
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f14
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f18
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f20
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f2c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f3c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f40
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f44
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f48
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f4c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f58
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f5c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f64
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f68
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f6c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f78
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f7c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f84
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f88
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f8c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f90
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f94
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f9c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fb0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fb4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fb8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fbc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fc0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fc4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fc8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fcc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fd0
			 0x80000000,                                                  // -zero                                       /// 00fd4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fd8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fdc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fe0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fe8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ff0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ff4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ff8
			 0x80800003 // min norm + 3ulp                               // subnormals -ve                              /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x7e8a8295, /// 0x0
			 0x25e69bee, /// 0x4
			 0xcbc93c86, /// 0x8
			 0x72a1b785, /// 0xc
			 0xb84c528c, /// 0x10
			 0x5405233e, /// 0x14
			 0xac0e2a8c, /// 0x18
			 0x68c3cee0, /// 0x1c
			 0x3039ba5c, /// 0x20
			 0x30f9638a, /// 0x24
			 0xe76ff890, /// 0x28
			 0x82343e2d, /// 0x2c
			 0x8e8f3c0e, /// 0x30
			 0x52d23a2f, /// 0x34
			 0xd9f556c5, /// 0x38
			 0xe99ef8eb, /// 0x3c
			 0xdfd53083, /// 0x40
			 0xf2c978e6, /// 0x44
			 0xfa2249fa, /// 0x48
			 0x88e109cf, /// 0x4c
			 0xd80ab1bc, /// 0x50
			 0xf186b69a, /// 0x54
			 0x6015f083, /// 0x58
			 0x30c63211, /// 0x5c
			 0x629f0c00, /// 0x60
			 0x90b7803a, /// 0x64
			 0x114a6500, /// 0x68
			 0x7580875d, /// 0x6c
			 0x7f6aa995, /// 0x70
			 0xbc105165, /// 0x74
			 0xbbfbc47d, /// 0x78
			 0xc6a0f910, /// 0x7c
			 0x7edbf0ad, /// 0x80
			 0x2d863442, /// 0x84
			 0x661b486c, /// 0x88
			 0x84f9e540, /// 0x8c
			 0xc21a3683, /// 0x90
			 0xbcd9d641, /// 0x94
			 0xe3712e4b, /// 0x98
			 0x584b0871, /// 0x9c
			 0xb417d0ef, /// 0xa0
			 0x88f0a527, /// 0xa4
			 0xbb1f3123, /// 0xa8
			 0xda3a1a57, /// 0xac
			 0x22009c16, /// 0xb0
			 0x45c24c02, /// 0xb4
			 0xe6effd0a, /// 0xb8
			 0x97298a7f, /// 0xbc
			 0x90007803, /// 0xc0
			 0xb7393fcb, /// 0xc4
			 0xc3e496b1, /// 0xc8
			 0xaa0e8146, /// 0xcc
			 0x909f6ee2, /// 0xd0
			 0x0d23e0ae, /// 0xd4
			 0xc4d880d6, /// 0xd8
			 0x660fc0f3, /// 0xdc
			 0x7dc63d71, /// 0xe0
			 0x78638226, /// 0xe4
			 0x7e2ee819, /// 0xe8
			 0x0e6190d9, /// 0xec
			 0xeb732226, /// 0xf0
			 0x5ebeeb67, /// 0xf4
			 0xc436baf2, /// 0xf8
			 0x6fb2aafe, /// 0xfc
			 0x9f1959eb, /// 0x100
			 0x448cf39c, /// 0x104
			 0x184e1c0f, /// 0x108
			 0x9df8ba97, /// 0x10c
			 0xbc59430c, /// 0x110
			 0x6b224374, /// 0x114
			 0x9ecef3b3, /// 0x118
			 0x5ab37eb4, /// 0x11c
			 0xabdd3807, /// 0x120
			 0x149c8b57, /// 0x124
			 0x9965663a, /// 0x128
			 0x73c690e2, /// 0x12c
			 0x39c539e0, /// 0x130
			 0x277b1e69, /// 0x134
			 0xc0632943, /// 0x138
			 0xbaf43c29, /// 0x13c
			 0x2e767336, /// 0x140
			 0x44f2f1a2, /// 0x144
			 0xc521b83e, /// 0x148
			 0xad8b54e8, /// 0x14c
			 0xf732b514, /// 0x150
			 0xe7a2071f, /// 0x154
			 0xc0677c35, /// 0x158
			 0xb4548fda, /// 0x15c
			 0x195b22d8, /// 0x160
			 0xaff660df, /// 0x164
			 0x4ed79a7c, /// 0x168
			 0x976e573d, /// 0x16c
			 0x827c1e68, /// 0x170
			 0x496dc981, /// 0x174
			 0x39777505, /// 0x178
			 0x649e118d, /// 0x17c
			 0x7b70aa45, /// 0x180
			 0x751a8d5c, /// 0x184
			 0x01d78cf9, /// 0x188
			 0xe77b19fd, /// 0x18c
			 0x4de86b85, /// 0x190
			 0x669c3ffc, /// 0x194
			 0xcf0ef4a6, /// 0x198
			 0xd9a394a5, /// 0x19c
			 0x2307af20, /// 0x1a0
			 0x7e5260e0, /// 0x1a4
			 0x80e3fdc6, /// 0x1a8
			 0xc401cc79, /// 0x1ac
			 0x27922f6a, /// 0x1b0
			 0xf6d7e8a9, /// 0x1b4
			 0xb8c3f287, /// 0x1b8
			 0xc68fbb5e, /// 0x1bc
			 0x0940943e, /// 0x1c0
			 0xfcf91c77, /// 0x1c4
			 0xf10b1b46, /// 0x1c8
			 0xd359665a, /// 0x1cc
			 0xc5146b97, /// 0x1d0
			 0x4114a7a6, /// 0x1d4
			 0x20690936, /// 0x1d8
			 0x2b0671d1, /// 0x1dc
			 0x4c1aabab, /// 0x1e0
			 0xd6d408f5, /// 0x1e4
			 0x0a1a0c30, /// 0x1e8
			 0xdb620740, /// 0x1ec
			 0x349aa50a, /// 0x1f0
			 0x1562efa4, /// 0x1f4
			 0x5e7f918b, /// 0x1f8
			 0xf3d463f8, /// 0x1fc
			 0x6e5fff77, /// 0x200
			 0x8bf45215, /// 0x204
			 0x29c7abfd, /// 0x208
			 0x5c5cdca1, /// 0x20c
			 0x91d9af00, /// 0x210
			 0x19b6137e, /// 0x214
			 0x0e081fa8, /// 0x218
			 0x2653abf8, /// 0x21c
			 0xc3c342a9, /// 0x220
			 0xcb623e43, /// 0x224
			 0xc1cc6499, /// 0x228
			 0x3fb6229e, /// 0x22c
			 0x8992e47d, /// 0x230
			 0x15e438c7, /// 0x234
			 0x3bc1c7aa, /// 0x238
			 0x3caa2c96, /// 0x23c
			 0x739f6e36, /// 0x240
			 0xa4f9ea1c, /// 0x244
			 0x8f959643, /// 0x248
			 0xaf9d83f5, /// 0x24c
			 0x2d30e5f1, /// 0x250
			 0x8b0099c3, /// 0x254
			 0x8fa3484f, /// 0x258
			 0x2b895565, /// 0x25c
			 0xfb66cd8e, /// 0x260
			 0xc6d3a032, /// 0x264
			 0x34f5a438, /// 0x268
			 0x77d457ce, /// 0x26c
			 0xe50f80b7, /// 0x270
			 0x9ff1dccc, /// 0x274
			 0x8d86eeca, /// 0x278
			 0x5c976be8, /// 0x27c
			 0xd1c2e58c, /// 0x280
			 0x5f14f493, /// 0x284
			 0x42377011, /// 0x288
			 0x0b573e48, /// 0x28c
			 0x3d79f86a, /// 0x290
			 0x566e13f3, /// 0x294
			 0xa6400ed3, /// 0x298
			 0xe53b5631, /// 0x29c
			 0xfd735567, /// 0x2a0
			 0xa0f5ae53, /// 0x2a4
			 0x43bd87fb, /// 0x2a8
			 0x8772de96, /// 0x2ac
			 0x7a81b426, /// 0x2b0
			 0x1361e1cb, /// 0x2b4
			 0x91220a08, /// 0x2b8
			 0xeffaf770, /// 0x2bc
			 0x6427bed5, /// 0x2c0
			 0x0640a4cd, /// 0x2c4
			 0x9596dd16, /// 0x2c8
			 0x031ec710, /// 0x2cc
			 0x1ee27c11, /// 0x2d0
			 0x491d8aee, /// 0x2d4
			 0x4b853f4c, /// 0x2d8
			 0x8f36a578, /// 0x2dc
			 0x972651d4, /// 0x2e0
			 0xcae80695, /// 0x2e4
			 0xdb339121, /// 0x2e8
			 0x0f55ad19, /// 0x2ec
			 0xe037d224, /// 0x2f0
			 0x4e4cd320, /// 0x2f4
			 0x231777b9, /// 0x2f8
			 0x7b764526, /// 0x2fc
			 0x7c716859, /// 0x300
			 0xae91f76b, /// 0x304
			 0x5d181dc7, /// 0x308
			 0x35cc8def, /// 0x30c
			 0x3dccf9e7, /// 0x310
			 0xa20ab779, /// 0x314
			 0x1345b0a3, /// 0x318
			 0x0dd66acf, /// 0x31c
			 0x0c8f4310, /// 0x320
			 0x1d697b33, /// 0x324
			 0x327367a4, /// 0x328
			 0x3b87e776, /// 0x32c
			 0xa46ee029, /// 0x330
			 0xeef63b24, /// 0x334
			 0xae380036, /// 0x338
			 0x04040d18, /// 0x33c
			 0xf0c15cf7, /// 0x340
			 0x02ca50c6, /// 0x344
			 0x748156e2, /// 0x348
			 0xdfb7823d, /// 0x34c
			 0xf332a41f, /// 0x350
			 0x64dfb0a7, /// 0x354
			 0xc56fafe9, /// 0x358
			 0x05941d37, /// 0x35c
			 0xbdc6322c, /// 0x360
			 0x50992afd, /// 0x364
			 0x2e097da3, /// 0x368
			 0x5dedbafa, /// 0x36c
			 0x6eea1bc4, /// 0x370
			 0x4c465696, /// 0x374
			 0xf442d57d, /// 0x378
			 0x2155f8f0, /// 0x37c
			 0xcb8060fc, /// 0x380
			 0x4fc2f973, /// 0x384
			 0xb6c17641, /// 0x388
			 0x3c6cc7eb, /// 0x38c
			 0x59385342, /// 0x390
			 0x229c77bc, /// 0x394
			 0xd29c176f, /// 0x398
			 0x51e941dd, /// 0x39c
			 0x7748c2bd, /// 0x3a0
			 0xf9578a3a, /// 0x3a4
			 0x66c64332, /// 0x3a8
			 0x84b1cc29, /// 0x3ac
			 0x51ece899, /// 0x3b0
			 0x3d391084, /// 0x3b4
			 0x49ad6202, /// 0x3b8
			 0x9e2bf1e7, /// 0x3bc
			 0xe0f16b03, /// 0x3c0
			 0x6b258e4e, /// 0x3c4
			 0xad101622, /// 0x3c8
			 0xa3ef6c9b, /// 0x3cc
			 0xca6f5e41, /// 0x3d0
			 0x34848861, /// 0x3d4
			 0x739e5e81, /// 0x3d8
			 0x6fca8989, /// 0x3dc
			 0xcb76891d, /// 0x3e0
			 0xb4998181, /// 0x3e4
			 0x36895c89, /// 0x3e8
			 0x4ef876e2, /// 0x3ec
			 0x615914ed, /// 0x3f0
			 0xac86cab7, /// 0x3f4
			 0x3b901755, /// 0x3f8
			 0x39baec86, /// 0x3fc
			 0x340eb46a, /// 0x400
			 0x044bacad, /// 0x404
			 0x2bc0d5f5, /// 0x408
			 0x96b1fbb4, /// 0x40c
			 0x97e06eec, /// 0x410
			 0xbe62e41b, /// 0x414
			 0x11a5e6e5, /// 0x418
			 0xe3f20162, /// 0x41c
			 0x97b725db, /// 0x420
			 0x9518d0ca, /// 0x424
			 0x8ef26b83, /// 0x428
			 0x2416d805, /// 0x42c
			 0x9de2bad5, /// 0x430
			 0x631dfa72, /// 0x434
			 0xbc9083ca, /// 0x438
			 0x55652da8, /// 0x43c
			 0xb0693516, /// 0x440
			 0x9eab76e2, /// 0x444
			 0x556c2f3d, /// 0x448
			 0x13f5683f, /// 0x44c
			 0x755b0f09, /// 0x450
			 0x51f84a26, /// 0x454
			 0x054c471c, /// 0x458
			 0x026a62ca, /// 0x45c
			 0x64122317, /// 0x460
			 0x227c4780, /// 0x464
			 0x00459b24, /// 0x468
			 0x953fbac6, /// 0x46c
			 0xf24a0d8d, /// 0x470
			 0xe8efbba5, /// 0x474
			 0xe409c92f, /// 0x478
			 0xd96e82ad, /// 0x47c
			 0xb5139cce, /// 0x480
			 0x9d080b02, /// 0x484
			 0x30119afc, /// 0x488
			 0x47a0f748, /// 0x48c
			 0x4d52e81d, /// 0x490
			 0x45ca5584, /// 0x494
			 0xa7876eb3, /// 0x498
			 0xb4c1510f, /// 0x49c
			 0x1cbaa080, /// 0x4a0
			 0x6461fcc8, /// 0x4a4
			 0xe3f21abc, /// 0x4a8
			 0x89be4a84, /// 0x4ac
			 0x2d5cbe17, /// 0x4b0
			 0xbef46cf5, /// 0x4b4
			 0x8b399621, /// 0x4b8
			 0xee1416f4, /// 0x4bc
			 0x1c387a5c, /// 0x4c0
			 0xbaaa61cd, /// 0x4c4
			 0x0ac2cb32, /// 0x4c8
			 0x2c242933, /// 0x4cc
			 0x17e24daf, /// 0x4d0
			 0xea91aeea, /// 0x4d4
			 0xbd6755ec, /// 0x4d8
			 0x965f784e, /// 0x4dc
			 0xe2c2b33b, /// 0x4e0
			 0xe21a2126, /// 0x4e4
			 0x3682a074, /// 0x4e8
			 0x1f26c3c8, /// 0x4ec
			 0x3876857f, /// 0x4f0
			 0xbc60d8a8, /// 0x4f4
			 0x4d9f07df, /// 0x4f8
			 0xca9aee60, /// 0x4fc
			 0x56823b31, /// 0x500
			 0x053f5254, /// 0x504
			 0x0e7f7e01, /// 0x508
			 0x52ea71fb, /// 0x50c
			 0x22e97cda, /// 0x510
			 0xe138e0c4, /// 0x514
			 0x660cad23, /// 0x518
			 0x663a19bf, /// 0x51c
			 0xfda6e679, /// 0x520
			 0xefd86a0a, /// 0x524
			 0xf9e9eb8c, /// 0x528
			 0x94e1d173, /// 0x52c
			 0xc6906004, /// 0x530
			 0x22937e6f, /// 0x534
			 0x08dcb2db, /// 0x538
			 0xc293c3fc, /// 0x53c
			 0x7e8cb9fc, /// 0x540
			 0x32838387, /// 0x544
			 0xb85f7fdb, /// 0x548
			 0x465dcee5, /// 0x54c
			 0x9e708194, /// 0x550
			 0x1f55f028, /// 0x554
			 0x685c678e, /// 0x558
			 0xba385f5b, /// 0x55c
			 0x5633b73e, /// 0x560
			 0x23d2f47f, /// 0x564
			 0x59800a10, /// 0x568
			 0x5cdf08b4, /// 0x56c
			 0x6e1b494d, /// 0x570
			 0xad1b9b33, /// 0x574
			 0xb98b8ef6, /// 0x578
			 0x79db5833, /// 0x57c
			 0x437f9304, /// 0x580
			 0x9e06790c, /// 0x584
			 0xcd692164, /// 0x588
			 0x97358843, /// 0x58c
			 0x794f9051, /// 0x590
			 0x0cbcacc2, /// 0x594
			 0x95115af2, /// 0x598
			 0x6e43ce4c, /// 0x59c
			 0xff40545c, /// 0x5a0
			 0x1da6c05d, /// 0x5a4
			 0x3ce0ff27, /// 0x5a8
			 0x82941d19, /// 0x5ac
			 0xcdd976b5, /// 0x5b0
			 0x7ed1d9fc, /// 0x5b4
			 0x478234eb, /// 0x5b8
			 0x93aa49f3, /// 0x5bc
			 0x5c12d242, /// 0x5c0
			 0x4c1dfd3b, /// 0x5c4
			 0x28458002, /// 0x5c8
			 0xcb72f8e2, /// 0x5cc
			 0x78444ce6, /// 0x5d0
			 0xfcb4ea17, /// 0x5d4
			 0x3fd2ccba, /// 0x5d8
			 0xd560636b, /// 0x5dc
			 0x5971cd6d, /// 0x5e0
			 0xb93837e7, /// 0x5e4
			 0x3d20d9ec, /// 0x5e8
			 0x7a806cd7, /// 0x5ec
			 0x8d556596, /// 0x5f0
			 0xd6d0f435, /// 0x5f4
			 0x1ef76c70, /// 0x5f8
			 0x8fe311ef, /// 0x5fc
			 0xdd9a3c8c, /// 0x600
			 0x5b625f8e, /// 0x604
			 0x0cdae02e, /// 0x608
			 0x23f2729f, /// 0x60c
			 0xb78daa1f, /// 0x610
			 0x984d9729, /// 0x614
			 0x05d0cb2f, /// 0x618
			 0xf7060a8b, /// 0x61c
			 0x96ca5e31, /// 0x620
			 0x7e08fc39, /// 0x624
			 0x80769ac0, /// 0x628
			 0x44408df9, /// 0x62c
			 0x72f1c0e4, /// 0x630
			 0x8e0e6d43, /// 0x634
			 0x96e88250, /// 0x638
			 0x15df88f5, /// 0x63c
			 0xf4e4a6ef, /// 0x640
			 0xf44e895b, /// 0x644
			 0xe3a80799, /// 0x648
			 0x731be50d, /// 0x64c
			 0x9110ab3c, /// 0x650
			 0x058c9406, /// 0x654
			 0xd8ea53a7, /// 0x658
			 0xa86def14, /// 0x65c
			 0x44339196, /// 0x660
			 0xefa2c82a, /// 0x664
			 0x9ece64d6, /// 0x668
			 0x86c71ccf, /// 0x66c
			 0x98606e16, /// 0x670
			 0xb53c726b, /// 0x674
			 0x59a0c2bd, /// 0x678
			 0x4889a768, /// 0x67c
			 0xbbc483f9, /// 0x680
			 0x13e54aba, /// 0x684
			 0x4b1c3350, /// 0x688
			 0x4f6ccb24, /// 0x68c
			 0x06de14c1, /// 0x690
			 0x5f013e69, /// 0x694
			 0x02d7422e, /// 0x698
			 0xebbffcb4, /// 0x69c
			 0xba9da188, /// 0x6a0
			 0x47559917, /// 0x6a4
			 0x3923786f, /// 0x6a8
			 0x26be50c7, /// 0x6ac
			 0xae464dd2, /// 0x6b0
			 0x1192a7f4, /// 0x6b4
			 0x90505541, /// 0x6b8
			 0x394537ac, /// 0x6bc
			 0x6b247e89, /// 0x6c0
			 0x0b983dea, /// 0x6c4
			 0xcbcf8f41, /// 0x6c8
			 0xefecb7ac, /// 0x6cc
			 0x9a881d7f, /// 0x6d0
			 0x05f130a5, /// 0x6d4
			 0x13649133, /// 0x6d8
			 0x772a18d9, /// 0x6dc
			 0xc61b5e92, /// 0x6e0
			 0x1e50b5b9, /// 0x6e4
			 0x10fc4c6e, /// 0x6e8
			 0xae07c2a3, /// 0x6ec
			 0xfea535d0, /// 0x6f0
			 0x1e0042f6, /// 0x6f4
			 0x664ad96e, /// 0x6f8
			 0xe4453dd2, /// 0x6fc
			 0x1723eed0, /// 0x700
			 0x2018daea, /// 0x704
			 0x11518764, /// 0x708
			 0x8f64ff77, /// 0x70c
			 0xd2653cfa, /// 0x710
			 0xbc06a4bb, /// 0x714
			 0xe6a751eb, /// 0x718
			 0x62e65e5f, /// 0x71c
			 0x7127011a, /// 0x720
			 0xf81c4aa2, /// 0x724
			 0xa654d628, /// 0x728
			 0x27bc29ee, /// 0x72c
			 0x12b6c370, /// 0x730
			 0x712ccba6, /// 0x734
			 0x1d859773, /// 0x738
			 0xb90cfc7c, /// 0x73c
			 0x51935166, /// 0x740
			 0x04825f19, /// 0x744
			 0x1743a371, /// 0x748
			 0x575a775a, /// 0x74c
			 0x21c38f35, /// 0x750
			 0xfe9671d4, /// 0x754
			 0xd4ec7912, /// 0x758
			 0x65289045, /// 0x75c
			 0xd478cc36, /// 0x760
			 0xdf8630dd, /// 0x764
			 0xd55490a3, /// 0x768
			 0xfd4fe85e, /// 0x76c
			 0x9bfef6a4, /// 0x770
			 0x585aac5a, /// 0x774
			 0xfd8227a3, /// 0x778
			 0xb3a7d284, /// 0x77c
			 0xe9275722, /// 0x780
			 0xf2bd9166, /// 0x784
			 0xd4f12904, /// 0x788
			 0x171f6e80, /// 0x78c
			 0x83415258, /// 0x790
			 0x7200ab78, /// 0x794
			 0x0e76bdd4, /// 0x798
			 0x2ac8507d, /// 0x79c
			 0x5ac60196, /// 0x7a0
			 0x55f6143a, /// 0x7a4
			 0x6adafbc0, /// 0x7a8
			 0x3985aede, /// 0x7ac
			 0x0a9cd052, /// 0x7b0
			 0xa02f43e1, /// 0x7b4
			 0x053f4cba, /// 0x7b8
			 0x4764fc5d, /// 0x7bc
			 0xb41bc4f2, /// 0x7c0
			 0xc5e045df, /// 0x7c4
			 0xde4cc101, /// 0x7c8
			 0x045d5a4c, /// 0x7cc
			 0xc87b1ed4, /// 0x7d0
			 0xaac9b947, /// 0x7d4
			 0x94dacb93, /// 0x7d8
			 0x30fcdccd, /// 0x7dc
			 0xd0050ce2, /// 0x7e0
			 0xbc3b499b, /// 0x7e4
			 0x67b2ddf9, /// 0x7e8
			 0x6a5d3755, /// 0x7ec
			 0x1ca08693, /// 0x7f0
			 0xe565c317, /// 0x7f4
			 0xee39e678, /// 0x7f8
			 0x03733962, /// 0x7fc
			 0xf65f8415, /// 0x800
			 0x358a866d, /// 0x804
			 0xdd147f6e, /// 0x808
			 0xa20b4ccf, /// 0x80c
			 0xcbc7ccaa, /// 0x810
			 0xa817dda7, /// 0x814
			 0x43cb708c, /// 0x818
			 0x54f39c37, /// 0x81c
			 0xdf7515c0, /// 0x820
			 0x3dbcd743, /// 0x824
			 0xa7fa3bf9, /// 0x828
			 0xe2ad4f6d, /// 0x82c
			 0x9116a834, /// 0x830
			 0xdcb883a5, /// 0x834
			 0xb7dc3ed6, /// 0x838
			 0xea069967, /// 0x83c
			 0x891f1be2, /// 0x840
			 0x5833f451, /// 0x844
			 0xcf790f17, /// 0x848
			 0xfbd51321, /// 0x84c
			 0x966bda96, /// 0x850
			 0xef13134b, /// 0x854
			 0x0b606720, /// 0x858
			 0xccdef73c, /// 0x85c
			 0xee0a3001, /// 0x860
			 0xad821897, /// 0x864
			 0xc8fdd4f8, /// 0x868
			 0x526f8e30, /// 0x86c
			 0x074fce97, /// 0x870
			 0xa278fe59, /// 0x874
			 0x3ec41de1, /// 0x878
			 0xc29a06d7, /// 0x87c
			 0x2915dddb, /// 0x880
			 0x87999b6c, /// 0x884
			 0xf5b18b1c, /// 0x888
			 0xfab476ea, /// 0x88c
			 0x884ba473, /// 0x890
			 0x47feb6aa, /// 0x894
			 0x229cd4da, /// 0x898
			 0x5c929f19, /// 0x89c
			 0x7656b0c0, /// 0x8a0
			 0xe683b985, /// 0x8a4
			 0xab49c41e, /// 0x8a8
			 0xd2e45407, /// 0x8ac
			 0x0c1a3257, /// 0x8b0
			 0x064fd2b1, /// 0x8b4
			 0x0118a550, /// 0x8b8
			 0xe205b2a0, /// 0x8bc
			 0x7fa930fa, /// 0x8c0
			 0x43b8cabe, /// 0x8c4
			 0x7ece35e7, /// 0x8c8
			 0xd4922022, /// 0x8cc
			 0x3499a152, /// 0x8d0
			 0x23994b12, /// 0x8d4
			 0xb8833c77, /// 0x8d8
			 0x41b53a08, /// 0x8dc
			 0x64847bb2, /// 0x8e0
			 0x81099a65, /// 0x8e4
			 0xb721ea09, /// 0x8e8
			 0x008a395e, /// 0x8ec
			 0x4cc80b2b, /// 0x8f0
			 0xaee0f845, /// 0x8f4
			 0x46841fdf, /// 0x8f8
			 0x77c88c10, /// 0x8fc
			 0x4d7cd40c, /// 0x900
			 0xd84bf6cd, /// 0x904
			 0x81d6838f, /// 0x908
			 0xa40fc477, /// 0x90c
			 0x64aa0259, /// 0x910
			 0x34160209, /// 0x914
			 0xbf0afcb8, /// 0x918
			 0x508d6a2d, /// 0x91c
			 0xc18fa943, /// 0x920
			 0xceda5f50, /// 0x924
			 0x72821b6e, /// 0x928
			 0x235458ba, /// 0x92c
			 0x6bb35f9d, /// 0x930
			 0xf63ea88a, /// 0x934
			 0xc13fb1d0, /// 0x938
			 0x34e1fa16, /// 0x93c
			 0x3b8435ac, /// 0x940
			 0xd34e3e79, /// 0x944
			 0x0a11c009, /// 0x948
			 0x6dc536a8, /// 0x94c
			 0x5d17475c, /// 0x950
			 0x64b38dbb, /// 0x954
			 0x18af228f, /// 0x958
			 0x83bdcc36, /// 0x95c
			 0x649015d8, /// 0x960
			 0xfc1b02f0, /// 0x964
			 0xb3a9d978, /// 0x968
			 0x1974c697, /// 0x96c
			 0x242d42dc, /// 0x970
			 0xda3f538d, /// 0x974
			 0x88efeec9, /// 0x978
			 0x82fc6a90, /// 0x97c
			 0x81ba571a, /// 0x980
			 0x4090cb44, /// 0x984
			 0x1d8f5b63, /// 0x988
			 0x26a3834b, /// 0x98c
			 0x9cea822f, /// 0x990
			 0x1adf59bd, /// 0x994
			 0xb5c3d654, /// 0x998
			 0xca2cae5c, /// 0x99c
			 0x8c7422f3, /// 0x9a0
			 0x796ca791, /// 0x9a4
			 0x4563f14a, /// 0x9a8
			 0xa225ba11, /// 0x9ac
			 0x4cbbc1b3, /// 0x9b0
			 0x932a6679, /// 0x9b4
			 0x5ee8f445, /// 0x9b8
			 0xe49e9bb4, /// 0x9bc
			 0x23b16a97, /// 0x9c0
			 0x37b40612, /// 0x9c4
			 0xb5c62abb, /// 0x9c8
			 0xaf7bc7eb, /// 0x9cc
			 0x704edcb6, /// 0x9d0
			 0x0802e87b, /// 0x9d4
			 0xa211cd41, /// 0x9d8
			 0x38ec4a42, /// 0x9dc
			 0x88f87e27, /// 0x9e0
			 0xc65dd6a0, /// 0x9e4
			 0xc7973e56, /// 0x9e8
			 0xf7dd2373, /// 0x9ec
			 0xa41ff646, /// 0x9f0
			 0xd1cf9575, /// 0x9f4
			 0x61f21097, /// 0x9f8
			 0x7056d2ba, /// 0x9fc
			 0x63f55b4d, /// 0xa00
			 0x492c93d7, /// 0xa04
			 0x1147abfb, /// 0xa08
			 0x32ebdd6c, /// 0xa0c
			 0xb2ab416d, /// 0xa10
			 0xc1dd1e41, /// 0xa14
			 0x54b666f7, /// 0xa18
			 0x1a89d08c, /// 0xa1c
			 0x069c1052, /// 0xa20
			 0xc6fef1ed, /// 0xa24
			 0x65d4b4c1, /// 0xa28
			 0x2abcb9c0, /// 0xa2c
			 0x43ad2759, /// 0xa30
			 0x271c33ae, /// 0xa34
			 0xf28e0b22, /// 0xa38
			 0x788638ef, /// 0xa3c
			 0xaf28a0c8, /// 0xa40
			 0x6c4fb9c8, /// 0xa44
			 0x4e19ba32, /// 0xa48
			 0x30d10483, /// 0xa4c
			 0x862efc40, /// 0xa50
			 0xc2c747b1, /// 0xa54
			 0xbfe552a2, /// 0xa58
			 0x3d2cdf95, /// 0xa5c
			 0x42e5df73, /// 0xa60
			 0x47acedf5, /// 0xa64
			 0xf78d805a, /// 0xa68
			 0x4ccba315, /// 0xa6c
			 0xf6db285d, /// 0xa70
			 0x03e5d0d8, /// 0xa74
			 0xfe9769d4, /// 0xa78
			 0x9470f51f, /// 0xa7c
			 0x088f5aff, /// 0xa80
			 0x519a8454, /// 0xa84
			 0xf41e2235, /// 0xa88
			 0x8b58878e, /// 0xa8c
			 0xdc1bbc40, /// 0xa90
			 0xde6667d3, /// 0xa94
			 0xae619ff0, /// 0xa98
			 0x58b66818, /// 0xa9c
			 0x10e4989c, /// 0xaa0
			 0x52f315cc, /// 0xaa4
			 0xe5ff5f26, /// 0xaa8
			 0x586cf4b6, /// 0xaac
			 0xe74f4fc7, /// 0xab0
			 0xa0e72732, /// 0xab4
			 0x8fdfe144, /// 0xab8
			 0x0ee47b0b, /// 0xabc
			 0xbfa76884, /// 0xac0
			 0x32a51ed5, /// 0xac4
			 0x35ddb2be, /// 0xac8
			 0xe4746c92, /// 0xacc
			 0xf683f0b0, /// 0xad0
			 0xfc47d157, /// 0xad4
			 0xa6ec1e1e, /// 0xad8
			 0x80bea80e, /// 0xadc
			 0x6815b50e, /// 0xae0
			 0xa20f7b62, /// 0xae4
			 0xc6588a91, /// 0xae8
			 0xd52f7596, /// 0xaec
			 0x47266db6, /// 0xaf0
			 0xbf9a3236, /// 0xaf4
			 0x15aca90e, /// 0xaf8
			 0x7fb17ff2, /// 0xafc
			 0x2321b508, /// 0xb00
			 0x7fd64c35, /// 0xb04
			 0xd7bf9860, /// 0xb08
			 0x487c6bc2, /// 0xb0c
			 0xfb2509b2, /// 0xb10
			 0x973ce9bd, /// 0xb14
			 0x65b682ec, /// 0xb18
			 0x4c9c090a, /// 0xb1c
			 0x22510b25, /// 0xb20
			 0x13fd47bb, /// 0xb24
			 0xf4cb738e, /// 0xb28
			 0xf71bd02d, /// 0xb2c
			 0x3e0d51ec, /// 0xb30
			 0x248c5a7f, /// 0xb34
			 0x9373251b, /// 0xb38
			 0x6647776f, /// 0xb3c
			 0xdd26a29b, /// 0xb40
			 0x43ed489e, /// 0xb44
			 0xda247d08, /// 0xb48
			 0x40b3a17a, /// 0xb4c
			 0x9bf0a64f, /// 0xb50
			 0x7b4f1025, /// 0xb54
			 0x0a2d2a82, /// 0xb58
			 0xc17697c8, /// 0xb5c
			 0x7248b0fb, /// 0xb60
			 0x2522bf9c, /// 0xb64
			 0x2590c150, /// 0xb68
			 0x785f752e, /// 0xb6c
			 0xa19eebb4, /// 0xb70
			 0x582e40df, /// 0xb74
			 0xe8a0ab3b, /// 0xb78
			 0xd9a2cd1b, /// 0xb7c
			 0x853f0ebd, /// 0xb80
			 0x3b2ccc4b, /// 0xb84
			 0x572c1fa8, /// 0xb88
			 0xba8ad3e0, /// 0xb8c
			 0x9e46caf9, /// 0xb90
			 0x139d332a, /// 0xb94
			 0xcf33d132, /// 0xb98
			 0x262fd251, /// 0xb9c
			 0x6fec5660, /// 0xba0
			 0x7034eed3, /// 0xba4
			 0xed95ee51, /// 0xba8
			 0x6624ead0, /// 0xbac
			 0x28e8faf5, /// 0xbb0
			 0xadd415f9, /// 0xbb4
			 0xf178de98, /// 0xbb8
			 0x1f0319e0, /// 0xbbc
			 0x08c3379f, /// 0xbc0
			 0x99d5fb3e, /// 0xbc4
			 0x90c7b9b1, /// 0xbc8
			 0x74777dc6, /// 0xbcc
			 0x855d7877, /// 0xbd0
			 0xdca3e105, /// 0xbd4
			 0x04f012e4, /// 0xbd8
			 0x16a445a5, /// 0xbdc
			 0x4e0c17a0, /// 0xbe0
			 0x0e54e2e5, /// 0xbe4
			 0x01b66aa3, /// 0xbe8
			 0xd072d71d, /// 0xbec
			 0xe7655db2, /// 0xbf0
			 0xa899fe49, /// 0xbf4
			 0xb4122a42, /// 0xbf8
			 0xd8944288, /// 0xbfc
			 0x7ba4772a, /// 0xc00
			 0xcd84e15e, /// 0xc04
			 0x689c2da7, /// 0xc08
			 0xa80fc382, /// 0xc0c
			 0xf0c3dbda, /// 0xc10
			 0x24ebeb3a, /// 0xc14
			 0xe5bd4586, /// 0xc18
			 0x2d96d8d7, /// 0xc1c
			 0xe7ddc671, /// 0xc20
			 0x24374b4b, /// 0xc24
			 0x4e3f19ef, /// 0xc28
			 0xf1112ade, /// 0xc2c
			 0xce7288fb, /// 0xc30
			 0xfff43ab4, /// 0xc34
			 0xa50c49b3, /// 0xc38
			 0xf6c6722b, /// 0xc3c
			 0x74d50cd2, /// 0xc40
			 0x76d203db, /// 0xc44
			 0x75c66b13, /// 0xc48
			 0x03b0297d, /// 0xc4c
			 0x701736b7, /// 0xc50
			 0xdb87a6b9, /// 0xc54
			 0x87df6a88, /// 0xc58
			 0xb2f48151, /// 0xc5c
			 0x9a1c06a1, /// 0xc60
			 0x67693a48, /// 0xc64
			 0xdffe4a15, /// 0xc68
			 0x3c0f1d5a, /// 0xc6c
			 0xee2f5dea, /// 0xc70
			 0x6d29bffc, /// 0xc74
			 0xcc4a01a2, /// 0xc78
			 0xe87f8c2a, /// 0xc7c
			 0x6288221d, /// 0xc80
			 0xecdb10fe, /// 0xc84
			 0xce50c070, /// 0xc88
			 0x14be0921, /// 0xc8c
			 0x47ffdb15, /// 0xc90
			 0x6cffe27d, /// 0xc94
			 0x4f33c0cd, /// 0xc98
			 0xe2e9e6c7, /// 0xc9c
			 0x115c3188, /// 0xca0
			 0xf7bd88bc, /// 0xca4
			 0xd12b31b9, /// 0xca8
			 0xdbc9a2a9, /// 0xcac
			 0x145af731, /// 0xcb0
			 0x801da4e2, /// 0xcb4
			 0x11308f8e, /// 0xcb8
			 0x5c1d5127, /// 0xcbc
			 0x5c4b772d, /// 0xcc0
			 0x38ddc7d9, /// 0xcc4
			 0x4b6b825a, /// 0xcc8
			 0x3961d64b, /// 0xccc
			 0x15bd61bd, /// 0xcd0
			 0x699874a1, /// 0xcd4
			 0x5529af9a, /// 0xcd8
			 0x39a525ba, /// 0xcdc
			 0x0722e147, /// 0xce0
			 0x0abf2a94, /// 0xce4
			 0x886362cd, /// 0xce8
			 0x9260d04a, /// 0xcec
			 0x57f56158, /// 0xcf0
			 0x8804ece6, /// 0xcf4
			 0xb00673d7, /// 0xcf8
			 0x58f55994, /// 0xcfc
			 0x03f01e84, /// 0xd00
			 0x656322fd, /// 0xd04
			 0xc46b7627, /// 0xd08
			 0x2f7075cd, /// 0xd0c
			 0x33c1d0e4, /// 0xd10
			 0x060ef296, /// 0xd14
			 0xbeff26ec, /// 0xd18
			 0x953bd4f0, /// 0xd1c
			 0x3bd37dc4, /// 0xd20
			 0x8661a8f0, /// 0xd24
			 0x9dab0a68, /// 0xd28
			 0x18420862, /// 0xd2c
			 0x452c6dcf, /// 0xd30
			 0xd72a409d, /// 0xd34
			 0x3e0bfe44, /// 0xd38
			 0xd1d5de1a, /// 0xd3c
			 0x771251a0, /// 0xd40
			 0x3ac86377, /// 0xd44
			 0xb164afee, /// 0xd48
			 0x15d363f3, /// 0xd4c
			 0x42a43fb1, /// 0xd50
			 0xeb572b51, /// 0xd54
			 0x39618116, /// 0xd58
			 0x033920b4, /// 0xd5c
			 0xf9ad10d7, /// 0xd60
			 0x3da9743d, /// 0xd64
			 0x11e83911, /// 0xd68
			 0xc9de93ea, /// 0xd6c
			 0xf2afaf12, /// 0xd70
			 0xff8faed5, /// 0xd74
			 0x1c6d1865, /// 0xd78
			 0x39382ecd, /// 0xd7c
			 0x7139f9b2, /// 0xd80
			 0xb4469456, /// 0xd84
			 0x68f8982b, /// 0xd88
			 0x8b099c25, /// 0xd8c
			 0xca4d2cb2, /// 0xd90
			 0x164b0de3, /// 0xd94
			 0xfb0c4400, /// 0xd98
			 0x99fad071, /// 0xd9c
			 0xc16a21e7, /// 0xda0
			 0x77f91d23, /// 0xda4
			 0x7417a7be, /// 0xda8
			 0xae3ad9a4, /// 0xdac
			 0xf9fdeccb, /// 0xdb0
			 0x7f514375, /// 0xdb4
			 0xa0c97dd2, /// 0xdb8
			 0x8140182f, /// 0xdbc
			 0x34a258fb, /// 0xdc0
			 0x2addf5f0, /// 0xdc4
			 0x9e1bc50f, /// 0xdc8
			 0x29151291, /// 0xdcc
			 0x3af96dd6, /// 0xdd0
			 0xe1080aec, /// 0xdd4
			 0xb87a1dc2, /// 0xdd8
			 0x7cd914a2, /// 0xddc
			 0x7bb87b1e, /// 0xde0
			 0x2b846726, /// 0xde4
			 0x9a433e59, /// 0xde8
			 0x4c5b3268, /// 0xdec
			 0x61fc4c35, /// 0xdf0
			 0x6699f9b8, /// 0xdf4
			 0x52350b89, /// 0xdf8
			 0x93474a92, /// 0xdfc
			 0x896edcb6, /// 0xe00
			 0xc94878a6, /// 0xe04
			 0xf530a373, /// 0xe08
			 0xfa520042, /// 0xe0c
			 0x56628633, /// 0xe10
			 0xc8f56d27, /// 0xe14
			 0x7b11f993, /// 0xe18
			 0xe19f528a, /// 0xe1c
			 0xd3ec3343, /// 0xe20
			 0x8e6e0592, /// 0xe24
			 0x56f44843, /// 0xe28
			 0xea5caec1, /// 0xe2c
			 0xd33f8a6e, /// 0xe30
			 0x5fb86f0e, /// 0xe34
			 0x647ef4c4, /// 0xe38
			 0x27ebd41f, /// 0xe3c
			 0xadfaeba9, /// 0xe40
			 0x16f6a992, /// 0xe44
			 0x519859b1, /// 0xe48
			 0xe1380171, /// 0xe4c
			 0x4cfe2f9e, /// 0xe50
			 0xffbb358b, /// 0xe54
			 0x125dc103, /// 0xe58
			 0x4046e27b, /// 0xe5c
			 0xb2657660, /// 0xe60
			 0x95846664, /// 0xe64
			 0xf6642fbe, /// 0xe68
			 0xd54f209a, /// 0xe6c
			 0xe198b74e, /// 0xe70
			 0xa96dd8fd, /// 0xe74
			 0xa55feef5, /// 0xe78
			 0x276b3509, /// 0xe7c
			 0xfbd8912d, /// 0xe80
			 0xa621eb75, /// 0xe84
			 0x5816130c, /// 0xe88
			 0x72d42edf, /// 0xe8c
			 0xdedee6a9, /// 0xe90
			 0xcaa3c034, /// 0xe94
			 0x4d8ba329, /// 0xe98
			 0x833d796b, /// 0xe9c
			 0x82c0ff5c, /// 0xea0
			 0xd75234ba, /// 0xea4
			 0x328bc06a, /// 0xea8
			 0xf53f956c, /// 0xeac
			 0x8589091f, /// 0xeb0
			 0xbd628240, /// 0xeb4
			 0x48493305, /// 0xeb8
			 0x5e77ca40, /// 0xebc
			 0x3cb92c9d, /// 0xec0
			 0x6985e108, /// 0xec4
			 0x905a9b69, /// 0xec8
			 0x292e96b5, /// 0xecc
			 0x46c3284b, /// 0xed0
			 0xd41b9d9f, /// 0xed4
			 0x2ea6694c, /// 0xed8
			 0x7f36a76d, /// 0xedc
			 0xe95a1b2a, /// 0xee0
			 0x6b3981a1, /// 0xee4
			 0xa0b732ec, /// 0xee8
			 0x1f374523, /// 0xeec
			 0x0eb4367c, /// 0xef0
			 0x0e51f797, /// 0xef4
			 0x52cb0782, /// 0xef8
			 0xbaa1a2ff, /// 0xefc
			 0x96d047b5, /// 0xf00
			 0x2db318da, /// 0xf04
			 0xa05b4830, /// 0xf08
			 0xddc5d14f, /// 0xf0c
			 0x9e47031b, /// 0xf10
			 0xf871cdda, /// 0xf14
			 0xa9e48df0, /// 0xf18
			 0xd416a232, /// 0xf1c
			 0x4593d7cf, /// 0xf20
			 0x11620466, /// 0xf24
			 0x4357f6e2, /// 0xf28
			 0x533389df, /// 0xf2c
			 0x2bf10c61, /// 0xf30
			 0x531dcf5f, /// 0xf34
			 0x93a2bf88, /// 0xf38
			 0x4327cc58, /// 0xf3c
			 0x5485d008, /// 0xf40
			 0xfa5f1027, /// 0xf44
			 0x1027c7b6, /// 0xf48
			 0xb97894bc, /// 0xf4c
			 0xd9178063, /// 0xf50
			 0x7f479413, /// 0xf54
			 0x7d892653, /// 0xf58
			 0x1fa54eb5, /// 0xf5c
			 0xd106ed15, /// 0xf60
			 0x609182cb, /// 0xf64
			 0x95e85e88, /// 0xf68
			 0x32cc551b, /// 0xf6c
			 0xfad0cc5d, /// 0xf70
			 0xfed00263, /// 0xf74
			 0x6f5fbec0, /// 0xf78
			 0xc8cbb380, /// 0xf7c
			 0xdcba8f64, /// 0xf80
			 0x438426e1, /// 0xf84
			 0x6cee41ab, /// 0xf88
			 0x1dc4ce8b, /// 0xf8c
			 0xc4a095e2, /// 0xf90
			 0xad49d711, /// 0xf94
			 0x0eb2118c, /// 0xf98
			 0x3311621c, /// 0xf9c
			 0xdd4d9983, /// 0xfa0
			 0x9d907110, /// 0xfa4
			 0x3197dac9, /// 0xfa8
			 0xb4f385f5, /// 0xfac
			 0x9f277fad, /// 0xfb0
			 0xaec2adb5, /// 0xfb4
			 0x7b28034a, /// 0xfb8
			 0x2fe5c2bb, /// 0xfbc
			 0x9eabc87f, /// 0xfc0
			 0x8075483f, /// 0xfc4
			 0xfe4d160e, /// 0xfc8
			 0x6fe79d12, /// 0xfcc
			 0x0d3e01d6, /// 0xfd0
			 0x992180e7, /// 0xfd4
			 0x9430ae94, /// 0xfd8
			 0xa569326a, /// 0xfdc
			 0x3f2a4979, /// 0xfe0
			 0x9a2884ec, /// 0xfe4
			 0x3d0db007, /// 0xfe8
			 0xc38fb50d, /// 0xfec
			 0xe99278cd, /// 0xff0
			 0xed8e3e70, /// 0xff4
			 0x8cf2bfa5, /// 0xff8
			 0x7874403b /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0512507a, /// 0x0
			 0x081c4bbc, /// 0x4
			 0x7a3badee, /// 0x8
			 0xb68fc886, /// 0xc
			 0xb07569b5, /// 0x10
			 0xa0729cd7, /// 0x14
			 0x76e9d6fa, /// 0x18
			 0x28ecb8e0, /// 0x1c
			 0xa1e6cf21, /// 0x20
			 0xfb096447, /// 0x24
			 0x580892fd, /// 0x28
			 0x4d333bca, /// 0x2c
			 0x1825ef97, /// 0x30
			 0xcd765bb5, /// 0x34
			 0x28789e9f, /// 0x38
			 0x54a2c6f3, /// 0x3c
			 0xace2258b, /// 0x40
			 0xf2483bfc, /// 0x44
			 0xc7c00785, /// 0x48
			 0x2f932015, /// 0x4c
			 0xe61e8251, /// 0x50
			 0xfff6ff16, /// 0x54
			 0x1ff229ec, /// 0x58
			 0x76e5822b, /// 0x5c
			 0x041caac2, /// 0x60
			 0xdeb11c9a, /// 0x64
			 0xe49f0fdc, /// 0x68
			 0xc507b2c6, /// 0x6c
			 0x0149807f, /// 0x70
			 0xffa9a67c, /// 0x74
			 0xa1fba301, /// 0x78
			 0xb573ae1e, /// 0x7c
			 0x5837ccab, /// 0x80
			 0x41ab32a1, /// 0x84
			 0xafeaf73e, /// 0x88
			 0xc902334f, /// 0x8c
			 0xec5164a4, /// 0x90
			 0xbccf773b, /// 0x94
			 0xef50adeb, /// 0x98
			 0xd00e8bac, /// 0x9c
			 0x428bb65f, /// 0xa0
			 0x1eee6c32, /// 0xa4
			 0x22806924, /// 0xa8
			 0xe35275a4, /// 0xac
			 0x501cbcd6, /// 0xb0
			 0x73d657be, /// 0xb4
			 0x4b58f5f2, /// 0xb8
			 0xefef57dd, /// 0xbc
			 0x23d88ce2, /// 0xc0
			 0x7e0ce7aa, /// 0xc4
			 0xcf561101, /// 0xc8
			 0xebe30fac, /// 0xcc
			 0xe6ed86c7, /// 0xd0
			 0xe8e2cf3f, /// 0xd4
			 0x55a0e6f5, /// 0xd8
			 0xa9ef2b6d, /// 0xdc
			 0x7057cc37, /// 0xe0
			 0xcca9a5e6, /// 0xe4
			 0x825a8ad4, /// 0xe8
			 0x3d9b708a, /// 0xec
			 0x5385944d, /// 0xf0
			 0xe57b4cf6, /// 0xf4
			 0x3d7c6eca, /// 0xf8
			 0xdb7192d0, /// 0xfc
			 0xf0cef073, /// 0x100
			 0xa4c3b4af, /// 0x104
			 0xc1a8ab31, /// 0x108
			 0x43fe7374, /// 0x10c
			 0x255b9c83, /// 0x110
			 0x6e7afe80, /// 0x114
			 0xfc5e78f8, /// 0x118
			 0xbd526da8, /// 0x11c
			 0x3915851e, /// 0x120
			 0x7113b4c9, /// 0x124
			 0xa897a2a5, /// 0x128
			 0xb8d7c304, /// 0x12c
			 0xf374d521, /// 0x130
			 0x55df463e, /// 0x134
			 0x1f2d16c7, /// 0x138
			 0xdeb74e64, /// 0x13c
			 0xb02e471b, /// 0x140
			 0xb28518e1, /// 0x144
			 0x0e4a03e1, /// 0x148
			 0xf6714956, /// 0x14c
			 0xc77a70da, /// 0x150
			 0xa9fd183d, /// 0x154
			 0x5dcb91a1, /// 0x158
			 0x378b4f06, /// 0x15c
			 0x7c83e349, /// 0x160
			 0xcb9025c2, /// 0x164
			 0x711bfdb2, /// 0x168
			 0x565b6439, /// 0x16c
			 0x3497bc5f, /// 0x170
			 0x7a4397dd, /// 0x174
			 0x44f67f0b, /// 0x178
			 0x4513bfda, /// 0x17c
			 0x2af6aa56, /// 0x180
			 0xbb1ceb1c, /// 0x184
			 0xf8aa557b, /// 0x188
			 0xcfbbe1cb, /// 0x18c
			 0xbbc12249, /// 0x190
			 0x8d7a3e30, /// 0x194
			 0x236fcf6b, /// 0x198
			 0xa127ddb7, /// 0x19c
			 0x6f16c266, /// 0x1a0
			 0xd8d50908, /// 0x1a4
			 0x03482b30, /// 0x1a8
			 0xf5a35cd2, /// 0x1ac
			 0xec173b33, /// 0x1b0
			 0xc64e0a41, /// 0x1b4
			 0xfc919a9c, /// 0x1b8
			 0xa4bdea7e, /// 0x1bc
			 0x224c9ec0, /// 0x1c0
			 0xaafeda8b, /// 0x1c4
			 0x589ee397, /// 0x1c8
			 0x7002bcc3, /// 0x1cc
			 0x75cebf82, /// 0x1d0
			 0x27056622, /// 0x1d4
			 0xb9109977, /// 0x1d8
			 0x9909045e, /// 0x1dc
			 0x4b441fa2, /// 0x1e0
			 0x66a22988, /// 0x1e4
			 0x4d18740b, /// 0x1e8
			 0x5935826c, /// 0x1ec
			 0x62da9441, /// 0x1f0
			 0x794a03a3, /// 0x1f4
			 0x1a430387, /// 0x1f8
			 0x4a2da743, /// 0x1fc
			 0x6abe35c7, /// 0x200
			 0x50221a54, /// 0x204
			 0x7a8d5860, /// 0x208
			 0xa3571b1f, /// 0x20c
			 0xd73859c0, /// 0x210
			 0xfe25eb79, /// 0x214
			 0xf3a37575, /// 0x218
			 0xc1479a82, /// 0x21c
			 0x3f988f79, /// 0x220
			 0xfbb9fcb3, /// 0x224
			 0xdaf4eafc, /// 0x228
			 0xe5656cab, /// 0x22c
			 0xe0b58194, /// 0x230
			 0xa925bed8, /// 0x234
			 0xd0ea8549, /// 0x238
			 0xec27460f, /// 0x23c
			 0xcdbf14e0, /// 0x240
			 0x7384f7d9, /// 0x244
			 0x61eba670, /// 0x248
			 0x7a29486c, /// 0x24c
			 0xa0acc815, /// 0x250
			 0x59f736f2, /// 0x254
			 0xa25ae80c, /// 0x258
			 0x06764188, /// 0x25c
			 0xd37b5184, /// 0x260
			 0x245e41d4, /// 0x264
			 0x49350fc0, /// 0x268
			 0xa27a0a06, /// 0x26c
			 0xeb1b14dd, /// 0x270
			 0xeb40fab4, /// 0x274
			 0x8e988827, /// 0x278
			 0x26138228, /// 0x27c
			 0x7821128b, /// 0x280
			 0x210381e2, /// 0x284
			 0x5f75f488, /// 0x288
			 0x56b42fc5, /// 0x28c
			 0x46777058, /// 0x290
			 0x82b2cc85, /// 0x294
			 0x32ccfb9e, /// 0x298
			 0x8d22b985, /// 0x29c
			 0x31dfdf5d, /// 0x2a0
			 0x68cd536b, /// 0x2a4
			 0x4390aad9, /// 0x2a8
			 0x9f0aedcd, /// 0x2ac
			 0x748b08ea, /// 0x2b0
			 0x0707e8d4, /// 0x2b4
			 0x23fb7f00, /// 0x2b8
			 0x591d49e7, /// 0x2bc
			 0xee693bba, /// 0x2c0
			 0x0bf5636f, /// 0x2c4
			 0x1ab66cd9, /// 0x2c8
			 0x631c1f71, /// 0x2cc
			 0xd944426e, /// 0x2d0
			 0xdd648d78, /// 0x2d4
			 0xe347290d, /// 0x2d8
			 0xc5021383, /// 0x2dc
			 0x96603028, /// 0x2e0
			 0xfcae47da, /// 0x2e4
			 0xba103cfa, /// 0x2e8
			 0xc4705fff, /// 0x2ec
			 0xe9fc2875, /// 0x2f0
			 0x4af8e579, /// 0x2f4
			 0x67b6445b, /// 0x2f8
			 0xa632f9fb, /// 0x2fc
			 0xeb74c0cb, /// 0x300
			 0xf431ff94, /// 0x304
			 0x05c23fa8, /// 0x308
			 0xaec8c834, /// 0x30c
			 0x1c170298, /// 0x310
			 0x88d15c67, /// 0x314
			 0x851570f3, /// 0x318
			 0xb82de596, /// 0x31c
			 0x0d42758c, /// 0x320
			 0x41b12ca8, /// 0x324
			 0x9d6a65c7, /// 0x328
			 0x51e38d0d, /// 0x32c
			 0x11772285, /// 0x330
			 0xbc33dd18, /// 0x334
			 0x9ce65b89, /// 0x338
			 0x6d72b73c, /// 0x33c
			 0x8b8372af, /// 0x340
			 0xc4d8aa26, /// 0x344
			 0x996212a1, /// 0x348
			 0x823002a7, /// 0x34c
			 0x465bba45, /// 0x350
			 0x40018b2b, /// 0x354
			 0x453aa74d, /// 0x358
			 0xde942ef8, /// 0x35c
			 0xd6f21716, /// 0x360
			 0x41824a9a, /// 0x364
			 0x55e7f05b, /// 0x368
			 0xfcb1b1df, /// 0x36c
			 0x3c6aff40, /// 0x370
			 0x75496518, /// 0x374
			 0x3bd941a7, /// 0x378
			 0xa8739b4d, /// 0x37c
			 0x0d669153, /// 0x380
			 0x6c427178, /// 0x384
			 0x2df1f2e0, /// 0x388
			 0x15b0490b, /// 0x38c
			 0x8e9e72a2, /// 0x390
			 0x1360adec, /// 0x394
			 0xea47c188, /// 0x398
			 0x860baa25, /// 0x39c
			 0x4a528901, /// 0x3a0
			 0xe2c62534, /// 0x3a4
			 0xfd2e2f04, /// 0x3a8
			 0xa57d348b, /// 0x3ac
			 0x3a82853d, /// 0x3b0
			 0xdec81a53, /// 0x3b4
			 0x8716f15d, /// 0x3b8
			 0xb5b4c28b, /// 0x3bc
			 0xf8f6d731, /// 0x3c0
			 0xf2683c06, /// 0x3c4
			 0x50f01189, /// 0x3c8
			 0x6185f3cc, /// 0x3cc
			 0x8b64d795, /// 0x3d0
			 0xf672776b, /// 0x3d4
			 0xec891c66, /// 0x3d8
			 0x8c6c4d94, /// 0x3dc
			 0x32db6787, /// 0x3e0
			 0x968260a2, /// 0x3e4
			 0x2b5db8bb, /// 0x3e8
			 0x648f38df, /// 0x3ec
			 0x5d9b02e9, /// 0x3f0
			 0x73a475d1, /// 0x3f4
			 0x7fb4e179, /// 0x3f8
			 0x884c2e23, /// 0x3fc
			 0x1eb8bd6c, /// 0x400
			 0xc6887f86, /// 0x404
			 0xa01233f6, /// 0x408
			 0x046e8364, /// 0x40c
			 0x509f3ff3, /// 0x410
			 0xa5de68fb, /// 0x414
			 0x231e5212, /// 0x418
			 0x7d0b11d9, /// 0x41c
			 0xe4e3b819, /// 0x420
			 0xeeb2f005, /// 0x424
			 0xc830f182, /// 0x428
			 0x8db07cb4, /// 0x42c
			 0xa52c7a23, /// 0x430
			 0x50908163, /// 0x434
			 0x9ec98372, /// 0x438
			 0x7606ac0b, /// 0x43c
			 0xd524b49a, /// 0x440
			 0x176db28e, /// 0x444
			 0x2b168e5c, /// 0x448
			 0xe3cde898, /// 0x44c
			 0xd815d986, /// 0x450
			 0x8833eca5, /// 0x454
			 0x4c2ddc57, /// 0x458
			 0xd106e635, /// 0x45c
			 0x7bb4cf1f, /// 0x460
			 0x0340c8ea, /// 0x464
			 0x9e148803, /// 0x468
			 0x4aecd9ae, /// 0x46c
			 0xacb1ac28, /// 0x470
			 0x03e81f40, /// 0x474
			 0xde6ee0b1, /// 0x478
			 0xf5ce9f05, /// 0x47c
			 0xb617a401, /// 0x480
			 0xdbcbb020, /// 0x484
			 0xbc043e3c, /// 0x488
			 0xcdbb4f96, /// 0x48c
			 0x1a0497ff, /// 0x490
			 0x6c5823cf, /// 0x494
			 0x57295987, /// 0x498
			 0x793bc7be, /// 0x49c
			 0x93d6088b, /// 0x4a0
			 0xcdb3e57f, /// 0x4a4
			 0x3f6798d1, /// 0x4a8
			 0x01341d5b, /// 0x4ac
			 0x326e8be6, /// 0x4b0
			 0x41ac1890, /// 0x4b4
			 0x4fc9e3c0, /// 0x4b8
			 0x34fbda46, /// 0x4bc
			 0xfca226e9, /// 0x4c0
			 0xd7f03854, /// 0x4c4
			 0x7a8fa9f2, /// 0x4c8
			 0xd2d3204c, /// 0x4cc
			 0x2f70b4eb, /// 0x4d0
			 0xd3009179, /// 0x4d4
			 0xa5f676e8, /// 0x4d8
			 0x0554c278, /// 0x4dc
			 0x29ab605b, /// 0x4e0
			 0x5f4bb37b, /// 0x4e4
			 0xc6344ad9, /// 0x4e8
			 0xd2c82beb, /// 0x4ec
			 0x83345ad2, /// 0x4f0
			 0xc2c74bac, /// 0x4f4
			 0x71bae6fc, /// 0x4f8
			 0x5788f95c, /// 0x4fc
			 0x749dfc4d, /// 0x500
			 0x059c336f, /// 0x504
			 0xc18a590d, /// 0x508
			 0xc744df73, /// 0x50c
			 0x3d967aee, /// 0x510
			 0x79f1365b, /// 0x514
			 0x4c222f21, /// 0x518
			 0x89524ac5, /// 0x51c
			 0xdb046719, /// 0x520
			 0xf9ca8cc9, /// 0x524
			 0xd5362dc4, /// 0x528
			 0x49b27441, /// 0x52c
			 0x76fa39d5, /// 0x530
			 0x1bd1cc70, /// 0x534
			 0x59ecf990, /// 0x538
			 0x6816a9c3, /// 0x53c
			 0xdbe300da, /// 0x540
			 0x76965a67, /// 0x544
			 0x0ec59fdc, /// 0x548
			 0xe0bfa123, /// 0x54c
			 0x098ff916, /// 0x550
			 0xf206a96c, /// 0x554
			 0xdd010cea, /// 0x558
			 0xf1a0bdb2, /// 0x55c
			 0xf99446f3, /// 0x560
			 0xd5174b2f, /// 0x564
			 0x2449a9aa, /// 0x568
			 0x6e50f08a, /// 0x56c
			 0x87981bc2, /// 0x570
			 0xbf4fa4af, /// 0x574
			 0x23170288, /// 0x578
			 0x69e300f8, /// 0x57c
			 0x3d867258, /// 0x580
			 0x32daa265, /// 0x584
			 0x93873f89, /// 0x588
			 0xaa39e4f3, /// 0x58c
			 0x436f2d43, /// 0x590
			 0xebef4854, /// 0x594
			 0x0c42a5fd, /// 0x598
			 0x648ada02, /// 0x59c
			 0x0fc48327, /// 0x5a0
			 0x5dfb491d, /// 0x5a4
			 0x274ed0c7, /// 0x5a8
			 0x34fdfc8a, /// 0x5ac
			 0x705d59a4, /// 0x5b0
			 0x49b27954, /// 0x5b4
			 0x756a59b7, /// 0x5b8
			 0x1413a3c5, /// 0x5bc
			 0xb03e0e5a, /// 0x5c0
			 0x3f6ce351, /// 0x5c4
			 0xdd69a7a6, /// 0x5c8
			 0xe9ee3a91, /// 0x5cc
			 0xddb01e14, /// 0x5d0
			 0xff75666d, /// 0x5d4
			 0x42c0cb3e, /// 0x5d8
			 0xc85d8c69, /// 0x5dc
			 0x6c8edd37, /// 0x5e0
			 0x5d64087d, /// 0x5e4
			 0x60d57d18, /// 0x5e8
			 0xb9525080, /// 0x5ec
			 0xcd5301ab, /// 0x5f0
			 0x96476c84, /// 0x5f4
			 0x1d86e993, /// 0x5f8
			 0xc1a9636d, /// 0x5fc
			 0xa91d5089, /// 0x600
			 0xbc6b6a96, /// 0x604
			 0xebc5b8fa, /// 0x608
			 0x4b88fc93, /// 0x60c
			 0x111fd290, /// 0x610
			 0xea82cc14, /// 0x614
			 0xe38c5001, /// 0x618
			 0x3b0a4300, /// 0x61c
			 0xad73bb20, /// 0x620
			 0xa64af7e4, /// 0x624
			 0x19e597c0, /// 0x628
			 0xb7e4c02a, /// 0x62c
			 0xa02e665e, /// 0x630
			 0xf16b3d57, /// 0x634
			 0x43674895, /// 0x638
			 0x2852f9dc, /// 0x63c
			 0x3cd6b455, /// 0x640
			 0x15f46767, /// 0x644
			 0x57988492, /// 0x648
			 0x7545e0ee, /// 0x64c
			 0x78c7ac01, /// 0x650
			 0x9003ab83, /// 0x654
			 0x1df0694c, /// 0x658
			 0x5c28e6fa, /// 0x65c
			 0x605c9342, /// 0x660
			 0xd02fc91c, /// 0x664
			 0xeb251e63, /// 0x668
			 0x9b213cd1, /// 0x66c
			 0xca46d0a3, /// 0x670
			 0x7dba9aec, /// 0x674
			 0x0776385a, /// 0x678
			 0x887ecd39, /// 0x67c
			 0x277aacad, /// 0x680
			 0x982411a4, /// 0x684
			 0xd0c753c5, /// 0x688
			 0x92eb088d, /// 0x68c
			 0xae9d8758, /// 0x690
			 0x1dc6120f, /// 0x694
			 0xc5df7c6a, /// 0x698
			 0x127b556c, /// 0x69c
			 0xd8088d78, /// 0x6a0
			 0x9285a08c, /// 0x6a4
			 0x6c8a1045, /// 0x6a8
			 0xdf85f2a0, /// 0x6ac
			 0xa1ba4d01, /// 0x6b0
			 0x5d9815aa, /// 0x6b4
			 0x8e1b5ae8, /// 0x6b8
			 0xd27d6f13, /// 0x6bc
			 0x1ff882c8, /// 0x6c0
			 0x9021c1cc, /// 0x6c4
			 0x350173f8, /// 0x6c8
			 0x9b56a0df, /// 0x6cc
			 0xb046eca7, /// 0x6d0
			 0x5b41cad7, /// 0x6d4
			 0x3cc26f83, /// 0x6d8
			 0xbcf2aff7, /// 0x6dc
			 0x7ef1557c, /// 0x6e0
			 0x6af8d0b5, /// 0x6e4
			 0x9ac18f34, /// 0x6e8
			 0x981633ec, /// 0x6ec
			 0xb2bb77a3, /// 0x6f0
			 0xeee6b850, /// 0x6f4
			 0x8e56d9b7, /// 0x6f8
			 0x7ad7117b, /// 0x6fc
			 0x196c6d2d, /// 0x700
			 0x74f5418a, /// 0x704
			 0x5c43ab40, /// 0x708
			 0xe835776c, /// 0x70c
			 0xa439806a, /// 0x710
			 0x56667885, /// 0x714
			 0x3b5605c3, /// 0x718
			 0xaa51fd2c, /// 0x71c
			 0x2d81a19f, /// 0x720
			 0x1fcfcb50, /// 0x724
			 0x07791b7c, /// 0x728
			 0x313b2a52, /// 0x72c
			 0xcb6ab47b, /// 0x730
			 0x4fc59471, /// 0x734
			 0x79fda1b7, /// 0x738
			 0x49046d2e, /// 0x73c
			 0x9caeb766, /// 0x740
			 0xd587978d, /// 0x744
			 0xc6cf8382, /// 0x748
			 0xd460c725, /// 0x74c
			 0xd8fc5479, /// 0x750
			 0x199dc8c8, /// 0x754
			 0xde0028a9, /// 0x758
			 0xdafa002f, /// 0x75c
			 0x6895da91, /// 0x760
			 0xd44ac2b8, /// 0x764
			 0xd6477840, /// 0x768
			 0x3e4c168c, /// 0x76c
			 0xc95ad9b6, /// 0x770
			 0x7e4c945c, /// 0x774
			 0x1b62e802, /// 0x778
			 0x58ce0c18, /// 0x77c
			 0x4bddc8c8, /// 0x780
			 0xc3304445, /// 0x784
			 0xd4b5fbcc, /// 0x788
			 0xc41823a7, /// 0x78c
			 0xcb95410d, /// 0x790
			 0x267eaabc, /// 0x794
			 0x5b75a49d, /// 0x798
			 0xbce6cfb0, /// 0x79c
			 0xc15beabb, /// 0x7a0
			 0xd65997cc, /// 0x7a4
			 0x0d69bdb1, /// 0x7a8
			 0x61da6d10, /// 0x7ac
			 0xc90a635e, /// 0x7b0
			 0x1248dae1, /// 0x7b4
			 0x49517260, /// 0x7b8
			 0x4f7cfea9, /// 0x7bc
			 0x1e22ea03, /// 0x7c0
			 0x277e6ebf, /// 0x7c4
			 0x86ec8129, /// 0x7c8
			 0x861eac67, /// 0x7cc
			 0xe30512fa, /// 0x7d0
			 0x48c1686f, /// 0x7d4
			 0xfed4f376, /// 0x7d8
			 0xb5b33f0a, /// 0x7dc
			 0x6cc6a20d, /// 0x7e0
			 0x01ef12c5, /// 0x7e4
			 0xe58b9ebf, /// 0x7e8
			 0xc1c472fa, /// 0x7ec
			 0x97ae3e52, /// 0x7f0
			 0x68965822, /// 0x7f4
			 0x31f7382d, /// 0x7f8
			 0x3710c188, /// 0x7fc
			 0x6e65ac66, /// 0x800
			 0x804b45cf, /// 0x804
			 0x85c275c8, /// 0x808
			 0x2f7d4e52, /// 0x80c
			 0x957b6d76, /// 0x810
			 0xe2edc4c1, /// 0x814
			 0x74c0da9b, /// 0x818
			 0xfe799136, /// 0x81c
			 0x6c2fb707, /// 0x820
			 0x41050146, /// 0x824
			 0x9c2964cf, /// 0x828
			 0x8f0b4738, /// 0x82c
			 0xf6f2cf69, /// 0x830
			 0xbd523ae7, /// 0x834
			 0xe56530da, /// 0x838
			 0xd3b7694b, /// 0x83c
			 0x2b0722b7, /// 0x840
			 0xea8cf5c5, /// 0x844
			 0xa3c2356d, /// 0x848
			 0x79584f1b, /// 0x84c
			 0xc9a4c9cd, /// 0x850
			 0x66f8bb6c, /// 0x854
			 0x6d284b45, /// 0x858
			 0x0ce5df72, /// 0x85c
			 0x4ddf111c, /// 0x860
			 0x77105d0e, /// 0x864
			 0xc9c65cd0, /// 0x868
			 0xdd7d7ded, /// 0x86c
			 0xfbb4a7d6, /// 0x870
			 0xc8377c9f, /// 0x874
			 0xaf07d19c, /// 0x878
			 0xc71718fc, /// 0x87c
			 0x05f75210, /// 0x880
			 0xcdd6c3e8, /// 0x884
			 0x48392624, /// 0x888
			 0x3a3429be, /// 0x88c
			 0xaf7a9b70, /// 0x890
			 0xd7da0a43, /// 0x894
			 0xce304490, /// 0x898
			 0xdd3f5916, /// 0x89c
			 0x9d1e31df, /// 0x8a0
			 0xb8f62fb6, /// 0x8a4
			 0x04da4d13, /// 0x8a8
			 0x9248ebd4, /// 0x8ac
			 0xcb89d730, /// 0x8b0
			 0x4ee234a8, /// 0x8b4
			 0xaaff62a3, /// 0x8b8
			 0xc17538f8, /// 0x8bc
			 0x6c759332, /// 0x8c0
			 0x168781cf, /// 0x8c4
			 0x3384a35b, /// 0x8c8
			 0x950fafa3, /// 0x8cc
			 0xf67144eb, /// 0x8d0
			 0x048307ca, /// 0x8d4
			 0x3f02b55a, /// 0x8d8
			 0xeb398a5c, /// 0x8dc
			 0x6a436a8c, /// 0x8e0
			 0xb065da97, /// 0x8e4
			 0xda73a68f, /// 0x8e8
			 0x3b74c06e, /// 0x8ec
			 0x1103e566, /// 0x8f0
			 0x3cc98a9b, /// 0x8f4
			 0x91f0c1f9, /// 0x8f8
			 0x37197715, /// 0x8fc
			 0xdd9bcf69, /// 0x900
			 0xa07b0dfc, /// 0x904
			 0x2b1b2e9d, /// 0x908
			 0xb7815ce9, /// 0x90c
			 0x2d22565d, /// 0x910
			 0xb6c4e497, /// 0x914
			 0xa7d140a7, /// 0x918
			 0xe673af92, /// 0x91c
			 0xfbcfa119, /// 0x920
			 0xa5b76309, /// 0x924
			 0x530a3acb, /// 0x928
			 0xfdd14e3a, /// 0x92c
			 0xd6bac0b1, /// 0x930
			 0x508e07a2, /// 0x934
			 0xc844e989, /// 0x938
			 0x1ff76ef7, /// 0x93c
			 0x44ec24a6, /// 0x940
			 0xf7eda5af, /// 0x944
			 0x481d9672, /// 0x948
			 0xc4159ec2, /// 0x94c
			 0xfdb91a13, /// 0x950
			 0x27a9ac49, /// 0x954
			 0x0655f165, /// 0x958
			 0xa8b2a9af, /// 0x95c
			 0x068c511a, /// 0x960
			 0xa48b9a26, /// 0x964
			 0xd733bfdf, /// 0x968
			 0x9a8204ca, /// 0x96c
			 0xa4567aa9, /// 0x970
			 0x9e61e311, /// 0x974
			 0x9f2db163, /// 0x978
			 0xc6282e58, /// 0x97c
			 0xacd2d485, /// 0x980
			 0x4f6a265c, /// 0x984
			 0x73676041, /// 0x988
			 0xe316df28, /// 0x98c
			 0x3aa69b90, /// 0x990
			 0x5629a8b4, /// 0x994
			 0x88d956c5, /// 0x998
			 0x5957109f, /// 0x99c
			 0x6be99a9a, /// 0x9a0
			 0xcfa87275, /// 0x9a4
			 0xb495027e, /// 0x9a8
			 0x3c4e4359, /// 0x9ac
			 0xf4df3b76, /// 0x9b0
			 0xe630f07a, /// 0x9b4
			 0xb28da5ee, /// 0x9b8
			 0xc0556495, /// 0x9bc
			 0xa4edf880, /// 0x9c0
			 0x29b2375d, /// 0x9c4
			 0xacda1dd5, /// 0x9c8
			 0x9c744010, /// 0x9cc
			 0x7f2c8ea7, /// 0x9d0
			 0x61f8e125, /// 0x9d4
			 0xf9c8cd4a, /// 0x9d8
			 0x028046b6, /// 0x9dc
			 0x86827556, /// 0x9e0
			 0x3c82c0dc, /// 0x9e4
			 0xa4c71c81, /// 0x9e8
			 0xdc3c24ed, /// 0x9ec
			 0xbc57137b, /// 0x9f0
			 0x5971ea03, /// 0x9f4
			 0x2d67ee94, /// 0x9f8
			 0xdac1ac85, /// 0x9fc
			 0x35295b89, /// 0xa00
			 0x4377bfe6, /// 0xa04
			 0xb8eaae10, /// 0xa08
			 0x044fb997, /// 0xa0c
			 0xc15de9a6, /// 0xa10
			 0x570bbd99, /// 0xa14
			 0xb277f2c6, /// 0xa18
			 0xf19df0c8, /// 0xa1c
			 0x30a23425, /// 0xa20
			 0x8dc27f51, /// 0xa24
			 0xf5cbe6a3, /// 0xa28
			 0x5e708631, /// 0xa2c
			 0x80324e2b, /// 0xa30
			 0xb05fe0eb, /// 0xa34
			 0x4ce6dec5, /// 0xa38
			 0x6029f0c5, /// 0xa3c
			 0xaf4dda3d, /// 0xa40
			 0x3c39d44e, /// 0xa44
			 0x06af3edf, /// 0xa48
			 0x25e5d433, /// 0xa4c
			 0x1d194088, /// 0xa50
			 0x69d4068e, /// 0xa54
			 0x7801b9f0, /// 0xa58
			 0xdb886af0, /// 0xa5c
			 0x1001f561, /// 0xa60
			 0x3adf34b5, /// 0xa64
			 0xaca4845f, /// 0xa68
			 0x77a465ef, /// 0xa6c
			 0xc8c3a6a3, /// 0xa70
			 0x5cd0a42a, /// 0xa74
			 0x687cfb0b, /// 0xa78
			 0xbd86d475, /// 0xa7c
			 0x0889cf91, /// 0xa80
			 0xc45e5e34, /// 0xa84
			 0x09aa51d4, /// 0xa88
			 0xd94b795b, /// 0xa8c
			 0xa01ae3dc, /// 0xa90
			 0xfbac9d93, /// 0xa94
			 0x3dbecae2, /// 0xa98
			 0x6a65a325, /// 0xa9c
			 0x54631494, /// 0xaa0
			 0xcad35162, /// 0xaa4
			 0x0fdb0dd7, /// 0xaa8
			 0x1d650dcf, /// 0xaac
			 0xc7f49c70, /// 0xab0
			 0x40739348, /// 0xab4
			 0x50154c08, /// 0xab8
			 0xc1d7105f, /// 0xabc
			 0x340c6d38, /// 0xac0
			 0x378dbc95, /// 0xac4
			 0xee9a14de, /// 0xac8
			 0x04a76a7b, /// 0xacc
			 0xb572daca, /// 0xad0
			 0x713d237c, /// 0xad4
			 0x0ab8a078, /// 0xad8
			 0x6b7e6bb5, /// 0xadc
			 0x10a4c858, /// 0xae0
			 0x7f15d74b, /// 0xae4
			 0x11935e5a, /// 0xae8
			 0x00be1317, /// 0xaec
			 0xe78b4f4a, /// 0xaf0
			 0x7abd80c3, /// 0xaf4
			 0x367a1712, /// 0xaf8
			 0xacb45257, /// 0xafc
			 0xabc0a630, /// 0xb00
			 0xa0c55fd9, /// 0xb04
			 0x1f268595, /// 0xb08
			 0xf6713b55, /// 0xb0c
			 0xf859bf60, /// 0xb10
			 0x5a2c9780, /// 0xb14
			 0x8dc09f90, /// 0xb18
			 0x856431a5, /// 0xb1c
			 0x71625358, /// 0xb20
			 0xc7a3f3d4, /// 0xb24
			 0xe4426b21, /// 0xb28
			 0x69d0ac55, /// 0xb2c
			 0x3114ed1e, /// 0xb30
			 0x8245d866, /// 0xb34
			 0xdc24901d, /// 0xb38
			 0x66466511, /// 0xb3c
			 0x07a7f8eb, /// 0xb40
			 0xa380feb1, /// 0xb44
			 0xfb455c05, /// 0xb48
			 0x8e382291, /// 0xb4c
			 0x2639ecd1, /// 0xb50
			 0xea93b41b, /// 0xb54
			 0xd0a34ebb, /// 0xb58
			 0xf7311881, /// 0xb5c
			 0xfe938aa1, /// 0xb60
			 0xce6497bb, /// 0xb64
			 0x8fba9c15, /// 0xb68
			 0x96cda0c3, /// 0xb6c
			 0x91f18175, /// 0xb70
			 0x6d7e35fc, /// 0xb74
			 0x5f72261e, /// 0xb78
			 0x0ae75260, /// 0xb7c
			 0x67161e28, /// 0xb80
			 0xcdd1a6ac, /// 0xb84
			 0x6f828c71, /// 0xb88
			 0xeb5cf8df, /// 0xb8c
			 0x27a12279, /// 0xb90
			 0x6aea5956, /// 0xb94
			 0xbf8f6cf2, /// 0xb98
			 0xf2d71068, /// 0xb9c
			 0x7112668d, /// 0xba0
			 0x6dbbcdb5, /// 0xba4
			 0x7d9f8c0c, /// 0xba8
			 0xdbd7fdc9, /// 0xbac
			 0x2724df33, /// 0xbb0
			 0xd27fe17d, /// 0xbb4
			 0x8cd02691, /// 0xbb8
			 0xac515e35, /// 0xbbc
			 0xc580348d, /// 0xbc0
			 0xec797d91, /// 0xbc4
			 0x4711be96, /// 0xbc8
			 0x8e3533f2, /// 0xbcc
			 0x59633acb, /// 0xbd0
			 0x7a9e6b7b, /// 0xbd4
			 0x1dcc4d9e, /// 0xbd8
			 0x0432809b, /// 0xbdc
			 0x992b5c00, /// 0xbe0
			 0x5e4ec196, /// 0xbe4
			 0x043f69b7, /// 0xbe8
			 0x8f0d0018, /// 0xbec
			 0xac59e953, /// 0xbf0
			 0xe6a00ff9, /// 0xbf4
			 0xa37c49ce, /// 0xbf8
			 0x294b8cba, /// 0xbfc
			 0x5a0c7d48, /// 0xc00
			 0x30394110, /// 0xc04
			 0x75ff9807, /// 0xc08
			 0x95cb65a2, /// 0xc0c
			 0xd3b664df, /// 0xc10
			 0x2eb63808, /// 0xc14
			 0x07c5c62e, /// 0xc18
			 0xa05c34e3, /// 0xc1c
			 0xa8376050, /// 0xc20
			 0xa77c8808, /// 0xc24
			 0xf90192fc, /// 0xc28
			 0x76c8e9b5, /// 0xc2c
			 0x0b832b47, /// 0xc30
			 0x44d1df43, /// 0xc34
			 0x95174e35, /// 0xc38
			 0x14a2f7ba, /// 0xc3c
			 0x074bb663, /// 0xc40
			 0x94e703a3, /// 0xc44
			 0xf7dc1977, /// 0xc48
			 0x28046cdd, /// 0xc4c
			 0xb7604001, /// 0xc50
			 0x32942ae6, /// 0xc54
			 0x536e713a, /// 0xc58
			 0xec3248f1, /// 0xc5c
			 0x1e242c53, /// 0xc60
			 0x3599d4c5, /// 0xc64
			 0x89e0587f, /// 0xc68
			 0x0f804d85, /// 0xc6c
			 0xba004f55, /// 0xc70
			 0x5f5b7d60, /// 0xc74
			 0xd859be16, /// 0xc78
			 0x5252f4e8, /// 0xc7c
			 0x323f8dd7, /// 0xc80
			 0xd6b6235e, /// 0xc84
			 0xb02764a4, /// 0xc88
			 0x60277b1f, /// 0xc8c
			 0x91e9ed84, /// 0xc90
			 0xdcdfa6b8, /// 0xc94
			 0xf42592f9, /// 0xc98
			 0x4dbd2d86, /// 0xc9c
			 0xd5329a6b, /// 0xca0
			 0xf26efd83, /// 0xca4
			 0x7000d06c, /// 0xca8
			 0xb743ba06, /// 0xcac
			 0x411e8c13, /// 0xcb0
			 0xe9252c03, /// 0xcb4
			 0x1b55c797, /// 0xcb8
			 0x426f0d8c, /// 0xcbc
			 0x8990a41d, /// 0xcc0
			 0xefd898eb, /// 0xcc4
			 0xf21c2928, /// 0xcc8
			 0x8be3b585, /// 0xccc
			 0x1fbaad09, /// 0xcd0
			 0x206278b4, /// 0xcd4
			 0x8ccde8ed, /// 0xcd8
			 0x623d1441, /// 0xcdc
			 0x1626955a, /// 0xce0
			 0xb3365462, /// 0xce4
			 0x81b87ffc, /// 0xce8
			 0x5b4bd057, /// 0xcec
			 0x3aabb18e, /// 0xcf0
			 0x0e539f27, /// 0xcf4
			 0x8c353af1, /// 0xcf8
			 0x179b86be, /// 0xcfc
			 0xe22965e3, /// 0xd00
			 0xdf722b5e, /// 0xd04
			 0xe048a9ae, /// 0xd08
			 0x30d50ad7, /// 0xd0c
			 0x91fef94a, /// 0xd10
			 0xafc625b9, /// 0xd14
			 0xbb00f814, /// 0xd18
			 0xfcef39f0, /// 0xd1c
			 0x0c532d10, /// 0xd20
			 0x80bb4eb5, /// 0xd24
			 0x86248972, /// 0xd28
			 0xa877d23a, /// 0xd2c
			 0x72e583ff, /// 0xd30
			 0xa8e9cafe, /// 0xd34
			 0x913ca24c, /// 0xd38
			 0x5fde5bba, /// 0xd3c
			 0xe46973cc, /// 0xd40
			 0x442ffda1, /// 0xd44
			 0x65cb3d38, /// 0xd48
			 0xdd186fa4, /// 0xd4c
			 0x9a590971, /// 0xd50
			 0x77e8eb0b, /// 0xd54
			 0x18e3d52b, /// 0xd58
			 0x25306247, /// 0xd5c
			 0x452edccc, /// 0xd60
			 0x38658afc, /// 0xd64
			 0xb7aaa22b, /// 0xd68
			 0x96337ed1, /// 0xd6c
			 0xc8e9c3a0, /// 0xd70
			 0xd12057d1, /// 0xd74
			 0x0103b1ce, /// 0xd78
			 0xa75fac71, /// 0xd7c
			 0x3ed17515, /// 0xd80
			 0x2f2aeb4c, /// 0xd84
			 0x03a7f021, /// 0xd88
			 0x14e6de3a, /// 0xd8c
			 0xf624103c, /// 0xd90
			 0xebccb31a, /// 0xd94
			 0x80c37c43, /// 0xd98
			 0xc3ded4b9, /// 0xd9c
			 0x164a79c7, /// 0xda0
			 0xcdc5895e, /// 0xda4
			 0x0dc4c894, /// 0xda8
			 0xd52c4597, /// 0xdac
			 0x910fca41, /// 0xdb0
			 0xd2d35d02, /// 0xdb4
			 0x9077a98c, /// 0xdb8
			 0x94efd8d2, /// 0xdbc
			 0x54574e7c, /// 0xdc0
			 0x75cfef72, /// 0xdc4
			 0x1741417a, /// 0xdc8
			 0x26a9be57, /// 0xdcc
			 0xc8f70894, /// 0xdd0
			 0xf2a3ab0e, /// 0xdd4
			 0xa0ad0a49, /// 0xdd8
			 0x4d097e69, /// 0xddc
			 0x93ce11f7, /// 0xde0
			 0xa47e1c3c, /// 0xde4
			 0xea5afac9, /// 0xde8
			 0x990a9d84, /// 0xdec
			 0xd2109c13, /// 0xdf0
			 0xaead593e, /// 0xdf4
			 0xeff223dd, /// 0xdf8
			 0xcb909a14, /// 0xdfc
			 0xa2ffd9a8, /// 0xe00
			 0x7ac40596, /// 0xe04
			 0x6991bfef, /// 0xe08
			 0x32d94893, /// 0xe0c
			 0x4d5c5f6e, /// 0xe10
			 0x5a9c23f6, /// 0xe14
			 0xf1a18d3c, /// 0xe18
			 0x9fbf9536, /// 0xe1c
			 0xb681b408, /// 0xe20
			 0x191939a8, /// 0xe24
			 0xbb9b4af4, /// 0xe28
			 0x35a1dd7f, /// 0xe2c
			 0xb6eb4a7b, /// 0xe30
			 0x660bd530, /// 0xe34
			 0x6e379db7, /// 0xe38
			 0x1897bcda, /// 0xe3c
			 0x384bd324, /// 0xe40
			 0x421f5a92, /// 0xe44
			 0x3b27c998, /// 0xe48
			 0xf2822f9a, /// 0xe4c
			 0xf736b1aa, /// 0xe50
			 0x2c3ea76c, /// 0xe54
			 0xc39cec14, /// 0xe58
			 0x6c6143db, /// 0xe5c
			 0x29d21553, /// 0xe60
			 0x9b1d62ab, /// 0xe64
			 0x18db3ca9, /// 0xe68
			 0x81ee24d1, /// 0xe6c
			 0xc0c388e8, /// 0xe70
			 0xda9cf984, /// 0xe74
			 0x45c9b23b, /// 0xe78
			 0x6f86175a, /// 0xe7c
			 0x1ce5375b, /// 0xe80
			 0xdf5125a8, /// 0xe84
			 0x221b05dd, /// 0xe88
			 0xfa32d54d, /// 0xe8c
			 0x43af7446, /// 0xe90
			 0xb0eb2f51, /// 0xe94
			 0x11a77d35, /// 0xe98
			 0x5fa8bffd, /// 0xe9c
			 0x255073a1, /// 0xea0
			 0xcf132315, /// 0xea4
			 0x9d75a8db, /// 0xea8
			 0x0513ef6c, /// 0xeac
			 0x2bab07d6, /// 0xeb0
			 0xa3373f1f, /// 0xeb4
			 0x091a84cd, /// 0xeb8
			 0x31ff1436, /// 0xebc
			 0x833f6972, /// 0xec0
			 0x96cf7068, /// 0xec4
			 0xa4d8e76a, /// 0xec8
			 0xa36d591d, /// 0xecc
			 0x7150056c, /// 0xed0
			 0x0d024fae, /// 0xed4
			 0xb2ea6970, /// 0xed8
			 0x11af8feb, /// 0xedc
			 0xbaa7e43e, /// 0xee0
			 0x450add4f, /// 0xee4
			 0xbe5d13c3, /// 0xee8
			 0x418f63d0, /// 0xeec
			 0xaf78ef6e, /// 0xef0
			 0x9f4f7b4f, /// 0xef4
			 0x541acff9, /// 0xef8
			 0x6778c72a, /// 0xefc
			 0xdee38d64, /// 0xf00
			 0xba6f6a16, /// 0xf04
			 0x5a52307b, /// 0xf08
			 0x35e762a4, /// 0xf0c
			 0xee595c40, /// 0xf10
			 0x8b02a51f, /// 0xf14
			 0xfd360c53, /// 0xf18
			 0xd9872324, /// 0xf1c
			 0x1ee7cef5, /// 0xf20
			 0xdf80385e, /// 0xf24
			 0x9a441a0a, /// 0xf28
			 0x6f2e195e, /// 0xf2c
			 0x4c36cc29, /// 0xf30
			 0x61dabf1c, /// 0xf34
			 0x3f259688, /// 0xf38
			 0xf4653495, /// 0xf3c
			 0x6a8387f3, /// 0xf40
			 0x53991c53, /// 0xf44
			 0x3bfad001, /// 0xf48
			 0x68721c71, /// 0xf4c
			 0x0a6a169e, /// 0xf50
			 0x445543f1, /// 0xf54
			 0x8747fbe6, /// 0xf58
			 0x1d554969, /// 0xf5c
			 0x0dcf6579, /// 0xf60
			 0x6f37cdbc, /// 0xf64
			 0x741f8e05, /// 0xf68
			 0xcbd29bc2, /// 0xf6c
			 0x10c3c6b0, /// 0xf70
			 0x05889f03, /// 0xf74
			 0x62310163, /// 0xf78
			 0x6e4eb83a, /// 0xf7c
			 0x23dbcd14, /// 0xf80
			 0x958ba2a6, /// 0xf84
			 0x60b0ddab, /// 0xf88
			 0x444cfead, /// 0xf8c
			 0xb3b4eed0, /// 0xf90
			 0x6b909175, /// 0xf94
			 0x4336e0b5, /// 0xf98
			 0x0a101860, /// 0xf9c
			 0x7bd6ede8, /// 0xfa0
			 0x41c532aa, /// 0xfa4
			 0xb83a3cdb, /// 0xfa8
			 0xdafefad7, /// 0xfac
			 0x287a1762, /// 0xfb0
			 0xbdf9062b, /// 0xfb4
			 0x1c94e33d, /// 0xfb8
			 0x24c102fa, /// 0xfbc
			 0x48b2bfa7, /// 0xfc0
			 0x4201b78b, /// 0xfc4
			 0xe063c7a1, /// 0xfc8
			 0x70d39136, /// 0xfcc
			 0x6726d51b, /// 0xfd0
			 0xcab50654, /// 0xfd4
			 0x962a0724, /// 0xfd8
			 0xd6cacb27, /// 0xfdc
			 0xdd95a635, /// 0xfe0
			 0xc27637ac, /// 0xfe4
			 0x1075e4a6, /// 0xfe8
			 0x53c9e317, /// 0xfec
			 0x72e85070, /// 0xff0
			 0x05e3a428, /// 0xff4
			 0xffc5a1b5, /// 0xff8
			 0x3d02e556 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0e000001,                                                  // Trailing 1s:                                /// 00000
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00004
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00014
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00018
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x3f028f5c,                                                  // 0.51                                        /// 00020
			 0x80000000,                                                  // -zero                                       /// 00024
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00030
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00038
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x4b000000,                                                  // 8388608.0                                   /// 00048
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0004c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00050
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00058
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00060
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00068
			 0x55555555,                                                  // 4 random values                             /// 0006c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00070
			 0x4b000000,                                                  // 8388608.0                                   /// 00074
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00078
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0007c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00084
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00088
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0008c
			 0xff800000,                                                  // -inf                                        /// 00090
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000a0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000b0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000f0
			 0x55555555,                                                  // 4 random values                             /// 000f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00108
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0010c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x7f800000,                                                  // inf                                         /// 00114
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00118
			 0xffc00001,                                                  // -signaling NaN                              /// 0011c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00120
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00128
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0012c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00130
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00134
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00138
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0013c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00140
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00144
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00148
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0014c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00150
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00154
			 0x00000000,                                                  // zero                                        /// 00158
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00160
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00164
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00168
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0016c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00170
			 0x3f028f5c,                                                  // 0.51                                        /// 00174
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00178
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0017c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00180
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x0e000007,                                                  // Trailing 1s:                                /// 00188
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0018c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00190
			 0x00011111,                                                  // 9.7958E-41                                  /// 00194
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00198
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0019c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001a8
			 0x00000000,                                                  // zero                                        /// 001ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001b0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001c0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001d0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001dc
			 0x55555555,                                                  // 4 random values                             /// 001e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 001e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00204
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0020c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00210
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00214
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00218
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00220
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00224
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00228
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0022c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0xffc00001,                                                  // -signaling NaN                              /// 00234
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00244
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00248
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0024c
			 0x00000000,                                                  // zero                                        /// 00250
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0x4b000000,                                                  // 8388608.0                                   /// 0025c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00264
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00268
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0026c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00274
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00278
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0027c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00280
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00288
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00294
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00298
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002a4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002c0
			 0x4b000000,                                                  // 8388608.0                                   /// 002c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002c8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x3f028f5c,                                                  // 0.51                                        /// 002d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002e4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x0c780000,                                                  // Leading 1s:                                 /// 002ec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002f4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00300
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00308
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0030c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00310
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00318
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0031c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00320
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00324
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00328
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0032c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00330
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00334
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00338
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0033c
			 0x7fc00001,                                                  // signaling NaN                               /// 00340
			 0xbf028f5c,                                                  // -0.51                                       /// 00344
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0034c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00354
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00358
			 0xffc00001,                                                  // -signaling NaN                              /// 0035c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00360
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00364
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0036c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00374
			 0x80000000,                                                  // -zero                                       /// 00378
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0037c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00380
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00388
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0038c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003ac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003c8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003d0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003d4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003e4
			 0x4b000000,                                                  // 8388608.0                                   /// 003e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003ec
			 0x80011111,                                                  // -9.7958E-41                                 /// 003f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00400
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00404
			 0xbf028f5c,                                                  // -0.51                                       /// 00408
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0040c
			 0x7fc00001,                                                  // signaling NaN                               /// 00410
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00418
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0041c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00420
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00424
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00428
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00430
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00434
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00438
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0043c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00440
			 0x80000000,                                                  // -zero                                       /// 00444
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00448
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0044c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00454
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0045c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00460
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00464
			 0xffc00001,                                                  // -signaling NaN                              /// 00468
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00470
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00474
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00478
			 0x55555555,                                                  // 4 random values                             /// 0047c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00480
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00488
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0048c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00490
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00494
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00498
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 004a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004b0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 004c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004cc
			 0x55555555,                                                  // 4 random values                             /// 004d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004d4
			 0x7fc00001,                                                  // signaling NaN                               /// 004d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004f8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004fc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00504
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00508
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0050c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00510
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00518
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0051c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00520
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00524
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00528
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0052c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00534
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0053c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00540
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00544
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00548
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00550
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00554
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00558
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00560
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00564
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00568
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0056c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00570
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00574
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00578
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0057c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0058c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00590
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00594
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00598
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0059c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005a0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x4b000000,                                                  // 8388608.0                                   /// 005c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005d0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005dc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005ec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00600
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00604
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00608
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0060c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00618
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0061c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00620
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x0c600000,                                                  // Leading 1s:                                 /// 00628
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0062c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00630
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00634
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00638
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0063c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00640
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00644
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00648
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00658
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0065c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00660
			 0x7fc00001,                                                  // signaling NaN                               /// 00664
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0066c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00670
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00678
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00680
			 0x0c600000,                                                  // Leading 1s:                                 /// 00684
			 0x0e000007,                                                  // Trailing 1s:                                /// 00688
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0068c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00690
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00694
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0069c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 006b8
			 0x7f800000,                                                  // inf                                         /// 006bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006d0
			 0x55555555,                                                  // 4 random values                             /// 006d4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 006e8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00700
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00704
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00708
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00710
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00718
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0071c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00724
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00728
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0072c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00730
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00738
			 0xff800000,                                                  // -inf                                        /// 0073c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00740
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00744
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00748
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00754
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00758
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0075c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00760
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00764
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00768
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0076c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00770
			 0x00000000,                                                  // zero                                        /// 00774
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0077c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00780
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00784
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0078c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00790
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00794
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00798
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007ac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0xbf028f5c,                                                  // -0.51                                       /// 007b4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007c4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007e8
			 0x7f800000,                                                  // inf                                         /// 007ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00800
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00808
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00818
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0081c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00820
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00824
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00828
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0082c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00830
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00834
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0083c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00844
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00848
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0084c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00850
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00854
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0085c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00864
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0086c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0087c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00880
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00884
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00888
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0088c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x0e000001,                                                  // Trailing 1s:                                /// 00898
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0089c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008a8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008d8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008e0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008f0
			 0xbf028f5c,                                                  // -0.51                                       /// 008f4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00900
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00904
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00908
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00910
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00918
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00920
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00924
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00928
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x80011111,                                                  // -9.7958E-41                                 /// 00938
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00948
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0094c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00954
			 0x00000000,                                                  // zero                                        /// 00958
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00964
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00968
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00970
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00974
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00978
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00980
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0098c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00990
			 0x55555555,                                                  // 4 random values                             /// 00994
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0099c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009a8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009b8
			 0x4b000000,                                                  // 8388608.0                                   /// 009bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009c8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009e0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009f8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a00
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a08
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a10
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a14
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a18
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a20
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a24
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x7f800000,                                                  // inf                                         /// 00a2c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a30
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a34
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a38
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a44
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a54
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a58
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a60
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a64
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a68
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a6c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a78
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a7c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a80
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a84
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a88
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a8c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a90
			 0xbf028f5c,                                                  // -0.51                                       /// 00a94
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a98
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a9c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00aa0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00aa4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00aa8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00aac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ab0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ab4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ac0
			 0x33333333,                                                  // 4 random values                             /// 00ac4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x7f800000,                                                  // inf                                         /// 00ad0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ad4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ad8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00adc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ae0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ae4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ae8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00aec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00af4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00afc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b00
			 0x55555555,                                                  // 4 random values                             /// 00b04
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b08
			 0x80000000,                                                  // -zero                                       /// 00b0c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b10
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b18
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b1c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b20
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b24
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b30
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b34
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b38
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b3c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b40
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b4c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b50
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b54
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b5c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b60
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b64
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b68
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b6c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b70
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b74
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b7c
			 0x3f028f5c,                                                  // 0.51                                        /// 00b80
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b88
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b8c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b90
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b94
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b98
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b9c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ba8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bb0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bb4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bbc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bc0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bc8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bcc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bd4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bd8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00be0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00be8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bf4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bf8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c04
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c08
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c10
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c14
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c1c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c24
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c30
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c34
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c3c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c40
			 0xcb000000,                                                  // -8388608.0                                  /// 00c44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c50
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c54
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c58
			 0x3f028f5c,                                                  // 0.51                                        /// 00c5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c60
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c68
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c6c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c70
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c80
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c84
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c88
			 0x55555555,                                                  // 4 random values                             /// 00c8c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c90
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c94
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c98
			 0xcb000000,                                                  // -8388608.0                                  /// 00c9c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ca0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ca4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cb4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cc8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ccc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cd0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cd8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ce0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ce4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ce8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cf0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cf4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x80000000,                                                  // -zero                                       /// 00cfc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d00
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d0c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d10
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d14
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d1c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d28
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d34
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d3c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d40
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d44
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d48
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d50
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d54
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d5c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d68
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d6c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d70
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d74
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d78
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d7c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d80
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d84
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d8c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d94
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d98
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00da0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x00000000,                                                  // zero                                        /// 00da8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00db8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dc0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dcc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dd0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dd4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dd8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00de0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00de4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00de8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00df8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dfc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e00
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e04
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0xff800000,                                                  // -inf                                        /// 00e0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e10
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e18
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e20
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e24
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e28
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e2c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e38
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e44
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e48
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e4c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e54
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e58
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e5c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e64
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e70
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e74
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e78
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e7c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e80
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e84
			 0x4b000000,                                                  // 8388608.0                                   /// 00e88
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e8c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e90
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e94
			 0xff800000,                                                  // -inf                                        /// 00e98
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e9c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ea0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ea4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ea8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x7fc00001,                                                  // signaling NaN                               /// 00eb0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00eb4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00eb8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ec0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x55555555,                                                  // 4 random values                             /// 00ed0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ed4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ed8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00edc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ee4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ee8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00eec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ef4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ef8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00efc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f00
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f08
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f10
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f14
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f18
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f1c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f20
			 0x55555555,                                                  // 4 random values                             /// 00f24
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f28
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f2c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f30
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f34
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f38
			 0x4b000000,                                                  // 8388608.0                                   /// 00f3c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f40
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f44
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f48
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f4c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f50
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f58
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f5c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f68
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f6c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f70
			 0x3f028f5c,                                                  // 0.51                                        /// 00f74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f7c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f80
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f84
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f94
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f98
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f9c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fa8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fb0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fb4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fb8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fc0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fc4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fc8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fcc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fd8
			 0xbf028f5c,                                                  // -0.51                                       /// 00fdc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fe0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fe4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fe8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x0e0007ff                                                  // Trailing 1s:                                /// last
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
			 0x00000730,
			 0x00000554,
			 0x00000384,
			 0x0000061c,
			 0x00000304,
			 0x00000240,
			 0x000003a8,
			 0x00000270,

			 /// vpu register f2
			 0x3f800000,
			 0x41400000,
			 0x41800000,
			 0x41a80000,
			 0x41e80000,
			 0x41880000,
			 0x41880000,
			 0x41100000,

			 /// vpu register f3
			 0x41c80000,
			 0x40e00000,
			 0x40e00000,
			 0x42000000,
			 0x41f00000,
			 0x41d80000,
			 0x41300000,
			 0x41c00000,

			 /// vpu register f4
			 0x41800000,
			 0x41d00000,
			 0x41300000,
			 0x40c00000,
			 0x41900000,
			 0x41c00000,
			 0x41300000,
			 0x41f80000,

			 /// vpu register f5
			 0x41e80000,
			 0x40800000,
			 0x41a00000,
			 0x41c00000,
			 0x41400000,
			 0x41f80000,
			 0x41b80000,
			 0x41e80000,

			 /// vpu register f6
			 0x41f00000,
			 0x40800000,
			 0x41700000,
			 0x3f800000,
			 0x41980000,
			 0x41880000,
			 0x41b00000,
			 0x41800000,

			 /// vpu register f7
			 0x42000000,
			 0x40e00000,
			 0x41c00000,
			 0x40400000,
			 0x41e80000,
			 0x41b80000,
			 0x41100000,
			 0x41980000,

			 /// vpu register f8
			 0x41600000,
			 0x40a00000,
			 0x41700000,
			 0x40800000,
			 0x41f80000,
			 0x41f80000,
			 0x41200000,
			 0x41b00000,

			 /// vpu register f9
			 0x3f800000,
			 0x41100000,
			 0x40a00000,
			 0x41a00000,
			 0x40e00000,
			 0x41f00000,
			 0x41a00000,
			 0x40400000,

			 /// vpu register f10
			 0x41a80000,
			 0x42000000,
			 0x41980000,
			 0x41600000,
			 0x41600000,
			 0x41a80000,
			 0x41000000,
			 0x41b80000,

			 /// vpu register f11
			 0x41700000,
			 0x40000000,
			 0x41800000,
			 0x41800000,
			 0x40000000,
			 0x41e00000,
			 0x41900000,
			 0x41500000,

			 /// vpu register f12
			 0x41500000,
			 0x41600000,
			 0x41c00000,
			 0x41b00000,
			 0x41e80000,
			 0x41200000,
			 0x41800000,
			 0x41e80000,

			 /// vpu register f13
			 0x41400000,
			 0x41300000,
			 0x41980000,
			 0x41b80000,
			 0x41400000,
			 0x41500000,
			 0x41000000,
			 0x41d80000,

			 /// vpu register f14
			 0x41b00000,
			 0x41f80000,
			 0x41b00000,
			 0x41c00000,
			 0x41d80000,
			 0x41b80000,
			 0x41a80000,
			 0x41400000,

			 /// vpu register f15
			 0x41e80000,
			 0x40a00000,
			 0x41e80000,
			 0x41f80000,
			 0x40c00000,
			 0x41980000,
			 0x41600000,
			 0x41200000,

			 /// vpu register f16
			 0x41000000,
			 0x41900000,
			 0x41600000,
			 0x40a00000,
			 0x41f00000,
			 0x40800000,
			 0x41000000,
			 0x41800000,

			 /// vpu register f17
			 0x3f800000,
			 0x41a00000,
			 0x41e00000,
			 0x41100000,
			 0x40000000,
			 0x42000000,
			 0x40800000,
			 0x40e00000,

			 /// vpu register f18
			 0x41a00000,
			 0x41c80000,
			 0x41e00000,
			 0x41900000,
			 0x40000000,
			 0x41200000,
			 0x41b80000,
			 0x41500000,

			 /// vpu register f19
			 0x41a00000,
			 0x40c00000,
			 0x41a80000,
			 0x40e00000,
			 0x41d80000,
			 0x40400000,
			 0x41880000,
			 0x41200000,

			 /// vpu register f20
			 0x41e80000,
			 0x41a80000,
			 0x41e00000,
			 0x41e80000,
			 0x41d00000,
			 0x41e00000,
			 0x41c00000,
			 0x41a80000,

			 /// vpu register f21
			 0x41e00000,
			 0x41d00000,
			 0x41400000,
			 0x41980000,
			 0x41700000,
			 0x42000000,
			 0x41500000,
			 0x41500000,

			 /// vpu register f22
			 0x41a00000,
			 0x41c00000,
			 0x41900000,
			 0x41f00000,
			 0x42000000,
			 0x40c00000,
			 0x41c80000,
			 0x41800000,

			 /// vpu register f23
			 0x41a00000,
			 0x41a00000,
			 0x41f00000,
			 0x40c00000,
			 0x41f80000,
			 0x41900000,
			 0x41b80000,
			 0x41f80000,

			 /// vpu register f24
			 0x40800000,
			 0x41980000,
			 0x41f00000,
			 0x41c80000,
			 0x41d80000,
			 0x40c00000,
			 0x41400000,
			 0x41f00000,

			 /// vpu register f25
			 0x41000000,
			 0x40800000,
			 0x41b80000,
			 0x41600000,
			 0x41e80000,
			 0x41800000,
			 0x40a00000,
			 0x40000000,

			 /// vpu register f26
			 0x41300000,
			 0x41400000,
			 0x41100000,
			 0x42000000,
			 0x41e00000,
			 0x41900000,
			 0x41900000,
			 0x40000000,

			 /// vpu register f27
			 0x41000000,
			 0x40a00000,
			 0x40400000,
			 0x40c00000,
			 0x40800000,
			 0x41c00000,
			 0x41300000,
			 0x41000000,

			 /// vpu register f28
			 0x41e80000,
			 0x41100000,
			 0x41f00000,
			 0x41200000,
			 0x41980000,
			 0x40c00000,
			 0x40000000,
			 0x40400000,

			 /// vpu register f29
			 0x41c80000,
			 0x41900000,
			 0x41980000,
			 0x41980000,
			 0x41a00000,
			 0x41d80000,
			 0x41d80000,
			 0x41f00000,

			 /// vpu register f30
			 0x40c00000,
			 0x41a00000,
			 0x40000000,
			 0x41d00000,
			 0x41400000,
			 0x41f80000,
			 0x41f80000,
			 0x40000000,

			 /// vpu register f31
			 0x41c80000,
			 0x41f00000,
			 0x40c00000,
			 0x3f800000,
			 0x40400000,
			 0x41300000,
			 0x40400000,
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

	"LBL_VPU_SEQ_START_0:\n"
	

		"flw.ps f8,  -432 (x15)\n"
		"flw.ps f9,  -428 (x15)\n"
		"flw.ps f10,  -424 (x15)\n"
		"flw.ps f11,  -420 (x15)\n"
		"flw.ps f12,  -416 (x15)\n"
		"flw.ps f13,  -412 (x15)\n"
		"flw.ps f14,  -408 (x15)\n"
		"flw.ps f15,  -404 (x15)\n"
		"flw.ps f16,  -400 (x15)\n"
		"flw.ps f17,  -396 (x15)\n"
		"flw.ps f18,  -392 (x15)\n"
		"flw.ps f19,  -388 (x15)\n"
		"flw.ps f20,  -384 (x15)\n"
		"flw.ps f21,  -380 (x15)\n"
		"flw.ps f22,  -376 (x15)\n"
		"flw.ps f23,  -372 (x15)\n"
		"flw.ps f24,  -368 (x15)\n"
		"flw.ps f25,  -364 (x15)\n"
		"flw.ps f26,  -360 (x15)\n"
		"flw.ps f27,  -356 (x15)\n"
		"flw.ps f28,  -352 (x15)\n"
		"flw.ps f29,  -348 (x15)\n"
		"flw.ps f30,  -344 (x15)\n"
		"flw.ps f31,  -340 (x15)\n"
		"fence\n"

		
		//RTLMIN-5479"fsin.ps f6, f22\n"                                   ///  0,   254
		//RTLMIN-5479"fsin.ps f4, f28\n"                                   ///  0,   260

		"fence\n"

//		"csrrw x0, 0x7D1,x0\n"                                     // reset txfma sleep 2to7 csr 
//
//		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (
	"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
