/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "tensors.h"
#include "common.h"

#ifdef __cplusplus
extern "C"
#endif
  
#define ITERATIONS 20
         
int main()
{
  uint64_t hart_id = getHartId();

  // 8bit signed integer
  signed char srow [ITERATIONS] = {2, -2, 13, -20, 111, -47, 104, -128, 80, -37,
                                   90, -78, 45, -105, 23, -64, 17, -54, 127, -6};
  // 8bit signed integer 
  signed char scol [ITERATIONS] = {-55, 83, 67, -113, -99, 22, 88, 90, 91, 111,
                                   73, -63, 68, 89, 3, -50, -41, 103, 107, 125}; 
  // 16bit unsigned integer
  uint16_t nrow [ITERATIONS] = {13577, 54840, 54408, 54242, 46423, 11774, 6112, 27062, 57453, 54309,
                                27337, 19110, 7940, 24513, 29213, 28869, 7049, 54658, 57054, 28180};

  // 16bit unsigned integer
  uint16_t ncol [ITERATIONS] = {53238, 38976, 54586, 41811, 45025, 23935, 59873, 56029, 55045, 710,
                                38455, 21105, 65302, 61184, 26393, 8259, 30091, 37647, 26962, 7851};

  // 16bit signed integer
  // Negative values to test out-of-bounds behaviour
  int16_t row_start [ITERATIONS] = {3111, 16091, 13270, -2774, -11, 23933, -2, 13858, 8563, 678,
                                    26395, 8843, 14546, 5185, -191, 8188, -13993, 23622, -31, 600};

  // 16bit signed integer
  // Negative values to test out-of-bounds behaviour
  int16_t col_start [ITERATIONS] = {13549, -13, 10207, 22253, 18922, 23920, 28233, 2278, -19, -140,
                                    12809, 268, -5, 1181, 25000, 92, 26410, -11872, 26620, 7169};

  int row, col;
  uint64_t expected_tmask, tmask, tmp;
  unsigned long reg_val;

  if( (hart_id & 1) == 0) {
 
      for (int i=0; i<ITERATIONS; i++){
        //                 srow     nrow     scol     ncol
        //               -------  -------  -------  -------
        convolution_size(srow[i], nrow[i], scol[i], ncol[i]);
             
        __asm__ __volatile__ ("csrr %0, 0x802" : "=r" (reg_val));
        if (reg_val != 0){
          C_TEST_FAIL;
        }

        //                 row_start     col_start 
        //               ------------  -------------  
        convolution_ctrl(row_start[i], col_start[i]);

        __asm__ __volatile__ ("csrr %0, 0x803" : "=r" (reg_val));
        if (reg_val != 0){
          C_TEST_FAIL;
        }

       __asm__ __volatile__ ("fence\n");

       expected_tmask = 0;
       // Compute expected tensor_mask
       for (int j = 0; j < 16; j++) {
         row = row_start[i] + j*srow[i];
         col = col_start[i] + j*scol[i];
         if (row >= 0 && col >= 0) {
           tmp = (((uint16_t)row < nrow[i]) && ((uint16_t)col < ncol[i])) << j;
           expected_tmask = expected_tmask | tmp;
         } 
       }
       
       tmask = get_tensor_mask() & 0xFFFFFFFF;

       if (tmask != expected_tmask){
         C_TEST_FAIL;
       }
    }

  }

  C_TEST_PASS;
  return 0;
}
