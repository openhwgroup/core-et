/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#define GFX_TRAP_ESR_PTR        0x80F40000ULL

volatile uint64_t *gfxTrapEsrPtr        = (uint64_t*)GFX_TRAP_ESR_PTR;  

int main() {

	C_TEST_START;

	// Avoid using x2 and x8. x2 is stack pointer. Assembler does not like if we use x8 and forbid it from using x8 - it errors out

   *gfxTrapEsrPtr =  (uintptr_t) (0); // Avoid trap for gfx instructions

	volatile uint32_t operands[256] __attribute__ ((aligned (4096))) = {

			 0x8683F7FF,
                         0xC07F3FFF,
                         0x00000000,
                         0x3C072C85,
                         0x9EDE38F7,
                         0x00000001,
                         0x3E7F7F7F,
                         0xDF7EFFFF,
                         0x007FFFFF,
                         0x4F951295,
                         0x41E00002,
                         0x007FFFFE,
                         0xC2800040,
                         0x4FFFDFF7,
                         0x00800000,
                         0xBFFFFFCF,
                         0x015E834A,
                         0x00800001,
                         0xC700FFBF,
                         0xCE7C0007,
                         0x00FFFFFF,
                         0xBE5FEFFF,
                         0x417FEBFF,
                         0x00FFFFFE,
                         0xCE7D4590,
                         0xC0FFFC3F,
                         0x01000000,
                         0x41FFFFEB,
                         0x137F7FFB,
                         0x01000001,
                         0xA68002FE,
                         0x7FFF0007,
                         0x017FFFFF,
                         0x2BFFFFCF,
                         0xDE00ACFD,
                         0x017FFFFE,
                         0x760077FF,
                         0xBCB1B7E5,
                         0x33800000,
                         0x340000EF,
                         0x0100087F,
                         0x33800001,
                         0xFE804FFF,
                         0x3D900000,
                         0x33FFFFFF,
                         0xBED56444,
                         0x3E7FF400,
                         0x33FFFFFE,
                         0x5E7F8003,
                         0xBEC111F7,
                         0x3D800000,
                         0xDE040000,
                         0x3FFFF7FE,
                         0x3D800001,
                         0x4E00FFEE,
                         0xC08400FF,
                         0x3DFFFFFF,
                         0xC2D0AA48,
                         0xCE820FFF,
                         0x3DFFFFFE,
                         0xBFFC1000,
                         0xC11DF309,
                         0x3E000000,
                         0xCBCF3EA9,
                         0x3EFFFFFD,
                         0x3E000001,
                         0xFF8000FD,
                         0x69FFFF7F,
                         0x3E7FFFFF,
                         0xBE000081,
                         0x40DD6229,
                         0x3E7FFFFE,
                         0x4BF7BFFF,
                         0xDA7EEFFF,
                         0x3E800000,
                         0xB4FF8003,
                         0xBF7FFF7B,
                         0x3E800001,
                         0xBE30FFBE,
                         0xC00FFFEE,
                         0x3EFFFFFF,
                         0x33BFFF7E,
                         0x46FE00FE,
                         0x3EFFFFFE,
                         0xDA5F117A,
                         0x39409B1B,
                         0x3F000000,
                         0x5FAFF4F6,
                         0xC1FBFFFC,
                         0x3F000001,
                         0xB5FE0100,
                         0x410000FD,
                         0x3F7FFFFF,
                         0xBFFFFE03,
                         0x4EEC20DF,
                         0x3F7FFFFE,
                         0xC120000F,
                         0x25FFEFBF,
                         0x3F800000,
                         0xA800081E,
                         0x87258873,
                         0x3F800001,
                         0x421FFC00,
                         0x017FE07E,
                         0x3FFFFFFF,
                         0x25877FFF,
                         0xC09FFBFE,
                         0x3FFFFFFE,
                         0x41FDFFFB,
                         0xC2CF5EC6,
                         0x40000000,
                         0xBC000FBF,
                         0xBE3672E3,
                         0x40000001,
                         0x72B139FA,
                         0x40FDBFFF,
                         0x407FFFFF,
                         0x3C7F0003,
                         0x4EAEA2E8,
                         0x407FFFFE,
                         0x9E7BFFF7,
                         0x41DE507F,
                         0x40800000,
                         0xFFFFFDDF,
                         0xC07DFBFE,
                         0x40800001,
                         0xCB84007F,
                         0xC090000F,
                         0x40FFFFFF,
                         0xDACC892B,
                         0xF2F80006,
                         0x40FFFFFE,
                         0xB8FFFFE4,
                         0x4178001F,
                         0x41000000,
                         0xBF807FFB,
                         0xFF6FFE00,
                         0x41000001,
                         0x5D000FFF,
                         0x3F00007F,
                         0x417FFFFF,
                         0xDEFFF7EF,
                         0x107FFFFE,
                         0x417FFFFE,
                         0x807C1FFF,
                         0x2C4716EA,
                         0x41800000,
                         0xFF97847C,
                         0x4064D70E,
                         0x41800001,
                         0x4FD8BEC6,
                         0x2CD956DB,
                         0x41FFFFFF,
                         0xDE0003FF,
                         0xC0561C35,
                         0x41FFFFFE,
                         0x4F7FC000,
                         0x33812EDF,
                         0x4B800000,
                         0x3F007FF7,
                         0xDF07FFDF,
                         0x4B800001,
                         0x4EFFDFE0,
                         0x1A8FFFFE,
                         0x4BFFFFFF,
                         0x4E4D6940,
                         0x3FFBFFFB,
                         0x4BFFFFFE,
                         0xBE886202,
                         0x3E800000,
                         0x4E000000,
                         0x4BD86177,
                         0xC0DFFFF8,
                         0x4E000001,
                         0x41DFF7FF,
                         0x3F080040,
                         0x4E7FFFFF,
                         0xC0FFFE80,
                         0x8D7F9FFE,
                         0x4E7FFFFE,
                         0xED9EFFFF,
                         0xBF61FE3E,
                         0x4E800000,
                         0x3F7FFFBF,
                         0xBEFC007E,
                         0x4E800001,
                         0x2200FFBE,
                         0x80804FFF,
                         0x4EFFFFFF,
                         0x3F5FFFDF,
                         0x7FF7FFFA,
                         0x4EFFFFFE,
                         0x7FF353AC,
                         0x408005FF,
                         0x4F000000,
                         0xB18997A1,
                         0x413FF7FE,
                         0x4F000001,
                         0xCEFFFF00,
                         0xC000DFFF,
                         0x4F7FFFFF,
                         0x48BFFFFC,
                         0x3E7BF7FE,
                         0x4F7FFFFE,
                         0x25000001,
                         0xFEBCDFF5,
                         0x4F800000,
                         0x5FFFEEFF,
                         0xDFF80000,
                         0x4F800001,
                         0xB2FFFDBE,
                         0x32C62227,
                         0x4FFFFFFF,
                         0x00012000,
                         0xC0FFFBFE,
                         0x4FFFFFFE,
                         0x4F7FE01F,
                         0xF174DEE7,
                         0x5E000000,
                         0xBB40001E,
                         0xDE65CBD0,
                         0x5E000001,
                         0x5DFFF80F,
                         0xED7FFFF1,
                         0x5E7FFFFF,
                         0x339FFEFE,
                         0x80FFFFED,
                         0x5E7FFFFE,
                         0x9E020000,
                         0xBF01FF00,
                         0x5E800000,
                         0xC17BF7FF,
                         0x4180807E,
                         0x5E800001,
                         0xDECF3286,
                         0xC17FDFFD,
                         0x5EFFFFFF,
                         0x2F00BFFF,
                         0x5E14D901,
                         0x5EFFFFFE,
                         0xBE784000,
                         0x7FC00002,
                         0x5F000000,
                         0xB4E4A9C2,
                         0xBF81F800,
                         0x5F000001,
                         0xBF7F9FFE,
                         0x4FFFFDFE,
                         0x5F7FFFFF,
                         0xCBFFF800,
                         0x3AFFDEFF,
                         0x5F7FFFFE,
                         0xC27FDFFB,
                         0xFFFBFDFF,
                         0x5F800000,
                         0xBE7FBFFF
                
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

