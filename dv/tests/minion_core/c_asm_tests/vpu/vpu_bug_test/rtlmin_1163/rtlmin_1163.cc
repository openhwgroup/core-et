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
			 0xffc00001,    // -signaling NaN        
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0x007fffff,    // 1.1754942E-38         
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0x80011111,    // -9.7958E-41           
			 0x80011111,    // -9.7958E-41           
			 0xff800000,    // -inf                  
			 0x3f800000,    // 1.0                   

			 // vpu register f3
			 0x80011111,    // -9.7958E-41           
			 0xffc00001,    // -signaling NaN        
			 0xcb000000,    // -8388608.0            
			 0x4b8c4b40,    // 18388608.0            
			 0x4b8c4b40,    // 18388608.0            
			 0xcb000000,    // -8388608.0            
			 0xcb000000,    // -8388608.0            
			 0x80011111,    // -9.7958E-41           

			 // vpu register f4
			 0xbf800000,    // -1.0                  
			 0x7fc00000,    // quiet NaN             
			 0x4b000000,    // 8388608.0             
			 0x3f800000,    // 1.0                   
			 0x7f800000,    // inf                   
			 0x7f800000,    // inf                   
			 0x80000000,    // -zero                 
			 0x00011111,    // 9.7958E-41            

			 // vpu register f5
			 0x4b8c4b40,    // 18388608.0            
			 0xcb8c4b40,    // -18388608.0           
			 0xff800000,    // -inf                  
			 0x00011111,    // 9.7958E-41            
			 0x00000000,    // zero                  
			 0xffc00001,    // -signaling NaN        
			 0xbf028f5c,    // -0.51                 
			 0x80000000,    // -zero                 

			 // vpu register f6
			 0xffc00001,    // -signaling NaN        
			 0xffc00001,    // -signaling NaN        
			 0x00000000,    // zero                  
			 0x7fc00001,    // signaling NaN         
			 0x7fc00001,    // signaling NaN         
			 0xffc00001,    // -signaling NaN        
			 0x80011111,    // -9.7958E-41           
			 0x3f800000,    // 1.0                   

			 // vpu register f7
			 0xcb000000,    // -8388608.0            
			 0x7f800000,    // inf                   
			 0x3f800000,    // 1.0                   
			 0xcb000000,    // -8388608.0            
			 0x7f800000,    // inf                   
			 0x7f800000,    // inf                   
			 0x00000000,    // zero                  
			 0xbf800000,    // -1.0                  

			 // vpu register f8
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0x7fc00000,    // quiet NaN             
			 0x4b8c4b40,    // 18388608.0            
			 0xcb000000,    // -8388608.0            
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0x3f028f5c,    // 0.51                  
			 0x80000001,    // -1.4E-45 (-denorm)    
			 0x7f800000,    // inf                   

			 // vpu register f9
			 0x80000000,    // -zero                 
			 0x80011111,    // -9.7958E-41           
			 0xffc00000,    // -quiet NaN            
			 0x00011111,    // 9.7958E-41            
			 0x00000000,    // zero                  
			 0x7fc00001,    // signaling NaN         
			 0x7fc00001,    // signaling NaN         
			 0x7f800000,    // inf                   

			 // vpu register f10
			 0x7f800000,    // inf                   
			 0x3f028f5c,    // 0.51                  
			 0x80000001,    // -1.4E-45 (-denorm)    
			 0x4b8c4b40,    // 18388608.0            
			 0x00000000,    // zero                  
			 0xbf800000,    // -1.0                  
			 0xffc00001,    // -signaling NaN        
			 0xbf028f5c,    // -0.51                 

			 // vpu register f11
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0xffc00001,    // -signaling NaN        
			 0x7fc00001,    // signaling NaN         
			 0xffc00000,    // -quiet NaN            
			 0xffc00001,    // -signaling NaN        
			 0xffc00000,    // -quiet NaN            
			 0x00011111,    // 9.7958E-41            
			 0x3f800000,    // 1.0                   

			 // vpu register f12
			 0x7fc00001,    // signaling NaN         
			 0x7f800000,    // inf                   
			 0x007fffff,    // 1.1754942E-38         
			 0x4b8c4b40,    // 18388608.0            
			 0x00000000,    // zero                  
			 0xffc00001,    // -signaling NaN        
			 0x007fffff,    // 1.1754942E-38         
			 0x80000001,    // -1.4E-45 (-denorm)    

			 // vpu register f13
			 0x3f800000,    // 1.0                   
			 0x7fc00001,    // signaling NaN         
			 0x00000000,    // zero                  
			 0x80000000,    // -zero                 
			 0x7f800000,    // inf                   
			 0x80011111,    // -9.7958E-41           
			 0xffc00001,    // -signaling NaN        
			 0xffc00001,    // -signaling NaN        

			 // vpu register f14
			 0x3f028f5c,    // 0.51                  
			 0xffc00000,    // -quiet NaN            
			 0x00011111,    // 9.7958E-41            
			 0x7fc00001,    // signaling NaN         
			 0x7f800000,    // inf                   
			 0x7fc00000,    // quiet NaN             
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0x7fc00000,    // quiet NaN             

			 // vpu register f15
			 0xffc00001,    // -signaling NaN        
			 0xff800000,    // -inf                  
			 0xcb000000,    // -8388608.0            
			 0x3f028f5c,    // 0.51                  
			 0x3f800000,    // 1.0                   
			 0x4b000000,    // 8388608.0             
			 0x3f800000,    // 1.0                   
			 0xff800000,    // -inf                  

			 // vpu register f16
			 0x7f800000,    // inf                   
			 0x4b000000,    // 8388608.0             
			 0x80000000,    // -zero                 
			 0x80011111,    // -9.7958E-41           
			 0x4b8c4b40,    // 18388608.0            
			 0xffc00001,    // -signaling NaN        
			 0x80000000,    // -zero                 
			 0xcb000000,    // -8388608.0            

			 // vpu register f17
			 0xffc00001,    // -signaling NaN        
			 0xffc00001,    // -signaling NaN        
			 0xbf028f5c,    // -0.51                 
			 0x80000001,    // -1.4E-45 (-denorm)    
			 0xbf028f5c,    // -0.51                 
			 0x80000000,    // -zero                 
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0xffc00001,    // -signaling NaN        

			 // vpu register f18
			 0x80000001,    // -1.4E-45 (-denorm)    
			 0x4b8c4b40,    // 18388608.0            
			 0x7fc00000,    // quiet NaN             
			 0x7fc00001,    // signaling NaN         
			 0xff800000,    // -inf                  
			 0x4b000000,    // 8388608.0             
			 0xbf800000,    // -1.0                  
			 0x7f800000,    // inf                   

			 // vpu register f19
			 0x00000000,    // zero                  
			 0x80011111,    // -9.7958E-41           
			 0x80011111,    // -9.7958E-41           
			 0x00011111,    // 9.7958E-41            
			 0x00000000,    // zero                  
			 0x4b8c4b40,    // 18388608.0            
			 0x7fc00001,    // signaling NaN         
			 0x00000000,    // zero                  

			 // vpu register f20
			 0x00011111,    // 9.7958E-41            
			 0xffc00001,    // -signaling NaN        
			 0x7f800000,    // inf                   
			 0x00011111,    // 9.7958E-41            
			 0xffc00001,    // -signaling NaN        
			 0xbf028f5c,    // -0.51                 
			 0x7fc00000,    // quiet NaN             
			 0x007fffff,    // 1.1754942E-38         

			 // vpu register f21
			 0x7fc00001,    // signaling NaN         
			 0x80000000,    // -zero                 
			 0xbf800000,    // -1.0                  
			 0xbf028f5c,    // -0.51                 
			 0x80000000,    // -zero                 
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0x80011111,    // -9.7958E-41           
			 0x7fc00001,    // signaling NaN         

			 // vpu register f22
			 0x007fffff,    // 1.1754942E-38         
			 0x80000000,    // -zero                 
			 0x7f800000,    // inf                   
			 0x80000001,    // -1.4E-45 (-denorm)    
			 0xffc00000,    // -quiet NaN            
			 0x7f800000,    // inf                   
			 0xbf800000,    // -1.0                  
			 0xff800000,    // -inf                  

			 // vpu register f23
			 0x7f800000,    // inf                   
			 0x00011111,    // 9.7958E-41            
			 0x4b000000,    // 8388608.0             
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0x7f800000,    // inf                   
			 0x3f028f5c,    // 0.51                  
			 0xbf028f5c,    // -0.51                 
			 0x3f028f5c,    // 0.51                  

			 // vpu register f24
			 0x00011111,    // 9.7958E-41            
			 0xcb000000,    // -8388608.0            
			 0xbf800000,    // -1.0                  
			 0xcb000000,    // -8388608.0            
			 0x4b000000,    // 8388608.0             
			 0x80011111,    // -9.7958E-41           
			 0xbf028f5c,    // -0.51                 
			 0xffc00000,    // -quiet NaN            

			 // vpu register f25
			 0x80011111,    // -9.7958E-41           
			 0x3f028f5c,    // 0.51                  
			 0x00011111,    // 9.7958E-41            
			 0xbf028f5c,    // -0.51                 
			 0x007fffff,    // 1.1754942E-38         
			 0x3f028f5c,    // 0.51                  
			 0x00000000,    // zero                  
			 0x7fc00000,    // quiet NaN             

			 // vpu register f26
			 0x7f800000,    // inf                   
			 0xcb000000,    // -8388608.0            
			 0xffc00000,    // -quiet NaN            
			 0x80000001,    // -1.4E-45 (-denorm)    
			 0xbf028f5c,    // -0.51                 
			 0x7f800000,    // inf                   
			 0x80011111,    // -9.7958E-41           
			 0xcb8c4b40,    // -18388608.0           

			 // vpu register f27
			 0xcb8c4b40,    // -18388608.0           
			 0xffc00001,    // -signaling NaN        
			 0xffc00001,    // -signaling NaN        
			 0x80011111,    // -9.7958E-41           
			 0x7fc00000,    // quiet NaN             
			 0x00000000,    // zero                  
			 0xcb000000,    // -8388608.0            
			 0x7fc00001,    // signaling NaN         

			 // vpu register f28
			 0x80000001,    // -1.4E-45 (-denorm)    
			 0xbf028f5c,    // -0.51                 
			 0x4b8c4b40,    // 18388608.0            
			 0x007fffff,    // 1.1754942E-38         
			 0x7f800000,    // inf                   
			 0xffc00000,    // -quiet NaN            
			 0xbf028f5c,    // -0.51                 
			 0xffc00000,    // -quiet NaN            

			 // vpu register f29
			 0x80011111,    // -9.7958E-41           
			 0x3f800000,    // 1.0                   
			 0xffc00000,    // -quiet NaN            
			 0xff800000,    // -inf                  
			 0x4b000000,    // 8388608.0             
			 0xff800000,    // -inf                  
			 0x80000000,    // -zero                 
			 0x3f028f5c,    // 0.51                  

			 // vpu register f30
			 0xcb8c4b40,    // -18388608.0           
			 0xff800000,    // -inf                  
			 0xbf800000,    // -1.0                  
			 0x7f800000,    // inf                   
			 0xcb8c4b40,    // -18388608.0           
			 0xbf800000,    // -1.0                  
			 0x00011111,    // 9.7958E-41            
			 0xcb000000,    // -8388608.0            

			 // vpu register f31
			 0x7f800000,    // inf                   
			 0xbf800000,    // -1.0                  
			 0x00000000,    // zero                  
			 0x4b000000,    // 8388608.0             
			 0x80011111,    // -9.7958E-41           
			 0xcb8c4b40,    // -18388608.0           
			 0x7fc00001,    // signaling NaN         
			 0x3f800000     // 1.0                   
	};
        C_TEST_START;
	__asm__ __volatile__ (
		"li x5, 0xffffffffffffffff\n"
		"mova.m.x x5\n"
		"flw.ps f17,  544 (%[operands])\n"
		//RTLMIN-5479"fsin.ps f26, f17\n"                                  // opcode=0x58688d7b
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
	    "fnmadd.ps f20, f25, f21, f7, dyn\n"                  // opcode=0x3f5cfa5b
	// Asm output results to C code
	:
	// Asm input data from C code
	: [operands] "r" (operands)
	// Registers used in Asm so C knows they change
	: "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
	);
         C_TEST_PASS;
	return 0;
}
