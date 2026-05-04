/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"

#ifdef __cplusplus
extern "C"
#endif

#define TENSOR_REDUCE_WAIT 9

#define N_LINES_X_MINION 1024ULL
#define N_LINES_DST 512ULL
#define N_LINES_OP 512ULL
#define N_TIMES 10
           
int main()
{

    uint64_t hart_id = getHartId();
    uint64_t minion_id = hart_id >>1;
  if(hart_id%2==0  && minion_id < 1024){
    setM0MaskFF();
    initFRegs(); 
    uint64_t startReg = 31;
    uint64_t OP = 0;// 0 - fadd   4-iadd
    uint64_t numRegisters = 1;
    uint64_t startLevel = 0;
    uint64_t numLevels = 10;
    uint64_t action = 3;
    uint64_t mask = (uint64_t) startReg << 57     |
                    (uint64_t) OP << 24           |
                    (uint64_t) numRegisters << 16 |
                  //(uint64_t) endLevel << 8      |  // FIXME: Tensor Reduce in spec allows specifying range of levels using
                                            //        start and end levels but current implementation only supports
                                            //        start level and need to generate multiple reduces to traverse
                                            //        a sequence of levels. Use JIRA ML-94 to discuss this topic
                    (uint64_t) startLevel << 3    |
                    (uint64_t) action             ;
 
      for(uint64_t i=0;i<numLevels;++i){
        _reduce(mask);
        mask+=8;
      }
      tensorWait(TENSOR_REDUCE_WAIT);
   }

   C_TEST_PASS;
   // End marker
   return 0;
}
