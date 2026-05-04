/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "cpu.h"
#include "soc.h"
#include "macros.h"
#include "tb.h"
//#include "spy.h"

/* TT Test Function */
et_status_t tt_TestFunction( void )
{
    #ifndef POSTSI_UST
      uint32_t maxionCore;
      uint32_t from;
      uint32_t to;
      from = IOMAX0;
      to   = IOMAX3;
      maxionCore = tb_getRandom( "RANGE %0d %0d", from, to );
      soc_releaseCpuReset(maxionCore);
      printDbg("\t\tMaxion %0d has started", maxionCore);
    #endif

    return ET_OK;

} /* tt_Test() */

/*****     < EOF >     *****/
