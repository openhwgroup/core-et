/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef __MACROS
#define __MACROS

#ifdef __ASSEMBLER__ // when included in an assembly file

.include "macros.s"; // assembly test macros for timers

#ifdef POSTSI_UST
    //this is Maxion, no validation registers in silicon
    #if defined CPU && CPU==2
        #pragma message "POSTSI and Maxion detected ASM_TEST_START (you can ignore this message)"
        #define ASM_TEST_START \
           fence; \
           TIMER_START; \
           lui x31, 0xDEAD0; \
           csrw mscratch, x31;
    #else
        // Write to validation0 (0x8d0) and validation2 (0x8d2)
        #define ASM_TEST_START \
           fence; \
           TIMER_END; \
           lui x31, 0xDEAD0; \
           csrw 0x8d0, x31; \
           csrwi 0x8d2, 0;
    #endif
#else
    // Write to validation0 (0x8d0)
    #define ASM_TEST_START \
       fence; \
       TIMER_START; \
       lui  a7, 0xDEAD0; \
       csrw 0x8d0, a7;
#endif

#ifdef POSTSI_UST
    #if defined CPU && CPU==2
        #pragma message "POSTSI and Maxion detected ASM_TEST_PASS (you can ignore this message)"
        #define ASM_TEST_PASS           \
            fence;                      \
            lui x31, 0x1FEED;           \
            TIMER_END;                  \
            lui t0, 0x30000;            \
            li t5,0; /* PASS */         \
          wait_for_ready:               \
            nop;        \
            lbu t3, 0(t0);              \
            bnez t3, wait_for_ready;    \
            sw t5, 4(t0);               \
            li t4, 3;                   \
            sb t4, 0(t0);               \
            fence;                      \
            lui t0, 0x30004;            \
            li t4, -1;                  \
            sw t4, 0(t0);               \
            csrw mscratch, x31;         \
          postsi_ust_test_end:          \
            nop;                        \
            1: wfi;                     \
            j 1b;
    #else
        #define ASM_TEST_PASS \
            fence ;\
            lui x31, 0x1FEED; \
            TIMER_END;  \
            csrwi validation2, 1; \
            call postsi_ust_test_end; \
        postsi_ust_test_end: \
            nop ; \
            nop ; \
            wfi ;
    #endif
#else
    // Write to validation0 (0x8d0)
    #define ASM_TEST_PASS \
       fence	   ;\
       lui a7, 0x1FEED; \
       TIMER_END;  \
       csrw 0x8d0, a7; \
       1:wfi; \
       j 1b;
#endif

// Write to validation0 (0x8d0)
#define ASM_TEST_PASS_CSTALL \
   fence	   ;\
   lui a7, 0x1FEED; \
   TIMER_END;  \
   csrw 0x8d0, a7; \
   csrw stall, zero; \

#ifdef POSTSI_UST
    #if defined CPU && CPU==2
       #pragma message "POSTSI and Maxion detected ASM_TEST_FAIL (you can ignore this message)"
       #define ASM_TEST_FAIL \
          fence;                    \
          lui x31, 0x50BAD;         \
          lui t0, 0x30000;          \
          li t5,1; /* FAIL */       \
        wait_for_ready_2:           \
          nop;                      \
          lbu t3, 0(t0);            \
          bnez t3, wait_for_ready_2;\
          sw t5, 4(t0);         \
          li t4, 3;             \
          sb t4, 0(t0);         \
          fence;                \
          lui t0, 0x30004;      \
          li t4, -1;            \
          sw t4, 0(t0);         \
          csrw mscratch, x31;         \
          call postsi_ust_test_end;
    #else
       #define ASM_TEST_FAIL \
          fence; \
          lui x31, 0x50BAD; \
          csrwi validation2, 1; \
          call postsi_ust_test_end;
    #endif
#else
    // Write to validation0 (0x8d0)
    #define ASM_TEST_FAIL \
       fence	   ;\
       lui a7, 0x50BAD; \
       csrw 0x8d0, a7; \
       wfi;
#endif

// Write to validation0 (0x8d0)
#define ASM_TEST_FAIL_CSTALL \
   fence	   ;\
   lui a7, 0x50BAD; \
   csrw 0x8d0, a7; \
   csrw stall, zero; \

#define ASM_TEST_PASS_MAX \
   fence	   ;\
   li a0, 2;\
   li a1, 0;\
   jal tb_write;\
   wfi; \

#define FENCE fence;

#define WAIT_TENSOR_LOAD_0     csrwi 0x830, 0;
#define WAIT_TENSOR_LOAD_1     csrwi 0x830, 1;
#define WAIT_TENSOR_LOAD_L2_0  csrwi 0x830, 2;
#define WAIT_TENSOR_LOAD_L2_1  csrwi 0x830, 3;
#define WAIT_PREFETCH_0        csrwi 0x830, 4;
#define WAIT_PREFETCH_1        csrwi 0x830, 5;
#define WAIT_CACHEOPS          csrwi 0x830, 6;
#define WAIT_TENSOR_FMA        csrwi 0x830, 7;
#define WAIT_TENSOR_STORE      csrwi 0x830, 8;
#define WAIT_TENSOR_REDUCE     csrwi 0x830, 9;
#define WAIT_TENSOR_QUANT      csrwi 0x830, 10;

#else // __ASSEMBLER__ --> when included in a C/C++ file

asm(".include \"macros.s\""); // assembly test macros for timers

#if defined CPU && CPU==2
#pragma message "CPU is defined!"
#endif

#if defined POSTSI_UST
    //this is Maxion, no validation registers in silicon
    #if defined CPU && CPU==2
    #pragma message "POSTSI and Maxion detected C_TEST_START (you can ignore this message)"
        #define C_TEST_START \
           __asm__ __volatile__ ( \
                 "fence\n" \
                 "TIMER_START\n" \
                 "lui x31, 0xDEAD0\n" \
                 "csrw mscratch, x31\n" \
                 : : : "x31");

    #else
        #ifdef TEST_IN_SMODE
        // Write to validation0 (0x8d0) and validation2 (0x8d2)
        #define C_TEST_START \
           __asm__ __volatile__ ( \
                 "fence\n" \
                 "TIMER_START\n" \
                 "lui  x31, 0xDEAD0\n" \
                 "csrw 0x8d0, x31\n" \
                 "csrwi 0x8d2, 0\n" \
                 "call switchToSmode\n" \
                 : : : "x31");
        #else
        // Write to validation0 (0x8d0) and validation2 (0x8d2)
        #define C_TEST_START \
           __asm__ __volatile__ ( \
                 "fence\n" \
                 "TIMER_START\n" \
                 "lui  x31, 0xDEAD0\n" \
                 "csrw 0x8d0, x31\n" \
                 "csrwi 0x8d2, 0\n" \
                 : : : "x31");
        #endif
    #endif
#else
    // Write to validation0 (0x8d0)
    #define C_TEST_START      \
       __asm__ __volatile__ ( \
             "fence\n"\
             "TIMER_START\n" \
             "lui  a7, 0xDEAD0\n" \
             "csrw 0x8d0, a7\n" \
             : : : "a7");
#endif

#if defined IOSHIRE_GATESIM

#define C_TEST_PASS       \
   __asm__ __volatile__ ( \
         "fence\n" \
         "TIMER_END\n" \
         "li a7, 0x8100000000\n" \
         "sd a0, (a7)\n" \
         "1:wfi\n" \
         "j 1b\n" \
         : : : "a7");

#elif defined POSTSI_UST
    #if defined CPU && CPU==2
        #pragma message "POSTSI and Maxion detected C_TEST_PASS (you can ignore this message)"
        #define C_TEST_PASS       \
            __asm__ __volatile__ (   \
                 "fence\n" \
                 "TIMER_END\n" \
                 "lui x31, 0x1FEED\n" \
                 "csrw mscratch, x31\n" \
                 "postsi_ust_test_end: nop\n" \
                 "1: wfi\n" \
                 "j 1b\n" \
                 : : : "x31");

    #else

        #ifdef TEST_IN_SMODE     
        // Write to validation0 (0x8d0) and validation2 (0x8d2)
        #define C_TEST_PASS       \
            __asm__ __volatile__ (                      \
                 "fence\n" \
                 "call switchToMmode\n" \
                 "TIMER_END\n" \
                 "lui x31, 0x1FEED\n" \
                 "csrwi 0x8d2, 1\n" \
                 "call postsi_ust_test_end\n" \
                 "postsi_ust_test_end:\n" \
                 "nop\n" \
                 "csrw 0x8d0, x31\n" \
                 "1: wfi\n" \
                 "j 1b\n" \
                 : : : "x31");
        #else
        // Write to validation0 (0x8d0) and validation2 (0x8d2)
        #define C_TEST_PASS       \
            __asm__ __volatile__ (                      \
                 "fence\n" \
                 "TIMER_END\n" \
                 "lui x31, 0x1FEED\n" \
                 "csrwi 0x8d2, 1\n" \
                 "call postsi_ust_test_end\n" \
                 "postsi_ust_test_end:\n" \
                 "nop\n" \
                 "csrw 0x8d0, x31\n" \
                 "1: wfi\n" \
                 "j 1b\n" \
                 : : : "x31");
        #endif 
    #endif
#else
    // Write to validation0 (0x8d0)
    #define C_TEST_PASS       \
       __asm__ __volatile__ ( \
             "fence\n" \
             "TIMER_END\n" \
             "lui a7, 0x1FEED\n" \
             "csrw 0x8d0, a7\n" \
             "1:wfi\n" \
             "j 1b\n" \
             : : : "a7");
#endif

#if defined POSTSI_UST
    #if defined CPU && CPU==2
    #pragma message "POSTSI and Maxion detected C_TEST_FAIL (you can ignore this message)"
    #define C_TEST_FAIL \
       __asm__ __volatile__ ( \
             "fence\n" \
             "lui x31, 0x50BAD\n"          \
             "csrw mscratch, x31\n" \
             "call postsi_ust_test_end\n" \
             : : : "x31");

    #else
    #ifdef TEST_IN_SMODE
    #define C_TEST_FAIL \
       __asm__ __volatile__ ( \
             "fence\n" \
             "call switchToMmode\n" \
             "lui x31, 0x50BAD\n" \
             "csrwi validation2, 1\n" \
             "call postsi_ust_test_end\n" \
             : : : "x31");
    #else
    #define C_TEST_FAIL \
       __asm__ __volatile__ ( \
             "fence\n" \
             "lui x31, 0x50BAD\n" \
             "csrwi validation2, 1\n" \
             "call postsi_ust_test_end\n" \
             : : : "x31");
    #endif
    #endif
#else
    // Write to validation0 (0x8d0)
    #define C_TEST_FAIL \
       __asm__ __volatile__ ( \
             "fence\n"\
             "lui a7, 0x50BAD\n" \
             "csrw 0x8d0, a7\n" \
             "wfi\n" \
             : : : "a7");
#endif

#define NOP  __asm__ __volatile__ ("nop\n");
#define FENCE __asm__ __volatile__ ("fence\n");
#define WFI __asm__ __volatile__ ("wfi\n");
#define WAIT_TENSOR_LOAD_0     __asm__ __volatile__ ( "csrwi 0x830, 0\n" : : );
#define WAIT_TENSOR_LOAD_1     __asm__ __volatile__ ( "csrwi 0x830, 1\n" : : );
#define WAIT_TENSOR_LOAD_L2_0  __asm__ __volatile__ ( "csrwi 0x830, 2\n" : : );
#define WAIT_TENSOR_LOAD_L2_1  __asm__ __volatile__ ( "csrwi 0x830, 3\n" : : );
#define WAIT_PREFETCH_0        __asm__ __volatile__ ( "csrwi 0x830, 4\n" : : );
#define WAIT_PREFETCH_1        __asm__ __volatile__ ( "csrwi 0x830, 5\n" : : );
#define WAIT_CACHEOPS          __asm__ __volatile__ ( "csrwi 0x830, 6\n" : : );
#define WAIT_TENSOR_FMA        __asm__ __volatile__ ( "csrwi 0x830, 7\n" : : );
#define WAIT_TENSOR_STORE      __asm__ __volatile__ ( "csrwi 0x830, 8\n" : : );
#define WAIT_TENSOR_REDUCE     __asm__ __volatile__ ( "csrwi 0x830, 9\n" : : );
#define WAIT_TENSOR_QUANT      __asm__ __volatile__ ( "csrwi 0x830, 10\n" : : );
#define STALL                  __asm__ __volatile__ ( "csrw stall, x0\n" : : );
#define CLEAR_TENSOR_ERROR     __asm__ __volatile__ ( "csrwi 0x808, 0" : : );
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

#endif // __ASSEMBLER__

#endif // ! __MACROS
