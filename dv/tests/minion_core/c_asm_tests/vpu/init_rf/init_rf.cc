/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"

int main() {

   // Init marker
   C_TEST_START;

   __asm__ __volatile__(
      "mov.m.x m0, x0, 0xff\n"

      "fmadd.ps f0, f0, f0, f0\n"
      "fmadd.ps f1, f1, f1, f1\n"
      "fmadd.ps f2, f2, f2, f2\n"
      "fmadd.ps f3, f3, f3, f3\n"
      "fmadd.ps f4, f4, f4, f4\n"
      "fmadd.ps f5, f5, f5, f5\n"
      "fmadd.ps f6, f6, f6, f6\n"
      "fmadd.ps f7, f7, f7, f7\n"
      "fmadd.ps f8, f8, f8, f8\n"
      "fmadd.ps f9, f9, f9, f9\n"
      "fmadd.ps f10, f10, f10, f10\n"
      "fmadd.ps f11, f11, f11, f11\n"
      "fmadd.ps f12, f12, f12, f12\n"
      "fmadd.ps f13, f13, f13, f13\n"
      "fmadd.ps f14, f14, f14, f14\n"
      "fmadd.ps f15, f15, f15, f15\n"
      "fmadd.ps f16, f16, f16, f16\n"
      "fmadd.ps f17, f17, f17, f17\n"
      "fmadd.ps f18, f18, f18, f18\n"
      "fmadd.ps f19, f19, f19, f19\n"
      "fmadd.ps f20, f20, f20, f20\n"
      "fmadd.ps f21, f21, f21, f21\n"
      "fmadd.ps f22, f22, f22, f22\n"
      "fmadd.ps f23, f23, f23, f23\n"
      "fmadd.ps f24, f24, f24, f24\n"
      "fmadd.ps f25, f25, f25, f25\n"
      "fmadd.ps f26, f26, f26, f26\n"
      "fmadd.ps f27, f27, f27, f27\n"
      "fmadd.ps f28, f28, f28, f28\n"
      "fmadd.ps f29, f29, f29, f29\n"
      "fmadd.ps f30, f30, f30, f30\n"
      "fmadd.ps f31, f31, f31, f31\n"
   );

   // End marker
   C_TEST_PASS;
   return 0;
}
