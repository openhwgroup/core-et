/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _COMMON_INTR_
#define _COMMON_INTR_

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"

#define N_BYTES_CL 64ULL
#define N_LONG_CL 8

#define EVICT_L1  0
#define EVICT_L2  1
#define EVICT_L3  2
#define EVICT_MEM 3

#define WFI_INS 0x10500073

#define ULTRASOC_TEST_ESR_AD 0x80100040 // Debug sync targeting neigh 0 ESR
#define ULTRASOC_TEST_ESR_1_AD 0x80110040 // Debug sync targeting neigh 1 ESR
#define ULTRASOC_TEST_ESR_2_AD 0x80120040 // Debug sync targeting neigh 2 ESR
 
inline __attribute__((always_inline)) long getHartId() {
    long  thread;
    __asm__ __volatile__
        (
         "  csrr %[tid], mhartid\n" // software thread
         : [tid] "=r" (thread)
         :
         :
        );
    return thread;
}


inline __attribute__((always_inline)) void tensorStore(long conf, long stride) {
    register long s asm("x31") = stride;
    __asm__ __volatile__
        (
         "  csrw 0x87F,  %[conf]\n"
         :
         : [conf] "r" (conf), "r" (s)
         : "memory"
        );
}

inline __attribute__((always_inline)) void tensorLoadL2SCP(long conf, long stride) {
    register long s asm("x31") = stride;
    __asm__ __volatile__
        (
         "  csrw 0x85F,  %[conf]\n"
         :
         : [conf] "r" (conf), "r" (s)
         : "memory"
        );
}

inline __attribute__((always_inline)) void tensorLoadL2SCP(long conf, long stride,long id) {
    register long s asm("x31") = stride | id;
    __asm__ __volatile__
        (
         "  csrw 0x85F,  %[conf]\n"
         :
         : [conf] "r" (conf), "r" (s)
         : "memory"
        );
}

inline __attribute__((always_inline)) void tensorLoad(long conf, long stride, long id) {
    register long s asm("x31") = stride | id;
    __asm__ __volatile__
        (
         "  csrw 0x83F,  %[conf]\n"
         :
         : [conf] "r" (conf), "r" (s)
         : "memory"
        );
}

inline __attribute__((always_inline)) void _reduce(long conf) {
    __asm__ __volatile__
        (
         "  csrw 0x800,  %[conf]\n"
         :
         : [conf] "r" (conf)
         : "memory"
        );
}

inline __attribute__((always_inline)) void tensorWait(long id) {
    __asm__ __volatile__
        (
         " csrw 0x830, %[id]\n"
         :
         : [id] "r" (id)
         : "memory"
        );
}

inline __attribute__((always_inline)) void prefetch(long conf, long stride, long id) {
    register long s asm("x31") = stride | id;
    __asm__ __volatile__
        (
         "  csrw 0x81F,  %[conf]\n"
         :
         : [conf] "r" (conf) , "r" (s)
         : "memory"
        );
}

inline __attribute__((always_inline)) void configure_cacheop(long conf) {
    __asm__ __volatile__
        (
         "  csrw 0x810,  %[conf]\n"
         :
         : [conf] "r" (conf)
         : "memory"
        );
}

inline __attribute__((always_inline)) void tensor_conv_size_conf(long srow, long nrow, long scol, long ncol) {

   uint64_t conf = srow << 56 | //8 bits
                   nrow << 32 | //16 bits
                   scol << 24 | //8 bits
                   ncol;        //16 bits
    __asm__ __volatile__
        (
         "  csrw 0x802,  %[conf]\n"
         :
         : [conf] "r" (conf)
         : "memory"
        );
}

inline __attribute__((always_inline)) void tensor_conv_ctrl_conf(long rowstart,long colstart) {
   
    uint64_t conf = rowstart << 32 | //16 bits
                    colstart;        //16 bits

    __asm__ __volatile__
        (
         "  csrw 0x803,  %[conf]\n"
         :
         : [conf] "r" (conf)
         : "memory"
        );
}

inline __attribute__((always_inline)) void __cacheOpWait() {
    __asm__ __volatile__
        (
         "  csrw 0x830,  6\n"
         :
         : 
         : "memory"
        );
}

inline __attribute__((always_inline)) void fence() {
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
}

inline __attribute__((always_inline)) void wfi() {
    __asm__ __volatile__
        (
         "wfi\n"
         : 
         : 
         : "memory"
        );
}

inline __attribute__((always_inline)) void store(long addr, long value) {

    __asm__ __volatile__
        (
         "  sd %[value], 0(%[addr])\n"
         :
         : [addr] "r" (addr), [value] "r" (value)
         : "memory"
        );
}

inline __attribute__((always_inline)) void load(long addr,long value) {

    __asm__ __volatile__
        (
         "  ld %[value], 0(%[addr])\n"
         :[value] "+&r" (value)
         :[addr] "r" (addr)         
         :"memory"
        );
}

inline __attribute__((always_inline)) void __evictVA(volatile void *addr,long hierarchy_level,long num_lines) { // real num_lines - 1
    long mask=0;
    mask += (long(addr) & ~(0x3F));
    mask += hierarchy_level<<58;
    mask +=num_lines;

    __asm__ __volatile__
        (
         " csrrw x0, 0x89f, %[mask]\n"
         :
         : [mask] "r" (mask)
         : "memory"
        );
}

inline __attribute__((always_inline)) void __evictVA(long *addr,long hierarchy_level,long num_lines) { // real num_lines - 1
    long mask=0;
    mask += (long(addr) & ~(0x3F));
    mask += hierarchy_level<<58;
    mask +=num_lines;

    __asm__ __volatile__
        (
         " csrrw x0, 0x89f, %[mask]\n"
         :
         : [mask] "r" (mask)
         : "memory"
        );
}

inline __attribute__((always_inline)) void __syncthreads(long *token) {
    
    long mask=0;
    mask += (long(token) & ~(0x3F));
    mask += 3ULL<<58;//2 L3 -- 3 RAM
    mask +=0;

    
    __asm__ __volatile__
        (
         " csrrw x0, 0x89f, %[mask]\n"//evict lane to L3 as sync point(no coherency)
         " fence \n"
         " li x31,-1\n"
         " amoaddg.d x31,x31,(%[token])\n"
         " csrrw x0, 0x89f, %[mask]\n"//evict lane to L3 as sync point(no coherency)
         " fence \n"
         " watchdog%=: \n"
         //".rept 10000\n"
         //" nop\n"
         //".endr\n"
         " amoorg.d x31,zero,0(%[token])\n"
         " fence \n"
         " csrrw x0, 0x89f, %[mask]\n"
         " fence \n"
         " bne x31,zero, watchdog%=\n"
         :
         : [token] "r" (token), [mask] "r" (mask)
         : "memory"
        );
    
}

inline __attribute__((always_inline)) void shg(long addr, uint16_t value) {

    __asm__ __volatile__
        (
         "  shg %[value], (%[addr])\n"
         :
         : [addr] "r" (addr), [value] "r" (value)
         : "memory"
        );
}


inline __attribute__((always_inline)) void waitFCC(long cnt_id) {
    __asm__ volatile
      (
       "csrrw x0, 0x821, %[id]\n"
       :
       : [id] "r" (cnt_id)
       :
      );
}

inline __attribute__((always_inline)) long read(long addr) {
  long val;
  __asm__ __volatile__
    (
     "ld %[val], 0(%[addr])\n"
     : [val] "=r" (val)
     : [addr] "r" (addr)
     : "memory"
    );
  return val;
}

inline __attribute__((always_inline)) void setUpPort(long addrport, long configport) {
 
  __asm__ __volatile__
    (
     "csrw %[port],%[confport]\n"
     :
     : [port] "r" (addrport),
       [confport] "r" (configport)
     : "memory"
    );
}

inline __attribute__((always_inline)) void writeTensorMask(long value) {
 
  __asm__ __volatile__
    (
     "csrw 0x805,%[value]\n"
     :
     : [value] "r" (value)
     : "memory"
    );
}

inline __attribute__((always_inline)) void writeConvCtrl(long value) {
 
  __asm__ __volatile__
    (
     "csrw 0x803,%[value]\n"
     :
     : [value] "r" (value)
     : "memory"
    );
}

inline __attribute__((always_inline)) long readTensorMask() {
    long  value;
    __asm__ __volatile__
        (
         "  csrr %[value], 0x805\n" // software thread
         : [value] "=r" (value)
         :
         :
        );
    return value;
}
inline __attribute__((always_inline)) void writeConvolutionSize(long value) {
 
  __asm__ __volatile__
    (
     "csrw 0x802,%[value]\n"
     :
     : [value] "r" (value)
     : "memory"
    );
}



inline __attribute__((always_inline)) void readPort(long addrport) {
 
  // 0xcc8  i 0<i<3 for blocking accesses
  __asm__ __volatile__
    (
     "csrr 0x0, %[port]\n"
     :
     : [port] "r" (addrport)
     : "memory"
    );
}



inline __attribute__((always_inline)) void setM0MaskFF() {
  __asm__ __volatile__
    (
     "mov.m.x m0, zero, 0xff\n"
     :
     : 
     : "memory"
    );
}
inline __attribute__((always_inline)) void initFRegs() {
 
  __asm__ __volatile__
    (
     "fbci.ps f0,0\n"
     "fbci.ps f1,1\n"
     "fbci.ps f2,2\n"
     "fbci.ps f3,3\n"
     "fbci.ps f4,4\n"
     "fbci.ps f5,5\n"
     "fbci.ps f6,6\n"
     "fbci.ps f7,7\n"
     "fbci.ps f8,8\n"
     "fbci.ps f9,9\n"
     "fbci.ps f10,10\n"
     "fbci.ps f11,11\n"
     "fbci.ps f12,12\n"
     "fbci.ps f13,13\n"
     "fbci.ps f14,14\n"
     "fbci.ps f15,15\n"
     "fbci.ps f16,16\n"
     "fbci.ps f17,17\n"
     "fbci.ps f18,18\n"
     "fbci.ps f19,19\n"
     "fbci.ps f20,20\n"
     "fbci.ps f21,21\n"
     "fbci.ps f22,22\n"
     "fbci.ps f23,23\n"
     "fbci.ps f24,24\n"
     "fbci.ps f25,25\n"
     "fbci.ps f26,26\n"
     "fbci.ps f27,27\n"
     "fbci.ps f28,28\n"
     "fbci.ps f29,29\n"
     "fbci.ps f30,30\n"
     "fbci.ps f31,31\n"
     :
     : 
     : "memory"
    );
}


inline __attribute__((always_inline)) void initFRegs_0() {
 
  __asm__ __volatile__
    (
     "fbci.ps  f0,0\n"
     "fbci.ps  f1,0\n"
     "fbci.ps  f2,0\n"
     "fbci.ps  f3,0\n"
     "fbci.ps  f4,0\n"
     "fbci.ps  f5,0\n"
     "fbci.ps  f6,0\n"
     "fbci.ps  f7,0\n"
     "fbci.ps  f8,0\n"
     "fbci.ps  f9,0\n"
     "fbci.ps f10,0\n"
     "fbci.ps f11,0\n"
     "fbci.ps f12,0\n"
     "fbci.ps f13,0\n"
     "fbci.ps f14,0\n"
     "fbci.ps f15,0\n"
     "fbci.ps f16,0\n"
     "fbci.ps f17,0\n"
     "fbci.ps f18,0\n"
     "fbci.ps f19,0\n"
     "fbci.ps f20,0\n"
     "fbci.ps f21,0\n"
     "fbci.ps f22,0\n"
     "fbci.ps f23,0\n"
     "fbci.ps f24,0\n"
     "fbci.ps f25,0\n"
     "fbci.ps f26,0\n"
     "fbci.ps f27,0\n"
     "fbci.ps f28,0\n"
     "fbci.ps f29,0\n"
     "fbci.ps f30,0\n"
     "fbci.ps f31,0\n"
     :
     : 
     : "memory"
    );
}

inline __attribute__((always_inline)) void initFRegs_2(long value) {
 
  __asm__ __volatile__
    (
     "add x5, zero, %[value]\n"   
     "fbcx.ps f0,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f1,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f2,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f3,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f4,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f5,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f6,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f7,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f8,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f9,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f10,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f11,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f12,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f13,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f14,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f15,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f16,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f17,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f18,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f19,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f20,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f21,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f22,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f23,x5\n"    
     "addi x5, x5, 1\n"  
     "fbcx.ps f24,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f25,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f26,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f27,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f28,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f29,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f30,x5\n"   
     "addi x5, x5, 1\n"  
     "fbcx.ps f31,x5\n"  
     :
     : [value] "r" (value)
     : "x5", "memory"
    );
}

inline __attribute__((always_inline)) void setFRegs_2(long value) {

 __asm__ __volatile__
   (
    "add x5, zero, %[value]\n"
    "fbcx.ps f0,x5\n"
    "fbcx.ps f1,x5\n"
    "fbcx.ps f2,x5\n"
    "fbcx.ps f3,x5\n"
    "fbcx.ps f4,x5\n"
    "fbcx.ps f5,x5\n"
    "fbcx.ps f6,x5\n"
    "fbcx.ps f7,x5\n"
    "fbcx.ps f8,x5\n"
    "fbcx.ps f9,x5\n"
    "fbcx.ps f10,x5\n"
    "fbcx.ps f11,x5\n"
    "fbcx.ps f12,x5\n"
    "fbcx.ps f13,x5\n"
    "fbcx.ps f14,x5\n"
    "fbcx.ps f15,x5\n"
    "fbcx.ps f16,x5\n"
    "fbcx.ps f17,x5\n"
    "fbcx.ps f18,x5\n"
    "fbcx.ps f19,x5\n"
    "fbcx.ps f20,x5\n"
    "fbcx.ps f21,x5\n"
    "fbcx.ps f22,x5\n"
    "fbcx.ps f23,x5\n"
    "fbcx.ps f24,x5\n"
    "fbcx.ps f25,x5\n"
    "fbcx.ps f26,x5\n"
    "fbcx.ps f27,x5\n"
    "fbcx.ps f28,x5\n"
    "fbcx.ps f29,x5\n"
    "fbcx.ps f30,x5\n"
    "fbcx.ps f31,x5\n"
    :
    : [value] "r" (value)
    : "x5", "memory"
   );
}


inline __attribute__((always_inline)) void forceDelay(void) {

  __asm__ volatile
    (
     "nop\n"
     "nop\n"
     "nop\n"
     "nop\n"
     "nop\n"     
    );
}

inline __attribute__((always_inline)) void initFLB(long counter) {
  __asm__ volatile
    (
     "csrw 0x820, %[counter]"
     :
     :[counter] "r" (counter)
     :
    );
}

inline __attribute__((always_inline)) long setFLB(long maxValueAndCnt) {
  long result;
  __asm__ volatile
    (
     "csrrw %0, 0x820, %1\n"
     : "=r"(result)
     : "r"(maxValueAndCnt)
     :
    );
  return result;
}

inline __attribute__((always_inline)) void evict_sw(long value) {

  __asm__ volatile
  (
   "csrw 0x7F9, %[value]\n"
   :
   :[value] "r" (value)
   :"memory"
  );
}

inline void __attribute__((always_inline)) evict_sw_no_mask(uint64_t use_tmask, uint64_t dst, uint64_t way, uint64_t set, uint64_t num_lines) {
   uint64_t csr_enc = ((use_tmask & 1) << 63) |
                      ((dst & 0x3) << 58)     |
                      ((set & 0xF) << 14)     |
                      ((way & 0x3) << 6)      |
                      ((num_lines & 0xF));

   __asm__ __volatile__ ( "csrw 0x7f9, %[csr_enc]\n" : : [csr_enc] "r" (csr_enc));

}

inline __attribute__((always_inline)) void lock_sw(long value) {

  __asm__ volatile
  (
   "csrw 0x7FD, %[value]\n"
   :
   :[value] "r" (value)
   :"memory"
  );
}

// Fuctions to print cycles without stdlib
//int get_str_len (const char* s){
//    int len = 0;
//    while(s[len++] != 0);
//    return len;
//}

void int_to_char(char *dst, int num){
  char tmp_buffer[20];
  int pos = 0;
  int i;

  // Deal with negative values
  if (num < 0) {
    *(dst++)='-';
    num=-num;
  }

  do {
    tmp_buffer[pos] = '0' + num % 10;
    num = num/10;
    pos++;
  } while(num!=0);

  for (i=0;i<pos;i++){
    dst[i]=tmp_buffer[pos-i-1];
  }

  dst[i]='\n';
  dst[i+1]='\0';
}

//void print_cycles (uint64_t cycles){
//
//    char str[50] = "Measured time: ";
//    int pos = get_str_len(str);
//    int_to_char(&str[pos-1], cycles);
//    et_printf(str);
//}

void print_number (uint64_t num){
     
    char str[50] = "";
    int_to_char(&str[0], num);  
    et_printf(str);
}

/*#ifdef __cplusplus
extern "C"
#endif

void __attribute__ ((aligned (1<<12))) __attribute__ ((interrupt)) trap_handler() {
  uint64_t mcause, mtval;

  __asm__ __volatile__
  (
    "csrr %[c], mcause\n"
    "csrr %[v], mtval\n"
    : [c] "=r" (mcause), [v] "=r" (mtval)
  );

  if (mcause == 2 && mtval == WFI_INS) {
    C_TEST_PASS;
  }
  else {
    C_TEST_FAIL;
  }
}


void change_to_user_mode (){
  // register long s asm("t0") = 0;  unused variable

  // configure trap vector and move to user mode
    __asm__ __volatile__
      (
       "la t0, trap_handler\n"
       "csrw mtvec, t0\n"
       // Configure satp to use physical addresses 
       "li t0, 0xF000000000000000\n" // Mode Sv48
       "csrrc x0, satp, t0\n"
       "li t0, 0x1800\n"
       "csrrc x0, mstatus, t0\n"  // clear mstatus.mpp
       "la t0, 1f\n"
       "csrw mepc, t0\n"          // set mepc to user-mode entry point
       "mret\n"                   // go to user mode
       "1:\n"                     // label to referr to user-mode
       :
       :
       : "t0"
       );
}*/


#endif
