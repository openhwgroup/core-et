/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"

#ifdef __cplusplus
extern "C"
#endif



#define N_LINES_X_MINION 1024ULL
#define N_LINES_DST 512ULL
#define N_LINES_OP 16ULL
#define N_TIMES 32
           
int main()
{
    C_TEST_START; 
    bool passing = true;
    uint64_t hart_id = getHartId();
    uint64_t shire_id  = ((hart_id>>6) & 0x3F);
    if( ((hart_id & 1) == 0 ) && (shire_id == 0) ) {


      uint64_t minion_id = ((hart_id>>1) & 0x1F);

      uint64_t VA = ((2U)<<30U)  + (shire_id << 23) +  (minion_id*N_LINES_X_MINION*N_BYTES_CL);
#if 0
      uint64_t ptr = VA;
      //init A
      for(int i = 0 ; i  < N_LINES_X_MINION; i++ ){
        //each CL has 32 elements of uint16
        for(int k = 0 ; k  < 32; k++ ){   
          shg(long(ptr) , i*32+k );
          ptr += 2;
        }
      }
#endif
///// INIT
      setM0MaskFF();
      initFRegs();
      uint64_t start_destination =  ((2U)<<30U)  + (shire_id << 23) +  (minion_id*N_LINES_X_MINION*N_BYTES_CL); //each minion writes 1024 CL * 64 Bytes/CL
      //n blocks:
      //each block is 128 b, a CL = 64 B => 4 blocks
      uint64_t n_blocks = 4;
      uint64_t conf_base =   ( (n_blocks-1) << 55)
                     | ( (N_LINES_OP-1) << 51);

      
      for(int i = 0; i < N_TIMES ; i++){
        uint64_t conf=conf_base|start_destination;
        tensorStore(conf, N_BYTES_CL);
        start_destination+=N_LINES_OP*N_BYTES_CL;
      }
//////END of INIT

      uint64_t VA_tl = ((2U)<<30U)  + (shire_id << 23) +  (minion_id*N_LINES_X_MINION*N_BYTES_CL) + N_LINES_DST*N_BYTES_CL;
      uint64_t l2_scp_line = (minion_id*N_LINES_X_MINION)+ N_LINES_DST;
      uint64_t l1_scp_line_tl = 0;


      uint64_t base_conf_tl = (N_LINES_OP -1);
      uint64_t conf_tl = base_conf_tl 
                        | l1_scp_line_tl<<53
                        | VA_tl ;

      uint64_t base_conf = (N_LINES_OP -1 );
      
      uint64_t conf = base_conf | VA | ( (l2_scp_line>>2ULL)   << 48);
      uint64_t id = 0;
      uint64_t *addr; 
      tensorLoadL2SCP(conf, N_BYTES_CL, id); // ID0
      id ^= 1;
      for(int i = 0 ; i < N_TIMES-1 ; i++){
        l2_scp_line +=N_LINES_OP;
        addr = (uint64_t*) VA;
        VA += N_LINES_OP * N_BYTES_CL;
        conf = base_conf | VA | ( (l2_scp_line>>2ULL)   << 48);
        tensorLoadL2SCP(conf, N_BYTES_CL, id);

        id ^= 1;

        tensorWait(id);
        tensorLoad(conf_tl,N_BYTES_CL,id);//l2 scp to l1
        VA_tl+=( N_LINES_OP * N_BYTES_CL);
        l1_scp_line_tl= ( l1_scp_line_tl +  N_LINES_OP) & 0x3F; //only 64 lines as destination
        conf_tl = base_conf_tl 
                        | l1_scp_line_tl<<53
                        | VA_tl ;


        uint64_t data = 0; 
        uint64_t exp_data = 0; 

        //Self-checking part: read the data from memory and check it matches the data initialy stored and then loaded into L2SCP
        for(int j = 0; j < 32; j++) {
          if (j < 8) {
            exp_data = (j << 12) | (j << 8) | (j << 4) | j; //(j << 44) | (j << 40) | (j << 36) | (j << 32) |
            exp_data = (exp_data << 32) | (j << 12) | (j << 8) | (j << 4) | j;
          }
          else if (j < 0x10) {
            exp_data = (j << 12) | (j << 8) | (j << 4) | ((j+1) & 0xffff);
            exp_data = (exp_data << 32) | (j << 12) | (j << 8) | (j << 4) | ((j+1) & 0xffff);
          }
          else if (j < 0x18) {
            exp_data = (1 << 16) | ((j & 0xf) << 12) | ((j & 0xf) << 8) | ((j & 0xf) << 4) | (j & 0xf);
            exp_data = (exp_data << 32) | (1 << 16) | ((j & 0xf) << 12) | ((j & 0xf) << 8) | ((j & 0xf) << 4) | (j & 0xf);
          }
          else {
            exp_data = (1 << 16) | ((j & 0xf) << 12) | ((j & 0xf) << 8) | ((j & 0xf) << 4) | ((j+1) & 0xf);
            exp_data = (exp_data << 32) | (1 << 16) | ((j & 0xf) << 12) | ((j & 0xf) << 8) | ((j & 0xf) << 4) | ((j+1) & 0xf);
          }

          data = *addr;
          addr+=0x4;

          if (exp_data != data) {
            passing = false;
          }
        }

      }
      id ^= 1;
      tensorWait(id);
      tensorLoad(conf_tl,N_BYTES_CL,id);//l2 scp to l1


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
