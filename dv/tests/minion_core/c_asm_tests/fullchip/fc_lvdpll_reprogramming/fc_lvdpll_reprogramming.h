/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _IPI_FC_
#define _IPI_FC_

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C void  __attribute__ ((interrupt)) irq_handler();
EXTERN_C void  __attribute__ ((interrupt)) bad_trap();
EXTERN_C void mtrap_vector_ipi();
EXTERN_C void dram_rw();

#endif
