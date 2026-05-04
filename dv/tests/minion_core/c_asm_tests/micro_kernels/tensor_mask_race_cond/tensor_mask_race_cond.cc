/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"

#ifdef __cplusplus
extern "C"
#endif



#define N_LINES_X_MINION 1024ULL
#define N_LINES_DST 512ULL
#define N_LINES_OP 512ULL
#define N_TIMES 10




int main()
{
    C_TEST_START; 
    long read=0;    
    bool passing = true;
    uint64_t hart_id = getHartId();
    uint64_t tensor_values[128] __attribute__((aligned(1024)));
    //todo init A
    //
    //only thread 0 of minion 0 
    if(hart_id == 0) {
      //enable L1 scratchpad
      uint64_t SCPEnabled = 0x01;
      __asm__ __volatile__
       (
        " csrw 0x7e0, %[SCPEnabled]\n"
        " csrr %[SCPEnabled],0x7e0\n"
        " fence\n"
        " li %[SCPEnabled],3\n"
        " csrw 0x7e0, %[SCPEnabled]\n"
        " csrr %[SCPEnabled],0x7e0\n"
        :
        : [SCPEnabled] "rw" (SCPEnabled)
        :
       );


      for(int i=0; i< 128; ++i)
        tensor_values[i] = i;

      uint64_t *addr = tensor_values;
      tensor_conv_size_conf(1,8,1,8);
      tensor_conv_ctrl_conf(0,0);

      uint64_t conf = long(addr) | //addr of tensor 
                      7ULL; // num_lines

      tensorLoad(conf,64,0);
      
      tensor_conv_ctrl_conf(2,2);

      read=readTensorMask();
      if(read!=0x3f){
        passing = false;
      }

    }
    
    

    if(passing){
      C_TEST_PASS;
    }
    else {
      C_TEST_FAIL;
    }
    // End marker
    return 0;
}
