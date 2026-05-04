/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "minion.h"


//#define IPI_CYCLES 10000
#define IPI_CYCLES 8000
#define INTER_LAYER_DELAY 300
#define WRITE_TO_IPI_GUARD 200
#define CONTROL_SHIRE_PHY 13
#define CONTROL_SHIRE_VIRT 32

typedef struct{
  uint64_t compute_pc;
  uint64_t helper_pc;
  uint64_t shire_mask;
  uint64_t minion_mask;
  uint64_t id;
  uint64_t tensor_a;
  uint64_t tensor_b;
  uint64_t tensor_c;
  uint64_t tensor_d;
  uint64_t tensor_e;
  uint64_t compute_size;
  uint64_t helper_size;
  uint64_t info_pointer;
} net_desc_t;

extern uint64_t _end;


void  delay_cycles(uint32_t cycles){
  for ( uint32_t i = 0; i < cycles / 8; i++) // coarse approximation, each iteration of the loop takes 8 cycles
    __asm__ __volatile__ ( "nop" );
}

#define SCP_CONF_CSR 0x51f

// only to be executed by the control shire
int main()
{
    char port_buf[16*4]; // FUTURE: Emmanuel Marie mida

    C_TEST_START;

    if (get_hart_id() % 64 > 0) { // stop all threads except for 0
      __asm__ __volatile__ ( "wfi\n");
    }

    // get data from net description
    uint32_t count = _end;
    net_desc_t *net_desc = (net_desc_t*) ( &_end + 1);

    //  lock set0, way0 in dcache
    intptr_t scp_conf = (intptr_t) port_buf;
    unsigned scp_set = (scp_conf >> 6) & 0xF;
    scp_conf &= ~0x3F; //remove offset
    scp_conf &= (1L<<56)-1; //set MSB to 0 (including op=lock)
    unsigned nr_sets = 1;  //16*4 bytes= 64 bytes = 1 cache line
  __asm__ __volatile__
    (
     "loop_lock:\n"
     "csrw %[csr], %[scp_conf]\n"
     "addi %[scp_conf], %[scp_conf], 64\n"
     "addi %[nr_sets], %[nr_sets], -1\n"
     "bne %[nr_sets], zero, loop_lock\n"
     : [scp_conf] "+r" (scp_conf), [nr_sets] "+r" (nr_sets)
     : [csr] "i" (SCP_CONF_CSR)
     :
     );

    // open port 0
    __asm__ __volatile__
      (
       /* FUTURE: Emmanuel Marie STEP 3
          --> This thread changes to supervisor mode (it is in machine mode)
          --> This thread opens a port to listen for shire responses
          --> Enable       = 1'b1
          --> out of band  = 1'b1
          --> reserved     = 2'b00
          --> user mode    = 1'b1
          --> log msg size = 3'b010
          --> max msgs     = 4'b1111
          --> reserved     = 4'b0000
          --> SCP set      = 8'h[scp_set]
          --> SCP way      = 8'h00
          --> reserved     = 32'h0000_0000
       */
       "li a3,0x00000F53\n"
       "slli a2,%[scp_set],16\n"
       "or a3, a3, a2\n"
       "csrw 0x9cc,a3\n" //Open port 0
       :
       : [scp_set] "r" (scp_set)
       : "a2","a3"
       );

    // and loop
    uint32_t delay = IPI_CYCLES;

    for (uint32_t iter = 0; iter < count; iter++) {


      // compute thread mask
      uint64_t thread_mask = 0;
      uint32_t minion_mask = net_desc[iter].minion_mask;
      for (int min = 0; min < 32; ++min) {
        if ( (minion_mask & 1) != 0)
          thread_mask |= 3 << (2*min);
        minion_mask >>=1;
      }

      delay_cycles(delay - WRITE_TO_IPI_GUARD);
      delay = INTER_LAYER_DELAY;

      // write configuration
      __asm__ __volatile__
        (
         // write data
         "sd %[tensor_a], 0(%[info_pointer])\n"
         "sd %[tensor_b], 8(%[info_pointer])\n"
         "sd %[tensor_c], 16(%[info_pointer])\n"
         "sd %[tensor_d], 24(%[info_pointer])\n"
         "sd %[tensor_e], 32(%[info_pointer])\n"
         "sd %[compute_pc], 40(%[info_pointer])\n"
         "sd %[helper_pc], 48(%[info_pointer])\n"
         "sd %[compute_size], 56(%[info_pointer])\n"
         "sd %[helper_size], 60(%[info_pointer])\n"
         :
         :
         [compute_pc]   "r" (net_desc[iter].compute_pc),
         [helper_pc]    "r" (net_desc[iter].helper_pc),
         [tensor_a]     "r" (net_desc[iter].tensor_a),
         [tensor_b]     "r" (net_desc[iter].tensor_b),
         [tensor_c]     "r" (net_desc[iter].tensor_c),
         [tensor_d]     "r" (net_desc[iter].tensor_d),
         [tensor_e]     "r" (net_desc[iter].tensor_e),
         [compute_size] "r" (net_desc[iter].compute_size),
         [helper_size]  "r" (net_desc[iter].helper_size),
         [info_pointer] "r" (net_desc[iter].info_pointer)
         :
         );

#ifdef NEW_CACHE_OPTS
      __asm__ __volatile__
        (
         // evict to L3
         // Perform evict cacheop --> Not following the official spec, using emu.cpp implementation
         // op_value[63] --> tensor mask --> 1'b1
         // op_value[62:60] --> operation --> 3'b111
         // op_value[59:58] --> destination --> 2'b10
         // op_value[57:56] --> start --> 2'b00
         // op_value[55:4] --> addr --> info pointer from the generated net_desc.txt
         // op_value[3:0] --> Repeat --> 4'b1111
         "li a3, 0xFA000000000000F\n"
         "or a3, a3, %[info_pointer]\n"
         "csrw 0x89F,a3\n"
         :
         : [info_pointer] "r" (net_desc[iter].info_pointer)
         : "a3"
         );
#else
            __asm__ __volatile__
        (
         // evict to L3
         // Perform evict cacheop --> Not following the official spec, using emu.cpp implementation
         // op_value[63] --> tensor mask --> 1'b1
         // op_value[62:60] --> operation --> 3'b111
         // op_value[59:58] --> destination --> 2'b10
         // op_value[57:56] --> start --> 2'b00
         // op_value[55:4] --> addr --> info pointer from the generated net_desc.txt
         // op_value[3:0] --> Repeat --> 4'b1111
         "li a3, 0x7800000000000000\n"
         "or a3, a3, %[info_pointer]\n"
         "csrw 0x81F,a3\n"
         :
         : [info_pointer] "r" (net_desc[iter].info_pointer)
         : "a3"
         );

#endif

      __asm__ __volatile__
        (
         // Wait until previous ends (tensor wait) --> Following official spec
         // op_value[63:4] --> anything --> '0
         // op_value[3:0] --> wait for cacheOp (6)--> 3'b110
         "li a3,0x06\n"
         "csrwi 0x830,6\n"
         ::
         : "a3"
        );

      // Set PC for thread 0 and also trigger those threads
      uint32_t shire_mask = net_desc[iter].shire_mask;
      unsigned active_shires = 0;
      bool active;
      int shire;
      for ( shire = 0 ; shire < 34; shire++) {
        if ( shire == 32) continue;
        active = shire_mask % 2 != 0;
        shire_mask>>=1;
        if (!active) continue;
        active_shires++;

        __asm__ __volatile__
          (
           "slli a2,%[shire],22\n" //shire_id to be added to ESR address

           ///////////////////
           // Perform IPI redirects for thread 0 --> Following official spec
           ///////////////////

           //IPI REDIRECT TRIGGER ESR  --> Neigh ESR
           "li a3,0x801F0040 \n" //shire_id = 0, neigh_id = 4'hF --> broadcast, PP = 2'b00
           "add a3,a3,a2\n" // add shire_id
           "sd %[compute_pc],0(a3)\n"

           "fence\n" //wait for neigh esr and shire esr to be written

           //IPI REDIRECT MASK ESR  --> Shire ESR
           "li a3,0x80340080 \n" //shire_id = 0 , PP = '0
           "add a3,a3,a2\n" // add shire_id
           "li a4,0x5555555555555555\n"
           "sd a4,0(a3)\n"

            "fence\n"
           :
           : [shire] "r" (shire), [compute_pc] "r" (net_desc[iter].compute_pc)
           : "a2",  "a3", "a4"
           );
        }

        // Set the PC to all the threads 1
        shire_mask = net_desc[iter].shire_mask;
        for ( shire = 0 ; shire < 34; shire++) {
            if ( shire == 32) continue;
            active = shire_mask % 2 != 0;
            shire_mask>>=1;
            if (!active) continue;

            __asm__ __volatile__
            (
            "slli a2,%[shire],22\n" //shire_id to be added to ESR address

            //IPI REDIRECT TRIGGER ESR  --> Neigh ESR
            "li a3,0x801F0040 \n" //shire_id = 0, neigh_id = 4'hF --> broadcast, PP = 2'b00
            "add a3,a3,a2\n" // add shire_id
            "sd %[helper_pc],0(a3)\n"
            "fence\n"
            :
            : [shire] "r" (shire), [helper_pc] "r" (net_desc[iter].helper_pc)
            : "a2", "a3"
            );
        }

        // Trigger all the threads 1
        shire_mask = net_desc[iter].shire_mask;
        for ( shire = 0 ; shire < 34; shire++) {
            if ( shire == 32) continue;
            active = shire_mask % 2 != 0;
            shire_mask>>=1;
            if (!active) continue;

            __asm__ __volatile__
            (
            "slli a2,%[shire],22\n" //shire_id to be added to ESR address

            //IPI REDIRECT MASK ESR  --> Shire ESR
            "li a3,0x80340080 \n" //shire_id = 0 , PP = '0
            "add a3,a3,a2\n" // add shire_id
            "li a4,0xAAAAAAAAAAAAAAAA\n"
            "sd a4,0(a3)\n"
            "fence\n"
            :
            : [shire] "r" (shire)
            : "a2",  "a3", "a4"
            );
        } //for shire

        // wait for layer to finish
        while( active_shires != 0) {
            active_shires--;

            // read head from port
            __asm__ __volatile__ ( "csrr x0,0xcc8\n"); //blocking read of port 0
        }
    }

    C_TEST_PASS;
}
