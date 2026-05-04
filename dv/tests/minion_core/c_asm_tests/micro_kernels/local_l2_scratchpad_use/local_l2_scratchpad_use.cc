/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"

#ifdef __cplusplus
extern "C"
#endif



           
int main()
{
    C_TEST_START;  
    bool passing = true;
    uint64_t hart_id = getHartId();
    uint64_t shire_id  = ((hart_id>>6) & 0x3F);
    uint64_t minion_id  = ((hart_id>>1) & 0x3F);
    if( ((hart_id & 1) == 0 ) && (shire_id == 0) && (minion_id == 0)  ) {
      long read=0;
      long convolutionSize = (1ULL  << 56ULL) //srow
                            |(30ULL << 32ULL) //nrow
                            |(2ULL  << 24ULL) //scol
                            |(15ULL  << 0ULL);  //ncol
      long convolutionSize2 = (1ULL  << 56ULL) //srow
                            |(4ULL << 32ULL) //nrow
                            |(2ULL  << 24ULL) //scol
                            |(15ULL  << 0ULL);  //ncol

      long convolutionSize3 = (1ULL  << 56ULL) //srow
                            |(7ULL << 32ULL) //nrow
                            |(2ULL  << 24ULL) //scol
                            |(15ULL  << 0ULL);  //ncol

      long convolutionCtrl = (1ULL  << 32ULL) //row start
                             |(1ULL  << 0ULL);  //col start

      long convolutionCtrl2 = (20ULL  << 32ULL) //row start
                             |(20ULL  << 0ULL);  //col start

      //write register 805
      writeTensorMask(0x3f3f);
      //write register 802
      writeConvolutionSize(convolutionSize);
      //write register 803
      writeConvCtrl(convolutionCtrl);
      //read register 805
      read=readTensorMask();
      if(read!=0x7f){
        passing = false;
      }

      //write register 802
      writeConvolutionSize(convolutionSize2);
      //read register 805
      read=readTensorMask();
      if(read!=0x7){
        passing = false;
      }

      //write register 802
      writeConvolutionSize(convolutionSize3);
      //read register 805
      read=readTensorMask();
      if(read!=0x3f){
        passing = false;
      }

      //write register 803
      writeConvCtrl(convolutionCtrl2);
      //read register 805
      read=readTensorMask();
      if(read!=0x0){
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

