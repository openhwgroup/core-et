/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "cpu.h" 
#include "tb.h" 
#include "soc.h" 
#include "macros.h" 
#include "plic_api.h"
#include "fcc.h"
#include "esr.h"
#include <inttypes.h>
#include "macros.h"
#include "et_test_common.h"
#include "minion.h"

#define MASTER 32

int main()
{
  uint64_t*   baseAddr =  (uint64_t*) R_PU_MBOX_MM_SP_BASEADDR;

  C_TEST_START; 

  volatile uint64_t hart_id = get_hart_id();

  WFI

  C_TEST_PASS;
}