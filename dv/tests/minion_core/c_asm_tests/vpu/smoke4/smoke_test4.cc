/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

//Other ESR
#define GFX_TRAP_ESR_PTR        0x80F40000ULL

volatile uint64_t *gfxTrapEsrPtr        = (uint64_t*)GFX_TRAP_ESR_PTR;  

int main() {

	C_TEST_START;

	// Avoid using x2 and x8. x2 is stack pointer. Assembler does not like if we use x8 and forbid it from using x8 - it errors out

   *gfxTrapEsrPtr =  (uintptr_t) (0); // Avoid trap for gfx instructions

	volatile uint32_t operands[256] __attribute__ ((aligned (4096))) = {

			                         0x3392B5F7,
                         0x00000000,
                         0x3C7F8008,
                         0x00000001,
                         0xB3FFFC00,
                         0x00000002,
                         0xBF00FFF0,
                         0x00000004,
                         0x4EB9BA4A,
                         0x00000008,
                         0x37BFDFFE,
                         0x00000010,
                         0x4EFFFD7F,
                         0x00000020,
                         0x3E23E59C,
                         0x00000040,
                         0xBFF84000,
                         0x00000080,
                         0xA27F2000,
                         0x00000100,
                         0x417FFFBD,
                         0x00000200,
                         0xB31007FF,
                         0x00000400,
                         0xC2FFEFBF,
                         0x00000800,
                         0x437FFC03,
                         0x00001000,
                         0xBD8027FF,
                         0x00002000,
                         0xBE2E4CF2,
                         0x00004000,
                         0x3F8FBFFF,
                         0x00008000,
                         0x43800408,
                         0x00010000,
                         0x493C4CCD,
                         0x00020000,
                         0x007C0FFF,
                         0x00040000,
                         0x4B8000BF,
                         0x00080000,
                         0x0207FFF0,
                         0x00100000,
                         0xDE7FFDEE,
                         0x00200000,
                         0x00800005,
                         0x00400000,
                         0x437EFFBF,
                         0x00600000,
                         0x31000077,
                         0x00700000,
                         0x3C0000FC,
                         0x00780000,
                         0x7D00003A,
                         0x007C0000,
                         0xB900401F,
                         0x007E0000,
                         0x408803FF,
                         0x007F0000,
                         0xBD800F7F,
                         0x007F8000,
                         0x3E7FFFFD,
                         0x007FC000,
                         0x407FFEFE,
                         0x007FE000,
                         0xDE000080,
                         0x007FF000,
                         0xC007FFFD,
                         0x007FF800,
                         0x3DCE7BCE,
                         0x007FFC00,
                         0xCBBEFFFF,
                         0x007FFE00,
                         0x4EB11295,
                         0x007FFF00,
                         0xBE828134,
                         0x007FFF80,
                         0xC6F7FFEF,
                         0x007FFFC0,
                         0x3B008020,
                         0x007FFFE0,
                         0x02FF4000,
                         0x007FFFF0,
                         0xBE758E43,
                         0x007FFFF8,
                         0x3381FFFF,
                         0x007FFFFC,
                         0x3F000005,
                         0x007FFFFE,
                         0xBEF77FFE,
                         0x007FFFFF,
                         0xFEFFFBFF,
                         0x007FFFFD,
                         0x3F07FFEE,
                         0x007FFFFB,
                         0x4080000F,
                         0x007FFFF7,
                         0x3E8E4DCD,
                         0x007FFFEF,
                         0x2A10001F,
                         0x007FFFDF,
                         0xCBE4AF23,
                         0x007FFFBF,
                         0x4E6711C1,
                         0x007FFF7F,
                         0x4103BFFF,
                         0x007FFEFF,
                         0xC4A48BA1,
                         0x007FFDFF,
                         0xBE789551,
                         0x007FFBFF,
                         0xC00CED99,
                         0x007FF7FF,
                         0xB38007FD,
                         0x007FEFFF,
                         0x3D9A8471,
                         0x007FDFFF,
                         0xFF7FFDC0,
                         0x007FBFFF,
                         0xCF80FFFF,
                         0x007F7FFF,
                         0xC1FFF7DF,
                         0x007EFFFF,
                         0x5F0004FF,
                         0x007DFFFF,
                         0xBF00007E,
                         0x007BFFFF,
                         0xC18000FC,
                         0x0077FFFF,
                         0x3DE995BE,
                         0x006FFFFF,
                         0xF3CADECD,
                         0x005FFFFF,
                         0x3A41E749,
                         0x003FFFFF,
                         0xFB802020,
                         0x001FFFFF,
                         0xFF800201,
                         0x000FFFFF,
                         0x2D800C00,
                         0x0007FFFF,
                         0x404BF024,
                         0x0003FFFF,
                         0xDFFFE0FF,
                         0x0001FFFF,
                         0xB1002002,
                         0x0000FFFF,
                         0xC503FFFF,
                         0x00007FFF,
                         0x541FFEFF,
                         0x00003FFF,
                         0x3E780002,
                         0x00001FFF,
                         0xDE7EFFDF,
                         0x00000FFF,
                         0x9E8FFFF7,
                         0x000007FF,
                         0x815FFFFF,
                         0x000003FF,
                         0xBFFFBE00,
                         0x000001FF,
                         0xC18001FE,
                         0x000000FF,
                         0xBC0003FE,
                         0x0000007F,
                         0xBE000FFE,
                         0x0000003F,
                         0x0BFFFFEC,
                         0x0000001F,
                         0x3D80040E,
                         0x0000000F,
                         0x3E821FFE,
                         0x00000007,
                         0x5C7FFFF3,
                         0x00000003,
                         0xC04D8FFB,
                         0x00800000,
                         0x4BE5E3BA,
                         0x00800001,
                         0x80C3491F,
                         0x00800002,
                         0x358007EF,
                         0x00800004,
                         0x4F8023FF,
                         0x00800008,
                         0xBE1FFFBE,
                         0x00800010,
                         0xBFFEFFF8,
                         0x00800020,
                         0xFF7EFFF8,
                         0x00800040,
                         0xC2700007,
                         0x00800080,
                         0x339FFFFD,
                         0x00800100,
                         0xB3B73C08,
                         0x00800200,
                         0x5FF7FFFE,
                         0x00800400,
                         0xBA7FF807,
                         0x00800800,
                         0xFFCFFFFF,
                         0x00801000,
                         0x9A3F900D,
                         0x00802000,
                         0xBF2001FF,
                         0x00804000,
                         0xED000EFF,
                         0x00808000,
                         0xFD3FFFEF,
                         0x00810000,
                         0xB8FF0080,
                         0x00820000,
                         0xFF000026,
                         0x00840000,
                         0xFEFFC00E,
                         0x00880000,
                         0xF6BFFFFD,
                         0x00900000,
                         0xC07E7FFF,
                         0x00A00000,
                         0x56000000,
                         0x00C00000,
                         0x3F4052D7,
                         0x00E00000,
                         0xC1F7E000,
                         0x00F00000,
                         0x589FDFFF,
                         0x00F80000,
                         0x00AEBA86,
                         0x00FC0000,
                         0x7EFFFFFE,
                         0x00FE0000,
                         0x5E200FFE,
                         0x00FF0000,
                         0xBDC9558C,
                         0x00FF8000,
                         0x40C4F047,
                         0x00FFC000,
                         0x3DFFFF84,
                         0x00FFE000,
                         0x3C080020,
                         0x00FFF000,
                         0x3D008007,
                         0x00FFF800,
                         0x8100FFF6,
                         0x00FFFC00,
                         0xEF800000,
                         0x00FFFE00,
                         0xDF5760BC,
                         0x00FFFF00,
                         0x81482556,
                         0x00FFFF80,
                         0x43000400,
                         0x00FFFFC0

                
	};

	__asm__ __volatile__ (

         //enable lanes
         "li x5, 0xff\n"
         "mova.m.x x5\n"

         //load operands
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


        "fnmadd.s f1, f0, f13, f12, dyn\n"
        //RTLMIN-5479"fsin.ps f17, f1\n"                                   
	"fclass.s x18, f1\n"                                  
	"fsgnj.s f23, f1, f1\n"
        "fswizz.ps f30, f13, 67\n"                            
	"fandi.pi f17, f1, 70\n" 
	"fmax.pi f17, f13, f13\n"      
	"fcvt.ps.f16 f30, f12\n"
	"fcvt.pwu.ps f21, f14, dyn\n"
	"fbci.ps f16, 382221\n"                               
	"fle.ps f20, f15, f12\n"                              
	"fmsub.ps f16, f1, f15, f1, dyn\n"     
	"fsub.pi f21, f14, f12\n"                             
	"fadd.ps f27, f1, f1, dyn\n"      
	"flt.s x16, f14, f15\n"                               
	"cubefaceidx.ps f1, f1, f13\n"       
	"fround.ps f28, f14, dyn\n"                           
	"fcvt.pw.ps f19, f13, rtz\n"             
	"feqm.ps m0, f14, f13\n"                              
	"fnmsub.s f27, f14, f15, f15, rdn\n"                  
	//RTLMIN-5479"frsq.ps f31, f12\n"                                  
	"fmin.pi f22, f14, f1\n"                              
	"fmadd.ps f25, f13, f0, f0, dyn\n" 
	"fmaxu.pi f25, f12, f0\n"                             
	//RTLMIN-5479"frsq.ps f17, f12\n"                                  
	"fsrl.pi f27, f15, f12\n"                             
	"faddi.pi f22, f14, 271\n"     
	"fmul.pi f28, f1, f14\n"   
	"fnmadd.ps f27, f12, f12, f15, dyn\n"                 
	"fcvt.pw.ps f25, f1, dyn\n"                 
	"fcvt.pw.ps f17, f12, dyn\n"       
	"fsat8.pi f26, f13\n"                                 
	"flt.pi f22, f1, f21\n"   
	"fmaxu.pi f31, f1, f1\n"
	"flem.ps m0, f0, f15\n"       
	"fmulhu.pi f1, f1, f1\n" 
	"fsgnj.s f24, f13, f0\n"     
	"cubesgntc.ps f26, f0, f14\n"                         
	"fcvt.s.w f0, zero\n"      
        "fnmadd.ps f24, f1, f2, f16, dyn\n"   
        "fmadd.ps f25, f3, f4, f0, dyn\n" 
	"fmaxu.pi f25, f12, f5\n"                             
	//RTLMIN-5479"frsq.ps f17, f6\n" 
        "fnmsub.s f27, f18, f7, f17, rdn\n"  
        "fcvt.pw.ps f25, f9, dyn\n" 
        //RTLMIN-5479"fsin.ps f17, f10\n" 
        "fmul.ps f28, f11, f17\n" 

        "fnmadd.s f1, f0, f13, f12, dyn\n"
        //RTLMIN-5479"fsin.ps f17, f1\n"                                   
	"fclass.s x18, f1\n"                                  
	"fsgnj.s f23, f1, f1\n"
        "fswizz.ps f30, f13, 67\n"                            
	"fandi.pi f17, f1, 70\n" 
	"fmax.pi f17, f13, f13\n"      
	"fcvt.ps.f16 f30, f12\n"
	"fcvt.pwu.ps f21, f14, dyn\n"
	"fbci.ps f16, 382221\n"                               
	"fle.ps f20, f15, f12\n"                              
	"fmsub.ps f16, f1, f15, f1, dyn\n"     
	"fsub.pi f21, f14, f12\n"                             
	"fadd.ps f27, f1, f1, dyn\n"      
	"flt.s x16, f14, f15\n"                               
	"cubefaceidx.ps f1, f1, f13\n"       
	"fround.ps f28, f14, dyn\n"                           
	"fcvt.pw.ps f19, f13, rtz\n"             
	"feqm.ps m0, f14, f13\n"                              
	"fnmsub.s f27, f14, f15, f15, rdn\n"                  
	//RTLMIN-5479"frsq.ps f31, f12\n"                                  
	"fmin.pi f22, f14, f1\n"                              
	"fmadd.ps f25, f13, f0, f0, dyn\n" 
	"fmaxu.pi f25, f12, f0\n"                             
	//RTLMIN-5479"frsq.ps f17, f12\n"                                  
	"fsrl.pi f27, f15, f12\n"                             
	"faddi.pi f22, f14, 271\n"     
	"fmul.pi f28, f1, f14\n"   
	"fnmadd.ps f27, f12, f12, f15, dyn\n"                 
	"fcvt.pw.ps f25, f1, dyn\n"                 
	"fcvt.pw.ps f17, f12, dyn\n"       
	"fsat8.pi f26, f13\n"                                 
	"flt.pi f22, f1, f21\n"   
	"fmaxu.pi f31, f1, f1\n"
	"flem.ps m0, f0, f15\n"       
	"fmulhu.pi f1, f1, f1\n" 
	"fsgnj.s f24, f13, f0\n"     
	"cubesgntc.ps f26, f0, f14\n"                         
	"fcvt.s.w f0, zero\n"      
        "fnmadd.ps f24, f1, f2, f16, dyn\n"   
        "fmadd.ps f25, f3, f4, f0, dyn\n" 
	"fmaxu.pi f25, f12, f5\n"                             
	//RTLMIN-5479"frsq.ps f17, f6\n" 
        "fnmsub.s f27, f18, f7, f17, rdn\n"  
        "fcvt.pw.ps f25, f9, dyn\n" 
        //RTLMIN-5479"fsin.ps f17, f10\n" 
        "fmul.ps f28, f11, f17\n"   

        "fnmadd.s f1, f0, f13, f12, dyn\n"
        //RTLMIN-5479"fsin.ps f17, f1\n"                                   
	"fclass.s x18, f1\n"                                  
	"fsgnj.s f23, f1, f1\n"
        "fswizz.ps f30, f13, 67\n"                            
	"fandi.pi f17, f1, 70\n" 
	"fmax.pi f17, f13, f13\n"      
	"fcvt.ps.f16 f30, f12\n"
	"fcvt.pwu.ps f21, f14, dyn\n"
	"fbci.ps f16, 382221\n"                               
	"fle.ps f20, f15, f12\n"                              
	"fmsub.ps f16, f1, f15, f1, dyn\n"     
	"fsub.pi f21, f14, f12\n"                             
	"fadd.ps f27, f1, f1, dyn\n"      
	"flt.s x16, f14, f15\n"                               
	"cubefaceidx.ps f1, f1, f13\n"       
	"fround.ps f28, f14, dyn\n"                           
	"fcvt.pw.ps f19, f13, rtz\n"             
	"feqm.ps m0, f14, f13\n"                              
	"fnmsub.s f27, f14, f15, f15, rdn\n"                  
	//RTLMIN-5479"frsq.ps f31, f12\n"                                  
	"fmin.pi f22, f14, f1\n"                              
	"fmadd.ps f25, f13, f0, f0, dyn\n" 
	"fmaxu.pi f25, f12, f0\n"                             
	//RTLMIN-5479"frsq.ps f17, f12\n"                                  
	"fsrl.pi f27, f15, f12\n"                             
	"faddi.pi f22, f14, 271\n"     
	"fmul.pi f28, f1, f14\n"   
	"fnmadd.ps f27, f12, f12, f15, dyn\n"                 
	"fcvt.pw.ps f25, f1, dyn\n"                 
	"fcvt.pw.ps f17, f12, dyn\n"       
	"fsat8.pi f26, f13\n"                                 
	"flt.pi f22, f1, f21\n"   
	"fmaxu.pi f31, f1, f1\n"
	"flem.ps m0, f0, f15\n"       
	"fmulhu.pi f1, f1, f1\n" 
	"fsgnj.s f24, f13, f0\n"     
	"cubesgntc.ps f26, f0, f14\n"                         
	"fcvt.s.w f0, zero\n"      
        "fnmadd.ps f24, f1, f2, f16, dyn\n"   
        "fmadd.ps f25, f3, f4, f0, dyn\n" 
	"fmaxu.pi f25, f12, f5\n"                             
	//RTLMIN-5479"frsq.ps f17, f6\n" 
        "fnmsub.s f27, f18, f7, f17, rdn\n"  
        "fcvt.pw.ps f25, f9, dyn\n" 
        //RTLMIN-5479"fsin.ps f17, f10\n" 
        "fmul.ps f28, f11, f17\n"   

        "fnmadd.s f1, f0, f13, f12, dyn\n"
        //RTLMIN-5479"fsin.ps f17, f1\n"                                   
	"fclass.s x18, f1\n"                                  
	"fsgnj.s f23, f1, f1\n"
        "fswizz.ps f30, f13, 67\n"                            
	"fandi.pi f17, f1, 70\n" 
	"fmax.pi f17, f13, f13\n"      
	"fcvt.ps.f16 f30, f12\n"
	"fcvt.pwu.ps f21, f14, dyn\n"
	"fbci.ps f16, 382221\n"                               
	"fle.ps f20, f15, f12\n"                              
	"fmsub.ps f16, f1, f15, f1, dyn\n"     
	"fsub.pi f21, f14, f12\n"                             
	"fadd.ps f27, f1, f1, dyn\n"      
	"flt.s x16, f14, f15\n"                               
	"cubefaceidx.ps f1, f1, f13\n"       
	"fround.ps f28, f14, dyn\n"                           
	"fcvt.pw.ps f19, f13, rtz\n"             
	"feqm.ps m0, f14, f13\n"                              
	"fnmsub.s f27, f14, f15, f15, rdn\n"                  
	//RTLMIN-5479"frsq.ps f31, f12\n"                                  
	"fmin.pi f22, f14, f1\n"                              
	"fmadd.ps f25, f13, f0, f0, dyn\n" 
	"fmaxu.pi f25, f12, f0\n"                             
	//RTLMIN-5479"frsq.ps f17, f12\n"                                  
	"fsrl.pi f27, f15, f12\n"                             
	"faddi.pi f22, f14, 271\n"     
	"fmul.pi f28, f1, f14\n"   
	"fnmadd.ps f27, f12, f12, f15, dyn\n"                 
	"fcvt.pw.ps f25, f1, dyn\n"                 
	"fcvt.pw.ps f17, f12, dyn\n"       
	"fsat8.pi f26, f13\n"                                 
	"flt.pi f22, f1, f21\n"   
	"fmaxu.pi f31, f1, f1\n"
	"flem.ps m0, f0, f15\n"       
	"fmulhu.pi f1, f1, f1\n" 
	"fsgnj.s f24, f13, f0\n"     
	"cubesgntc.ps f26, f0, f14\n"                         
	"fcvt.s.w f0, zero\n"      
        "fnmadd.ps f24, f1, f2, f16, dyn\n"   
        "fmadd.ps f25, f3, f4, f0, dyn\n" 
	"fmaxu.pi f25, f12, f5\n"                             
	//RTLMIN-5479"frsq.ps f17, f6\n" 
        "fnmsub.s f27, f18, f7, f17, rdn\n"  
        "fcvt.pw.ps f25, f9, dyn\n" 
        //RTLMIN-5479"fsin.ps f17, f10\n" 
        "fmul.ps f28, f11, f17\n"   

        "fnmadd.s f1, f0, f13, f12, dyn\n"
        //RTLMIN-5479"fsin.ps f17, f1\n"                                   
	"fclass.s x18, f1\n"                                  
	"fsgnj.s f23, f1, f1\n"
        "fswizz.ps f30, f13, 67\n"                            
	"fandi.pi f17, f1, 70\n" 
	"fmax.pi f17, f13, f13\n"      
	"fcvt.ps.f16 f30, f12\n"
	"fcvt.pwu.ps f21, f14, dyn\n"
	"fbci.ps f16, 382221\n"                               
	"fle.ps f20, f15, f12\n"                              
	"fmsub.ps f16, f1, f15, f1, dyn\n"     
	"fsub.pi f21, f14, f12\n"                             
	"fadd.ps f27, f1, f1, dyn\n"      
	"flt.s x16, f14, f15\n"                               
	"cubefaceidx.ps f1, f1, f13\n"       
	"fround.ps f28, f14, dyn\n"                           
	"fcvt.pw.ps f19, f13, rtz\n"             
	"feqm.ps m0, f14, f13\n"                              
	"fnmsub.s f27, f14, f15, f15, rdn\n"                  
	//RTLMIN-5479"frsq.ps f31, f12\n"                                  
	"fmin.pi f22, f14, f1\n"                              
	"fmadd.ps f25, f13, f0, f0, dyn\n" 
	"fmaxu.pi f25, f12, f0\n"                             
	//RTLMIN-5479"frsq.ps f17, f12\n"                                  
	"fsrl.pi f27, f15, f12\n"                             
	"faddi.pi f22, f14, 271\n"     
	"fmul.pi f28, f1, f14\n"   
	"fnmadd.ps f27, f12, f12, f15, dyn\n"                 
	"fcvt.pw.ps f25, f1, dyn\n"                 
	"fcvt.pw.ps f17, f12, dyn\n"       
	"fsat8.pi f26, f13\n"                                 
	"flt.pi f22, f1, f21\n"   
	"fmaxu.pi f31, f1, f1\n"
	"flem.ps m0, f0, f15\n"       
	"fmulhu.pi f1, f1, f1\n" 
	"fsgnj.s f24, f13, f0\n"     
	"cubesgntc.ps f26, f0, f14\n"                         
	"fcvt.s.w f0, zero\n"      
        "fnmadd.ps f24, f1, f2, f16, dyn\n"   
        "fmadd.ps f25, f3, f4, f0, dyn\n" 
	"fmaxu.pi f25, f12, f5\n"                             
	//RTLMIN-5479"frsq.ps f17, f6\n" 
        "fnmsub.s f27, f18, f7, f17, rdn\n"  
        "fcvt.pw.ps f25, f9, dyn\n" 
        //RTLMIN-5479"fsin.ps f17, f10\n" 
        "fmul.ps f28, f11, f17\n"   

        "fnmadd.s f1, f0, f13, f12, dyn\n"
        //RTLMIN-5479"fsin.ps f17, f1\n"                                   
	"fclass.s x18, f1\n"                                  
	"fsgnj.s f23, f1, f1\n"
        "fswizz.ps f30, f13, 67\n"                            
	"fandi.pi f17, f1, 70\n" 
	"fmax.pi f17, f13, f13\n"      
	"fcvt.ps.f16 f30, f12\n"
	"fcvt.pwu.ps f21, f14, dyn\n"
	"fbci.ps f16, 382221\n"                               
	"fle.ps f20, f15, f12\n"                              
	"fmsub.ps f16, f1, f15, f1, dyn\n"     
	"fsub.pi f21, f14, f12\n"                             
	"fadd.ps f27, f1, f1, dyn\n"      
	"flt.s x16, f14, f15\n"                               
	"cubefaceidx.ps f1, f1, f13\n"       
	"fround.ps f28, f14, dyn\n"                           
	"fcvt.pw.ps f19, f13, rtz\n"             
	"feqm.ps m0, f14, f13\n"                              
	"fnmsub.s f27, f14, f15, f15, rdn\n"                  
	//RTLMIN-5479"frsq.ps f31, f12\n"                                  
	"fmin.pi f22, f14, f1\n"                              
	"fmadd.ps f25, f13, f0, f0, dyn\n" 
	"fmaxu.pi f25, f12, f0\n"                             
	//RTLMIN-5479"frsq.ps f17, f12\n"                                  
	"fsrl.pi f27, f15, f12\n"                             
	"faddi.pi f22, f14, 271\n"     
	"fmul.pi f28, f1, f14\n"   
	"fnmadd.ps f27, f12, f12, f15, dyn\n"                 
	"fcvt.pw.ps f25, f1, dyn\n"                 
	"fcvt.pw.ps f17, f12, dyn\n"       
	"fsat8.pi f26, f13\n"                                 
	"flt.pi f22, f1, f21\n"   
	"fmaxu.pi f31, f1, f1\n"
	"flem.ps m0, f0, f15\n"       
	"fmulhu.pi f1, f1, f1\n" 
	"fsgnj.s f24, f13, f0\n"     
	"cubesgntc.ps f26, f0, f14\n"                         
	"fcvt.s.w f0, zero\n"      
        "fnmadd.ps f24, f1, f2, f16, dyn\n"   
        "fmadd.ps f25, f3, f4, f0, dyn\n" 
	"fmaxu.pi f25, f12, f5\n"                             
	//RTLMIN-5479"frsq.ps f17, f6\n" 
        "fnmsub.s f27, f18, f7, f17, rdn\n"  
        "fcvt.pw.ps f25, f9, dyn\n" 
        //RTLMIN-5479"fsin.ps f17, f10\n" 
        "fmul.ps f28, f11, f17\n"   

        "fnmadd.s f1, f0, f13, f12, dyn\n"
        //RTLMIN-5479"fsin.ps f17, f1\n"                                   
	"fclass.s x18, f1\n"                                  
	"fsgnj.s f23, f1, f1\n"
        "fswizz.ps f30, f13, 67\n"                            
	"fandi.pi f17, f1, 70\n" 
	"fmax.pi f17, f13, f13\n"      
	"fcvt.ps.f16 f30, f12\n"
	"fcvt.pwu.ps f21, f14, dyn\n"
	"fbci.ps f16, 382221\n"                               
	"fle.ps f20, f15, f12\n"                              
	"fmsub.ps f16, f1, f15, f1, dyn\n"     
	"fsub.pi f21, f14, f12\n"                             
	"fadd.ps f27, f1, f1, dyn\n"      
	"flt.s x16, f14, f15\n"                               
	"cubefaceidx.ps f1, f1, f13\n"       
	"fround.ps f28, f14, dyn\n"                           
	"fcvt.pw.ps f19, f13, rtz\n"             
	"feqm.ps m0, f14, f13\n"                              
	"fnmsub.s f27, f14, f15, f15, rdn\n"                  
	//RTLMIN-5479"frsq.ps f31, f12\n"                                  
	"fmin.pi f22, f14, f1\n"                              
	"fmadd.ps f25, f13, f0, f0, dyn\n" 
	"fmaxu.pi f25, f12, f0\n"                             
	//RTLMIN-5479"frsq.ps f17, f12\n"                                  
	"fsrl.pi f27, f15, f12\n"                             
	"faddi.pi f22, f14, 271\n"     
	"fmul.pi f28, f1, f14\n"   
	"fnmadd.ps f27, f12, f12, f15, dyn\n"                 
	"fcvt.pw.ps f25, f1, dyn\n"                 
	"fcvt.pw.ps f17, f12, dyn\n"       
	"fsat8.pi f26, f13\n"                                 
	"flt.pi f22, f1, f21\n"   
	"fmaxu.pi f31, f1, f1\n"
	"flem.ps m0, f0, f15\n"       
	"fmulhu.pi f1, f1, f1\n" 
	"fsgnj.s f24, f13, f0\n"     
	"cubesgntc.ps f26, f0, f14\n"                         
	"fcvt.s.w f0, zero\n"      
        "fnmadd.ps f24, f1, f2, f16, dyn\n"   
        "fmadd.ps f25, f3, f4, f0, dyn\n" 
	"fmaxu.pi f25, f12, f5\n"                             
	//RTLMIN-5479"frsq.ps f17, f6\n" 
        "fnmsub.s f27, f18, f7, f17, rdn\n"  
        "fcvt.pw.ps f25, f9, dyn\n" 
        //RTLMIN-5479"fsin.ps f17, f10\n" 
        "fmul.ps f28, f11, f17\n"   

        "fnmadd.s f1, f0, f13, f12, dyn\n"
        //RTLMIN-5479"fsin.ps f17, f1\n"                                   
	"fclass.s x18, f1\n"                                  
	"fsgnj.s f23, f1, f1\n"
        "fswizz.ps f30, f13, 67\n"                            
	"fandi.pi f17, f1, 70\n" 
	"fmax.pi f17, f13, f13\n"      
	"fcvt.ps.f16 f30, f12\n"
	"fcvt.pwu.ps f21, f14, dyn\n"
	"fbci.ps f16, 382221\n"                               
	"fle.ps f20, f15, f12\n"                              
	"fmsub.ps f16, f1, f15, f1, dyn\n"     
	"fsub.pi f21, f14, f12\n"                             
	"fadd.ps f27, f1, f1, dyn\n"      
	"flt.s x16, f14, f15\n"                               
	"cubefaceidx.ps f1, f1, f13\n"       
	"fround.ps f28, f14, dyn\n"                           
	"fcvt.pw.ps f19, f13, rtz\n"             
	"feqm.ps m0, f14, f13\n"                              
	"fnmsub.s f27, f14, f15, f15, rdn\n"                  
	//RTLMIN-5479"frsq.ps f31, f12\n"                                  
	"fmin.pi f22, f14, f1\n"                              
	"fmadd.ps f25, f13, f0, f0, dyn\n" 
	"fmaxu.pi f25, f12, f0\n"                             
	//RTLMIN-5479"frsq.ps f17, f12\n"                                  
	"fsrl.pi f27, f15, f12\n"                             
	"faddi.pi f22, f14, 271\n"     
	"fmul.pi f28, f1, f14\n"   
	"fnmadd.ps f27, f12, f12, f15, dyn\n"                 
	"fcvt.pw.ps f25, f1, dyn\n"                 
	"fcvt.pw.ps f17, f12, dyn\n"       
	"fsat8.pi f26, f13\n"                                 
	"flt.pi f22, f1, f21\n"   
	"fmaxu.pi f31, f1, f1\n"
	"flem.ps m0, f0, f15\n"       
	"fmulhu.pi f1, f1, f1\n" 
	"fsgnj.s f24, f13, f0\n"     
	"cubesgntc.ps f26, f0, f14\n"                         
	"fcvt.s.w f0, zero\n"      
        "fnmadd.ps f24, f1, f2, f16, dyn\n"   
        "fmadd.ps f25, f3, f4, f0, dyn\n" 
	"fmaxu.pi f25, f12, f5\n"                             
	//RTLMIN-5479"frsq.ps f17, f6\n" 
        "fnmsub.s f27, f18, f7, f17, rdn\n"  
        "fcvt.pw.ps f25, f9, dyn\n" 
        //RTLMIN-5479"fsin.ps f17, f10\n" 
        "fmul.ps f28, f11, f17\n"   

        "fnmadd.s f1, f0, f13, f12, dyn\n"
        //RTLMIN-5479"fsin.ps f17, f1\n"                                   
	"fclass.s x18, f1\n"                                  
	"fsgnj.s f23, f1, f1\n"
        "fswizz.ps f30, f13, 67\n"                            
	"fandi.pi f17, f1, 70\n" 
	"fmax.pi f17, f13, f13\n"      
	"fcvt.ps.f16 f30, f12\n"
	"fcvt.pwu.ps f21, f14, dyn\n"
	"fbci.ps f16, 382221\n"                               
	"fle.ps f20, f15, f12\n"                              
	"fmsub.ps f16, f1, f15, f1, dyn\n"     
	"fsub.pi f21, f14, f12\n"                             
	"fadd.ps f27, f1, f1, dyn\n"      
	"flt.s x16, f14, f15\n"                               
	"cubefaceidx.ps f1, f1, f13\n"       
	"fround.ps f28, f14, dyn\n"                           
	"fcvt.pw.ps f19, f13, rtz\n"             
	"feqm.ps m0, f14, f13\n"                              
	"fnmsub.s f27, f14, f15, f15, rdn\n"                  
	//RTLMIN-5479"frsq.ps f31, f12\n"                                  
	"fmin.pi f22, f14, f1\n"                              
	"fmadd.ps f25, f13, f0, f0, dyn\n" 
	"fmaxu.pi f25, f12, f0\n"                             
	//RTLMIN-5479"frsq.ps f17, f12\n"                                  
	"fsrl.pi f27, f15, f12\n"                             
	"faddi.pi f22, f14, 271\n"     
	"fmul.pi f28, f1, f14\n"   
	"fnmadd.ps f27, f12, f12, f15, dyn\n"                 
	"fcvt.pw.ps f25, f1, dyn\n"                 
	"fcvt.pw.ps f17, f12, dyn\n"       
	"fsat8.pi f26, f13\n"                                 
	"flt.pi f22, f1, f21\n"   
	"fmaxu.pi f31, f1, f1\n"
	"flem.ps m0, f0, f15\n"       
	"fmulhu.pi f1, f1, f1\n" 
	"fsgnj.s f24, f13, f0\n"     
	"cubesgntc.ps f26, f0, f14\n"                         
	"fcvt.s.w f0, zero\n"      
        "fnmadd.ps f24, f1, f2, f16, dyn\n"   
        "fmadd.ps f25, f3, f4, f0, dyn\n" 
	"fmaxu.pi f25, f12, f5\n"                             
	//RTLMIN-5479"frsq.ps f17, f6\n" 
        "fnmsub.s f27, f18, f7, f17, rdn\n"  
        "fcvt.pw.ps f25, f9, dyn\n" 
        //RTLMIN-5479"fsin.ps f17, f10\n" 
        "fmul.ps f28, f11, f17\n"   

        "fnmadd.s f1, f0, f13, f12, dyn\n"
        //RTLMIN-5479"fsin.ps f17, f1\n"                                   
	"fclass.s x18, f1\n"                                  
	"fsgnj.s f23, f1, f1\n"
        "fswizz.ps f30, f13, 67\n"                            
	"fandi.pi f17, f1, 70\n" 
	"fmax.pi f17, f13, f13\n"      
	"fcvt.ps.f16 f30, f12\n"
	"fcvt.pwu.ps f21, f14, dyn\n"
	"fbci.ps f16, 382221\n"                               
	"fle.ps f20, f15, f12\n"                              
	"fmsub.ps f16, f1, f15, f1, dyn\n"     
	"fsub.pi f21, f14, f12\n"                             
	"fadd.ps f27, f1, f1, dyn\n"      
	"flt.s x16, f14, f15\n"                               
	"cubefaceidx.ps f1, f1, f13\n"       
	"fround.ps f28, f14, dyn\n"                           
	"fcvt.pw.ps f19, f13, rtz\n"             
	"feqm.ps m0, f14, f13\n"                              
	"fnmsub.s f27, f14, f15, f15, rdn\n"                  
	//RTLMIN-5479"frsq.ps f31, f12\n"                                  
	"fmin.pi f22, f14, f1\n"                              
	"fmadd.ps f25, f13, f0, f0, dyn\n" 
	"fmaxu.pi f25, f12, f0\n"                             
	//RTLMIN-5479"frsq.ps f17, f12\n"                                  
	"fsrl.pi f27, f15, f12\n"                             
	"faddi.pi f22, f14, 271\n"     
	"fmul.pi f28, f1, f14\n"   
	"fnmadd.ps f27, f12, f12, f15, dyn\n"                 
	"fcvt.pw.ps f25, f1, dyn\n"                 
	"fcvt.pw.ps f17, f12, dyn\n"       
	"fsat8.pi f26, f13\n"                                 
	"flt.pi f22, f1, f21\n"   
	"fmaxu.pi f31, f1, f1\n"
	"flem.ps m0, f0, f15\n"       
	"fmulhu.pi f1, f1, f1\n" 
	"fsgnj.s f24, f13, f0\n"     
	"cubesgntc.ps f26, f0, f14\n"                         
	"fcvt.s.w f0, zero\n"      
        "fnmadd.ps f24, f1, f2, f16, dyn\n"   
        "fmadd.ps f25, f3, f4, f0, dyn\n" 
	"fmaxu.pi f25, f12, f5\n"                             
	//RTLMIN-5479"frsq.ps f17, f6\n" 
        "fnmsub.s f27, f18, f7, f17, rdn\n"  
        "fcvt.pw.ps f25, f9, dyn\n" 
        //RTLMIN-5479"fsin.ps f17, f10\n" 
        "fmul.ps f28, f11, f17\n"   

         : 
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "x5", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
         );

   C_TEST_PASS;
   return 0;
}

