/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"
#include "cacheops.h"
#include "ucache_control_bw.h"

#ifdef __cplusplus
extern "C"
#endif

#define NEIGH0_IPI_REDIRECT_PC 0x80100040

#define N_LINES_X_MINION 1024ULL
#define N_LINES_DST 512ULL
#define N_LINES_OP 512ULL
#define N_TIMES 10


#define NUM_MINIONS 32
#define OTHERS_REP_RATE 0
#define OTHERS_MAX_OPS 31

#define BARRIER(x) ((31 << 5) | x)

struct Timer {
  int64_t start;
  int64_t end;
  int64_t total;
  //24 bytes
  //Adding 40 more bytes to ensure that each timer goes to different cache lane
  int64_t lane_padding[5] = {1,2,3,4,5};
};

void initialize_scp() {
     //setup cache op state machine to zero out the SCP region
       __asm__ __volatile__ 
           (
	     "li t0, 0x00000901\n"

	     "li t1, 0x80F00030\n"
	     "sd t0, 0(t1)\n"

	     "li t1, 0x80F02030\n"
	     "sd t0, 0(t1)\n"

	     "li t1, 0x80F04030\n"
	     "sd t0, 0(t1)\n"

	     "li t1, 0x80F06030\n"
	     "sd t0, 0(t1)\n"

	     "fence iorw, iorw\n"
             : : 
	     );
}

void setup_trap_vector() {
     //configure trap vector
       __asm__ __volatile__ 
           (
             "la t0, test_mtrap_vector\n"
             "csrw mtvec, t0\n"
             : : : "t0");
}

inline void waiting_barrier()
{
    if (setFLB(BARRIER(1)) == 1) {
        volatile uint64_t* fcc0 = (uint64_t*)(0x803400C0);
        *fcc0 = 0xffffffff;
    }

    waitFCC(0);
}

inline void waiting_barrier(bool & was_i_last)
{
    if (setFLB(BARRIER(1)) == 1) {
        was_i_last = true;
        volatile uint64_t* fcc0 = (uint64_t*)(0x803400C0);
        *fcc0 = 0xffffffff;
    }
    else
        was_i_last = false;

    waitFCC(0);
}

void timed_sequence(uint64_t minion_id,uint64_t slow_minion_id, uint64_t rep_rate, uint64_t maxops, volatile struct Timer *time){

  
  if(minion_id == slow_minion_id) 
    ucache_control(0x01ULL,rep_rate,maxops);
  else
    ucache_control(0x01ULL,OTHERS_REP_RATE,OTHERS_MAX_OPS);

  uint64_t hart_id = getHartId();
  

  uint64_t shire_id  = ((hart_id>>6) & 0x3F);

  uint64_t VA = ((2U)<<30U)  + (shire_id << 23) +  (minion_id*N_LINES_X_MINION*N_BYTES_CL);
  uint64_t l2_scp_line = (minion_id*N_LINES_OP)+ N_LINES_DST;

  uint64_t conf =  ( (l2_scp_line>>2ULL)   << 48)
                     | VA 
                     | N_LINES_OP;
 
  m_to_u_mode();
  time->start = et_get_cycle_count();
  asm("li x31,64");
  tensorLoadL2SCP(conf, N_BYTES_CL);

  tensorWait(0);
  time->end = et_get_cycle_count();
  to_m_mode();
  

  time->total = time->end - time->start;
} 

/*void check_that_throttled_is_the_slowest(uint64_t throt_minion_id, uint64_t rep_rate, uint64_t others_rep_rate, 
                                         uint64_t max_ops, uint64_t others_max_ops){



}*/

//init to 32 cause only threads 0 of 1 shire are going to use the barrier
long barrier __attribute__((aligned(1024))) = 32;
long *p_barrier=&barrier;

uint64_t rand_minion[NUM_MINIONS] = {26,  22,  6, 18,  1, 30,
                            15,  0,  28,  9,  20,  24,
                            17,  4, 13,  31,  8, 23,
                            10,  21,  9, 3, 27,  23,
                            16,  5, 12,  24,  11,  2,
                            7, 25};  

volatile Timer minion_times[32] __attribute__((aligned(64)));

int main()
{
  initialize_scp();
  setup_trap_vector();
 
  #ifdef ULTRASOC_DEBUG_TEST
  // Wait for the debugger to write this ESR
  volatile uint64_t *esr_poll_value_neigh0;
  esr_poll_value_neigh0 = (volatile uint64_t*) NEIGH0_IPI_REDIRECT_PC; 
  while ( *esr_poll_value_neigh0 != 0xFE );
  #endif

  //only executed on 1 shire -- only thread 0 executing main code
  uint64_t rep_rates_values[8] = {7,7,7,7,7,5,6,7};
  //uint64_t max_op_values[32] = {31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
  uint64_t max_op_values[32] = {31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31};

  uint64_t hart_id = getHartId();
  uint64_t minion_id = ((hart_id>>1) & 0x1F);
  if((hart_id & 1) == 0){
    for(int i=0;i<8;++i){
      for(int j=0;j<32;++j){
        uint64_t slow_minion_id = rand_minion[j];
        if((rep_rates_values[i] != OTHERS_REP_RATE) && (max_op_values[j] != OTHERS_MAX_OPS)){
          
          waiting_barrier();

          timed_sequence(minion_id,slow_minion_id,rep_rates_values[i],max_op_values[j],
                         &minion_times[minion_id]);
          fence();
          __evictVA(&(minion_times[minion_id].start),3,0);
          tensorWait(6);

          bool was_i_last;
          waiting_barrier(was_i_last);

          if (was_i_last) {

            for(uint64_t k=0;k<NUM_MINIONS;++k) {
              if (k != minion_id)
                __evictVA(&(minion_times[k].total),3,0);
            }

            tensorWait(6);

            for(int k=0;k<NUM_MINIONS;++k){
              //et_write_val2(minion_times[k].total);
              if(minion_times[k].total < minion_times[slow_minion_id].total) C_TEST_FAIL; 
            }
            //et_write_val2(((0x17e2)<< 16) | (i*32+j));
           }
           
        }
      }
    }
  tensorWait(6);
  }
  C_TEST_PASS;
  // End marker
  return 0;
}

uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval)
{
  //it's a jump to 'm mode'
   et_write_val2(cause);
   et_write_val2(tval);
    __asm__ __volatile__
      (
       "li t0, 0x1800\n"
       "csrs mstatus, t0\n"
       :
       :
       : "t0" );

    return epc+4;
}
//Same behaviour than the bigger test in mem acces terms but in small scale, nice for Debug
/*  if((hart_id & 1) == {
    minion_times[minion_id].start = minion_id; 
    __evictVA(&(minion_times[minion_id].start),3,0);
    tensorWait(6);
    //if(minion_id==0) initFLB(BARRIER(1));//init barrier 0 for 32 threads
    if(setFLB(BARRIER(1)) == 1){
      for(int k=0;k<NUM_MINIONS;++k){
        et_write_val2(minion_times[k].start);
      }
    }
}*/

