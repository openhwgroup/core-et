/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _VERIF_511__H_
#define _VERIF_511__H_

#define CAUSE_USER_ECALL   0x8
#define BASE 0x8080000000

#ifdef __cplusplus
extern "C"
#endif
void   __attribute__ ((aligned (1<<12))) __attribute__ ((interrupt)) trap_handler();

void setup_vmem();
#endif
