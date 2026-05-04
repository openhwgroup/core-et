/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "et_test_common.h"
#include "minion.h"
#include "macros.h"

/*
 * This test constanly ask for line to the icache
 * WARNING: This tests never finish! it should be finished from the debugger!
 */

#define FILL_NOPS __asm__ volatile (".rep (8*1024)\n"  \
				    "nop\n"  \
				    ".endr\n" \
				   ); \

void foo0();
void foo1();
void foo2();
void foo3();
void foo4();
void foo5();
void foo6();
void foo7();
void foo8();
void foo9();
void fooA();
void fooB();
void fooC();
void fooD();
void fooE();
void fooF();
void return_handler();

void foo0() { foo1(); FILL_NOPS }
void foo1() { foo2(); FILL_NOPS }
void foo2() { foo3(); FILL_NOPS }
void foo3() { foo4(); FILL_NOPS }
void foo4() { foo5(); FILL_NOPS }
void foo5() { foo6(); FILL_NOPS }
void foo6() { foo7(); FILL_NOPS }
void foo7() { foo8(); FILL_NOPS }
void foo8() { foo9(); FILL_NOPS }
void foo9() { fooA(); FILL_NOPS }
void fooA() { fooB(); FILL_NOPS }
void fooB() { fooC(); FILL_NOPS }
void fooC() { fooD(); FILL_NOPS }
void fooD() { fooE(); FILL_NOPS }
void fooE() { fooF(); FILL_NOPS }
void fooF() { foo0(); FILL_NOPS }

__attribute__((section(".pass_handler"))) void return_handler() {
  C_TEST_PASS;
 // __asm__ volatile("1: wfi\n"
//		   "j 1b\n" :::);
		   
}


int main()
{
   C_TEST_START;
   foo0();
   C_TEST_PASS;
}
