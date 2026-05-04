/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"

int main() {

   volatile uint32_t operands[256] __attribute__ ((aligned (4096))) = {

      // vpu register f0
      0x00000000,                      
      0x00000000,                      
      0x00000000,                      
      0x00000000,                      
      0x00000000,                      
      0x00000000,                      
      0x00000000,                      
      0x00000000,                      

      // vpu register f1
      0x00000100,                      
      0x00000200,                      
      0x00000300,                      
      0x00000400,                      
      0x00000500,                      
      0x00000600,                      
      0x00000700,                      
      0x00000800,                      

      // vpu register f2
      0x00000000,    // zero                  
      0xbf028f5c,    // -0.51                 
      0x3f800000,    // 1.0                   
      0xffc00001,    // -signaling NaN        
      0x4b000000,    // 8388608.0             
      0xcb8c4b40,    // -18388608.0           
      0x00000000,    // zero                  
      0x80000000,    // -zero                 

      // vpu register f3
      0xcb000000,    // -8388608.0            
      0x00000000,    // zero                  
      0x80000000,    // -zero                 
      0x80000000,    // -zero                 
      0xbf028f5c,    // -0.51                 
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000000,    // zero                  
      0x7fc00000,    // quiet NaN             

      // vpu register f4
      0x80011111,    // -9.7958E-41           
      0xffc00001,    // -signaling NaN        
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf800000,    // -1.0                  
      0x3f800000,    // 1.0                   
      0x4b8c4b40,    // 18388608.0            
      0xcb000000,    // -8388608.0            
      0x7fc00001,    // signaling NaN         

      // vpu register f5
      0x7fc00000,    // quiet NaN             
      0xbf028f5c,    // -0.51                 
      0xff800000,    // -inf                  
      0x7fc00001,    // signaling NaN         
      0xcb8c4b40,    // -18388608.0           
      0x4b8c4b40,    // 18388608.0            
      0x80000000,    // -zero                 
      0xffc00001,    // -signaling NaN        

      // vpu register f6
      0xffc00001,    // -signaling NaN        
      0x7f800000,    // inf                   
      0xffc00000,    // -quiet NaN            
      0xff800000,    // -inf                  
      0x3f028f5c,    // 0.51                  
      0xffc00000,    // -quiet NaN            
      0xcb000000,    // -8388608.0            
      0x00011111,    // 9.7958E-41            

      // vpu register f7
      0xcb000000,    // -8388608.0            
      0x80000000,    // -zero                 
      0x7fc00000,    // quiet NaN             
      0x7f800000,    // inf                   
      0xff800000,    // -inf                  
      0x00011111,    // 9.7958E-41            
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf800000,    // -1.0                  

      // vpu register f8
      0x007fffff,    // 1.1754942E-38         
      0x4b8c4b40,    // 18388608.0            
      0x3f028f5c,    // 0.51                  
      0x007fffff,    // 1.1754942E-38         
      0xff800000,    // -inf                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f800000,    // 1.0                   
      0x7f800000,    // inf                   

      // vpu register f9
      0xcb8c4b40,    // -18388608.0           
      0xcb8c4b40,    // -18388608.0           
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f800000,    // 1.0                   
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb000000,    // -8388608.0            
      0x3f028f5c,    // 0.51                  
      0x7fc00001,    // signaling NaN         

      // vpu register f10
      0x00000000,    // zero                  
      0x80000000,    // -zero                 
      0xffc00001,    // -signaling NaN        
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 
      0x3f800000,    // 1.0                   
      0x00011111,    // 9.7958E-41            
      0xcb8c4b40,    // -18388608.0           

      // vpu register f11
      0x007fffff,    // 1.1754942E-38         
      0x007fffff,    // 1.1754942E-38         
      0x7fc00001,    // signaling NaN         
      0xcb8c4b40,    // -18388608.0           
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00011111,    // 9.7958E-41            
      0x80011111,    // -9.7958E-41           
      0xffc00000,    // -quiet NaN            

      // vpu register f12
      0x00011111,    // 9.7958E-41            
      0xbf028f5c,    // -0.51                 
      0x00000000,    // zero                  
      0x00000000,    // zero                  
      0x7fc00000,    // quiet NaN             
      0x80011111,    // -9.7958E-41           
      0x007fffff,    // 1.1754942E-38         
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f13
      0x00000001,    // 1.4E-45 (+denorm)     
      0xffc00000,    // -quiet NaN            
      0xbf028f5c,    // -0.51                 
      0x7f800000,    // inf                   
      0xffc00001,    // -signaling NaN        
      0x007fffff,    // 1.1754942E-38         
      0x80000000,    // -zero                 
      0xbf028f5c,    // -0.51                 

      // vpu register f14
      0x7fc00001,    // signaling NaN         
      0x80011111,    // -9.7958E-41           
      0xffc00001,    // -signaling NaN        
      0x007fffff,    // 1.1754942E-38         
      0x3f800000,    // 1.0                   
      0x80000000,    // -zero                 
      0xbf028f5c,    // -0.51                 
      0x00000000,    // zero                  

      // vpu register f15
      0x7fc00000,    // quiet NaN             
      0x3f028f5c,    // 0.51                  
      0xffc00001,    // -signaling NaN        
      0xffc00000,    // -quiet NaN            
      0x3f028f5c,    // 0.51                  
      0x7fc00001,    // signaling NaN         
      0x80011111,    // -9.7958E-41           
      0x7fc00001,    // signaling NaN         

      // vpu register f16
      0xff800000,    // -inf                  
      0x4b000000,    // 8388608.0             
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf800000,    // -1.0                  
      0x3f800000,    // 1.0                   
      0x00011111,    // 9.7958E-41            
      0xcb000000,    // -8388608.0            
      0xff800000,    // -inf                  

      // vpu register f17
      0x00000000,    // zero                  
      0x007fffff,    // 1.1754942E-38         
      0x007fffff,    // 1.1754942E-38         
      0xbf800000,    // -1.0                  
      0x3f800000,    // 1.0                   
      0x80000000,    // -zero                 
      0xbf800000,    // -1.0                  
      0x4b8c4b40,    // 18388608.0            

      // vpu register f18
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb000000,    // -8388608.0            
      0x80000001,    // -1.4E-45 (-denorm)    
      0xffc00001,    // -signaling NaN        
      0x00000000,    // zero                  
      0xcb8c4b40,    // -18388608.0           
      0x3f800000,    // 1.0                   
      0x007fffff,    // 1.1754942E-38         

      // vpu register f19
      0x3f7ffc00,    
      0x00000000,    // zero                  
      0xbf800000,    // -1.0                  
      0x80000000,    // -zero                 
      0xff800000,    // -inf                  
      0xbf028f5c,    // -0.51                 
      0xbf7ffc00,                      
      0xcb000000,    // -8388608.0            

      // vpu register f20
      0x4b000000,    // 8388608.0             
      0xbf028f5c,    // -0.51                 
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb000000,    // -8388608.0            
      0x7fc00000,    // quiet NaN             
      0x3f800000,    // 1.0                   
      0xbf800000,    // -1.0                  
      0xffc00001,    // -signaling NaN        

      // vpu register f21
      0x80000000,    // -zero                 
      0xff800000,    // -inf                  
      0x00011111,    // 9.7958E-41            
      0xffc00001,    // -signaling NaN        
      0x7fc00000,    // quiet NaN             
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0xffc00000,    // -quiet NaN            

      // vpu register f22
      0xffc00001,    // -signaling NaN        
      0xcb8c4b40,    // -18388608.0           
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb000000,    // -8388608.0            
      0x3f028f5c,    // 0.51                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0x7f800000,    // inf                   
      0xcb000000,    // -8388608.0            

      // vpu register f23
      0xff800000,    // -inf                  
      0x80000000,    // -zero                 
      0x4b8c4b40,    // 18388608.0            
      0x80000000,    // -zero                 
      0xffc00000,    // -quiet NaN            
      0xbf800000,    // -1.0                  
      0x7f800000,    // inf                   
      0x4b000000,    // 8388608.0             

      // vpu register f24
      0x00000000,    // zero                  
      0x80011111,    // -9.7958E-41           
      0x4b8c4b40,    // 18388608.0            
      0x4b000000,    // 8388608.0             
      0x80000000,    // -zero                 
      0x3f028f5c,    // 0.51                  
      0x7f800000,    // inf                   
      0xbf028f5c,    // -0.51                 

      // vpu register f25
      0x4b8c4b40,    // 18388608.0            
      0x4b000000,    // 8388608.0             
      0x00000000,    // zero                  
      0xcb8c4b40,    // -18388608.0           
      0xbf028f5c,    // -0.51                 
      0x80000001,    // -1.4E-45 (-denorm)    
      0x007fffff,    // 1.1754942E-38         
      0xbf800000,    // -1.0                  

      // vpu register f26
      0x7fc00000,    // quiet NaN             
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00011111,    // 9.7958E-41            
      0xffc00001,    // -signaling NaN        
      0x00000000,    // zero                  
      0xcb8c4b40,    // -18388608.0           
      0xcb8c4b40,    // -18388608.0           
      0xcb8c4b40,    // -18388608.0           

      // vpu register f27
      0x4b000000,    // 8388608.0             
      0xbf800000,    // -1.0                  
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0x7f800000,    // inf                   
      0xcb8c4b40,    // -18388608.0           
      0x7fc00001,    // signaling NaN         

      // vpu register f28
      0xbf800000,    // -1.0                  
      0x4b8c4b40,    // 18388608.0            
      0x007fffff,    // 1.1754942E-38         
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf028f5c,    // -0.51                 
      0x80011111,    // -9.7958E-41           
      0x4b8c4b40,    // 18388608.0            
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f29
      0xffc00001,    // -signaling NaN        
      0x4b8c4b40,    // 18388608.0            
      0x3f800000,    // 1.0                   
      0xcb000000,    // -8388608.0            
      0xbf028f5c,    // -0.51                 
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80000000,    // -zero                 
      0x80011111,    // -9.7958E-41           

      // vpu register f30
      0x7fc00000,    // quiet NaN             
      0x7fc00001,    // signaling NaN         
      0xbf028f5c,    // -0.51                 
      0xffc00001,    // -signaling NaN        
      0x007fffff,    // 1.1754942E-38         
      0xffc00001,    // -signaling NaN        
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   

      // vpu register f31
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0x80011111,    // -9.7958E-41           
      0xbf028f5c,    // -0.51                 
      0xbf800000,    // -1.0                  
      0xffc00000,    // -quiet NaN            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f800000     // 1.0                   
   };
   C_TEST_START;
   __asm__ __volatile__ (
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "flw.ps f19,  608 (%[operands])\n"
         "fcvt.f16.ps f31, f19\n"        
         // Asm output results to C code
         :
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f19", "f30", "f31"
         );
   C_TEST_PASS;
   return 0;
}
