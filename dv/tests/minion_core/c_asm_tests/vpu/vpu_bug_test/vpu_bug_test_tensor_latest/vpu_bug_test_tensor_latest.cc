/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#define GFX_TRAP_ESR_PTR        0x80F40000ULL

volatile uint64_t *gfxTrapEsrPtr        = (uint64_t*)GFX_TRAP_ESR_PTR;  
 


int do_load (uint32_t* m_a) {
   register uint64_t p_a_ asm("s3") = (uint64_t) m_a;

   __asm__ __volatile__ (
         "flw.ps f0,  0 (s3)\n"
         "flw.ps f1,  32 (s3)\n"
         "flw.ps f2,  64 (s3)\n"
         "flw.ps f3,  96 (s3)\n"
         "flw.ps f4,  128 (s3)\n"
         "flw.ps f5,  160 (s3)\n"
         "flw.ps f6,  192 (s3)\n"
         "flw.ps f7,  224 (s3)\n"
         "flw.ps f8,  256 (s3)\n"
         "flw.ps f9,  288 (s3)\n"
         "flw.ps f10,  320 (s3)\n"
         "flw.ps f11,  352 (s3)\n"
         "flw.ps f12,  384 (s3)\n"
         "flw.ps f13,  416 (s3)\n"
         "flw.ps f14,  448 (s3)\n"
         "flw.ps f15,  480 (s3)\n"
         "flw.ps f16,  512 (s3)\n"
         "flw.ps f17,  544 (s3)\n"
         "flw.ps f18,  576 (s3)\n"
         "flw.ps f19,  608 (s3)\n"
         "flw.ps f20,  640 (s3)\n"
         "flw.ps f21,  672 (s3)\n"
         "flw.ps f22,  704 (s3)\n"
         "flw.ps f23,  736 (s3)\n"
         "flw.ps f24,  768 (s3)\n"
         "flw.ps f25,  800 (s3)\n"
         "flw.ps f26,  832 (s3)\n"
         "flw.ps f27,  864 (s3)\n"
         "flw.ps f28,  896 (s3)\n"
         "flw.ps f29,  928 (s3)\n"
         "flw.ps f30,  960 (s3)\n"
         "flw.ps f31,  992 (s3)\n"
         :
         : "r" (p_a_)
         :"f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
            );
   return 0;
}

int main() {

   *gfxTrapEsrPtr =  (uintptr_t) (0); // Avoid trap for gfx instructions
   static uint32_t operands[512] __attribute__ ((aligned (16))) = {

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
      0x7fc00000,    // quiet NaN             
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000000,    // zero                  
      0x7fc00001,    // signaling NaN         
      0x00011111,    // 9.7958E-41            
      0xffc00000,    // -quiet NaN            
      0xcb8c4b40,    // -18388608.0           
      0x00011111,    // 9.7958E-41            

      // vpu register f3
      0x007fffff,    // 1.1754942E-38         
      0xbf028f5c,    // -0.51                 
      0x7fc00000,    // quiet NaN             
      0x7f800000,    // inf                   
      0xff800000,    // -inf                  
      0x80011111,    // -9.7958E-41           
      0xffc00001,    // -signaling NaN        
      0x7fc00001,    // signaling NaN         

      // vpu register f4
      0x007fffff,    // 1.1754942E-38         
      0xffc00001,    // -signaling NaN        
      0x7fc00000,    // quiet NaN             
      0x00011111,    // 9.7958E-41            
      0xcb8c4b40,    // -18388608.0           
      0xff800000,    // -inf                  
      0xffc00000,    // -quiet NaN            
      0x007fffff,    // 1.1754942E-38         

      // vpu register f5
      0x4b8c4b40,    // 18388608.0            
      0x007fffff,    // 1.1754942E-38         
      0x80000000,    // -zero                 
      0xff800000,    // -inf                  
      0xbf800000,    // -1.0                  
      0x4b000000,    // 8388608.0             
      0xcb8c4b40,    // -18388608.0           
      0x00011111,    // 9.7958E-41            

      // vpu register f6
      0xcb8c4b40,    // -18388608.0           
      0xff800000,    // -inf                  
      0x80011111,    // -9.7958E-41           
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0x00000000,    // zero                  
      0x00000000,    // zero                  
      0x007fffff,    // 1.1754942E-38         

      // vpu register f7
      0xbf028f5c,    // -0.51                 
      0xffc00000,    // -quiet NaN            
      0x00000001,    // 1.4E-45 (+denorm)     
      0xffc00000,    // -quiet NaN            
      0x007fffff,    // 1.1754942E-38         
      0x00000000,    // zero                  
      0x007fffff,    // 1.1754942E-38         
      0x80011111,    // -9.7958E-41           

      // vpu register f8
      0x4b8c4b40,    // 18388608.0            
      0x7fc00001,    // signaling NaN         
      0x7fc00000,    // quiet NaN             
      0x80000000,    // -zero                 
      0x007fffff,    // 1.1754942E-38         
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf800000,    // -1.0                  
      0xffc00001,    // -signaling NaN        

      // vpu register f9
      0x80000001,    // -1.4E-45 (-denorm)    
      0xff800000,    // -inf                  
      0x3f028f5c,    // 0.51                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80000000,    // -zero                 
      0x7fc00001,    // signaling NaN         
      0x3f800000,    // 1.0                   
      0x4b000000,    // 8388608.0             

      // vpu register f10
      0x3f028f5c,    // 0.51                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0x7fc00000,    // quiet NaN             
      0xff800000,    // -inf                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00011111,    // 9.7958E-41            
      0xcb000000,    // -8388608.0            
      0x7fc00000,    // quiet NaN             

      // vpu register f11
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            
      0x4b8c4b40,    // 18388608.0            
      0x7fc00000,    // quiet NaN             
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00000000,    // zero                  
      0xff800000,    // -inf                  
      0x7fc00000,    // quiet NaN             

      // vpu register f12
      0x3f028f5c,    // 0.51                  
      0x7fc00000,    // quiet NaN             
      0x00011111,    // 9.7958E-41            
      0x80000001,    // -1.4E-45 (-denorm)    
      0xffc00001,    // -signaling NaN        
      0x007fffff,    // 1.1754942E-38         
      0xcb8c4b40,    // -18388608.0           
      0x4b8c4b40,    // 18388608.0            

      // vpu register f13
      0x3f800000,    // 1.0                   
      0xcb8c4b40,    // -18388608.0           
      0x00011111,    // 9.7958E-41            
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0x3f028f5c,    // 0.51                  

      // vpu register f14
      0x7fc00001,    // signaling NaN         
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80011111,    // -9.7958E-41           
      0x7fc00000,    // quiet NaN             
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb000000,    // -8388608.0            
      0xcb000000,    // -8388608.0            
      0x4b000000,    // 8388608.0             

      // vpu register f15
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80011111,    // -9.7958E-41           
      0x3f800000,    // 1.0                   
      0xffc00000,    // -quiet NaN            
      0x007fffff,    // 1.1754942E-38         
      0xcb000000,    // -8388608.0            
      0x4b8c4b40,    // 18388608.0            
      0x00000000,    // zero                  

      // vpu register f16
      0x00000000,    // zero                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0x7fc00000,    // quiet NaN             
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b000000,    // 8388608.0             
      0x3f028f5c,    // 0.51                  
      0x007fffff,    // 1.1754942E-38         
      0x7fc00000,    // quiet NaN             

      // vpu register f17
      0x4b8c4b40,    // 18388608.0            
      0xffc00000,    // -quiet NaN            
      0x80011111,    // -9.7958E-41           
      0x7fc00000,    // quiet NaN             
      0x00011111,    // 9.7958E-41            
      0xffc00001,    // -signaling NaN        
      0xbf800000,    // -1.0                  
      0xffc00001,    // -signaling NaN        

      // vpu register f18
      0x4b8c4b40,    // 18388608.0            
      0x7fc00001,    // signaling NaN         
      0x3f028f5c,    // 0.51                  
      0x4b8c4b40,    // 18388608.0            
      0x7fc00001,    // signaling NaN         
      0xcb8c4b40,    // -18388608.0           
      0x4b8c4b40,    // 18388608.0            
      0x7f800000,    // inf                   

      // vpu register f19
      0x4b8c4b40,    // 18388608.0            
      0xffc00001,    // -signaling NaN        
      0x4b000000,    // 8388608.0             
      0x3f800000,    // 1.0                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0x7fc00001,    // signaling NaN         
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            

      // vpu register f20
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            
      0x80000000,    // -zero                 
      0x00011111,    // 9.7958E-41            
      0x3f028f5c,    // 0.51                  
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            
      0x7fc00000,    // quiet NaN             

      // vpu register f21
      0x80000000,    // -zero                 
      0xffc00000,    // -quiet NaN            
      0x7fc00001,    // signaling NaN         
      0x007fffff,    // 1.1754942E-38         
      0x00000001,    // 1.4E-45 (+denorm)     
      0xffc00000,    // -quiet NaN            
      0x4b000000,    // 8388608.0             
      0x80011111,    // -9.7958E-41           

      // vpu register f22
      0xcb8c4b40,    // -18388608.0           
      0xffc00001,    // -signaling NaN        
      0x7f800000,    // inf                   
      0x007fffff,    // 1.1754942E-38         
      0x00000001,    // 1.4E-45 (+denorm)     
      0xffc00000,    // -quiet NaN            
      0x4b000000,    // 8388608.0             
      0x00011111,    // 9.7958E-41            

      // vpu register f23
      0x7fc00000,    // quiet NaN             
      0x80000000,    // -zero                 
      0x3f028f5c,    // 0.51                  
      0x7fc00001,    // signaling NaN         
      0x7f800000,    // inf                   
      0xcb000000,    // -8388608.0            
      0x7f800000,    // inf                   
      0x007fffff,    // 1.1754942E-38         

      // vpu register f24
      0xbf028f5c,    // -0.51                 
      0x4b000000,    // 8388608.0             
      0x7fc00000,    // quiet NaN             
      0xbf800000,    // -1.0                  
      0xff800000,    // -inf                  
      0x7fc00000,    // quiet NaN             
      0xcb000000,    // -8388608.0            
      0xbf028f5c,    // -0.51                 

      // vpu register f25
      0x00011111,    // 9.7958E-41            
      0x3f028f5c,    // 0.51                  
      0x7fc00000,    // quiet NaN             
      0x7f800000,    // inf                   
      0xffc00001,    // -signaling NaN        
      0x00000000,    // zero                  
      0x3f800000,    // 1.0                   
      0x00011111,    // 9.7958E-41            

      // vpu register f26
      0xcb000000,    // -8388608.0            
      0x4b000000,    // 8388608.0             
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f800000,    // 1.0                   
      0x3f800000,    // 1.0                   
      0x7f800000,    // inf                   
      0x7fc00001,    // signaling NaN         
      0xbf800000,    // -1.0                  

      // vpu register f27
      0x7f800000,    // inf                   
      0x3f800000,    // 1.0                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb000000,    // -8388608.0            
      0x3f028f5c,    // 0.51                  
      0xff800000,    // -inf                  
      0x7fc00000,    // quiet NaN             
      0xff800000,    // -inf                  

      // vpu register f28
      0xff800000,    // -inf                  
      0x4b8c4b40,    // 18388608.0            
      0x00000000,    // zero                  
      0xcb8c4b40,    // -18388608.0           
      0x80000000,    // -zero                 
      0xffc00000,    // -quiet NaN            
      0xffc00000,    // -quiet NaN            
      0x4b000000,    // 8388608.0             

      // vpu register f29
      0xff800000,    // -inf                  
      0xbf028f5c,    // -0.51                 
      0x7f800000,    // inf                   
      0x4b8c4b40,    // 18388608.0            
      0xcb000000,    // -8388608.0            
      0x7fc00000,    // quiet NaN             
      0xff800000,    // -inf                  
      0x7f800000,    // inf                   

      // vpu register f30
      0xcb000000,    // -8388608.0            
      0x3f028f5c,    // 0.51                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0x7f800000,    // inf                   
      0xff800000,    // -inf                  
      0xcb8c4b40,    // -18388608.0           
      0x7fc00001,    // signaling NaN         
      0x3f800000,    // 1.0                   

      // vpu register f31
      0x3f800000,    // 1.0                   
      0x7fc00000,    // quiet NaN             
      0x80000000,    // -zero                 
      0xff800000,    // -inf                  
      0x80000000,    // -zero                 
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf028f5c,    // -0.51                 
      0x3f800000,     // 1.0                   
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
      0x7fc00000,    // quiet NaN             
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000000,    // zero                  
      0x7fc00001,    // signaling NaN         
      0x00011111,    // 9.7958E-41            
      0xffc00000,    // -quiet NaN            
      0xcb8c4b40,    // -18388608.0           
      0x00011111,    // 9.7958E-41            

      // vpu register f3
      0x007fffff,    // 1.1754942E-38         
      0xbf028f5c,    // -0.51                 
      0x7fc00000,    // quiet NaN             
      0x7f800000,    // inf                   
      0xff800000,    // -inf                  
      0x80011111,    // -9.7958E-41           
      0xffc00001,    // -signaling NaN        
      0x7fc00001,    // signaling NaN         

      // vpu register f4
      0x007fffff,    // 1.1754942E-38         
      0xffc00001,    // -signaling NaN        
      0x7fc00000,    // quiet NaN             
      0x00011111,    // 9.7958E-41            
      0xcb8c4b40,    // -18388608.0           
      0xff800000,    // -inf                  
      0xffc00000,    // -quiet NaN            
      0x007fffff,    // 1.1754942E-38         

      // vpu register f5
      0x4b8c4b40,    // 18388608.0            
      0x007fffff,    // 1.1754942E-38         
      0x80000000,    // -zero                 
      0xff800000,    // -inf                  
      0xbf800000,    // -1.0                  
      0x4b000000,    // 8388608.0             
      0xcb8c4b40,    // -18388608.0           
      0x00011111,    // 9.7958E-41            

      // vpu register f6
      0xcb8c4b40,    // -18388608.0           
      0xff800000,    // -inf                  
      0x80011111,    // -9.7958E-41           
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0x00000000,    // zero                  
      0x00000000,    // zero                  
      0x007fffff,    // 1.1754942E-38         

      // vpu register f7
      0xbf028f5c,    // -0.51                 
      0xffc00000,    // -quiet NaN            
      0x00000001,    // 1.4E-45 (+denorm)     
      0xffc00000,    // -quiet NaN            
      0x007fffff,    // 1.1754942E-38         
      0x00000000,    // zero                  
      0x007fffff,    // 1.1754942E-38         
      0x80011111,    // -9.7958E-41           

      // vpu register f8
      0x4b8c4b40,    // 18388608.0            
      0x7fc00001,    // signaling NaN         
      0x7fc00000,    // quiet NaN             
      0x80000000,    // -zero                 
      0x007fffff,    // 1.1754942E-38         
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf800000,    // -1.0                  
      0xffc00001,    // -signaling NaN        

      // vpu register f9
      0x80000001,    // -1.4E-45 (-denorm)    
      0xff800000,    // -inf                  
      0x3f028f5c,    // 0.51                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80000000,    // -zero                 
      0x7fc00001,    // signaling NaN         
      0x3f800000,    // 1.0                   
      0x4b000000,    // 8388608.0             

      // vpu register f10
      0x3f028f5c,    // 0.51                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0x7fc00000,    // quiet NaN             
      0xff800000,    // -inf                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00011111,    // 9.7958E-41            
      0xcb000000,    // -8388608.0            
      0x7fc00000,    // quiet NaN             

      // vpu register f11
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            
      0x4b8c4b40,    // 18388608.0            
      0x7fc00000,    // quiet NaN             
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00000000,    // zero                  
      0xff800000,    // -inf                  
      0x7fc00000,    // quiet NaN             

      // vpu register f12
      0x3f028f5c,    // 0.51                  
      0x7fc00000,    // quiet NaN             
      0x00011111,    // 9.7958E-41            
      0x80000001,    // -1.4E-45 (-denorm)    
      0xffc00001,    // -signaling NaN        
      0x007fffff,    // 1.1754942E-38         
      0xcb8c4b40,    // -18388608.0           
      0x4b8c4b40,    // 18388608.0            

      // vpu register f13
      0x3f800000,    // 1.0                   
      0xcb8c4b40,    // -18388608.0           
      0x00011111,    // 9.7958E-41            
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0x3f028f5c,    // 0.51                  

      // vpu register f14
      0x7fc00001,    // signaling NaN         
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80011111,    // -9.7958E-41           
      0x7fc00000,    // quiet NaN             
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb000000,    // -8388608.0            
      0xcb000000,    // -8388608.0            
      0x4b000000,    // 8388608.0             

      // vpu register f15
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80011111,    // -9.7958E-41           
      0x3f800000,    // 1.0                   
      0xffc00000,    // -quiet NaN            
      0x007fffff,    // 1.1754942E-38         
      0xcb000000,    // -8388608.0            
      0x4b8c4b40,    // 18388608.0            
      0x00000000,    // zero                  

      // vpu register f16
      0x00000000,    // zero                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0x7fc00000,    // quiet NaN             
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b000000,    // 8388608.0             
      0x3f028f5c,    // 0.51                  
      0x007fffff,    // 1.1754942E-38         
      0x7fc00000,    // quiet NaN             

      // vpu register f17
      0x4b8c4b40,    // 18388608.0            
      0xffc00000,    // -quiet NaN            
      0x80011111,    // -9.7958E-41           
      0x7fc00000,    // quiet NaN             
      0x00011111,    // 9.7958E-41            
      0xffc00001,    // -signaling NaN        
      0xbf800000,    // -1.0                  
      0xffc00001,    // -signaling NaN        

      // vpu register f18
      0x4b8c4b40,    // 18388608.0            
      0x7fc00001,    // signaling NaN         
      0x3f028f5c,    // 0.51                  
      0x4b8c4b40,    // 18388608.0            
      0x7fc00001,    // signaling NaN         
      0xcb8c4b40,    // -18388608.0           
      0x4b8c4b40,    // 18388608.0            
      0x7f800000,    // inf                   

      // vpu register f19
      0x4b8c4b40,    // 18388608.0            
      0xffc00001,    // -signaling NaN        
      0x4b000000,    // 8388608.0             
      0x3f800000,    // 1.0                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0x7fc00001,    // signaling NaN         
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            

      // vpu register f20
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            
      0x80000000,    // -zero                 
      0x00011111,    // 9.7958E-41            
      0x3f028f5c,    // 0.51                  
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            
      0x7fc00000,    // quiet NaN             

      // vpu register f21
      0x80000000,    // -zero                 
      0xffc00000,    // -quiet NaN            
      0x7fc00001,    // signaling NaN         
      0x007fffff,    // 1.1754942E-38         
      0x00000001,    // 1.4E-45 (+denorm)     
      0xffc00000,    // -quiet NaN            
      0x4b000000,    // 8388608.0             
      0x80011111,    // -9.7958E-41           

      // vpu register f22
      0xcb8c4b40,    // -18388608.0           
      0xffc00001,    // -signaling NaN        
      0x7f800000,    // inf                   
      0x007fffff,    // 1.1754942E-38         
      0x00000001,    // 1.4E-45 (+denorm)     
      0xffc00000,    // -quiet NaN            
      0x4b000000,    // 8388608.0             
      0x00011111,    // 9.7958E-41            

      // vpu register f23
      0x7fc00000,    // quiet NaN             
      0x80000000,    // -zero                 
      0x3f028f5c,    // 0.51                  
      0x7fc00001,    // signaling NaN         
      0x7f800000,    // inf                   
      0xcb000000,    // -8388608.0            
      0x7f800000,    // inf                   
      0x007fffff,    // 1.1754942E-38         

      // vpu register f24
      0xbf028f5c,    // -0.51                 
      0x4b000000,    // 8388608.0             
      0x7fc00000,    // quiet NaN             
      0xbf800000,    // -1.0                  
      0xff800000,    // -inf                  
      0x7fc00000,    // quiet NaN             
      0xcb000000,    // -8388608.0            
      0xbf028f5c,    // -0.51                 

      // vpu register f25
      0x00011111,    // 9.7958E-41            
      0x3f028f5c,    // 0.51                  
      0x7fc00000,    // quiet NaN             
      0x7f800000,    // inf                   
      0xffc00001,    // -signaling NaN        
      0x00000000,    // zero                  
      0x3f800000,    // 1.0                   
      0x00011111,    // 9.7958E-41            

      // vpu register f26
      0xcb000000,    // -8388608.0            
      0x4b000000,    // 8388608.0             
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f800000,    // 1.0                   
      0x3f800000,    // 1.0                   
      0x7f800000,    // inf                   
      0x7fc00001,    // signaling NaN         
      0xbf800000,    // -1.0                  

      // vpu register f27
      0x7f800000,    // inf                   
      0x3f800000,    // 1.0                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb000000,    // -8388608.0            
      0x3f028f5c,    // 0.51                  
      0xff800000,    // -inf                  
      0x7fc00000,    // quiet NaN             
      0xff800000,    // -inf                  

      // vpu register f28
      0xff800000,    // -inf                  
      0x4b8c4b40,    // 18388608.0            
      0x00000000,    // zero                  
      0xcb8c4b40,    // -18388608.0           
      0x80000000,    // -zero                 
      0xffc00000,    // -quiet NaN            
      0xffc00000,    // -quiet NaN            
      0x4b000000,    // 8388608.0             

      // vpu register f29
      0xff800000,    // -inf                  
      0xbf028f5c,    // -0.51                 
      0x7f800000,    // inf                   
      0x4b8c4b40,    // 18388608.0            
      0xcb000000,    // -8388608.0            
      0x7fc00000,    // quiet NaN             
      0xff800000,    // -inf                  
      0x7f800000,    // inf                   

      // vpu register f30
      0xcb000000,    // -8388608.0            
      0x3f028f5c,    // 0.51                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0x7f800000,    // inf                   
      0xff800000,    // -inf                  
      0xcb8c4b40,    // -18388608.0           
      0x7fc00001,    // signaling NaN         
      0x3f800000,    // 1.0                   

      // vpu register f31
      0x3f800000,    // 1.0                   
      0x7fc00000,    // quiet NaN             
      0x80000000,    // -zero                 
      0xff800000,    // -inf                  
      0x80000000,    // -zero                 
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf028f5c,    // -0.51                 
      0x3f800000     // 1.0                   
   };

   // this routine loads into all fp registers
   // due to the calling conventions, it restores fp saved registers at the end, overwriting the loaded values before returning
   // so, function calls cannot be used to load registers
   // registers will have to be loaded inside the same asm volatile call where they are used
   //do_load (operands);


   __asm__ __volatile__ (
         // enable all vpu lanes
         "addi x5, zero, -1\n"
         "mova.m.x x5\n"

         // enable scratchpad
         "addi x5, zero, 1\n"
         "csrrw zero, 0x810, x5\n"

         // tensor sequence: set scratchpad, do tensorload, do tensorwait, do tima 
         "add x14, %[operands], zero\n"	
         "andi x14, %[operands], -64\n"	
         "ori x14, x14, 11\n"	
         "li x29, 0x0048000000000007\n"
         "li x31, 0x40\n"
         "csrrw zero, 0x83F, x14\n"   // tensorload to scratchpad
         "fence\n"
         "csrrw zero, 0x830, zero\n"  // tensorwait for tensor load to finish
         "fence\n"
         "csrrw zero, 0x801, x29\n"     // tima
         "fence\n"
         "andi x29, x29, -2\n"	
         "csrrw zero, 0x801, x29\n"     // tima
         "fence\n"

         "flw.ps f0,  0 (%[operands])\n"
         "flw.ps f1,  32 (%[operands])\n"
         "flw.ps f2,  64 (%[operands])\n"
         "flw.ps f3,  96 (%[operands])\n"
         "flw.ps f4,  128 (%[operands])\n"
         "flw.ps f5,  160 (%[operands])\n"
         "flw.ps f6,  192 (%[operands])\n"
         "flw.ps f7,  224 (%[operands])\n"
         "flw.ps f8,  256 (%[operands])\n"
         "flw.ps f9,  288 (%[operands])\n"
         "flw.ps f10,  320 (%[operands])\n"
         "flw.ps f11,  352 (%[operands])\n"
         "flw.ps f12,  384 (%[operands])\n"
         "flw.ps f13,  416 (%[operands])\n"
         "flw.ps f14,  448 (%[operands])\n"
         "flw.ps f15,  480 (%[operands])\n"
         "flw.ps f16,  512 (%[operands])\n"
         "flw.ps f17,  544 (%[operands])\n"
         "flw.ps f18,  576 (%[operands])\n"
         "flw.ps f19,  608 (%[operands])\n"
         "flw.ps f20,  640 (%[operands])\n"
         "flw.ps f21,  672 (%[operands])\n"
         "flw.ps f22,  704 (%[operands])\n"
         "flw.ps f23,  736 (%[operands])\n"
         "flw.ps f24,  768 (%[operands])\n"
         "flw.ps f25,  800 (%[operands])\n"
         "flw.ps f26,  832 (%[operands])\n"
         "flw.ps f27,  864 (%[operands])\n"
         "flw.ps f28,  896 (%[operands])\n"
         "flw.ps f29,  928 (%[operands])\n"
         "flw.ps f30,  960 (%[operands])\n"
         "flw.ps f31,  992 (%[operands])\n"
         "fsgnjx.ps f31, f30, f31\n"                           // opcode=0x21ff2ffb
         "fsgnjx.s f30, f30, f31\n"                            // opcode=0x21ff2f53
         "fsll.pi f31, f30, f30\n"                             // opcode=0x07ef1ffb
         "cubesgnsc.ps f31, f31, f31\n"                        // opcode=0x89ffaffb
         "fsetm.pi m5, f30\n"                                  // opcode=0xe00f52fb
         "feq.pi f30, f30, f30\n"                              // opcode=0xa7ef2f7b
         "fltm.ps m2, f30, f30\n"                              // opcode=0xa1ef517b
         "fle.s x3, f30, f30\n"                               // opcode=0xa1ef0f53
         "fsetm.pi m5, f31\n"                                  // opcode=0xe00fd2fb
         "fadd.pi f31, f31, f30\n"                             // opcode=0x07ef8ffb
         "fclass.s t3, f31\n"                                 // opcode=0xe00f9f53
         "fswizz.ps f30, f31, 155\n"                           // opcode=0xe73fbf7b
         "fand.pi f31, f30, f31\n"                             // opcode=0x07ff7ffb
         "fltu.pi f31, f31, f31\n"                             // opcode=0xa7ffbffb
         "fpackreph.pi f30, f30\n"                             // opcode=0x260f1f7b
         // Asm output results to C code
         :
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "t3", "x3", "x4", "x19", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
            );
   C_TEST_PASS;
   return 0;
}
