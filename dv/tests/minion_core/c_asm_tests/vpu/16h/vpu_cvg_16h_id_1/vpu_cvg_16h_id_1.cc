/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include "minion.h"
#include "macros.h"
#include "et_postsi_common.h"
#define VSNIP_RSV           : :  : "x31"
int main() {
		__asm__ __volatile__ ( 
			"j LBL_TEST_START\n"          
			VSNIP_RSV     
		);
		__asm__ __volatile__ (
			".align 12\n"
			"LBL_M_MODE_E_H:\n"
			".global LBL_M_MODE_E_H\n"
			"csrr x5, mcause\n"      
			"bltz x5, LBL_INTERRUPT_TABLE\n"
			"LBL_EXCEPTION_TABLE:\n"
			".global LBL_EXCEPTION_TABLE\n"
			"li x6, -1\n"
			"srli x6, x6, 1\n"
			"and x5, x5, x6\n"
			"li x6, 16\n"
			"mulw x6, x5, x6\n"
			"la x5, LBL_EXCP_INSTRUCTION_ADDRESS_MISALIGN_0\n"
			"add x6, x5, x6\n"
			"jr x6\n"
			"LBL_EXCP_INSTRUCTION_ADDRESS_MISALIGN_0:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_EXCP_INSTRUCTION_ACCESS_FAULT_1:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_EXCP_ILLEGAL_INSTRUCTION_2:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_EXCP_BREAKPOINT_3:\n"
			"csrr x5, mepc\n"
			"addi x5, x5, 4\n"
			"csrw mepc, x5\n"
			"mret\n"
			"LBL_EXCP_LOAD_ADDRESS_MISALIGNED_4:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_EXCP_LOAD_ACCESS_FAULT_5:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_EXCP_STORE_OR_AMO_ADDRESS_MISALIGNED_6:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_EXCP_STORE_OR_AMO_ACCESS_FAULT_7:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_EXCP_ECALL_FROM_UMODE_8:\n"
			"lui	x5,0x2\n"      
			"addiw	x5,x5,-2048\n" 
			"csrrs   zero, mstatus, x5\n" 
			"j LBL_RET_TO_CALLER\n"
			"LBL_EXCP_ECALL_FROM_SMODE_9:\n"
			"lui	x5,0x2\n"      
			"addiw	x5,x5,-2048\n" 
			"csrrs   zero, mstatus, x5\n" 
			"j LBL_RET_TO_CALLER\n"
			"LBL_DUMMY_10:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_EXCP_ECALL_FROM_MMODE_11:\n"
			"csrr x5, mepc\n"
			"addi x5, x5, 4\n"
			"csrw mepc, x5\n"
			"mret\n"
			"LBL_EXCP_INSTRUCTION_PAGE_FAULT_12:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_EXCP_LOAD_PAGE_FAULT_13:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_DUMMY_14:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_EXCP_STORE_OR_AMO_PAGE_FAULT_15:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_DUMMY_16:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_DUMMY_17:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_DUMMY_18:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_DUMMY_19:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_DUMMY_20:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_DUMMY_21:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_DUMMY_22:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_DUMMY_23:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_DUMMY_24:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_EXCP_INSTRUCTION_BUS_ERROR_25:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_EXCP_INSTRUCTION_ECC_ERROR_26:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_EXCP_LOAD_SPLIT_PAGE_FAULT_27:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_EXCP_STORE_SPLIT_PAGE_FAULT_28:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_DUMMY_29:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_EXCP_MCODE_EMULATION_30:\n"
			"csrr x5, mepc\n"
			"addi x5, x5, 4\n"
			"csrw mepc, x5\n"
			"mret\n"
			"LBL_INTERRUPT_TABLE:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_RET_TO_CALLER\n"
			"LBL_RET_TO_CALLER:\n"
			".global LBL_RET_TO_CALLER\n"
			"csrw mcause, x0\n"
			"csrw mtval, x0\n"
			"csrr x5, mscratch\n"
			"csrw mepc, x5\n"       
			"li x5, 0xDEADDEAD\n"            
			"mret\n"                
			VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"LBL_TEST_START:\n"          
			".global LBL_TEST_START\n"          
			VSNIP_RSV     
		);
	uint64_t hid;
	hid = get_hart_id();
	if ( hid == 0 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_0:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_0\n"
			"csrw mscratch, x5\n"
			"csrwi menable_shadows, 0xf\n"
			"li x5, -1\n"
			"csrw mcounteren, x5\n"
			"csrw scounteren, x5\n"
			"csrwi mhpmcounter3, 0\n"
			"csrwi mhpmcounter4, 0\n"
			"csrwi mhpmcounter5, 0\n"
			"csrwi mhpmcounter6, 0\n"
			"csrwi mhpmcounter7, 0\n"
			"csrwi mhpmcounter8, 0\n"
			"csrwi mhpmevent3, 21\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 28\n"   
			"csrwi mhpmevent6, 20\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 6\n"   
			"LBL_EN_COOP_ESR_HID_0:\n"          
			"li x5, 0x80740290\n"  
			"li x31, 1\n"
			"sd x31, 0(x5)\n"
			"mov.m.x m0, x0, 0xff\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f15,  0 (x5)\n"
			"flw.ps f10,  64 (x5)\n"
			"flw.ps f3,  576 (x5)\n"
			"flw.ps f28,  672 (x5)\n"
			"flw.ps f21,  736 (x5)\n"
			"flw.ps f2,  480 (x5)\n"
			"flw.ps f30,  192 (x5)\n"
			"flw.ps f31,  704 (x5)\n"
			"flw.ps f12,  608 (x5)\n"
			"flw.ps f9,  416 (x5)\n"
			"flw.ps f13,  768 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"flw.ps f22,  224 (x5)\n"
			"flw.ps f17,  928 (x5)\n"
			"flw.ps f19,  96 (x5)\n"
			"flw.ps f23,  384 (x5)\n"
			"flw.ps f7,  288 (x5)\n"
			"flw.ps f0,  992 (x5)\n"
			"flw.ps f25,  32 (x5)\n"
			"flw.ps f24,  352 (x5)\n"
			"flw.ps f8,  320 (x5)\n"
			"flw.ps f1,  128 (x5)\n"
			"flw.ps f4,  832 (x5)\n"
			"flw.ps f27,  896 (x5)\n"
			"flw.ps f18,  640 (x5)\n"
			"flw.ps f26,  448 (x5)\n"
			"flw.ps f14,  544 (x5)\n"
			"flw.ps f20,  960 (x5)\n"
			"flw.ps f6,  160 (x5)\n"
			"flw.ps f5,  800 (x5)\n"
			"flw.ps f11,  256 (x5)\n"
			"flw.ps f29,  512 (x5)\n"
			"LBL_INIT_FCC_CTRS_HID_0:\n"          
			"li x31, 0x803400C0\n" 
			"sd x0, 0(x31)\n"
			"li x31, 0x803400C8\n" 
			"sd x0, 0(x31)\n"
			"li x31, 0x803400D0\n" 
			"sd x0, 0(x31)\n"
			"li x31, 0x803400D8\n" 
			"sd x0, 0(x31)\n"
			"fence\n"
			"LBL_INIT_FLB_CTRS_HID_0:\n"          
			"li x31, 0x80340100\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340108\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340110\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340118\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340120\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340128\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340130\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340138\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340140\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340148\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340150\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340158\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340160\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340168\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340170\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340178\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340180\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340188\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340190\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340198\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401A0\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401A8\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401B0\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401B8\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401C0\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401C8\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401D0\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401D8\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401E0\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401E8\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401F0\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401F8\n"
			"sd x0, 0(x31)\n"
			"fence\n"
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"LBL_SYNC_POINT_0_HID_0:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_0_WAIT_HID_0\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_0_EXIT_HID_0\n"
			"LBL_SYNC_POINT_0_WAIT_HID_0:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_EXIT_HID_0:\n"          
			"nop\n"
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0x5539bc4286\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_HID_0:\n"
			"li x13, 0x8011bc70b8\n"
			"sd x5, 256(x13)\n"
			"lh x26, 256(x13)\n" 
			"fmul.pi f13, f16, f20\n"
			"fsgnjn.s f6, f9, f2\n"
			"fcvt.ps.pw f6, f2, rmm\n"
			"fsgnjn.s f23, f6, f6\n"
			"fmul.ps f10, f16, f6, rdn\n"
			"mulhu x7, x7, x10\n"
			"fsetm.pi m4, f20\n"
			"fmul.s f23, f6, f16, rmm\n"
			"lui x5, 0x0\n"
			"li x7, 0x805250b008\n"
			"la x27, mem_ptr_offset\n"
			"flw.ps f2, 640(x27)\n"
			"famoorl.pi f16, f2 (x7)\n" 
			"fround.ps f18, f18, rdn\n"
			"fsra.pi f6, f9, f9\n"
			"fsat8.pi f18, f9\n"
			"fnmsub.s f11, f27, f27, f27, rup\n"
			"flt.pi f27, f9, f27\n"
			"fcvt.ps.pwu f9, f9, rmm\n"
			"fcmovm.ps f18, f18, f18\n"
			"fpackrepb.pi f18, f18\n"
			"feq.pi f18, f18, f18\n"
			"li x26, 0x80445d8028\n"
			"amoandg.d x29, x14, (x26)\n" 
			"fsgnjx.s f29, f11, f29\n"
			"fmax.pi f18, f8, f3\n"
			"fsgnjn.ps f11, f11, f10\n"
			"fadd.pi f29, f8, f20\n"
			"fle.pi f27, f11, f10\n"
			"fminu.pi f29, f27, f16\n"
			"fcmovm.ps f0, f3, f20\n"
			"fclass.ps f8, f8\n"
			"flt.pi f6, f29, f28\n"
			"li x10, 0xb0c570b347142a38\n"
			"li x11, 0xc290ad1804088f19\n"
			"packb x27, x10, x11\n" 
			"slti x29, x27, 0x1e\n"
			"fnot.pi f28, f6\n"
			"addi x9, x26, 0x8\n"
			"fsrl.pi f18, f18, f3\n"
			"fmax.ps f9, f11, f9\n"
			"remu x19, x13, x19\n"
			"fsll.pi f10, f29, f11\n"
			"frcp.ps f2, f28\n"
			"fmin.ps f29, f18, f28\n"
			"li x10, 0x80110247e0\n"
			"fsw.ps f23, -512(x10)\n"
			"flq2 f18, -512(x10)\n" 
			"frcp.ps f28, f0\n"
			"bge x4, x0, 1f\n"
			"1:\n"
			"fnmsub.ps f3, f8, f3, f27, rdn\n"
			"fpackreph.pi f18, f18\n"
			"fcvt.ps.f16 f0, f20\n"
			"fadd.pi f23, f10, f9\n"
			"feq.ps f11, f10, f6\n"
			"fmadd.s f27, f23, f0, f10, rtz\n"
			"fmaxu.pi f8, f23, f9\n"
			"fsgnj.ps f8, f6, f8\n"
			"li x29, 0x8012b3ebc8\n"
			"li x10, 2795992107\n"
			"fg32w.ps f0, x9 (x29)\n" 
			"sraw x7, x19, x19\n"
			"bltu x6, x13, 1f\n"
			"1:\n"
			"feq.pi f8, f18, f3\n"
			"fmv.s.x f6, x10\n"
			"fcvt.s.wu f27, x9, rup\n"
			"sra x19, x27, x9\n"
			"srliw x27, x10, 5\n"
			"remw x14, x14, x9\n"
			"fmulhu.pi f8, f3, f18\n"
			"fmul.s f6, f3, f3, rmm\n"
			"bltu x10, x5, 1f\n"
			"1:\n"
			"li x7, 0x8055c60018\n"
			"la x27, mem_ptr_offset\n"
			"flw.ps f20, 352(x27)\n"
			"famoandl.pi f10, f20 (x7)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"feq.s x11, f9, f6\n"
			"for.pi f9, f9, f27\n"
			"or x14, x27, x14\n"
			"remu x7, x27, x7\n"
			"faddi.pi f18, f6, 0x22\n"
			"xori x10, x10, 0x4\n"
			"auipc x10, 0x12\n"
			"srliw x26, x10, 3\n"
			"subw x11, x10, x18\n"
			"li x29, 0x8053752018\n"
			"la x11, mem_ptr_offset\n"
			"flw.ps f3, 256(x11)\n"
			"famomaxl.ps f9, f3 (x29)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"packb x26, x10, x26\n"
			"divw x14, x19, x9\n"
			"fexp.ps f11, f6\n"
			"addiw x27, x18, 0x7\n"
			"maskpopc x9, m6\n"
			"beq x6, x29, 1f\n"
			"1:\n"
			"flem.ps m4, f9, f13\n"
			"divuw x18, x13, x18\n"
			"bge x26, x25, 1f\n"
			"1:\n"
			"slti x7, x5, 0x7\n"
			"bgeu x11, x6, 1f\n"
			"1:\n"
			"fpackreph.pi f29, f27\n"
			"li x31, 0x8010008880\n"
			"li x14, 0x8000000000000009\n"
			"add x14, x31, x14\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw unlock_va, x14\n"	
			"csrwi tensor_wait, 6\n" 
			"flt.pi f11, f16, f23\n"
			"fsat8.pi f27, f16\n"
			"bne x4, x18, 1f\n"
			"1:\n"
			"fclass.ps f10, f10\n"
			"fmadd.ps f27, f10, f23, f10, rtz\n"
			"flt.s x7, f16, f23\n"
			"fnmsub.s f16, f23, f10, f16, rdn\n"
			"flt.s x9, f16, f16\n"
			"bltu x16, x11, 1f\n"
			"1:\n"
			"packb x9, x9, x14\n"
			"fcvt.pw.ps f11, f16, rtz\n"
			"li x7, 0x80441f7038\n"
			"la x13, mem_ptr_offset\n"
			"flw.ps f2, 64(x13)\n"
			"famomaxug.pi f20, f2 (x7)\n" 
			"li x5, 0x2ab8b9cc8148cd2d\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x7e09bbf172416fd1\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"maskxor m3, m6, m3\n"
			"fminu.pi f29, f23, f29\n"
			"maskor m6, m1, m1\n"
			"auipc x14, 0x1b\n"
			"rem x27, x9, x14\n"
			"mul x11, x26, x19\n"
			"frcp.ps f13, f28\n"
			"rem x19, x14, x9\n"
			"sraiw x7, x29, 2\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f20, 576(x14)\n"
			"flw.ps f23, 512(x14)\n"
			"flw.ps f18, 416(x14)\n"
			"fmadd.s f8, f20, f23, f18, rtz\n" 
			"fsgnj.ps f0, f10, f6\n"
			"bgeu x8, x20, 1f\n"
			"1:\n"
			"sra x13, x13, x19\n"
			"fsrl.pi f9, f27, f9\n"
			"fadd.ps f20, f6, f23, rmm\n"
			"sll x27, x27, x14\n"
			"fcvt.ps.pw f23, f20, rmm\n"
			"feq.pi f9, f29, f6\n"
			"fmsub.s f27, f10, f8, f23, rmm\n"
			"sub x14, x10, x26\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f3, 160(x18)\n"
			"flw.ps f9, 0(x18)\n"
			"flw.ps f2, 704(x18)\n"
			"fmsub.s f3, f3, f9, f2, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"srlw x26, x9, x27\n"
			"mulhsu x11, x10, x27\n"
			"fmadd.ps f2, f23, f28, f28, rdn\n"
			"sltiu x29, x7, 0xc\n"
			"fnmadd.s f23, f0, f23, f8, rne\n"
			"fsat8.pi f23, f23\n"
			"srai x14, x14, 0\n"
			"fcvt.f16.ps f16, f20\n"
			"andi x9, x9, 0x9\n"
			"li x5, 0x8050438018\n"
			"la x9, mem_ptr_offset\n"
			"flw.ps f23, 352(x9)\n"
			"famomaxul.pi f13, f23 (x5)\n" 
			"fcvt.s.w f18, x13, rtz\n"
			"fcvt.pwu.ps f2, f16, rmm\n"
			"divu x26, x10, x10\n"
			"bge x8, x27, 1f\n"
			"1:\n"
			"slti x5, x5, 0x5\n"
			"fmul.pi f13, f6, f11\n"
			"bltu x23, x29, 1f\n"
			"1:\n"
			"fltm.pi m6, f18, f27\n"
			"fadd.ps f29, f0, f6, rdn\n"
			"fsgnjn.s f3, f11, f20\n"
			"fmul.s f10, f0, f10, rup\n"
			"li x29, 0x8051d70018\n"
			"la x27, mem_ptr_offset\n"
			"flw.ps f28, 736(x27)\n"
			"fghl.ps f27, f28 (x29)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fmadd.ps f11, f13, f6, f11, rne\n"
			"bltu x14, x17, 1f\n"
			"1:\n"
			"ori x7, x10, 0x9\n"
			"bgeu x6, x28, 1f\n"
			"1:\n"
			"fmadd.ps f29, f10, f27, f6, rtz\n"
			"bgeu x8, x25, 1f\n"
			"1:\n"
			"fsrl.pi f10, f29, f8\n"
			"flt.ps f23, f13, f10\n"
			"fbci.pi f16, 0x7edc3\n"
			"srli x19, x9, 1\n"
			"bge x1, x6, 1f\n"
			"1:\n"
			"srli x26, x14, 3\n"
			"srli x18, x14, 2\n"
			"bgeu x15, x0, 1f\n"
			"1:\n"
			"la x26, rand_ieee754_16\n"
			"flw.ps f20, 0(x26)\n"
			"fcvt.ps.f16 f29, f20\n" 
			"fclass.s x19, f28\n"
			"fmulhu.pi f3, f28, f11\n"
			"bltu x20, x4, 1f\n"
			"1:\n"
			"mulh x9, x9, x19\n"
			"mul x29, x10, x5\n"
			"feqm.ps m3, f11, f3\n"
			"maskpopcz x11, m6\n"
			"fcvt.pw.ps f28, f28, rtz\n"
			"blt x23, x12, 1f\n"
			"1:\n"
			"fmax.s f3, f28, f23\n"
			"flem.ps m6, f6, f29\n"
			"li x9, 0x8045abe000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f13, (x9)\n" 
			"subw x29, x7, x29\n"
			"lui x29, 0x0\n"
			"bge x27, x0, 1f\n"
			"1:\n"
			"feqm.ps m3, f2, f8\n"
			"srl x7, x13, x7\n"
			"fbci.pi f0, 0x2c68b\n"
			"fcvt.ps.f16 f2, f8\n"
			"bne x0, x19, 1f\n"
			"1:\n"
			"flem.ps m1, f8, f2\n"
			"fsgnj.s f2, f0, f0\n"
			"fle.pi f2, f8, f8\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_0_TLD_ID_0_NMASK_0x1_CMASK_0xfb_SHIRE_0_HID_0:\n"
			"li x31, 0x1fb1e\n"
			"csrw tensor_coop, x31\n" 
			"li x31, 0x01FF000001FF0000\n"
			"csrw tensor_mask, x31\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"li x5, 0xd440008020006b28\n"
			"csrw tensor_load, x5\n" 
			"csrwi tensor_wait, 1\n" 
			"LBL_SYNC_POINT_1_HID_0:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_1_WAIT_HID_0\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_1_EXIT_HID_0\n"
			"LBL_SYNC_POINT_1_WAIT_HID_0:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_1_EXIT_HID_0:\n"          
			"nop\n"
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_0:\n"
			"li x5, 0x3200000008730019\n"
			"csrw tensor_reduce, x5\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SYNC_POINT_2_HID_0:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_2_WAIT_HID_0\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_2_EXIT_HID_0\n"
			"LBL_SYNC_POINT_2_WAIT_HID_0:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_2_EXIT_HID_0:\n"          
			"nop\n"
			"LBL_SEQID_0_TST_ID_0_QUAD128_START_CORE_0_SHIRE_0_HID_0:\n"
			"li x31, (0xc << 4)\n"
			"li x5, 0x841e008020006b00\n"
			"csrw tensor_store, x5\n" 
			"csrwi tensor_wait, 8\n" 
			"LBL_SYNC_POINT_3_HID_0:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_3_WAIT_HID_0\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_3_EXIT_HID_0\n"
			"LBL_SYNC_POINT_3_WAIT_HID_0:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_3_EXIT_HID_0:\n"          
			"nop\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, 0x3080008010006648\n"
			"li x6, 0x10008010006646\n"
			"li x7, 0xc3000000d19047\n"
			"li x9, 0x3cc4e0000001a33a\n"
			"li x10, 0xeb98008010006640\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrwi tensor_wait, 8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 2\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f10, 4(x31)\n"
			"flw.ps f11, 7(x31)\n"
			"flw.ps f13, 1(x31)\n"
			"flw.ps f16, 10(x31)\n"
			"flw.ps f18, 17(x31)\n"
			"flw.ps f2, 1(x31)\n"
			"flw.ps f20, 0(x31)\n"
			"flw.ps f23, 21(x31)\n"
			"flw.ps f27, 18(x31)\n"
			"flw.ps f28, 1(x31)\n"
			"flw.ps f29, 8(x31)\n"
			"flw.ps f3, 41(x31)\n"
			"flw.ps f6, 24(x31)\n"
			"flw.ps f8, 14(x31)\n"
			"flw.ps f9, 46(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x10, 0(x31)\n"
			"lw x11, 2(x31)\n"
			"lw x13, 6(x31)\n"
			"lw x14, 2(x31)\n"
			"lw x18, 12(x31)\n"
			"lw x19, 10(x31)\n"
			"lw x26, 19(x31)\n"
			"lw x27, 20(x31)\n"
			"lw x29, 28(x31)\n"
			"lw x5, 35(x31)\n"
			"lw x7, 27(x31)\n"
			"lw x9, 16(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_0:\n"
			"fclass.s x18, f8\n"
			"fsgnjx.s f0, f13, f6\n"
			"fence\n"
			"flem.ps m3, f3, f18\n"
			"beq x10, x21, 1f\n"
			"1:\n"
			"ecall\n"
			"fmv.x.s x19, f18\n"
			"fadd.ps f18, f9, f18, rdn\n"
			"fmax.s f2, f0, f18\n"
			"ori x26, x9, 0x7\n"
			"slli x26, x26, 4\n"
			"blt x29, x12, 1f\n"
			"1:\n"
			"srl x11, x13, x13\n"
			"fsgnj.s f3, f8, f13\n"
			"fbci.ps f8, 0x24af6\n"
			"fmvs.x.ps x10, f11, 6\n"
			"flem.ps m3, f28, f11\n"
			"slli x10, x7, 4\n"
			"bne x13, x6, 1f\n"
			"1:\n"
			"flog.ps f23, f8\n"
			"fclass.ps f0, f11\n"
			"fcmovm.ps f18, f29, f2\n"
			"slti x14, x18, 0x5\n"
			"bltu x12, x23, 1f\n"
			"1:\n"
			"fandi.pi f23, f29, 0x1f3\n"
			"bltu x4, x9, 1f\n"
			"1:\n"
			"fadd.ps f23, f13, f13, rne\n"
			"sra x18, x29, x14\n"
			"fsetm.pi m3, f2\n"
			"fsgnj.s f29, f29, f28\n"
			"mova.x.m x10\n"
			"for.pi f11, f28, f11\n"
			"fltm.ps m4, f0, f6\n"
			"fcvt.ps.f16 f11, f28\n"
			"bne x5, x1, 1f\n"
			"1:\n"
			"fsgnj.s f23, f28, f9\n"
			"bge x27, x13, 1f\n"
			"1:\n"
			"fclass.ps f29, f29\n"
			"div x11, x5, x13\n"
			"remuw x19, x18, x19\n"
			"srliw x14, x10, 3\n"
			"fcmovm.ps f28, f13, f28\n"
			"fmax.s f23, f13, f28\n"
			"fsra.pi f10, f23, f6\n"
			"fle.ps f16, f16, f29\n"
			"auipc x5, 0x0\n"
			"fmvz.x.ps x10, f28, 0\n"
			"mulhu x13, x26, x19\n"
			"mova.m.x x26\n"
			"fpackreph.pi f2, f0\n"
			"divw x13, x13, x11\n"
			"fnmadd.ps f13, f16, f13, f16, rtz\n"
			"fltm.ps m4, f11, f8\n"
			"fsub.s f20, f3, f6, rne\n"
			"feq.pi f13, f8, f13\n"
			"bne x22, x13, 1f\n"
			"1:\n"
			"fle.pi f10, f10, f29\n"
			"blt x17, x29, 1f\n"
			"1:\n"
			"fmin.ps f6, f3, f3\n"
			"srliw x26, x5, 1\n"
			"mova.m.x x29\n"
			"mova.m.x x27\n"
			"beq x5, x1, 1f\n"
			"1:\n"
			"srliw x13, x13, 3\n"
			"fle.ps f3, f27, f11\n"
			"remu x27, x11, x13\n"
			"fltu.pi f29, f6, f29\n"
			"fadd.pi f23, f23, f27\n"
			"fcvt.ps.f16 f10, f10\n"
			"mulhu x14, x7, x14\n"
			"divw x5, x11, x14\n"
			"fmv.x.s x7, f18\n"
			"fsrai.pi f10, f27, 0x5\n"
			"blt x8, x21, 1f\n"
			"1:\n"
			"mulw x29, x19, x29\n"
			"srliw x26, x14, 4\n"
			"beq x18, x4, 1f\n"
			"1:\n"
			"fnmsub.s f11, f8, f6, f13, rne\n"
			"bgeu x26, x29, 1f\n"
			"1:\n"
			"maskor m6, m3, m6\n"
			"fadd.ps f16, f20, f13, rne\n"
			"fnot.pi f6, f9\n"
			"bge x20, x9, 1f\n"
			"1:\n"
			"fexp.ps f11, f2\n"
			"beq x3, x20, 1f\n"
			"1:\n"
			"lui x10, 0x15\n"
			"for.pi f13, f27, f6\n"
			"feq.pi f13, f8, f3\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_0_LOOP_SEQ_HID_0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_HID_0:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f6, 128(x13)\n"
			"flw.ps f11, 960(x13)\n"
			"fsgnjn.ps f20, f6, f11\n" 
			"fcvt.pw.ps f13, f16, rdn\n"
			"fround.ps f18, f16, rdn\n"
			"fandi.pi f13, f29, 0x8d\n"
			"frcp.ps f8, f8\n"
			"fle.s x18, f29, f23\n"
			"flog.ps f10, f16\n"
			"fsub.ps f23, f23, f0, rup\n"
			"blt x16, x25, 1f\n"
			"1:\n"
			"fswizz.ps f13, f28, 0x15\n"
			"mulh x29, x9, x14\n"
			"bge x13, x29, 1f\n"
			"1:\n"
			"la x5, rand_int_32\n"
			"flw.ps f11, 224(x5)\n"
			"flw.ps f20, 864(x5)\n"
			"fand.pi f27, f11, f20\n" 
			"fnmsub.s f0, f13, f28, f16, rup\n"
			"bgeu x18, x14, 1f\n"
			"1:\n"
			"frcp.ps f10, f13\n"
			"srliw x19, x19, 5\n"
			"srli x11, x11, 5\n"
			"divw x5, x19, x5\n"
			"sub x19, x29, x19\n"
			"fmul.pi f29, f11, f11\n"
			"bgeu x9, x24, 1f\n"
			"1:\n"
			"auipc x10, 0xb\n"
			"sltiu x13, x11, 0x1\n"
			"li x19, 0x804033600c\n"
			"amoming.w x14, x7, (x19)\n" 
			"slli x11, x11, 0\n"
			"fsetm.pi m1, f16\n"
			"fmvz.x.ps x26, f16, 1\n"
			"flt.pi f13, f13, f28\n"
			"srai x14, x26, 2\n"
			"fle.s x27, f23, f29\n"
			"fsub.pi f27, f27, f28\n"
			"flt.ps f20, f13, f10\n"
			"fmv.s.x f16, x26\n"
			"li x11, 0x8015336824\n"
			"sd x5, 128(x11)\n"
			"lbu x19, 128(x11)\n" 
			"subw x7, x19, x7\n"
			"slli x9, x10, 1\n"
			"slli x19, x10, 2\n"
			"feq.pi f8, f16, f11\n"
			"maskxor m3, m3, m4\n"
			"blt x17, x18, 1f\n"
			"1:\n"
			"xor x14, x14, x13\n"
			"bne x22, x2, 1f\n"
			"1:\n"
			"fmul.ps f28, f18, f11, rup\n"
			"fcvt.pwu.ps f6, f20, rup\n"
			"fmin.pi f29, f20, f6\n"
			"li x5, 0x8046dcf024\n"
			"amomaxg.w x26, x14, (x5)\n" 
			"fmulhu.pi f3, f10, f27\n"
			"fcvt.pwu.ps f6, f10, rtz\n"
			"fexp.ps f2, f3\n"
			"fmul.pi f3, f3, f10\n"
			"fsgnj.s f6, f10, f27\n"
			"beq x1, x26, 1f\n"
			"1:\n"
			"fnmsub.ps f27, f27, f3, f10, rmm\n"
			"fsgnj.s f3, f3, f10\n"
			"bltu x19, x20, 1f\n"
			"1:\n"
			"fcvt.pw.ps f3, f10, rmm\n"
			"fnot.pi f10, f3\n"
			"csrr x0, mscratch\n"
			"sltiu x29, x29, 0x19\n"
			"ffrc.ps f27, f20\n"
			"beq x12, x11, 1f\n"
			"1:\n"
			"mulw x11, x18, x19\n"
			"fence\n"
			"fsgnjx.ps f0, f13, f9\n"
			"fsll.pi f0, f0, f13\n"
			"fsra.pi f13, f8, f6\n"
			"divu x10, x27, x10\n"
			"srai x27, x26, 5\n"
			"li x7, 0x8010b8c300\n"
			"li x27, 2276242541\n"
			"fsc32h.ps f18, x14 (x7)\n" 
			"sraw x9, x10, x10\n"
			"fmax.ps f2, f16, f8\n"
			"fltu.pi f6, f6, f0\n"
			"flem.ps m1, f9, f6\n"
			"bgeu x6, x7, 1f\n"
			"1:\n"
			"fle.pi f16, f0, f23\n"
			"bge x0, x31, 1f\n"
			"1:\n"
			"lui x11, 0xb\n"
			"fmin.ps f3, f8, f9\n"
			"fmadd.ps f28, f13, f29, f11, rtz\n"
			"fsrl.pi f6, f8, f29\n"
			"li x18, 0x80148a2a30\n"
			"fsw.ps f3, 256(x18)\n"
			"flq2 f23, 256(x18)\n" 
			"feq.ps f2, f8, f27\n"
			"fmax.s f18, f18, f8\n"
			"fsgnjx.ps f18, f8, f18\n"
			"fcvt.pwu.ps f8, f27, rne\n"
			"faddi.pi f8, f18, 0x80\n"
			"fsgnjx.ps f20, f27, f18\n"
			"fxor.pi f8, f23, f27\n"
			"bne x19, x17, 1f\n"
			"1:\n"
			"ffrc.ps f20, f18\n"
			"fexp.ps f8, f23\n"
			"bgeu x26, x26, 1f\n"
			"1:\n"
			"li x11, 0x80527b2020\n"
			"amoxorl.d x11, x10, (x11)\n" 
			"divw x27, x11, x13\n"
			"fpackreph.pi f3, f10\n"
			"flt.ps f9, f16, f9\n"
			"feq.ps f10, f0, f6\n"
			"fmvz.x.ps x5, f20, 4\n"
			"fcvt.f16.ps f8, f18\n"
			"flog.ps f2, f28\n"
			"bge x10, x20, 1f\n"
			"1:\n"
			"ori x29, x9, 0x5\n"
			"fltm.pi m4, f8, f10\n"
			"li x5, 0x805657f028\n"
			"amomaxul.w x11, x13, (x5)\n" 
			"flt.pi f0, f27, f0\n"
			"bgeu x6, x21, 1f\n"
			"1:\n"
			"flog.ps f13, f27\n"
			"auipc x18, 0x18\n"
			"slt x26, x26, x7\n"
			"mulh x5, x29, x5\n"
			"remu x26, x10, x26\n"
			"bge x15, x4, 1f\n"
			"1:\n"
			"fsat8.pi f8, f10\n"
			"fslli.pi f16, f13, 0xc\n"
			"fsatu8.pi f3, f13\n"
			"li x14, 0x8017814290\n"
			"sb x14, -512(x14)\n" 
			"fsgnj.ps f11, f11, f3\n"
			"fsub.ps f0, f11, f10, rtz\n"
			"fsgnjx.ps f23, f23, f29\n"
			"bge x21, x3, 1f\n"
			"1:\n"
			"flt.ps f20, f29, f3\n"
			"fmul.ps f27, f27, f28, rup\n"
			"fmax.pi f20, f23, f28\n"
			"fsrai.pi f18, f3, 0xa\n"
			"fsgnjn.s f28, f18, f28\n"
			"fsgnjn.ps f2, f29, f29\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f16, 640(x14)\n"
			"flw.ps f28, 480(x14)\n"
			"flt.s x5, f16, f28\n" 
			"fnmadd.s f11, f16, f28, f11, rtz\n"
			"beq x11, x26, 1f\n"
			"1:\n"
			"feq.pi f13, f10, f2\n"
			"fadd.s f2, f29, f6, rtz\n"
			"fmadd.ps f2, f0, f18, f16, rtz\n"
			"for.pi f29, f0, f29\n"
			"feq.ps f23, f3, f16\n"
			"bne x15, x4, 1f\n"
			"1:\n"
			"fslli.pi f8, f16, 0xc\n"
			"fpackreph.pi f13, f23\n"
			"fsrl.pi f16, f29, f18\n"
			"li x27, 0x8014557890\n"
			"fsw.ps f28, 512(x27)\n"
			"flw.ps f8, 512(x27)\n" 
			"fnmsub.s f23, f18, f16, f27, rtz\n"
			"fmax.pi f18, f18, f13\n"
			"fcvt.ps.pw f16, f11, rup\n"
			"blt x19, x7, 1f\n"
			"1:\n"
			"fsub.pi f23, f3, f0\n"
			"ffrc.ps f11, f13\n"
			"fcvt.pw.ps f10, f11, rne\n"
			"blt x17, x24, 1f\n"
			"1:\n"
			"faddi.pi f13, f11, 0x19c\n"
			"fcvt.ps.pw f16, f11, rmm\n"
			"fpackreph.pi f27, f0\n"
			"li x14, 0x8046d1d020\n"
			"amoming.w x18, x7, (x14)\n" 
			"fnmadd.ps f20, f9, f18, f23, rmm\n"
			"fcvt.ps.pw f28, f29, rmm\n"
			"flt.ps f2, f0, f9\n"
			"fcvt.pw.ps f9, f0, rmm\n"
			"fcmovm.ps f20, f20, f11\n"
			"fandi.pi f2, f2, 0x1cb\n"
			"blt x31, x10, 1f\n"
			"1:\n"
			"fmax.pi f28, f20, f23\n"
			"fnmadd.s f9, f18, f20, f18, rne\n"
			"fle.pi f18, f23, f18\n"
			"blt x0, x28, 1f\n"
			"1:\n"
			"li x10, 0x8053da902c\n"
			"amoswapl.w x27, x26, (x10)\n" 
			"fmin.pi f13, f2, f23\n"
			"fcvt.pwu.ps f6, f28, rtz\n"
			"srl x5, x5, x11\n"
			"maskand m3, m6, m1\n"
			"fmsub.s f10, f28, f9, f10, rdn\n"
			"bgeu x9, x0, 1f\n"
			"1:\n"
			"fle.s x5, f29, f6\n"
			"srli x5, x26, 2\n"
			"fmin.s f28, f28, f20\n"
			"fbci.pi f10, 0x59a9a\n"
			"li x27, 0x8010c2e368\n"
			"la x11, mem_ptr_offset\n"
			"flw.ps f13, 128(x11)\n"
			"fsch.ps f16, f13 (x27)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"or x11, x27, x18\n"
			"fsgnjn.s f6, f8, f27\n"
			"divu x27, x27, x26\n"
			"divw x10, x5, x29\n"
			"fclass.ps f11, f8\n"
			"fnmadd.ps f16, f28, f8, f28, rdn\n"
			"or x5, x18, x18\n"
			"bltu x10, x3, 1f\n"
			"1:\n"
			"subw x10, x19, x29\n"
			"sllw x26, x19, x26\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_1_TLD_ID_1_NMASK_0x1_CMASK_0xbf_SHIRE_0_HID_0:\n"
			"li x31, 0x1bf0e\n"
			"csrw tensor_coop, x31\n" 
			"li x31, 0x01FF000001FF0000\n"
			"csrw tensor_mask, x31\n" 
			"li x31, (0x3 << 6) | 0x1\n"
			"li x10, 0x4010008020006b0b\n"
			"csrw tensor_load, x10\n" 
			"li x7, 0x15d800000118007\n"
			"csrw tensor_fma, x7\n" 
			"LBL_SYNC_POINT_4_HID_0:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_4_WAIT_HID_0\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_4_EXIT_HID_0\n"
			"LBL_SYNC_POINT_4_WAIT_HID_0:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_4_EXIT_HID_0:\n"          
			"nop\n"
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_0:\n"
			"li x10, 0x2600000000790018\n"
			"csrw tensor_reduce, x10\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SYNC_POINT_5_HID_0:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_5_WAIT_HID_0\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_5_EXIT_HID_0\n"
			"LBL_SYNC_POINT_5_WAIT_HID_0:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_5_EXIT_HID_0:\n"          
			"nop\n"
			"LBL_SEQID_1_TST_ID_1_QUAD128_START_CORE_0_SHIRE_0_HID_0:\n"
			"li x31, (0x4 << 4)\n"
			"li x10, 0xa25e008020006b00\n"
			"csrw tensor_store, x10\n" 
			"csrwi tensor_wait, 8\n" 
			"LBL_SYNC_POINT_6_HID_0:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_6_WAIT_HID_0\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_6_EXIT_HID_0\n"
			"LBL_SYNC_POINT_6_WAIT_HID_0:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_6_EXIT_HID_0:\n"          
			"nop\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, 0x1d72800000133394\n"
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x6, 0x2138600000000aa9\n"
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 2\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrci x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f11, 3(x31)\n"
			"flw.ps f16, 3(x31)\n"
			"flw.ps f18, 10(x31)\n"
			"flw.ps f23, 2(x31)\n"
			"flw.ps f27, 20(x31)\n"
			"flw.ps f29, 19(x31)\n"
			"flw.ps f9, 13(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x10, 0(x31)\n"
			"lw x14, 2(x31)\n"
			"lw x29, 5(x31)\n"
			"lw x7, 5(x31)\n"
			"li x31, 0x2\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_0:\n"
			"fsrl.pi f6, f10, f10\n"
			"sltiu x10, x10, 0x1c\n"
			"fltm.ps m4, f27, f27\n"
			"for.pi f16, f10, f9\n"
			"mova.x.m x18\n"
			"fmulh.pi f0, f29, f11\n"
			"maskor m3, m1, m4\n"
			"fmin.pi f0, f29, f18\n"
			"bgeu x15, x20, 1f\n"
			"1:\n"
			"sraiw x7, x7, 3\n"
			"bge x24, x8, 1f\n"
			"1:\n"
			"remw x10, x29, x10\n"
			"slti x7, x7, 0xe\n"
			"flt.s x10, f9, f18\n"
			"fsetm.pi m6, f23\n"
			"maskor m6, m3, m4\n"
			"fcvt.ps.pw f29, f29, rne\n"
			"bne x17, x22, 1f\n"
			"1:\n"
			"fmin.pi f18, f9, f18\n"
			"feq.pi f2, f11, f23\n"
			"auipc x14, 0x14\n"
			"fcvt.s.wu f11, x7, rup\n"
			"fmin.pi f2, f29, f27\n"
			"mulh x7, x10, x14\n"
			"fmin.ps f9, f9, f18\n"
			"sllw x18, x10, x10\n"
			"fexp.ps f18, f9\n"
			"fnot.pi f2, f16\n"
			"fand.pi f9, f16, f9\n"
			"fmv.s.x f18, x10\n"
			"fmulh.pi f8, f16, f11\n"
			"and x7, x7, x14\n"
			"div x7, x29, x10\n"
			"fnmadd.s f18, f18, f16, f9, rmm\n"
			"fsgnj.ps f20, f9, f23\n"
			"fsrli.pi f9, f9, 0xb\n"
			"addw x18, x14, x10\n"
			"bne x10, x27, 1f\n"
			"1:\n"
			"div x10, x10, x14\n"
			"fence\n"
			"fcvt.wu.s x7, f29, rdn\n"
			"sra x10, x10, x10\n"
			"fsrai.pi f20, f11, 0x0\n"
			"andi x13, x7, 0xc\n"
			"xor x14, x29, x14\n"
			"fence\n"
			"feq.ps f18, f9, f18\n"
			"feq.pi f0, f29, f23\n"
			"ebreak\n"
			"fmin.pi f18, f18, f11\n"
			"remuw x13, x7, x10\n"
			"bge x2, x5, 1f\n"
			"1:\n"
			"remuw x14, x29, x14\n"
			"fmsub.s f16, f10, f27, f9, rdn\n"
			"srlw x27, x10, x14\n"
			"fmin.s f11, f11, f10\n"
			"fcmov.ps f6, f9, f27, f18\n"
			"fminu.pi f23, f18, f10\n"
			"blt x2, x18, 1f\n"
			"1:\n"
			"fcvt.ps.pw f27, f11, rdn\n"
			"flem.ps m4, f29, f10\n"
			"fmaxu.pi f16, f16, f29\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_1_LOOP_SEQ_HID_0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_HID_0:\n"
			"li x18, 0x8051ce4008\n"
			"amoaddl.w x26, x11, (x18)\n" 
			"fmadd.s f28, f10, f9, f28, rne\n"
			"sllw x5, x29, x27\n"
			"ffrc.ps f29, f0\n"
			"fsat8.pi f28, f0\n"
			"blt x21, x27, 1f\n"
			"1:\n"
			"mul x5, x11, x13\n"
			"addw x11, x9, x11\n"
			"bge x8, x23, 1f\n"
			"1:\n"
			"xori x9, x5, 0x8\n"
			"bltu x1, x27, 1f\n"
			"1:\n"
			"fence\n"
			"blt x13, x3, 1f\n"
			"1:\n"
			"srliw x10, x13, 5\n"
			"bltu x3, x1, 1f\n"
			"1:\n"
			"fmax.pi f3, f18, f3\n"
			"blt x25, x23, 1f\n"
			"1:\n"
			"la x18, rand_int_32\n"
			"flw.ps f23, 352(x18)\n"
			"faddi.pi f6, f23, 0x156\n" 
			"fnmsub.ps f18, f0, f18, f18, rne\n"
			"subw x10, x18, x10\n"
			"fpackreph.pi f20, f23\n"
			"addi x5, x10, 0xe\n"
			"blt x16, x19, 1f\n"
			"1:\n"
			"frcp.ps f0, f2\n"
			"bge x28, x9, 1f\n"
			"1:\n"
			"fle.pi f6, f10, f18\n"
			"fcvt.ps.pw f9, f16, rup\n"
			"sub x10, x11, x18\n"
			"fsgnj.ps f20, f16, f20\n"
			"xor x7, x7, x18\n"
			"li x7, 0x8041e08008\n"
			"amominug.d x19, x10, (x7)\n" 
			"fsgnj.s f20, f23, f29\n"
			"feq.ps f16, f29, f0\n"
			"fcvt.ps.f16 f6, f23\n"
			"flog.ps f0, f0\n"
			"fcvt.ps.pwu f29, f29, rup\n"
			"fmul.pi f29, f29, f29\n"
			"fmin.pi f28, f29, f23\n"
			"blt x1, x16, 1f\n"
			"1:\n"
			"fsrl.pi f6, f23, f29\n"
			"fsrai.pi f9, f23, 0x5\n"
			"fmax.pi f29, f0, f29\n"
			"li x11, 0x805643a038\n"
			"amominl.d x18, x13, (x11)\n" 
			"fmul.s f13, f3, f27, rne\n"
			"slli x5, x13, 5\n"
			"sraw x5, x13, x14\n"
			"fcvt.s.wu f10, x14, rdn\n"
			"flt.s x5, f8, f3\n"
			"fsgnj.s f0, f6, f0\n"
			"blt x1, x18, 1f\n"
			"1:\n"
			"fand.pi f13, f13, f10\n"
			"add x10, x5, x10\n"
			"blt x26, x11, 1f\n"
			"1:\n"
			"maskxor m6, m1, m1\n"
			"fmul.s f0, f10, f0, rmm\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f11, 736(x18)\n"
			"fround.ps f27, f11, rdn\n" 
			"fcmov.ps f20, f10, f3, f20\n"
			"fcmovm.ps f10, f8, f0\n"
			"frcp.ps f8, f16\n"
			"fcvt.f16.ps f9, f20\n"
			"fandi.pi f9, f23, 0x47\n"
			"fadd.ps f13, f8, f27, rdn\n"
			"fcmov.ps f9, f3, f16, f27\n"
			"fmax.pi f0, f23, f27\n"
			"fandi.pi f29, f0, 0xa\n"
			"fadd.s f27, f2, f11, rne\n"
			"li x13, 0x8010d64a68\n"
			"fsw.ps f28, 128(x13)\n"
			"fbc.ps f10, 128(x13)\n" 
			"fcvt.ps.pwu f0, f8, rup\n"
			"fmsub.ps f20, f10, f10, f20, rne\n"
			"fsrai.pi f13, f20, 0x1\n"
			"flt.pi f10, f16, f8\n"
			"fsgnjn.s f8, f3, f10\n"
			"fsra.pi f11, f18, f8\n"
			"fcvt.pwu.ps f0, f6, rup\n"
			"fadd.s f20, f9, f8, rdn\n"
			"fsrli.pi f13, f6, 0x1\n"
			"fpackreph.pi f3, f6\n"
			"li x7, 0x8042e96010\n"
			"amoaddg.w x29, x10, (x7)\n" 
			"fmax.s f13, f29, f2\n"
			"flt.pi f18, f0, f9\n"
			"flt.pi f11, f6, f16\n"
			"fcvt.ps.pw f6, f16, rne\n"
			"feq.ps f13, f8, f20\n"
			"bne x16, x18, 1f\n"
			"1:\n"
			"fmin.s f3, f16, f6\n"
			"fadd.ps f6, f8, f2, rne\n"
			"fmul.s f6, f2, f0, rtz\n"
			"fsgnjn.ps f29, f0, f6\n"
			"fsgnjx.ps f27, f8, f2\n"
			"la x26, rand_int_32\n"
			"flw.ps f29, 704(x26)\n"
			"flw.ps f9, 992(x26)\n"
			"fmax.pi f27, f29, f9\n" 
			"and x10, x27, x11\n"
			"fnmsub.s f3, f3, f23, f23, rmm\n"
			"bge x17, x8, 1f\n"
			"1:\n"
			"fnmsub.s f8, f8, f8, f3, rtz\n"
			"blt x6, x20, 1f\n"
			"1:\n"
			"fswizz.ps f27, f8, 0x12\n"
			"bge x20, x25, 1f\n"
			"1:\n"
			"fsgnj.s f0, f23, f23\n"
			"fnmsub.ps f10, f3, f23, f8, rup\n"
			"bltu x12, x18, 1f\n"
			"1:\n"
			"fxor.pi f20, f0, f3\n"
			"subw x9, x10, x5\n"
			"fsrai.pi f10, f23, 0xd\n"
			"slliw x13, x10, 4\n"
			"li x5, 0x4b1df17bc8ddd357\n"
			"li x29, 0x6f7d08f9cf6a40de\n"
			"sraw x13, x5, x29\n" 
			"fpackreph.pi f29, f11\n"
			"fbci.ps f29, 0x4993a\n"
			"fsub.pi f8, f8, f8\n"
			"blt x5, x25, 1f\n"
			"1:\n"
			"addiw x19, x13, 0x13\n"
			"fmadd.ps f29, f10, f11, f29, rmm\n"
			"div x19, x19, x14\n"
			"fmadd.s f11, f11, f11, f29, rdn\n"
			"sraiw x18, x13, 5\n"
			"fxor.pi f10, f0, f10\n"
			"blt x5, x28, 1f\n"
			"1:\n"
			"divuw x29, x26, x29\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f0, 704(x11)\n"
			"ffrc.ps f0, f0\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fcvt.s.wu f13, x13, rup\n"
			"feq.s x27, f20, f20\n"
			"divu x10, x13, x10\n"
			"fsat8.pi f20, f16\n"
			"mulhu x13, x19, x13\n"
			"mulhu x26, x19, x7\n"
			"bgeu x11, x19, 1f\n"
			"1:\n"
			"fsgnjn.ps f28, f28, f20\n"
			"srli x18, x18, 0\n"
			"ebreak\n"
			"fclass.ps f9, f16\n"
			"li x10, 0x8040c73010\n"
			"la x5, mem_ptr_offset\n"
			"flw.ps f13, 640(x5)\n"
			"fgbg.ps f3, f13 (x10)\n" 
			"fsrl.pi f9, f13, f9\n"
			"frcp.ps f29, f13\n"
			"fsgnjx.ps f27, f16, f13\n"
			"fmadd.ps f18, f23, f13, f9, rmm\n"
			"flog.ps f2, f27\n"
			"fcmovm.ps f29, f6, f13\n"
			"fsub.ps f23, f13, f0, rmm\n"
			"fadd.ps f23, f23, f2, rmm\n"
			"bgeu x28, x12, 1f\n"
			"1:\n"
			"fsgnj.ps f6, f0, f2\n"
			"fcvt.pw.ps f27, f6, rup\n"
			"li x29, 0x80168d6010\n"
			"la x18, mem_ptr_offset\n"
			"flw.ps f28, 832(x18)\n"
			"fgh.ps f6, f28 (x29)\n" 
			"fsra.pi f2, f6, f6\n"
			"fsgnjn.s f28, f6, f6\n"
			"fcvt.pw.ps f8, f13, rne\n"
			"fnot.pi f29, f27\n"
			"fmulh.pi f0, f13, f8\n"
			"fsub.pi f2, f8, f13\n"
			"fsatu8.pi f29, f2\n"
			"fsub.s f10, f8, f6, rup\n"
			"fcmov.ps f8, f8, f2, f10\n"
			"bne x24, x25, 1f\n"
			"1:\n"
			"fpackrepb.pi f0, f13\n"
			"li x7, 0x8047e8f038\n"
			"la x18, mem_ptr_offset\n"
			"flw.ps f20, 608(x18)\n"
			"famoorg.pi f23, f20 (x7)\n" 
			"fmulh.pi f0, f29, f23\n"
			"fnmsub.s f13, f29, f13, f23, rdn\n"
			"fltu.pi f20, f9, f8\n"
			"fmsub.ps f29, f8, f29, f16, rne\n"
			"feq.pi f20, f13, f23\n"
			"fcvt.f16.ps f27, f9\n"
			"fmul.s f18, f16, f8, rdn\n"
			"fcvt.pw.ps f11, f8, rne\n"
			"bge x15, x16, 1f\n"
			"1:\n"
			"fsub.ps f11, f8, f29, rup\n"
			"fxor.pi f20, f29, f16\n"
			"bge x20, x20, 1f\n"
			"1:\n"
			"csrr x0, mhpmevent5\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fltu.pi f29, f20, f23\n"
			"fadd.pi f29, f10, f8\n"
			"fslli.pi f10, f11, 0x2\n"
			"fxor.pi f2, f29, f23\n"
			"fcvt.pwu.ps f11, f23, rmm\n"
			"fsgnjx.s f8, f8, f8\n"
			"fnmadd.ps f29, f11, f29, f8, rmm\n"
			"bltu x9, x28, 1f\n"
			"1:\n"
			"fadd.ps f27, f2, f11, rmm\n"
			"fsgnjn.ps f10, f11, f18\n"
			"fround.ps f29, f23, rmm\n"
			"bge x25, x0, 1f\n"
			"1:\n"
			"li x14, 0x804267201c\n"
			"amoswapg.w x27, x26, (x14)\n" 
			"ffrc.ps f29, f6\n"
			"bgeu x29, x23, 1f\n"
			"1:\n"
			"fmin.s f11, f11, f2\n"
			"fmulh.pi f2, f0, f2\n"
			"bgeu x27, x2, 1f\n"
			"1:\n"
			"fsrai.pi f0, f3, 0x9\n"
			"fle.ps f13, f3, f2\n"
			"fmsub.ps f13, f10, f10, f13, rdn\n"
			"bge x22, x31, 1f\n"
			"1:\n"
			"fexp.ps f10, f2\n"
			"beq x20, x19, 1f\n"
			"1:\n"
			"fmul.pi f0, f0, f11\n"
			"fle.ps f13, f13, f13\n"
			"fmul.s f9, f9, f9, rne\n"
			"li x9, 0x8016c5cd01\n"
			"sd x27, -256(x9)\n"
			"lwu x29, -256(x9)\n" 
			"fadd.ps f16, f13, f13, rdn\n"
			"beq x1, x1, 1f\n"
			"1:\n"
			"fsgnjn.s f2, f2, f2\n"
			"fsub.pi f2, f13, f2\n"
			"flog.ps f28, f13\n"
			"bne x25, x29, 1f\n"
			"1:\n"
			"fpackrepb.pi f0, f2\n"
			"fnmsub.s f2, f13, f2, f10, rtz\n"
			"fpackreph.pi f0, f8\n"
			"flt.ps f0, f2, f2\n"
			"fsrai.pi f28, f2, 0x2\n"
			"fnmsub.ps f2, f2, f2, f2, rmm\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, 0x40008010004ccb\n"
			"li x6, 0x10008010008881\n"
			"li x7, 0x15880000011f021\n"
			"li x9, 0x2f5b200000007766\n"
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, 0x38c0008010008884\n"
			"li x6, 0x10008010006644\n"
			"li x7, 0x12200000011b060\n"
			"li x9, 0x3127600000000077\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 3\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x30\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f10, 1(x31)\n"
			"flw.ps f11, 6(x31)\n"
			"flw.ps f2, 11(x31)\n"
			"flw.ps f27, 5(x31)\n"
			"flw.ps f29, 6(x31)\n"
			"flw.ps f3, 11(x31)\n"
			"flw.ps f8, 27(x31)\n"
			"li x31, 0x2\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_0:\n"
			"ffrc.ps f20, f27\n"
			"flog.ps f6, f10\n"
			"fle.pi f20, f29, f2\n"
			"fxor.pi f2, f10, f11\n"
			"fslli.pi f2, f2, 0x7\n"
			"fsatu8.pi f11, f11\n"
			"fmul.ps f20, f0, f8, rtz\n"
			"fsub.pi f29, f29, f3\n"
			"fmin.s f3, f3, f8\n"
			"fexp.ps f20, f11\n"
			"fpackrepb.pi f20, f2\n"
			"fsrai.pi f6, f8, 0xc\n"
			"fcvt.pw.ps f11, f11, rdn\n"
			"fsgnj.ps f3, f10, f0\n"
			"fmsub.ps f20, f3, f11, f2, rdn\n"
			"bltu x4, x17, 1f\n"
			"1:\n"
			"fminu.pi f10, f10, f8\n"
			"bge x25, x27, 1f\n"
			"1:\n"
			"fadd.pi f10, f11, f27\n"
			"bltu x1, x9, 1f\n"
			"1:\n"
			"fslli.pi f6, f11, 0x6\n"
			"fcmov.ps f29, f0, f3, f29\n"
			"feq.pi f8, f27, f8\n"
			"blt x10, x19, 1f\n"
			"1:\n"
			"fcmov.ps f2, f0, f2, f2\n"
			"fcmov.ps f0, f3, f0, f0\n"
			"fmul.s f2, f2, f2, rne\n"
			"fmax.pi f6, f10, f29\n"
			"fcvt.pw.ps f6, f2, rup\n"
			"fandi.pi f8, f8, 0x7a\n"
			"fmax.pi f0, f3, f0\n"
			"fsrli.pi f27, f10, 0xb\n"
			"fadd.pi f6, f27, f10\n"
			"feq.pi f6, f3, f8\n"
			"bltu x1, x15, 1f\n"
			"1:\n"
			"fmulhu.pi f20, f10, f11\n"
			"fsub.s f6, f10, f8, rne\n"
			"fmulhu.pi f29, f3, f11\n"
			"fmax.pi f11, f11, f2\n"
			"fsgnj.ps f20, f0, f8\n"
			"fsgnjn.s f20, f29, f11\n"
			"fsub.ps f29, f27, f29, rup\n"
			"fclass.ps f29, f29\n"
			"fadd.ps f10, f2, f10, rtz\n"
			"feq.pi f8, f8, f8\n"
			"fmin.s f0, f0, f8\n"
			"fmul.s f10, f29, f10, rup\n"
			"bge x1, x10, 1f\n"
			"1:\n"
			"fslli.pi f29, f8, 0x0\n"
			"fmulhu.pi f20, f2, f8\n"
			"fmul.pi f2, f0, f2\n"
			"fpackreph.pi f29, f3\n"
			"fsub.ps f20, f2, f29, rup\n"
			"fcvt.pw.ps f6, f8, rdn\n"
			"blt x29, x21, 1f\n"
			"1:\n"
			"fle.pi f29, f10, f0\n"
			"fadd.ps f8, f8, f0, rup\n"
			"fmin.s f20, f3, f8\n"
			"fcvt.pwu.ps f20, f27, rup\n"
			"fmax.s f6, f3, f10\n"
			"fexp.ps f10, f11\n"
			"fandi.pi f20, f3, 0xde\n"
			"fcvt.ps.f16 f8, f8\n"
			"fsrai.pi f6, f10, 0x9\n"
			"fmsub.s f6, f27, f0, f2, rne\n"
			"blt x18, x28, 1f\n"
			"1:\n"
			"fand.pi f6, f10, f10\n"
			"frcp.ps f10, f8\n"
			"fnmsub.ps f29, f8, f8, f10, rmm\n"
			"fmax.pi f29, f11, f10\n"
			"fcvt.ps.pwu f6, f10, rtz\n"
			"fadd.pi f11, f11, f2\n"
			"bltu x31, x31, 1f\n"
			"1:\n"
			"fsatu8.pi f20, f8\n"
			"fclass.ps f2, f2\n"
			"fand.pi f8, f10, f8\n"
			"fsub.ps f10, f10, f8, rtz\n"
			"fadd.s f8, f29, f8, rtz\n"
			"fmadd.s f29, f29, f29, f29, rdn\n"
			"frcp.ps f6, f29\n"
			"fcvt.pw.ps f27, f11, rup\n"
			"fsub.pi f6, f8, f3\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_2_LOOP_SEQ_HID_0\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f17,  384 (x5)\n"
			"flw.ps f25,  608 (x5)\n"
			"flw.ps f27,  128 (x5)\n"
			"flw.ps f4,  576 (x5)\n"
			"flw.ps f3,  160 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f1,  736 (x5)\n"
			"flw.ps f23,  544 (x5)\n"
			"flw.ps f19,  640 (x5)\n"
			"flw.ps f21,  960 (x5)\n"
			"flw.ps f18,  32 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f28,  800 (x5)\n"
			"flw.ps f22,  256 (x5)\n"
			"flw.ps f10,  768 (x5)\n"
			"flw.ps f14,  0 (x5)\n"
			"flw.ps f0,  192 (x5)\n"
			"flw.ps f31,  448 (x5)\n"
			"flw.ps f20,  704 (x5)\n"
			"flw.ps f12,  928 (x5)\n"
			"flw.ps f26,  416 (x5)\n"
			"flw.ps f24,  512 (x5)\n"
			"flw.ps f16,  224 (x5)\n"
			"flw.ps f9,  320 (x5)\n"
			"flw.ps f7,  672 (x5)\n"
			"flw.ps f6,  864 (x5)\n"
			"flw.ps f8,  896 (x5)\n"
			"flw.ps f30,  288 (x5)\n"
			"flw.ps f2,  480 (x5)\n"
			"flw.ps f13,  96 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_3_HID_0:\n"
			"li x27, 0x8c259234e3c066b1\n"
			"li x26, 0xe0996bc405374ae9\n"
			"add x27, x27, x26\n" 
			"fsgnjx.s f28, f13, f8\n"
			"li x7, 0x8051ad7008\n"
			"la x11, mem_ptr_offset\n"
			"flw.ps f6, 928(x11)\n"
			"fgwl.ps f18, f6 (x7)\n" 
			"flt.ps f28, f27, f11\n"
			"li x18, 0x801243e56b\n"
			"sd x9, -512(x18)\n"
			"lw x13, -512(x18)\n" 
			"mulhsu x11, x11, x27\n"
			"li x29, 0xa2529e38d3403c9f\n"
			"li x14, 0x9a361b1d4d58407e\n"
			"sll x7, x29, x14\n" 
			"fle.s x18, f6, f6\n"
			"bltu x5, x9, 1f\n"
			"1:\n"
			"li x10, 0x8015c011b0\n"
			"la x18, mem_ptr_offset\n"
			"flw.ps f23, 224(x18)\n"
			"fsch.ps f16, f23 (x10)\n" 
			"fsrl.pi f20, f18, f3\n"
			"bge x3, x18, 1f\n"
			"1:\n"
			"li x18, 0x8010950d4e\n"
			"sb x29, 128(x18)\n" 
			"srliw x10, x29, 0\n"
			"li x7, 0x8046d30038\n"
			"amoorg.w x18, x13, (x7)\n" 
			"li x5, 0x33c4d72b5d672b9b\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x9206b1a97a82b8cc\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"mova.x.m x19\n"
			"li x14, 0x8055078030\n"
			"la x29, mem_ptr_offset\n"
			"flw.ps f29, 224(x29)\n"
			"famomaxul.pi f11, f29 (x14)\n" 
			"maskor m1, m1, m4\n"
			"li x13, 0x805352c030\n"
			"la x19, mem_ptr_offset\n"
			"flw.ps f27, 544(x19)\n"
			"famominul.pi f27, f27 (x13)\n" 
			"fcmovm.ps f9, f11, f18\n"
			"li x18, 0x8013b097d0\n"
			"li x29, 2573156070\n"
			"fg32h.ps f11, x9 (x18)\n" 
			"fmulh.pi f16, f27, f0\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 256(x7)\n"
			"fcvt.lu.s x19, f10, rne\n" 
			"mova.m.x x27\n"
			"li x27, 0x80139dfc53\n"
			"sd x14, -256(x27)\n"
			"lw x18, -256(x27)\n" 
			"li x5, 0x1053b8483c6cb001\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xfcab0791d6d43e75\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"fnmsub.s f11, f9, f18, f11, rdn\n"
			"li x11, 0x8052e1b018\n"
			"la x29, mem_ptr_offset\n"
			"flw.ps f10, 864(x29)\n"
			"famoaddl.pi f27, f10 (x11)\n" 
			"fnmadd.s f0, f8, f3, f9, rtz\n"
			"csrr x0, mhpmcounter8\n"
			"ecall\n"
			"li x19, 0x8054d36020\n"
			"amomaxul.w x13, x26, (x19)\n" 
			"fsub.pi f16, f10, f9\n"
			"maskpopcz x10, m3\n" 
			"fsgnjx.ps f28, f10, f27\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, 0x10200080100020c3\n"
			"li x6, 0x10008010008884\n"
			"li x7, 0x9a00000011f012\n"
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 3\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f10, 4(x31)\n"
			"flw.ps f11, 7(x31)\n"
			"flw.ps f13, 8(x31)\n"
			"flw.ps f16, 4(x31)\n"
			"flw.ps f18, 4(x31)\n"
			"flw.ps f2, 19(x31)\n"
			"flw.ps f20, 14(x31)\n"
			"flw.ps f23, 28(x31)\n"
			"flw.ps f27, 25(x31)\n"
			"flw.ps f28, 10(x31)\n"
			"flw.ps f29, 13(x31)\n"
			"flw.ps f6, 10(x31)\n"
			"flw.ps f8, 50(x31)\n"
			"flw.ps f9, 47(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x10, 0(x31)\n"
			"lw x11, 3(x31)\n"
			"lw x13, 7(x31)\n"
			"lw x14, 7(x31)\n"
			"lw x18, 16(x31)\n"
			"lw x19, 10(x31)\n"
			"lw x26, 2(x31)\n"
			"lw x27, 18(x31)\n"
			"lw x29, 24(x31)\n"
			"lw x5, 35(x31)\n"
			"lw x7, 7(x31)\n"
			"lw x9, 28(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_3_LOOP_SEQ_HID_0:\n"
			"fclass.s x10, f8\n"
			"fence\n"
			"fnmsub.s f10, f6, f9, f20, rdn\n"
			"bne x16, x27, 1f\n"
			"1:\n"
			"mova.x.m x14\n"
			"mulhu x14, x14, x18\n"
			"fsll.pi f28, f18, f27\n"
			"fltu.pi f18, f0, f10\n"
			"fsgnjx.ps f23, f13, f20\n"
			"feq.pi f2, f29, f9\n"
			"slti x10, x9, 0x1c\n"
			"fmsub.ps f11, f20, f28, f11, rdn\n"
			"fbcx.ps f20, x27\n"
			"bgeu x8, x27, 1f\n"
			"1:\n"
			"auipc x13, 0x14\n"
			"fcvt.w.s x27, f0, rmm\n"
			"mulhu x26, x26, x5\n"
			"fmsub.ps f8, f9, f6, f27, rdn\n"
			"rem x29, x13, x13\n"
			"xori x14, x5, 0x1b\n"
			"fcvt.f16.ps f10, f27\n"
			"or x5, x5, x11\n"
			"ffrc.ps f11, f18\n"
			"bltu x20, x17, 1f\n"
			"1:\n"
			"addiw x10, x27, 0x1d\n"
			"bne x28, x16, 1f\n"
			"1:\n"
			"masknot m1, m4\n"
			"srl x18, x19, x10\n"
			"blt x23, x31, 1f\n"
			"1:\n"
			"maskxor m3, m6, m1\n"
			"bltu x24, x18, 1f\n"
			"1:\n"
			"ebreak\n"
			"ecall\n"
			"fcmov.ps f13, f23, f29, f13\n"
			"remw x29, x13, x29\n"
			"bltu x28, x18, 1f\n"
			"1:\n"
			"fmv.x.s x14, f2\n"
			"bltu x5, x23, 1f\n"
			"1:\n"
			"div x27, x5, x27\n"
			"xori x9, x5, 0x0\n"
			"remu x7, x27, x7\n"
			"ffrc.ps f6, f2\n"
			"fsrl.pi f23, f20, f16\n"
			"fltu.pi f23, f18, f20\n"
			"fadd.s f9, f13, f29, rdn\n"
			"fmadd.ps f23, f8, f27, f29, rne\n"
			"fxor.pi f28, f28, f11\n"
			"mulhu x5, x14, x5\n"
			"fnmsub.s f10, f11, f23, f8, rdn\n"
			"fmul.pi f10, f10, f11\n"
			"fsatu8.pi f13, f13\n"
			"xori x7, x7, 0x7\n"
			"fand.pi f3, f20, f8\n"
			"fsra.pi f10, f13, f28\n"
			"fsrli.pi f10, f10, 0x9\n"
			"slli x26, x19, 1\n"
			"bgeu x23, x31, 1f\n"
			"1:\n"
			"sraiw x9, x9, 0\n"
			"mulh x19, x9, x9\n"
			"bgeu x3, x7, 1f\n"
			"1:\n"
			"mulhu x18, x7, x9\n"
			"fsetm.pi m3, f13\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_3_LOOP_SEQ_HID_0\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_4_HID_0:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f23, 672(x27)\n"
			"flw.ps f18, 960(x27)\n"
			"flw.ps f0, 192(x27)\n"
			"fnmsub.s f28, f23, f18, f0, rtz\n" 
			"sltiu x19, x27, 0x1b\n"
			"li x9, 0xa2ac7d63d5c25dc1\n"
			"li x13, 0xc4f6bad1a0c33a09\n"
			"sub x11, x9, x13\n" 
			"mova.m.x x7\n"
			"li x7, 0x80560e7000\n"
			"amomaxul.d x29, x26, (x7)\n" 
			"fcvt.w.s x10, f10, rdn\n"
			"li x11, 0x8055e31000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f11, (x11)\n"
			"flwl.ps f27, (x11)\n" 
			"fnmadd.s f6, f6, f6, f27, rne\n"
			"li x18, 0x8050024010\n"
			"amoswapl.d x7, x14, (x18)\n" 
			"fminu.pi f6, f0, f0\n"
			"fbci.ps f8, 0x147b5\n" 
			"feq.pi f16, f10, f28\n"
			"li x5, 0x80459f3008\n"
			"amominug.d x26, x29, (x5)\n" 
			"srli x26, x26, 3\n"
			"li x7, 0x80168d8b60\n"
			"li x11, 1001438595\n"
			"fsc32b.ps f11, x19 (x7)\n" 
			"fsub.ps f9, f23, f9, rmm\n"
			"li x14, 0x80154794b8\n"
			"fsw f9, -512(x14)\n" 
			"fsrai.pi f29, f23, 0x8\n"
			"li x9, 0x8056b0701c\n"
			"amominl.w x7, x29, (x9)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fsub.s f9, f13, f11, rup\n"
			"li x26, 0x8017ed413d\n"
			"sh x9, 256(x26)\n" 
			"fsll.pi f0, f23, f28\n"
			"bltu x17, x14, 1f\n"
			"1:\n"
			"li x9, 0x804098c030\n"
			"la x13, mem_ptr_offset\n"
			"flw.ps f8, 608(x13)\n"
			"fgbg.ps f10, f8 (x9)\n" 
			"srlw x9, x10, x10\n"
			"la x14, rand_int_32\n"
			"flw.ps f18, 512(x14)\n"
			"fmulh.pi f0, f18, f18\n" 
			"fslli.pi f29, f2, 0x7\n"
			"li x13, 0x80509c3024\n"
			"amoorl.w x10, x27, (x13)\n" 
			"fcmov.ps f11, f27, f0, f20\n"
			"li x26, 0x801269a00c\n"
			"sd x29, 512(x26)\n"
			"lbu x11, 512(x26)\n" 
			"fandi.pi f28, f16, 0x142\n"
			"li x29, 0x8055eba010\n"
			"la x7, mem_ptr_offset\n"
			"flw.ps f16, 384(x7)\n"
			"fghl.ps f29, f16 (x29)\n" 
			"maskpopc x27, m1\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, 0x399f602776776666\n"
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x6, 0x1ba2200776666676\n"
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 3\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x50\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f11, 8(x31)\n"
			"flw.ps f13, 6(x31)\n"
			"flw.ps f16, 5(x31)\n"
			"flw.ps f18, 17(x31)\n"
			"flw.ps f2, 10(x31)\n"
			"flw.ps f20, 12(x31)\n"
			"flw.ps f23, 18(x31)\n"
			"flw.ps f27, 3(x31)\n"
			"flw.ps f28, 33(x31)\n"
			"flw.ps f29, 39(x31)\n"
			"flw.ps f3, 42(x31)\n"
			"flw.ps f6, 47(x31)\n"
			"flw.ps f8, 52(x31)\n"
			"flw.ps f9, 17(x31)\n"
			"li x31, 0x2\n"
			"LBL_SEQID_4_LOOP_SEQ_HID_0:\n"
			"fsgnj.ps f3, f3, f8\n"
			"fsgnjx.s f8, f29, f3\n"
			"bne x18, x7, 1f\n"
			"1:\n"
			"fmul.ps f27, f6, f27, rup\n"
			"fclass.ps f13, f18\n"
			"bne x21, x11, 1f\n"
			"1:\n"
			"fmadd.s f27, f8, f8, f18, rne\n"
			"bltu x7, x22, 1f\n"
			"1:\n"
			"fsrli.pi f8, f29, 0x8\n"
			"fmin.ps f23, f13, f16\n"
			"fcmov.ps f16, f13, f18, f16\n"
			"fcvt.pwu.ps f20, f10, rne\n"
			"fsgnjn.ps f8, f29, f20\n"
			"fmaxu.pi f2, f27, f11\n"
			"fswizz.ps f18, f2, 0x1f\n"
			"fxor.pi f0, f0, f29\n"
			"flog.ps f9, f20\n"
			"fsll.pi f2, f18, f28\n"
			"fmaxu.pi f11, f16, f28\n"
			"fmulhu.pi f10, f2, f18\n"
			"beq x23, x30, 1f\n"
			"1:\n"
			"feq.ps f9, f20, f27\n"
			"blt x16, x3, 1f\n"
			"1:\n"
			"fsgnj.ps f6, f6, f9\n"
			"fle.pi f28, f23, f16\n"
			"fmulhu.pi f28, f20, f27\n"
			"fsat8.pi f3, f29\n"
			"fclass.ps f18, f23\n"
			"beq x2, x6, 1f\n"
			"1:\n"
			"fround.ps f23, f10, rdn\n"
			"fsgnjx.ps f18, f6, f23\n"
			"bge x24, x14, 1f\n"
			"1:\n"
			"fmsub.s f29, f29, f16, f10, rne\n"
			"fsll.pi f8, f2, f11\n"
			"beq x8, x30, 1f\n"
			"1:\n"
			"fnmsub.ps f13, f9, f8, f13, rdn\n"
			"fmadd.ps f29, f23, f29, f13, rup\n"
			"fsgnjn.ps f20, f18, f23\n"
			"fmaxu.pi f0, f23, f10\n"
			"fmul.s f6, f10, f6, rne\n"
			"bge x9, x26, 1f\n"
			"1:\n"
			"fmulhu.pi f13, f2, f29\n"
			"fsrai.pi f29, f8, 0xd\n"
			"fmulh.pi f2, f20, f28\n"
			"fmin.pi f10, f16, f29\n"
			"fandi.pi f27, f29, 0x9a\n"
			"fsub.pi f16, f16, f11\n"
			"fsub.ps f0, f29, f28, rne\n"
			"fmax.pi f9, f9, f29\n"
			"fcvt.f16.ps f8, f13\n"
			"fcmovm.ps f23, f6, f0\n"
			"bltu x8, x25, 1f\n"
			"1:\n"
			"fxor.pi f8, f8, f8\n"
			"fsll.pi f9, f9, f27\n"
			"bltu x27, x17, 1f\n"
			"1:\n"
			"fnmadd.s f29, f2, f23, f13, rtz\n"
			"fsgnjn.ps f8, f8, f28\n"
			"fcmov.ps f13, f13, f28, f16\n"
			"fsgnjx.s f18, f3, f18\n"
			"fmaxu.pi f27, f20, f27\n"
			"bne x13, x1, 1f\n"
			"1:\n"
			"fsrl.pi f29, f29, f29\n"
			"fmul.pi f10, f29, f10\n"
			"flt.pi f0, f27, f6\n"
			"fmulh.pi f29, f13, f18\n"
			"fsub.pi f11, f20, f3\n"
			"flt.pi f23, f27, f10\n"
			"frcp.ps f13, f13\n"
			"fmulh.pi f9, f9, f13\n"
			"fclass.ps f29, f9\n"
			"fsrai.pi f3, f13, 0xa\n"
			"bltu x31, x21, 1f\n"
			"1:\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_4_LOOP_SEQ_HID_0\n"
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f16,  0 (x5)\n"
			"flw.ps f26,  544 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f27,  32 (x5)\n"
			"flw.ps f4,  64 (x5)\n"
			"flw.ps f17,  864 (x5)\n"
			"flw.ps f6,  96 (x5)\n"
			"flw.ps f9,  384 (x5)\n"
			"flw.ps f19,  576 (x5)\n"
			"flw.ps f2,  928 (x5)\n"
			"flw.ps f18,  736 (x5)\n"
			"flw.ps f0,  960 (x5)\n"
			"flw.ps f11,  832 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f12,  768 (x5)\n"
			"flw.ps f14,  512 (x5)\n"
			"flw.ps f23,  224 (x5)\n"
			"flw.ps f20,  160 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
			"flw.ps f13,  128 (x5)\n"
			"flw.ps f3,  992 (x5)\n"
			"flw.ps f24,  896 (x5)\n"
			"flw.ps f31,  640 (x5)\n"
			"flw.ps f29,  416 (x5)\n"
			"flw.ps f7,  288 (x5)\n"
			"flw.ps f1,  320 (x5)\n"
			"flw.ps f25,  608 (x5)\n"
			"flw.ps f8,  448 (x5)\n"
			"flw.ps f10,  672 (x5)\n"
			"flw.ps f22,  352 (x5)\n"
			"flw.ps f15,  480 (x5)\n"
			"la x31, check_sum_hid_0\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"LBL_POSTAMBLE_HID_0:\n"          
			"csrwi   tensor_wait, 0x0\n"
			"csrwi   tensor_wait, 0x1\n"
			"csrwi   tensor_wait, 0x2\n"
			"csrwi   tensor_wait, 0x3\n"
			"csrwi   tensor_wait, 0x4\n"
			"csrwi   tensor_wait, 0x5\n"
			"csrwi   tensor_wait, 0x6\n"
			"csrwi   tensor_wait, 0x7\n"
			"csrwi   tensor_wait, 0x8\n"
			"csrwi   tensor_wait, 0x9\n"
			"csrwi   tensor_wait, 0xa\n"
			"fence\n"
			"LBL_SYNC_POINT_7_HID_0:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_7_WAIT_HID_0\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_7_EXIT_HID_0\n"
			"LBL_SYNC_POINT_7_WAIT_HID_0:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_7_EXIT_HID_0:\n"          
			"nop\n"
			"LBL_DIS_COOP_ESR_HID_0:\n"          
			"li x5, 0x80740290\n"  
			"li x31, 0\n"
			"sd x31, 0(x5)\n"
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_0\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_0:\n"          
			".global LBL_FAIL_HID_0\n"          
			"la x31, check_sum_hid_0\n"
			"li x5, 0xDEADDEADDEADDEAD\n"
			"sd x5, 24(x31)\n"
			"LBL_HPM_3_CORE_TXFMA_OPS32_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_MISSES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TQUANT_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_OPS16_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_NEIGH_COOP_TLD_REQ_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_TX_COOP_TST_REQ_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter8\n"
			"sd x5, 72 (x31)\n"
			"slti x0, x0, 0x7ff\n"
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_0\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_0:\n"          
			".global LBL_PASS_HID_0\n"          
			"la x31, check_sum_hid_0\n"
			"li x5, 0xACEDACEDACEDACED\n"
			"sd x5, 24(x31)\n"
			"LBL_HPM_3_CORE_TXFMA_OPS32_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_MISSES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TQUANT_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_OPS16_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_NEIGH_COOP_TLD_REQ_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_TX_COOP_TST_REQ_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter8\n"
			"sd x5, 72 (x31)\n"
			"slti x0, x0, 0x7fe\n"
			VSNIP_RSV
		);
		c_test_pass();
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_0:\n"          
			".global LBL_HALT_HID_0\n"          
			"wfi\n"
			"j LBL_HALT_HID_0\n"          
			VSNIP_RSV
		);
		return 0;       
	};
	if ( hid == 1 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_1:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_1\n"
			"csrw mscratch, x5\n"
			"csrwi menable_shadows, 0xf\n"
			"li x5, -1\n"
			"csrw mcounteren, x5\n"
			"csrw scounteren, x5\n"
			"csrwi mhpmcounter3, 0\n"
			"csrwi mhpmcounter4, 0\n"
			"csrwi mhpmcounter5, 0\n"
			"csrwi mhpmcounter6, 0\n"
			"csrwi mhpmcounter7, 0\n"
			"csrwi mhpmcounter8, 0\n"
			"csrwi mhpmevent3, 7\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 9\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 23\n"   
			"mov.m.x m0, x0, 0xff\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f6,  320 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"flw.ps f29,  768 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f24,  160 (x5)\n"
			"flw.ps f30,  960 (x5)\n"
			"flw.ps f16,  0 (x5)\n"
			"flw.ps f18,  352 (x5)\n"
			"flw.ps f31,  512 (x5)\n"
			"flw.ps f27,  640 (x5)\n"
			"flw.ps f19,  384 (x5)\n"
			"flw.ps f28,  32 (x5)\n"
			"flw.ps f4,  96 (x5)\n"
			"flw.ps f8,  544 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f23,  608 (x5)\n"
			"flw.ps f14,  288 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f11,  896 (x5)\n"
			"flw.ps f1,  736 (x5)\n"
			"flw.ps f2,  416 (x5)\n"
			"flw.ps f21,  64 (x5)\n"
			"flw.ps f0,  128 (x5)\n"
			"flw.ps f25,  928 (x5)\n"
			"flw.ps f7,  864 (x5)\n"
			"flw.ps f3,  704 (x5)\n"
			"flw.ps f17,  480 (x5)\n"
			"flw.ps f26,  448 (x5)\n"
			"flw.ps f9,  672 (x5)\n"
			"flw.ps f15,  992 (x5)\n"
			"flw.ps f10,  576 (x5)\n"
			"flw.ps f22,  192 (x5)\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_HID_1:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_0_WAIT_HID_1\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_0_EXIT_HID_1\n"
			"LBL_SYNC_POINT_0_WAIT_HID_1:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_EXIT_HID_1:\n"          
			"nop\n"
			"LBL_KERNEL_HID_1:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_1\n"
			"li x5, 0x8\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_HID_1:\n"
			"la x21, rand_int_32\n"
			"flw.ps f13, 64(x21)\n"
			"flw.ps f28, 352(x21)\n"
			"feq.pi f7, f13, f28\n" 
			"fsub.ps f4, f13, f10, rdn\n"
			"divuw x5, x29, x28\n"
			"li x14, 0x8048b06000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f28, (x14)\n"
			"flwg.ps f26, (x14)\n" 
			"rem x0, x22, x0\n"
			"bltu x7, x23, 1f\n"
			"1:\n"
			"fexp.ps f13, f21\n"
			"bne x12, x11, 1f\n"
			"1:\n"
			"li x29, 0x805b033000\n"
			"amomaxl.d x7, x29, (x29)\n" 
			"fmsub.ps f28, f7, f10, f29, rup\n"
			"fnot.pi f29, f13\n"
			"li x13, 0x801b5f3b00\n"
			"sd x6, 128(x13)\n"
			"lb x5, 128(x13)\n" 
			"fminu.pi f29, f11, f25\n"
			"fsat8.pi f16, f1\n"
			"li x12, 0x8049096028\n"
			"la x28, mem_ptr_offset\n"
			"flw.ps f29, 64(x28)\n"
			"famoming.ps f4, f29 (x12)\n" 
			"fmulhu.pi f24, f24, f26\n"
			"fsub.ps f25, f16, f1, rne\n"
			"li x10, 0x804e108038\n"
			"amominug.w x7, x13, (x10)\n" 
			"fxor.pi f29, f8, f21\n"
			"fcvt.s.w f16, x0, rne\n"
			"li x21, 0x805fdd4010\n"
			"amoswapl.d x6, x14, (x21)\n" 
			"fcvt.ps.f16 f7, f28\n"
			"fxor.pi f17, f28, f28\n"
			"li x31, 0x8020006b00\n"
			"li x29, 0x8c0000000000000e\n"
			"add x29, x31, x29\n"
			"li x31, (0x1 << 6) | 0x0\n"
			"csrwi tensor_wait, 6\n" 
			"fsrli.pi f23, f13, 0x7\n"
			"bgeu x12, x22, 1f\n"
			"1:\n"
			"fnmadd.ps f4, f13, f21, f13, rtz\n"
			"li x13, 0x805a4f4020\n"
			"la x12, mem_ptr_offset\n"
			"flw.ps f21, 320(x12)\n"
			"famoxorl.pi f26, f21 (x13)\n" 
			"fsub.pi f1, f26, f7\n"
			"beq x30, x23, 1f\n"
			"1:\n"
			"flt.pi f25, f26, f25\n"
			"bltu x16, x18, 1f\n"
			"1:\n"
			"li x13, 0x805ed5c028\n"
			"la x12, mem_ptr_offset\n"
			"flw.ps f28, 896(x12)\n"
			"famoandl.pi f13, f28 (x13)\n" 
			"for.pi f1, f7, f8\n"
			"for.pi f1, f8, f4\n"
			"mova.x.m x21\n" 
			"div x21, x21, x21\n"
			"fcvt.pwu.ps f4, f16, rne\n"
			"bne x21, x25, 1f\n"
			"1:\n"
			"li x13, 0x805fd62028\n"
			"amomaxul.d x14, x14, (x13)\n" 
			"fxor.pi f25, f11, f25\n"
			"bltu x28, x16, 1f\n"
			"1:\n"
			"frcp.ps f25, f7\n"
			"li x14, 0x7e12ec73e0d35749\n"
			"andi x22, x14, 0x2\n" 
			"sltu x5, x5, x5\n"
			"fswizz.ps f11, f7, 0x12\n"
			"li x28, 0x805deec010\n"
			"la x14, mem_ptr_offset\n"
			"flw.ps f24, 704(x14)\n"
			"fscbl.ps f7, f24 (x28)\n" 
			"fsat8.pi f29, f28\n"
			"beq x5, x20, 1f\n"
			"1:\n"
			"fle.s x0, f4, f26\n"
			"li x22, 0x804972f028\n"
			"amoandg.d x14, x28, (x22)\n" 
			"flt.ps f7, f1, f21\n"
			"fmulhu.pi f10, f7, f10\n"
			"li x6, 0x805b437010\n"
			"amominl.d x7, x28, (x6)\n" 
			"fmin.s f10, f21, f10\n"
			"fmul.pi f4, f26, f16\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x8\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f13, 5(x31)\n"
			"flw.ps f16, 6(x31)\n"
			"flw.ps f17, 7(x31)\n"
			"flw.ps f21, 6(x31)\n"
			"flw.ps f23, 23(x31)\n"
			"flw.ps f24, 0(x31)\n"
			"flw.ps f25, 14(x31)\n"
			"flw.ps f26, 23(x31)\n"
			"flw.ps f28, 1(x31)\n"
			"flw.ps f29, 38(x31)\n"
			"flw.ps f4, 20(x31)\n"
			"flw.ps f7, 0(x31)\n"
			"flw.ps f8, 16(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x10, 1(x31)\n"
			"lw x12, 2(x31)\n"
			"lw x13, 10(x31)\n"
			"lw x14, 5(x31)\n"
			"lw x21, 14(x31)\n"
			"lw x22, 4(x31)\n"
			"lw x28, 8(x31)\n"
			"lw x29, 31(x31)\n"
			"lw x5, 25(x31)\n"
			"lw x6, 16(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_1:\n"
			"fclass.ps f25, f25\n"
			"bne x11, x2, 1f\n"
			"1:\n"
			"maskor m2, m2, m6\n"
			"slt x0, x0, x28\n"
			"fmv.x.s x10, f10\n"
			"addw x22, x6, x10\n"
			"fle.s x12, f21, f13\n"
			"fsgnjx.s f13, f13, f16\n"
			"sll x7, x14, x28\n"
			"slti x7, x12, 0x1b\n"
			"sll x5, x5, x22\n"
			"div x0, x13, x13\n"
			"fmadd.ps f29, f26, f23, f7, rne\n"
			"srli x21, x5, 5\n"
			"fcmovm.ps f23, f8, f29\n"
			"blt x12, x20, 1f\n"
			"1:\n"
			"mulh x29, x6, x10\n"
			"rem x14, x14, x10\n"
			"fbcx.ps f4, x5\n"
			"fsatu8.pi f23, f23\n"
			"fadd.pi f21, f21, f10\n"
			"remuw x14, x0, x14\n"
			"blt x30, x26, 1f\n"
			"1:\n"
			"fbcx.ps f13, x22\n"
			"flt.s x28, f7, f16\n"
			"fadd.pi f23, f8, f8\n"
			"fminu.pi f10, f10, f25\n"
			"mulw x6, x6, x10\n"
			"sltu x12, x22, x12\n"
			"addw x21, x6, x21\n"
			"srliw x5, x21, 5\n"
			"bne x21, x9, 1f\n"
			"1:\n"
			"slliw x12, x21, 1\n"
			"fcvt.pw.ps f4, f23, rne\n"
			"remw x29, x6, x29\n"
			"bltu x3, x25, 1f\n"
			"1:\n"
			"fmv.x.s x6, f7\n"
			"bne x4, x2, 1f\n"
			"1:\n"
			"divuw x13, x0, x10\n"
			"fmin.pi f21, f23, f28\n"
			"fmsub.ps f13, f17, f28, f1, rmm\n"
			"fcvt.ps.pw f25, f29, rne\n"
			"fbci.ps f21, 0x6adf3\n"
			"fslli.pi f25, f8, 0xe\n"
			"fsgnjx.ps f8, f29, f29\n"
			"or x28, x28, x13\n"
			"fle.s x22, f13, f25\n"
			"fnmsub.s f26, f26, f23, f7, rdn\n"
			"fsatu8.pi f8, f29\n"
			"bltu x7, x11, 1f\n"
			"1:\n"
			"fle.pi f8, f23, f29\n"
			"fmulh.pi f16, f16, f25\n"
			"masknot m2, m3\n"
			"fmulh.pi f8, f25, f7\n"
			"fle.ps f13, f24, f21\n"
			"remw x21, x12, x10\n"
			"beq x24, x13, 1f\n"
			"1:\n"
			"fminu.pi f24, f7, f21\n"
			"fcvt.ps.pw f1, f1, rup\n"
			"fnot.pi f29, f26\n"
			"fnot.pi f26, f4\n"
			"bne x20, x25, 1f\n"
			"1:\n"
			"fltm.ps m1, f24, f16\n"
			"fmax.ps f1, f29, f25\n"
			"slti x6, x0, 0xa\n"
			"fclass.ps f16, f17\n"
			"fsgnj.s f16, f16, f17\n"
			"add x6, x10, x6\n"
			"fslli.pi f13, f26, 0x7\n"
			"fcvt.w.s x28, f26, rmm\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_0_LOOP_SEQ_HID_1\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_HID_1:\n"
			"li x28, 0xfdee2b0efef72459\n"
			"li x7, 0xb378cee80bdf9f76\n"
			"srlw x28, x28, x7\n" 
			"fmax.s f26, f13, f26\n"
			"fmsub.ps f8, f1, f4, f29, rne\n"
			"fmul.pi f23, f28, f1\n"
			"fcvt.pw.ps f21, f29, rdn\n"
			"li x21, 0x8058322038\n"
			"la x22, mem_ptr_offset\n"
			"flw.ps f29, 736(x22)\n"
			"fgwl.ps f16, f29 (x21)\n" 
			"divw x22, x10, x22\n"
			"maskpopcz x22, m1\n"
			"bgeu x9, x19, 1f\n"
			"1:\n"
			"fmsub.s f25, f25, f28, f8, rne\n"
			"fmax.s f8, f28, f16\n"
			"li x21, 0x805bd3f01c\n"
			"amoaddl.w x29, x5, (x21)\n" 
			"fround.ps f21, f13, rmm\n"
			"fltu.pi f1, f13, f26\n"
			"fslli.pi f17, f25, 0xe\n"
			"fandi.pi f16, f26, 0x1d0\n"
			"li x29, 0xc3db0d2c7e522be8\n"
			"fcvt.s.wu f21, x29, rmm\n" 
			"fsgnj.ps f4, f8, f29\n"
			"fle.pi f29, f29, f8\n"
			"blt x25, x21, 1f\n"
			"1:\n"
			"fsra.pi f16, f21, f29\n"
			"fsra.pi f17, f8, f8\n"
			"li x5, 0x44c2dc929e4f721e\n"
			"li x7, 0x2ececabd7050b908\n"
			"srlw x5, x5, x7\n" 
			"fxor.pi f28, f16, f4\n"
			"blt x16, x9, 1f\n"
			"1:\n"
			"feq.pi f24, f25, f24\n"
			"bge x26, x8, 1f\n"
			"1:\n"
			"fand.pi f23, f10, f24\n"
			"fmax.ps f10, f10, f16\n"
			"blt x22, x10, 1f\n"
			"1:\n"
			"la x21, rand_int_32\n"
			"flw.ps f23, 544(x21)\n"
			"fpackrepb.pi f29, f23\n" 
			"divw x22, x0, x7\n"
			"fcvt.w.s x0, f11, rup\n"
			"fmin.s f25, f23, f23\n"
			"fnmadd.ps f13, f4, f4, f10, rmm\n"
			"li x22, 0x804a87d000\n"
			"amominug.d x5, x10, (x22)\n" 
			"fswizz.ps f21, f26, 0x1\n"
			"bgeu x6, x21, 1f\n"
			"1:\n"
			"feq.ps f16, f4, f23\n"
			"sll x13, x28, x28\n"
			"and x29, x29, x5\n"
			"bltu x21, x7, 1f\n"
			"1:\n"
			"csrr x0, stall\n"
			"feq.ps f11, f21, f11\n"
			"fmsub.ps f17, f7, f7, f13, rdn\n"
			"ffrc.ps f21, f29\n"
			"bltu x8, x26, 1f\n"
			"1:\n"
			"fnmsub.s f10, f10, f7, f8, rup\n"
			"beq x31, x10, 1f\n"
			"1:\n"
			"li x5, 0x805f9b8008\n"
			"la x28, mem_ptr_offset\n"
			"flw.ps f25, 480(x28)\n"
			"famominl.pi f21, f25 (x5)\n" 
			"fsrli.pi f25, f23, 0x8\n"
			"fsub.ps f25, f17, f17, rne\n"
			"bne x31, x25, 1f\n"
			"1:\n"
			"fsgnjx.ps f17, f17, f7\n"
			"bge x30, x14, 1f\n"
			"1:\n"
			"fsub.s f17, f13, f23, rne\n"
			"li x22, 0x804945e000\n"
			"la x21, mem_ptr_offset\n"
			"flw.ps f17, 512(x21)\n"
			"famoxorg.pi f23, f17 (x22)\n" 
			"fmul.pi f1, f25, f13\n"
			"and x21, x5, x29\n"
			"ebreak\n"
			"bltu x21, x21, 1f\n"
			"1:\n"
			"fandi.pi f11, f25, 0x5b\n"
			"li x13, 0x805cea8008\n"
			"la x12, mem_ptr_offset\n"
			"flw.ps f11, 256(x12)\n"
			"fscwl.ps f24, f11 (x13)\n" 
			"fmvs.x.ps x10, f21, 4\n"
			"mulhsu x21, x14, x7\n"
			"sltiu x5, x0, 0x18\n"
			"fsgnjx.ps f29, f21, f10\n"
			"la x13, rand_int_32\n"
			"flw.ps f4, 992(x13)\n"
			"fslli.pi f10, f4, 0x8\n" 
			"fmvz.x.ps x10, f28, 5\n"
			"fnmsub.s f25, f16, f26, f21, rne\n"
			"frcp.ps f13, f28\n"
			"sllw x21, x10, x12\n"
			"li x5, 0x805d355004\n"
			"amoxorl.w x10, x28, (x5)\n" 
			"fcvt.pw.ps f29, f21, rdn\n"
			"fnmadd.s f1, f21, f23, f25, rtz\n"
			"fle.pi f21, f23, f25\n"
			"bge x29, x21, 1f\n"
			"1:\n"
			"fsgnj.s f25, f25, f23\n"
			"la x29, rand_ieee754_32\n"
			"flw.ps f24, 0(x29)\n"
			"fcvt.w.s x29, f24, rdn\n" 
			"divu x0, x10, x29\n"
			"fbci.ps f10, 0x2131a\n"
			"divu x13, x13, x28\n"
			"fmv.x.s x29, f10\n"
			"li x7, 0x804f78f00c\n"
			"amoandg.w x5, x10, (x7)\n" 
			"fmsub.s f1, f25, f1, f1, rmm\n"
			"fnmsub.ps f25, f1, f13, f29, rmm\n"
			"ffrc.ps f25, f4\n"
			"blt x16, x25, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f11, f10\n"
			"li x28, 0x9edd5d5a4898122f\n"
			"li x14, 0x82f4d73ee26827a6\n"
			"or x6, x28, x14\n" 
			"addi x29, x29, 0x7\n"
			"bltu x21, x10, 1f\n"
			"1:\n"
			"fltu.pi f1, f23, f23\n"
			"slli x14, x14, 3\n"
			"fnmsub.s f21, f21, f26, f7, rtz\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x40\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f10, 3(x31)\n"
			"flw.ps f11, 1(x31)\n"
			"flw.ps f13, 3(x31)\n"
			"flw.ps f16, 13(x31)\n"
			"flw.ps f17, 17(x31)\n"
			"flw.ps f21, 5(x31)\n"
			"flw.ps f23, 17(x31)\n"
			"flw.ps f25, 2(x31)\n"
			"flw.ps f26, 21(x31)\n"
			"flw.ps f28, 8(x31)\n"
			"flw.ps f29, 25(x31)\n"
			"flw.ps f4, 45(x31)\n"
			"flw.ps f7, 10(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x10, 0(x31)\n"
			"lw x13, 5(x31)\n"
			"lw x14, 4(x31)\n"
			"lw x21, 14(x31)\n"
			"lw x22, 18(x31)\n"
			"lw x28, 19(x31)\n"
			"lw x5, 19(x31)\n"
			"lw x6, 18(x31)\n"
			"lw x7, 1(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_1:\n"
			"fnot.pi f16, f10\n"
			"addiw x28, x6, 0x1f\n"
			"ebreak\n"
			"bge x5, x17, 1f\n"
			"1:\n"
			"maskpopc x12, m3\n"
			"bgeu x0, x30, 1f\n"
			"1:\n"
			"mul x13, x13, x7\n"
			"for.pi f10, f7, f10\n"
			"mul x10, x14, x13\n"
			"fsatu8.pi f11, f23\n"
			"fmsub.ps f21, f7, f10, f23, rup\n"
			"fmadd.ps f17, f23, f4, f17, rtz\n"
			"bltu x15, x24, 1f\n"
			"1:\n"
			"fmin.ps f23, f13, f23\n"
			"fpackrepb.pi f29, f29\n"
			"blt x16, x3, 1f\n"
			"1:\n"
			"fcvt.f16.ps f13, f21\n"
			"bne x30, x10, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f21, f21, rdn\n"
			"srl x0, x14, x13\n"
			"maskpopcz x5, m2\n"
			"masknot m6, m6\n"
			"sll x14, x6, x14\n"
			"fbci.pi f10, 0x41a0d\n"
			"flt.s x21, f23, f11\n"
			"fmulh.pi f11, f11, f26\n"
			"fsrl.pi f21, f13, f10\n"
			"bne x15, x20, 1f\n"
			"1:\n"
			"remu x28, x14, x14\n"
			"subw x13, x7, x22\n"
			"fadd.s f26, f7, f16, rne\n"
			"fltu.pi f7, f11, f1\n"
			"feq.ps f17, f26, f23\n"
			"for.pi f10, f16, f25\n"
			"fsub.s f21, f13, f11, rtz\n"
			"blt x24, x12, 1f\n"
			"1:\n"
			"srli x10, x5, 3\n"
			"fmadd.s f26, f11, f21, f21, rdn\n"
			"fsgnjn.ps f28, f23, f28\n"
			"mov.m.x m2, x0, 0x2c\n"
			"bgeu x12, x3, 1f\n"
			"1:\n"
			"maskand m6, m6, m2\n"
			"ffrc.ps f7, f29\n"
			"faddi.pi f1, f7, 0xb7\n"
			"fnmsub.s f17, f17, f23, f21, rdn\n"
			"sllw x14, x10, x5\n"
			"bltu x0, x0, 1f\n"
			"1:\n"
			"feq.pi f1, f11, f17\n"
			"bge x2, x4, 1f\n"
			"1:\n"
			"and x7, x28, x7\n"
			"fle.s x10, f29, f16\n"
			"fmin.ps f7, f7, f7\n"
			"masknot m1, m6\n"
			"div x22, x22, x6\n"
			"fsgnjn.ps f28, f28, f16\n"
			"fmin.pi f17, f11, f26\n"
			"fcvt.s.w f16, x21, rup\n"
			"sllw x10, x7, x22\n"
			"fmin.ps f16, f16, f4\n"
			"fsrai.pi f17, f28, 0x0\n"
			"bge x24, x3, 1f\n"
			"1:\n"
			"xor x29, x0, x6\n"
			"fltm.pi m6, f28, f26\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_1_LOOP_SEQ_HID_1\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_HID_1:\n"
			"li x29, 0x804fb36008\n"
			"la x10, mem_ptr_offset\n"
			"flw.ps f29, 288(x10)\n"
			"famominug.pi f11, f29 (x29)\n" 
			"fnmadd.ps f28, f7, f17, f28, rup\n"
			"faddi.pi f24, f29, 0x2\n"
			"faddi.pi f11, f10, 0x35\n"
			"bgeu x22, x10, 1f\n"
			"1:\n"
			"fsrai.pi f25, f29, 0x7\n"
			"ffrc.ps f1, f23\n"
			"for.pi f7, f7, f7\n"
			"fcvt.ps.f16 f10, f11\n"
			"li x28, 0x801f84fbd1\n"
			"sd x14, 256(x28)\n" 
			"remu x14, x14, x21\n"
			"fltu.pi f29, f16, f4\n"
			"fsetm.pi m2, f1\n"
			"fsra.pi f24, f13, f1\n"
			"fcvt.wu.s x7, f10, rmm\n"
			"bltu x1, x24, 1f\n"
			"1:\n"
			"fmin.ps f29, f29, f16\n"
			"fsub.pi f21, f10, f10\n"
			"li x14, 0x804f6b5008\n"
			"la x22, mem_ptr_offset\n"
			"flw.ps f1, 608(x22)\n"
			"famomaxug.pi f24, f1 (x14)\n" 
			"fcvt.pw.ps f21, f4, rne\n"
			"fmadd.s f11, f17, f24, f11, rtz\n"
			"fcvt.ps.pw f24, f8, rmm\n"
			"bne x12, x12, 1f\n"
			"1:\n"
			"fsub.pi f24, f23, f24\n"
			"fsgnjx.s f26, f26, f25\n"
			"beq x18, x30, 1f\n"
			"1:\n"
			"fcmov.ps f26, f10, f23, f1\n"
			"fsub.pi f11, f13, f11\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f25, 192(x10)\n"
			"flw.ps f24, 384(x10)\n"
			"fmin.ps f25, f25, f24\n" 
			"for.pi f13, f28, f4\n"
			"remu x12, x5, x13\n"
			"fcvt.ps.pwu f21, f29, rmm\n"
			"or x22, x22, x10\n"
			"beq x16, x9, 1f\n"
			"1:\n"
			"addi x6, x12, 0x1b\n"
			"fbci.pi f8, 0x26ef\n"
			"mulh x5, x14, x10\n"
			"fltm.ps m3, f10, f4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fcvt.f16.ps f4, f21\n"
			"slliw x14, x21, 4\n"
			"bgeu x2, x0, 1f\n"
			"1:\n"
			"xori x22, x13, 0x6\n"
			"fmul.pi f10, f13, f25\n"
			"fsll.pi f13, f13, f13\n"
			"fnmadd.ps f13, f13, f17, f17, rtz\n"
			"addw x6, x28, x0\n"
			"li x10, 0x801a44d1b0\n"
			"li x7, 2876215217\n"
			"fsc32b.ps f11, x6 (x10)\n" 
			"fsatu8.pi f16, f16\n"
			"fnmadd.ps f8, f13, f23, f8, rdn\n"
			"fcmov.ps f13, f13, f8, f13\n"
			"fadd.pi f1, f1, f23\n"
			"fadd.pi f16, f4, f23\n"
			"fcvt.ps.pw f24, f29, rmm\n"
			"fpackreph.pi f29, f26\n"
			"li x7, 0x801bb024e6\n"
			"sd x6, -128(x7)\n"
			"lb x10, -128(x7)\n" 
			"fcvt.ps.pwu f1, f16, rdn\n"
			"fadd.s f26, f4, f13, rdn\n"
			"feq.pi f4, f13, f28\n"
			"fsat8.pi f11, f24\n"
			"blt x14, x6, 1f\n"
			"1:\n"
			"fle.ps f1, f4, f29\n"
			"bge x0, x7, 1f\n"
			"1:\n"
			"fxor.pi f26, f17, f24\n"
			"fmadd.ps f26, f23, f26, f1, rtz\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f24, 512(x5)\n"
			"flw.ps f29, 480(x5)\n"
			"fmul.s f10, f24, f29, rne\n" 
			"divuw x10, x10, x0\n"
			"slli x29, x29, 3\n"
			"fmul.pi f10, f26, f26\n"
			"srliw x29, x29, 3\n"
			"beq x8, x18, 1f\n"
			"1:\n"
			"sub x10, x10, x22\n"
			"addw x6, x29, x29\n"
			"bgeu x6, x22, 1f\n"
			"1:\n"
			"fsgnjx.ps f4, f23, f4\n"
			"li x31, 0x80300062c0\n"
			"li x28, 0x8000000000000004\n"
			"add x28, x31, x28\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrwi tensor_wait, 6\n" 
			"fslli.pi f10, f23, 0x8\n"
			"faddi.pi f21, f7, 0xc0\n"
			"fsub.ps f16, f16, f26, rup\n"
			"fsgnj.ps f13, f7, f25\n"
			"fmul.ps f13, f26, f1, rne\n"
			"fmaxu.pi f21, f13, f29\n"
			"bge x30, x20, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f26, f26, rup\n"
			"bne x13, x6, 1f\n"
			"1:\n"
			"li x5, 0x804ca5b018\n"
			"la x10, mem_ptr_offset\n"
			"flw.ps f25, 832(x10)\n"
			"famomaxg.pi f24, f25 (x5)\n" 
			"fsgnjx.ps f1, f26, f29\n"
			"fmul.s f21, f21, f21, rmm\n"
			"feq.ps f26, f29, f26\n"
			"fmul.pi f7, f23, f23\n"
			"fnot.pi f24, f16\n"
			"fsatu8.pi f7, f26\n"
			"beq x22, x14, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f11, f16, rdn\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f25, 896(x13)\n"
			"flw.ps f10, 544(x13)\n"
			"flw.ps f23, 256(x13)\n"
			"fcmov.ps f7, f25, f10, f23\n" 
			"fpackrepb.pi f1, f17\n"
			"bgeu x14, x31, 1f\n"
			"1:\n"
			"fnot.pi f7, f10\n"
			"bne x2, x13, 1f\n"
			"1:\n"
			"fpackrepb.pi f26, f26\n"
			"fcvt.pwu.ps f1, f8, rmm\n"
			"bne x24, x6, 1f\n"
			"1:\n"
			"fcmov.ps f16, f29, f1, f28\n"
			"fxor.pi f11, f17, f11\n"
			"fltu.pi f7, f16, f4\n"
			"li x10, 0x8019a1dd58\n"
			"li x7, 1674537358\n"
			"fsc32h.ps f1, x21 (x10)\n" 
			"fsat8.pi f25, f29\n"
			"srai x21, x5, 2\n"
			"fslli.pi f11, f23, 0xf\n"
			"fsat8.pi f4, f21\n"
			"maskor m1, m2, m3\n"
			"beq x10, x15, 1f\n"
			"1:\n"
			"mova.x.m x0\n"
			"fsll.pi f13, f8, f17\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f10, 672(x6)\n"
			"flw.ps f29, 352(x6)\n"
			"fdiv.ps f1, f10, f29, rup\n" 
			"remuw x0, x12, x0\n"
			"for.pi f7, f28, f29\n"
			"blt x20, x6, 1f\n"
			"1:\n"
			"fsetm.pi m6, f26\n"
			"and x7, x0, x21\n"
			"fadd.pi f29, f8, f26\n"
			"mulw x13, x12, x14\n"
			"fsll.pi f1, f11, f4\n"
			"li x31, 0x8020004980\n"
			"li x21, 0x8000000000000004\n"
			"add x21, x31, x21\n"
			"li x31, (0x2 << 6) | 0x1\n"
			"csrwi tensor_wait, 6\n" 
			"fsrli.pi f24, f11, 0x6\n"
			"frcp.ps f1, f17\n"
			"fmax.pi f7, f7, f23\n"
			"fmax.s f17, f29, f26\n"
			"fcvt.pwu.ps f1, f24, rmm\n"
			"feq.pi f28, f23, f29\n"
			"fmadd.ps f4, f11, f28, f24, rup\n"
			"li x22, 0x804a6bd030\n"
			"la x29, mem_ptr_offset\n"
			"flw.ps f25, 192(x29)\n"
			"fschg.ps f10, f25 (x22)\n" 
			"xor x6, x29, x6\n"
			"div x28, x28, x29\n"
			"fsatu8.pi f16, f10\n"
			"fsat8.pi f10, f25\n"
			"fsrl.pi f24, f24, f29\n"
			"or x21, x21, x29\n"
			"feqm.ps m6, f29, f25\n"
			"li x29, 0x801aad0838\n"
			"li x5, 3560735045\n"
			"fsc32w.ps f1, x7 (x29)\n" 
			"remw x28, x13, x12\n"
			"fsat8.pi f1, f25\n"
			"maskpopcz x6, m2\n"
			"fsgnjx.ps f7, f1, f7\n"
			"maskand m1, m6, m2\n"
			"srl x29, x22, x29\n"
			"bgeu x18, x22, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f17, f29, rne\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x18\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f13, 4(x31)\n"
			"flw.ps f16, 0(x31)\n"
			"flw.ps f23, 10(x31)\n"
			"flw.ps f25, 15(x31)\n"
			"flw.ps f28, 19(x31)\n"
			"flw.ps f29, 1(x31)\n"
			"flw.ps f4, 7(x31)\n"
			"flw.ps f7, 28(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x12, 2(x31)\n"
			"lw x13, 0(x31)\n"
			"lw x14, 7(x31)\n"
			"lw x22, 13(x31)\n"
			"lw x28, 19(x31)\n"
			"lw x29, 1(x31)\n"
			"lw x5, 22(x31)\n"
			"lw x6, 15(x31)\n"
			"lw x7, 16(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_1:\n"
			"rem x7, x7, x13\n"
			"fadd.pi f25, f13, f16\n"
			"fxor.pi f16, f7, f16\n"
			"remu x22, x13, x6\n"
			"fmaxu.pi f1, f4, f10\n"
			"auipc x7, 0x1d\n"
			"bltu x15, x8, 1f\n"
			"1:\n"
			"fence\n"
			"fcvt.pwu.ps f25, f25, rdn\n"
			"srli x29, x22, 1\n"
			"andi x6, x29, 0x1b\n"
			"bne x30, x5, 1f\n"
			"1:\n"
			"fmin.ps f10, f28, f23\n"
			"sll x12, x14, x7\n"
			"sraiw x5, x14, 4\n"
			"feqm.ps m2, f25, f23\n"
			"fbci.pi f29, 0x7fe8d\n"
			"fmin.ps f7, f29, f7\n"
			"beq x9, x29, 1f\n"
			"1:\n"
			"fcvt.s.wu f25, x5, rdn\n"
			"mulw x21, x13, x13\n"
			"mov.m.x m1, x13, 0x6a\n"
			"bne x3, x10, 1f\n"
			"1:\n"
			"fmaxu.pi f1, f4, f7\n"
			"masknot m3, m2\n"
			"fbci.pi f8, 0x27d80\n"
			"fand.pi f29, f29, f10\n"
			"mulhsu x22, x7, x5\n"
			"fsra.pi f23, f28, f25\n"
			"fcmov.ps f23, f28, f16, f29\n"
			"maskor m2, m1, m3\n"
			"feq.pi f8, f23, f13\n"
			"fltm.pi m1, f25, f23\n"
			"xori x12, x12, 0xf\n"
			"mov.m.x m2, x12, 0x6c\n"
			"fmax.s f8, f29, f10\n"
			"flt.ps f13, f16, f23\n"
			"bltu x31, x22, 1f\n"
			"1:\n"
			"remu x10, x6, x14\n"
			"sltu x6, x5, x6\n"
			"fsra.pi f25, f13, f23\n"
			"fcvt.pwu.ps f28, f23, rup\n"
			"fmvs.x.ps x13, f23, 2\n"
			"fmax.ps f25, f25, f25\n"
			"remuw x14, x14, x14\n"
			"beq x22, x13, 1f\n"
			"1:\n"
			"fmax.s f13, f16, f4\n"
			"fsgnjx.s f10, f23, f4\n"
			"fexp.ps f28, f29\n"
			"sub x28, x14, x7\n"
			"sll x22, x22, x0\n"
			"fsra.pi f29, f29, f29\n"
			"slti x6, x29, 0x8\n"
			"beq x8, x0, 1f\n"
			"1:\n"
			"srai x7, x12, 4\n"
			"fsub.s f10, f10, f7, rne\n"
			"sltiu x28, x28, 0x12\n"
			"sub x0, x0, x14\n"
			"feq.ps f25, f28, f25\n"
			"fnot.pi f1, f16\n"
			"fcmov.ps f13, f23, f13, f16\n"
			"srlw x0, x5, x22\n"
			"maskor m2, m3, m2\n"
			"fandi.pi f16, f16, 0x1bf\n"
			"fxor.pi f1, f4, f13\n"
			"fnmadd.ps f23, f29, f25, f23, rne\n"
			"blt x14, x11, 1f\n"
			"1:\n"
			"fsgnj.ps f29, f4, f7\n"
			"fltm.ps m6, f23, f7\n"
			"fcmov.ps f29, f16, f16, f29\n"
			"fnmsub.ps f10, f4, f28, f23, rdn\n"
			"fsub.s f13, f13, f13, rup\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_2_LOOP_SEQ_HID_1\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f29,  608 (x5)\n"
			"flw.ps f11,  832 (x5)\n"
			"flw.ps f24,  288 (x5)\n"
			"flw.ps f15,  512 (x5)\n"
			"flw.ps f14,  160 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f16,  64 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f13,  256 (x5)\n"
			"flw.ps f1,  96 (x5)\n"
			"flw.ps f23,  352 (x5)\n"
			"flw.ps f0,  672 (x5)\n"
			"flw.ps f27,  32 (x5)\n"
			"flw.ps f2,  224 (x5)\n"
			"flw.ps f22,  896 (x5)\n"
			"flw.ps f25,  640 (x5)\n"
			"flw.ps f30,  416 (x5)\n"
			"flw.ps f26,  736 (x5)\n"
			"flw.ps f5,  544 (x5)\n"
			"flw.ps f3,  576 (x5)\n"
			"flw.ps f20,  864 (x5)\n"
			"flw.ps f19,  320 (x5)\n"
			"flw.ps f18,  0 (x5)\n"
			"flw.ps f17,  992 (x5)\n"
			"flw.ps f31,  128 (x5)\n"
			"flw.ps f6,  192 (x5)\n"
			"flw.ps f8,  800 (x5)\n"
			"flw.ps f7,  704 (x5)\n"
			"flw.ps f4,  928 (x5)\n"
			"flw.ps f9,  448 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f28,  384 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_3_HID_1:\n"
			"li x28, 0x804821b030\n"
			"amominug.d x22, x29, (x28)\n" 
			"fnmsub.ps f29, f26, f25, f11, rne\n"
			"fsra.pi f17, f26, f13\n"
			"frcp.ps f25, f8\n"
			"fslli.pi f11, f29, 0x2\n"
			"fmin.pi f16, f25, f8\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f26, 352(x5)\n"
			"fmvs.x.ps x14, f26, 6\n" 
			"fclass.ps f25, f29\n"
			"fcvt.pw.ps f7, f7, rup\n"
			"blt x8, x12, 1f\n"
			"1:\n"
			"feq.s x13, f4, f17\n"
			"fand.pi f17, f26, f17\n"
			"fpackreph.pi f7, f11\n"
			"csrr x0, marchid\n"
			"slliw x13, x13, 0\n"
			"fmadd.s f17, f16, f7, f29, rup\n"
			"fround.ps f1, f23, rtz\n"
			"xori x0, x0, 0x17\n"
			"fclass.s x5, f13\n"
			"li x13, 0x8048b5b038\n"
			"amoxorg.d x29, x13, (x13)\n" 
			"lui x12, 0x2\n"
			"bltu x30, x28, 1f\n"
			"1:\n"
			"fbci.ps f23, 0x7c30c\n"
			"mova.x.m x28\n"
			"ori x14, x5, 0x1e\n"
			"fround.ps f8, f16, rdn\n"
			"li x14, 0x804e39d020\n"
			"la x6, mem_ptr_offset\n"
			"flw.ps f29, 672(x6)\n"
			"famomaxug.pi f4, f29 (x14)\n" 
			"fle.pi f16, f29, f26\n"
			"fmax.pi f29, f16, f23\n"
			"fnmsub.ps f1, f17, f25, f17, rdn\n"
			"fadd.pi f1, f1, f4\n"
			"fcvt.ps.pwu f7, f23, rtz\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f28, 704(x5)\n"
			"fswizz.ps f26, f28, 0x6\n" 
			"fsll.pi f1, f4, f23\n"
			"fle.pi f29, f8, f21\n"
			"mulh x22, x10, x21\n"
			"bgeu x17, x28, 1f\n"
			"1:\n"
			"fand.pi f4, f7, f4\n"
			"fmsub.s f7, f7, f25, f8, rdn\n"
			"li x5, 0x804f4bb020\n"
			"la x12, mem_ptr_offset\n"
			"flw.ps f4, 160(x12)\n"
			"famoming.ps f23, f4 (x5)\n" 
			"addiw x28, x0, 0x13\n"
			"sub x22, x28, x28\n"
			"sraiw x13, x6, 4\n"
			"masknot m3, m3\n"
			"fadd.s f8, f21, f25, rne\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f11, 64(x12)\n"
			"fmv.x.s x14, f11\n" 
			"fcvt.pw.ps f1, f24, rmm\n"
			"blt x4, x10, 1f\n"
			"1:\n"
			"fsgnjn.ps f26, f28, f28\n"
			"fround.ps f23, f23, rtz\n"
			"fcvt.pwu.ps f24, f24, rtz\n"
			"beq x14, x2, 1f\n"
			"1:\n"
			"fmul.s f21, f21, f23, rtz\n"
			"li x6, 0x804c348028\n"
			"amoandg.d x13, x14, (x6)\n" 
			"addiw x0, x6, 0x9\n"
			"sra x5, x0, x28\n"
			"beq x1, x28, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f16, f25, rne\n"
			"addi x21, x21, 0x10\n"
			"ffrc.ps f4, f23\n"
			"li x12, 0x805c1cf030\n"
			"la x13, mem_ptr_offset\n"
			"flw.ps f16, 0(x13)\n"
			"famominl.ps f8, f16 (x12)\n" 
			"fsll.pi f1, f7, f4\n"
			"fexp.ps f16, f11\n"
			"beq x17, x0, 1f\n"
			"1:\n"
			"flog.ps f4, f16\n"
			"fadd.s f29, f28, f11, rne\n"
			"fadd.s f21, f7, f11, rmm\n"
			"la x7, rand_int_32\n"
			"flw.ps f17, 288(x7)\n"
			"fpackreph.pi f1, f17\n" 
			"fltm.pi m3, f8, f25\n"
			"bne x3, x25, 1f\n"
			"1:\n"
			"remw x22, x22, x12\n"
			"srliw x28, x0, 1\n"
			"ebreak\n"
			"divw x13, x14, x13\n"
			"li x6, 0x8059c9d038\n"
			"la x28, mem_ptr_offset\n"
			"flw.ps f24, 736(x28)\n"
			"famoandl.pi f28, f24 (x6)\n" 
			"fbci.ps f21, 0x50b05\n"
			"fsgnj.ps f10, f10, f16\n"
			"fbci.pi f16, 0x17a33\n"
			"beq x17, x14, 1f\n"
			"1:\n"
			"maskpopc x13, m2\n"
			"mova.x.m x6\n"
			"li x13, 0x805a1a0028\n"
			"la x21, mem_ptr_offset\n"
			"flw.ps f16, 672(x21)\n"
			"famomaxul.pi f29, f16 (x13)\n" 
			"fpackrepb.pi f13, f10\n"
			"frcp.ps f17, f24\n"
			"bne x10, x21, 1f\n"
			"1:\n"
			"fsgnjn.s f21, f8, f13\n"
			"fsub.pi f29, f25, f24\n"
			"fcvt.pwu.ps f24, f7, rup\n"
			"li x12, 0x804f190000\n"
			"amoorg.w x7, x5, (x12)\n" 
			"fcvt.ps.pw f1, f11, rdn\n"
			"fmsub.ps f17, f11, f10, f29, rne\n"
			"fsll.pi f16, f29, f16\n"
			"fmax.ps f16, f21, f16\n"
			"ffrc.ps f10, f26\n"
			"li x13, 0x805b26e038\n"
			"la x14, mem_ptr_offset\n"
			"flw.ps f8, 224(x14)\n"
			"fgbl.ps f29, f8 (x13)\n" 
			"ffrc.ps f29, f29\n"
			"frcp.ps f17, f17\n"
			"fminu.pi f29, f29, f11\n"
			"fmsub.ps f4, f4, f25, f17, rne\n"
			"ffrc.ps f11, f8\n"
			"li x7, 0x805abdf038\n"
			"la x22, mem_ptr_offset\n"
			"flw.ps f7, 288(x22)\n"
			"famoaddl.pi f24, f7 (x7)\n" 
			"feq.pi f13, f4, f13\n"
			"fsrai.pi f1, f1, 0xf\n"
			"fmax.pi f11, f1, f16\n"
			"fsll.pi f24, f24, f26\n"
			"bltu x5, x2, 1f\n"
			"1:\n"
			"fsub.s f13, f7, f16, rup\n"
			"bge x5, x18, 1f\n"
			"1:\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x80\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f11, 0(x31)\n"
			"flw.ps f13, 6(x31)\n"
			"flw.ps f21, 3(x31)\n"
			"flw.ps f23, 13(x31)\n"
			"flw.ps f24, 7(x31)\n"
			"flw.ps f25, 4(x31)\n"
			"flw.ps f26, 17(x31)\n"
			"flw.ps f28, 31(x31)\n"
			"flw.ps f29, 27(x31)\n"
			"flw.ps f4, 15(x31)\n"
			"flw.ps f7, 27(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x10, 0(x31)\n"
			"lw x12, 7(x31)\n"
			"lw x13, 11(x31)\n"
			"lw x14, 5(x31)\n"
			"lw x22, 5(x31)\n"
			"lw x28, 1(x31)\n"
			"lw x29, 24(x31)\n"
			"lw x5, 8(x31)\n"
			"lw x6, 24(x31)\n"
			"lw x7, 13(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_3_LOOP_SEQ_HID_1:\n"
			"fsub.s f26, f26, f24, rtz\n"
			"blt x5, x31, 1f\n"
			"1:\n"
			"fltm.ps m2, f28, f26\n"
			"fmv.s.x f24, x22\n"
			"fltm.pi m3, f4, f28\n"
			"add x10, x12, x28\n"
			"bltu x4, x6, 1f\n"
			"1:\n"
			"fmsub.s f26, f29, f4, f26, rne\n"
			"slti x22, x7, 0x1e\n"
			"faddi.pi f11, f23, 0x41\n"
			"fmax.ps f25, f13, f10\n"
			"bge x14, x30, 1f\n"
			"1:\n"
			"mulhsu x22, x12, x29\n"
			"fadd.ps f25, f26, f10, rup\n"
			"ebreak\n"
			"fmax.s f13, f11, f13\n"
			"feqm.ps m3, f29, f10\n"
			"srl x28, x5, x28\n"
			"fadd.pi f21, f21, f29\n"
			"bgeu x15, x9, 1f\n"
			"1:\n"
			"fsat8.pi f21, f21\n"
			"fsrli.pi f17, f11, 0x3\n"
			"xori x7, x5, 0x13\n"
			"masknot m1, m2\n"
			"srl x5, x0, x12\n"
			"divuw x22, x22, x22\n"
			"flt.pi f28, f26, f24\n"
			"beq x0, x31, 1f\n"
			"1:\n"
			"fsub.s f28, f25, f26, rtz\n"
			"fltu.pi f25, f24, f11\n"
			"bge x24, x7, 1f\n"
			"1:\n"
			"fsgnj.s f1, f25, f24\n"
			"fadd.ps f24, f13, f21, rmm\n"
			"fcvt.f16.ps f24, f7\n"
			"fnmsub.ps f17, f24, f10, f26, rtz\n"
			"beq x4, x14, 1f\n"
			"1:\n"
			"mova.m.x x22\n"
			"fmin.ps f11, f28, f23\n"
			"fsgnj.ps f17, f29, f28\n"
			"blt x19, x14, 1f\n"
			"1:\n"
			"sltiu x0, x12, 0x12\n"
			"fadd.pi f26, f24, f26\n"
			"srl x7, x0, x5\n"
			"fsrai.pi f17, f11, 0xb\n"
			"beq x12, x27, 1f\n"
			"1:\n"
			"faddi.pi f13, f13, 0x13\n"
			"fcvt.s.w f24, x28, rup\n"
			"sllw x29, x10, x0\n"
			"fcvt.s.wu f4, x7, rdn\n"
			"fcvt.f16.ps f8, f26\n"
			"remu x12, x28, x22\n"
			"beq x17, x0, 1f\n"
			"1:\n"
			"fsgnjn.s f29, f7, f7\n"
			"blt x21, x22, 1f\n"
			"1:\n"
			"slli x0, x0, 2\n"
			"bgeu x0, x3, 1f\n"
			"1:\n"
			"fminu.pi f29, f29, f23\n"
			"beq x4, x27, 1f\n"
			"1:\n"
			"fmsub.ps f13, f25, f24, f13, rup\n"
			"bgeu x16, x25, 1f\n"
			"1:\n"
			"fbci.pi f1, 0x151a7\n"
			"subw x28, x5, x0\n"
			"fadd.s f11, f26, f24, rne\n"
			"faddi.pi f10, f28, 0x177\n"
			"fswizz.ps f26, f4, 0xb\n"
			"fexp.ps f8, f26\n"
			"addiw x14, x14, 0x1a\n"
			"subw x6, x13, x12\n"
			"fsub.ps f23, f23, f24, rmm\n"
			"flt.pi f26, f24, f13\n"
			"sra x22, x29, x5\n"
			"fmin.pi f11, f13, f10\n"
			"fnmsub.s f25, f4, f11, f11, rne\n"
			"fexp.ps f26, f7\n"
			"fbcx.ps f11, x7\n"
			"blt x21, x9, 1f\n"
			"1:\n"
			"fsgnjn.s f21, f26, f4\n"
			"mova.m.x x28\n"
			"fmax.s f29, f26, f4\n"
			"sub x12, x6, x12\n"
			"fcmovm.ps f1, f7, f4\n"
			"feq.ps f17, f11, f21\n"
			"flog.ps f24, f24\n"
			"feq.pi f29, f26, f29\n"
			"xori x28, x10, 0x13\n"
			"srliw x14, x6, 5\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_3_LOOP_SEQ_HID_1\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_4_HID_1:\n"
			"li x10, 0x805ec96000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f25, (x10)\n" 
			"fmadd.ps f13, f13, f29, f8, rmm\n"
			"fmadd.s f25, f10, f25, f8, rtz\n"
			"blt x0, x22, 1f\n"
			"1:\n"
			"sllw x13, x13, x12\n"
			"fcvt.ps.f16 f24, f24\n"
			"blt x15, x8, 1f\n"
			"1:\n"
			"sll x14, x14, x22\n"
			"sub x7, x5, x5\n"
			"fround.ps f10, f26, rne\n"
			"fcvt.s.w f17, x6, rup\n"
			"li x21, 0x801e5348aa\n"
			"sb x13, -512(x21)\n" 
			"maskand m3, m6, m3\n"
			"slti x5, x28, 0xc\n"
			"fsrl.pi f17, f1, f10\n"
			"mova.m.x x7\n"
			"fmsub.s f11, f23, f11, f25, rtz\n"
			"srai x22, x12, 4\n"
			"ecall\n"
			"fcvt.ps.f16 f11, f28\n"
			"li x21, 0x801810449c\n"
			"sd x14, 512(x21)\n" 
			"faddi.pi f26, f4, 0x187\n"
			"fpackreph.pi f28, f17\n"
			"fmadd.ps f28, f7, f7, f24, rne\n"
			"bgeu x2, x11, 1f\n"
			"1:\n"
			"fsra.pi f16, f16, f16\n"
			"for.pi f13, f10, f10\n"
			"fmulhu.pi f21, f26, f28\n"
			"bgeu x31, x0, 1f\n"
			"1:\n"
			"fsll.pi f7, f7, f4\n"
			"fsgnjx.ps f23, f24, f13\n"
			"bne x29, x18, 1f\n"
			"1:\n"
			"li x14, 0x80192a5950\n"
			"fsw f25, 512(x14)\n" 
			"fcvt.f16.ps f11, f29\n"
			"fcvt.ps.pwu f8, f8, rne\n"
			"fmax.pi f8, f24, f24\n"
			"flt.pi f1, f28, f10\n"
			"fsatu8.pi f28, f4\n"
			"flog.ps f1, f28\n"
			"fltu.pi f13, f28, f29\n"
			"fmul.s f11, f7, f11, rne\n"
			"li x7, 0x8058c86028\n"
			"la x13, mem_ptr_offset\n"
			"flw.ps f8, 320(x13)\n"
			"famomaxl.ps f21, f8 (x7)\n" 
			"fcvt.pw.ps f21, f24, rne\n"
			"fmul.pi f21, f23, f23\n"
			"fsgnjx.s f25, f23, f25\n"
			"bltu x20, x26, 1f\n"
			"1:\n"
			"fclass.ps f25, f7\n"
			"fsatu8.pi f25, f24\n"
			"fmulh.pi f11, f1, f7\n"
			"fsgnjn.s f21, f1, f23\n"
			"blt x29, x5, 1f\n"
			"1:\n"
			"fmulh.pi f23, f25, f7\n"
			"bge x6, x5, 1f\n"
			"1:\n"
			"la x5, rand_int_32\n"
			"flw.ps f10, 160(x5)\n"
			"flw.ps f11, 704(x5)\n"
			"fdiv.pi f28, f10, f11\n" 
			"fltm.pi m1, f11, f13\n"
			"fmax.s f8, f13, f13\n"
			"feq.pi f1, f16, f13\n"
			"fsgnj.s f21, f16, f13\n"
			"fcvt.ps.f16 f13, f13\n"
			"fxor.pi f21, f16, f11\n"
			"divuw x22, x7, x22\n"
			"ecall\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f28, 960(x12)\n"
			"fswizz.ps f11, f28, 0x1\n" 
			"fsub.s f1, f1, f1, rmm\n"
			"fsrli.pi f13, f11, 0xe\n"
			"fsll.pi f28, f21, f28\n"
			"for.pi f11, f25, f21\n"
			"fcvt.pw.ps f26, f21, rtz\n"
			"fadd.s f10, f28, f28, rup\n"
			"fround.ps f13, f25, rne\n"
			"fsll.pi f26, f28, f28\n"
			"li x22, 0x80180d6200\n"
			"fsw f28, -256(x22)\n" 
			"fcvt.pwu.ps f26, f26, rdn\n"
			"feqm.ps m1, f7, f17\n"
			"feq.ps f23, f25, f23\n"
			"fmvz.x.ps x28, f7, 6\n"
			"fsat8.pi f29, f7\n"
			"beq x25, x5, 1f\n"
			"1:\n"
			"fsgnjx.ps f8, f11, f26\n"
			"slliw x12, x10, 1\n"
			"fltu.pi f7, f7, f17\n"
			"li x6, 0x805e017010\n"
			"amoaddl.d x21, x5, (x6)\n" 
			"fmax.s f26, f13, f26\n"
			"fsrai.pi f4, f4, 0x3\n"
			"fxor.pi f4, f13, f4\n"
			"fsrli.pi f25, f26, 0x7\n"
			"bltu x22, x28, 1f\n"
			"1:\n"
			"feq.pi f13, f1, f26\n"
			"fpackrepb.pi f16, f28\n"
			"blt x3, x11, 1f\n"
			"1:\n"
			"feq.pi f13, f11, f11\n"
			"fnmadd.ps f29, f11, f26, f4, rmm\n"
			"li x21, 0x801c880938\n"
			"la x5, mem_ptr_offset\n"
			"flw.ps f10, 896(x5)\n"
			"fgw.ps f4, f10 (x21)\n" 
			"fswizz.ps f21, f25, 0xd\n"
			"fsgnjx.s f10, f25, f8\n"
			"feq.ps f4, f11, f4\n"
			"fmul.s f11, f23, f21, rmm\n"
			"fnmadd.s f7, f16, f17, f13, rup\n"
			"fmax.pi f16, f1, f1\n"
			"fnmadd.ps f21, f7, f21, f29, rup\n"
			"fsrli.pi f26, f25, 0x5\n"
			"csrr x0, hpmcounter3\n"
			"fadd.ps f25, f26, f16, rup\n"
			"bne x10, x14, 1f\n"
			"1:\n"
			"ecall\n"
			"maskand m6, m3, m6\n"
			"fsub.s f24, f7, f29, rne\n"
			"add x13, x22, x6\n"
			"xor x29, x28, x0\n"
			"feqm.ps m6, f16, f7\n"
			"srai x13, x13, 5\n"
			"li x13, 0x801a40dda8\n"
			"li x14, 203622169\n"
			"fg32b.ps f25, x10 (x13)\n" 
			"fsgnj.s f13, f21, f24\n"
			"fsatu8.pi f4, f17\n"
			"fcvt.ps.pwu f29, f29, rne\n"
			"fcvt.ps.pw f4, f24, rmm\n"
			"fmin.s f26, f1, f23\n"
			"blt x2, x12, 1f\n"
			"1:\n"
			"fsra.pi f10, f11, f7\n"
			"fexp.ps f11, f8\n"
			"fround.ps f17, f26, rup\n"
			"li x21, 0x804a21403c\n"
			"amoming.w x13, x10, (x21)\n" 
			"fnmadd.ps f26, f11, f25, f11, rup\n"
			"blt x12, x12, 1f\n"
			"1:\n"
			"remu x28, x28, x28\n"
			"xor x21, x29, x29\n"
			"fpackrepb.pi f7, f11\n"
			"mova.x.m x28\n"
			"fsll.pi f26, f11, f17\n"
			"fclass.s x29, f11\n"
			"srli x13, x29, 0\n"
			"bgeu x9, x26, 1f\n"
			"1:\n"
			"li x5, 0x801d9e9d20\n"
			"li x28, 4130143578\n"
			"fg32w.ps f21, x14 (x5)\n" 
			"fsgnjn.ps f11, f29, f11\n"
			"flt.ps f21, f8, f21\n"
			"fcvt.ps.pw f24, f17, rne\n"
			"fcvt.pw.ps f24, f21, rdn\n"
			"fmulhu.pi f25, f8, f16\n"
			"bltu x22, x4, 1f\n"
			"1:\n"
			"fmulhu.pi f25, f17, f24\n"
			"bltu x23, x7, 1f\n"
			"1:\n"
			"fadd.pi f25, f29, f8\n"
			"fsll.pi f1, f10, f25\n"
			"li x21, 0x804e094004\n"
			"amoming.w x12, x13, (x21)\n" 
			"fmax.ps f29, f13, f10\n"
			"beq x31, x7, 1f\n"
			"1:\n"
			"fsgnjx.ps f29, f25, f25\n"
			"fsgnjn.ps f29, f13, f29\n"
			"fmadd.ps f25, f25, f10, f10, rne\n"
			"frcp.ps f23, f25\n"
			"fnot.pi f10, f25\n"
			"fslli.pi f25, f29, 0x9\n"
			"fsgnjx.ps f13, f13, f29\n"
			"li x6, 0x805b91d028\n"
			"amomaxul.d x12, x28, (x6)\n" 
			"masknot m3, m1\n"
			"fnmsub.s f21, f7, f11, f7, rdn\n"
			"bltu x19, x2, 1f\n"
			"1:\n"
			"fnmadd.ps f29, f7, f7, f29, rdn\n"
			"mova.x.m x22\n"
			"bge x31, x21, 1f\n"
			"1:\n"
			"fmul.pi f1, f29, f11\n"
			"fsub.s f21, f7, f7, rtz\n"
			"fmv.s.x f29, x13\n"
			"fmin.s f11, f29, f29\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x28\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f10, 2(x31)\n"
			"flw.ps f11, 4(x31)\n"
			"flw.ps f16, 1(x31)\n"
			"flw.ps f21, 7(x31)\n"
			"flw.ps f23, 13(x31)\n"
			"flw.ps f24, 14(x31)\n"
			"flw.ps f25, 20(x31)\n"
			"flw.ps f28, 23(x31)\n"
			"flw.ps f29, 35(x31)\n"
			"flw.ps f7, 33(x31)\n"
			"flw.ps f8, 23(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x10, 3(x31)\n"
			"lw x12, 2(x31)\n"
			"lw x13, 7(x31)\n"
			"lw x14, 4(x31)\n"
			"lw x21, 8(x31)\n"
			"lw x22, 4(x31)\n"
			"lw x28, 7(x31)\n"
			"lw x29, 24(x31)\n"
			"lw x5, 30(x31)\n"
			"lw x6, 39(x31)\n"
			"lw x7, 10(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_4_LOOP_SEQ_HID_1:\n"
			"lui x10, 0x1d\n"
			"fsgnjn.ps f24, f24, f7\n"
			"fcvt.ps.pw f7, f24, rtz\n"
			"fnmsub.s f11, f23, f28, f11, rtz\n"
			"subw x12, x10, x0\n"
			"fslli.pi f1, f11, 0x2\n"
			"fsgnj.s f24, f23, f29\n"
			"fbci.ps f28, 0x6bf90\n"
			"add x10, x10, x29\n"
			"fnmsub.s f4, f11, f16, f10, rup\n"
			"fltm.pi m6, f11, f10\n"
			"bne x21, x6, 1f\n"
			"1:\n"
			"fmv.x.s x7, f29\n"
			"fmadd.s f28, f28, f11, f28, rup\n"
			"mulh x12, x12, x29\n"
			"slt x21, x5, x21\n"
			"divw x0, x0, x12\n"
			"srai x5, x0, 2\n"
			"maskpopc x28, m3\n"
			"bne x23, x23, 1f\n"
			"1:\n"
			"fmulhu.pi f4, f7, f8\n"
			"fswizz.ps f11, f23, 0x0\n"
			"fmadd.s f24, f10, f25, f24, rtz\n"
			"bgeu x4, x21, 1f\n"
			"1:\n"
			"fcvt.s.w f28, x7, rdn\n"
			"bltu x20, x8, 1f\n"
			"1:\n"
			"fmv.x.s x5, f21\n"
			"andi x21, x13, 0x1f\n"
			"div x6, x12, x6\n"
			"flt.pi f24, f10, f28\n"
			"fpackrepb.pi f11, f24\n"
			"mulhu x12, x28, x22\n"
			"fsra.pi f13, f8, f25\n"
			"blt x28, x6, 1f\n"
			"1:\n"
			"fsub.ps f1, f16, f1, rmm\n"
			"srliw x22, x14, 1\n"
			"fmin.ps f10, f10, f21\n"
			"srliw x22, x13, 0\n"
			"blt x25, x2, 1f\n"
			"1:\n"
			"fround.ps f4, f24, rmm\n"
			"fadd.pi f24, f23, f1\n"
			"divuw x12, x7, x29\n"
			"blt x18, x25, 1f\n"
			"1:\n"
			"srlw x10, x10, x6\n"
			"maskpopc x7, m2\n"
			"fence\n"
			"fmax.s f16, f21, f7\n"
			"divuw x29, x7, x7\n"
			"sraw x5, x0, x10\n"
			"fsgnj.ps f1, f25, f25\n"
			"beq x20, x12, 1f\n"
			"1:\n"
			"xori x0, x12, 0x1a\n"
			"fsgnjx.ps f13, f1, f24\n"
			"blt x10, x26, 1f\n"
			"1:\n"
			"feq.ps f25, f24, f25\n"
			"fnmsub.ps f1, f28, f10, f25, rmm\n"
			"fcvt.pwu.ps f23, f21, rdn\n"
			"bgeu x1, x9, 1f\n"
			"1:\n"
			"sraw x14, x7, x22\n"
			"slt x12, x6, x13\n"
			"fbci.pi f24, 0x34d35\n"
			"rem x0, x14, x0\n"
			"fsgnj.s f24, f7, f28\n"
			"flt.s x28, f29, f16\n"
			"fmvs.x.ps x0, f25, 5\n"
			"fcvt.f16.ps f11, f25\n"
			"faddi.pi f28, f28, 0x171\n"
			"slliw x5, x21, 0\n"
			"xor x12, x12, x22\n"
			"fle.s x14, f10, f10\n"
			"fcvt.s.wu f28, x22, rne\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_4_LOOP_SEQ_HID_1\n"
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f30,  832 (x5)\n"
			"flw.ps f24,  288 (x5)\n"
			"flw.ps f15,  480 (x5)\n"
			"flw.ps f19,  544 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f6,  800 (x5)\n"
			"flw.ps f22,  384 (x5)\n"
			"flw.ps f26,  512 (x5)\n"
			"flw.ps f0,  608 (x5)\n"
			"flw.ps f5,  928 (x5)\n"
			"flw.ps f29,  576 (x5)\n"
			"flw.ps f17,  768 (x5)\n"
			"flw.ps f1,  896 (x5)\n"
			"flw.ps f21,  672 (x5)\n"
			"flw.ps f8,  704 (x5)\n"
			"flw.ps f10,  640 (x5)\n"
			"flw.ps f27,  64 (x5)\n"
			"flw.ps f13,  0 (x5)\n"
			"flw.ps f20,  160 (x5)\n"
			"flw.ps f14,  992 (x5)\n"
			"flw.ps f28,  736 (x5)\n"
			"flw.ps f31,  32 (x5)\n"
			"flw.ps f25,  864 (x5)\n"
			"flw.ps f4,  320 (x5)\n"
			"flw.ps f12,  256 (x5)\n"
			"flw.ps f11,  448 (x5)\n"
			"flw.ps f23,  96 (x5)\n"
			"flw.ps f16,  960 (x5)\n"
			"flw.ps f3,  224 (x5)\n"
			"flw.ps f2,  416 (x5)\n"
			"flw.ps f18,  192 (x5)\n"
			"flw.ps f9,  128 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_5_HID_1:\n"
			"li x5, 0x805cafe010\n"
			"amominul.d x12, x29, (x5)\n" 
			"li x13, 0x805810d030\n"
			"amoxorl.w x6, x13, (x13)\n" 
			"li x7, 0x804bc9a018\n"
			"amoming.d x28, x14, (x7)\n" 
			"li x13, 0x805cf8e000\n"
			"la x6, mem_ptr_offset\n"
			"flw.ps f13, 832(x6)\n"
			"famominl.ps f16, f13 (x13)\n" 
			"li x28, 0x801922c688\n"
			"li x29, 2598259955\n"
			"fg32h.ps f29, x5 (x28)\n" 
			"li x29, 0x805966b018\n"
			"la x7, mem_ptr_offset\n"
			"flw.ps f7, 768(x7)\n"
			"fgbl.ps f24, f7 (x29)\n" 
			"la x13, rand_int_32\n"
			"flw.ps f8, 480(x13)\n"
			"flw.ps f25, 480(x13)\n"
			"fminu.pi f4, f8, f25\n" 
			"li x21, 0x801b6c87db\n"
			"sd x5, 128(x21)\n"
			"lwu x5, 128(x21)\n" 
			"li x7, 0x801b16ac60\n"
			"li x14, 3272351885\n"
			"fsc32w.ps f1, x22 (x7)\n" 
			"csrr x0, hpmcounter7\n"
			"li x7, 0x804887c010\n"
			"amoxorg.d x29, x13, (x7)\n" 
			"li x13, 0x805b51d038\n"
			"amoandl.d x29, x7, (x13)\n" 
			"li x22, 0x801e1bc6fb\n"
			"sd x13, 256(x22)\n"
			"lb x28, 256(x22)\n" 
			"li x22, 0x804dcff030\n"
			"la x29, mem_ptr_offset\n"
			"flw.ps f29, 256(x29)\n"
			"famoming.pi f10, f29 (x22)\n" 
			"li x28, 0x805b2e8000\n"
			"la x22, mem_ptr_offset\n"
			"flw.ps f17, 512(x22)\n"
			"fschl.ps f10, f17 (x28)\n" 
			"li x13, 0x804ee9d018\n"
			"amoorg.d x6, x6, (x13)\n" 
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x60\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f10, 3(x31)\n"
			"flw.ps f11, 8(x31)\n"
			"flw.ps f21, 10(x31)\n"
			"flw.ps f24, 3(x31)\n"
			"flw.ps f28, 14(x31)\n"
			"flw.ps f4, 15(x31)\n"
			"flw.ps f7, 26(x31)\n"
			"flw.ps f8, 14(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x12, 1(x31)\n"
			"lw x13, 3(x31)\n"
			"lw x14, 5(x31)\n"
			"lw x21, 3(x31)\n"
			"lw x22, 10(x31)\n"
			"lw x28, 15(x31)\n"
			"lw x29, 23(x31)\n"
			"lw x5, 5(x31)\n"
			"lw x6, 28(x31)\n"
			"lw x7, 24(x31)\n"
			"li x31, 0x2\n"
			"LBL_SEQID_5_LOOP_SEQ_HID_1:\n"
			"ffrc.ps f28, f28\n"
			"bne x18, x6, 1f\n"
			"1:\n"
			"fmin.ps f10, f1, f21\n"
			"sraw x29, x6, x6\n"
			"srliw x10, x13, 0\n"
			"fexp.ps f29, f11\n"
			"bge x27, x8, 1f\n"
			"1:\n"
			"fround.ps f24, f21, rtz\n"
			"fslli.pi f21, f11, 0xf\n"
			"fbci.pi f10, 0x7d4f\n"
			"fcvt.ps.f16 f8, f8\n"
			"mova.x.m x7\n"
			"maskpopcz x14, m1\n"
			"bgeu x4, x11, 1f\n"
			"1:\n"
			"fadd.s f7, f7, f7, rup\n"
			"blt x11, x20, 1f\n"
			"1:\n"
			"divuw x7, x6, x0\n"
			"lui x10, 0x0\n"
			"mul x29, x29, x29\n"
			"feq.pi f21, f21, f7\n"
			"fnmadd.ps f23, f11, f7, f10, rne\n"
			"sll x6, x29, x7\n"
			"fminu.pi f24, f7, f8\n"
			"fadd.pi f1, f28, f4\n"
			"fmin.ps f7, f28, f8\n"
			"fsll.pi f23, f7, f21\n"
			"fround.ps f16, f8, rmm\n"
			"ebreak\n"
			"beq x29, x0, 1f\n"
			"1:\n"
			"mulh x29, x14, x21\n"
			"for.pi f11, f11, f1\n"
			"sll x13, x13, x6\n"
			"sllw x29, x28, x29\n"
			"fcmovm.ps f10, f10, f7\n"
			"bne x13, x3, 1f\n"
			"1:\n"
			"sub x5, x12, x7\n"
			"beq x20, x24, 1f\n"
			"1:\n"
			"fcvt.s.wu f21, x12, rmm\n"
			"fpackreph.pi f28, f10\n"
			"lui x13, 0xa\n"
			"bgeu x27, x4, 1f\n"
			"1:\n"
			"remuw x22, x22, x6\n"
			"bne x18, x3, 1f\n"
			"1:\n"
			"sra x0, x21, x0\n"
			"fmax.ps f4, f24, f11\n"
			"fmin.pi f8, f8, f28\n"
			"fbci.ps f28, 0x289b0\n"
			"beq x19, x9, 1f\n"
			"1:\n"
			"flt.ps f1, f4, f11\n"
			"bltu x17, x10, 1f\n"
			"1:\n"
			"fexp.ps f4, f8\n"
			"bltu x4, x31, 1f\n"
			"1:\n"
			"frcp.ps f8, f11\n"
			"packb x28, x28, x28\n"
			"mulh x5, x7, x5\n"
			"auipc x12, 0x5\n"
			"beq x6, x2, 1f\n"
			"1:\n"
			"fmax.ps f25, f10, f24\n"
			"fmsub.ps f24, f8, f28, f24, rmm\n"
			"fcmov.ps f7, f11, f7, f28\n"
			"flem.ps m1, f7, f10\n"
			"slli x22, x22, 5\n"
			"or x29, x21, x6\n"
			"fclass.ps f25, f11\n"
			"addi x5, x7, 0x19\n"
			"fsgnj.ps f23, f28, f21\n"
			"fmsub.s f8, f28, f8, f8, rne\n"
			"fsrli.pi f24, f10, 0x4\n"
			"sll x7, x7, x6\n"
			"bgeu x5, x1, 1f\n"
			"1:\n"
			"fmv.s.x f7, x7\n"
			"frcp.ps f8, f7\n"
			"fsgnj.ps f21, f1, f11\n"
			"bge x16, x1, 1f\n"
			"1:\n"
			"sllw x14, x12, x6\n"
			"bgeu x31, x0, 1f\n"
			"1:\n"
			"fmax.ps f7, f21, f7\n"
			"mul x5, x5, x29\n"
			"maskand m1, m1, m1\n"
			"auipc x6, 0x13\n"
			"fsra.pi f29, f1, f11\n"
			"fmsub.s f4, f4, f10, f4, rmm\n"
			"blt x31, x6, 1f\n"
			"1:\n"
			"xori x5, x28, 0xd\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_5_LOOP_SEQ_HID_1\n"
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_6_HID_1:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f11, 992(x22)\n"
			"flw.ps f8, 480(x22)\n"
			"fsgnjx.ps f24, f11, f8\n" 
			"fltu.pi f25, f29, f23\n"
			"fmaxu.pi f25, f23, f23\n"
			"fle.pi f23, f29, f25\n"
			"fsgnjx.ps f25, f29, f29\n"
			"fcvt.ps.f16 f17, f23\n"
			"la x7, rand_int_32\n"
			"flw.ps f17, 512(x7)\n"
			"fcvt.ps.pw f1, f17, rtz\n" 
			"fsrl.pi f23, f23, f8\n"
			"fence\n"
			"fexp.ps f1, f8\n"
			"fcvt.w.s x22, f23, rtz\n"
			"fmul.pi f8, f13, f8\n"
			"li x14, 0x801918d198\n"
			"fsw.ps f10, -128(x14)\n"
			"flw.ps f1, -128(x14)\n" 
			"fsat8.pi f23, f1\n"
			"beq x31, x8, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f23, f23, rne\n"
			"feq.ps f1, f26, f26\n"
			"beq x27, x16, 1f\n"
			"1:\n"
			"fexp.ps f11, f23\n"
			"fmax.ps f23, f1, f26\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f13, 640(x22)\n"
			"flw.ps f11, 896(x22)\n"
			"fle.s x14, f13, f11\n" 
			"and x28, x28, x29\n"
			"fcvt.wu.s x6, f28, rne\n"
			"fcvt.ps.f16 f25, f24\n"
			"fbci.pi f4, 0x8b2f\n"
			"fsgnjx.s f16, f29, f16\n"
			"li x5, 0x8018577cb8\n"
			"li x21, 2857328195\n"
			"fg32b.ps f28, x22 (x5)\n" 
			"slti x22, x28, 0x1\n"
			"fmax.s f16, f4, f25\n"
			"bgeu x2, x6, 1f\n"
			"1:\n"
			"andi x13, x28, 0x19\n"
			"bltu x21, x3, 1f\n"
			"1:\n"
			"fle.ps f26, f29, f24\n"
			"fslli.pi f29, f4, 0x0\n"
			"li x28, 0x804df3a030\n"
			"amoswapg.d x21, x7, (x28)\n" 
			"divuw x14, x5, x28\n"
			"mulhsu x5, x5, x12\n"
			"masknot m3, m1\n"
			"fmsub.s f26, f26, f28, f26, rdn\n"
			"masknot m1, m1\n"
			"beq x6, x14, 1f\n"
			"1:\n"
			"li x28, 0x804d93601c\n"
			"amoxorg.w x13, x28, (x28)\n" 
			"fmin.pi f28, f28, f4\n"
			"fsub.s f24, f26, f26, rdn\n"
			"feq.ps f23, f29, f11\n"
			"bge x26, x5, 1f\n"
			"1:\n"
			"fsatu8.pi f8, f16\n"
			"beq x17, x26, 1f\n"
			"1:\n"
			"fcvt.f16.ps f11, f24\n"
			"li x10, 0x4a879af576be6a75\n"
			"andi x29, x10, 0x1a\n" 
			"divuw x21, x21, x6\n"
			"beq x22, x26, 1f\n"
			"1:\n"
			"ori x12, x12, 0x5\n"
			"fmulh.pi f8, f8, f21\n"
			"bge x24, x6, 1f\n"
			"1:\n"
			"fsra.pi f16, f8, f13\n"
			"fcvt.s.w f10, x10, rmm\n"
			"li x10, 0x801e396488\n"
			"fsw.ps f1, -512(x10)\n" 
			"divw x10, x6, x28\n"
			"lui x10, 0x0\n"
			"divw x7, x22, x7\n"
			"masknot m3, m6\n"
			"fsgnj.ps f29, f28, f28\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f1, 160(x5)\n"
			"flw.ps f7, 640(x5)\n"
			"flt.ps f4, f1, f7\n" 
			"fmaxu.pi f1, f7, f17\n"
			"flt.pi f10, f25, f7\n"
			"feq.ps f4, f29, f24\n"
			"fsub.pi f17, f10, f26\n"
			"fsrl.pi f4, f25, f23\n"
			"li x12, 0x274cd7cc58b41dd2\n"
			"slliw x12, x12, 3\n" 
			"for.pi f23, f10, f23\n"
			"bne x22, x4, 1f\n"
			"1:\n"
			"fpackrepb.pi f1, f10\n"
			"fmin.pi f8, f8, f4\n"
			"fcvt.ps.f16 f4, f23\n"
			"fcmov.ps f1, f13, f8, f1\n"
			"blt x15, x30, 1f\n"
			"1:\n"
			"li x7, 0x801e07bea0\n"
			"la x10, mem_ptr_offset\n"
			"flw.ps f25, 352(x10)\n"
			"fgb.ps f13, f25 (x7)\n" 
			"fsub.ps f7, f16, f26, rne\n"
			"bgeu x21, x10, 1f\n"
			"1:\n"
			"fslli.pi f8, f28, 0xc\n"
			"remw x21, x6, x7\n"
			"fadd.pi f8, f8, f13\n"
			"fle.pi f8, f26, f13\n"
			"bge x10, x0, 1f\n"
			"1:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f24, 416(x6)\n"
			"flw.ps f13, 736(x6)\n"
			"flw.ps f28, 544(x6)\n"
			"fnmadd.ps f7, f24, f13, f28, rup\n" 
			"fmulhu.pi f17, f7, f4\n"
			"bge x20, x14, 1f\n"
			"1:\n"
			"srlw x6, x29, x6\n"
			"rem x28, x22, x7\n"
			"sub x6, x6, x22\n"
			"fswizz.ps f4, f4, 0x0\n"
			"blt x5, x1, 1f\n"
			"1:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 288(x5)\n"
			"flw.ps f13, 480(x5)\n"
			"fnmsub.s f8, f10, f13, f13, rmm\n" 
			"fsgnjx.s f13, f16, f13\n"
			"fmul.s f7, f28, f7, rup\n"
			"fsatu8.pi f13, f17\n"
			"bne x4, x28, 1f\n"
			"1:\n"
			"fsrai.pi f7, f7, 0x5\n"
			"fclass.ps f16, f26\n"
			"li x31, 0x8020004980\n"
			"li x13, 0xa\n"
			"add x13, x31, x13\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrwi tensor_wait, 6\n" 
			"csrwi tensor_wait, 5\n" 
			"fpackrepb.pi f10, f23\n"
			"bge x17, x1, 1f\n"
			"1:\n"
			"fcvt.pw.ps f24, f16, rmm\n"
			"fsra.pi f29, f16, f29\n"
			"fsgnjn.ps f21, f13, f4\n"
			"fnmadd.ps f21, f10, f7, f28, rne\n"
			"li x7, 0x801857566b\n"
			"sd x21, -512(x7)\n"
			"lw x14, -512(x7)\n" 
			"fsrli.pi f17, f13, 0xf\n"
			"beq x1, x12, 1f\n"
			"1:\n"
			"maskpopc x22, m2\n"
			"fand.pi f24, f4, f28\n"
			"fclass.s x29, f8\n"
			"packb x0, x29, x0\n"
			"bgeu x23, x19, 1f\n"
			"1:\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x38\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f28, 0(x31)\n"
			"flw.ps f4, 0(x31)\n"
			"flw.ps f7, 1(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x29, 0(x31)\n"
			"lw x5, 0(x31)\n"
			"lw x6, 0(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_6_LOOP_SEQ_HID_1:\n"
			"flem.ps m3, f4, f4\n"
			"srai x13, x6, 0\n"
			"fsub.s f28, f28, f28, rdn\n"
			"fle.s x12, f4, f4\n"
			"blt x11, x7, 1f\n"
			"1:\n"
			"frcp.ps f7, f7\n"
			"blt x21, x15, 1f\n"
			"1:\n"
			"addi x6, x29, 0x15\n"
			"bgeu x28, x9, 1f\n"
			"1:\n"
			"sll x29, x29, x6\n"
			"bgeu x9, x7, 1f\n"
			"1:\n"
			"sraiw x22, x6, 3\n"
			"sraw x13, x5, x29\n"
			"feq.pi f4, f4, f28\n"
			"bgeu x26, x26, 1f\n"
			"1:\n"
			"feq.pi f1, f7, f7\n"
			"fcvt.w.s x22, f28, rdn\n"
			"fclass.ps f4, f4\n"
			"fmv.s.x f26, x5\n"
			"packb x6, x6, x6\n"
			"bltu x21, x1, 1f\n"
			"1:\n"
			"sll x6, x6, x5\n"
			"addi x22, x5, 0x3\n"
			"slli x29, x6, 0\n"
			"fnmsub.ps f7, f28, f28, f7, rtz\n"
			"fcvt.ps.pwu f17, f28, rne\n"
			"fltu.pi f4, f4, f4\n"
			"divu x10, x29, x29\n"
			"blt x19, x15, 1f\n"
			"1:\n"
			"addi x6, x29, 0x1e\n"
			"bge x31, x24, 1f\n"
			"1:\n"
			"for.pi f7, f4, f7\n"
			"fle.pi f4, f7, f28\n"
			"blt x14, x5, 1f\n"
			"1:\n"
			"remu x10, x29, x6\n"
			"xor x5, x6, x5\n"
			"feq.pi f4, f7, f4\n"
			"slt x6, x6, x5\n"
			"bne x4, x3, 1f\n"
			"1:\n"
			"mulhu x22, x5, x5\n"
			"sltu x12, x5, x6\n"
			"mulh x10, x5, x29\n"
			"sltiu x22, x6, 0x18\n"
			"bge x11, x21, 1f\n"
			"1:\n"
			"fmax.pi f10, f28, f4\n"
			"or x10, x29, x6\n"
			"beq x24, x1, 1f\n"
			"1:\n"
			"sltu x29, x6, x29\n"
			"fadd.pi f28, f28, f7\n"
			"fminu.pi f1, f7, f4\n"
			"bne x11, x12, 1f\n"
			"1:\n"
			"fcmov.ps f7, f4, f28, f7\n"
			"fclass.s x5, f4\n"
			"addi x5, x6, 0x15\n"
			"bne x3, x10, 1f\n"
			"1:\n"
			"fmax.s f1, f28, f4\n"
			"maskpopcz x5, m2\n"
			"sltiu x6, x29, 0x10\n"
			"sltu x6, x29, x6\n"
			"mulh x6, x6, x29\n"
			"flog.ps f17, f4\n"
			"bne x0, x21, 1f\n"
			"1:\n"
			"xori x13, x29, 0x11\n"
			"fcvt.s.w f11, x29, rtz\n"
			"fmulh.pi f4, f7, f7\n"
			"bne x21, x24, 1f\n"
			"1:\n"
			"mulhsu x29, x29, x5\n"
			"bltu x3, x22, 1f\n"
			"1:\n"
			"sltu x29, x6, x5\n"
			"frcp.ps f16, f4\n"
			"beq x9, x18, 1f\n"
			"1:\n"
			"fmin.ps f10, f7, f28\n"
			"fmaxu.pi f16, f28, f28\n"
			"fcvt.pw.ps f4, f28, rdn\n"
			"bltu x31, x22, 1f\n"
			"1:\n"
			"fle.s x6, f28, f28\n"
			"xor x5, x5, x5\n"
			"auipc x6, 0x10\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_6_LOOP_SEQ_HID_1\n"
			"li x5, 7\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f25,  640 (x5)\n"
			"flw.ps f29,  896 (x5)\n"
			"flw.ps f2,  288 (x5)\n"
			"flw.ps f9,  64 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
			"flw.ps f23,  96 (x5)\n"
			"flw.ps f0,  576 (x5)\n"
			"flw.ps f3,  352 (x5)\n"
			"flw.ps f7,  544 (x5)\n"
			"flw.ps f22,  448 (x5)\n"
			"flw.ps f19,  704 (x5)\n"
			"flw.ps f14,  512 (x5)\n"
			"flw.ps f1,  864 (x5)\n"
			"flw.ps f31,  672 (x5)\n"
			"flw.ps f26,  480 (x5)\n"
			"flw.ps f15,  800 (x5)\n"
			"flw.ps f6,  416 (x5)\n"
			"flw.ps f11,  928 (x5)\n"
			"flw.ps f18,  32 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f12,  768 (x5)\n"
			"flw.ps f24,  0 (x5)\n"
			"flw.ps f30,  320 (x5)\n"
			"flw.ps f27,  224 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f16,  384 (x5)\n"
			"flw.ps f20,  160 (x5)\n"
			"flw.ps f8,  128 (x5)\n"
			"flw.ps f13,  960 (x5)\n"
			"flw.ps f10,  256 (x5)\n"
			"flw.ps f21,  736 (x5)\n"
			"flw.ps f17,  608 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_7_HID_1:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f13, 96(x6)\n"
			"flw.ps f28, 160(x6)\n"
			"flt.s x6, f13, f28\n" 
			"sraw x6, x7, x12\n"
			"fsra.pi f8, f29, f16\n"
			"fexp.ps f16, f29\n"
			"rem x5, x5, x12\n"
			"bltu x1, x28, 1f\n"
			"1:\n"
			"li x28, 0x8058674008\n"
			"amoxorl.w x5, x14, (x28)\n" 
			"ffrc.ps f16, f16\n"
			"fsub.ps f13, f13, f25, rdn\n"
			"fcvt.pw.ps f7, f13, rmm\n"
			"frcp.ps f28, f16\n"
			"li x7, 0x805a3ce010\n"
			"la x10, mem_ptr_offset\n"
			"flw.ps f16, 352(x10)\n"
			"fschl.ps f26, f16 (x7)\n" 
			"fmsub.ps f29, f28, f21, f25, rmm\n"
			"fcvt.ps.f16 f8, f4\n"
			"frcp.ps f29, f23\n"
			"bltu x21, x9, 1f\n"
			"1:\n"
			"flt.pi f8, f21, f28\n"
			"li x10, 0x801b2fb2cb\n"
			"sd x29, -512(x10)\n"
			"lbu x28, -512(x10)\n" 
			"maskxor m1, m1, m1\n"
			"bltu x6, x13, 1f\n"
			"1:\n"
			"fround.ps f23, f7, rmm\n"
			"mov.m.x m1, x10, 0x0\n"
			"fle.ps f11, f23, f13\n"
			"li x29, 0x804c58b020\n"
			"amoaddg.d x5, x21, (x29)\n" 
			"fsgnj.s f1, f4, f25\n"
			"fnot.pi f26, f4\n"
			"fclass.ps f23, f11\n"
			"fclass.ps f1, f8\n"
			"blt x28, x8, 1f\n"
			"1:\n"
			"li x5, 0x805cc3e028\n"
			"la x6, mem_ptr_offset\n"
			"flw.ps f17, 192(x6)\n"
			"famomaxl.pi f11, f17 (x5)\n" 
			"fmulhu.pi f23, f24, f29\n"
			"flt.ps f24, f8, f7\n"
			"fround.ps f16, f4, rdn\n"
			"bge x24, x16, 1f\n"
			"1:\n"
			"fcvt.pw.ps f4, f23, rtz\n"
			"li x29, 0x8049351008\n"
			"amomaxg.d x29, x28, (x29)\n" 
			"fnmsub.s f21, f21, f16, f21, rne\n"
			"feq.ps f17, f7, f17\n"
			"bne x2, x8, 1f\n"
			"1:\n"
			"fandi.pi f13, f13, 0x14c\n"
			"fsgnjx.s f13, f17, f13\n"
			"li x14, 0x80181d5a60\n"
			"la x13, mem_ptr_offset\n"
			"flw.ps f4, 992(x13)\n"
			"fscb.ps f4, f4 (x14)\n" 
			"fnmsub.ps f28, f17, f11, f7, rne\n"
			"fsatu8.pi f25, f4\n"
			"fmul.s f28, f1, f1, rtz\n"
			"fnmsub.s f24, f11, f17, f10, rmm\n"
			"li x6, 0x805830d018\n"
			"la x28, mem_ptr_offset\n"
			"flw.ps f26, 32(x28)\n"
			"famominul.pi f8, f26 (x6)\n" 
			"remu x22, x0, x22\n"
			"bltu x17, x5, 1f\n"
			"1:\n"
			"fcvt.w.s x5, f13, rup\n"
			"fmin.ps f17, f23, f28\n"
			"bne x16, x10, 1f\n"
			"1:\n"
			"mul x0, x12, x0\n"
			"blt x14, x8, 1f\n"
			"1:\n"
			"li x14, 0x8059df8028\n"
			"amoorl.w x22, x22, (x14)\n" 
			"fle.pi f21, f21, f7\n"
			"fmul.pi f29, f21, f21\n"
			"fcvt.ps.pwu f10, f11, rtz\n"
			"fmulh.pi f16, f8, f4\n"
			"feqm.ps m6, f13, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fsrl.pi f11, f8, f29\n"
			"fsrl.pi f29, f25, f7\n"
			"fmin.ps f8, f29, f8\n"
			"bne x6, x26, 1f\n"
			"1:\n"
			"fmadd.s f26, f26, f26, f26, rtz\n"
			"li x22, 0x805bc72028\n"
			"la x21, mem_ptr_offset\n"
			"flw.ps f8, 960(x21)\n"
			"famominul.pi f1, f8 (x22)\n" 
			"fswizz.ps f28, f8, 0xf\n"
			"for.pi f21, f21, f26\n"
			"fsrai.pi f4, f4, 0x1\n"
			"bgeu x1, x27, 1f\n"
			"1:\n"
			"fsub.ps f23, f26, f29, rtz\n"
			"la x21, rand_int_32\n"
			"flw.ps f13, 800(x21)\n"
			"fnot.pi f16, f13\n" 
			"mulw x28, x5, x22\n"
			"fnot.pi f4, f24\n"
			"fmulhu.pi f16, f4, f16\n"
			"fadd.pi f21, f21, f16\n"
			"li x21, 0x804df52000\n"
			"amoandg.d x10, x6, (x21)\n" 
			"fle.pi f23, f23, f16\n"
			"fandi.pi f21, f25, 0x5\n"
			"fsra.pi f21, f7, f25\n"
			"fmax.s f24, f16, f25\n"
			"li x28, 0xef69d747eb245f7f\n"
			"xori x5, x28, 0x1a\n" 
			"fround.ps f24, f10, rtz\n"
			"bltu x20, x18, 1f\n"
			"1:\n"
			"fsatu8.pi f17, f7\n"
			"for.pi f28, f28, f8\n"
			"add x5, x7, x5\n"
			"li x6, 0x801bf65270\n"
			"fsw.ps f4, 256(x6)\n"
			"flw f17, 256(x6)\n" 
			"mulw x22, x28, x22\n"
			"fbcx.ps f1, x7\n"
			"fcvt.ps.f16 f28, f23\n"
			"bge x6, x2, 1f\n"
			"1:\n"
			"fnmsub.s f16, f8, f16, f21, rup\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x100\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f13, 2(x31)\n"
			"flw.ps f23, 3(x31)\n"
			"flw.ps f25, 1(x31)\n"
			"flw.ps f26, 12(x31)\n"
			"flw.ps f4, 10(x31)\n"
			"flw.ps f7, 8(x31)\n"
			"flw.ps f8, 24(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x14, 0(x31)\n"
			"lw x29, 4(x31)\n"
			"lw x6, 4(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_7_LOOP_SEQ_HID_1:\n"
			"sltu x6, x14, x29\n"
			"fcvt.pwu.ps f1, f26, rdn\n"
			"beq x4, x18, 1f\n"
			"1:\n"
			"andi x6, x29, 0x4\n"
			"fexp.ps f7, f25\n"
			"fsub.s f13, f23, f23, rtz\n"
			"fcvt.s.wu f7, x14, rmm\n"
			"fcvt.s.wu f13, x29, rne\n"
			"fmin.pi f7, f4, f25\n"
			"feq.ps f1, f4, f23\n"
			"mova.m.x x14\n"
			"fmulh.pi f1, f10, f25\n"
			"bge x14, x26, 1f\n"
			"1:\n"
			"fsgnjx.ps f25, f4, f25\n"
			"fsrl.pi f13, f13, f26\n"
			"blt x21, x26, 1f\n"
			"1:\n"
			"ffrc.ps f8, f7\n"
			"bge x27, x7, 1f\n"
			"1:\n"
			"fnmadd.ps f1, f7, f10, f7, rmm\n"
			"fsgnjx.ps f4, f4, f7\n"
			"fmax.pi f13, f7, f25\n"
			"bltu x27, x3, 1f\n"
			"1:\n"
			"sra x14, x14, x14\n"
			"fle.pi f7, f26, f7\n"
			"ffrc.ps f1, f26\n"
			"sltu x14, x14, x14\n"
			"beq x26, x1, 1f\n"
			"1:\n"
			"slti x10, x29, 0x1\n"
			"bltu x28, x26, 1f\n"
			"1:\n"
			"packb x14, x6, x14\n"
			"fmsub.ps f1, f4, f10, f8, rdn\n"
			"fminu.pi f1, f13, f25\n"
			"mov.m.x m2, x29, 0x6e\n"
			"fminu.pi f7, f23, f10\n"
			"fcmov.ps f7, f8, f8, f7\n"
			"bne x9, x20, 1f\n"
			"1:\n"
			"fnot.pi f1, f23\n"
			"fsgnjn.ps f13, f8, f23\n"
			"divuw x6, x6, x14\n"
			"for.pi f8, f8, f23\n"
			"mulh x7, x29, x6\n"
			"fltm.ps m6, f4, f10\n"
			"fcvt.f16.ps f1, f26\n"
			"fsra.pi f1, f7, f4\n"
			"div x7, x6, x14\n"
			"sub x10, x6, x14\n"
			"fadd.ps f1, f25, f23, rup\n"
			"bge x5, x3, 1f\n"
			"1:\n"
			"fcvt.s.w f7, x14, rup\n"
			"fmul.pi f23, f26, f23\n"
			"fle.ps f7, f7, f8\n"
			"fmax.pi f8, f8, f4\n"
			"auipc x10, 0x1e\n"
			"for.pi f1, f13, f7\n"
			"sraw x6, x29, x14\n"
			"fltm.ps m2, f8, f4\n"
			"remu x14, x6, x6\n"
			"subw x6, x6, x6\n"
			"fsub.ps f1, f8, f4, rmm\n"
			"sltu x7, x6, x6\n"
			"fmul.pi f7, f7, f10\n"
			"fbci.ps f1, 0x4327c\n"
			"div x7, x6, x6\n"
			"flog.ps f25, f25\n"
			"blt x25, x15, 1f\n"
			"1:\n"
			"mulw x10, x6, x29\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_7_LOOP_SEQ_HID_1\n"
			"li x5, 8\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"la x31, check_sum_hid_1\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"LBL_POSTAMBLE_HID_1:\n"          
			"csrwi   tensor_wait, 0x0\n"
			"csrwi   tensor_wait, 0x1\n"
			"csrwi   tensor_wait, 0x2\n"
			"csrwi   tensor_wait, 0x3\n"
			"csrwi   tensor_wait, 0x4\n"
			"csrwi   tensor_wait, 0x5\n"
			"csrwi   tensor_wait, 0x6\n"
			"csrwi   tensor_wait, 0x7\n"
			"csrwi   tensor_wait, 0x8\n"
			"csrwi   tensor_wait, 0x9\n"
			"csrwi   tensor_wait, 0xa\n"
			"fence\n"
			"LBL_SYNC_POINT_1_HID_1:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_1_WAIT_HID_1\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_1_EXIT_HID_1\n"
			"LBL_SYNC_POINT_1_WAIT_HID_1:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_1_EXIT_HID_1:\n"          
			"nop\n"
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_1\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_1:\n"          
			".global LBL_FAIL_HID_1\n"          
			"la x31, check_sum_hid_1\n"
			"li x5, 0xDEADDEADDEADDEAD\n"
			"sd x5, 24(x31)\n"
			"LBL_HPM_3_CORE_DCACHE_ACCESS1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_ACCESS0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_MISSES1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_MCYCLES_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_INTER_CORE_MSG_TX_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ETL_RSP_FROM_SC_UC_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter8\n"
			"sd x5, 72 (x31)\n"
			"slti x0, x0, 0x7ff\n"
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_1\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_1:\n"          
			".global LBL_PASS_HID_1\n"          
			"la x31, check_sum_hid_1\n"
			"li x5, 0xACEDACEDACEDACED\n"
			"sd x5, 24(x31)\n"
			"LBL_HPM_3_CORE_DCACHE_ACCESS1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_ACCESS0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_MISSES1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_MCYCLES_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_INTER_CORE_MSG_TX_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ETL_RSP_FROM_SC_UC_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter8\n"
			"sd x5, 72 (x31)\n"
			"slti x0, x0, 0x7fe\n"
			VSNIP_RSV
		);
		c_test_pass();
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_1:\n"          
			".global LBL_HALT_HID_1\n"          
			"wfi\n"
			"j LBL_HALT_HID_1\n"          
			VSNIP_RSV
		);
		return 0;       
	};
	if ( hid == 2 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_2:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_2\n"
			"csrw mscratch, x5\n"
			"csrwi menable_shadows, 0xf\n"
			"li x5, -1\n"
			"csrw mcounteren, x5\n"
			"csrw scounteren, x5\n"
			"csrwi mhpmcounter3, 0\n"
			"csrwi mhpmcounter4, 0\n"
			"csrwi mhpmcounter5, 0\n"
			"csrwi mhpmcounter6, 0\n"
			"csrwi mhpmcounter7, 0\n"
			"csrwi mhpmcounter8, 0\n"
			"csrwi mhpmevent3, 21\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 28\n"   
			"csrwi mhpmevent6, 20\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 6\n"   
			"mov.m.x m0, x0, 0xff\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f1,  288 (x5)\n"
			"flw.ps f26,  832 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f19,  224 (x5)\n"
			"flw.ps f8,  896 (x5)\n"
			"flw.ps f6,  320 (x5)\n"
			"flw.ps f21,  192 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f16,  256 (x5)\n"
			"flw.ps f20,  928 (x5)\n"
			"flw.ps f22,  64 (x5)\n"
			"flw.ps f7,  672 (x5)\n"
			"flw.ps f17,  992 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f3,  128 (x5)\n"
			"flw.ps f28,  448 (x5)\n"
			"flw.ps f30,  640 (x5)\n"
			"flw.ps f4,  800 (x5)\n"
			"flw.ps f23,  352 (x5)\n"
			"flw.ps f14,  768 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f18,  544 (x5)\n"
			"flw.ps f27,  736 (x5)\n"
			"flw.ps f15,  0 (x5)\n"
			"flw.ps f5,  608 (x5)\n"
			"flw.ps f29,  384 (x5)\n"
			"flw.ps f31,  160 (x5)\n"
			"flw.ps f9,  32 (x5)\n"
			"flw.ps f13,  480 (x5)\n"
			"flw.ps f11,  864 (x5)\n"
			"flw.ps f2,  96 (x5)\n"
			"flw.ps f24,  576 (x5)\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_HID_2:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_0_WAIT_HID_2\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_0_EXIT_HID_2\n"
			"LBL_SYNC_POINT_0_WAIT_HID_2:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_EXIT_HID_2:\n"          
			"nop\n"
			"LBL_KERNEL_HID_2:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_2\n"
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_HID_2:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f11, 256(x11)\n"
			"flw.ps f4, 416(x11)\n"
			"flw.ps f31, 320(x11)\n"
			"fnmsub.ps f4, f11, f4, f31, rdn\n" 
			"div x11, x15, x18\n"
			"bgeu x31, x24, 1f\n"
			"1:\n"
			"fmul.ps f16, f16, f6, rne\n"
			"fsetm.pi m1, f6\n"
			"mulhsu x5, x18, x5\n"
			"blt x11, x14, 1f\n"
			"1:\n"
			"fmin.ps f27, f12, f23\n"
			"bge x12, x29, 1f\n"
			"1:\n"
			"fcvt.wu.s x15, f12, rdn\n"
			"fcvt.pwu.ps f14, f16, rmm\n"
			"ori x18, x18, 0xd\n"
			"fmvs.x.ps x18, f16, 5\n"
			"li x18, 0x8053aee010\n"
			"la x6, mem_ptr_offset\n"
			"flw.ps f20, 0(x6)\n"
			"famoxorg.pi f13, f20 (x18)\n" 
			"fbci.ps f18, 0x11e3a\n"
			"bltu x2, x11, 1f\n"
			"1:\n"
			"fmin.pi f27, f11, f13\n"
			"fmadd.s f16, f6, f16, f18, rne\n"
			"fsub.ps f12, f12, f11, rne\n"
			"srli x27, x9, 3\n"
			"fbcx.ps f17, x5\n"
			"fmsub.s f17, f18, f11, f14, rdn\n"
			"fsub.s f12, f17, f12, rdn\n"
			"mulhsu x27, x7, x27\n"
			"li x6, 0x8061ad9000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f17, (x6)\n" 
			"fmadd.ps f12, f17, f4, f12, rdn\n"
			"fsat8.pi f22, f14\n"
			"fadd.ps f4, f4, f13, rup\n"
			"fcvt.ps.f16 f13, f13\n"
			"fsrl.pi f13, f14, f12\n"
			"fsrl.pi f22, f13, f17\n"
			"fmulh.pi f22, f13, f13\n"
			"flt.ps f18, f12, f12\n"
			"fmax.s f13, f13, f14\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f17, 512(x18)\n"
			"flw.ps f6, 288(x18)\n"
			"feq.s x6, f17, f6\n" 
			"fmin.pi f16, f18, f22\n"
			"fcvt.f16.ps f26, f31\n"
			"fbcx.ps f26, x27\n"
			"flt.s x29, f18, f31\n"
			"fbci.pi f6, 0x441df\n"
			"fmul.s f13, f16, f23, rtz\n"
			"fminu.pi f16, f16, f6\n"
			"xori x29, x29, 0x17\n"
			"bgeu x30, x30, 1f\n"
			"1:\n"
			"fcvt.s.wu f16, x9, rdn\n"
			"bltu x17, x7, 1f\n"
			"1:\n"
			"li x17, 0x802403e588\n"
			"fsw.ps f14, 256(x17)\n"
			"flq2 f22, 256(x17)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fcmov.ps f2, f20, f4, f4\n"
			"ffrc.ps f4, f26\n"
			"blt x15, x1, 1f\n"
			"1:\n"
			"feq.ps f23, f16, f18\n"
			"fcvt.f16.ps f12, f12\n"
			"fsll.pi f4, f26, f20\n"
			"fcvt.pw.ps f18, f6, rup\n"
			"fltu.pi f12, f11, f16\n"
			"fle.ps f12, f12, f11\n"
			"fnmadd.s f17, f20, f20, f11, rup\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f13, 192(x9)\n"
			"flw.ps f12, 128(x9)\n"
			"flw.ps f20, 320(x9)\n"
			"fmsub.ps f18, f13, f12, f20, rne\n" 
			"mova.x.m x25\n"
			"bne x14, x21, 1f\n"
			"1:\n"
			"subw x6, x9, x6\n"
			"fle.pi f13, f23, f26\n"
			"fsrl.pi f14, f26, f17\n"
			"fsgnj.s f14, f23, f4\n"
			"mulhu x25, x25, x25\n"
			"fmax.ps f26, f13, f23\n"
			"fmv.x.s x0, f23\n"
			"fltu.pi f4, f2, f4\n"
			"li x27, 0x806554f038\n"
			"la x25, mem_ptr_offset\n"
			"flw.ps f18, 608(x25)\n"
			"famomaxl.ps f22, f18 (x27)\n" 
			"fle.ps f6, f13, f12\n"
			"fle.ps f31, f17, f26\n"
			"fmaxu.pi f20, f27, f13\n"
			"fsub.pi f22, f31, f22\n"
			"fsatu8.pi f31, f6\n"
			"bgeu x23, x20, 1f\n"
			"1:\n"
			"fmul.pi f31, f4, f18\n"
			"fmax.ps f12, f31, f12\n"
			"fadd.ps f18, f23, f2, rmm\n"
			"beq x15, x1, 1f\n"
			"1:\n"
			"fsub.ps f18, f27, f18, rup\n"
			"li x5, 0x8054875024\n"
			"amominug.w x15, x9, (x5)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"addiw x9, x5, 0x2\n"
			"fsgnjx.ps f16, f2, f18\n"
			"fnmsub.s f27, f14, f18, f6, rup\n"
			"bltu x1, x22, 1f\n"
			"1:\n"
			"fmsub.s f27, f18, f27, f27, rne\n"
			"sub x15, x15, x18\n"
			"fsrli.pi f26, f2, 0xb\n"
			"sltiu x27, x5, 0xa\n"
			"bne x4, x20, 1f\n"
			"1:\n"
			"fand.pi f14, f22, f14\n"
			"fmul.s f4, f18, f14, rmm\n"
			"li x11, 0x445a0c5abbe01fa3\n"
			"li x6, 0xf59b868b2f8c886d\n"
			"srlw x11, x11, x6\n" 
			"fmsub.ps f12, f11, f4, f23, rne\n"
			"flt.pi f22, f22, f4\n"
			"bltu x10, x13, 1f\n"
			"1:\n"
			"fmax.ps f20, f20, f22\n"
			"fexp.ps f16, f14\n"
			"fcvt.ps.pw f12, f12, rdn\n"
			"fmsub.ps f6, f4, f23, f2, rtz\n"
			"fcvt.ps.pw f20, f23, rtz\n"
			"fltu.pi f2, f31, f31\n"
			"fsatu8.pi f17, f17\n"
			"bne x6, x25, 1f\n"
			"1:\n"
			"li x25, 0x8054e59038\n"
			"la x18, mem_ptr_offset\n"
			"flw.ps f23, 256(x18)\n"
			"famoming.pi f13, f23 (x25)\n" 
			"li x5, 0xcee0\n"
			"csrw tensor_mask, x5\n"	
			"fpackreph.pi f2, f31\n"
			"fmax.pi f16, f4, f11\n"
			"blt x4, x15, 1f\n"
			"1:\n"
			"fand.pi f31, f31, f11\n"
			"sll x15, x6, x18\n"
			"mulh x0, x17, x29\n"
			"beq x1, x3, 1f\n"
			"1:\n"
			"fcvt.s.w f31, x6, rup\n"
			"slt x15, x11, x25\n"
			"sub x15, x6, x18\n"
			"andi x6, x6, 0x1f\n"
			"li x29, 0x8024b04983\n"
			"sh x18, -512(x29)\n" 
			"fmv.x.s x17, f11\n"
			"bgeu x16, x3, 1f\n"
			"1:\n"
			"sub x18, x6, x18\n"
			"fsll.pi f27, f14, f22\n"
			"fbci.ps f13, 0x4d690\n"
			"bge x24, x9, 1f\n"
			"1:\n"
			"fsatu8.pi f13, f27\n"
			"fsrl.pi f11, f27, f13\n"
			"fclass.s x5, f26\n"
			"blt x15, x28, 1f\n"
			"1:\n"
			"divuw x0, x11, x27\n"
			"ffrc.ps f17, f13\n"
			"blt x12, x24, 1f\n"
			"1:\n"
			"li x9, 0x80641bc038\n"
			"amomaxl.d x11, x11, (x9)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fround.ps f26, f26, rmm\n"
			"bgeu x23, x23, 1f\n"
			"1:\n"
			"fclass.ps f16, f16\n"
			"fsatu8.pi f27, f14\n"
			"fnmsub.s f2, f27, f26, f2, rup\n"
			"feq.ps f31, f27, f2\n"
			"flog.ps f2, f4\n"
			"flt.ps f20, f14, f20\n"
			"bgeu x11, x15, 1f\n"
			"1:\n"
			"flt.ps f6, f27, f22\n"
			"flt.ps f16, f14, f2\n"
			"bltu x20, x9, 1f\n"
			"1:\n"
			"li x6, 0xbdee84a62386d42a\n"
			"srliw x11, x6, 4\n" 
			"fadd.ps f17, f2, f26, rmm\n"
			"fround.ps f23, f16, rdn\n"
			"fadd.s f26, f26, f2, rdn\n"
			"fsrli.pi f31, f12, 0x9\n"
			"fadd.pi f27, f20, f11\n"
			"fsrl.pi f4, f18, f16\n"
			"fsgnjx.s f4, f20, f18\n"
			"fcvt.pwu.ps f2, f13, rtz\n"
			"flt.pi f20, f23, f11\n"
			"li x15, 0x80250e71f8\n"
			"fsw.ps f20, -256(x15)\n" 
			"slliw x0, x11, 3\n"
			"sraw x9, x17, x15\n"
			"beq x21, x7, 1f\n"
			"1:\n"
			"fsatu8.pi f18, f13\n"
			"fmax.ps f16, f12, f12\n"
			"fltu.pi f4, f4, f27\n"
			"fpackreph.pi f27, f4\n"
			"fle.pi f12, f2, f12\n"
			"beq x2, x22, 1f\n"
			"1:\n"
			"ebreak\n"
			"sll x27, x29, x27\n"
			"li x11, 0x806452c028\n"
			"la x27, mem_ptr_offset\n"
			"flw.ps f22, 256(x27)\n"
			"famoorl.pi f11, f22 (x11)\n" 
			"fandi.pi f22, f22, 0x1eb\n"
			"fadd.s f26, f6, f22, rmm\n"
			"blt x1, x18, 1f\n"
			"1:\n"
			"fmax.pi f22, f22, f13\n"
			"fadd.ps f6, f26, f6, rup\n"
			"fsub.s f17, f6, f17, rup\n"
			"faddi.pi f22, f22, 0x198\n"
			"flog.ps f13, f26\n"
			"bgeu x19, x9, 1f\n"
			"1:\n"
			"fmadd.ps f26, f26, f22, f6, rtz\n"
			"for.pi f12, f13, f22\n"
			"la x17, rand_int_32\n"
			"flw.ps f2, 768(x17)\n"
			"fsatu8.pi f22, f2\n" 
			"fsgnj.ps f13, f14, f27\n"
			"fsub.pi f18, f18, f18\n"
			"bne x24, x13, 1f\n"
			"1:\n"
			"srl x6, x5, x29\n"
			"blt x29, x21, 1f\n"
			"1:\n"
			"fcvt.f16.ps f27, f31\n"
			"fandi.pi f14, f27, 0x143\n"
			"fadd.s f22, f14, f14, rtz\n"
			"srl x9, x5, x5\n"
			"fmax.ps f13, f11, f22\n"
			"fsgnj.ps f22, f22, f11\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_0_TLD_ID_0_NMASK_0x1_CMASK_0xfb_SHIRE_0_HID_2:\n"
			"li x31, 0x1fb1e\n"
			"csrw tensor_coop, x31\n" 
			"li x31, 0x01FF000001FF0000\n"
			"csrw tensor_mask, x31\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"li x6, 0xd440008020006b28\n"
			"csrw tensor_load, x6\n" 
			"csrwi tensor_wait, 1\n" 
			"LBL_SYNC_POINT_1_HID_2:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_1_WAIT_HID_2\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_1_EXIT_HID_2\n"
			"LBL_SYNC_POINT_1_WAIT_HID_2:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_1_EXIT_HID_2:\n"          
			"nop\n"
			"LBL_SYNC_POINT_2_HID_2:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_2_WAIT_HID_2\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_2_EXIT_HID_2\n"
			"LBL_SYNC_POINT_2_WAIT_HID_2:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_2_EXIT_HID_2:\n"          
			"nop\n"
			"LBL_SEQID_0_TST_ID_0_QUAD128_START_CORE_0_SHIRE_0_HID_2:\n"
			"li x31, (0xc << 4)\n"
			"li x6, 0x841e008020006b10\n"
			"csrw tensor_store, x6\n" 
			"csrwi tensor_wait, 8\n" 
			"LBL_SYNC_POINT_3_HID_2:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_3_WAIT_HID_2\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_3_EXIT_HID_2\n"
			"LBL_SYNC_POINT_3_WAIT_HID_2:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_3_EXIT_HID_2:\n"          
			"nop\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, 0x308000802000838f\n"
			"li x6, 0x1000802000098d\n"
			"li x7, 0x1fe80000091b047\n"
			"li x9, 0x17f9a00000000439\n"
			"li x10, 0x421008020004880\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"li x31, (0x1 << 6)\n"
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 3\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f11, 0(x31)\n"
			"flw.ps f12, 3(x31)\n"
			"flw.ps f13, 6(x31)\n"
			"flw.ps f14, 12(x31)\n"
			"flw.ps f16, 4(x31)\n"
			"flw.ps f17, 8(x31)\n"
			"flw.ps f18, 15(x31)\n"
			"flw.ps f2, 23(x31)\n"
			"flw.ps f20, 29(x31)\n"
			"flw.ps f23, 23(x31)\n"
			"flw.ps f26, 39(x31)\n"
			"flw.ps f27, 11(x31)\n"
			"flw.ps f4, 13(x31)\n"
			"flw.ps f6, 16(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x11, 4(x31)\n"
			"lw x15, 1(x31)\n"
			"lw x17, 7(x31)\n"
			"lw x18, 10(x31)\n"
			"lw x25, 11(x31)\n"
			"lw x27, 11(x31)\n"
			"lw x29, 9(x31)\n"
			"lw x5, 5(x31)\n"
			"lw x6, 2(x31)\n"
			"lw x9, 25(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_2:\n"
			"fround.ps f26, f14, rup\n"
			"bltu x5, x6, 1f\n"
			"1:\n"
			"fmvs.x.ps x29, f12, 5\n"
			"fcvt.pwu.ps f4, f23, rup\n"
			"feq.pi f14, f13, f23\n"
			"fslli.pi f2, f26, 0xb\n"
			"bne x15, x25, 1f\n"
			"1:\n"
			"fpackreph.pi f31, f27\n"
			"fcvt.ps.pwu f14, f16, rmm\n"
			"fround.ps f2, f20, rdn\n"
			"maskpopcz x17, m1\n"
			"fmvs.x.ps x6, f2, 4\n"
			"slti x18, x11, 0xb\n"
			"sltiu x0, x0, 0x13\n"
			"fexp.ps f27, f4\n"
			"fmv.x.s x29, f6\n"
			"addiw x25, x0, 0x17\n"
			"masknot m0, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"sltu x15, x0, x6\n"
			"divuw x18, x5, x15\n"
			"mulhsu x15, x0, x11\n"
			"fsgnj.ps f22, f13, f13\n"
			"fclass.ps f16, f13\n"
			"mulhsu x27, x18, x25\n"
			"bltu x5, x8, 1f\n"
			"1:\n"
			"fcvt.f16.ps f11, f14\n"
			"blt x3, x16, 1f\n"
			"1:\n"
			"divuw x15, x9, x6\n"
			"sra x9, x27, x18\n"
			"lui x29, 0x1f\n"
			"faddi.pi f12, f26, 0xec\n"
			"feq.s x7, f23, f13\n"
			"sltu x17, x5, x11\n"
			"bgeu x2, x0, 1f\n"
			"1:\n"
			"fsgnjx.ps f11, f6, f26\n"
			"fsrai.pi f13, f13, 0x5\n"
			"flog.ps f22, f12\n"
			"div x15, x18, x15\n"
			"andi x29, x29, 0x13\n"
			"fmv.x.s x29, f26\n"
			"fmax.pi f17, f14, f12\n"
			"mulw x29, x29, x5\n"
			"fclass.s x29, f17\n"
			"blt x20, x22, 1f\n"
			"1:\n"
			"fminu.pi f18, f13, f6\n"
			"fcvt.ps.pw f31, f16, rne\n"
			"fclass.s x9, f18\n"
			"fmin.pi f23, f11, f14\n"
			"fcvt.pwu.ps f11, f11, rtz\n"
			"sraiw x7, x9, 4\n"
			"bge x23, x6, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f16, f12\n"
			"fmulh.pi f26, f2, f4\n"
			"ori x25, x15, 0xe\n"
			"packb x6, x17, x25\n"
			"fmax.ps f27, f20, f14\n"
			"frcp.ps f23, f17\n"
			"fnmadd.s f2, f26, f17, f2, rdn\n"
			"feq.ps f11, f4, f27\n"
			"fmv.x.s x6, f20\n"
			"sltu x17, x15, x11\n"
			"flem.ps m5, f4, f12\n"
			"fmin.ps f12, f20, f4\n"
			"fsgnjn.s f2, f2, f6\n"
			"for.pi f2, f13, f4\n"
			"fmul.s f11, f13, f11, rdn\n"
			"blt x14, x28, 1f\n"
			"1:\n"
			"mov.m.x m1, x5, 0x48\n"
			"fcvt.wu.s x29, f20, rup\n"
			"fmvs.x.ps x0, f17, 2\n"
			"slliw x29, x0, 3\n"
			"mulh x11, x17, x15\n"
			"or x27, x27, x5\n"
			"sraw x7, x15, x15\n"
			"fcvt.pw.ps f12, f11, rne\n"
			"sub x29, x29, x29\n"
			"remu x15, x27, x25\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_0_LOOP_SEQ_HID_2\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_HID_2:\n"
			"csrr x0, instret\n"
			"flem.ps m6, f6, f4\n"
			"flem.ps m1, f22, f4\n"
			"xor x29, x7, x18\n"
			"feq.s x29, f22, f20\n"
			"fsub.pi f26, f23, f20\n"
			"srl x5, x7, x7\n"
			"mulh x9, x7, x9\n"
			"fsgnjn.s f17, f6, f6\n"
			"fsgnj.s f26, f22, f18\n"
			"csrr x0, fcc\n"
			"flt.s x0, f20, f14\n"
			"fcmov.ps f4, f4, f16, f26\n"
			"lui x7, 0x8\n"
			"fpackrepb.pi f18, f26\n"
			"bne x12, x6, 1f\n"
			"1:\n"
			"srai x0, x25, 1\n"
			"fsatu8.pi f12, f12\n"
			"lui x18, 0x16\n"
			"fmadd.s f18, f20, f16, f18, rne\n"
			"bltu x14, x26, 1f\n"
			"1:\n"
			"maskxor m5, m6, m6\n"
			"li x25, 0xc6467665f4d76cf2\n"
			"ori x9, x25, 0x18\n" 
			"divw x29, x7, x25\n"
			"fsatu8.pi f6, f23\n"
			"fsra.pi f23, f16, f22\n"
			"fsgnjx.ps f6, f23, f12\n"
			"flog.ps f2, f17\n"
			"divu x11, x25, x29\n"
			"fltm.pi m6, f23, f23\n"
			"mova.m.x x18\n"
			"blt x23, x31, 1f\n"
			"1:\n"
			"faddi.pi f27, f6, 0x1e6\n"
			"li x11, 0x806292b030\n"
			"la x17, mem_ptr_offset\n"
			"flw.ps f13, 736(x17)\n"
			"famoaddl.pi f16, f13 (x11)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fcvt.wu.s x25, f2, rne\n"
			"beq x27, x25, 1f\n"
			"1:\n"
			"fmulh.pi f17, f11, f18\n"
			"fsgnjx.ps f23, f20, f17\n"
			"srliw x25, x11, 2\n"
			"slliw x9, x25, 1\n"
			"fcvt.ps.pwu f18, f4, rtz\n"
			"fmvz.x.ps x6, f12, 1\n"
			"fmin.pi f27, f27, f18\n"
			"bgeu x6, x0, 1f\n"
			"1:\n"
			"srai x29, x9, 2\n"
			"li x18, 0x805380b008\n"
			"la x5, mem_ptr_offset\n"
			"flw.ps f17, 32(x5)\n"
			"famoorg.pi f14, f17 (x18)\n" 
			"addi x7, x0, 0x0\n"
			"beq x9, x1, 1f\n"
			"1:\n"
			"fmsub.ps f17, f2, f17, f17, rdn\n"
			"fcvt.wu.s x11, f2, rne\n"
			"remuw x6, x27, x0\n"
			"bge x29, x31, 1f\n"
			"1:\n"
			"fnmadd.s f2, f20, f20, f20, rmm\n"
			"addw x27, x27, x0\n"
			"fcvt.pw.ps f14, f17, rdn\n"
			"faddi.pi f2, f20, 0x140\n"
			"fadd.ps f17, f2, f17, rdn\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 320(x25)\n"
			"fcvt.pw.ps f22, f26, rdn\n" 
			"li x5, 0x4111\n"
			"csrw tensor_mask, x5\n"	
			"fcvt.pwu.ps f16, f27, rne\n"
			"fmax.s f18, f23, f26\n"
			"fmadd.s f18, f4, f4, f11, rtz\n"
			"bltu x0, x23, 1f\n"
			"1:\n"
			"faddi.pi f20, f23, 0x1fd\n"
			"fmul.ps f18, f4, f26, rdn\n"
			"bltu x6, x18, 1f\n"
			"1:\n"
			"for.pi f6, f16, f11\n"
			"bltu x18, x27, 1f\n"
			"1:\n"
			"fsrl.pi f16, f16, f16\n"
			"fmin.pi f23, f12, f23\n"
			"fslli.pi f16, f11, 0x2\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f14, 480(x27)\n"
			"flw.ps f4, 224(x27)\n"
			"flw.ps f6, 480(x27)\n"
			"fmadd.ps f11, f14, f4, f6, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fsat8.pi f20, f16\n"
			"fmsub.s f4, f16, f20, f16, rmm\n"
			"fsll.pi f6, f16, f6\n"
			"fle.ps f20, f6, f6\n"
			"bne x30, x12, 1f\n"
			"1:\n"
			"feq.ps f20, f6, f6\n"
			"fcmov.ps f16, f16, f16, f16\n"
			"faddi.pi f6, f16, 0xe6\n"
			"fmul.pi f4, f16, f16\n"
			"fmin.pi f20, f20, f20\n"
			"li x6, 0x8054964000\n"
			"la x29, mem_ptr_offset\n"
			"flw.ps f6, 896(x29)\n"
			"famoxorg.pi f4, f6 (x6)\n" 
			"li x5, 0xec60\n"
			"csrw tensor_mask, x5\n"	
			"fsll.pi f20, f20, f14\n"
			"fsub.pi f14, f20, f14\n"
			"beq x22, x11, 1f\n"
			"1:\n"
			"fcmovm.ps f20, f14, f20\n"
			"fcmovm.ps f17, f17, f14\n"
			"bltu x16, x7, 1f\n"
			"1:\n"
			"fswizz.ps f11, f20, 0xb\n"
			"bltu x19, x28, 1f\n"
			"1:\n"
			"fmul.pi f20, f20, f17\n"
			"fmin.pi f14, f14, f20\n"
			"fsgnjn.s f17, f20, f17\n"
			"fpackrepb.pi f11, f17\n"
			"la x29, rand_ieee754_32\n"
			"flw.ps f2, 576(x29)\n"
			"flw.ps f16, 544(x29)\n"
			"fmax.ps f4, f2, f16\n" 
			"fnot.pi f11, f20\n"
			"bge x0, x19, 1f\n"
			"1:\n"
			"fxor.pi f26, f26, f26\n"
			"maskxor m1, m5, m1\n"
			"frcp.ps f17, f18\n"
			"slt x15, x0, x9\n"
			"fmadd.s f4, f18, f18, f31, rne\n"
			"srl x0, x0, x0\n"
			"bgeu x25, x25, 1f\n"
			"1:\n"
			"fmvs.x.ps x27, f22, 1\n"
			"sraw x0, x9, x0\n"
			"bge x0, x22, 1f\n"
			"1:\n"
			"li x9, 0x80241eb6d8\n"
			"li x11, 175002112\n"
			"fsc32h.ps f14, x15 (x9)\n" 
			"packb x29, x18, x9\n"
			"fmadd.s f14, f14, f12, f26, rmm\n"
			"bgeu x14, x11, 1f\n"
			"1:\n"
			"fclass.s x27, f20\n"
			"fmax.ps f26, f23, f2\n"
			"fbci.ps f18, 0x785fd\n"
			"fround.ps f13, f13, rup\n"
			"packb x25, x17, x25\n"
			"sub x7, x29, x9\n"
			"fadd.s f4, f13, f17, rne\n"
			"li x11, 0x8050862018\n"
			"amoxorg.w x7, x18, (x11)\n" 
			"fmaxu.pi f20, f2, f2\n"
			"fslli.pi f13, f13, 0xb\n"
			"fcvt.ps.pw f22, f31, rne\n"
			"fsgnj.ps f23, f23, f4\n"
			"fmin.pi f16, f4, f13\n"
			"fxor.pi f20, f20, f13\n"
			"fmulh.pi f27, f26, f27\n"
			"fsgnjx.ps f20, f20, f13\n"
			"fsatu8.pi f11, f4\n"
			"li x15, 0x9411000088ba\n"
			"csrw tensor_conv_ctrl, x15\n"	
			"fsgnjx.s f2, f26, f17\n"
			"bge x4, x29, 1f\n"
			"1:\n"
			"fsgnjn.ps f26, f26, f26\n"
			"fnmsub.s f20, f17, f17, f17, rmm\n"
			"fsub.pi f20, f17, f2\n"
			"fmadd.ps f16, f17, f17, f26, rne\n"
			"fsub.pi f20, f26, f17\n"
			"blt x27, x5, 1f\n"
			"1:\n"
			"fmax.ps f31, f17, f2\n"
			"fnmsub.ps f31, f17, f17, f26, rdn\n"
			"flt.ps f31, f2, f2\n"
			"la x9, rand_int_32\n"
			"flw.ps f22, 736(x9)\n"
			"flw.ps f18, 576(x9)\n"
			"fmulhu.pi f11, f22, f18\n" 
			"fnot.pi f31, f13\n"
			"sll x15, x25, x29\n"
			"bgeu x17, x4, 1f\n"
			"1:\n"
			"fmul.pi f12, f12, f4\n"
			"bltu x30, x13, 1f\n"
			"1:\n"
			"fmul.s f4, f12, f13, rmm\n"
			"bge x2, x18, 1f\n"
			"1:\n"
			"fltu.pi f4, f27, f4\n"
			"remu x5, x29, x29\n"
			"fsatu8.pi f11, f27\n"
			"bne x7, x3, 1f\n"
			"1:\n"
			"maskor m1, m5, m6\n"
			"fnot.pi f17, f23\n"
			"csrr x0, medeleg\n"
			"frcp.ps f11, f17\n"
			"fcvt.pw.ps f31, f22, rtz\n"
			"fsgnjx.s f31, f6, f22\n"
			"fnmsub.s f6, f22, f17, f6, rtz\n"
			"fmin.s f27, f31, f6\n"
			"fsll.pi f6, f6, f6\n"
			"beq x2, x0, 1f\n"
			"1:\n"
			"fcvt.f16.ps f17, f6\n"
			"bge x8, x8, 1f\n"
			"1:\n"
			"fswizz.ps f17, f17, 0x9\n"
			"fswizz.ps f6, f22, 0x15\n"
			"li x7, 0x198010e8a81114ea\n"
			"sraiw x11, x7, 5\n" 
			"fsatu8.pi f26, f16\n"
			"fclass.ps f16, f18\n"
			"srlw x11, x9, x11\n"
			"slt x15, x25, x15\n"
			"blt x23, x19, 1f\n"
			"1:\n"
			"fsub.pi f18, f18, f4\n"
			"blt x30, x24, 1f\n"
			"1:\n"
			"mulh x6, x6, x7\n"
			"mulhsu x6, x0, x0\n"
			"for.pi f17, f4, f16\n"
			"addw x5, x17, x5\n"
			"li x29, 0x8066fbd010\n"
			"la x11, mem_ptr_offset\n"
			"flw.ps f14, 416(x11)\n"
			"famoxorl.pi f12, f14 (x29)\n" 
			"fnmsub.s f14, f26, f2, f26, rup\n"
			"addiw x17, x9, 0x18\n"
			"fltm.ps m0, f2, f22\n"
			"fslli.pi f14, f4, 0x4\n"
			"sltiu x27, x9, 0xf\n"
			"fcvt.ps.f16 f31, f26\n"
			"addw x27, x18, x27\n"
			"bge x1, x10, 1f\n"
			"1:\n"
			"fcvt.w.s x5, f4, rmm\n"
			"fle.ps f4, f26, f22\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_1_TLD_ID_1_NMASK_0x1_CMASK_0xbf_SHIRE_0_HID_2:\n"
			"li x31, 0x1bf0e\n"
			"csrw tensor_coop, x31\n" 
			"li x31, 0x01FF000001FF0000\n"
			"csrw tensor_mask, x31\n" 
			"li x31, (0x3 << 6) | 0x1\n"
			"li x6, 0x4010008020006b0b\n"
			"csrw tensor_load, x6\n" 
			"li x7, 0x85800000118000\n"
			"csrw tensor_fma, x7\n" 
			"LBL_SYNC_POINT_4_HID_2:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_4_WAIT_HID_2\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_4_EXIT_HID_2\n"
			"LBL_SYNC_POINT_4_WAIT_HID_2:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_4_EXIT_HID_2:\n"          
			"nop\n"
			"LBL_SYNC_POINT_5_HID_2:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_5_WAIT_HID_2\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_5_EXIT_HID_2\n"
			"LBL_SYNC_POINT_5_WAIT_HID_2:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_5_EXIT_HID_2:\n"          
			"nop\n"
			"LBL_SEQID_1_TST_ID_1_QUAD128_START_CORE_0_SHIRE_0_HID_2:\n"
			"li x31, (0x4 << 4)\n"
			"li x6, 0xa25e008020006b10\n"
			"csrw tensor_store, x6\n" 
			"csrwi tensor_wait, 8\n" 
			"LBL_SYNC_POINT_6_HID_2:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_6_WAIT_HID_2\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_6_EXIT_HID_2\n"
			"LBL_SYNC_POINT_6_WAIT_HID_2:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_6_EXIT_HID_2:\n"          
			"nop\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x6, 0x1f8dc00000026677\n"
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 3\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f12, 0(x31)\n"
			"flw.ps f14, 3(x31)\n"
			"flw.ps f16, 2(x31)\n"
			"flw.ps f2, 8(x31)\n"
			"flw.ps f20, 8(x31)\n"
			"flw.ps f22, 2(x31)\n"
			"flw.ps f26, 23(x31)\n"
			"flw.ps f4, 6(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x11, 0(x31)\n"
			"lw x17, 5(x31)\n"
			"lw x18, 4(x31)\n"
			"lw x25, 4(x31)\n"
			"lw x5, 4(x31)\n"
			"lw x9, 15(x31)\n"
			"li x31, 0x2\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_2:\n"
			"fmadd.s f22, f12, f12, f22, rmm\n"
			"maskand m1, m5, m6\n"
			"maskpopc x15, m5\n"
			"fcvt.wu.s x25, f2, rne\n"
			"fmul.ps f4, f20, f4, rtz\n"
			"flt.ps f12, f12, f12\n"
			"fbcx.ps f11, x18\n"
			"sub x25, x25, x11\n"
			"xori x18, x0, 0xa\n"
			"srl x25, x18, x5\n"
			"srl x18, x5, x0\n"
			"fsgnj.ps f22, f2, f16\n"
			"bgeu x16, x17, 1f\n"
			"1:\n"
			"srli x15, x25, 3\n"
			"fcvt.ps.pw f23, f16, rup\n"
			"fcvt.ps.pwu f18, f14, rup\n"
			"fmax.ps f22, f26, f20\n"
			"sub x17, x5, x17\n"
			"fle.s x9, f14, f2\n"
			"ebreak\n"
			"fle.ps f14, f14, f26\n"
			"srl x15, x17, x9\n"
			"sllw x11, x11, x5\n"
			"bge x7, x3, 1f\n"
			"1:\n"
			"fsgnjx.ps f12, f22, f22\n"
			"beq x7, x27, 1f\n"
			"1:\n"
			"fmaxu.pi f12, f4, f12\n"
			"ebreak\n"
			"fsub.ps f26, f20, f26, rdn\n"
			"mulhsu x25, x25, x18\n"
			"flem.ps m5, f2, f20\n"
			"srai x15, x25, 4\n"
			"addi x15, x18, 0x1a\n"
			"srli x11, x11, 5\n"
			"beq x27, x11, 1f\n"
			"1:\n"
			"and x25, x25, x25\n"
			"maskpopcz x18, m6\n"
			"addi x17, x11, 0x2\n"
			"maskpopc x25, m5\n"
			"mulh x25, x0, x11\n"
			"slt x18, x25, x18\n"
			"fadd.s f4, f20, f4, rtz\n"
			"fsgnj.s f4, f14, f20\n"
			"fmax.s f2, f12, f20\n"
			"fsgnjx.ps f2, f26, f2\n"
			"remw x18, x0, x25\n"
			"divw x15, x0, x5\n"
			"fminu.pi f18, f12, f2\n"
			"subw x5, x5, x0\n"
			"bltu x5, x19, 1f\n"
			"1:\n"
			"fmulh.pi f26, f26, f20\n"
			"fmul.ps f18, f20, f16, rup\n"
			"mulh x15, x25, x18\n"
			"fbci.ps f22, 0x6e773\n"
			"fnmsub.ps f27, f20, f12, f14, rmm\n"
			"mov.m.x m0, x0, 0x3f\n"
			"fsgnjx.ps f20, f20, f16\n"
			"slti x17, x17, 0x0\n"
			"maskand m5, m5, m1\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_1_LOOP_SEQ_HID_2\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_HID_2:\n"
			"li x11, 0x8026343a38\n"
			"la x9, mem_ptr_offset\n"
			"flw.ps f23, 64(x9)\n"
			"fgh.ps f13, f23 (x11)\n" 
			"li x17, 0x8060889008\n"
			"amoaddl.d x27, x5, (x17)\n" 
			"li x17, 0x8052ae8020\n"
			"amoswapg.d x5, x25, (x17)\n" 
			"li x18, 0xc5cbc561e7767fd5\n"
			"andi x7, x18, 0x17\n" 
			"li x29, 0xf737681c0de15960\n"
			"li x25, 0x212a036f83ce6aec\n"
			"sra x27, x29, x25\n" 
			"li x17, 0x806499a024\n"
			"amominul.w x6, x25, (x17)\n" 
			"li x6, 0x80572d6030\n"
			"la x7, mem_ptr_offset\n"
			"flw.ps f12, 160(x7)\n"
			"famoandg.pi f27, f12 (x6)\n" 
			"li x11, 0x80251815c0\n"
			"la x25, mem_ptr_offset\n"
			"flw.ps f13, 192(x25)\n"
			"fgh.ps f12, f13 (x11)\n" 
			"li x5, 0xb201\n"
			"csrw tensor_mask, x5\n"	
			"li x29, 0x8063e5d018\n"
			"amominl.d x6, x27, (x29)\n" 
			"li x6, 0x8057c0600c\n"
			"amoaddg.w x29, x5, (x6)\n" 
			"li x15, 0x8023ba0ea8\n"
			"sd x25, 256(x15)\n"
			"lh x9, 256(x15)\n" 
			"li x5, 0xb131\n"
			"csrw tensor_mask, x5\n"	
			"li x15, 0x80657a0038\n"
			"amomaxl.w x17, x15, (x15)\n" 
			"li x5, 0xb9e3\n"
			"csrw tensor_mask, x5\n"	
			"li x25, 0x8061827030\n"
			"amominl.w x5, x9, (x25)\n" 
			"li x5, 0x30bb\n"
			"csrw tensor_mask, x5\n"	
			"li x11, 0x2e021a78a32416a3\n"
			"packb x29, x11, x11\n" 
			"li x5, 0xc342\n"
			"csrw tensor_mask, x5\n"	
			"li x11, 0x8050d1c004\n"
			"amoandg.w x17, x29, (x11)\n" 
			"li x9, 0x8054931010\n"
			"amoorg.w x25, x29, (x9)\n" 
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"li x5, 0xe615\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, 0x3840008020000988\n"
			"li x6, 0x334000802000280e\n"
			"li x7, 0x1c700000001a021\n"
			"li x9, 0x31c7000000067666\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_2:\n"
			"la x31, self_check_32\n"
			"li x5, 0x1\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000006\n"
			"add x6, x6, x31\n"
			"li x7, 0x18b000000119001\n"
			"li x10, 0xfb88008020002800\n"
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x7\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 3\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x30\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f16, 0(x31)\n"
			"flw.ps f22, 0(x31)\n"
			"flw.ps f31, 7(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x11, 0(x31)\n"
			"lw x18, 1(x31)\n"
			"lw x27, 7(x31)\n"
			"lw x29, 12(x31)\n"
			"lw x6, 2(x31)\n"
			"lw x7, 17(x31)\n"
			"lw x9, 5(x31)\n"
			"li x31, 0x2\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_2:\n"
			"remuw x6, x27, x0\n"
			"xori x29, x11, 0x8\n"
			"mova.x.m x5\n"
			"fmulh.pi f12, f22, f31\n"
			"fltm.ps m5, f31, f31\n"
			"fbci.pi f23, 0x1fedf\n"
			"fexp.ps f6, f31\n"
			"fbci.pi f27, 0x6518b\n"
			"masknot m1, m1\n"
			"frcp.ps f6, f16\n"
			"divuw x0, x11, x27\n"
			"fmvs.x.ps x9, f31, 7\n"
			"blt x14, x22, 1f\n"
			"1:\n"
			"fmul.ps f23, f31, f31, rne\n"
			"fbci.ps f31, 0x72188\n"
			"fsat8.pi f31, f16\n"
			"fmaxu.pi f14, f22, f31\n"
			"fcvt.f16.ps f31, f22\n"
			"fcvt.ps.pw f12, f22, rup\n"
			"fmax.ps f14, f31, f22\n"
			"fcvt.pw.ps f22, f31, rtz\n"
			"mova.m.x x27\n"
			"fcvt.w.s x0, f16, rne\n"
			"fandi.pi f27, f22, 0x1ca\n"
			"fbcx.ps f22, x29\n"
			"subw x18, x29, x18\n"
			"fsgnj.s f31, f31, f31\n"
			"fcmovm.ps f12, f31, f31\n"
			"fcmovm.ps f12, f31, f16\n"
			"fsrai.pi f14, f22, 0xc\n"
			"fle.pi f12, f22, f31\n"
			"flt.pi f23, f16, f16\n"
			"fmax.ps f14, f31, f31\n"
			"fsgnjn.s f22, f31, f22\n"
			"remu x0, x7, x0\n"
			"xori x5, x29, 0xf\n"
			"beq x8, x28, 1f\n"
			"1:\n"
			"divw x9, x9, x9\n"
			"sltu x6, x27, x6\n"
			"sllw x9, x9, x18\n"
			"sll x17, x7, x27\n"
			"blt x6, x10, 1f\n"
			"1:\n"
			"sra x29, x9, x18\n"
			"fadd.pi f14, f31, f31\n"
			"mova.x.m x0\n"
			"fmax.ps f23, f31, f16\n"
			"fcvt.ps.pw f31, f31, rdn\n"
			"bgeu x27, x3, 1f\n"
			"1:\n"
			"fadd.pi f31, f31, f31\n"
			"sltiu x18, x9, 0x5\n"
			"masknot m0, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"blt x24, x14, 1f\n"
			"1:\n"
			"sraw x5, x6, x27\n"
			"beq x9, x6, 1f\n"
			"1:\n"
			"srliw x6, x6, 4\n"
			"bltu x31, x16, 1f\n"
			"1:\n"
			"fle.pi f22, f22, f22\n"
			"fsub.s f31, f16, f31, rmm\n"
			"fmvz.x.ps x18, f22, 2\n"
			"fcvt.s.wu f16, x6, rup\n"
			"maskor m6, m6, m6\n"
			"fcvt.pwu.ps f12, f22, rdn\n"
			"fmv.s.x f27, x7\n"
			"or x27, x6, x27\n"
			"feqm.ps m1, f31, f22\n"
			"ecall\n"
			"fsgnjx.ps f22, f16, f22\n"
			"mulhsu x6, x6, x18\n"
			"fsetm.pi m1, f16\n"
			"xor x29, x29, x7\n"
			"faddi.pi f31, f31, 0x125\n"
			"fnmsub.ps f16, f16, f31, f31, rmm\n"
			"fandi.pi f31, f31, 0x60\n"
			"slt x18, x7, x18\n"
			"remuw x0, x6, x11\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_2_LOOP_SEQ_HID_2\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f31,  672 (x5)\n"
			"flw.ps f28,  992 (x5)\n"
			"flw.ps f19,  576 (x5)\n"
			"flw.ps f17,  896 (x5)\n"
			"flw.ps f22,  512 (x5)\n"
			"flw.ps f25,  832 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f26,  704 (x5)\n"
			"flw.ps f11,  352 (x5)\n"
			"flw.ps f29,  64 (x5)\n"
			"flw.ps f20,  0 (x5)\n"
			"flw.ps f9,  320 (x5)\n"
			"flw.ps f7,  224 (x5)\n"
			"flw.ps f4,  96 (x5)\n"
			"flw.ps f27,  416 (x5)\n"
			"flw.ps f5,  192 (x5)\n"
			"flw.ps f16,  128 (x5)\n"
			"flw.ps f13,  960 (x5)\n"
			"flw.ps f18,  256 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f3,  544 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f24,  384 (x5)\n"
			"flw.ps f10,  448 (x5)\n"
			"flw.ps f30,  288 (x5)\n"
			"flw.ps f23,  928 (x5)\n"
			"flw.ps f2,  768 (x5)\n"
			"flw.ps f6,  640 (x5)\n"
			"flw.ps f15,  480 (x5)\n"
			"flw.ps f0,  32 (x5)\n"
			"flw.ps f8,  800 (x5)\n"
			"flw.ps f14,  864 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_3_HID_2:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f6, 960(x9)\n"
			"fcvt.wu.s x25, f6, rdn\n" 
			"li x5, 0x2053\n"
			"csrw tensor_mask, x5\n"	
			"flt.ps f2, f14, f13\n"
			"fcvt.pwu.ps f13, f23, rne\n"
			"fsub.pi f13, f20, f13\n"
			"fle.pi f31, f18, f12\n"
			"fmax.ps f12, f14, f12\n"
			"andi x7, x7, 0xf\n"
			"ffrc.ps f23, f4\n"
			"bge x21, x12, 1f\n"
			"1:\n"
			"li x7, 0x805499c020\n"
			"amoaddg.d x27, x6, (x7)\n" 
			"fmul.pi f17, f27, f14\n"
			"bgeu x2, x29, 1f\n"
			"1:\n"
			"fsgnjx.s f22, f22, f12\n"
			"or x29, x0, x15\n"
			"bltu x3, x18, 1f\n"
			"1:\n"
			"fbci.pi f2, 0x7510\n"
			"fmadd.ps f22, f22, f4, f6, rup\n"
			"mul x17, x17, x6\n"
			"bge x2, x25, 1f\n"
			"1:\n"
			"flt.pi f20, f20, f20\n"
			"li x11, 0x8066dd3020\n"
			"amominul.d x25, x27, (x11)\n" 
			"fmvs.x.ps x15, f17, 4\n"
			"fmadd.s f20, f23, f22, f20, rne\n"
			"divu x6, x6, x15\n"
			"fmvs.x.ps x17, f31, 5\n"
			"fsll.pi f2, f18, f16\n"
			"fnmsub.s f13, f4, f13, f23, rmm\n"
			"for.pi f13, f16, f11\n"
			"li x29, 0x8063494028\n"
			"la x11, mem_ptr_offset\n"
			"flw.ps f20, 544(x11)\n"
			"famoandl.pi f12, f20 (x29)\n" 
			"frcp.ps f26, f26\n"
			"fmadd.ps f16, f11, f18, f12, rtz\n"
			"fcvt.ps.f16 f26, f18\n"
			"ffrc.ps f2, f11\n"
			"for.pi f12, f20, f26\n"
			"fexp.ps f16, f12\n"
			"fcvt.ps.pw f2, f12, rtz\n"
			"bgeu x14, x0, 1f\n"
			"1:\n"
			"li x6, 0x8050fa3018\n"
			"la x5, mem_ptr_offset\n"
			"flw.ps f20, 800(x5)\n"
			"famoorg.pi f27, f20 (x6)\n" 
			"fsrai.pi f11, f11, 0xe\n"
			"fmax.pi f26, f23, f14\n"
			"andi x25, x6, 0xe\n"
			"fmsub.ps f26, f26, f26, f16, rtz\n"
			"ecall\n"
			"addw x18, x17, x17\n"
			"fcvt.f16.ps f4, f11\n"
			"li x25, 0x8050ca3010\n"
			"amoming.w x29, x18, (x25)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fsgnjx.ps f27, f11, f17\n"
			"fmsub.s f27, f17, f16, f18, rmm\n"
			"fswizz.ps f6, f6, 0xd\n"
			"fsrl.pi f22, f17, f26\n"
			"bge x23, x29, 1f\n"
			"1:\n"
			"lui x17, 0xe\n"
			"sub x9, x9, x6\n"
			"fsra.pi f17, f31, f17\n"
			"li x27, 0x80550c3028\n"
			"amoswapg.d x5, x29, (x27)\n" 
			"fcvt.s.w f13, x29, rne\n"
			"bltu x6, x22, 1f\n"
			"1:\n"
			"divu x18, x18, x11\n"
			"fcvt.s.wu f4, x11, rmm\n"
			"fsub.ps f26, f16, f13, rdn\n"
			"divuw x11, x29, x18\n"
			"fadd.pi f26, f2, f2\n"
			"fsgnjx.ps f17, f22, f2\n"
			"li x17, 0xa1ff30f69927e53d\n"
			"li x29, 0x960be376d82541f\n"
			"slt x17, x17, x29\n" 
			"fmax.s f12, f6, f18\n"
			"addw x29, x27, x25\n"
			"add x11, x15, x5\n"
			"fmsub.ps f11, f11, f11, f13, rtz\n"
			"fsra.pi f31, f17, f13\n"
			"fcvt.ps.pw f13, f6, rtz\n"
			"fpackrepb.pi f4, f4\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f18, 896(x6)\n"
			"fround.ps f13, f18, rdn\n" 
			"li x5, 0xa377\n"
			"csrw tensor_mask, x5\n"	
			"fle.s x18, f26, f16\n"
			"fsub.ps f16, f31, f31, rne\n"
			"fmin.s f6, f13, f23\n"
			"fpackreph.pi f16, f16\n"
			"beq x31, x12, 1f\n"
			"1:\n"
			"subw x15, x7, x0\n"
			"sltu x15, x27, x15\n"
			"fsgnjx.s f14, f17, f14\n"
			"li x17, 0x80651f5014\n"
			"amomaxl.w x6, x6, (x17)\n" 
			"srlw x0, x9, x29\n"
			"faddi.pi f17, f12, 0x17b\n"
			"div x29, x29, x9\n"
			"sltiu x11, x29, 0x1b\n"
			"fsgnjx.ps f20, f20, f20\n"
			"fclass.ps f17, f18\n"
			"fmvz.x.ps x5, f20, 7\n"
			"li x18, 0x8051841028\n"
			"amoxorg.w x11, x18, (x18)\n" 
			"maskand m6, m0, m0\n"
			"ffrc.ps f26, f11\n"
			"mul x18, x17, x17\n"
			"maskxor m6, m0, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"faddi.pi f12, f6, 0x127\n"
			"fmvs.x.ps x17, f23, 5\n"
			"slli x27, x7, 2\n"
			"li x18, 0x8063907028\n"
			"amoxorl.w x7, x29, (x18)\n" 
			"for.pi f23, f23, f13\n"
			"fmax.s f23, f20, f18\n"
			"beq x10, x24, 1f\n"
			"1:\n"
			"fmul.pi f16, f4, f6\n"
			"fsgnj.ps f22, f16, f22\n"
			"flt.ps f18, f22, f18\n"
			"fmul.s f20, f23, f4, rtz\n"
			"beq x14, x3, 1f\n"
			"1:\n"
			"fround.ps f31, f4, rmm\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 768(x17)\n"
			"flw.ps f27, 160(x17)\n"
			"fdiv.ps f4, f16, f27, rmm\n" 
			"divu x25, x25, x11\n"
			"ecall\n"
			"bge x27, x2, 1f\n"
			"1:\n"
			"sra x7, x17, x25\n"
			"maskpopcz x11, m0\n"
			"maskpopc x11, m1\n"
			"fle.pi f17, f31, f26\n"
			"fmvz.x.ps x11, f16, 6\n"
			"bge x31, x30, 1f\n"
			"1:\n"
			"li x7, 0x52f472390073825c\n"
			"srai x27, x7, 5\n" 
			"fcvt.f16.ps f27, f2\n"
			"beq x21, x15, 1f\n"
			"1:\n"
			"sll x0, x29, x0\n"
			"bltu x11, x30, 1f\n"
			"1:\n"
			"fmax.pi f23, f26, f23\n"
			"fcmovm.ps f16, f26, f23\n"
			"fsub.ps f11, f6, f22, rne\n"
			"bltu x5, x13, 1f\n"
			"1:\n"
			"fsgnjx.ps f18, f18, f27\n"
			"sraiw x6, x11, 4\n"
			"bltu x30, x19, 1f\n"
			"1:\n"
			"li x7, 0x80644f4030\n"
			"la x6, mem_ptr_offset\n"
			"flw.ps f26, 992(x6)\n"
			"famomaxl.pi f27, f26 (x7)\n" 
			"fmsub.s f11, f12, f11, f17, rdn\n"
			"bltu x16, x15, 1f\n"
			"1:\n"
			"fsgnj.s f12, f12, f22\n"
			"fslli.pi f11, f17, 0x2\n"
			"fmul.ps f23, f13, f14, rup\n"
			"fminu.pi f16, f26, f27\n"
			"fmax.ps f20, f26, f16\n"
			"fcvt.f16.ps f13, f26\n"
			"li x15, 0x8027bd2b98\n"
			"la x25, mem_ptr_offset\n"
			"flw.ps f27, 736(x25)\n"
			"fgw.ps f13, f27 (x15)\n" 
			"fsgnjx.ps f6, f6, f6\n"
			"fsrli.pi f14, f18, 0xb\n"
			"fsgnjx.s f6, f6, f11\n"
			"fmul.ps f26, f6, f6, rdn\n"
			"fcvt.pwu.ps f18, f6, rup\n"
			"bge x25, x9, 1f\n"
			"1:\n"
			"fexp.ps f12, f11\n"
			"for.pi f18, f18, f6\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, 0x60008020002800\n"
			"li x6, 0x1000802000838e\n"
			"li x7, 0x8700000011d031\n"
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 2\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x80\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f18, 0(x31)\n"
			"flw.ps f20, 0(x31)\n"
			"flw.ps f23, 3(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_3_LOOP_SEQ_HID_2:\n"
			"fsrai.pi f16, f18, 0xc\n"
			"fmulhu.pi f20, f18, f20\n"
			"fmax.s f20, f20, f18\n"
			"fcvt.ps.pwu f12, f18, rne\n"
			"fmulhu.pi f18, f20, f18\n"
			"beq x17, x2, 1f\n"
			"1:\n"
			"fmsub.ps f20, f20, f20, f20, rne\n"
			"fadd.pi f16, f23, f23\n"
			"blt x7, x11, 1f\n"
			"1:\n"
			"ffrc.ps f16, f23\n"
			"fnmadd.s f20, f20, f20, f18, rne\n"
			"fcvt.ps.pwu f31, f20, rmm\n"
			"fadd.s f31, f20, f20, rtz\n"
			"fsrli.pi f31, f20, 0xb\n"
			"beq x22, x5, 1f\n"
			"1:\n"
			"flt.pi f18, f18, f18\n"
			"blt x12, x14, 1f\n"
			"1:\n"
			"fsrli.pi f12, f23, 0x6\n"
			"fsrli.pi f18, f18, 0xa\n"
			"bltu x1, x17, 1f\n"
			"1:\n"
			"fnmadd.ps f16, f23, f20, f23, rne\n"
			"fmsub.s f18, f20, f18, f20, rdn\n"
			"frcp.ps f20, f20\n"
			"fltu.pi f12, f18, f18\n"
			"fnmsub.ps f12, f20, f18, f18, rtz\n"
			"fcmovm.ps f31, f20, f18\n"
			"fadd.pi f31, f18, f18\n"
			"fand.pi f23, f23, f18\n"
			"blt x18, x28, 1f\n"
			"1:\n"
			"fnot.pi f12, f20\n"
			"bltu x1, x3, 1f\n"
			"1:\n"
			"fsub.s f23, f23, f20, rdn\n"
			"fswizz.ps f12, f20, 0x13\n"
			"fle.ps f12, f23, f20\n"
			"feq.ps f18, f20, f18\n"
			"fpackreph.pi f31, f23\n"
			"fmadd.ps f18, f23, f23, f18, rne\n"
			"fcvt.pwu.ps f31, f20, rup\n"
			"fswizz.ps f12, f20, 0x1c\n"
			"blt x29, x11, 1f\n"
			"1:\n"
			"fminu.pi f31, f20, f23\n"
			"flt.ps f16, f20, f18\n"
			"fcvt.pwu.ps f16, f18, rdn\n"
			"fsatu8.pi f31, f20\n"
			"fcmov.ps f16, f23, f18, f20\n"
			"fandi.pi f12, f18, 0xc5\n"
			"fmadd.ps f31, f23, f23, f18, rup\n"
			"fsatu8.pi f16, f20\n"
			"fswizz.ps f12, f18, 0x1f\n"
			"fsub.pi f12, f23, f20\n"
			"fsrl.pi f16, f23, f20\n"
			"fmsub.s f20, f23, f20, f23, rmm\n"
			"fmaxu.pi f16, f18, f20\n"
			"feq.ps f18, f20, f18\n"
			"beq x17, x31, 1f\n"
			"1:\n"
			"fsub.ps f31, f18, f23, rne\n"
			"bge x15, x2, 1f\n"
			"1:\n"
			"flog.ps f16, f23\n"
			"flt.ps f12, f20, f18\n"
			"blt x1, x13, 1f\n"
			"1:\n"
			"fmax.s f31, f23, f18\n"
			"bgeu x13, x25, 1f\n"
			"1:\n"
			"fmin.ps f23, f20, f23\n"
			"fsgnj.ps f16, f23, f18\n"
			"fsgnjx.s f20, f23, f23\n"
			"bne x7, x27, 1f\n"
			"1:\n"
			"fsgnjn.ps f18, f18, f18\n"
			"fsub.s f23, f23, f20, rmm\n"
			"bne x13, x18, 1f\n"
			"1:\n"
			"fmin.ps f18, f23, f18\n"
			"flog.ps f12, f18\n"
			"beq x30, x13, 1f\n"
			"1:\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_3_LOOP_SEQ_HID_2\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"la x31, check_sum_hid_2\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"LBL_POSTAMBLE_HID_2:\n"          
			"csrwi   tensor_wait, 0x0\n"
			"csrwi   tensor_wait, 0x1\n"
			"csrwi   tensor_wait, 0x2\n"
			"csrwi   tensor_wait, 0x3\n"
			"csrwi   tensor_wait, 0x4\n"
			"csrwi   tensor_wait, 0x5\n"
			"csrwi   tensor_wait, 0x6\n"
			"csrwi   tensor_wait, 0x7\n"
			"csrwi   tensor_wait, 0x8\n"
			"csrwi   tensor_wait, 0x9\n"
			"csrwi   tensor_wait, 0xa\n"
			"fence\n"
			"LBL_SYNC_POINT_7_HID_2:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_7_WAIT_HID_2\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_7_EXIT_HID_2\n"
			"LBL_SYNC_POINT_7_WAIT_HID_2:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_7_EXIT_HID_2:\n"          
			"nop\n"
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_2\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_2:\n"          
			".global LBL_FAIL_HID_2\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_2\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_2:\n"          
			".global LBL_PASS_HID_2\n"          
			VSNIP_RSV
		);
		c_test_pass();
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_2:\n"          
			".global LBL_HALT_HID_2\n"          
			"wfi\n"
			"j LBL_HALT_HID_2\n"          
			VSNIP_RSV
		);
		return 0;       
	};
	if ( hid == 3 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_3:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_3\n"
			"csrw mscratch, x5\n"
			"csrwi menable_shadows, 0xf\n"
			"li x5, -1\n"
			"csrw mcounteren, x5\n"
			"csrw scounteren, x5\n"
			"csrwi mhpmcounter3, 0\n"
			"csrwi mhpmcounter4, 0\n"
			"csrwi mhpmcounter5, 0\n"
			"csrwi mhpmcounter6, 0\n"
			"csrwi mhpmcounter7, 0\n"
			"csrwi mhpmcounter8, 0\n"
			"csrwi mhpmevent3, 7\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 9\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 23\n"   
			"mov.m.x m0, x0, 0xff\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f10,  384 (x5)\n"
			"flw.ps f30,  928 (x5)\n"
			"flw.ps f5,  672 (x5)\n"
			"flw.ps f9,  544 (x5)\n"
			"flw.ps f17,  160 (x5)\n"
			"flw.ps f11,  32 (x5)\n"
			"flw.ps f25,  96 (x5)\n"
			"flw.ps f0,  864 (x5)\n"
			"flw.ps f19,  832 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f16,  640 (x5)\n"
			"flw.ps f14,  288 (x5)\n"
			"flw.ps f29,  896 (x5)\n"
			"flw.ps f12,  256 (x5)\n"
			"flw.ps f22,  992 (x5)\n"
			"flw.ps f3,  224 (x5)\n"
			"flw.ps f24,  192 (x5)\n"
			"flw.ps f28,  608 (x5)\n"
			"flw.ps f1,  64 (x5)\n"
			"flw.ps f27,  736 (x5)\n"
			"flw.ps f26,  800 (x5)\n"
			"flw.ps f21,  0 (x5)\n"
			"flw.ps f31,  512 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f13,  704 (x5)\n"
			"flw.ps f2,  480 (x5)\n"
			"flw.ps f20,  352 (x5)\n"
			"flw.ps f18,  576 (x5)\n"
			"flw.ps f15,  128 (x5)\n"
			"flw.ps f23,  416 (x5)\n"
			"flw.ps f8,  448 (x5)\n"
			"flw.ps f6,  320 (x5)\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_HID_3:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_0_WAIT_HID_3\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_0_EXIT_HID_3\n"
			"LBL_SYNC_POINT_0_WAIT_HID_3:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_EXIT_HID_3:\n"          
			"nop\n"
			"LBL_KERNEL_HID_3:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_3\n"
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_HID_3:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f29, 544(x9)\n"
			"flw.ps f1, 480(x9)\n"
			"flw.ps f14, 896(x9)\n"
			"fmadd.ps f29, f29, f1, f14, rtz\n" 
			"addiw x16, x16, 0x13\n"
			"fsetm.pi m3, f5\n"
			"xori x15, x25, 0x13\n"
			"li x21, 0x805ca2d010\n"
			"la x9, mem_ptr_offset\n"
			"flw.ps f29, 960(x9)\n"
			"famoswapg.pi f14, f29 (x21)\n" 
			"srai x12, x18, 4\n"
			"fbcx.ps f7, x18\n"
			"ffrc.ps f13, f22\n"
			"beq x19, x29, 1f\n"
			"1:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f15, 896(x25)\n"
			"fmvz.x.ps x9, f15, 5\n" 
			"sltu x17, x16, x21\n"
			"fmv.s.x f0, x16\n"
			"fmax.pi f29, f15, f11\n"
			"li x15, 0x805b1b7038\n"
			"amoswapg.d x21, x16, (x15)\n" 
			"sllw x0, x9, x21\n"
			"fle.s x18, f14, f9\n"
			"fmvs.x.ps x12, f9, 1\n"
			"bne x15, x31, 1f\n"
			"1:\n"
			"li x18, 0x805fd74008\n"
			"amoandg.w x20, x29, (x18)\n" 
			"remw x7, x7, x25\n"
			"feqm.ps m3, f15, f5\n"
			"fsrl.pi f11, f26, f22\n"
			"li x15, 0xf80ba155b5acaa05\n"
			"li x7, 0x62ce8c089184989d\n"
			"addw x15, x15, x7\n" 
			"divw x21, x25, x20\n"
			"sra x7, x17, x7\n"
			"rem x29, x0, x29\n"
			"bge x7, x6, 1f\n"
			"1:\n"
			"li x15, 0x802eaac9a0\n"
			"li x9, 168736456\n"
			"fg32w.ps f5, x15 (x15)\n" 
			"fcmov.ps f7, f7, f19, f22\n"
			"fcvt.f16.ps f19, f22\n"
			"fnmsub.s f7, f22, f23, f7, rne\n"
			"li x29, 0x805a5eb000\n"
			"la x9, mem_ptr_offset\n"
			"flw.ps f23, 64(x9)\n"
			"fscbg.ps f14, f23 (x29)\n" 
			"fmax.pi f14, f30, f9\n"
			"flt.ps f8, f14, f19\n"
			"fsgnj.s f22, f22, f26\n"
			"blt x22, x18, 1f\n"
			"1:\n"
			"li x18, 0x8068ca2000\n"
			"amomaxl.d x7, x21, (x18)\n" 
			"fclass.ps f7, f9\n"
			"fsub.pi f1, f15, f23\n"
			"fround.ps f5, f29, rup\n"
			"li x18, 0x805f81d030\n"
			"la x16, mem_ptr_offset\n"
			"flw.ps f23, 416(x16)\n"
			"famoaddg.pi f22, f23 (x18)\n" 
			"fmax.s f5, f30, f23\n"
			"frcp.ps f15, f13\n"
			"fsgnjn.ps f22, f13, f14\n"
			"bne x7, x1, 1f\n"
			"1:\n"
			"li x31, 0x8020002d80\n"
			"li x18, 0x2\n"
			"add x18, x31, x18\n"
			"li x31, (0x2 << 6) | 0x1\n"
			"csrwi tensor_wait, 6\n" 
			"fmin.pi f13, f8, f7\n"
			"fsgnjx.s f9, f19, f14\n"
			"bge x5, x3, 1f\n"
			"1:\n"
			"xor x25, x29, x16\n"
			"li x20, 0x802c790808\n"
			"la x15, mem_ptr_offset\n"
			"flw.ps f11, 960(x15)\n"
			"fscw.ps f5, f11 (x20)\n" 
			"fsrai.pi f19, f5, 0x3\n"
			"fmul.s f14, f9, f5, rdn\n"
			"fexp.ps f8, f8\n"
			"li x9, 0x802f722638\n"
			"la x17, mem_ptr_offset\n"
			"flw.ps f30, 0(x17)\n"
			"fscb.ps f22, f30 (x9)\n" 
			"sllw x9, x15, x17\n"
			"fmv.x.s x21, f26\n"
			"sll x17, x12, x15\n"
			"bgeu x2, x3, 1f\n"
			"1:\n"
			"li x17, 0x806a60d000\n"
			"amoswapl.d x7, x7, (x17)\n" 
			"fcvt.pw.ps f14, f8, rmm\n"
			"fsetm.pi m6, f23\n"
			"sraw x15, x21, x15\n"
			"li x15, 0xeb681de86c7fa674\n"
			"ori x7, x15, 0x1c\n" 
			"fle.pi f0, f30, f22\n"
			"flog.ps f0, f19\n"
			"fcmov.ps f8, f26, f9, f15\n"
			"bne x31, x19, 1f\n"
			"1:\n"
			"li x16, 0x805f2ca000\n"
			"la x20, mem_ptr_offset\n"
			"flw.ps f11, 672(x20)\n"
			"famoaddg.pi f0, f11 (x16)\n" 
			"fsatu8.pi f8, f8\n"
			"fle.ps f7, f22, f1\n"
			"fclass.ps f11, f9\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x8\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f13, 1(x31)\n"
			"flw.ps f19, 3(x31)\n"
			"flw.ps f22, 0(x31)\n"
			"flw.ps f26, 1(x31)\n"
			"flw.ps f30, 19(x31)\n"
			"flw.ps f9, 22(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x17, 4(x31)\n"
			"lw x21, 0(x31)\n"
			"lw x25, 11(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_3:\n"
			"and x17, x17, x21\n"
			"fcvt.wu.s x17, f30, rdn\n"
			"bne x10, x2, 1f\n"
			"1:\n"
			"feq.s x0, f13, f0\n"
			"fbci.pi f7, 0x5aafc\n"
			"blt x0, x31, 1f\n"
			"1:\n"
			"feq.ps f14, f22, f26\n"
			"fmax.s f30, f26, f30\n"
			"flt.pi f0, f30, f9\n"
			"flt.ps f7, f0, f0\n"
			"div x9, x17, x17\n"
			"fslli.pi f22, f22, 0xf\n"
			"bltu x5, x7, 1f\n"
			"1:\n"
			"fcvt.wu.s x0, f22, rtz\n"
			"bgeu x1, x16, 1f\n"
			"1:\n"
			"srai x21, x21, 4\n"
			"fltm.pi m3, f9, f19\n"
			"fclass.ps f26, f19\n"
			"fmul.ps f7, f26, f30, rne\n"
			"sra x0, x0, x17\n"
			"blt x1, x17, 1f\n"
			"1:\n"
			"fadd.pi f11, f30, f9\n"
			"maskand m2, m2, m3\n"
			"divw x9, x17, x21\n"
			"flog.ps f0, f26\n"
			"fmaxu.pi f14, f19, f13\n"
			"fnmsub.ps f19, f19, f9, f19, rtz\n"
			"bgeu x4, x7, 1f\n"
			"1:\n"
			"fsatu8.pi f30, f13\n"
			"fmaxu.pi f0, f0, f26\n"
			"mova.m.x x25\n"
			"feq.pi f13, f13, f26\n"
			"fnmsub.s f29, f22, f13, f26, rtz\n"
			"fsrai.pi f9, f9, 0x5\n"
			"blt x5, x1, 1f\n"
			"1:\n"
			"mul x25, x25, x17\n"
			"fmaxu.pi f9, f26, f0\n"
			"fsub.pi f7, f9, f22\n"
			"fsub.pi f7, f9, f0\n"
			"fbci.pi f29, 0x3cd07\n"
			"blt x16, x28, 1f\n"
			"1:\n"
			"fmv.s.x f14, x17\n"
			"sllw x0, x0, x17\n"
			"xori x17, x21, 0x6\n"
			"bge x4, x16, 1f\n"
			"1:\n"
			"fmul.ps f0, f9, f30, rup\n"
			"fclass.s x0, f26\n"
			"divuw x29, x21, x21\n"
			"frcp.ps f13, f19\n"
			"slli x17, x17, 1\n"
			"fsrai.pi f13, f19, 0x4\n"
			"mov.m.x m3, x17, 0x69\n"
			"beq x29, x18, 1f\n"
			"1:\n"
			"sra x0, x25, x25\n"
			"beq x27, x3, 1f\n"
			"1:\n"
			"maskpopcz x21, m6\n"
			"slliw x29, x17, 2\n"
			"fnmsub.s f13, f9, f13, f26, rne\n"
			"maskand m7, m2, m7\n"
			"bne x27, x3, 1f\n"
			"1:\n"
			"flt.s x12, f26, f0\n"
			"bltu x17, x6, 1f\n"
			"1:\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_0_LOOP_SEQ_HID_3\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_HID_3:\n"
			"li x25, 0x805e806010\n"
			"la x12, mem_ptr_offset\n"
			"flw.ps f8, 608(x12)\n"
			"famoandg.pi f13, f8 (x25)\n" 
			"fltu.pi f5, f22, f5\n"
			"li x9, 0x8028f2a9ef\n"
			"sb x21, -256(x9)\n" 
			"fminu.pi f11, f11, f11\n"
			"beq x4, x0, 1f\n"
			"1:\n"
			"li x12, 0x806943f000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f26, (x12)\n" 
			"fnot.pi f5, f19\n"
			"li x21, 0x806e873008\n"
			"la x20, mem_ptr_offset\n"
			"flw.ps f0, 480(x20)\n"
			"famomaxl.ps f1, f0 (x21)\n" 
			"fsgnjn.ps f14, f19, f14\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f29, 320(x16)\n"
			"flw.ps f13, 384(x16)\n"
			"flw.ps f9, 544(x16)\n"
			"fcmov.ps f23, f29, f13, f9\n" 
			"fmul.pi f15, f15, f9\n"
			"li x17, 0x805e844020\n"
			"amoandg.d x12, x7, (x17)\n" 
			"fsgnj.s f19, f5, f19\n"
			"la x9, rand_int_32\n"
			"flw.ps f8, 224(x9)\n"
			"fandi.pi f15, f8, 0x156\n" 
			"fcvt.pwu.ps f23, f23, rtz\n"
			"divw x15, x21, x20\n" 
			"fsgnj.ps f22, f23, f8\n"
			"li x25, 0x806e40e000\n"
			"la x16, mem_ptr_offset\n"
			"flw.ps f0, 128(x16)\n"
			"famominl.ps f13, f0 (x25)\n" 
			"fpackrepb.pi f0, f0\n"
			"li x18, 0x802fb2b048\n"
			"li x21, 2549468896\n"
			"fg32b.ps f30, x29 (x18)\n" 
			"fsgnj.s f8, f9, f5\n"
			"li x9, 0x78b1a3c660c2379e\n"
			"fcvt.s.w f23, x9, rtz\n" 
			"for.pi f14, f15, f22\n"
			"la x7, rand_int_32\n"
			"flw.ps f22, 544(x7)\n"
			"flw.ps f29, 832(x7)\n"
			"for.pi f23, f22, f29\n" 
			"fround.ps f15, f14, rdn\n"
			"lui x16, 0x0\n" 
			"maskpopc x15, m3\n"
			"bgeu x1, x18, 1f\n"
			"1:\n"
			"li x9, 0x80591de028\n"
			"amomaxug.d x17, x15, (x9)\n" 
			"add x0, x17, x0\n"
			"li x17, 0x805d011020\n"
			"amomaxg.d x25, x20, (x17)\n" 
			"srli x16, x16, 4\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f30, 672(x15)\n"
			"flw.ps f15, 800(x15)\n"
			"fmsub.ps f5, f30, f15, f30, rtz\n" 
			"fsra.pi f14, f19, f29\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x10\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f1, 2(x31)\n"
			"flw.ps f11, 5(x31)\n"
			"flw.ps f13, 1(x31)\n"
			"flw.ps f14, 10(x31)\n"
			"flw.ps f15, 20(x31)\n"
			"flw.ps f19, 0(x31)\n"
			"flw.ps f22, 8(x31)\n"
			"flw.ps f23, 31(x31)\n"
			"flw.ps f26, 27(x31)\n"
			"flw.ps f29, 32(x31)\n"
			"flw.ps f30, 7(x31)\n"
			"flw.ps f5, 4(x31)\n"
			"flw.ps f7, 5(x31)\n"
			"flw.ps f8, 35(x31)\n"
			"flw.ps f9, 28(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x12, 0(x31)\n"
			"lw x15, 7(x31)\n"
			"lw x16, 8(x31)\n"
			"lw x17, 7(x31)\n"
			"lw x18, 14(x31)\n"
			"lw x20, 9(x31)\n"
			"lw x21, 10(x31)\n"
			"lw x25, 2(x31)\n"
			"lw x29, 35(x31)\n"
			"lw x7, 25(x31)\n"
			"lw x9, 19(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_3:\n"
			"fence\n"
			"fminu.pi f23, f22, f11\n"
			"for.pi f9, f11, f14\n"
			"bgeu x28, x0, 1f\n"
			"1:\n"
			"flem.ps m7, f15, f30\n"
			"fmul.pi f30, f11, f7\n"
			"addi x29, x16, 0x7\n"
			"srai x17, x16, 4\n"
			"bltu x1, x3, 1f\n"
			"1:\n"
			"fadd.ps f7, f0, f11, rne\n"
			"fcvt.w.s x9, f19, rtz\n"
			"fsgnj.ps f9, f8, f8\n"
			"fminu.pi f0, f5, f1\n"
			"mul x20, x15, x18\n"
			"fmul.s f11, f19, f7, rup\n"
			"fmvz.x.ps x9, f22, 7\n"
			"fmv.s.x f1, x9\n"
			"fnmsub.s f22, f19, f23, f29, rmm\n"
			"mulh x0, x20, x9\n"
			"fsll.pi f14, f9, f15\n"
			"bgeu x27, x7, 1f\n"
			"1:\n"
			"ori x17, x29, 0x17\n"
			"bge x23, x9, 1f\n"
			"1:\n"
			"mul x18, x0, x20\n"
			"fmadd.s f22, f23, f1, f22, rdn\n"
			"fltm.pi m3, f14, f11\n"
			"srli x15, x15, 4\n"
			"blt x27, x23, 1f\n"
			"1:\n"
			"fmsub.s f19, f11, f26, f15, rdn\n"
			"fmin.s f7, f5, f23\n"
			"sll x18, x18, x7\n"
			"feqm.ps m3, f15, f29\n"
			"fcvt.wu.s x21, f29, rmm\n"
			"bgeu x28, x0, 1f\n"
			"1:\n"
			"auipc x29, 0x2\n"
			"fmvz.x.ps x7, f13, 5\n"
			"fence\n"
			"fcvt.pwu.ps f23, f8, rne\n"
			"sltu x0, x12, x17\n"
			"fsrai.pi f9, f29, 0xe\n"
			"bge x24, x25, 1f\n"
			"1:\n"
			"fmul.pi f23, f15, f8\n"
			"bgeu x29, x1, 1f\n"
			"1:\n"
			"ori x0, x17, 0x11\n"
			"addiw x15, x15, 0x7\n"
			"feq.s x15, f9, f29\n"
			"bne x14, x27, 1f\n"
			"1:\n"
			"fpackrepb.pi f29, f9\n"
			"blt x8, x14, 1f\n"
			"1:\n"
			"slli x16, x7, 1\n"
			"mova.x.m x12\n"
			"bltu x15, x14, 1f\n"
			"1:\n"
			"flt.pi f8, f13, f14\n"
			"beq x18, x23, 1f\n"
			"1:\n"
			"fsgnj.s f23, f9, f8\n"
			"bne x6, x5, 1f\n"
			"1:\n"
			"fnmadd.s f8, f13, f9, f0, rmm\n"
			"maskor m2, m3, m3\n"
			"fmax.pi f9, f22, f8\n"
			"slt x7, x25, x21\n"
			"blt x2, x13, 1f\n"
			"1:\n"
			"srai x12, x18, 5\n"
			"fltm.pi m7, f19, f5\n"
			"blt x20, x12, 1f\n"
			"1:\n"
			"rem x7, x7, x16\n"
			"fslli.pi f19, f0, 0xe\n"
			"lui x9, 0x16\n"
			"beq x6, x25, 1f\n"
			"1:\n"
			"fmax.pi f9, f13, f8\n"
			"sltiu x17, x18, 0x11\n"
			"fltm.ps m3, f7, f13\n"
			"bgeu x16, x1, 1f\n"
			"1:\n"
			"fmvs.x.ps x18, f29, 7\n"
			"flt.ps f23, f0, f19\n"
			"for.pi f26, f26, f22\n"
			"beq x4, x13, 1f\n"
			"1:\n"
			"fcvt.s.w f26, x16, rup\n"
			"fmadd.ps f26, f15, f23, f23, rmm\n"
			"fslli.pi f29, f5, 0xa\n"
			"fmax.pi f29, f30, f8\n"
			"fmax.ps f14, f8, f5\n"
			"maskxor m7, m3, m6\n"
			"xori x21, x15, 0x15\n"
			"fsrli.pi f14, f7, 0xe\n"
			"srl x16, x21, x17\n"
			"andi x9, x29, 0x15\n"
			"fxor.pi f9, f13, f9\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_1_LOOP_SEQ_HID_3\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_HID_3:\n"
			"li x21, 0x805eff0020\n"
			"la x29, mem_ptr_offset\n"
			"flw.ps f0, 32(x29)\n"
			"famomaxug.pi f23, f0 (x21)\n" 
			"feq.s x16, f19, f19\n"
			"divw x7, x25, x7\n"
			"fclass.ps f9, f9\n"
			"fbci.pi f5, 0x5b1d2\n"
			"blt x4, x20, 1f\n"
			"1:\n"
			"ffrc.ps f19, f19\n"
			"remuw x16, x17, x16\n"
			"and x25, x25, x18\n"
			"fcvt.w.s x15, f29, rtz\n"
			"sllw x9, x25, x12\n"
			"frcp.ps f5, f5\n"
			"li x18, 0x805bd18038\n"
			"la x7, mem_ptr_offset\n"
			"flw.ps f13, 96(x7)\n"
			"famomaxg.ps f30, f13 (x18)\n" 
			"fle.s x16, f13, f7\n"
			"flog.ps f7, f7\n"
			"maskand m3, m2, m7\n"
			"lui x25, 0x10\n"
			"fmv.s.x f19, x12\n"
			"remuw x18, x18, x18\n"
			"fslli.pi f23, f0, 0x6\n"
			"subw x16, x20, x16\n"
			"beq x10, x24, 1f\n"
			"1:\n"
			"mova.m.x x12\n"
			"and x7, x12, x20\n"
			"mova.x.m x18\n" 
			"flt.pi f11, f15, f8\n"
			"fsll.pi f22, f8, f8\n"
			"fclass.ps f8, f7\n"
			"blt x4, x7, 1f\n"
			"1:\n"
			"fsgnjn.s f1, f8, f15\n"
			"fround.ps f8, f15, rmm\n"
			"fmul.s f11, f8, f8, rdn\n"
			"fsub.pi f22, f8, f7\n"
			"flt.pi f15, f15, f15\n"
			"fcvt.ps.pwu f7, f15, rtz\n"
			"fsat8.pi f1, f15\n"
			"bge x14, x10, 1f\n"
			"1:\n"
			"li x15, 0x805ad70028\n"
			"la x17, mem_ptr_offset\n"
			"flw.ps f15, 192(x17)\n"
			"fgbg.ps f13, f15 (x15)\n" 
			"fence\n"
			"fsub.s f0, f9, f11, rdn\n"
			"fbci.ps f14, 0x15683\n"
			"flem.ps m3, f8, f1\n"
			"fle.pi f30, f9, f1\n"
			"maskpopcz x9, m6\n"
			"masknot m3, m3\n"
			"fswizz.ps f19, f1, 0x9\n"
			"fsetm.pi m3, f1\n"
			"fbcx.ps f13, x12\n"
			"li x16, 0x802f6f4818\n"
			"fsq2 f0, -256(x16)\n" 
			"mova.x.m x12\n"
			"rem x16, x18, x12\n"
			"bge x0, x11, 1f\n"
			"1:\n"
			"ori x0, x12, 0x4\n"
			"divuw x9, x21, x9\n"
			"mulw x20, x29, x0\n"
			"mova.m.x x17\n"
			"bgeu x16, x10, 1f\n"
			"1:\n"
			"sub x7, x15, x16\n"
			"fsub.s f8, f30, f22, rdn\n"
			"bgeu x6, x14, 1f\n"
			"1:\n"
			"fcvt.s.w f26, x0, rtz\n"
			"bne x5, x9, 1f\n"
			"1:\n"
			"fmulh.pi f29, f13, f0\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f8, 640(x16)\n"
			"flw.ps f30, 640(x16)\n"
			"fmsub.ps f13, f8, f30, f30, rdn\n" 
			"fround.ps f1, f8, rtz\n"
			"or x15, x20, x16\n"
			"fmaxu.pi f9, f26, f30\n"
			"packb x29, x21, x7\n"
			"fbci.ps f14, 0x7c44b\n"
			"masknot m2, m2\n"
			"mulh x12, x7, x7\n"
			"blt x3, x29, 1f\n"
			"1:\n"
			"fxor.pi f0, f11, f0\n"
			"bgeu x22, x8, 1f\n"
			"1:\n"
			"fence\n"
			"slt x7, x16, x7\n"
			"blt x4, x26, 1f\n"
			"1:\n"
			"li x7, 0x8029153a10\n"
			"li x21, 3962376748\n"
			"fg32w.ps f22, x29 (x7)\n" 
			"mova.x.m x17\n"
			"fnot.pi f9, f0\n"
			"fsat8.pi f9, f15\n"
			"divuw x18, x17, x18\n"
			"fcvt.ps.pwu f15, f8, rne\n"
			"divu x17, x17, x20\n"
			"fcmov.ps f5, f7, f15, f30\n"
			"fmaxu.pi f9, f11, f0\n"
			"divw x29, x18, x20\n"
			"sllw x29, x20, x20\n"
			"li x7, 0x80580aa018\n"
			"amoswapg.d x18, x20, (x7)\n" 
			"fround.ps f23, f22, rmm\n"
			"fsrli.pi f1, f14, 0xa\n"
			"fmax.pi f14, f5, f9\n"
			"feq.ps f23, f23, f23\n"
			"fsll.pi f26, f22, f13\n"
			"feq.pi f14, f7, f5\n"
			"fmulhu.pi f30, f19, f13\n"
			"fmax.pi f26, f19, f15\n"
			"fcvt.ps.f16 f9, f13\n"
			"fmaxu.pi f9, f26, f29\n"
			"li x25, 0x802fda7998\n"
			"la x21, mem_ptr_offset\n"
			"flw.ps f8, 96(x21)\n"
			"fgh.ps f11, f8 (x25)\n" 
			"mulhu x7, x9, x7\n"
			"fltm.ps m2, f5, f23\n"
			"fmul.s f15, f0, f23, rup\n"
			"fsrli.pi f19, f30, 0x1\n"
			"blt x3, x14, 1f\n"
			"1:\n"
			"ecall\n"
			"bge x25, x31, 1f\n"
			"1:\n"
			"fmulhu.pi f13, f0, f13\n"
			"fcvt.w.s x12, f13, rtz\n"
			"fmul.pi f11, f11, f5\n"
			"flem.ps m2, f9, f30\n"
			"fsat8.pi f5, f29\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f14, 704(x17)\n"
			"fexp.ps f0, f14\n" 
			"fsgnj.ps f26, f11, f22\n"
			"fsgnjx.s f30, f0, f30\n"
			"fcmov.ps f30, f9, f15, f30\n"
			"bge x6, x1, 1f\n"
			"1:\n"
			"fmsub.ps f22, f30, f9, f22, rup\n"
			"bne x12, x10, 1f\n"
			"1:\n"
			"flt.pi f0, f0, f11\n"
			"fsgnj.ps f23, f0, f15\n"
			"fltu.pi f14, f9, f22\n"
			"fnmsub.ps f0, f15, f0, f11, rmm\n"
			"fle.ps f14, f19, f15\n"
			"fsub.ps f23, f8, f0, rne\n"
			"li x16, 0x806f571008\n"
			"la x12, mem_ptr_offset\n"
			"flw.ps f14, 448(x12)\n"
			"famominl.ps f26, f14 (x16)\n" 
			"frcp.ps f29, f29\n"
			"beq x24, x1, 1f\n"
			"1:\n"
			"fmv.x.s x9, f15\n"
			"fsgnjn.ps f29, f29, f7\n"
			"divu x0, x18, x17\n"
			"fcvt.wu.s x29, f15, rne\n"
			"mulw x20, x18, x17\n"
			"fsub.s f14, f14, f7, rmm\n"
			"srl x29, x20, x7\n"
			"fbci.ps f11, 0x24134\n"
			"fmsub.ps f14, f7, f7, f14, rup\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 896(x15)\n"
			"flw.ps f15, 0(x15)\n"
			"flw.ps f0, 640(x15)\n"
			"fnmadd.ps f7, f1, f15, f0, rmm\n" 
			"srai x0, x9, 0\n"
			"fmin.s f30, f15, f30\n"
			"sltiu x15, x15, 0x1b\n"
			"fbcx.ps f9, x0\n"
			"flt.s x18, f23, f7\n"
			"mulh x20, x20, x9\n"
			"mulhsu x9, x20, x9\n"
			"sra x12, x9, x12\n"
			"flem.ps m6, f30, f26\n"
			"fsub.ps f29, f9, f29, rmm\n"
			"li x12, 0x805aecc038\n"
			"la x20, mem_ptr_offset\n"
			"flw.ps f22, 256(x20)\n"
			"famoming.pi f23, f22 (x12)\n" 
			"divu x7, x7, x7\n"
			"feqm.ps m2, f11, f1\n"
			"bge x5, x29, 1f\n"
			"1:\n"
			"maskand m6, m7, m6\n"
			"fence\n"
			"bne x0, x3, 1f\n"
			"1:\n"
			"fsgnj.s f13, f29, f19\n"
			"feq.pi f9, f15, f7\n"
			"fsrai.pi f19, f19, 0xe\n"
			"mul x12, x25, x12\n"
			"bge x28, x17, 1f\n"
			"1:\n"
			"addiw x29, x15, 0x1c\n"
			"mul x25, x25, x21\n"
			"li x16, 0x806c313010\n"
			"amomaxul.w x17, x9, (x16)\n" 
			"fle.pi f19, f19, f19\n"
			"ffrc.ps f22, f23\n"
			"fswizz.ps f1, f30, 0x1d\n"
			"fnmadd.ps f8, f19, f23, f8, rdn\n"
			"bge x16, x15, 1f\n"
			"1:\n"
			"fmin.s f8, f19, f19\n"
			"fclass.ps f23, f23\n"
			"bltu x14, x27, 1f\n"
			"1:\n"
			"fmax.s f30, f30, f5\n"
			"fltu.pi f15, f23, f30\n"
			"fmaxu.pi f19, f19, f23\n"
			"fle.ps f8, f8, f5\n"
			"li x25, 0x806b408038\n"
			"amoorl.d x17, x7, (x25)\n" 
			"fcvt.wu.s x20, f9, rne\n"
			"bge x17, x3, 1f\n"
			"1:\n"
			"maskxor m3, m7, m6\n"
			"fswizz.ps f26, f0, 0x5\n"
			"fsgnj.ps f29, f29, f8\n"
			"ecall\n"
			"bgeu x11, x21, 1f\n"
			"1:\n"
			"flog.ps f8, f9\n"
			"mulhsu x18, x7, x20\n"
			"fsgnjx.ps f23, f1, f1\n"
			"bge x29, x26, 1f\n"
			"1:\n"
			"srliw x20, x17, 5\n"
			"slli x15, x20, 3\n"
			"la x17, rand_ieee754_16\n"
			"flw.ps f11, 384(x17)\n"
			"fcvt.ps.f16 f23, f11\n" 
			"fnot.pi f8, f14\n"
			"beq x21, x8, 1f\n"
			"1:\n"
			"faddi.pi f26, f26, 0x1e3\n"
			"fslli.pi f11, f26, 0xe\n"
			"fnmsub.s f0, f22, f0, f29, rmm\n"
			"fmul.s f1, f5, f9, rup\n"
			"fmul.s f9, f15, f23, rdn\n"
			"fnmsub.s f9, f5, f22, f26, rmm\n"
			"fcvt.ps.pw f5, f29, rdn\n"
			"fmul.s f23, f23, f9, rup\n"
			"fsub.ps f7, f1, f0, rtz\n"
			"bge x13, x13, 1f\n"
			"1:\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x18\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f1, 1(x31)\n"
			"flw.ps f11, 8(x31)\n"
			"flw.ps f13, 12(x31)\n"
			"flw.ps f14, 16(x31)\n"
			"flw.ps f15, 20(x31)\n"
			"flw.ps f19, 8(x31)\n"
			"flw.ps f22, 4(x31)\n"
			"flw.ps f23, 7(x31)\n"
			"flw.ps f26, 0(x31)\n"
			"flw.ps f29, 8(x31)\n"
			"flw.ps f30, 44(x31)\n"
			"flw.ps f5, 38(x31)\n"
			"flw.ps f7, 8(x31)\n"
			"flw.ps f8, 33(x31)\n"
			"flw.ps f9, 18(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_3:\n"
			"fmulhu.pi f0, f29, f23\n"
			"fmax.pi f1, f0, f1\n"
			"fcvt.ps.f16 f7, f30\n"
			"fcvt.pwu.ps f29, f29, rdn\n"
			"ffrc.ps f0, f9\n"
			"fpackrepb.pi f9, f5\n"
			"frcp.ps f7, f22\n"
			"fsgnjx.ps f30, f23, f15\n"
			"fsgnjn.ps f0, f5, f0\n"
			"frcp.ps f8, f5\n"
			"blt x29, x25, 1f\n"
			"1:\n"
			"fmul.s f11, f22, f26, rup\n"
			"bltu x24, x3, 1f\n"
			"1:\n"
			"fmadd.s f19, f30, f26, f23, rdn\n"
			"fcmovm.ps f7, f0, f7\n"
			"fswizz.ps f22, f30, 0x0\n"
			"fsub.s f5, f19, f26, rup\n"
			"feq.pi f15, f7, f9\n"
			"bgeu x12, x12, 1f\n"
			"1:\n"
			"fcmov.ps f8, f7, f23, f7\n"
			"bltu x27, x22, 1f\n"
			"1:\n"
			"fsatu8.pi f14, f13\n"
			"fslli.pi f1, f1, 0xa\n"
			"fxor.pi f8, f22, f7\n"
			"faddi.pi f13, f8, 0xe0\n"
			"fcvt.ps.f16 f9, f23\n"
			"fround.ps f11, f11, rdn\n"
			"bgeu x26, x15, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f29, f29\n"
			"bltu x28, x12, 1f\n"
			"1:\n"
			"fsatu8.pi f15, f15\n"
			"fadd.ps f1, f1, f15, rup\n"
			"fsgnjx.s f9, f22, f14\n"
			"fround.ps f30, f19, rmm\n"
			"fsgnjx.ps f29, f29, f13\n"
			"bge x23, x28, 1f\n"
			"1:\n"
			"fsatu8.pi f0, f30\n"
			"fcvt.f16.ps f15, f0\n"
			"fsgnj.ps f29, f15, f30\n"
			"fcvt.f16.ps f13, f22\n"
			"fcvt.ps.pw f15, f8, rmm\n"
			"bge x12, x31, 1f\n"
			"1:\n"
			"fround.ps f29, f22, rne\n"
			"bne x5, x9, 1f\n"
			"1:\n"
			"fpackrepb.pi f19, f11\n"
			"fclass.ps f19, f22\n"
			"fmsub.ps f19, f11, f15, f5, rtz\n"
			"feq.pi f5, f7, f0\n"
			"fmadd.s f1, f30, f1, f15, rup\n"
			"fsll.pi f15, f8, f1\n"
			"faddi.pi f30, f26, 0x180\n"
			"fround.ps f15, f1, rmm\n"
			"fpackrepb.pi f0, f0\n"
			"fsgnjx.s f30, f19, f8\n"
			"fsrl.pi f14, f30, f26\n"
			"bltu x0, x20, 1f\n"
			"1:\n"
			"fmul.ps f11, f26, f15, rne\n"
			"fsatu8.pi f5, f5\n"
			"fsll.pi f8, f1, f29\n"
			"fcvt.f16.ps f26, f14\n"
			"bltu x7, x12, 1f\n"
			"1:\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_2_LOOP_SEQ_HID_3\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f21,  704 (x5)\n"
			"flw.ps f20,  608 (x5)\n"
			"flw.ps f29,  480 (x5)\n"
			"flw.ps f6,  160 (x5)\n"
			"flw.ps f7,  864 (x5)\n"
			"flw.ps f11,  384 (x5)\n"
			"flw.ps f19,  544 (x5)\n"
			"flw.ps f17,  640 (x5)\n"
			"flw.ps f23,  736 (x5)\n"
			"flw.ps f5,  352 (x5)\n"
			"flw.ps f15,  224 (x5)\n"
			"flw.ps f0,  832 (x5)\n"
			"flw.ps f12,  128 (x5)\n"
			"flw.ps f4,  448 (x5)\n"
			"flw.ps f9,  928 (x5)\n"
			"flw.ps f26,  672 (x5)\n"
			"flw.ps f14,  96 (x5)\n"
			"flw.ps f1,  960 (x5)\n"
			"flw.ps f16,  896 (x5)\n"
			"flw.ps f31,  192 (x5)\n"
			"flw.ps f13,  768 (x5)\n"
			"flw.ps f2,  64 (x5)\n"
			"flw.ps f10,  32 (x5)\n"
			"flw.ps f3,  256 (x5)\n"
			"flw.ps f28,  800 (x5)\n"
			"flw.ps f27,  288 (x5)\n"
			"flw.ps f22,  992 (x5)\n"
			"flw.ps f24,  0 (x5)\n"
			"flw.ps f8,  512 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f25,  320 (x5)\n"
			"flw.ps f30,  576 (x5)\n"
			"la x31, check_sum_hid_3\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"LBL_POSTAMBLE_HID_3:\n"          
			"csrwi   tensor_wait, 0x0\n"
			"csrwi   tensor_wait, 0x1\n"
			"csrwi   tensor_wait, 0x2\n"
			"csrwi   tensor_wait, 0x3\n"
			"csrwi   tensor_wait, 0x4\n"
			"csrwi   tensor_wait, 0x5\n"
			"csrwi   tensor_wait, 0x6\n"
			"csrwi   tensor_wait, 0x7\n"
			"csrwi   tensor_wait, 0x8\n"
			"csrwi   tensor_wait, 0x9\n"
			"csrwi   tensor_wait, 0xa\n"
			"fence\n"
			"LBL_SYNC_POINT_1_HID_3:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_1_WAIT_HID_3\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_1_EXIT_HID_3\n"
			"LBL_SYNC_POINT_1_WAIT_HID_3:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_1_EXIT_HID_3:\n"          
			"nop\n"
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_3\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_3:\n"          
			".global LBL_FAIL_HID_3\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_3\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_3:\n"          
			".global LBL_PASS_HID_3\n"          
			VSNIP_RSV
		);
		c_test_pass();
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_3:\n"          
			".global LBL_HALT_HID_3\n"          
			"wfi\n"
			"j LBL_HALT_HID_3\n"          
			VSNIP_RSV
		);
		return 0;       
	};
	if ( hid == 4 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_4:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_4\n"
			"csrw mscratch, x5\n"
			"csrwi menable_shadows, 0xf\n"
			"li x5, -1\n"
			"csrw mcounteren, x5\n"
			"csrw scounteren, x5\n"
			"csrwi mhpmcounter3, 0\n"
			"csrwi mhpmcounter4, 0\n"
			"csrwi mhpmcounter5, 0\n"
			"csrwi mhpmcounter6, 0\n"
			"csrwi mhpmcounter7, 0\n"
			"csrwi mhpmcounter8, 0\n"
			"csrwi mhpmevent3, 21\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 28\n"   
			"csrwi mhpmevent6, 20\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 6\n"   
			"mov.m.x m0, x0, 0xff\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f8,  0 (x5)\n"
			"flw.ps f5,  704 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f3,  288 (x5)\n"
			"flw.ps f10,  608 (x5)\n"
			"flw.ps f24,  160 (x5)\n"
			"flw.ps f26,  640 (x5)\n"
			"flw.ps f30,  544 (x5)\n"
			"flw.ps f15,  192 (x5)\n"
			"flw.ps f13,  768 (x5)\n"
			"flw.ps f7,  384 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f4,  128 (x5)\n"
			"flw.ps f14,  928 (x5)\n"
			"flw.ps f9,  512 (x5)\n"
			"flw.ps f28,  352 (x5)\n"
			"flw.ps f6,  256 (x5)\n"
			"flw.ps f23,  960 (x5)\n"
			"flw.ps f19,  32 (x5)\n"
			"flw.ps f1,  896 (x5)\n"
			"flw.ps f25,  864 (x5)\n"
			"flw.ps f31,  800 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f17,  736 (x5)\n"
			"flw.ps f0,  992 (x5)\n"
			"flw.ps f21,  832 (x5)\n"
			"flw.ps f2,  96 (x5)\n"
			"flw.ps f12,  224 (x5)\n"
			"flw.ps f29,  480 (x5)\n"
			"flw.ps f16,  672 (x5)\n"
			"flw.ps f20,  448 (x5)\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_HID_4:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_0_WAIT_HID_4\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_0_EXIT_HID_4\n"
			"LBL_SYNC_POINT_0_WAIT_HID_4:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_EXIT_HID_4:\n"          
			"nop\n"
			"LBL_KERNEL_HID_4:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_4\n"
			"li x5, 0x2\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_HID_4:\n"
			"li x20, 0x807703e030\n"
			"amoaddl.w x13, x20, (x20)\n" 
			"li x26, 0x8035dae8d4\n"
			"sd x29, 512(x26)\n"
			"lh x26, 512(x26)\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f31, 96(x20)\n"
			"flw.ps f11, 320(x20)\n"
			"feq.ps f7, f31, f11\n" 
			"li x12, 0x80712da000\n"
			"amominl.d x15, x22, (x12)\n" 
			"li x15, 0x8033c923e8\n"
			"fsq2 f31, -128(x15)\n" 
			"li x5, 0x8074ff4020\n"
			"la x29, mem_ptr_offset\n"
			"flw.ps f7, 832(x29)\n"
			"famominl.pi f12, f7 (x5)\n" 
			"li x13, 0x8070b3a010\n"
			"la x29, mem_ptr_offset\n"
			"flw.ps f7, 384(x29)\n"
			"famoaddl.pi f26, f7 (x13)\n" 
			"li x5, 0x4e77\n"
			"csrw tensor_mask, x5\n"	
			"la x20, rand_int_32\n"
			"flw.ps f30, 672(x20)\n"
			"flw.ps f26, 768(x20)\n"
			"fmulhu.pi f29, f30, f26\n" 
			"li x22, 0x806727f000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f10, (x22)\n"
			"flwg.ps f25, (x22)\n" 
			"li x5, 0xb69d\n"
			"csrw tensor_mask, x5\n"	
			"li x29, 0x807792e028\n"
			"amoswapl.w x12, x18, (x29)\n" 
			"li x13, 0x80334a1688\n"
			"li x18, 3461818267\n"
			"fg32b.ps f6, x5 (x13)\n" 
			"li x21, 0x8035d53cbb\n"
			"sd x26, -256(x21)\n" 
			"li x18, 0x8037f7e5d9\n"
			"sh x13, -128(x18)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"li x20, 0x803015b3c8\n"
			"li x26, 2113288521\n"
			"fg32b.ps f12, x13 (x20)\n" 
			"li x20, 0x80304b70e8\n"
			"la x26, mem_ptr_offset\n"
			"flw.ps f15, 192(x26)\n"
			"fgb.ps f10, f15 (x20)\n" 
			"li x29, 0x80702e4018\n"
			"la x15, mem_ptr_offset\n"
			"flw.ps f10, 224(x15)\n"
			"famoxorl.pi f30, f10 (x29)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SYNC_POINT_1_HID_4:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_1_WAIT_HID_4\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_1_EXIT_HID_4\n"
			"LBL_SYNC_POINT_1_WAIT_HID_4:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_1_EXIT_HID_4:\n"          
			"nop\n"
			"LBL_SYNC_POINT_2_HID_4:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_2_WAIT_HID_4\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_2_EXIT_HID_4\n"
			"LBL_SYNC_POINT_2_WAIT_HID_4:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_2_EXIT_HID_4:\n"          
			"nop\n"
			"LBL_SEQID_0_TST_ID_0_QUAD128_START_CORE_0_SHIRE_0_HID_4:\n"
			"li x31, (0xc << 4)\n"
			"li x26, 0x841e008020006b20\n"
			"csrw tensor_store, x26\n" 
			"csrwi tensor_wait, 8\n" 
			"LBL_SYNC_POINT_3_HID_4:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_3_WAIT_HID_4\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_3_EXIT_HID_4\n"
			"LBL_SYNC_POINT_3_WAIT_HID_4:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_3_EXIT_HID_4:\n"          
			"nop\n"
			"li x5, 0x7584\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, 0x10c0008030006304\n"
			"li x6, 0x10008030002c07\n"
			"li x7, 0x1a3800000118062\n"
			"li x9, 0x1da4a00000002767\n"
			"li x10, 0x8d51008030000d40\n"
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"li x31, (0x3 << 6)\n"
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_4:\n"
			"la x31, self_check_16\n"
			"li x5, 0x4000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000005\n"
			"add x6, x6, x31\n"
			"li x7, 0x16280000011e023\n"
			"li x9, 0x1b63a00000000766\n"
			"li x10, 0x8b11008030000d40\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xc\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"li x31, (0x2 << 6)\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_2_HID_4:\n"
			"li x5, 0x28a000803000630f\n"
			"li x6, 0x3be0008030006309\n"
			"li x7, 0x1fc80000061f056\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 3\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x8\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f13, 0(x31)\n"
			"flw.ps f3, 0(x31)\n"
			"flw.ps f31, 3(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x20, 0(x31)\n"
			"lw x22, 3(x31)\n"
			"lw x5, 7(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_4:\n"
			"packb x5, x20, x5\n"
			"fle.ps f3, f3, f3\n"
			"fsgnj.s f3, f3, f3\n"
			"fsrl.pi f3, f3, f13\n"
			"bne x18, x8, 1f\n"
			"1:\n"
			"sllw x20, x22, x20\n"
			"slti x18, x22, 0x15\n"
			"fsatu8.pi f3, f3\n"
			"sltu x15, x20, x22\n"
			"maskor m2, m1, m2\n"
			"flt.s x18, f31, f13\n"
			"fmax.s f3, f3, f31\n"
			"fmvz.x.ps x20, f13, 0\n"
			"fmvz.x.ps x20, f3, 7\n"
			"srl x22, x20, x22\n"
			"fsub.s f3, f31, f31, rtz\n"
			"fexp.ps f23, f31\n"
			"fmul.s f13, f13, f3, rne\n"
			"bltu x2, x18, 1f\n"
			"1:\n"
			"ecall\n"
			"ffrc.ps f31, f31\n"
			"srli x18, x22, 2\n"
			"sraiw x15, x22, 1\n"
			"fmadd.ps f31, f13, f13, f31, rtz\n"
			"bltu x10, x9, 1f\n"
			"1:\n"
			"flt.s x26, f3, f13\n"
			"fle.ps f1, f31, f13\n"
			"fslli.pi f13, f3, 0x8\n"
			"fle.ps f31, f31, f3\n"
			"srli x15, x20, 4\n"
			"fsll.pi f1, f31, f3\n"
			"packb x18, x22, x20\n"
			"bltu x8, x25, 1f\n"
			"1:\n"
			"feqm.ps m1, f31, f31\n"
			"srl x5, x5, x5\n"
			"srl x15, x5, x22\n"
			"fpackrepb.pi f23, f31\n"
			"mulw x22, x22, x20\n"
			"maskpopcz x20, m4\n"
			"remu x22, x22, x22\n"
			"fmul.s f13, f13, f13, rup\n"
			"fmvz.x.ps x26, f31, 2\n"
			"fadd.s f13, f13, f3, rne\n"
			"bltu x6, x17, 1f\n"
			"1:\n"
			"fcvt.wu.s x20, f13, rne\n"
			"fcvt.ps.f16 f13, f13\n"
			"rem x20, x20, x5\n"
			"fsgnj.ps f31, f13, f3\n"
			"fround.ps f1, f13, rmm\n"
			"fcmovm.ps f3, f3, f13\n"
			"ori x26, x5, 0x3\n"
			"bgeu x11, x28, 1f\n"
			"1:\n"
			"feqm.ps m2, f13, f31\n"
			"fswizz.ps f31, f31, 0x1f\n"
			"fmvz.x.ps x26, f13, 4\n"
			"fcmov.ps f3, f3, f31, f3\n"
			"fcmov.ps f31, f3, f31, f13\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_0_LOOP_SEQ_HID_4\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_HID_4:\n"
			"li x13, 0x80769f8028\n"
			"amoandl.w x26, x20, (x13)\n" 
			"fsgnj.ps f22, f3, f12\n"
			"fpackreph.pi f6, f13\n"
			"fsrl.pi f31, f22, f10\n"
			"fmax.pi f3, f29, f3\n"
			"fclass.ps f22, f12\n"
			"fminu.pi f31, f13, f10\n"
			"feq.pi f13, f13, f12\n"
			"li x5, 0x8070e04028\n"
			"la x26, mem_ptr_offset\n"
			"flw.ps f7, 224(x26)\n"
			"famominul.pi f31, f7 (x5)\n" 
			"fsgnjn.s f10, f7, f1\n"
			"flt.ps f22, f7, f12\n"
			"bne x2, x22, 1f\n"
			"1:\n"
			"fsgnjn.s f29, f7, f3\n"
			"bge x18, x15, 1f\n"
			"1:\n"
			"fmul.ps f7, f7, f7, rmm\n"
			"fnot.pi f25, f3\n"
			"fmax.pi f3, f3, f3\n"
			"fsgnj.s f7, f7, f25\n"
			"li x18, 0x8076fb7010\n"
			"la x22, mem_ptr_offset\n"
			"flw.ps f10, 288(x22)\n"
			"fghl.ps f11, f10 (x18)\n" 
			"fexp.ps f23, f10\n"
			"flog.ps f23, f12\n"
			"fxor.pi f25, f22, f10\n"
			"fnmadd.ps f22, f12, f23, f12, rdn\n"
			"fsgnjx.s f23, f1, f23\n"
			"fnot.pi f31, f13\n"
			"fltu.pi f26, f11, f30\n"
			"li x18, 0x8075eac000\n"
			"la x27, mem_ptr_offset\n"
			"flw.ps f31, 576(x27)\n"
			"fscbl.ps f12, f31 (x18)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fsgnj.s f13, f30, f25\n"
			"fsub.ps f13, f11, f29, rmm\n"
			"fnmsub.s f6, f10, f31, f1, rup\n"
			"fsub.s f25, f29, f26, rup\n"
			"fsgnjx.s f22, f30, f3\n"
			"fnot.pi f10, f7\n"
			"fcmovm.ps f30, f15, f30\n"
			"li x21, 0x806041b000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f1, (x21)\n"
			"flwg.ps f30, (x21)\n" 
			"fadd.s f22, f30, f13, rtz\n"
			"fadd.ps f10, f29, f29, rne\n"
			"fsat8.pi f29, f11\n"
			"fmulhu.pi f22, f13, f13\n"
			"fcmov.ps f10, f7, f7, f23\n"
			"fsatu8.pi f29, f11\n"
			"fmul.ps f1, f30, f29, rmm\n"
			"li x27, 0x803031d038\n"
			"li x22, 3191151642\n"
			"fg32w.ps f3, x29 (x27)\n" 
			"fle.pi f15, f3, f1\n"
			"fcmov.ps f22, f3, f25, f26\n"
			"fcvt.ps.pw f12, f12, rne\n"
			"for.pi f26, f1, f12\n"
			"fswizz.ps f7, f3, 0x17\n"
			"fsatu8.pi f29, f7\n"
			"fsgnj.s f25, f29, f26\n"
			"beq x27, x17, 1f\n"
			"1:\n"
			"li x22, 0x8065f3e000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f10, (x22)\n" 
			"fminu.pi f7, f7, f30\n"
			"fsgnjx.s f25, f7, f11\n"
			"fcvt.pwu.ps f3, f13, rne\n"
			"blt x14, x29, 1f\n"
			"1:\n"
			"flt.ps f15, f23, f13\n"
			"for.pi f25, f25, f31\n"
			"beq x26, x25, 1f\n"
			"1:\n"
			"fmax.s f23, f29, f11\n"
			"fmaxu.pi f13, f15, f7\n"
			"la x29, rand_ieee754_32\n"
			"flw.ps f31, 672(x29)\n"
			"fsqrt.s f30, f31, rup\n" 
			"sltiu x26, x26, 0x5\n"
			"fclass.s x20, f10\n"
			"fmv.x.s x22, f1\n"
			"fandi.pi f12, f13, 0x1e\n"
			"fslli.pi f31, f1, 0x2\n"
			"fmvz.x.ps x15, f31, 4\n"
			"packb x21, x12, x18\n"
			"bne x14, x29, 1f\n"
			"1:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 352(x5)\n"
			"fexp.ps f29, f25\n" 
			"fmax.s f15, f25, f12\n"
			"fand.pi f30, f6, f12\n"
			"beq x13, x27, 1f\n"
			"1:\n"
			"fsatu8.pi f25, f6\n"
			"fmsub.s f12, f6, f25, f12, rdn\n"
			"feq.ps f23, f11, f6\n"
			"fmulhu.pi f22, f6, f6\n"
			"fmin.s f11, f22, f7\n"
			"divw x21, x22, x27\n" 
			"fltu.pi f12, f6, f12\n"
			"flt.ps f25, f11, f29\n"
			"fmin.pi f11, f25, f29\n"
			"blt x4, x19, 1f\n"
			"1:\n"
			"fmin.s f3, f6, f1\n"
			"fmadd.s f29, f15, f3, f29, rdn\n"
			"fcvt.pw.ps f22, f7, rdn\n"
			"fexp.ps f1, f29\n"
			"bne x11, x7, 1f\n"
			"1:\n"
			"csrr x0, scounteren\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fsub.s f29, f29, f6, rmm\n"
			"blt x9, x14, 1f\n"
			"1:\n"
			"flt.ps f3, f15, f31\n"
			"fmax.ps f31, f3, f10\n"
			"fnmadd.s f29, f10, f23, f25, rmm\n"
			"fmax.pi f29, f31, f10\n"
			"beq x8, x19, 1f\n"
			"1:\n"
			"fcvt.pw.ps f13, f3, rup\n"
			"beq x24, x14, 1f\n"
			"1:\n"
			"feq.pi f22, f22, f11\n"
			"la x18, rand_int_32\n"
			"flw.ps f1, 736(x18)\n"
			"flw.ps f11, 224(x18)\n"
			"fsrl.pi f12, f1, f11\n" 
			"fmin.s f31, f6, f26\n"
			"maskor m6, m1, m1\n"
			"fnmadd.ps f31, f1, f15, f15, rmm\n"
			"fsatu8.pi f1, f30\n"
			"blt x31, x8, 1f\n"
			"1:\n"
			"fcmov.ps f7, f13, f10, f3\n"
			"fcmovm.ps f30, f10, f15\n"
			"masknot m1, m6\n"
			"li x29, 0xf1f4c5849a789fab\n"
			"fcvt.s.w f1, x29, rmm\n" 
			"mova.x.m x29\n"
			"fcmovm.ps f23, f31, f23\n"
			"fslli.pi f26, f29, 0xf\n"
			"xor x21, x21, x21\n"
			"slli x27, x27, 0\n"
			"fnmadd.s f23, f3, f25, f29, rne\n"
			"beq x17, x1, 1f\n"
			"1:\n"
			"fsatu8.pi f12, f3\n"
			"la x15, rand_int_32\n"
			"flw.ps f15, 512(x15)\n"
			"flw.ps f7, 512(x15)\n"
			"fsrl.pi f1, f15, f7\n" 
			"fcmov.ps f10, f10, f1, f1\n"
			"fsetm.pi m2, f11\n"
			"fltm.pi m4, f26, f11\n"
			"beq x21, x19, 1f\n"
			"1:\n"
			"fsgnj.s f31, f15, f31\n"
			"divu x29, x0, x12\n"
			"fmax.s f6, f25, f10\n"
			"fle.ps f12, f12, f30\n"
			"li x5, 0x80371f75a4\n"
			"sd x27, 256(x5)\n"
			"lbu x29, 256(x5)\n" 
			"subw x20, x0, x12\n"
			"fmsub.ps f10, f15, f15, f15, rup\n"
			"fmulhu.pi f10, f22, f10\n"
			"fsll.pi f7, f22, f6\n"
			"bgeu x5, x4, 1f\n"
			"1:\n"
			"addi x13, x27, 0xb\n"
			"fmin.ps f1, f15, f7\n"
			"fltm.ps m6, f11, f15\n"
			"li x21, 0x806188a008\n"
			"la x12, mem_ptr_offset\n"
			"flw.ps f23, 928(x12)\n"
			"fscwg.ps f22, f23 (x21)\n" 
			"fnmsub.s f25, f25, f11, f11, rtz\n"
			"bne x6, x20, 1f\n"
			"1:\n"
			"fxor.pi f3, f23, f23\n"
			"bne x17, x26, 1f\n"
			"1:\n"
			"fxor.pi f26, f29, f31\n"
			"bltu x10, x10, 1f\n"
			"1:\n"
			"fsll.pi f22, f22, f31\n"
			"fsub.s f31, f31, f10, rtz\n"
			"fsra.pi f25, f6, f11\n"
			"fsgnjn.s f22, f6, f22\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_1_TLD_ID_1_NMASK_0x1_CMASK_0xbf_SHIRE_0_HID_4:\n"
			"li x31, 0x1bf0e\n"
			"csrw tensor_coop, x31\n" 
			"li x31, 0x01FF000001FF0000\n"
			"csrw tensor_mask, x31\n" 
			"li x31, (0x3 << 6) | 0x1\n"
			"li x29, 0x4010008020006b0b\n"
			"csrw tensor_load, x29\n" 
			"li x7, 0x19d800000118000\n"
			"csrw tensor_fma, x7\n" 
			"LBL_SYNC_POINT_4_HID_4:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_4_WAIT_HID_4\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_4_EXIT_HID_4\n"
			"LBL_SYNC_POINT_4_WAIT_HID_4:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_4_EXIT_HID_4:\n"          
			"nop\n"
			"LBL_SYNC_POINT_5_HID_4:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_5_WAIT_HID_4\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_5_EXIT_HID_4\n"
			"LBL_SYNC_POINT_5_WAIT_HID_4:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_5_EXIT_HID_4:\n"          
			"nop\n"
			"LBL_SEQID_1_TST_ID_1_QUAD128_START_CORE_0_SHIRE_0_HID_4:\n"
			"li x31, (0x4 << 4)\n"
			"li x29, 0xa25e008020006b20\n"
			"csrw tensor_store, x29\n" 
			"csrwi tensor_wait, 8\n" 
			"LBL_SYNC_POINT_6_HID_4:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_6_WAIT_HID_4\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_6_EXIT_HID_4\n"
			"LBL_SYNC_POINT_6_WAIT_HID_4:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_6_EXIT_HID_4:\n"          
			"nop\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, 0x184000000d3723\n"
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 2\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x10\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f11, 0(x31)\n"
			"flw.ps f13, 2(x31)\n"
			"flw.ps f22, 3(x31)\n"
			"flw.ps f23, 7(x31)\n"
			"flw.ps f25, 5(x31)\n"
			"flw.ps f26, 6(x31)\n"
			"flw.ps f3, 20(x31)\n"
			"flw.ps f31, 13(x31)\n"
			"flw.ps f6, 27(x31)\n"
			"flw.ps f7, 5(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x12, 1(x31)\n"
			"lw x13, 3(x31)\n"
			"lw x15, 2(x31)\n"
			"lw x18, 12(x31)\n"
			"lw x20, 12(x31)\n"
			"lw x21, 21(x31)\n"
			"lw x22, 23(x31)\n"
			"lw x26, 19(x31)\n"
			"lw x27, 3(x31)\n"
			"lw x29, 24(x31)\n"
			"lw x5, 39(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_4:\n"
			"xori x5, x5, 0xa\n"
			"fnmsub.s f11, f7, f13, f7, rmm\n"
			"feq.pi f6, f11, f6\n"
			"bltu x28, x6, 1f\n"
			"1:\n"
			"remw x29, x26, x29\n"
			"beq x10, x2, 1f\n"
			"1:\n"
			"fsgnj.ps f26, f6, f11\n"
			"fle.s x13, f23, f3\n"
			"subw x22, x22, x26\n"
			"slt x12, x12, x26\n"
			"fcvt.pwu.ps f3, f11, rup\n"
			"sub x21, x5, x12\n"
			"fmaxu.pi f7, f23, f26\n"
			"fmul.pi f3, f3, f13\n"
			"bgeu x21, x31, 1f\n"
			"1:\n"
			"divw x29, x29, x15\n"
			"fbcx.ps f11, x0\n"
			"fsub.pi f15, f6, f11\n"
			"bgeu x28, x6, 1f\n"
			"1:\n"
			"maskand m2, m2, m2\n"
			"sub x0, x0, x0\n"
			"fsub.ps f7, f22, f26, rmm\n"
			"fsrli.pi f22, f23, 0xe\n"
			"feq.s x29, f25, f25\n"
			"ebreak\n"
			"mov.m.x m2, x20, 0x7f\n"
			"sllw x12, x21, x18\n"
			"fnmsub.ps f11, f23, f6, f26, rtz\n"
			"blt x30, x19, 1f\n"
			"1:\n"
			"feq.ps f11, f3, f23\n"
			"fadd.pi f1, f23, f25\n"
			"fle.s x0, f7, f25\n"
			"fsatu8.pi f13, f3\n"
			"fsgnj.s f13, f22, f26\n"
			"fclass.s x5, f7\n"
			"bge x8, x5, 1f\n"
			"1:\n"
			"lui x12, 0xe\n"
			"blt x4, x11, 1f\n"
			"1:\n"
			"fmax.s f7, f13, f22\n"
			"fpackreph.pi f11, f31\n"
			"maskor m1, m1, m4\n"
			"fsat8.pi f7, f26\n"
			"bgeu x25, x12, 1f\n"
			"1:\n"
			"sra x27, x20, x29\n"
			"fnot.pi f22, f6\n"
			"bltu x28, x25, 1f\n"
			"1:\n"
			"fmulhu.pi f23, f26, f23\n"
			"fsrli.pi f7, f7, 0xc\n"
			"fbcx.ps f3, x29\n"
			"fsll.pi f15, f11, f26\n"
			"maskpopcz x12, m4\n"
			"ori x12, x5, 0x1e\n"
			"fsub.pi f15, f7, f11\n"
			"beq x27, x8, 1f\n"
			"1:\n"
			"fnmsub.ps f6, f11, f6, f25, rne\n"
			"flt.pi f11, f11, f23\n"
			"fsrli.pi f15, f25, 0x9\n"
			"bne x17, x6, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f23, f7\n"
			"divu x26, x27, x15\n"
			"srai x27, x27, 1\n"
			"bgeu x24, x16, 1f\n"
			"1:\n"
			"mul x12, x27, x20\n"
			"sll x21, x5, x21\n"
			"maskor m1, m4, m2\n"
			"flog.ps f23, f23\n"
			"fbci.ps f23, 0x3f639\n"
			"fmin.pi f11, f3, f13\n"
			"flt.ps f11, f11, f26\n"
			"fltm.pi m4, f31, f13\n"
			"maskand m4, m1, m4\n"
			"slliw x18, x5, 1\n"
			"fsra.pi f7, f13, f26\n"
			"fcvt.ps.f16 f15, f23\n"
			"beq x27, x6, 1f\n"
			"1:\n"
			"fxor.pi f11, f11, f3\n"
			"srai x13, x13, 4\n"
			"mova.x.m x5\n"
			"srliw x15, x0, 2\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_1_LOOP_SEQ_HID_4\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"la x31, check_sum_hid_4\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"LBL_POSTAMBLE_HID_4:\n"          
			"csrwi   tensor_wait, 0x0\n"
			"csrwi   tensor_wait, 0x1\n"
			"csrwi   tensor_wait, 0x2\n"
			"csrwi   tensor_wait, 0x3\n"
			"csrwi   tensor_wait, 0x4\n"
			"csrwi   tensor_wait, 0x5\n"
			"csrwi   tensor_wait, 0x6\n"
			"csrwi   tensor_wait, 0x7\n"
			"csrwi   tensor_wait, 0x8\n"
			"csrwi   tensor_wait, 0x9\n"
			"csrwi   tensor_wait, 0xa\n"
			"fence\n"
			"LBL_SYNC_POINT_7_HID_4:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_7_WAIT_HID_4\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_7_EXIT_HID_4\n"
			"LBL_SYNC_POINT_7_WAIT_HID_4:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_7_EXIT_HID_4:\n"          
			"nop\n"
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_4\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_4:\n"          
			".global LBL_FAIL_HID_4\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_4\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_4:\n"          
			".global LBL_PASS_HID_4\n"          
			VSNIP_RSV
		);
		c_test_pass();
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_4:\n"          
			".global LBL_HALT_HID_4\n"          
			"wfi\n"
			"j LBL_HALT_HID_4\n"          
			VSNIP_RSV
		);
		return 0;       
	};
	if ( hid == 5 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_5:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_5\n"
			"csrw mscratch, x5\n"
			"csrwi menable_shadows, 0xf\n"
			"li x5, -1\n"
			"csrw mcounteren, x5\n"
			"csrw scounteren, x5\n"
			"csrwi mhpmcounter3, 0\n"
			"csrwi mhpmcounter4, 0\n"
			"csrwi mhpmcounter5, 0\n"
			"csrwi mhpmcounter6, 0\n"
			"csrwi mhpmcounter7, 0\n"
			"csrwi mhpmcounter8, 0\n"
			"csrwi mhpmevent3, 7\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 9\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 23\n"   
			"mov.m.x m0, x0, 0xff\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f13,  704 (x5)\n"
			"flw.ps f3,  576 (x5)\n"
			"flw.ps f15,  640 (x5)\n"
			"flw.ps f5,  928 (x5)\n"
			"flw.ps f14,  384 (x5)\n"
			"flw.ps f21,  288 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f31,  192 (x5)\n"
			"flw.ps f8,  544 (x5)\n"
			"flw.ps f9,  800 (x5)\n"
			"flw.ps f17,  736 (x5)\n"
			"flw.ps f24,  160 (x5)\n"
			"flw.ps f26,  32 (x5)\n"
			"flw.ps f25,  64 (x5)\n"
			"flw.ps f18,  256 (x5)\n"
			"flw.ps f1,  512 (x5)\n"
			"flw.ps f20,  480 (x5)\n"
			"flw.ps f19,  416 (x5)\n"
			"flw.ps f28,  672 (x5)\n"
			"flw.ps f12,  128 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f11,  448 (x5)\n"
			"flw.ps f10,  960 (x5)\n"
			"flw.ps f16,  608 (x5)\n"
			"flw.ps f29,  0 (x5)\n"
			"flw.ps f0,  768 (x5)\n"
			"flw.ps f30,  96 (x5)\n"
			"flw.ps f6,  896 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f23,  864 (x5)\n"
			"flw.ps f27,  832 (x5)\n"
			"flw.ps f2,  224 (x5)\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_HID_5:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_0_WAIT_HID_5\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_0_EXIT_HID_5\n"
			"LBL_SYNC_POINT_0_WAIT_HID_5:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_EXIT_HID_5:\n"          
			"nop\n"
			"LBL_KERNEL_HID_5:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_5\n"
			"li x5, 0x2\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_HID_5:\n"
			"la x21, rand_ieee754_32\n"
			"flw.ps f4, 544(x21)\n"
			"fexp.ps f31, f4\n" 
			"sraiw x19, x24, 0\n"
			"fsgnjn.ps f31, f29, f16\n"
			"bltu x23, x31, 1f\n"
			"1:\n"
			"fpackrepb.pi f5, f16\n"
			"bgeu x26, x8, 1f\n"
			"1:\n"
			"flt.pi f2, f29, f20\n"
			"sra x25, x21, x9\n"
			"mulhu x21, x14, x21\n"
			"bne x29, x6, 1f\n"
			"1:\n"
			"ori x12, x21, 0x1b\n"
			"rem x14, x12, x14\n"
			"fsgnjn.ps f29, f29, f29\n"
			"sltiu x12, x9, 0x13\n"
			"li x16, 0x803f6c678e\n"
			"sb x12, -512(x16)\n" 
			"feq.ps f24, f28, f16\n"
			"fmax.s f5, f2, f1\n"
			"fnmadd.s f20, f28, f12, f5, rne\n"
			"beq x18, x25, 1f\n"
			"1:\n"
			"fnmsub.s f24, f26, f24, f28, rtz\n"
			"fmaxu.pi f23, f5, f23\n"
			"fxor.pi f28, f23, f4\n"
			"fsub.pi f28, f12, f1\n"
			"feq.ps f4, f12, f26\n"
			"fmin.s f28, f16, f28\n"
			"fnmsub.ps f5, f24, f20, f20, rdn\n"
			"li x10, 0x803a8b08dc\n"
			"sw x21, 128(x10)\n" 
			"fnot.pi f2, f29\n"
			"fmul.pi f29, f29, f4\n"
			"fmul.s f29, f4, f29, rne\n"
			"feq.ps f29, f29, f29\n"
			"fcvt.pw.ps f26, f4, rdn\n"
			"fcvt.pwu.ps f4, f4, rdn\n"
			"fslli.pi f4, f24, 0xd\n"
			"bne x20, x2, 1f\n"
			"1:\n"
			"fmin.s f29, f4, f24\n"
			"fsll.pi f4, f29, f29\n"
			"bge x15, x13, 1f\n"
			"1:\n"
			"fcvt.f16.ps f7, f4\n"
			"bltu x9, x13, 1f\n"
			"1:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 800(x20)\n"
			"fmvz.x.ps x24, f25, 1\n" 
			"fcvt.pwu.ps f12, f1, rdn\n"
			"fexp.ps f31, f20\n"
			"fmax.s f13, f4, f23\n"
			"frcp.ps f29, f25\n"
			"fmax.ps f28, f25, f31\n"
			"for.pi f2, f2, f2\n"
			"fsub.pi f16, f4, f20\n"
			"bne x19, x26, 1f\n"
			"1:\n"
			"for.pi f20, f20, f29\n"
			"beq x4, x26, 1f\n"
			"1:\n"
			"frcp.ps f5, f23\n"
			"bge x21, x8, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f4, f1, rne\n"
			"li x12, 0x8038bb05a8\n"
			"la x27, mem_ptr_offset\n"
			"flw.ps f25, 192(x27)\n"
			"fgh.ps f26, f25 (x12)\n" 
			"fcmov.ps f24, f24, f24, f24\n"
			"fcvt.wu.s x25, f23, rtz\n"
			"fswizz.ps f31, f23, 0x13\n"
			"fcvt.s.w f26, x12, rne\n"
			"fmulh.pi f24, f24, f23\n"
			"fmulh.pi f24, f24, f23\n"
			"bne x16, x20, 1f\n"
			"1:\n"
			"sraiw x24, x24, 5\n"
			"beq x16, x1, 1f\n"
			"1:\n"
			"fle.ps f16, f2, f2\n"
			"fmul.ps f23, f24, f2, rtz\n"
			"remw x24, x24, x24\n"
			"li x24, 0x98da788499857e47\n"
			"srliw x16, x24, 5\n" 
			"fclass.ps f12, f24\n"
			"bne x11, x9, 1f\n"
			"1:\n"
			"fmax.ps f16, f25, f1\n"
			"divw x20, x9, x16\n"
			"ffrc.ps f13, f1\n"
			"fswizz.ps f23, f25, 0x10\n"
			"fmax.ps f25, f1, f16\n"
			"sll x20, x20, x9\n"
			"feq.ps f12, f25, f23\n"
			"bge x8, x21, 1f\n"
			"1:\n"
			"for.pi f23, f23, f25\n"
			"packb x9, x23, x9\n"
			"li x24, 0x803f5dc2a8\n"
			"fsw.ps f24, -256(x24)\n"
			"flw.ps f24, -256(x24)\n" 
			"fandi.pi f31, f24, 0x33\n"
			"flog.ps f2, f24\n"
			"fsgnj.ps f12, f31, f12\n"
			"bgeu x28, x4, 1f\n"
			"1:\n"
			"fexp.ps f13, f1\n"
			"fnmadd.ps f31, f24, f24, f24, rne\n"
			"for.pi f2, f26, f24\n"
			"fsll.pi f31, f1, f31\n"
			"bltu x21, x10, 1f\n"
			"1:\n"
			"fsgnjn.ps f31, f31, f26\n"
			"fmaxu.pi f1, f12, f1\n"
			"bltu x16, x11, 1f\n"
			"1:\n"
			"fnmsub.s f31, f12, f31, f31, rtz\n"
			"li x27, 0x806ed18008\n"
			"la x19, mem_ptr_offset\n"
			"flw.ps f5, 192(x19)\n"
			"fghg.ps f20, f5 (x27)\n" 
			"fcvt.ps.f16 f12, f12\n"
			"fcvt.ps.pw f4, f26, rdn\n"
			"fmin.ps f16, f7, f16\n"
			"fmul.s f16, f26, f1, rtz\n"
			"fsrli.pi f12, f31, 0x4\n"
			"bge x31, x31, 1f\n"
			"1:\n"
			"fmulhu.pi f5, f20, f7\n"
			"fround.ps f12, f12, rne\n"
			"fminu.pi f12, f16, f12\n"
			"fnmsub.s f16, f31, f7, f12, rne\n"
			"fand.pi f16, f16, f20\n"
			"li x10, 0x807d8a4024\n"
			"amoorl.w x16, x9, (x10)\n" 
			"mulh x16, x20, x16\n"
			"blt x15, x18, 1f\n"
			"1:\n"
			"sra x9, x9, x19\n"
			"remuw x14, x16, x19\n"
			"bge x21, x22, 1f\n"
			"1:\n"
			"fsub.pi f4, f29, f7\n"
			"fsra.pi f2, f13, f29\n"
			"slliw x27, x20, 1\n"
			"fsgnjx.ps f2, f2, f4\n"
			"fmax.s f7, f1, f13\n"
			"fpackrepb.pi f1, f4\n"
			"fadd.pi f29, f2, f7\n"
			"la x20, rand_int_32\n"
			"flw.ps f13, 768(x20)\n"
			"fslli.pi f5, f13, 0x6\n" 
			"fmin.s f23, f23, f23\n"
			"fmul.s f4, f1, f20, rdn\n"
			"flem.ps m0, f25, f1\n"
			"mov.m.x m0, x0, 0xff\n"
			"blt x20, x22, 1f\n"
			"1:\n"
			"slt x20, x21, x16\n"
			"ecall\n"
			"or x20, x25, x10\n"
			"addi x23, x9, 0x4\n"
			"mova.x.m x9\n"
			"fandi.pi f16, f13, 0x18d\n"
			"xor x16, x9, x16\n"
			"bge x26, x31, 1f\n"
			"1:\n"
			"li x10, 0x803eb3197c\n"
			"sd x20, -512(x10)\n"
			"ld x25, -512(x10)\n" 
			"ebreak\n"
			"fexp.ps f28, f31\n"
			"add x23, x23, x24\n"
			"bge x18, x16, 1f\n"
			"1:\n"
			"or x21, x24, x23\n"
			"fmax.pi f25, f31, f25\n"
			"bgeu x30, x16, 1f\n"
			"1:\n"
			"feq.ps f1, f31, f29\n"
			"fcvt.s.wu f2, x24, rmm\n"
			"bne x10, x1, 1f\n"
			"1:\n"
			"feq.ps f29, f13, f29\n"
			"fmulhu.pi f25, f4, f13\n"
			"blt x21, x8, 1f\n"
			"1:\n"
			"mov.m.x m0, x24, 0x11\n"
			"mov.m.x m0, x0, 0xff\n"
			"li x19, 0x807be2f034\n"
			"amoxorl.w x10, x20, (x19)\n" 
			"fmin.s f29, f20, f1\n"
			"frcp.ps f1, f2\n"
			"bltu x7, x12, 1f\n"
			"1:\n"
			"fxor.pi f31, f12, f28\n"
			"fcvt.pw.ps f26, f20, rtz\n"
			"fsat8.pi f7, f7\n"
			"fsgnjn.ps f26, f20, f28\n"
			"fcvt.ps.f16 f16, f7\n"
			"fnmadd.s f28, f28, f2, f26, rne\n"
			"fxor.pi f13, f2, f23\n"
			"fcvt.ps.pw f26, f2, rtz\n"
			"li x12, 0x803a0bae58\n"
			"la x10, mem_ptr_offset\n"
			"flw.ps f4, 768(x10)\n"
			"fgh.ps f24, f4 (x12)\n" 
			"fmaxu.pi f5, f20, f26\n"
			"maskpopc x12, m1\n"
			"xor x10, x23, x16\n"
			"fsgnjn.ps f26, f2, f26\n"
			"sraiw x23, x23, 5\n"
			"fltm.pi m1, f4, f12\n"
			"blt x16, x27, 1f\n"
			"1:\n"
			"fmaxu.pi f5, f5, f5\n"
			"remw x9, x21, x20\n"
			"lui x16, 0x6\n"
			"ffrc.ps f1, f1\n"
			"li x20, 0x806fa3c028\n"
			"la x9, mem_ptr_offset\n"
			"flw.ps f23, 160(x9)\n"
			"famomaxug.pi f25, f23 (x20)\n" 
			"remw x16, x16, x27\n"
			"fcmov.ps f20, f31, f7, f20\n"
			"sraiw x19, x16, 0\n"
			"fcvt.f16.ps f2, f4\n"
			"fsub.s f31, f28, f28, rtz\n"
			"fcvt.f16.ps f2, f28\n"
			"bltu x13, x0, 1f\n"
			"1:\n"
			"fsgnjn.s f2, f28, f29\n"
			"fsgnj.s f31, f7, f7\n"
			"feq.s x16, f31, f4\n"
			"mul x25, x25, x25\n"
			"li x14, 0x8079b13038\n"
			"amoandl.w x21, x10, (x14)\n" 
			"fcvt.pwu.ps f23, f2, rmm\n"
			"fslli.pi f2, f1, 0xe\n"
			"bltu x8, x27, 1f\n"
			"1:\n"
			"fsrl.pi f1, f12, f23\n"
			"fsub.pi f13, f13, f13\n"
			"fslli.pi f4, f23, 0xc\n"
			"bne x7, x23, 1f\n"
			"1:\n"
			"fmul.ps f1, f23, f31, rtz\n"
			"flog.ps f2, f26\n"
			"fexp.ps f28, f12\n"
			"fslli.pi f7, f16, 0x7\n"
			"bge x19, x6, 1f\n"
			"1:\n"
			"fnmsub.ps f4, f29, f1, f4, rtz\n"
			"li x20, 0x807b473008\n"
			"la x23, mem_ptr_offset\n"
			"flw.ps f7, 928(x23)\n"
			"famoxorl.pi f4, f7 (x20)\n" 
			"fmul.ps f29, f7, f20, rmm\n"
			"fnmadd.s f24, f7, f7, f20, rne\n"
			"fmsub.s f23, f20, f20, f16, rdn\n"
			"fmaxu.pi f1, f7, f20\n"
			"fcvt.pw.ps f29, f16, rmm\n"
			"fsrai.pi f1, f16, 0x4\n"
			"fnmadd.ps f23, f16, f16, f20, rne\n"
			"fswizz.ps f1, f16, 0x12\n"
			"fadd.ps f23, f7, f7, rtz\n"
			"fcmovm.ps f1, f7, f16\n"
			"bgeu x8, x21, 1f\n"
			"1:\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x10\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f16, 0(x31)\n"
			"flw.ps f20, 1(x31)\n"
			"flw.ps f24, 5(x31)\n"
			"flw.ps f7, 0(x31)\n"
			"li x31, 0x2\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_5:\n"
			"fsatu8.pi f31, f20\n"
			"fmul.pi f24, f24, f24\n"
			"bge x22, x30, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f31, f20, rmm\n"
			"fnmsub.ps f31, f24, f20, f7, rdn\n"
			"fltu.pi f28, f16, f24\n"
			"fand.pi f28, f24, f20\n"
			"fsrai.pi f7, f20, 0x1\n"
			"flt.ps f16, f20, f16\n"
			"bne x20, x19, 1f\n"
			"1:\n"
			"fclass.ps f7, f24\n"
			"fcvt.f16.ps f7, f7\n"
			"bgeu x23, x31, 1f\n"
			"1:\n"
			"fswizz.ps f7, f7, 0x8\n"
			"bgeu x28, x29, 1f\n"
			"1:\n"
			"fmul.ps f28, f7, f24, rtz\n"
			"fcmovm.ps f2, f16, f7\n"
			"fsub.pi f16, f16, f24\n"
			"fnmsub.ps f16, f16, f16, f24, rdn\n"
			"fnmadd.s f24, f20, f7, f7, rtz\n"
			"fadd.s f20, f20, f24, rtz\n"
			"fmin.s f31, f24, f16\n"
			"for.pi f7, f7, f7\n"
			"fmadd.s f2, f16, f24, f24, rne\n"
			"ffrc.ps f7, f7\n"
			"bge x7, x13, 1f\n"
			"1:\n"
			"flt.pi f24, f16, f16\n"
			"frcp.ps f28, f24\n"
			"bgeu x27, x15, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f2, f7, rne\n"
			"fand.pi f31, f20, f20\n"
			"blt x5, x15, 1f\n"
			"1:\n"
			"fround.ps f16, f16, rtz\n"
			"feq.pi f24, f24, f7\n"
			"frcp.ps f2, f24\n"
			"fltu.pi f24, f24, f24\n"
			"beq x14, x23, 1f\n"
			"1:\n"
			"fltu.pi f16, f24, f16\n"
			"blt x4, x2, 1f\n"
			"1:\n"
			"fsgnjn.s f7, f20, f20\n"
			"fmul.ps f28, f16, f7, rmm\n"
			"blt x8, x7, 1f\n"
			"1:\n"
			"faddi.pi f16, f20, 0x180\n"
			"fnmadd.ps f24, f24, f16, f24, rtz\n"
			"fpackreph.pi f2, f20\n"
			"fmax.pi f16, f24, f20\n"
			"fcvt.f16.ps f28, f16\n"
			"bltu x8, x18, 1f\n"
			"1:\n"
			"fclass.ps f20, f7\n"
			"fmin.ps f16, f16, f7\n"
			"bgeu x26, x13, 1f\n"
			"1:\n"
			"fmadd.s f31, f7, f20, f24, rdn\n"
			"flt.ps f24, f7, f7\n"
			"beq x21, x26, 1f\n"
			"1:\n"
			"flt.ps f28, f7, f24\n"
			"flog.ps f24, f7\n"
			"flog.ps f2, f16\n"
			"faddi.pi f16, f20, 0x1de\n"
			"fcvt.pw.ps f16, f16, rup\n"
			"fmax.ps f31, f20, f24\n"
			"flog.ps f24, f16\n"
			"fxor.pi f2, f7, f7\n"
			"ffrc.ps f31, f7\n"
			"fsgnjn.ps f24, f7, f20\n"
			"fltu.pi f16, f24, f16\n"
			"bltu x6, x19, 1f\n"
			"1:\n"
			"fminu.pi f24, f24, f7\n"
			"fle.pi f7, f24, f7\n"
			"fmulh.pi f24, f20, f16\n"
			"fminu.pi f24, f24, f24\n"
			"fmax.s f24, f7, f16\n"
			"bge x0, x9, 1f\n"
			"1:\n"
			"fclass.ps f28, f16\n"
			"fmadd.ps f31, f7, f7, f20, rmm\n"
			"fcmov.ps f2, f24, f20, f7\n"
			"fnmadd.ps f7, f24, f7, f20, rtz\n"
			"frcp.ps f31, f7\n"
			"blt x7, x19, 1f\n"
			"1:\n"
			"fmul.ps f16, f20, f16, rdn\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_0_LOOP_SEQ_HID_5\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_HID_5:\n"
			"li x20, 0x807cdc8014\n"
			"amominul.w x9, x10, (x20)\n" 
			"fmsub.s f26, f7, f29, f28, rne\n"
			"fsra.pi f28, f28, f29\n"
			"bne x15, x15, 1f\n"
			"1:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f13, 608(x14)\n"
			"flw.ps f12, 128(x14)\n"
			"fmul.ps f24, f13, f12, rne\n" 
			"fcvt.ps.pw f20, f16, rmm\n"
			"bne x16, x19, 1f\n"
			"1:\n"
			"mulw x16, x27, x19\n"
			"li x16, 0x9bc025c3c0ea2adc\n"
			"fcvt.s.w f31, x16, rup\n" 
			"fswizz.ps f28, f5, 0x7\n"
			"mulw x10, x12, x20\n"
			"li x23, 0xa4ec0261f7aacd71\n"
			"slli x24, x23, 5\n" 
			"div x19, x19, x16\n"
			"bge x7, x20, 1f\n"
			"1:\n"
			"ebreak\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f31, 608(x25)\n"
			"flw.ps f5, 608(x25)\n"
			"fmin.s f31, f31, f5\n" 
			"mulh x12, x20, x19\n"
			"sltu x24, x10, x27\n"
			"li x16, 0x807d06b018\n"
			"amoaddl.d x20, x19, (x16)\n" 
			"fmul.s f26, f2, f16, rtz\n"
			"bgeu x24, x3, 1f\n"
			"1:\n"
			"for.pi f24, f28, f5\n"
			"li x10, 0x806c135024\n"
			"amoaddg.w x12, x21, (x10)\n" 
			"fand.pi f13, f7, f5\n"
			"fadd.s f4, f4, f20, rdn\n"
			"li x25, 0x807b20c020\n"
			"la x12, mem_ptr_offset\n"
			"flw.ps f20, 960(x12)\n"
			"fscbl.ps f24, f20 (x25)\n" 
			"flog.ps f31, f20\n"
			"fsgnj.s f1, f4, f1\n"
			"beq x3, x25, 1f\n"
			"1:\n"
			"li x16, 0x807a5de018\n"
			"la x24, mem_ptr_offset\n"
			"flw.ps f1, 992(x24)\n"
			"fscwl.ps f12, f1 (x16)\n" 
			"fmax.pi f5, f12, f12\n"
			"fsat8.pi f5, f7\n"
			"li x23, 0x806f269020\n"
			"amomaxug.d x23, x9, (x23)\n" 
			"fmul.ps f26, f4, f25, rtz\n"
			"fcvt.ps.pwu f25, f25, rdn\n"
			"li x10, 0x806ae6d030\n"
			"la x23, mem_ptr_offset\n"
			"flw.ps f26, 256(x23)\n"
			"famoorg.pi f23, f26 (x10)\n" 
			"faddi.pi f5, f25, 0x194\n"
			"flog.ps f25, f12\n"
			"li x25, 0x806b288038\n"
			"la x16, mem_ptr_offset\n"
			"flw.ps f7, 768(x16)\n"
			"famominug.pi f26, f7 (x25)\n" 
			"fpackreph.pi f1, f12\n"
			"fsra.pi f28, f25, f13\n"
			"li x19, 0x806a9dc028\n"
			"la x12, mem_ptr_offset\n"
			"flw.ps f24, 736(x12)\n"
			"fgwg.ps f4, f24 (x19)\n" 
			"feq.pi f7, f5, f7\n"
			"maskand m5, m6, m6\n"
			"li x14, 0x807922903c\n"
			"amoaddl.w x21, x16, (x14)\n" 
			"fpackrepb.pi f31, f23\n"
			"fcvt.ps.pwu f4, f4, rup\n"
			"bne x22, x25, 1f\n"
			"1:\n"
			"la x10, rand_int_32\n"
			"flw.ps f31, 576(x10)\n"
			"for.pi f23, f31, f31\n" 
			"fsgnj.ps f7, f23, f4\n"
			"fmin.s f23, f1, f4\n"
			"li x14, 0x8038c56e2d\n"
			"sd x9, -512(x14)\n"
			"lw x24, -512(x14)\n" 
			"faddi.pi f16, f1, 0x13\n"
			"fsrai.pi f25, f31, 0x3\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x40\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f12, 0(x31)\n"
			"flw.ps f13, 4(x31)\n"
			"flw.ps f20, 5(x31)\n"
			"flw.ps f23, 6(x31)\n"
			"flw.ps f25, 7(x31)\n"
			"flw.ps f26, 16(x31)\n"
			"flw.ps f28, 8(x31)\n"
			"flw.ps f29, 26(x31)\n"
			"flw.ps f5, 12(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x10, 0(x31)\n"
			"lw x12, 4(x31)\n"
			"lw x14, 3(x31)\n"
			"lw x16, 2(x31)\n"
			"lw x19, 0(x31)\n"
			"lw x20, 19(x31)\n"
			"lw x21, 5(x31)\n"
			"lw x23, 3(x31)\n"
			"lw x24, 6(x31)\n"
			"lw x25, 36(x31)\n"
			"lw x27, 32(x31)\n"
			"lw x9, 42(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_5:\n"
			"fle.s x12, f25, f23\n"
			"fmul.ps f26, f28, f13, rdn\n"
			"fnmadd.ps f26, f25, f23, f25, rmm\n"
			"slti x24, x16, 0x19\n"
			"fsgnjx.ps f12, f28, f20\n"
			"fbci.pi f16, 0x17dd6\n"
			"bgeu x22, x24, 1f\n"
			"1:\n"
			"fpackreph.pi f26, f26\n"
			"andi x27, x20, 0xa\n"
			"fltm.pi m0, f13, f13\n"
			"mov.m.x m0, x0, 0xff\n"
			"fcvt.wu.s x25, f5, rup\n"
			"fmv.s.x f23, x25\n"
			"blt x17, x1, 1f\n"
			"1:\n"
			"fence\n"
			"bge x30, x6, 1f\n"
			"1:\n"
			"fnmsub.s f16, f20, f13, f28, rdn\n"
			"sraiw x23, x16, 3\n"
			"fcvt.f16.ps f28, f20\n"
			"fadd.s f5, f28, f20, rup\n"
			"sltu x23, x16, x19\n"
			"beq x3, x23, 1f\n"
			"1:\n"
			"fadd.s f2, f25, f23, rup\n"
			"bgeu x21, x26, 1f\n"
			"1:\n"
			"div x14, x14, x10\n"
			"fcvt.f16.ps f12, f25\n"
			"fmadd.ps f23, f13, f20, f20, rdn\n"
			"bltu x11, x25, 1f\n"
			"1:\n"
			"sraw x24, x24, x19\n"
			"maskand m6, m1, m6\n"
			"beq x3, x15, 1f\n"
			"1:\n"
			"fcvt.w.s x12, f20, rup\n"
			"beq x29, x17, 1f\n"
			"1:\n"
			"srl x20, x27, x24\n"
			"fmin.s f24, f13, f12\n"
			"fbcx.ps f7, x16\n"
			"bltu x18, x0, 1f\n"
			"1:\n"
			"fmul.pi f7, f20, f12\n"
			"mova.m.x x23\n"
			"beq x21, x11, 1f\n"
			"1:\n"
			"sub x24, x24, x9\n"
			"fsatu8.pi f2, f13\n"
			"slti x12, x19, 0xa\n"
			"fmul.s f25, f12, f20, rmm\n"
			"masknot m1, m1\n"
			"auipc x24, 0xb\n"
			"blt x23, x6, 1f\n"
			"1:\n"
			"sraw x16, x16, x24\n"
			"fence\n"
			"fand.pi f28, f28, f5\n"
			"sltu x14, x9, x14\n"
			"fmadd.ps f12, f23, f5, f5, rmm\n"
			"xori x27, x25, 0x1f\n"
			"maskor m5, m5, m5\n"
			"xor x14, x20, x14\n"
			"fcvt.wu.s x10, f13, rup\n"
			"subw x14, x19, x27\n"
			"flem.ps m5, f25, f25\n"
			"fltu.pi f1, f5, f13\n"
			"fnmadd.s f23, f29, f12, f29, rne\n"
			"fence\n"
			"mulhsu x9, x14, x27\n"
			"mulw x27, x12, x9\n"
			"fround.ps f12, f12, rup\n"
			"slt x16, x16, x10\n"
			"blt x25, x28, 1f\n"
			"1:\n"
			"fswizz.ps f28, f13, 0x14\n"
			"addw x14, x27, x21\n"
			"add x14, x27, x27\n"
			"bgeu x15, x30, 1f\n"
			"1:\n"
			"srl x23, x24, x24\n"
			"bge x1, x14, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f26, f28, rmm\n"
			"fcmovm.ps f25, f28, f5\n"
			"bne x29, x29, 1f\n"
			"1:\n"
			"and x12, x12, x21\n"
			"fsrai.pi f7, f29, 0x8\n"
			"slliw x10, x10, 4\n"
			"bltu x7, x23, 1f\n"
			"1:\n"
			"fle.ps f12, f20, f20\n"
			"blt x4, x4, 1f\n"
			"1:\n"
			"flt.s x14, f23, f5\n"
			"mova.x.m x12\n"
			"fsub.ps f23, f25, f29, rne\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_1_LOOP_SEQ_HID_5\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"la x31, check_sum_hid_5\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"LBL_POSTAMBLE_HID_5:\n"          
			"csrwi   tensor_wait, 0x0\n"
			"csrwi   tensor_wait, 0x1\n"
			"csrwi   tensor_wait, 0x2\n"
			"csrwi   tensor_wait, 0x3\n"
			"csrwi   tensor_wait, 0x4\n"
			"csrwi   tensor_wait, 0x5\n"
			"csrwi   tensor_wait, 0x6\n"
			"csrwi   tensor_wait, 0x7\n"
			"csrwi   tensor_wait, 0x8\n"
			"csrwi   tensor_wait, 0x9\n"
			"csrwi   tensor_wait, 0xa\n"
			"fence\n"
			"LBL_SYNC_POINT_1_HID_5:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_1_WAIT_HID_5\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_1_EXIT_HID_5\n"
			"LBL_SYNC_POINT_1_WAIT_HID_5:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_1_EXIT_HID_5:\n"          
			"nop\n"
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_5\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_5:\n"          
			".global LBL_FAIL_HID_5\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_5\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_5:\n"          
			".global LBL_PASS_HID_5\n"          
			VSNIP_RSV
		);
		c_test_pass();
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_5:\n"          
			".global LBL_HALT_HID_5\n"          
			"wfi\n"
			"j LBL_HALT_HID_5\n"          
			VSNIP_RSV
		);
		return 0;       
	};
	if ( hid == 6 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_6:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_6\n"
			"csrw mscratch, x5\n"
			"csrwi menable_shadows, 0xf\n"
			"li x5, -1\n"
			"csrw mcounteren, x5\n"
			"csrw scounteren, x5\n"
			"csrwi mhpmcounter3, 0\n"
			"csrwi mhpmcounter4, 0\n"
			"csrwi mhpmcounter5, 0\n"
			"csrwi mhpmcounter6, 0\n"
			"csrwi mhpmcounter7, 0\n"
			"csrwi mhpmcounter8, 0\n"
			"csrwi mhpmevent3, 21\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 28\n"   
			"csrwi mhpmevent6, 20\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 6\n"   
			"mov.m.x m0, x0, 0xff\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f29,  896 (x5)\n"
			"flw.ps f11,  0 (x5)\n"
			"flw.ps f9,  352 (x5)\n"
			"flw.ps f3,  768 (x5)\n"
			"flw.ps f26,  64 (x5)\n"
			"flw.ps f16,  32 (x5)\n"
			"flw.ps f4,  416 (x5)\n"
			"flw.ps f1,  544 (x5)\n"
			"flw.ps f15,  960 (x5)\n"
			"flw.ps f27,  832 (x5)\n"
			"flw.ps f2,  128 (x5)\n"
			"flw.ps f6,  928 (x5)\n"
			"flw.ps f31,  256 (x5)\n"
			"flw.ps f30,  640 (x5)\n"
			"flw.ps f24,  576 (x5)\n"
			"flw.ps f0,  992 (x5)\n"
			"flw.ps f23,  320 (x5)\n"
			"flw.ps f20,  448 (x5)\n"
			"flw.ps f17,  800 (x5)\n"
			"flw.ps f22,  704 (x5)\n"
			"flw.ps f5,  672 (x5)\n"
			"flw.ps f7,  608 (x5)\n"
			"flw.ps f14,  480 (x5)\n"
			"flw.ps f10,  192 (x5)\n"
			"flw.ps f18,  864 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f21,  288 (x5)\n"
			"flw.ps f12,  96 (x5)\n"
			"flw.ps f19,  736 (x5)\n"
			"flw.ps f8,  160 (x5)\n"
			"flw.ps f28,  384 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_HID_6:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_0_WAIT_HID_6\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_0_EXIT_HID_6\n"
			"LBL_SYNC_POINT_0_WAIT_HID_6:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_EXIT_HID_6:\n"          
			"nop\n"
			"LBL_KERNEL_HID_6:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_6\n"
			"li x5, 0x2\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_HID_6:\n"
			"li x13, 0x807372a028\n"
			"la x21, mem_ptr_offset\n"
			"flw.ps f15, 992(x21)\n"
			"fscbg.ps f11, f15 (x13)\n" 
			"feq.pi f10, f10, f10\n"
			"li x13, 0x80829b5020\n"
			"amomaxul.d x5, x19, (x13)\n" 
			"fsra.pi f18, f18, f30\n"
			"li x19, 0x807152d008\n"
			"amoxorg.d x6, x27, (x19)\n" 
			"fsgnj.ps f30, f30, f11\n"
			"bltu x21, x22, 1f\n"
			"1:\n"
			"fltm.ps m5, f28, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fsll.pi f10, f6, f6\n"
			"la x5, rand_int_32\n"
			"flw.ps f15, 352(x5)\n"
			"flw.ps f7, 384(x5)\n"
			"fmulh.pi f28, f15, f7\n" 
			"fsgnjn.ps f7, f6, f7\n"
			"la x6, rand_int_32\n"
			"flw.ps f18, 832(x6)\n"
			"fnot.pi f21, f18\n" 
			"and x5, x6, x27\n"
			"li x19, 0x8043e8c678\n"
			"sd x5, 256(x19)\n"
			"lhu x13, 256(x19)\n" 
			"fmvs.x.ps x5, f11, 6\n"
			"li x17, 0x807686d028\n"
			"la x25, mem_ptr_offset\n"
			"flw.ps f3, 32(x25)\n"
			"famomaxg.pi f3, f3 (x17)\n" 
			"fmadd.s f20, f28, f20, f28, rup\n"
			"flem.ps m0, f12, f3\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"fclass.ps f12, f7\n"
			"bge x20, x5, 1f\n"
			"1:\n"
			"li x22, 0x8044298b00\n"
			"fsw.ps f6, -256(x22)\n"
			"flw.ps f11, -256(x22)\n" 
			"fcvt.pw.ps f3, f10, rup\n"
			"la x14, rand_int_32\n"
			"flw.ps f21, 352(x14)\n"
			"flw.ps f6, 480(x14)\n"
			"fmul.pi f11, f21, f6\n" 
			"fmul.pi f15, f15, f15\n"
			"li x5, 0x49673f5c2df9e22f\n"
			"li x22, 0xbd621af27f639178\n"
			"mulhsu x13, x5, x22\n" 
			"fcvt.f16.ps f10, f10\n"
			"li x17, 0x8080a68018\n"
			"la x11, mem_ptr_offset\n"
			"flw.ps f3, 960(x11)\n"
			"famomaxl.ps f20, f3 (x17)\n" 
			"fsgnj.ps f5, f3, f5\n"
			"bltu x27, x18, 1f\n"
			"1:\n"
			"li x25, 0x80705a8010\n"
			"la x21, mem_ptr_offset\n"
			"flw.ps f15, 640(x21)\n"
			"famoandg.pi f30, f15 (x25)\n" 
			"ffrc.ps f12, f14\n"
			"li x11, 0x80476304c0\n"
			"fsw.ps f21, 512(x11)\n" 
			"feq.pi f20, f11, f7\n"
			"li x22, 0x80407dce98\n"
			"la x5, mem_ptr_offset\n"
			"flw.ps f11, 928(x5)\n"
			"fgw.ps f3, f11 (x22)\n" 
			"fcvt.pw.ps f5, f5, rmm\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_0_TLD_ID_0_NMASK_0x1_CMASK_0xfb_SHIRE_0_HID_6:\n"
			"li x31, 0x1fb1e\n"
			"csrw tensor_coop, x31\n" 
			"li x31, 0x01FF000001FF0000\n"
			"csrw tensor_mask, x31\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"li x6, 0xd440008020006b28\n"
			"csrw tensor_load, x6\n" 
			"csrwi tensor_wait, 1\n" 
			"LBL_SYNC_POINT_1_HID_6:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_1_WAIT_HID_6\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_1_EXIT_HID_6\n"
			"LBL_SYNC_POINT_1_WAIT_HID_6:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_1_EXIT_HID_6:\n"          
			"nop\n"
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_6:\n"
			"li x6, 0x3200000000730000\n"
			"csrw tensor_reduce, x6\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SYNC_POINT_2_HID_6:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_2_WAIT_HID_6\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_2_EXIT_HID_6\n"
			"LBL_SYNC_POINT_2_WAIT_HID_6:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_2_EXIT_HID_6:\n"          
			"nop\n"
			"LBL_SEQID_0_TST_ID_0_QUAD128_START_CORE_0_SHIRE_0_HID_6:\n"
			"li x31, (0xc << 4)\n"
			"li x6, 0x841e008020006b30\n"
			"csrw tensor_store, x6\n" 
			"csrwi tensor_wait, 8\n" 
			"LBL_SYNC_POINT_3_HID_6:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_3_WAIT_HID_6\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_3_EXIT_HID_6\n"
			"LBL_SYNC_POINT_3_WAIT_HID_6:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_3_EXIT_HID_6:\n"          
			"nop\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, 0x28c00080400086c9\n"
			"li x6, 0x10008040002689\n"
			"li x7, 0x4c80000011d062\n"
			"li x9, 0x49200000002666\n"
			"li x10, 0x4dc80080400086c0\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000008\n"
			"add x6, x6, x31\n"
			"li x7, 0x7c00000001b061\n"
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x9\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 3\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f15, 1(x31)\n"
			"flw.ps f18, 5(x31)\n"
			"flw.ps f23, 7(x31)\n"
			"flw.ps f27, 16(x31)\n"
			"flw.ps f28, 14(x31)\n"
			"flw.ps f30, 24(x31)\n"
			"flw.ps f5, 8(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x11, 1(x31)\n"
			"lw x13, 3(x31)\n"
			"lw x14, 7(x31)\n"
			"lw x17, 0(x31)\n"
			"lw x19, 8(x31)\n"
			"lw x21, 2(x31)\n"
			"lw x22, 22(x31)\n"
			"lw x25, 27(x31)\n"
			"lw x27, 17(x31)\n"
			"lw x5, 34(x31)\n"
			"lw x6, 40(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_6:\n"
			"fmsub.ps f20, f30, f15, f5, rmm\n"
			"fmul.s f27, f27, f15, rtz\n"
			"remw x6, x6, x25\n"
			"slli x14, x14, 2\n"
			"bge x10, x25, 1f\n"
			"1:\n"
			"sltu x6, x0, x6\n"
			"fsll.pi f5, f28, f18\n"
			"fmin.ps f21, f28, f27\n"
			"fandi.pi f15, f5, 0xf0\n"
			"srlw x13, x5, x13\n"
			"remuw x25, x11, x22\n"
			"slt x11, x11, x13\n"
			"fswizz.ps f5, f5, 0x3\n"
			"fsgnj.ps f27, f27, f10\n"
			"maskpopcz x21, m4\n"
			"sll x5, x5, x17\n"
			"bltu x2, x17, 1f\n"
			"1:\n"
			"srli x19, x11, 4\n"
			"blt x0, x14, 1f\n"
			"1:\n"
			"srl x0, x0, x21\n"
			"mul x13, x13, x17\n"
			"bge x19, x20, 1f\n"
			"1:\n"
			"fnmadd.s f23, f30, f30, f10, rmm\n"
			"fbcx.ps f30, x14\n"
			"fmulh.pi f23, f30, f28\n"
			"fmv.x.s x11, f5\n"
			"masknot m4, m0\n"
			"fsll.pi f20, f5, f5\n"
			"remu x21, x19, x21\n"
			"sraw x17, x17, x22\n"
			"fcmovm.ps f28, f5, f28\n"
			"fadd.ps f27, f18, f18, rup\n"
			"blt x3, x7, 1f\n"
			"1:\n"
			"fsat8.pi f20, f30\n"
			"fltu.pi f15, f23, f15\n"
			"fsrl.pi f23, f18, f15\n"
			"fpackreph.pi f15, f5\n"
			"fsub.s f28, f28, f30, rmm\n"
			"fcvt.pw.ps f30, f30, rtz\n"
			"bne x5, x21, 1f\n"
			"1:\n"
			"mulw x19, x19, x11\n"
			"bltu x24, x11, 1f\n"
			"1:\n"
			"fsub.ps f18, f18, f18, rmm\n"
			"beq x16, x15, 1f\n"
			"1:\n"
			"fsll.pi f20, f23, f15\n"
			"fmul.ps f20, f23, f5, rdn\n"
			"fbci.pi f10, 0x38506\n"
			"sllw x13, x13, x19\n"
			"fand.pi f30, f23, f18\n"
			"fltu.pi f27, f28, f27\n"
			"fle.ps f27, f27, f18\n"
			"xor x19, x5, x19\n"
			"sra x5, x5, x27\n"
			"flt.ps f7, f23, f18\n"
			"fmadd.s f28, f30, f23, f27, rmm\n"
			"fbci.pi f27, 0x17976\n"
			"fnmadd.s f21, f28, f5, f15, rmm\n"
			"flem.ps m6, f27, f28\n"
			"rem x13, x25, x19\n"
			"srli x21, x27, 1\n"
			"divw x13, x5, x25\n"
			"fbci.ps f21, 0x4a387\n"
			"fmulhu.pi f27, f15, f28\n"
			"packb x19, x13, x19\n"
			"fcvt.ps.pwu f10, f10, rdn\n"
			"fmv.x.s x19, f5\n"
			"fmsub.ps f30, f30, f10, f18, rtz\n"
			"fmadd.ps f18, f30, f30, f18, rne\n"
			"srai x5, x13, 2\n"
			"beq x31, x19, 1f\n"
			"1:\n"
			"mova.x.m x27\n"
			"fmul.pi f28, f28, f5\n"
			"bne x4, x22, 1f\n"
			"1:\n"
			"fltu.pi f27, f30, f18\n"
			"fltm.ps m4, f28, f15\n"
			"feq.s x13, f18, f5\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_0_LOOP_SEQ_HID_6\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_HID_6:\n"
			"la x27, rand_int_32\n"
			"flw.ps f21, 96(x27)\n"
			"fcvt.ps.pwu f30, f21, rdn\n" 
			"fmadd.s f27, f10, f10, f27, rmm\n"
			"fmul.s f20, f7, f27, rne\n"
			"bgeu x25, x15, 1f\n"
			"1:\n"
			"slt x14, x17, x0\n"
			"li x22, 0x8074f5c008\n"
			"amoorg.d x19, x25, (x22)\n" 
			"fmsub.ps f28, f20, f5, f20, rne\n"
			"fnot.pi f10, f20\n"
			"faddi.pi f3, f30, 0x79\n"
			"li x6, 0x8072263038\n"
			"la x27, mem_ptr_offset\n"
			"flw.ps f23, 384(x27)\n"
			"famomaxg.pi f28, f23 (x6)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fmin.pi f5, f5, f20\n"
			"fcvt.wu.s x21, f6, rtz\n"
			"beq x30, x4, 1f\n"
			"1:\n"
			"fsub.ps f6, f6, f23, rne\n"
			"li x6, 0x80812df028\n"
			"amomaxl.d x13, x25, (x6)\n" 
			"flt.ps f20, f11, f23\n"
			"fxor.pi f28, f28, f28\n"
			"fsub.pi f3, f20, f3\n"
			"li x21, 0x8082fc3038\n"
			"la x13, mem_ptr_offset\n"
			"flw.ps f10, 576(x13)\n"
			"fgwl.ps f10, f10 (x21)\n" 
			"fle.pi f28, f6, f5\n"
			"fsrl.pi f18, f18, f10\n"
			"fsub.s f10, f6, f6, rmm\n"
			"bltu x8, x15, 1f\n"
			"1:\n"
			"li x11, 0x8084294028\n"
			"la x19, mem_ptr_offset\n"
			"flw.ps f11, 736(x19)\n"
			"fgbl.ps f27, f11 (x11)\n" 
			"fandi.pi f10, f15, 0xce\n"
			"fmul.pi f15, f27, f21\n"
			"feq.pi f27, f20, f12\n"
			"la x21, rand_int_32\n"
			"flw.ps f23, 992(x21)\n"
			"flw.ps f21, 832(x21)\n"
			"fmulhu.pi f3, f23, f21\n" 
			"srl x27, x17, x0\n"
			"add x5, x5, x6\n"
			"fcvt.s.w f6, x27, rtz\n"
			"li x25, 0x80767d9030\n"
			"amoorg.d x14, x22, (x25)\n" 
			"ebreak\n"
			"remuw x11, x19, x11\n"
			"beq x1, x3, 1f\n"
			"1:\n"
			"fsgnjx.ps f27, f18, f15\n"
			"bgeu x25, x15, 1f\n"
			"1:\n"
			"li x13, 0x8084b2e008\n"
			"la x11, mem_ptr_offset\n"
			"flw.ps f27, 576(x11)\n"
			"fschl.ps f21, f27 (x13)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fsgnjx.ps f20, f15, f3\n"
			"fand.pi f28, f3, f28\n"
			"fsub.pi f20, f15, f5\n"
			"li x21, 0x804669cfe8\n"
			"li x22, 3647286715\n"
			"fsc32w.ps f21, x6 (x21)\n" 
			"remw x17, x17, x17\n"
			"slti x21, x21, 0xc\n"
			"fmul.ps f14, f11, f3, rmm\n"
			"li x14, 0x8045b23164\n"
			"sb x6, -512(x14)\n" 
			"li x5, 0x793b81eea888845\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xd7860b4516456088\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"srl x0, x0, x5\n"
			"bltu x6, x8, 1f\n"
			"1:\n"
			"subw x19, x11, x17\n"
			"addiw x0, x6, 0x1d\n"
			"li x11, 0x8084cf0008\n"
			"amoorl.d x14, x19, (x11)\n" 
			"fsgnjx.ps f30, f10, f30\n"
			"divw x5, x21, x19\n"
			"fxor.pi f14, f28, f10\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f5, 672(x13)\n"
			"flw.ps f3, 928(x13)\n"
			"fsgnj.s f23, f5, f3\n" 
			"li x5, 0x3d01\n"
			"csrw tensor_mask, x5\n"	
			"flem.ps m6, f27, f18\n"
			"for.pi f15, f15, f28\n"
			"fcvt.s.w f20, x6, rmm\n"
			"li x6, 0x80424879c8\n"
			"fsw.ps f27, 512(x6)\n"
			"flq2 f3, 512(x6)\n" 
			"li x5, 0xd3b\n"
			"csrw tensor_mask, x5\n"	
			"fround.ps f30, f23, rmm\n"
			"fround.ps f20, f10, rtz\n"
			"bgeu x18, x9, 1f\n"
			"1:\n"
			"fsrl.pi f23, f23, f20\n"
			"li x11, 0x80407e8d10\n"
			"fsw.ps f15, 256(x11)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fmadd.ps f6, f21, f6, f21, rmm\n"
			"fmul.ps f23, f6, f27, rmm\n"
			"fmul.pi f23, f5, f5\n"
			"mova.m.x x27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fsub.ps f3, f27, f3, rtz\n"
			"fsll.pi f30, f23, f30\n"
			"fnmadd.ps f10, f11, f23, f10, rup\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_1_TLD_ID_1_NMASK_0x1_CMASK_0xbf_SHIRE_0_HID_6:\n"
			"li x31, 0x1bf0e\n"
			"csrw tensor_coop, x31\n" 
			"li x31, 0x01FF000001FF0000\n"
			"csrw tensor_mask, x31\n" 
			"li x31, (0x3 << 6) | 0x1\n"
			"li x6, 0x4010008020006b0b\n"
			"csrw tensor_load, x6\n" 
			"li x7, 0x15d800000118001\n"
			"csrw tensor_fma, x7\n" 
			"LBL_SYNC_POINT_4_HID_6:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_4_WAIT_HID_6\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_4_EXIT_HID_6\n"
			"LBL_SYNC_POINT_4_WAIT_HID_6:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_4_EXIT_HID_6:\n"          
			"nop\n"
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_6:\n"
			"li x6, 0x2600000007790001\n"
			"csrw tensor_reduce, x6\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SYNC_POINT_5_HID_6:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_5_WAIT_HID_6\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_5_EXIT_HID_6\n"
			"LBL_SYNC_POINT_5_WAIT_HID_6:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_5_EXIT_HID_6:\n"          
			"nop\n"
			"LBL_SEQID_1_TST_ID_1_QUAD128_START_CORE_0_SHIRE_0_HID_6:\n"
			"li x31, (0x4 << 4)\n"
			"li x6, 0xa25e008020006b30\n"
			"csrw tensor_store, x6\n" 
			"csrwi tensor_wait, 8\n" 
			"LBL_SYNC_POINT_6_HID_6:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_6_WAIT_HID_6\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_6_EXIT_HID_6\n"
			"LBL_SYNC_POINT_6_WAIT_HID_6:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_6_EXIT_HID_6:\n"          
			"nop\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x7, 0xd8d80080400086c0\n"
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_6:\n"
			"li x7, 0x4eb00080400086c0\n"
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 2\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x10\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f11, 1(x31)\n"
			"flw.ps f12, 6(x31)\n"
			"flw.ps f14, 8(x31)\n"
			"flw.ps f15, 7(x31)\n"
			"flw.ps f18, 17(x31)\n"
			"flw.ps f20, 0(x31)\n"
			"flw.ps f21, 0(x31)\n"
			"flw.ps f23, 10(x31)\n"
			"flw.ps f27, 22(x31)\n"
			"flw.ps f28, 5(x31)\n"
			"flw.ps f3, 31(x31)\n"
			"flw.ps f30, 23(x31)\n"
			"flw.ps f5, 30(x31)\n"
			"flw.ps f6, 47(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x11, 0(x31)\n"
			"lw x13, 7(x31)\n"
			"lw x14, 3(x31)\n"
			"lw x17, 14(x31)\n"
			"lw x19, 4(x31)\n"
			"lw x21, 22(x31)\n"
			"lw x22, 12(x31)\n"
			"lw x25, 20(x31)\n"
			"lw x27, 20(x31)\n"
			"lw x5, 35(x31)\n"
			"lw x6, 33(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_6:\n"
			"remw x27, x27, x19\n"
			"mulhsu x5, x17, x17\n"
			"bltu x31, x26, 1f\n"
			"1:\n"
			"fsrl.pi f18, f21, f11\n"
			"slt x11, x11, x11\n"
			"fmul.s f23, f11, f10, rdn\n"
			"feq.pi f27, f11, f28\n"
			"remuw x27, x22, x27\n"
			"divu x22, x11, x22\n"
			"flog.ps f5, f12\n"
			"fsub.s f28, f12, f10, rdn\n"
			"fsgnj.s f30, f18, f21\n"
			"divw x14, x5, x13\n"
			"fcvt.ps.pwu f6, f15, rne\n"
			"fsub.s f20, f14, f6, rup\n"
			"srai x0, x0, 0\n"
			"frcp.ps f11, f11\n"
			"fsub.pi f14, f6, f5\n"
			"fcmov.ps f5, f6, f3, f27\n"
			"slliw x25, x25, 1\n"
			"fmin.s f18, f18, f14\n"
			"faddi.pi f27, f27, 0x5\n"
			"fmax.ps f15, f23, f11\n"
			"blt x22, x30, 1f\n"
			"1:\n"
			"fle.pi f3, f27, f5\n"
			"remuw x21, x11, x5\n"
			"fsgnjx.ps f12, f30, f12\n"
			"flog.ps f5, f15\n"
			"fsrai.pi f20, f14, 0xd\n"
			"slliw x14, x13, 2\n"
			"blt x31, x14, 1f\n"
			"1:\n"
			"fadd.ps f21, f10, f21, rtz\n"
			"slliw x21, x17, 3\n"
			"fmulh.pi f11, f15, f11\n"
			"remw x14, x14, x0\n"
			"remw x19, x19, x22\n"
			"fmin.pi f30, f5, f6\n"
			"fmax.pi f5, f5, f3\n"
			"fandi.pi f30, f30, 0x1ef\n"
			"feq.ps f15, f11, f21\n"
			"fnmadd.ps f6, f11, f21, f18, rup\n"
			"fround.ps f27, f20, rmm\n"
			"fmin.pi f30, f23, f5\n"
			"bge x24, x11, 1f\n"
			"1:\n"
			"fandi.pi f28, f18, 0x9d\n"
			"bltu x8, x19, 1f\n"
			"1:\n"
			"feq.pi f10, f10, f21\n"
			"fnmadd.ps f20, f10, f5, f21, rtz\n"
			"addw x11, x5, x19\n"
			"fsub.pi f20, f3, f3\n"
			"fround.ps f6, f23, rtz\n"
			"addi x14, x21, 0xc\n"
			"feq.pi f14, f28, f5\n"
			"fsgnjn.s f10, f27, f5\n"
			"fcvt.ps.f16 f3, f6\n"
			"bgeu x11, x19, 1f\n"
			"1:\n"
			"addi x19, x22, 0xe\n"
			"srli x6, x21, 3\n"
			"rem x22, x5, x21\n"
			"bne x6, x25, 1f\n"
			"1:\n"
			"mulw x6, x22, x25\n"
			"andi x17, x6, 0x1b\n"
			"sraw x6, x6, x13\n"
			"fmin.s f23, f3, f21\n"
			"faddi.pi f11, f30, 0x9a\n"
			"remu x0, x25, x13\n"
			"fadd.ps f27, f15, f30, rmm\n"
			"blt x13, x28, 1f\n"
			"1:\n"
			"fle.ps f6, f11, f5\n"
			"andi x21, x25, 0xe\n"
			"mulw x19, x17, x19\n"
			"fsgnjx.ps f12, f30, f23\n"
			"fcmovm.ps f7, f15, f27\n"
			"srliw x6, x19, 3\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_1_LOOP_SEQ_HID_6\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"la x31, check_sum_hid_6\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"LBL_POSTAMBLE_HID_6:\n"          
			"csrwi   tensor_wait, 0x0\n"
			"csrwi   tensor_wait, 0x1\n"
			"csrwi   tensor_wait, 0x2\n"
			"csrwi   tensor_wait, 0x3\n"
			"csrwi   tensor_wait, 0x4\n"
			"csrwi   tensor_wait, 0x5\n"
			"csrwi   tensor_wait, 0x6\n"
			"csrwi   tensor_wait, 0x7\n"
			"csrwi   tensor_wait, 0x8\n"
			"csrwi   tensor_wait, 0x9\n"
			"csrwi   tensor_wait, 0xa\n"
			"fence\n"
			"LBL_SYNC_POINT_7_HID_6:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_7_WAIT_HID_6\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_7_EXIT_HID_6\n"
			"LBL_SYNC_POINT_7_WAIT_HID_6:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_7_EXIT_HID_6:\n"          
			"nop\n"
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_6\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_6:\n"          
			".global LBL_FAIL_HID_6\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_6\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_6:\n"          
			".global LBL_PASS_HID_6\n"          
			VSNIP_RSV
		);
		c_test_pass();
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_6:\n"          
			".global LBL_HALT_HID_6\n"          
			"wfi\n"
			"j LBL_HALT_HID_6\n"          
			VSNIP_RSV
		);
		return 0;       
	};
	if ( hid == 7 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_7:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_7\n"
			"csrw mscratch, x5\n"
			"csrwi menable_shadows, 0xf\n"
			"li x5, -1\n"
			"csrw mcounteren, x5\n"
			"csrw scounteren, x5\n"
			"csrwi mhpmcounter3, 0\n"
			"csrwi mhpmcounter4, 0\n"
			"csrwi mhpmcounter5, 0\n"
			"csrwi mhpmcounter6, 0\n"
			"csrwi mhpmcounter7, 0\n"
			"csrwi mhpmcounter8, 0\n"
			"csrwi mhpmevent3, 7\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 9\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 23\n"   
			"mov.m.x m0, x0, 0xff\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f5,  224 (x5)\n"
			"flw.ps f22,  256 (x5)\n"
			"flw.ps f20,  576 (x5)\n"
			"flw.ps f17,  512 (x5)\n"
			"flw.ps f31,  992 (x5)\n"
			"flw.ps f8,  832 (x5)\n"
			"flw.ps f11,  928 (x5)\n"
			"flw.ps f9,  864 (x5)\n"
			"flw.ps f28,  896 (x5)\n"
			"flw.ps f30,  384 (x5)\n"
			"flw.ps f2,  640 (x5)\n"
			"flw.ps f13,  320 (x5)\n"
			"flw.ps f12,  64 (x5)\n"
			"flw.ps f26,  0 (x5)\n"
			"flw.ps f10,  160 (x5)\n"
			"flw.ps f6,  960 (x5)\n"
			"flw.ps f19,  672 (x5)\n"
			"flw.ps f24,  128 (x5)\n"
			"flw.ps f21,  736 (x5)\n"
			"flw.ps f25,  32 (x5)\n"
			"flw.ps f0,  768 (x5)\n"
			"flw.ps f16,  352 (x5)\n"
			"flw.ps f1,  288 (x5)\n"
			"flw.ps f7,  704 (x5)\n"
			"flw.ps f15,  416 (x5)\n"
			"flw.ps f14,  544 (x5)\n"
			"flw.ps f29,  448 (x5)\n"
			"flw.ps f23,  608 (x5)\n"
			"flw.ps f27,  96 (x5)\n"
			"flw.ps f18,  480 (x5)\n"
			"flw.ps f3,  192 (x5)\n"
			"flw.ps f4,  800 (x5)\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_HID_7:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_0_WAIT_HID_7\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_0_EXIT_HID_7\n"
			"LBL_SYNC_POINT_0_WAIT_HID_7:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_EXIT_HID_7:\n"          
			"nop\n"
			"LBL_KERNEL_HID_7:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_7\n"
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_HID_7:\n"
			"li x16, 0x808dbb0038\n"
			"amomaxul.d x11, x23, (x16)\n" 
			"fcvt.ps.pwu f7, f21, rdn\n"
			"bge x18, x18, 1f\n"
			"1:\n"
			"fand.pi f4, f0, f4\n"
			"fpackreph.pi f0, f23\n"
			"fle.s x11, f1, f30\n"
			"divuw x21, x5, x10\n"
			"fmsub.ps f27, f7, f7, f5, rdn\n"
			"fmin.ps f22, f21, f31\n"
			"bltu x0, x8, 1f\n"
			"1:\n"
			"la x21, rand_ieee754_32\n"
			"flw.ps f6, 416(x21)\n"
			"fcvt.pw.ps f22, f6, rne\n" 
			"fnmadd.ps f1, f5, f30, f1, rdn\n"
			"fnot.pi f27, f23\n"
			"fsub.pi f18, f0, f23\n"
			"flt.pi f21, f7, f0\n"
			"fltu.pi f16, f30, f6\n"
			"fcvt.ps.pwu f21, f31, rdn\n"
			"fsgnjx.ps f31, f0, f16\n"
			"bltu x16, x3, 1f\n"
			"1:\n"
			"li x21, 0x807ddae004\n"
			"amoorg.w x7, x23, (x21)\n" 
			"fcmov.ps f7, f27, f6, f18\n"
			"fsub.s f7, f27, f0, rmm\n"
			"fadd.ps f4, f30, f30, rtz\n"
			"fbci.pi f23, 0x6ef3f\n"
			"fmul.ps f31, f27, f31, rne\n"
			"fbcx.ps f0, x7\n"
			"fle.ps f22, f27, f30\n"
			"bgeu x8, x21, 1f\n"
			"1:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f18, 992(x27)\n"
			"fmvs.x.ps x22, f18, 5\n" 
			"fcvt.s.wu f6, x23, rup\n"
			"fence\n"
			"fcvt.wu.s x21, f4, rmm\n"
			"fbci.ps f27, 0x671a6\n"
			"add x9, x9, x27\n"
			"flem.ps m4, f4, f31\n"
			"srliw x14, x16, 2\n"
			"beq x2, x31, 1f\n"
			"1:\n"
			"li x31, 0x8030000580\n"
			"li x23, 0x7\n"
			"add x23, x31, x23\n"
			"li x31, (0x3 << 6) | 0x1\n"
			"csrwi tensor_wait, 6\n" 
			"maskpopcz x11, m2\n"
			"fmaxu.pi f1, f21, f21\n"
			"fbci.pi f6, 0x66bda\n"
			"bltu x11, x30, 1f\n"
			"1:\n"
			"flt.pi f30, f1, f27\n"
			"divuw x21, x10, x23\n"
			"bgeu x2, x13, 1f\n"
			"1:\n"
			"xori x21, x14, 0x11\n"
			"fmvs.x.ps x27, f21, 1\n"
			"bne x30, x7, 1f\n"
			"1:\n"
			"li x11, 0x807e72c038\n"
			"la x9, mem_ptr_offset\n"
			"flw.ps f4, 448(x9)\n"
			"fgbg.ps f5, f4 (x11)\n" 
			"fmadd.s f27, f27, f5, f18, rtz\n"
			"blt x24, x25, 1f\n"
			"1:\n"
			"fnmadd.s f5, f27, f5, f5, rmm\n"
			"bltu x0, x9, 1f\n"
			"1:\n"
			"fclass.ps f30, f30\n"
			"fnot.pi f31, f5\n"
			"blt x2, x12, 1f\n"
			"1:\n"
			"fle.ps f7, f30, f18\n"
			"fsgnj.s f0, f4, f8\n"
			"fpackrepb.pi f1, f27\n"
			"li x14, 0x804c7c6b58\n"
			"li x9, 1720146087\n"
			"fsc32w.ps f4, x23 (x14)\n" 
			"div x15, x21, x15\n"
			"flog.ps f30, f30\n"
			"flt.pi f22, f22, f31\n"
			"srl x5, x27, x15\n"
			"fmvs.x.ps x16, f22, 2\n"
			"fnmadd.s f16, f4, f8, f14, rup\n"
			"fcvt.s.w f1, x16, rdn\n"
			"li x27, 0x8089b66000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f22, (x27)\n"
			"flwl.ps f22, (x27)\n" 
			"flt.ps f16, f22, f7\n"
			"fexp.ps f4, f30\n"
			"bltu x22, x25, 1f\n"
			"1:\n"
			"fsgnjn.ps f16, f22, f16\n"
			"ffrc.ps f4, f5\n"
			"ffrc.ps f18, f8\n"
			"bgeu x4, x14, 1f\n"
			"1:\n"
			"fsub.ps f16, f16, f30, rne\n"
			"fmulhu.pi f23, f16, f31\n"
			"la x14, rand_ieee754_16\n"
			"flw.ps f8, 416(x14)\n"
			"fcvt.ps.f16 f30, f8\n" 
			"fbci.ps f4, 0x4e45d\n"
			"ffrc.ps f21, f23\n"
			"fnmsub.s f8, f16, f5, f18, rdn\n"
			"bne x30, x19, 1f\n"
			"1:\n"
			"fadd.pi f18, f18, f4\n"
			"bltu x23, x25, 1f\n"
			"1:\n"
			"srli x14, x9, 2\n"
			"beq x23, x17, 1f\n"
			"1:\n"
			"divu x16, x21, x16\n"
			"fxor.pi f18, f5, f18\n"
			"li x7, 0x804d6cea38\n"
			"la x5, mem_ptr_offset\n"
			"flw.ps f0, 480(x5)\n"
			"fscb.ps f30, f0 (x7)\n" 
			"fcmovm.ps f18, f27, f1\n"
			"blt x28, x24, 1f\n"
			"1:\n"
			"fmaxu.pi f21, f4, f4\n"
			"fcvt.pw.ps f23, f21, rne\n"
			"fmulh.pi f16, f27, f16\n"
			"fmul.ps f21, f18, f14, rne\n"
			"fand.pi f14, f7, f14\n"
			"fsatu8.pi f7, f14\n"
			"li x16, 0x8088569028\n"
			"la x7, mem_ptr_offset\n"
			"flw.ps f4, 64(x7)\n"
			"famoxorl.pi f31, f4 (x16)\n" 
			"rem x22, x5, x9\n"
			"fmvz.x.ps x5, f22, 5\n"
			"fxor.pi f16, f5, f30\n"
			"fcvt.ps.pw f21, f5, rup\n"
			"fcvt.w.s x5, f30, rne\n"
			"feq.s x22, f22, f22\n"
			"add x27, x16, x9\n"
			"li x15, 0x807cd17030\n"
			"la x16, mem_ptr_offset\n"
			"flw.ps f16, 128(x16)\n"
			"famomaxug.pi f7, f16 (x15)\n" 
			"fsrl.pi f1, f5, f8\n"
			"fmul.pi f1, f31, f6\n"
			"fmul.pi f16, f21, f18\n"
			"fnmadd.s f16, f6, f7, f31, rtz\n"
			"for.pi f30, f18, f23\n"
			"fmul.s f4, f30, f4, rmm\n"
			"fsgnjx.ps f31, f21, f23\n"
			"li x15, 0x807e2bf008\n"
			"la x9, mem_ptr_offset\n"
			"flw.ps f23, 512(x9)\n"
			"famoswapg.pi f23, f23 (x15)\n" 
			"fsub.ps f1, f0, f31, rup\n"
			"for.pi f21, f22, f31\n"
			"fsub.ps f18, f7, f7, rup\n"
			"fsgnj.ps f22, f22, f7\n"
			"bge x11, x5, 1f\n"
			"1:\n"
			"fnmsub.s f0, f7, f0, f0, rmm\n"
			"fslli.pi f14, f31, 0xf\n"
			"fand.pi f31, f22, f7\n"
			"li x10, 0xc855aabe7470f87d\n"
			"srliw x11, x10, 0\n" 
			"fnmadd.s f31, f31, f5, f8, rtz\n"
			"add x10, x21, x10\n"
			"andi x23, x11, 0x15\n"
			"maskpopcz x16, m4\n"
			"feq.s x27, f14, f8\n"
			"faddi.pi f30, f30, 0x1d0\n"
			"bne x6, x10, 1f\n"
			"1:\n"
			"faddi.pi f0, f27, 0x16\n"
			"li x31, 0x8048000900\n"
			"li x16, 0x1\n"
			"add x16, x31, x16\n"
			"li x31, (0x2 << 6) | 0x0\n"
			"csrwi tensor_wait, 6\n" 
			"fcvt.f16.ps f6, f30\n"
			"fnmsub.s f14, f14, f23, f31, rdn\n"
			"fmax.pi f14, f14, f30\n"
			"fsgnj.s f14, f23, f30\n"
			"fsrl.pi f1, f5, f14\n"
			"fsgnj.ps f18, f23, f23\n"
			"beq x9, x24, 1f\n"
			"1:\n"
			"fpackreph.pi f6, f31\n"
			"li x11, 0x8049e75aa7\n"
			"sd x15, 512(x11)\n"
			"ld x27, 512(x11)\n" 
			"fltu.pi f7, f22, f0\n"
			"fmul.ps f7, f14, f14, rmm\n"
			"fcvt.pw.ps f27, f14, rtz\n"
			"fltu.pi f5, f22, f7\n"
			"feq.pi f1, f6, f27\n"
			"fand.pi f7, f8, f27\n"
			"beq x6, x22, 1f\n"
			"1:\n"
			"flt.ps f22, f6, f22\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f1, 4(x31)\n"
			"flw.ps f14, 1(x31)\n"
			"flw.ps f16, 11(x31)\n"
			"flw.ps f18, 11(x31)\n"
			"flw.ps f21, 4(x31)\n"
			"flw.ps f22, 0(x31)\n"
			"flw.ps f23, 11(x31)\n"
			"flw.ps f27, 30(x31)\n"
			"flw.ps f30, 31(x31)\n"
			"flw.ps f31, 12(x31)\n"
			"flw.ps f4, 29(x31)\n"
			"flw.ps f5, 33(x31)\n"
			"flw.ps f6, 17(x31)\n"
			"flw.ps f7, 2(x31)\n"
			"flw.ps f8, 26(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x10, 0(x31)\n"
			"lw x11, 3(x31)\n"
			"lw x14, 2(x31)\n"
			"lw x15, 1(x31)\n"
			"lw x16, 8(x31)\n"
			"lw x21, 5(x31)\n"
			"lw x22, 24(x31)\n"
			"lw x23, 21(x31)\n"
			"lw x27, 19(x31)\n"
			"lw x5, 30(x31)\n"
			"lw x7, 5(x31)\n"
			"lw x9, 23(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_7:\n"
			"mulhu x22, x15, x22\n"
			"fsrli.pi f16, f5, 0x3\n"
			"fminu.pi f5, f14, f23\n"
			"fmul.pi f21, f6, f0\n"
			"feqm.ps m2, f0, f5\n"
			"fbcx.ps f5, x16\n"
			"blt x13, x17, 1f\n"
			"1:\n"
			"fminu.pi f30, f18, f8\n"
			"fnmsub.ps f4, f4, f23, f4, rtz\n"
			"fmv.x.s x5, f5\n"
			"bne x20, x16, 1f\n"
			"1:\n"
			"or x7, x15, x14\n"
			"frcp.ps f5, f27\n"
			"fslli.pi f0, f8, 0x8\n"
			"fbci.ps f5, 0x34174\n"
			"beq x2, x17, 1f\n"
			"1:\n"
			"fmulhu.pi f0, f0, f6\n"
			"remuw x21, x22, x10\n"
			"or x10, x11, x23\n"
			"fmin.ps f8, f7, f6\n"
			"divuw x21, x22, x22\n"
			"bge x14, x26, 1f\n"
			"1:\n"
			"fmul.s f22, f14, f0, rdn\n"
			"fsgnjn.ps f8, f31, f1\n"
			"fsub.s f7, f8, f4, rdn\n"
			"fsrl.pi f5, f0, f1\n"
			"remu x5, x27, x7\n"
			"fcvt.pwu.ps f1, f22, rmm\n"
			"fcvt.f16.ps f27, f0\n"
			"fbcx.ps f7, x5\n"
			"fcvt.s.wu f5, x15, rdn\n"
			"ecall\n"
			"beq x0, x8, 1f\n"
			"1:\n"
			"maskor m4, m5, m2\n"
			"bltu x16, x0, 1f\n"
			"1:\n"
			"fsrl.pi f18, f23, f22\n"
			"xor x7, x7, x7\n"
			"rem x7, x15, x7\n"
			"fand.pi f21, f4, f16\n"
			"feq.s x21, f21, f23\n"
			"fcvt.pwu.ps f6, f31, rtz\n"
			"fnmsub.ps f14, f4, f14, f31, rne\n"
			"flem.ps m4, f8, f7\n"
			"fcvt.ps.pw f21, f4, rne\n"
			"blt x28, x3, 1f\n"
			"1:\n"
			"fcvt.w.s x14, f30, rup\n"
			"fnmsub.ps f4, f5, f31, f8, rmm\n"
			"bgeu x31, x5, 1f\n"
			"1:\n"
			"and x14, x9, x14\n"
			"bgeu x30, x15, 1f\n"
			"1:\n"
			"mul x14, x11, x27\n"
			"fpackreph.pi f30, f16\n"
			"fsatu8.pi f5, f1\n"
			"fpackreph.pi f4, f4\n"
			"ebreak\n"
			"fle.s x22, f22, f21\n"
			"remu x7, x23, x15\n"
			"fmul.s f8, f31, f31, rup\n"
			"xor x5, x21, x9\n"
			"fsrl.pi f7, f7, f1\n"
			"sll x21, x21, x5\n"
			"bltu x4, x12, 1f\n"
			"1:\n"
			"packb x21, x11, x21\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_0_LOOP_SEQ_HID_7\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_HID_7:\n"
			"li x7, 0x807afaa000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f7, (x7)\n"
			"flwg.ps f23, (x7)\n" 
			"addi x27, x23, 0x13\n"
			"fsra.pi f30, f30, f4\n"
			"divw x23, x21, x15\n"
			"bltu x13, x23, 1f\n"
			"1:\n"
			"fmaxu.pi f30, f8, f31\n"
			"fcvt.f16.ps f23, f5\n"
			"bge x25, x1, 1f\n"
			"1:\n"
			"li x11, 0xf3507053642bd770\n"
			"li x27, 0xf82fcaf21d8f8715\n"
			"addw x22, x11, x27\n" 
			"fadd.pi f31, f0, f1\n"
			"fmadd.ps f22, f1, f27, f8, rup\n"
			"fsgnj.ps f31, f31, f31\n"
			"fminu.pi f4, f7, f5\n"
			"fsgnjx.s f23, f23, f4\n"
			"bltu x7, x13, 1f\n"
			"1:\n"
			"li x11, 0x8049fb93d0\n"
			"li x27, 1383346344\n"
			"fg32b.ps f14, x10 (x11)\n" 
			"fswizz.ps f5, f27, 0x3\n"
			"fcvt.pw.ps f23, f5, rdn\n"
			"blt x12, x11, 1f\n"
			"1:\n"
			"fmul.s f6, f30, f4, rne\n"
			"faddi.pi f5, f5, 0xa5\n"
			"fswizz.ps f6, f27, 0x3\n"
			"li x5, 0x1c215a03c2776e88\n"
			"li x14, 0x3faddfce9adc1add\n"
			"add x9, x5, x14\n" 
			"fxor.pi f21, f7, f21\n"
			"bne x3, x2, 1f\n"
			"1:\n"
			"fcvt.ps.pw f30, f7, rup\n"
			"bltu x28, x26, 1f\n"
			"1:\n"
			"fadd.ps f4, f30, f4, rne\n"
			"fsub.ps f14, f1, f30, rdn\n"
			"fsub.pi f5, f16, f5\n"
			"bne x3, x11, 1f\n"
			"1:\n"
			"li x7, 0x808d0df038\n"
			"la x10, mem_ptr_offset\n"
			"flw.ps f18, 608(x10)\n"
			"famoswapl.pi f30, f18 (x7)\n" 
			"slli x7, x21, 4\n"
			"fminu.pi f4, f7, f18\n"
			"beq x1, x23, 1f\n"
			"1:\n"
			"packb x7, x11, x11\n"
			"fbcx.ps f23, x5\n"
			"addiw x22, x10, 0x18\n"
			"li x9, 0x673eaedf54f0eb89\n"
			"li x5, 0x5a8148f1fc56ca6a\n"
			"slt x9, x9, x5\n" 
			"packb x21, x7, x11\n"
			"fsll.pi f27, f6, f14\n"
			"fpackrepb.pi f27, f1\n"
			"sll x15, x27, x23\n"
			"fcmov.ps f1, f1, f1, f4\n"
			"la x22, rand_int_32\n"
			"flw.ps f14, 672(x22)\n"
			"flw.ps f30, 608(x22)\n"
			"fsub.pi f27, f14, f30\n" 
			"fmax.s f22, f16, f4\n"
			"flog.ps f14, f14\n"
			"ffrc.ps f27, f1\n"
			"fcvt.ps.pw f23, f23, rmm\n"
			"bgeu x6, x17, 1f\n"
			"1:\n"
			"fandi.pi f8, f30, 0x1e9\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 992(x10)\n"
			"flw.ps f16, 128(x10)\n"
			"fmax.s f16, f6, f16\n" 
			"sraiw x11, x23, 0\n"
			"fxor.pi f18, f31, f23\n"
			"beq x2, x19, 1f\n"
			"1:\n"
			"srli x21, x9, 4\n"
			"fpackreph.pi f14, f16\n"
			"xori x5, x5, 0x5\n"
			"li x23, 0x807e9ef038\n"
			"la x22, mem_ptr_offset\n"
			"flw.ps f23, 384(x22)\n"
			"fghg.ps f22, f23 (x23)\n" 
			"fadd.pi f4, f1, f23\n"
			"fsub.s f30, f1, f23, rne\n"
			"fround.ps f30, f1, rmm\n"
			"fsra.pi f30, f7, f7\n"
			"fsll.pi f21, f8, f1\n"
			"li x5, 0x808b3e7008\n"
			"la x14, mem_ptr_offset\n"
			"flw.ps f7, 0(x14)\n"
			"famoaddl.pi f6, f7 (x5)\n" 
			"fsra.pi f7, f22, f16\n"
			"beq x20, x29, 1f\n"
			"1:\n"
			"fmin.s f8, f8, f7\n"
			"fmul.ps f5, f30, f30, rmm\n"
			"bltu x24, x11, 1f\n"
			"1:\n"
			"fsgnjn.ps f22, f31, f5\n"
			"fsgnj.s f27, f22, f18\n"
			"bge x11, x12, 1f\n"
			"1:\n"
			"li x27, 0x808d201020\n"
			"la x9, mem_ptr_offset\n"
			"flw.ps f22, 320(x9)\n"
			"famomaxl.ps f27, f22 (x27)\n" 
			"fsra.pi f4, f21, f27\n"
			"bltu x16, x30, 1f\n"
			"1:\n"
			"fsrai.pi f21, f14, 0x0\n"
			"blt x13, x27, 1f\n"
			"1:\n"
			"fsgnjn.s f4, f0, f21\n"
			"fnmadd.s f30, f8, f4, f14, rup\n"
			"fmulh.pi f14, f4, f18\n"
			"fbci.pi f16, 0x51dde\n" 
			"mova.m.x x23\n"
			"fcmov.ps f22, f30, f31, f23\n"
			"fmulh.pi f1, f21, f4\n"
			"beq x5, x30, 1f\n"
			"1:\n"
			"flog.ps f4, f14\n"
			"fsrai.pi f16, f4, 0x7\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f27, 64(x27)\n"
			"fmvs.x.ps x22, f27, 4\n" 
			"fsgnjx.s f5, f8, f7\n"
			"fmin.s f14, f14, f8\n"
			"fmin.ps f21, f4, f6\n"
			"feq.pi f30, f0, f21\n"
			"fsgnj.ps f14, f22, f21\n"
			"masknot m5, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fsrl.pi f8, f1, f22\n"
			"fpackreph.pi f22, f23\n"
			"ffrc.ps f1, f23\n"
			"fmax.s f8, f30, f7\n"
			"feq.ps f4, f23, f22\n"
			"li x16, 0x807f953030\n"
			"la x23, mem_ptr_offset\n"
			"flw.ps f30, 320(x23)\n"
			"famoswapg.pi f31, f30 (x16)\n" 
			"fcvt.ps.pw f8, f18, rup\n"
			"fsgnjn.ps f16, f16, f16\n"
			"feq.s x22, f18, f16\n"
			"fslli.pi f16, f16, 0x0\n"
			"fltm.pi m4, f22, f16\n"
			"li x23, 0x807a5f6030\n"
			"amoxorg.d x16, x16, (x23)\n" 
			"fsub.ps f31, f16, f21, rne\n"
			"fmin.s f14, f0, f5\n"
			"for.pi f30, f18, f1\n"
			"fsgnjx.ps f4, f23, f4\n"
			"fexp.ps f8, f8\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x10\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f1, 4(x31)\n"
			"flw.ps f14, 8(x31)\n"
			"flw.ps f16, 12(x31)\n"
			"flw.ps f18, 8(x31)\n"
			"flw.ps f21, 0(x31)\n"
			"flw.ps f22, 5(x31)\n"
			"flw.ps f23, 23(x31)\n"
			"flw.ps f27, 25(x31)\n"
			"flw.ps f30, 11(x31)\n"
			"flw.ps f31, 40(x31)\n"
			"flw.ps f4, 8(x31)\n"
			"flw.ps f5, 4(x31)\n"
			"flw.ps f6, 44(x31)\n"
			"flw.ps f7, 27(x31)\n"
			"flw.ps f8, 53(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_7:\n"
			"frcp.ps f21, f21\n"
			"bne x6, x3, 1f\n"
			"1:\n"
			"fnot.pi f7, f23\n"
			"bge x30, x4, 1f\n"
			"1:\n"
			"fsub.pi f27, f23, f21\n"
			"flt.pi f16, f16, f16\n"
			"beq x25, x26, 1f\n"
			"1:\n"
			"fmax.s f16, f16, f16\n"
			"fnot.pi f23, f4\n"
			"fcmov.ps f21, f23, f31, f14\n"
			"fsgnj.s f21, f18, f14\n"
			"fsgnjn.ps f18, f6, f6\n"
			"fmax.s f16, f5, f30\n"
			"fnmadd.s f7, f16, f7, f6, rne\n"
			"fsgnjn.s f7, f21, f31\n"
			"fmax.pi f16, f4, f14\n"
			"blt x11, x20, 1f\n"
			"1:\n"
			"flog.ps f18, f31\n"
			"fpackrepb.pi f1, f5\n"
			"fsatu8.pi f6, f0\n"
			"fmsub.s f8, f4, f5, f5, rmm\n"
			"fsgnj.ps f27, f8, f18\n"
			"bge x16, x24, 1f\n"
			"1:\n"
			"fsgnjn.ps f5, f14, f23\n"
			"fand.pi f27, f21, f16\n"
			"bgeu x10, x11, 1f\n"
			"1:\n"
			"frcp.ps f23, f14\n"
			"fswizz.ps f30, f30, 0x18\n"
			"fmadd.s f14, f23, f6, f7, rdn\n"
			"fsatu8.pi f5, f7\n"
			"fxor.pi f4, f14, f7\n"
			"feq.ps f6, f6, f18\n"
			"fandi.pi f8, f7, 0x17b\n"
			"fmul.s f14, f7, f6, rmm\n"
			"fround.ps f1, f21, rdn\n"
			"fsatu8.pi f6, f8\n"
			"bltu x27, x28, 1f\n"
			"1:\n"
			"fmsub.s f6, f21, f23, f14, rdn\n"
			"fround.ps f6, f1, rup\n"
			"fcmov.ps f5, f16, f8, f22\n"
			"fmulh.pi f5, f1, f16\n"
			"fcvt.ps.pwu f0, f30, rtz\n"
			"fcmov.ps f14, f14, f21, f30\n"
			"bge x27, x28, 1f\n"
			"1:\n"
			"fsgnj.ps f7, f22, f31\n"
			"fmul.ps f7, f18, f18, rup\n"
			"beq x19, x9, 1f\n"
			"1:\n"
			"fmin.ps f1, f16, f1\n"
			"fmaxu.pi f6, f6, f1\n"
			"fsgnjn.s f18, f27, f27\n"
			"fsgnj.ps f0, f21, f0\n"
			"fnmadd.s f4, f4, f31, f23, rmm\n"
			"blt x0, x20, 1f\n"
			"1:\n"
			"fmin.pi f30, f22, f21\n"
			"fswizz.ps f1, f1, 0x19\n"
			"fnmadd.ps f21, f1, f21, f5, rup\n"
			"fmax.ps f23, f27, f14\n"
			"fcvt.ps.f16 f0, f27\n"
			"bgeu x31, x16, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f21, f21, rdn\n"
			"fmadd.ps f31, f4, f31, f7, rdn\n"
			"bltu x8, x21, 1f\n"
			"1:\n"
			"fcvt.f16.ps f23, f22\n"
			"bgeu x0, x12, 1f\n"
			"1:\n"
			"fnmadd.ps f0, f1, f14, f4, rtz\n"
			"bge x16, x4, 1f\n"
			"1:\n"
			"fle.ps f4, f14, f4\n"
			"fsgnj.s f16, f1, f16\n"
			"bltu x13, x6, 1f\n"
			"1:\n"
			"fcmov.ps f21, f7, f22, f30\n"
			"fsgnj.s f27, f27, f0\n"
			"bne x16, x14, 1f\n"
			"1:\n"
			"fsgnjx.s f27, f4, f22\n"
			"fmaxu.pi f18, f14, f31\n"
			"beq x24, x25, 1f\n"
			"1:\n"
			"fpackreph.pi f16, f18\n"
			"fadd.s f21, f5, f22, rne\n"
			"fcmov.ps f0, f0, f22, f16\n"
			"fandi.pi f6, f5, 0x1ca\n"
			"fmin.s f21, f30, f16\n"
			"fsat8.pi f7, f22\n"
			"bltu x13, x18, 1f\n"
			"1:\n"
			"fsgnj.s f5, f21, f1\n"
			"fsgnjn.s f16, f16, f8\n"
			"fsgnj.s f0, f5, f1\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_1_LOOP_SEQ_HID_7\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_HID_7:\n"
			"li x7, 0x808f6a1018\n"
			"la x16, mem_ptr_offset\n"
			"flw.ps f30, 608(x16)\n"
			"famoandl.pi f7, f30 (x7)\n" 
			"fcvt.pwu.ps f27, f1, rtz\n"
			"fle.ps f14, f14, f21\n"
			"beq x11, x25, 1f\n"
			"1:\n"
			"fmul.s f4, f1, f4, rtz\n"
			"fmax.s f4, f4, f6\n"
			"li x7, 0x80889b9028\n"
			"amomaxl.d x27, x27, (x7)\n" 
			"addiw x16, x14, 0x11\n"
			"fnmsub.s f0, f30, f23, f30, rtz\n"
			"fle.pi f31, f31, f4\n"
			"flem.ps m5, f5, f18\n"
			"masknot m5, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fsll.pi f5, f6, f0\n"
			"beq x19, x5, 1f\n"
			"1:\n"
			"fmsub.s f27, f27, f5, f14, rdn\n"
			"fclass.ps f1, f6\n"
			"fcvt.pw.ps f0, f1, rdn\n"
			"li x9, 0x804cf6f900\n"
			"fsw.ps f14, 128(x9)\n"
			"flw.ps f0, 128(x9)\n" 
			"slli x22, x22, 5\n"
			"fmsub.s f0, f0, f4, f0, rup\n"
			"fadd.s f6, f0, f30, rdn\n"
			"maskpopc x16, m4\n"
			"li x9, 0x807b728004\n"
			"amominug.w x9, x22, (x9)\n" 
			"sra x11, x10, x15\n"
			"fle.pi f1, f16, f0\n"
			"srlw x21, x5, x21\n"
			"fmin.s f7, f31, f0\n"
			"li x11, 0x808ef5f010\n"
			"amomaxl.w x21, x27, (x11)\n" 
			"fmul.pi f5, f14, f4\n"
			"fle.pi f6, f14, f6\n"
			"fxor.pi f0, f7, f21\n"
			"bltu x25, x25, 1f\n"
			"1:\n"
			"fsatu8.pi f8, f5\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 480(x5)\n"
			"fcvt.lu.s x11, f16, rne\n" 
			"fsgnjx.s f27, f31, f1\n"
			"beq x21, x31, 1f\n"
			"1:\n"
			"fsgnjx.ps f31, f8, f1\n"
			"fsgnj.s f1, f23, f22\n"
			"bgeu x22, x18, 1f\n"
			"1:\n"
			"fsgnjn.ps f27, f6, f27\n"
			"li x27, 0x807c776000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f8, (x27)\n"
			"flwg.ps f23, (x27)\n" 
			"fadd.ps f22, f6, f22, rtz\n"
			"flog.ps f8, f8\n"
			"faddi.pi f1, f8, 0x100\n"
			"srl x23, x23, x10\n"
			"li x11, 0x804c3a7e68\n"
			"fsw f16, 512(x11)\n" 
			"mul x15, x23, x14\n"
			"fmax.ps f14, f14, f0\n"
			"fnmsub.ps f18, f18, f31, f18, rup\n"
			"sllw x11, x23, x7\n"
			"li x21, 0x807c62c010\n"
			"la x5, mem_ptr_offset\n"
			"flw.ps f30, 96(x5)\n"
			"famoswapg.pi f27, f30 (x21)\n" 
			"fcvt.pwu.ps f21, f16, rdn\n"
			"fmulhu.pi f27, f21, f5\n"
			"fslli.pi f31, f14, 0xb\n"
			"fsrl.pi f16, f4, f14\n"
			"bge x7, x1, 1f\n"
			"1:\n"
			"li x7, 0x807f372000\n"
			"amoandg.d x10, x22, (x7)\n" 
			"fcmovm.ps f23, f31, f18\n"
			"bgeu x19, x21, 1f\n"
			"1:\n"
			"feqm.ps m3, f6, f7\n"
			"fnmsub.ps f16, f6, f6, f6, rup\n"
			"fnot.pi f7, f7\n"
			"li x15, 0x807e081018\n"
			"amoswapg.w x27, x5, (x15)\n" 
			"fcvt.f16.ps f1, f8\n"
			"fsgnjn.ps f18, f7, f31\n"
			"fle.pi f23, f31, f27\n"
			"fcmov.ps f14, f22, f30, f23\n"
			"li x27, 0x807bb57000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f8, (x27)\n" 
			"fminu.pi f30, f22, f31\n"
			"fsub.pi f4, f27, f22\n"
			"fsgnjn.s f22, f30, f22\n"
			"fand.pi f31, f31, f31\n"
			"li x15, 0x808bc5f020\n"
			"la x9, mem_ptr_offset\n"
			"flw.ps f21, 800(x9)\n"
			"famominl.ps f7, f21 (x15)\n" 
			"fmin.ps f6, f5, f6\n"
			"fmin.ps f30, f5, f30\n"
			"fcmovm.ps f30, f30, f27\n"
			"fmin.s f5, f5, f5\n"
			"bgeu x9, x11, 1f\n"
			"1:\n"
			"li x21, 0xde138e8793a74976\n"
			"sraiw x10, x21, 2\n" 
			"fcvt.ps.pwu f8, f30, rtz\n"
			"fmsub.ps f14, f30, f14, f14, rup\n"
			"bgeu x11, x17, 1f\n"
			"1:\n"
			"fminu.pi f16, f16, f5\n"
			"fltu.pi f8, f31, f6\n"
			"la x27, rand_int_32\n"
			"flw.ps f22, 288(x27)\n"
			"flw.ps f16, 928(x27)\n"
			"fminu.pi f18, f22, f16\n" 
			"fand.pi f7, f5, f31\n"
			"fsll.pi f27, f14, f1\n"
			"sraiw x27, x9, 2\n"
			"sllw x7, x15, x16\n"
			"beq x8, x23, 1f\n"
			"1:\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x18\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f14, 0(x31)\n"
			"flw.ps f18, 3(x31)\n"
			"flw.ps f21, 0(x31)\n"
			"flw.ps f27, 12(x31)\n"
			"flw.ps f30, 8(x31)\n"
			"flw.ps f31, 8(x31)\n"
			"flw.ps f5, 17(x31)\n"
			"flw.ps f6, 7(x31)\n"
			"flw.ps f7, 25(x31)\n"
			"flw.ps f8, 20(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x11, 0(x31)\n"
			"lw x15, 0(x31)\n"
			"lw x16, 2(x31)\n"
			"lw x21, 8(x31)\n"
			"lw x22, 13(x31)\n"
			"lw x23, 10(x31)\n"
			"lw x27, 15(x31)\n"
			"lw x5, 9(x31)\n"
			"lw x7, 4(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_7:\n"
			"maskpopcz x21, m3\n"
			"beq x29, x15, 1f\n"
			"1:\n"
			"fltm.pi m2, f6, f8\n"
			"flt.ps f0, f8, f21\n"
			"bge x13, x18, 1f\n"
			"1:\n"
			"fsetm.pi m2, f14\n"
			"divuw x15, x15, x23\n"
			"fsub.s f21, f21, f27, rne\n"
			"flt.s x27, f8, f6\n"
			"fnmsub.ps f5, f14, f18, f14, rtz\n"
			"fpackreph.pi f14, f27\n"
			"fmin.pi f7, f30, f18\n"
			"mulw x16, x21, x16\n"
			"blt x20, x15, 1f\n"
			"1:\n"
			"lui x22, 0x16\n"
			"ecall\n"
			"packb x15, x11, x15\n"
			"feq.pi f31, f30, f31\n"
			"fmadd.s f27, f27, f7, f27, rdn\n"
			"bge x11, x10, 1f\n"
			"1:\n"
			"fsgnjn.s f6, f21, f14\n"
			"fmadd.ps f18, f8, f6, f18, rmm\n"
			"srl x22, x7, x27\n"
			"fsub.pi f0, f8, f14\n"
			"fence\n"
			"for.pi f21, f21, f21\n"
			"flt.ps f14, f8, f30\n"
			"slli x16, x22, 1\n"
			"masknot m4, m5\n"
			"sub x5, x11, x5\n"
			"flt.s x21, f21, f7\n"
			"fbci.ps f8, 0x63ea8\n"
			"fmul.ps f27, f18, f31, rup\n"
			"bge x10, x3, 1f\n"
			"1:\n"
			"divuw x16, x23, x5\n"
			"fxor.pi f14, f7, f14\n"
			"feqm.ps m2, f21, f8\n"
			"slti x21, x23, 0x11\n"
			"bgeu x10, x3, 1f\n"
			"1:\n"
			"fmaxu.pi f6, f21, f14\n"
			"feq.s x22, f8, f18\n"
			"fadd.ps f7, f31, f30, rup\n"
			"fand.pi f8, f14, f8\n"
			"fmin.pi f27, f21, f30\n"
			"blt x19, x10, 1f\n"
			"1:\n"
			"fnot.pi f6, f6\n"
			"flog.ps f8, f31\n"
			"fmul.pi f8, f27, f18\n"
			"fcvt.pw.ps f0, f6, rtz\n"
			"fsgnj.s f30, f30, f27\n"
			"sltiu x27, x27, 0xe\n"
			"addw x16, x16, x22\n"
			"fmul.pi f5, f30, f27\n"
			"lui x23, 0xd\n"
			"faddi.pi f30, f27, 0x1a4\n"
			"beq x31, x17, 1f\n"
			"1:\n"
			"flog.ps f21, f21\n"
			"fnmadd.s f23, f27, f31, f21, rmm\n"
			"sllw x27, x22, x27\n"
			"maskxor m2, m4, m2\n"
			"fmadd.ps f8, f5, f8, f18, rne\n"
			"feqm.ps m2, f21, f21\n"
			"srli x10, x22, 0\n"
			"bne x30, x23, 1f\n"
			"1:\n"
			"fcvt.pw.ps f23, f5, rne\n"
			"rem x15, x23, x11\n"
			"fsll.pi f7, f18, f30\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_2_LOOP_SEQ_HID_7\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f19,  864 (x5)\n"
			"flw.ps f11,  224 (x5)\n"
			"flw.ps f20,  704 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f14,  736 (x5)\n"
			"flw.ps f31,  672 (x5)\n"
			"flw.ps f30,  608 (x5)\n"
			"flw.ps f2,  288 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f13,  128 (x5)\n"
			"flw.ps f5,  640 (x5)\n"
			"flw.ps f27,  96 (x5)\n"
			"flw.ps f15,  192 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
			"flw.ps f28,  512 (x5)\n"
			"flw.ps f4,  480 (x5)\n"
			"flw.ps f23,  64 (x5)\n"
			"flw.ps f24,  544 (x5)\n"
			"flw.ps f18,  928 (x5)\n"
			"flw.ps f3,  800 (x5)\n"
			"flw.ps f25,  832 (x5)\n"
			"flw.ps f1,  320 (x5)\n"
			"flw.ps f9,  992 (x5)\n"
			"flw.ps f12,  160 (x5)\n"
			"flw.ps f6,  448 (x5)\n"
			"flw.ps f16,  0 (x5)\n"
			"flw.ps f17,  32 (x5)\n"
			"flw.ps f26,  896 (x5)\n"
			"flw.ps f21,  384 (x5)\n"
			"flw.ps f8,  352 (x5)\n"
			"flw.ps f10,  768 (x5)\n"
			"flw.ps f22,  576 (x5)\n"
			"la x31, check_sum_hid_7\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"LBL_POSTAMBLE_HID_7:\n"          
			"csrwi   tensor_wait, 0x0\n"
			"csrwi   tensor_wait, 0x1\n"
			"csrwi   tensor_wait, 0x2\n"
			"csrwi   tensor_wait, 0x3\n"
			"csrwi   tensor_wait, 0x4\n"
			"csrwi   tensor_wait, 0x5\n"
			"csrwi   tensor_wait, 0x6\n"
			"csrwi   tensor_wait, 0x7\n"
			"csrwi   tensor_wait, 0x8\n"
			"csrwi   tensor_wait, 0x9\n"
			"csrwi   tensor_wait, 0xa\n"
			"fence\n"
			"LBL_SYNC_POINT_1_HID_7:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_1_WAIT_HID_7\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_1_EXIT_HID_7\n"
			"LBL_SYNC_POINT_1_WAIT_HID_7:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_1_EXIT_HID_7:\n"          
			"nop\n"
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_7\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_7:\n"          
			".global LBL_FAIL_HID_7\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_7\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_7:\n"          
			".global LBL_PASS_HID_7\n"          
			VSNIP_RSV
		);
		c_test_pass();
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_7:\n"          
			".global LBL_HALT_HID_7\n"          
			"wfi\n"
			"j LBL_HALT_HID_7\n"          
			VSNIP_RSV
		);
		return 0;       
	};
	if ( hid == 8 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_8:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_8\n"
			"csrw mscratch, x5\n"
			"csrwi menable_shadows, 0xf\n"
			"li x5, -1\n"
			"csrw mcounteren, x5\n"
			"csrw scounteren, x5\n"
			"csrwi mhpmcounter3, 0\n"
			"csrwi mhpmcounter4, 0\n"
			"csrwi mhpmcounter5, 0\n"
			"csrwi mhpmcounter6, 0\n"
			"csrwi mhpmcounter7, 0\n"
			"csrwi mhpmcounter8, 0\n"
			"csrwi mhpmevent3, 21\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 28\n"   
			"csrwi mhpmevent6, 20\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 6\n"   
			"mov.m.x m0, x0, 0xff\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f5,  544 (x5)\n"
			"flw.ps f2,  320 (x5)\n"
			"flw.ps f29,  0 (x5)\n"
			"flw.ps f8,  256 (x5)\n"
			"flw.ps f7,  288 (x5)\n"
			"flw.ps f28,  928 (x5)\n"
			"flw.ps f25,  832 (x5)\n"
			"flw.ps f26,  32 (x5)\n"
			"flw.ps f6,  480 (x5)\n"
			"flw.ps f22,  384 (x5)\n"
			"flw.ps f12,  96 (x5)\n"
			"flw.ps f21,  576 (x5)\n"
			"flw.ps f20,  416 (x5)\n"
			"flw.ps f17,  896 (x5)\n"
			"flw.ps f27,  864 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f0,  128 (x5)\n"
			"flw.ps f31,  448 (x5)\n"
			"flw.ps f30,  672 (x5)\n"
			"flw.ps f24,  352 (x5)\n"
			"flw.ps f14,  160 (x5)\n"
			"flw.ps f23,  64 (x5)\n"
			"flw.ps f19,  640 (x5)\n"
			"flw.ps f16,  512 (x5)\n"
			"flw.ps f3,  768 (x5)\n"
			"flw.ps f4,  192 (x5)\n"
			"flw.ps f18,  704 (x5)\n"
			"flw.ps f1,  608 (x5)\n"
			"flw.ps f15,  960 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"flw.ps f9,  992 (x5)\n"
			"flw.ps f11,  800 (x5)\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_HID_8:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_0_WAIT_HID_8\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_0_EXIT_HID_8\n"
			"LBL_SYNC_POINT_0_WAIT_HID_8:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_EXIT_HID_8:\n"          
			"nop\n"
			"LBL_KERNEL_HID_8:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_8\n"
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_HID_8:\n"
			"li x27, 0x809226800c\n"
			"amominl.w x10, x19, (x27)\n" 
			"fcvt.w.s x0, f7, rdn\n"
			"faddi.pi f22, f27, 0x15f\n"
			"slli x27, x15, 0\n"
			"fnot.pi f3, f21\n"
			"fmadd.ps f1, f22, f11, f22, rup\n"
			"li x31, 0x8030000580\n"
			"li x28, 0xc00000000000004\n"
			"add x28, x31, x28\n"
			"li x31, (0x3 << 6) | 0x0\n"
			"csrw evict_va, x28\n"	
			"csrwi tensor_wait, 6\n" 
			"li x5, 0x3f\n"
			"csrw tensor_mask, x5\n"	
			"fmaxu.pi f0, f21, f21\n"
			"fmadd.s f15, f21, f15, f19, rmm\n"
			"beq x5, x2, 1f\n"
			"1:\n"
			"fminu.pi f0, f3, f13\n"
			"fswizz.ps f21, f21, 0x12\n"
			"fandi.pi f7, f15, 0x1c4\n"
			"bgeu x7, x13, 1f\n"
			"1:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f0, 736(x16)\n"
			"fcvt.pw.ps f20, f0, rup\n" 
			"fnmadd.s f0, f19, f0, f13, rtz\n"
			"fcvt.ps.f16 f13, f11\n"
			"fmadd.ps f29, f13, f15, f21, rne\n"
			"fsgnj.ps f29, f19, f27\n"
			"fnmadd.ps f29, f21, f0, f15, rdn\n"
			"beq x31, x28, 1f\n"
			"1:\n"
			"li x16, 0x80836a802c\n"
			"amoming.w x19, x13, (x16)\n" 
			"fsat8.pi f1, f7\n"
			"fsgnjx.s f18, f29, f7\n"
			"fcvt.ps.pw f27, f29, rtz\n"
			"fand.pi f18, f13, f15\n"
			"ebreak\n"
			"li x10, 0x99c57ef42233dfe8\n"
			"li x22, 0xce5de01d63f5b673\n"
			"xor x7, x10, x22\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"mulhsu x27, x23, x10\n"
			"bne x20, x5, 1f\n"
			"1:\n"
			"addi x28, x19, 0x15\n"
			"fcvt.wu.s x0, f20, rtz\n"
			"remuw x7, x0, x7\n"
			"fmadd.s f9, f27, f11, f30, rmm\n"
			"csrr x0, mscratch\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fsrai.pi f1, f11, 0xd\n"
			"fsgnj.ps f30, f18, f30\n"
			"fmulh.pi f29, f21, f7\n"
			"bltu x9, x10, 1f\n"
			"1:\n"
			"feq.pi f0, f29, f0\n"
			"fsrai.pi f27, f1, 0x3\n"
			"beq x18, x17, 1f\n"
			"1:\n"
			"li x10, 0x8093cbd000\n"
			"la x27, mem_ptr_offset\n"
			"flw.ps f0, 576(x27)\n"
			"fgbl.ps f7, f0 (x10)\n" 
			"fadd.pi f3, f1, f9\n"
			"fnmsub.ps f27, f19, f21, f27, rmm\n"
			"bge x20, x9, 1f\n"
			"1:\n"
			"fadd.pi f13, f13, f0\n"
			"fmin.ps f29, f22, f19\n"
			"fsub.ps f20, f30, f15, rdn\n"
			"li x10, 0x8090dd302c\n"
			"amomaxul.w x23, x27, (x10)\n" 
			"lui x22, 0x11\n"
			"fmul.ps f9, f1, f21, rne\n"
			"fand.pi f19, f13, f19\n"
			"fcvt.ps.f16 f21, f29\n"
			"fmv.x.s x13, f22\n"
			"li x10, 0x8ceaba0dd34904ff\n"
			"li x13, 0x9b0bf5a067fc6c8b\n"
			"packb x28, x10, x13\n" 
			"fmsub.ps f0, f0, f11, f18, rtz\n"
			"fmulhu.pi f0, f9, f0\n"
			"bne x0, x15, 1f\n"
			"1:\n"
			"fcvt.pw.ps f19, f15, rmm\n"
			"fsub.s f30, f0, f1, rmm\n"
			"flt.ps f15, f11, f20\n"
			"li x13, 0x8081f2802c\n"
			"amoswapg.w x22, x27, (x13)\n" 
			"fclass.ps f0, f29\n"
			"fadd.pi f0, f30, f27\n"
			"fmax.pi f0, f29, f13\n"
			"feq.ps f3, f27, f21\n"
			"fmul.ps f29, f29, f27, rup\n"
			"li x27, 0x80948d4000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f29, (x27)\n"
			"flwl.ps f22, (x27)\n" 
			"fsgnj.s f13, f27, f21\n"
			"ori x0, x0, 0x16\n"
			"fbci.ps f1, 0x1e765\n"
			"fcvt.s.w f18, x0, rtz\n"
			"blt x26, x24, 1f\n"
			"1:\n"
			"fminu.pi f20, f1, f19\n"
			"blt x15, x6, 1f\n"
			"1:\n"
			"li x27, 0x8085e8a028\n"
			"amoswapg.w x16, x10, (x27)\n" 
			"for.pi f1, f29, f21\n"
			"fsgnj.ps f13, f7, f30\n"
			"fcvt.f16.ps f3, f19\n"
			"frcp.ps f30, f29\n"
			"fnmsub.ps f19, f21, f29, f7, rmm\n"
			"li x27, 0x8085d68014\n"
			"amomaxg.w x22, x18, (x27)\n" 
			"fnmsub.ps f15, f0, f9, f19, rdn\n"
			"flt.pi f11, f18, f11\n"
			"fcvt.ps.pwu f7, f1, rne\n"
			"fmin.pi f18, f19, f18\n"
			"fsra.pi f13, f27, f13\n"
			"li x15, 0x8095147018\n"
			"amominl.w x18, x23, (x15)\n" 
			"mov.m.x m6, x28, 0x5f\n"
			"fcvt.s.wu f0, x23, rne\n"
			"fandi.pi f15, f15, 0xfe\n"
			"mulhsu x23, x23, x28\n"
			"fsrli.pi f18, f1, 0xd\n"
			"li x28, 0x8091d24028\n"
			"la x18, mem_ptr_offset\n"
			"flw.ps f30, 544(x18)\n"
			"famoandl.pi f7, f30 (x28)\n" 
			"slt x23, x23, x18\n"
			"slt x7, x27, x7\n"
			"fsrl.pi f20, f18, f15\n"
			"fsrl.pi f22, f15, f29\n"
			"fnmsub.s f9, f1, f9, f29, rne\n"
			"li x22, 0x8051291268\n"
			"la x27, mem_ptr_offset\n"
			"flw.ps f18, 864(x27)\n"
			"fgh.ps f0, f18 (x22)\n" 
			"fcmovm.ps f18, f9, f21\n"
			"fcvt.pw.ps f21, f21, rup\n"
			"fsgnjn.ps f18, f30, f9\n"
			"fsrai.pi f21, f21, 0x1\n"
			"fsll.pi f27, f21, f0\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_0_TLD_ID_0_NMASK_0x1_CMASK_0xfb_SHIRE_0_HID_8:\n"
			"li x31, 0x1fb1e\n"
			"csrw tensor_coop, x31\n" 
			"li x31, 0x01FF000001FF0000\n"
			"csrw tensor_mask, x31\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"li x28, 0xd440008020006b28\n"
			"csrw tensor_load, x28\n" 
			"csrwi tensor_wait, 1\n" 
			"LBL_SYNC_POINT_1_HID_8:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_1_WAIT_HID_8\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_1_EXIT_HID_8\n"
			"LBL_SYNC_POINT_1_WAIT_HID_8:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_1_EXIT_HID_8:\n"          
			"nop\n"
			"LBL_SYNC_POINT_2_HID_8:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_2_WAIT_HID_8\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_2_EXIT_HID_8\n"
			"LBL_SYNC_POINT_2_WAIT_HID_8:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_2_EXIT_HID_8:\n"          
			"nop\n"
			"LBL_SYNC_POINT_3_HID_8:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_3_WAIT_HID_8\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_3_EXIT_HID_8\n"
			"LBL_SYNC_POINT_3_WAIT_HID_8:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_3_EXIT_HID_8:\n"          
			"nop\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_32\n"
			"li x5, 0xa0000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x380000000000009\n"
			"add x6, x6, x31\n"
			"li x7, 0x1b480000001c051\n"
			"li x9, 0xfb2600000000000\n"
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xa\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_8:\n"
			"la x31, self_check_8\n"
			"li x5, 0x60000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, 0x13f00000081f037\n"
			"li x9, 0x293f200000000003\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x3c\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 3\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x8\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f11, 0(x31)\n"
			"flw.ps f15, 0(x31)\n"
			"flw.ps f27, 5(x31)\n"
			"flw.ps f3, 2(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_8:\n"
			"fcvt.pw.ps f11, f11, rup\n"
			"fmax.s f1, f11, f11\n"
			"fsrl.pi f18, f3, f3\n"
			"fclass.ps f15, f11\n"
			"fmul.s f3, f15, f3, rup\n"
			"fmul.pi f7, f3, f27\n"
			"fswizz.ps f18, f11, 0x1e\n"
			"fmsub.s f22, f3, f27, f15, rdn\n"
			"fpackrepb.pi f22, f3\n"
			"fsgnj.ps f19, f27, f11\n"
			"fnmadd.ps f27, f27, f15, f27, rne\n"
			"flt.pi f15, f15, f15\n"
			"fclass.ps f15, f15\n"
			"bge x15, x12, 1f\n"
			"1:\n"
			"fmulhu.pi f7, f3, f3\n"
			"fltu.pi f11, f11, f3\n"
			"beq x23, x17, 1f\n"
			"1:\n"
			"fpackreph.pi f22, f11\n"
			"fround.ps f29, f27, rup\n"
			"bltu x21, x22, 1f\n"
			"1:\n"
			"fexp.ps f7, f15\n"
			"fmsub.s f22, f15, f11, f11, rmm\n"
			"fpackrepb.pi f11, f11\n"
			"fsat8.pi f27, f27\n"
			"fsub.ps f18, f11, f11, rne\n"
			"fcvt.ps.f16 f11, f27\n"
			"fmaxu.pi f15, f15, f11\n"
			"bltu x0, x0, 1f\n"
			"1:\n"
			"fnmsub.ps f7, f15, f15, f11, rne\n"
			"fle.pi f15, f15, f27\n"
			"fsrli.pi f27, f27, 0xa\n"
			"beq x27, x3, 1f\n"
			"1:\n"
			"faddi.pi f18, f27, 0x186\n"
			"fmadd.s f29, f11, f3, f11, rtz\n"
			"feq.pi f19, f27, f15\n"
			"fcvt.pw.ps f1, f27, rne\n"
			"flog.ps f11, f11\n"
			"blt x4, x13, 1f\n"
			"1:\n"
			"fsrai.pi f27, f27, 0x2\n"
			"bltu x26, x30, 1f\n"
			"1:\n"
			"fnmadd.s f7, f11, f27, f11, rne\n"
			"fcvt.ps.f16 f1, f15\n"
			"fsgnj.s f18, f15, f15\n"
			"fminu.pi f11, f11, f27\n"
			"fsrai.pi f22, f15, 0x9\n"
			"fle.ps f27, f11, f27\n"
			"beq x6, x19, 1f\n"
			"1:\n"
			"fltu.pi f29, f27, f11\n"
			"fmsub.s f15, f27, f15, f11, rdn\n"
			"fsub.ps f15, f27, f27, rmm\n"
			"fmin.s f27, f27, f3\n"
			"fpackrepb.pi f3, f15\n"
			"fswizz.ps f7, f11, 0x1f\n"
			"beq x14, x12, 1f\n"
			"1:\n"
			"fnmadd.s f22, f3, f3, f11, rne\n"
			"fnmsub.s f22, f3, f15, f27, rtz\n"
			"bne x5, x17, 1f\n"
			"1:\n"
			"fmulhu.pi f11, f15, f3\n"
			"flt.ps f29, f3, f11\n"
			"fsgnjn.ps f7, f11, f11\n"
			"bltu x26, x29, 1f\n"
			"1:\n"
			"fmax.pi f22, f15, f3\n"
			"fcvt.ps.pw f7, f3, rne\n"
			"fmaxu.pi f27, f3, f27\n"
			"fsll.pi f3, f15, f3\n"
			"fcvt.ps.pw f7, f3, rtz\n"
			"fmulhu.pi f19, f15, f15\n"
			"fnmadd.s f15, f15, f3, f3, rtz\n"
			"beq x1, x22, 1f\n"
			"1:\n"
			"fmax.s f7, f3, f27\n"
			"fmulhu.pi f15, f3, f15\n"
			"fmadd.s f18, f27, f27, f11, rtz\n"
			"bne x13, x12, 1f\n"
			"1:\n"
			"fmax.pi f22, f15, f15\n"
			"fsatu8.pi f15, f11\n"
			"fmul.ps f7, f11, f3, rne\n"
			"fmax.pi f1, f15, f27\n"
			"bgeu x2, x15, 1f\n"
			"1:\n"
			"feq.pi f22, f11, f15\n"
			"frcp.ps f1, f3\n"
			"fmadd.ps f15, f3, f15, f27, rne\n"
			"bne x10, x28, 1f\n"
			"1:\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_0_LOOP_SEQ_HID_8\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_HID_8:\n"
			"la x16, rand_int_32\n"
			"flw.ps f0, 640(x16)\n"
			"flw.ps f9, 160(x16)\n"
			"fdiv.pi f1, f0, f9\n" 
			"fadd.pi f3, f3, f30\n"
			"fbci.ps f18, 0x749b7\n"
			"sub x15, x19, x7\n"
			"fcvt.s.w f29, x19, rdn\n"
			"flt.pi f9, f13, f29\n"
			"li x7, 0x808354f018\n"
			"amomaxg.d x23, x18, (x7)\n" 
			"fmsub.s f11, f9, f21, f13, rup\n"
			"bge x8, x2, 1f\n"
			"1:\n"
			"fsra.pi f11, f21, f9\n"
			"fsgnj.s f20, f9, f21\n"
			"beq x1, x16, 1f\n"
			"1:\n"
			"fsrl.pi f11, f13, f9\n"
			"fmadd.ps f30, f21, f9, f9, rup\n"
			"li x16, 0x80979a1020\n"
			"amoxorl.d x19, x27, (x16)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"feq.pi f27, f27, f20\n"
			"fadd.pi f3, f20, f0\n"
			"fmsub.ps f19, f0, f21, f21, rne\n"
			"frcp.ps f20, f3\n"
			"fmul.s f20, f20, f7, rdn\n"
			"li x23, 0x8096db2000\n"
			"la x15, mem_ptr_offset\n"
			"flw.ps f13, 32(x15)\n"
			"famomaxl.pi f27, f13 (x23)\n" 
			"xor x13, x7, x0\n"
			"fmulh.pi f18, f13, f9\n"
			"remw x0, x0, x0\n"
			"fcvt.wu.s x13, f9, rmm\n"
			"srl x18, x18, x0\n"
			"li x10, 0x8095d6c030\n"
			"amoaddl.d x27, x19, (x10)\n" 
			"slti x0, x22, 0x8\n"
			"fxor.pi f7, f7, f21\n"
			"fmax.ps f3, f29, f21\n"
			"auipc x23, 0x11\n"
			"fmulh.pi f1, f30, f18\n"
			"li x19, 0x8093eda018\n"
			"la x10, mem_ptr_offset\n"
			"flw.ps f3, 96(x10)\n"
			"famomaxl.pi f18, f3 (x19)\n" 
			"fmadd.ps f1, f11, f7, f11, rne\n"
			"fmulh.pi f0, f21, f21\n"
			"mul x15, x15, x28\n"
			"fexp.ps f29, f11\n"
			"addw x23, x13, x15\n"
			"la x22, rand_int_32\n"
			"flw.ps f21, 576(x22)\n"
			"flw.ps f0, 576(x22)\n"
			"for.pi f29, f21, f0\n" 
			"fcmovm.ps f20, f3, f19\n"
			"fmin.pi f29, f20, f29\n"
			"fcmovm.ps f30, f18, f19\n"
			"fmul.s f19, f29, f19, rtz\n"
			"fcvt.ps.pwu f15, f15, rdn\n"
			"la x16, rand_int_32\n"
			"flw.ps f21, 800(x16)\n"
			"flw.ps f13, 544(x16)\n"
			"fxor.pi f1, f21, f13\n" 
			"srai x13, x0, 1\n"
			"sraiw x13, x13, 3\n"
			"fminu.pi f20, f11, f11\n"
			"srliw x18, x27, 5\n"
			"fslli.pi f1, f30, 0xe\n"
			"li x19, 0x8084950000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f29, (x19)\n" 
			"faddi.pi f22, f3, 0x93\n"
			"fcvt.ps.pw f18, f11, rne\n"
			"fsll.pi f21, f21, f7\n"
			"fnmsub.s f7, f7, f20, f20, rne\n"
			"bgeu x11, x9, 1f\n"
			"1:\n"
			"fltu.pi f7, f3, f7\n"
			"li x27, 0x8050947fc0\n"
			"fsw.ps f27, -256(x27)\n"
			"flw.ps f3, -256(x27)\n" 
			"fxor.pi f11, f13, f21\n"
			"bge x9, x10, 1f\n"
			"1:\n"
			"feqm.ps m6, f13, f30\n"
			"ffrc.ps f15, f27\n"
			"fmul.s f1, f7, f19, rup\n"
			"bne x14, x18, 1f\n"
			"1:\n"
			"fmin.s f3, f7, f20\n"
			"li x22, 0xaab710b93a0e3ea3\n"
			"srai x22, x22, 4\n" 
			"fmulhu.pi f15, f29, f15\n"
			"fxor.pi f15, f9, f15\n"
			"fcvt.wu.s x23, f11, rup\n"
			"fltu.pi f9, f22, f19\n"
			"fsatu8.pi f29, f29\n"
			"li x27, 0x80816a9010\n"
			"la x15, mem_ptr_offset\n"
			"flw.ps f27, 96(x15)\n"
			"fghg.ps f19, f27 (x27)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fltm.ps m0, f9, f27\n"
			"mov.m.x m0, x0, 0xff\n"
			"slt x15, x19, x0\n"
			"flt.pi f18, f19, f19\n"
			"mov.m.x m6, x13, 0x6\n"
			"fadd.s f18, f30, f0, rmm\n"
			"blt x30, x17, 1f\n"
			"1:\n"
			"li x22, 0x8090e3e020\n"
			"la x10, mem_ptr_offset\n"
			"flw.ps f11, 160(x10)\n"
			"famoaddl.pi f9, f11 (x22)\n" 
			"fmvs.x.ps x28, f0, 5\n"
			"fmax.ps f22, f11, f1\n"
			"fround.ps f27, f20, rdn\n"
			"bltu x17, x1, 1f\n"
			"1:\n"
			"mulhu x22, x19, x22\n"
			"fsgnjn.ps f30, f30, f27\n"
			"li x16, 0x8091669030\n"
			"amoorl.d x28, x22, (x16)\n" 
			"fadd.s f18, f22, f29, rup\n"
			"fmax.ps f27, f27, f27\n"
			"fswizz.ps f11, f11, 0xd\n"
			"blt x10, x23, 1f\n"
			"1:\n"
			"slt x16, x27, x19\n"
			"fcvt.s.wu f30, x7, rup\n"
			"bne x20, x24, 1f\n"
			"1:\n"
			"li x27, 0x8054c49444\n"
			"sd x28, 512(x27)\n"
			"ld x19, 512(x27)\n" 
			"fnmadd.ps f1, f18, f9, f30, rdn\n"
			"fclass.s x23, f3\n"
			"bge x17, x0, 1f\n"
			"1:\n"
			"feq.ps f9, f20, f20\n"
			"bltu x30, x8, 1f\n"
			"1:\n"
			"fltm.ps m6, f18, f18\n"
			"fsgnj.ps f27, f18, f9\n"
			"fence.i\n" 
			"li x5, 0xae497245900181f8\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xb4e79dd65c53feff\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"srliw x18, x27, 0\n"
			"fcvt.pwu.ps f18, f1, rmm\n"
			"bgeu x23, x24, 1f\n"
			"1:\n"
			"fcvt.ps.pw f30, f0, rne\n"
			"sltu x27, x13, x27\n"
			"fence\n"
			"bltu x20, x23, 1f\n"
			"1:\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_1_TLD_ID_1_NMASK_0x1_CMASK_0xbf_SHIRE_0_HID_8:\n"
			"li x31, 0x1bf0e\n"
			"csrw tensor_coop, x31\n" 
			"li x31, 0x01FF000001FF0000\n"
			"csrw tensor_mask, x31\n" 
			"li x31, (0x3 << 6) | 0x1\n"
			"li x10, 0x4010008020006b0b\n"
			"csrw tensor_load, x10\n" 
			"li x7, 0x18d800000118002\n"
			"csrw tensor_fma, x7\n" 
			"LBL_SYNC_POINT_4_HID_8:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_4_WAIT_HID_8\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_4_EXIT_HID_8\n"
			"LBL_SYNC_POINT_4_WAIT_HID_8:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_4_EXIT_HID_8:\n"          
			"nop\n"
			"LBL_SYNC_POINT_5_HID_8:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_5_WAIT_HID_8\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_5_EXIT_HID_8\n"
			"LBL_SYNC_POINT_5_WAIT_HID_8:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_5_EXIT_HID_8:\n"          
			"nop\n"
			"LBL_SYNC_POINT_6_HID_8:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_6_WAIT_HID_8\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_6_EXIT_HID_8\n"
			"LBL_SYNC_POINT_6_WAIT_HID_8:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_6_EXIT_HID_8:\n"          
			"nop\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, 0x14f9800008908c6\n"
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 2\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x10\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f1, 1(x31)\n"
			"flw.ps f11, 1(x31)\n"
			"flw.ps f13, 0(x31)\n"
			"flw.ps f15, 14(x31)\n"
			"flw.ps f19, 9(x31)\n"
			"flw.ps f20, 20(x31)\n"
			"flw.ps f21, 17(x31)\n"
			"flw.ps f22, 30(x31)\n"
			"flw.ps f27, 20(x31)\n"
			"flw.ps f29, 5(x31)\n"
			"flw.ps f7, 16(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_8:\n"
			"fsrai.pi f21, f21, 0xc\n"
			"fmin.s f1, f13, f1\n"
			"fcvt.ps.f16 f21, f1\n"
			"fandi.pi f0, f0, 0x5a\n"
			"fle.ps f18, f15, f11\n"
			"fsgnjx.s f30, f1, f29\n"
			"for.pi f22, f21, f7\n"
			"flt.ps f30, f13, f15\n"
			"fswizz.ps f30, f21, 0xa\n"
			"fsra.pi f30, f29, f27\n"
			"fmsub.ps f29, f29, f1, f29, rdn\n"
			"fnmsub.s f21, f11, f21, f20, rdn\n"
			"fnmsub.ps f27, f13, f22, f29, rup\n"
			"fslli.pi f19, f19, 0x4\n"
			"fmax.ps f0, f0, f20\n"
			"bne x26, x28, 1f\n"
			"1:\n"
			"fcmov.ps f18, f19, f27, f20\n"
			"fmin.ps f1, f19, f27\n"
			"fmaxu.pi f27, f11, f21\n"
			"frcp.ps f27, f11\n"
			"fmax.s f27, f7, f13\n"
			"fround.ps f7, f1, rdn\n"
			"bne x25, x25, 1f\n"
			"1:\n"
			"fsatu8.pi f22, f19\n"
			"fmax.ps f7, f19, f29\n"
			"fnmsub.s f7, f27, f21, f13, rmm\n"
			"beq x17, x16, 1f\n"
			"1:\n"
			"fswizz.ps f20, f22, 0x11\n"
			"fmaxu.pi f13, f15, f20\n"
			"fmaxu.pi f11, f27, f1\n"
			"flog.ps f1, f7\n"
			"bltu x17, x27, 1f\n"
			"1:\n"
			"fround.ps f27, f22, rtz\n"
			"fmulhu.pi f15, f20, f29\n"
			"fsgnj.s f29, f22, f29\n"
			"fmulhu.pi f0, f13, f22\n"
			"fcvt.ps.f16 f18, f20\n"
			"fmul.pi f13, f0, f11\n"
			"fsub.pi f11, f13, f7\n"
			"flt.ps f0, f0, f7\n"
			"bgeu x15, x22, 1f\n"
			"1:\n"
			"feq.ps f15, f21, f15\n"
			"blt x11, x12, 1f\n"
			"1:\n"
			"fnmsub.s f20, f11, f0, f19, rmm\n"
			"fmin.pi f21, f27, f11\n"
			"fslli.pi f29, f29, 0xc\n"
			"fswizz.ps f11, f20, 0x11\n"
			"fle.ps f21, f11, f29\n"
			"bne x4, x1, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f21, f29, rne\n"
			"fsra.pi f11, f20, f7\n"
			"bge x3, x10, 1f\n"
			"1:\n"
			"fltu.pi f29, f29, f20\n"
			"fpackrepb.pi f22, f11\n"
			"bne x13, x24, 1f\n"
			"1:\n"
			"fadd.pi f7, f0, f7\n"
			"fsll.pi f29, f29, f29\n"
			"beq x28, x24, 1f\n"
			"1:\n"
			"fmin.ps f11, f11, f1\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_1_LOOP_SEQ_HID_8\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_HID_8:\n"
			"li x23, 0x8090642018\n"
			"amoorl.d x16, x7, (x23)\n" 
			"fnmadd.s f21, f18, f21, f30, rup\n"
			"fle.pi f21, f1, f1\n"
			"fmulhu.pi f29, f3, f27\n"
			"fmul.s f18, f1, f11, rdn\n"
			"blt x5, x23, 1f\n"
			"1:\n"
			"fcmov.ps f29, f29, f11, f11\n"
			"fadd.ps f1, f29, f18, rtz\n"
			"li x23, 0x230654340e1d7e0d\n"
			"li x19, 0x4d5b7d2d57b06367\n"
			"or x23, x23, x19\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fcvt.ps.pwu f15, f29, rne\n"
			"slti x28, x22, 0x5\n"
			"fbci.ps f20, 0x514d2\n"
			"fcvt.w.s x19, f29, rdn\n"
			"fsgnjn.ps f18, f3, f9\n"
			"sllw x10, x15, x18\n"
			"li x18, 0x8053379190\n"
			"li x16, 3504006898\n"
			"fg32h.ps f9, x15 (x18)\n" 
			"fadd.pi f3, f1, f3\n"
			"fmax.s f3, f7, f21\n"
			"fsgnjn.ps f27, f21, f27\n"
			"fcvt.pwu.ps f1, f0, rmm\n"
			"fle.ps f9, f7, f21\n"
			"fmax.ps f21, f21, f1\n"
			"li x7, 0x809398802c\n"
			"amoorl.w x15, x10, (x7)\n" 
			"li x5, 0xd096\n"
			"csrw tensor_mask, x5\n"	
			"fmadd.ps f11, f30, f11, f7, rup\n"
			"bge x20, x31, 1f\n"
			"1:\n"
			"fcmovm.ps f9, f13, f7\n"
			"flem.ps m5, f21, f27\n"
			"fmulhu.pi f18, f18, f21\n"
			"bltu x16, x25, 1f\n"
			"1:\n"
			"fmulhu.pi f18, f11, f13\n"
			"fsgnj.s f27, f18, f27\n"
			"blt x22, x21, 1f\n"
			"1:\n"
			"li x15, 0x80859ff008\n"
			"la x23, mem_ptr_offset\n"
			"flw.ps f9, 256(x23)\n"
			"famoandg.pi f0, f9 (x15)\n" 
			"fsgnjx.ps f27, f18, f18\n"
			"fsgnj.s f21, f22, f27\n"
			"fmadd.s f21, f20, f29, f27, rup\n"
			"fmaxu.pi f20, f20, f18\n"
			"fsub.pi f18, f3, f29\n"
			"fsrl.pi f11, f19, f9\n"
			"li x19, 0x8082504038\n"
			"amominug.d x15, x23, (x19)\n" 
			"fslli.pi f29, f27, 0x2\n"
			"blt x2, x29, 1f\n"
			"1:\n"
			"fmsub.s f18, f21, f0, f1, rdn\n"
			"fsra.pi f11, f29, f1\n"
			"feq.s x16, f21, f18\n"
			"fmsub.ps f22, f15, f21, f13, rdn\n"
			"beq x22, x13, 1f\n"
			"1:\n"
			"fsrli.pi f7, f22, 0x0\n"
			"bge x3, x1, 1f\n"
			"1:\n"
			"li x16, 0x8084afe00c\n"
			"amomaxug.w x10, x7, (x16)\n" 
			"fmax.s f30, f20, f18\n"
			"fcvt.pwu.ps f30, f13, rup\n"
			"bne x15, x24, 1f\n"
			"1:\n"
			"fnot.pi f7, f13\n"
			"frcp.ps f7, f18\n"
			"fmin.pi f30, f20, f30\n"
			"beq x8, x13, 1f\n"
			"1:\n"
			"fle.ps f15, f18, f30\n"
			"li x23, 0x8084d2b018\n"
			"la x19, mem_ptr_offset\n"
			"flw.ps f1, 576(x19)\n"
			"famoorg.pi f19, f1 (x23)\n" 
			"fmul.s f15, f0, f30, rdn\n"
			"fmulh.pi f1, f15, f7\n"
			"fsgnjn.s f1, f30, f18\n"
			"fadd.ps f15, f0, f7, rmm\n"
			"fltu.pi f18, f15, f7\n"
			"fmin.ps f15, f7, f0\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 544(x15)\n"
			"flw.ps f22, 992(x15)\n"
			"fmin.s f1, f9, f22\n" 
			"fsgnjx.s f20, f29, f19\n"
			"fadd.pi f29, f29, f19\n"
			"fround.ps f29, f19, rdn\n"
			"fnot.pi f29, f7\n"
			"fmsub.ps f30, f15, f29, f27, rmm\n"
			"fadd.ps f29, f7, f7, rup\n"
			"li x23, 0x8056ff582c\n"
			"sd x15, 512(x23)\n"
			"lw x13, 512(x23)\n" 
			"fmulh.pi f20, f29, f29\n"
			"fcvt.s.wu f9, x7, rdn\n"
			"fsrai.pi f9, f21, 0xd\n"
			"srai x28, x7, 5\n"
			"fmsub.ps f29, f29, f3, f3, rne\n"
			"fbcx.ps f13, x7\n"
			"li x18, 0x8095c7a008\n"
			"la x23, mem_ptr_offset\n"
			"flw.ps f1, 192(x23)\n"
			"fgbl.ps f3, f1 (x18)\n" 
			"fmax.ps f27, f15, f27\n"
			"bltu x16, x10, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f20, f11, rne\n"
			"fcvt.f16.ps f30, f7\n"
			"andi x0, x13, 0x2\n"
			"bne x23, x27, 1f\n"
			"1:\n"
			"fmv.s.x f3, x18\n"
			"fadd.pi f7, f27, f9\n"
			"bgeu x13, x12, 1f\n"
			"1:\n"
			"li x22, 0x8085fba014\n"
			"amoming.w x15, x15, (x22)\n" 
			"fsrli.pi f0, f0, 0x9\n"
			"fnmsub.s f0, f0, f19, f30, rne\n"
			"fmulhu.pi f18, f19, f29\n"
			"flt.ps f0, f22, f0\n"
			"fadd.ps f29, f27, f22, rne\n"
			"ecall\n"
			"li x18, 0x8085e1c03c\n"
			"amomaxug.w x10, x23, (x18)\n" 
			"fle.ps f20, f20, f11\n"
			"feq.pi f21, f19, f22\n"
			"ori x19, x7, 0x7\n"
			"fandi.pi f29, f19, 0x1a3\n"
			"beq x8, x23, 1f\n"
			"1:\n"
			"fcvt.wu.s x18, f3, rmm\n"
			"fsra.pi f9, f18, f27\n"
			"li x23, 0x8081f41008\n"
			"la x7, mem_ptr_offset\n"
			"flw.ps f7, 224(x7)\n"
			"famoming.ps f22, f7 (x23)\n" 
			"fle.s x27, f3, f7\n"
			"fmulhu.pi f9, f3, f30\n"
			"bne x14, x18, 1f\n"
			"1:\n"
			"fmin.pi f3, f11, f11\n"
			"fmulhu.pi f3, f30, f7\n"
			"masknot m4, m4\n"
			"blt x21, x31, 1f\n"
			"1:\n"
			"fmin.s f9, f3, f11\n"
			"li x19, 0x8053771198\n"
			"li x27, 3694540713\n"
			"fsc32w.ps f3, x13 (x19)\n" 
			"fsat8.pi f7, f13\n"
			"for.pi f18, f20, f3\n"
			"bge x9, x12, 1f\n"
			"1:\n"
			"fcvt.f16.ps f7, f3\n"
			"bne x29, x24, 1f\n"
			"1:\n"
			"fmsub.ps f0, f3, f20, f3, rup\n"
			"fsgnj.s f18, f20, f18\n"
			"fswizz.ps f7, f20, 0x19\n"
			"li x23, 0x80855d0000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f15, (x23)\n"
			"flwg.ps f21, (x23)\n" 
			"fminu.pi f19, f11, f3\n"
			"fsrli.pi f11, f21, 0x5\n"
			"fnot.pi f11, f1\n"
			"fmul.s f3, f11, f19, rne\n"
			"fcvt.pw.ps f30, f3, rne\n"
			"beq x20, x6, 1f\n"
			"1:\n"
			"fmaxu.pi f11, f11, f1\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, 0x60008050004245\n"
			"li x6, 0x10008050008007\n"
			"li x7, 0x1ab80000051a037\n"
			"li x9, 0x19af000000000059\n"
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 2\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x30\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f19, 0(x31)\n"
			"flw.ps f21, 2(x31)\n"
			"flw.ps f7, 6(x31)\n"
			"li x31, 0x2\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_8:\n"
			"fmaxu.pi f7, f21, f7\n"
			"fsrai.pi f13, f21, 0xf\n"
			"frcp.ps f13, f19\n"
			"fminu.pi f0, f7, f21\n"
			"fsatu8.pi f19, f19\n"
			"fsatu8.pi f19, f19\n"
			"fand.pi f19, f21, f19\n"
			"fnmadd.s f18, f7, f19, f19, rup\n"
			"fclass.ps f21, f21\n"
			"bltu x3, x27, 1f\n"
			"1:\n"
			"fmax.pi f0, f7, f7\n"
			"fcmovm.ps f21, f21, f21\n"
			"bne x17, x24, 1f\n"
			"1:\n"
			"fsub.s f13, f7, f7, rmm\n"
			"fsgnjx.ps f18, f19, f21\n"
			"bgeu x3, x7, 1f\n"
			"1:\n"
			"fcvt.pw.ps f30, f19, rup\n"
			"flt.ps f21, f21, f21\n"
			"fcvt.pwu.ps f19, f19, rdn\n"
			"fnmsub.ps f18, f7, f19, f21, rdn\n"
			"fle.ps f21, f21, f19\n"
			"flt.ps f19, f19, f19\n"
			"fminu.pi f18, f7, f7\n"
			"fcvt.ps.pw f21, f19, rdn\n"
			"ffrc.ps f19, f19\n"
			"bne x28, x21, 1f\n"
			"1:\n"
			"fsgnjn.s f18, f7, f21\n"
			"faddi.pi f7, f7, 0x146\n"
			"frcp.ps f0, f19\n"
			"bltu x27, x0, 1f\n"
			"1:\n"
			"fcvt.pw.ps f30, f7, rup\n"
			"feq.pi f0, f21, f21\n"
			"fle.pi f30, f19, f21\n"
			"fmin.s f30, f21, f19\n"
			"flt.ps f19, f7, f19\n"
			"fsgnjx.s f30, f19, f7\n"
			"fmulh.pi f18, f21, f19\n"
			"fcmovm.ps f13, f7, f7\n"
			"fcvt.f16.ps f0, f19\n"
			"fmin.s f7, f21, f7\n"
			"fnmadd.s f21, f19, f19, f21, rne\n"
			"bne x2, x8, 1f\n"
			"1:\n"
			"fsll.pi f13, f21, f21\n"
			"bgeu x13, x25, 1f\n"
			"1:\n"
			"frcp.ps f19, f7\n"
			"fsgnjn.s f30, f19, f21\n"
			"fle.pi f30, f7, f7\n"
			"fcvt.ps.f16 f7, f7\n"
			"bne x29, x9, 1f\n"
			"1:\n"
			"fmul.s f19, f21, f19, rup\n"
			"fmsub.s f13, f21, f21, f21, rtz\n"
			"fadd.pi f13, f21, f21\n"
			"fswizz.ps f21, f7, 0x14\n"
			"fmul.pi f18, f7, f19\n"
			"fmax.pi f30, f19, f7\n"
			"fsgnj.ps f7, f21, f7\n"
			"fle.ps f18, f19, f19\n"
			"fcvt.pwu.ps f30, f19, rne\n"
			"fmin.ps f0, f7, f19\n"
			"fmulhu.pi f13, f7, f7\n"
			"fcvt.ps.pw f21, f21, rne\n"
			"fnmsub.ps f30, f19, f21, f19, rtz\n"
			"feq.pi f18, f21, f21\n"
			"bne x1, x19, 1f\n"
			"1:\n"
			"fnmadd.s f18, f21, f19, f7, rne\n"
			"fcvt.ps.f16 f19, f19\n"
			"fsub.s f7, f7, f19, rdn\n"
			"fadd.ps f7, f21, f7, rmm\n"
			"fnmadd.ps f18, f7, f19, f7, rmm\n"
			"fadd.pi f18, f21, f21\n"
			"fsgnjx.s f18, f21, f21\n"
			"fsgnj.ps f18, f7, f7\n"
			"frcp.ps f13, f7\n"
			"fcvt.f16.ps f18, f21\n"
			"fpackrepb.pi f18, f21\n"
			"beq x14, x20, 1f\n"
			"1:\n"
			"fcvt.ps.pw f21, f21, rup\n"
			"fcvt.ps.pw f21, f19, rmm\n"
			"fsgnjn.ps f21, f21, f19\n"
			"fpackreph.pi f18, f21\n"
			"fswizz.ps f18, f21, 0x1b\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_2_LOOP_SEQ_HID_8\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f16,  224 (x5)\n"
			"flw.ps f6,  0 (x5)\n"
			"flw.ps f24,  800 (x5)\n"
			"flw.ps f5,  160 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f14,  480 (x5)\n"
			"flw.ps f7,  448 (x5)\n"
			"flw.ps f22,  512 (x5)\n"
			"flw.ps f2,  576 (x5)\n"
			"flw.ps f28,  672 (x5)\n"
			"flw.ps f4,  128 (x5)\n"
			"flw.ps f12,  544 (x5)\n"
			"flw.ps f18,  96 (x5)\n"
			"flw.ps f17,  992 (x5)\n"
			"flw.ps f0,  64 (x5)\n"
			"flw.ps f21,  736 (x5)\n"
			"flw.ps f27,  864 (x5)\n"
			"flw.ps f31,  384 (x5)\n"
			"flw.ps f26,  960 (x5)\n"
			"flw.ps f30,  288 (x5)\n"
			"flw.ps f9,  768 (x5)\n"
			"flw.ps f11,  416 (x5)\n"
			"flw.ps f8,  704 (x5)\n"
			"flw.ps f19,  320 (x5)\n"
			"flw.ps f10,  608 (x5)\n"
			"flw.ps f3,  352 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f23,  256 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f15,  928 (x5)\n"
			"flw.ps f13,  896 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_3_HID_8:\n"
			"li x18, 0x809226d020\n"
			"amoaddl.d x18, x28, (x18)\n" 
			"li x15, 0x8053a76043\n"
			"sd x27, 256(x15)\n"
			"lw x16, 256(x15)\n" 
			"li x15, 0x8080243008\n"
			"la x18, mem_ptr_offset\n"
			"flw.ps f0, 160(x18)\n"
			"fgwg.ps f1, f0 (x15)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"li x23, 0x8090678010\n"
			"la x7, mem_ptr_offset\n"
			"flw.ps f21, 416(x7)\n"
			"famoorl.pi f20, f21 (x23)\n" 
			"li x23, 0x8080336020\n"
			"amominug.d x7, x10, (x23)\n" 
			"li x19, 0x8086381000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f9, (x19)\n"
			"flwg.ps f3, (x19)\n" 
			"li x27, 0x805159a684\n"
			"sh x16, 256(x27)\n" 
			"li x19, 0x80869b6030\n"
			"amominug.d x19, x13, (x19)\n" 
			"li x10, 0x80505b5a9d\n"
			"sd x18, 256(x10)\n"
			"ld x13, 256(x10)\n" 
			"csrr x0, mhpmevent6\n"
			"la x19, rand_int_32\n"
			"flw.ps f0, 736(x19)\n"
			"faddi.pi f22, f0, 0x1cc\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 320(x19)\n"
			"fcvt.lu.s x22, f27, rdn\n" 
			"li x23, 0x8055f26130\n"
			"li x16, 1920126416\n"
			"fsc32w.ps f1, x22 (x23)\n" 
			"li x7, 0x805665d85f\n"
			"sh x18, -256(x7)\n" 
			"li x23, 0x8090d41038\n"
			"la x22, mem_ptr_offset\n"
			"flw.ps f30, 704(x22)\n"
			"famoswapl.pi f30, f30 (x23)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"li x16, 0x8092c51020\n"
			"la x18, mem_ptr_offset\n"
			"flw.ps f20, 0(x18)\n"
			"famomaxul.pi f19, f20 (x16)\n" 
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, 0xc00080500063c6\n"
			"li x6, 0x3bc0008050000840\n"
			"li x7, 0x1b000000001e062\n"
			"li x10, 0x5c11008050000840\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"li x31, (0x3 << 6)\n"
			"csrwi tensor_wait, 7\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_1_HID_8:\n"
			"li x5, 0x38e0008050000845\n"
			"li x6, 0x2b40008050008004\n"
			"li x7, 0x12a00000001a072\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 3\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x80\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f13, 4(x31)\n"
			"flw.ps f15, 8(x31)\n"
			"flw.ps f19, 11(x31)\n"
			"flw.ps f27, 14(x31)\n"
			"flw.ps f29, 17(x31)\n"
			"flw.ps f7, 18(x31)\n"
			"flw.ps f9, 26(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_3_LOOP_SEQ_HID_8:\n"
			"fnmadd.ps f0, f0, f7, f9, rne\n"
			"fsra.pi f22, f27, f13\n"
			"fmadd.ps f15, f15, f15, f0, rmm\n"
			"feq.pi f7, f27, f7\n"
			"fxor.pi f9, f9, f9\n"
			"fmax.ps f7, f0, f13\n"
			"fmadd.s f19, f15, f29, f19, rne\n"
			"fslli.pi f19, f0, 0x4\n"
			"bne x4, x25, 1f\n"
			"1:\n"
			"flt.pi f22, f15, f13\n"
			"blt x30, x28, 1f\n"
			"1:\n"
			"fandi.pi f18, f9, 0x1e4\n"
			"fcmov.ps f19, f13, f13, f9\n"
			"fnmsub.s f9, f9, f13, f0, rne\n"
			"fmul.ps f27, f27, f7, rtz\n"
			"fsrl.pi f21, f0, f15\n"
			"bne x7, x7, 1f\n"
			"1:\n"
			"fsrl.pi f29, f9, f9\n"
			"fmul.s f18, f13, f15, rmm\n"
			"fsgnj.ps f21, f19, f27\n"
			"bge x11, x23, 1f\n"
			"1:\n"
			"for.pi f15, f27, f9\n"
			"fsll.pi f15, f0, f7\n"
			"fcvt.f16.ps f29, f15\n"
			"fmax.pi f21, f27, f0\n"
			"flt.pi f21, f29, f15\n"
			"fmul.pi f19, f7, f27\n"
			"fmaxu.pi f0, f0, f13\n"
			"fsub.ps f29, f29, f27, rne\n"
			"fcvt.f16.ps f29, f15\n"
			"bne x25, x10, 1f\n"
			"1:\n"
			"fsgnj.ps f19, f0, f27\n"
			"fsrl.pi f9, f29, f15\n"
			"bne x12, x23, 1f\n"
			"1:\n"
			"fclass.ps f29, f19\n"
			"fsgnjn.ps f18, f9, f19\n"
			"fsgnjx.s f29, f15, f13\n"
			"fmax.s f9, f15, f15\n"
			"fadd.pi f21, f15, f9\n"
			"fsrli.pi f18, f27, 0x7\n"
			"fcvt.pw.ps f9, f7, rne\n"
			"fclass.ps f29, f0\n"
			"fandi.pi f22, f19, 0x9f\n"
			"blt x4, x30, 1f\n"
			"1:\n"
			"fmulh.pi f29, f27, f9\n"
			"fsrl.pi f22, f29, f15\n"
			"fsgnjx.s f0, f19, f9\n"
			"fsat8.pi f29, f13\n"
			"fmadd.s f15, f13, f19, f29, rne\n"
			"fcvt.ps.pw f29, f29, rmm\n"
			"fcvt.ps.f16 f7, f7\n"
			"fsrl.pi f15, f19, f13\n"
			"fcvt.pw.ps f18, f27, rne\n"
			"fsatu8.pi f27, f27\n"
			"bge x20, x9, 1f\n"
			"1:\n"
			"fpackreph.pi f22, f13\n"
			"flt.pi f18, f15, f13\n"
			"feq.pi f15, f19, f0\n"
			"faddi.pi f21, f29, 0xb8\n"
			"fcvt.ps.f16 f29, f29\n"
			"fcvt.f16.ps f19, f9\n"
			"bltu x15, x24, 1f\n"
			"1:\n"
			"fmax.pi f19, f19, f15\n"
			"fmul.ps f29, f7, f29, rmm\n"
			"fsgnj.ps f18, f13, f19\n"
			"fltu.pi f29, f29, f15\n"
			"fltu.pi f21, f19, f9\n"
			"for.pi f15, f0, f13\n"
			"flt.ps f27, f19, f15\n"
			"ffrc.ps f15, f15\n"
			"fsrai.pi f21, f13, 0x7\n"
			"fminu.pi f29, f0, f15\n"
			"fmul.pi f15, f29, f27\n"
			"bgeu x22, x15, 1f\n"
			"1:\n"
			"fnmadd.ps f22, f7, f15, f13, rup\n"
			"fmadd.ps f18, f29, f27, f0, rdn\n"
			"fsra.pi f29, f27, f19\n"
			"beq x19, x12, 1f\n"
			"1:\n"
			"fmin.s f27, f29, f27\n"
			"fadd.s f29, f27, f9, rup\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_3_LOOP_SEQ_HID_8\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_4_HID_8:\n"
			"li x28, 0x8053c5d228\n"
			"fsq2 f7, -512(x28)\n" 
			"mulh x19, x23, x0\n"
			"slliw x0, x18, 5\n"
			"li x16, 0x8091c6d030\n"
			"la x28, mem_ptr_offset\n"
			"flw.ps f1, 896(x28)\n"
			"famoswapl.pi f30, f1 (x16)\n" 
			"fxor.pi f27, f27, f18\n"
			"sllw x10, x10, x28\n"
			"li x19, 0x80855c8020\n"
			"la x7, mem_ptr_offset\n"
			"flw.ps f1, 160(x7)\n"
			"fgbg.ps f21, f1 (x19)\n" 
			"fsgnj.ps f27, f22, f27\n"
			"fmul.s f21, f7, f27, rne\n"
			"bge x4, x12, 1f\n"
			"1:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f0, 576(x16)\n"
			"flw.ps f13, 224(x16)\n"
			"flw.ps f1, 672(x16)\n"
			"fnmadd.ps f21, f0, f13, f1, rne\n" 
			"li x5, 0xe62a08e4419ee44d\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x4f3bb1f4e275ec05\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"mulhu x23, x7, x23\n"
			"srlw x0, x27, x0\n"
			"li x10, 0x8095530010\n"
			"la x16, mem_ptr_offset\n"
			"flw.ps f7, 288(x16)\n"
			"famoorl.pi f3, f7 (x10)\n" 
			"fsgnj.s f9, f7, f7\n"
			"maskpopcz x18, m6\n"
			"li x22, 0x805261014d\n"
			"sd x19, 128(x22)\n"
			"lhu x7, 128(x22)\n" 
			"fsub.pi f21, f11, f3\n"
			"fadd.pi f13, f18, f9\n"
			"li x19, 0x8057dbafe8\n"
			"fsq2 f20, -256(x19)\n" 
			"maskpopc x16, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"fmul.pi f19, f30, f19\n"
			"li x10, 0x8080f40030\n"
			"amoxorg.d x19, x28, (x10)\n" 
			"fpackreph.pi f19, f19\n"
			"srai x15, x23, 2\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f1, 32(x7)\n"
			"flw.ps f0, 736(x7)\n"
			"fcmovm.ps f9, f1, f0\n" 
			"fcvt.ps.f16 f0, f15\n"
			"fle.pi f3, f3, f9\n"
			"beq x8, x18, 1f\n"
			"1:\n"
			"csrr x0, instret\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fmadd.s f7, f7, f29, f3, rdn\n"
			"srlw x23, x23, x27\n"
			"la x23, rand_int_32\n"
			"flw.ps f9, 640(x23)\n"
			"fnot.pi f29, f9\n" 
			"fsrai.pi f3, f13, 0x9\n"
			"for.pi f19, f19, f7\n"
			"li x18, 0x8053a7ff69\n"
			"sb x18, -512(x18)\n" 
			"fcvt.ps.pw f13, f22, rtz\n"
			"remw x16, x18, x16\n"
			"bltu x24, x8, 1f\n"
			"1:\n"
			"li x10, 0x8057976378\n"
			"la x28, mem_ptr_offset\n"
			"flw.ps f18, 640(x28)\n"
			"fsch.ps f0, f18 (x10)\n" 
			"fandi.pi f7, f7, 0x1ff\n"
			"bgeu x2, x20, 1f\n"
			"1:\n"
			"fcmov.ps f7, f0, f11, f11\n"
			"li x16, 0x805163fb28\n"
			"la x18, mem_ptr_offset\n"
			"flw.ps f0, 64(x18)\n"
			"fgh.ps f11, f0 (x16)\n" 
			"addw x27, x19, x27\n"
			"fmvz.x.ps x10, f20, 4\n"
			"li x10, 0x80929f9028\n"
			"amoswapl.d x23, x27, (x10)\n" 
			"fmvz.x.ps x15, f3, 4\n"
			"bne x10, x30, 1f\n"
			"1:\n"
			"fence\n"
			"beq x28, x22, 1f\n"
			"1:\n"
			"li x18, 0x8085152020\n"
			"amoaddg.w x10, x16, (x18)\n" 
			"fsgnjn.ps f19, f3, f19\n"
			"fnmsub.ps f7, f19, f13, f7, rmm\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, 0x3100000444fa6\n"
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_8:\n"
			"li x5, 0x27e80000257c77\n"
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_2_HID_8:\n"
			"li x5, 0x186000000aa840\n"
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 3\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xa0\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f18, 0(x31)\n"
			"flw.ps f7, 4(x31)\n"
			"flw.ps f9, 0(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_4_LOOP_SEQ_HID_8:\n"
			"fclass.ps f9, f9\n"
			"fpackrepb.pi f15, f9\n"
			"fminu.pi f15, f18, f18\n"
			"fcvt.f16.ps f9, f7\n"
			"fxor.pi f7, f9, f7\n"
			"fnmsub.ps f15, f7, f7, f7, rne\n"
			"fmul.s f18, f9, f18, rup\n"
			"fcvt.pw.ps f22, f7, rmm\n"
			"beq x23, x21, 1f\n"
			"1:\n"
			"fexp.ps f15, f9\n"
			"fsll.pi f18, f7, f18\n"
			"fmadd.s f22, f7, f9, f18, rtz\n"
			"fmsub.s f18, f18, f18, f9, rup\n"
			"fexp.ps f15, f18\n"
			"fnmadd.ps f7, f18, f9, f7, rtz\n"
			"fnmadd.ps f22, f7, f9, f7, rne\n"
			"fsub.pi f7, f18, f7\n"
			"fsgnjx.ps f7, f18, f7\n"
			"fcmov.ps f15, f18, f9, f7\n"
			"fmax.ps f15, f9, f9\n"
			"fcvt.pw.ps f15, f7, rup\n"
			"fmaxu.pi f9, f9, f18\n"
			"fminu.pi f7, f7, f7\n"
			"fsgnjx.ps f22, f9, f9\n"
			"fadd.pi f9, f9, f18\n"
			"fsgnjx.ps f15, f7, f18\n"
			"fsatu8.pi f7, f9\n"
			"beq x21, x5, 1f\n"
			"1:\n"
			"fsatu8.pi f18, f18\n"
			"fxor.pi f9, f7, f7\n"
			"fcvt.f16.ps f9, f18\n"
			"fsub.s f7, f7, f7, rup\n"
			"fmax.ps f18, f18, f18\n"
			"feq.ps f22, f18, f18\n"
			"fsub.s f9, f9, f9, rup\n"
			"feq.ps f22, f9, f9\n"
			"fsgnjx.ps f9, f9, f7\n"
			"bltu x1, x1, 1f\n"
			"1:\n"
			"fsgnj.ps f15, f7, f18\n"
			"fmin.pi f15, f9, f7\n"
			"fnmadd.s f22, f18, f7, f9, rmm\n"
			"fnmadd.ps f22, f7, f9, f18, rtz\n"
			"fcvt.f16.ps f15, f9\n"
			"faddi.pi f22, f7, 0x180\n"
			"fsgnj.s f9, f7, f9\n"
			"fexp.ps f18, f18\n"
			"fmul.pi f9, f7, f9\n"
			"fcvt.pw.ps f9, f18, rup\n"
			"fsll.pi f15, f7, f7\n"
			"fsrl.pi f18, f9, f18\n"
			"fsat8.pi f18, f9\n"
			"fsrli.pi f9, f7, 0x4\n"
			"for.pi f15, f9, f9\n"
			"feq.ps f9, f18, f9\n"
			"fmax.s f15, f18, f18\n"
			"faddi.pi f15, f9, 0x162\n"
			"fmul.pi f15, f9, f7\n"
			"blt x31, x28, 1f\n"
			"1:\n"
			"fmax.ps f9, f9, f9\n"
			"fadd.pi f7, f18, f9\n"
			"feq.pi f18, f7, f7\n"
			"feq.ps f9, f18, f7\n"
			"fadd.pi f9, f7, f18\n"
			"fadd.pi f15, f18, f7\n"
			"fnmsub.s f18, f18, f9, f9, rne\n"
			"frcp.ps f22, f18\n"
			"bgeu x27, x7, 1f\n"
			"1:\n"
			"flt.pi f9, f7, f7\n"
			"fsra.pi f22, f7, f18\n"
			"fround.ps f9, f18, rtz\n"
			"fle.ps f18, f7, f18\n"
			"fswizz.ps f18, f18, 0xe\n"
			"frcp.ps f7, f7\n"
			"fmul.s f22, f18, f9, rne\n"
			"fmadd.ps f15, f18, f9, f7, rmm\n"
			"fpackrepb.pi f15, f9\n"
			"fsra.pi f22, f9, f18\n"
			"fmul.ps f18, f18, f18, rne\n"
			"fround.ps f22, f9, rne\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_4_LOOP_SEQ_HID_8\n"
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f3,  256 (x5)\n"
			"flw.ps f7,  992 (x5)\n"
			"flw.ps f14,  384 (x5)\n"
			"flw.ps f24,  128 (x5)\n"
			"flw.ps f9,  0 (x5)\n"
			"flw.ps f30,  864 (x5)\n"
			"flw.ps f0,  896 (x5)\n"
			"flw.ps f4,  736 (x5)\n"
			"flw.ps f8,  32 (x5)\n"
			"flw.ps f28,  832 (x5)\n"
			"flw.ps f5,  64 (x5)\n"
			"flw.ps f11,  768 (x5)\n"
			"flw.ps f18,  608 (x5)\n"
			"flw.ps f22,  416 (x5)\n"
			"flw.ps f13,  288 (x5)\n"
			"flw.ps f23,  672 (x5)\n"
			"flw.ps f1,  480 (x5)\n"
			"flw.ps f15,  544 (x5)\n"
			"flw.ps f21,  576 (x5)\n"
			"flw.ps f25,  224 (x5)\n"
			"flw.ps f16,  704 (x5)\n"
			"flw.ps f12,  96 (x5)\n"
			"flw.ps f19,  640 (x5)\n"
			"flw.ps f20,  928 (x5)\n"
			"flw.ps f31,  448 (x5)\n"
			"flw.ps f2,  512 (x5)\n"
			"flw.ps f27,  960 (x5)\n"
			"flw.ps f26,  800 (x5)\n"
			"flw.ps f17,  352 (x5)\n"
			"flw.ps f29,  160 (x5)\n"
			"flw.ps f10,  192 (x5)\n"
			"flw.ps f6,  320 (x5)\n"
			"la x31, check_sum_hid_8\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"LBL_POSTAMBLE_HID_8:\n"          
			"csrwi   tensor_wait, 0x0\n"
			"csrwi   tensor_wait, 0x1\n"
			"csrwi   tensor_wait, 0x2\n"
			"csrwi   tensor_wait, 0x3\n"
			"csrwi   tensor_wait, 0x4\n"
			"csrwi   tensor_wait, 0x5\n"
			"csrwi   tensor_wait, 0x6\n"
			"csrwi   tensor_wait, 0x7\n"
			"csrwi   tensor_wait, 0x8\n"
			"csrwi   tensor_wait, 0x9\n"
			"csrwi   tensor_wait, 0xa\n"
			"fence\n"
			"LBL_SYNC_POINT_7_HID_8:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_7_WAIT_HID_8\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_7_EXIT_HID_8\n"
			"LBL_SYNC_POINT_7_WAIT_HID_8:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_7_EXIT_HID_8:\n"          
			"nop\n"
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_8\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_8:\n"          
			".global LBL_FAIL_HID_8\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_8\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_8:\n"          
			".global LBL_PASS_HID_8\n"          
			VSNIP_RSV
		);
		c_test_pass();
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_8:\n"          
			".global LBL_HALT_HID_8\n"          
			"wfi\n"
			"j LBL_HALT_HID_8\n"          
			VSNIP_RSV
		);
		return 0;       
	};
	if ( hid == 9 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_9:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_9\n"
			"csrw mscratch, x5\n"
			"csrwi menable_shadows, 0xf\n"
			"li x5, -1\n"
			"csrw mcounteren, x5\n"
			"csrw scounteren, x5\n"
			"csrwi mhpmcounter3, 0\n"
			"csrwi mhpmcounter4, 0\n"
			"csrwi mhpmcounter5, 0\n"
			"csrwi mhpmcounter6, 0\n"
			"csrwi mhpmcounter7, 0\n"
			"csrwi mhpmcounter8, 0\n"
			"csrwi mhpmevent3, 7\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 9\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 23\n"   
			"mov.m.x m0, x0, 0xff\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f20,  480 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f14,  704 (x5)\n"
			"flw.ps f5,  928 (x5)\n"
			"flw.ps f19,  576 (x5)\n"
			"flw.ps f22,  64 (x5)\n"
			"flw.ps f8,  544 (x5)\n"
			"flw.ps f4,  160 (x5)\n"
			"flw.ps f1,  768 (x5)\n"
			"flw.ps f11,  288 (x5)\n"
			"flw.ps f23,  864 (x5)\n"
			"flw.ps f9,  0 (x5)\n"
			"flw.ps f10,  352 (x5)\n"
			"flw.ps f12,  96 (x5)\n"
			"flw.ps f29,  192 (x5)\n"
			"flw.ps f16,  32 (x5)\n"
			"flw.ps f6,  640 (x5)\n"
			"flw.ps f25,  416 (x5)\n"
			"flw.ps f30,  224 (x5)\n"
			"flw.ps f3,  672 (x5)\n"
			"flw.ps f2,  608 (x5)\n"
			"flw.ps f31,  992 (x5)\n"
			"flw.ps f27,  320 (x5)\n"
			"flw.ps f13,  384 (x5)\n"
			"flw.ps f26,  896 (x5)\n"
			"flw.ps f0,  960 (x5)\n"
			"flw.ps f17,  128 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f18,  512 (x5)\n"
			"flw.ps f7,  256 (x5)\n"
			"flw.ps f15,  832 (x5)\n"
			"flw.ps f28,  448 (x5)\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_HID_9:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_0_WAIT_HID_9\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_0_EXIT_HID_9\n"
			"LBL_SYNC_POINT_0_WAIT_HID_9:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_EXIT_HID_9:\n"          
			"nop\n"
			"LBL_KERNEL_HID_9:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_9\n"
			"li x5, 0x6\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_HID_9:\n"
			"li x23, 0xedfd9cf16c536b56\n"
			"li x21, 0xa4794201870352f\n"
			"remuw x17, x23, x21\n" 
			"remu x14, x28, x14\n"
			"bne x25, x25, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f16, f29, rtz\n"
			"blt x30, x2, 1f\n"
			"1:\n"
			"sraw x28, x28, x17\n"
			"auipc x0, 0x11\n"
			"li x22, 0xc79217470afa01bc\n"
			"li x17, 0x773c8510e2979544\n"
			"remw x27, x22, x17\n" 
			"fsgnjx.s f14, f7, f2\n"
			"fadd.s f12, f30, f7, rmm\n"
			"fclass.ps f13, f13\n"
			"fadd.s f19, f8, f11, rtz\n"
			"li x28, 0x809b6c4000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f14, (x28)\n"
			"flwl.ps f12, (x28)\n" 
			"fmaxu.pi f7, f7, f3\n"
			"fcvt.ps.pw f28, f3, rne\n"
			"feq.s x24, f12, f27\n"
			"mov.m.x m2, x28, 0x5a\n"
			"maskpopcz x27, m2\n" 
			"fcvt.s.wu f12, x14, rne\n"
			"bge x15, x26, 1f\n"
			"1:\n"
			"fexp.ps f16, f3\n"
			"addi x0, x7, 0x1a\n"
			"fsgnjx.s f3, f16, f3\n"
			"masknot m5, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"divuw x23, x7, x19\n"
			"flem.ps m3, f1, f19\n"
			"fcvt.pw.ps f14, f14, rup\n"
			"bltu x22, x21, 1f\n"
			"1:\n"
			"mulhu x17, x0, x22\n"
			"beq x7, x26, 1f\n"
			"1:\n"
			"li x7, 0x808a525000\n"
			"la x23, mem_ptr_offset\n"
			"flw.ps f30, 0(x23)\n"
			"famoaddg.pi f14, f30 (x7)\n" 
			"ffrc.ps f15, f29\n"
			"fround.ps f28, f28, rmm\n"
			"fandi.pi f16, f11, 0x2f\n"
			"fsrl.pi f29, f14, f14\n"
			"li x20, 0x805cc62d70\n"
			"sd x27, -128(x20)\n"
			"lh x21, -128(x20)\n" 
			"fand.pi f28, f19, f16\n"
			"bge x30, x17, 1f\n"
			"1:\n"
			"fminu.pi f28, f13, f1\n"
			"fcvt.ps.pwu f12, f14, rmm\n"
			"fltu.pi f12, f1, f19\n"
			"li x22, 0x808d7d8000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f2, (x22)\n" 
			"fmul.pi f1, f19, f28\n"
			"bge x22, x18, 1f\n"
			"1:\n"
			"fxor.pi f27, f28, f19\n"
			"fpackreph.pi f27, f19\n"
			"beq x7, x17, 1f\n"
			"1:\n"
			"fmulhu.pi f27, f28, f28\n"
			"li x17, 0x808c1b4028\n"
			"la x22, mem_ptr_offset\n"
			"flw.ps f30, 704(x22)\n"
			"fscwg.ps f27, f30 (x17)\n" 
			"lui x23, 0x4\n"
			"sraiw x0, x23, 3\n"
			"fandi.pi f19, f30, 0x2c\n"
			"fcmov.ps f19, f11, f12, f1\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f7, 768(x20)\n"
			"fcvt.wu.s x7, f7, rne\n" 
			"ffrc.ps f30, f7\n"
			"beq x2, x21, 1f\n"
			"1:\n"
			"fmin.pi f28, f30, f16\n"
			"flt.ps f2, f28, f30\n"
			"feq.ps f15, f30, f15\n"
			"la x21, rand_int_32\n"
			"flw.ps f27, 96(x21)\n"
			"fsrli.pi f3, f27, 0xd\n" 
			"addiw x23, x23, 0x17\n"
			"fmin.ps f8, f27, f11\n"
			"fsll.pi f29, f7, f12\n"
			"frcp.ps f27, f1\n"
			"li x19, 0x8088a29028\n"
			"amoaddg.w x24, x23, (x19)\n" 
			"fltu.pi f28, f28, f13\n"
			"fmulhu.pi f28, f1, f1\n"
			"fminu.pi f1, f19, f1\n"
			"fsub.pi f19, f28, f19\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f30, 416(x28)\n"
			"fround.ps f14, f30, rne\n" 
			"fmulhu.pi f28, f7, f19\n"
			"ffrc.ps f8, f8\n"
			"divw x19, x22, x19\n"
			"fcvt.wu.s x21, f30, rtz\n"
			"li x17, 0x805a2f2290\n"
			"la x28, mem_ptr_offset\n"
			"flw.ps f27, 960(x28)\n"
			"fscw.ps f11, f27 (x17)\n" 
			"and x21, x23, x21\n"
			"bge x23, x6, 1f\n"
			"1:\n"
			"maskpopc x17, m5\n"
			"fsrli.pi f14, f11, 0x1\n"
			"ebreak\n"
			"li x19, 0x808ce61008\n"
			"amoandg.d x23, x27, (x19)\n" 
			"fmulh.pi f16, f1, f3\n"
			"fsra.pi f2, f11, f3\n"
			"fand.pi f19, f11, f11\n"
			"bgeu x27, x10, 1f\n"
			"1:\n"
			"fpackreph.pi f2, f28\n"
			"li x23, 0x808ff46020\n"
			"amoxorg.w x23, x7, (x23)\n" 
			"fmul.s f12, f7, f14, rtz\n"
			"fbcx.ps f2, x19\n"
			"fmin.ps f2, f3, f29\n"
			"fmvz.x.ps x28, f16, 2\n"
			"bne x21, x14, 1f\n"
			"1:\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x8\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f14, 1(x31)\n"
			"flw.ps f15, 5(x31)\n"
			"flw.ps f16, 6(x31)\n"
			"flw.ps f19, 14(x31)\n"
			"flw.ps f29, 18(x31)\n"
			"flw.ps f30, 0(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_9:\n"
			"fsgnjx.ps f15, f15, f15\n"
			"fmulhu.pi f12, f29, f30\n"
			"blt x5, x31, 1f\n"
			"1:\n"
			"fmsub.ps f14, f16, f14, f1, rdn\n"
			"fclass.ps f19, f14\n"
			"fcvt.f16.ps f19, f29\n"
			"faddi.pi f11, f29, 0x4\n"
			"fmax.s f15, f29, f1\n"
			"fadd.pi f15, f14, f30\n"
			"fsat8.pi f12, f30\n"
			"fsll.pi f15, f30, f15\n"
			"fmin.pi f15, f30, f15\n"
			"fcmov.ps f16, f19, f29, f19\n"
			"fle.pi f16, f15, f29\n"
			"fsub.s f15, f16, f29, rne\n"
			"fmaxu.pi f16, f15, f14\n"
			"fsgnjx.ps f14, f30, f1\n"
			"fadd.ps f28, f14, f14, rne\n"
			"fsatu8.pi f12, f16\n"
			"fnot.pi f15, f16\n"
			"flog.ps f15, f16\n"
			"fnmadd.ps f28, f29, f29, f14, rup\n"
			"bne x13, x26, 1f\n"
			"1:\n"
			"fsrl.pi f15, f15, f30\n"
			"fswizz.ps f14, f14, 0x11\n"
			"fadd.ps f19, f19, f29, rne\n"
			"fmaxu.pi f15, f19, f30\n"
			"fltu.pi f28, f16, f1\n"
			"fexp.ps f11, f30\n"
			"fsat8.pi f15, f15\n"
			"fnmsub.s f30, f15, f30, f14, rmm\n"
			"fmaxu.pi f16, f14, f16\n"
			"blt x24, x9, 1f\n"
			"1:\n"
			"fmax.s f16, f16, f1\n"
			"bne x22, x17, 1f\n"
			"1:\n"
			"fmulh.pi f28, f1, f1\n"
			"fcmovm.ps f14, f30, f30\n"
			"fsrai.pi f14, f15, 0x1\n"
			"bltu x18, x26, 1f\n"
			"1:\n"
			"fsgnj.s f28, f19, f19\n"
			"bge x14, x10, 1f\n"
			"1:\n"
			"fsra.pi f15, f15, f30\n"
			"fsgnjn.s f28, f30, f29\n"
			"fmsub.s f29, f1, f29, f29, rup\n"
			"fnot.pi f12, f1\n"
			"fsgnjx.ps f15, f29, f29\n"
			"fsgnjx.ps f28, f15, f30\n"
			"fsll.pi f28, f15, f29\n"
			"fxor.pi f16, f16, f30\n"
			"fsrli.pi f1, f1, 0x1\n"
			"bgeu x11, x6, 1f\n"
			"1:\n"
			"flt.pi f1, f19, f29\n"
			"fcvt.ps.f16 f14, f29\n"
			"fsrai.pi f12, f30, 0x6\n"
			"fround.ps f12, f1, rdn\n"
			"feq.pi f12, f15, f30\n"
			"fpackreph.pi f29, f1\n"
			"fslli.pi f15, f15, 0x3\n"
			"fmin.ps f29, f14, f19\n"
			"frcp.ps f12, f29\n"
			"fnmadd.s f14, f15, f14, f16, rtz\n"
			"fmul.pi f29, f29, f15\n"
			"fnmsub.s f12, f15, f19, f29, rne\n"
			"beq x10, x27, 1f\n"
			"1:\n"
			"fclass.ps f12, f16\n"
			"blt x8, x26, 1f\n"
			"1:\n"
			"fsra.pi f12, f16, f1\n"
			"blt x10, x12, 1f\n"
			"1:\n"
			"fmulhu.pi f16, f15, f1\n"
			"fadd.pi f19, f29, f30\n"
			"fle.ps f16, f29, f14\n"
			"fsra.pi f19, f15, f15\n"
			"for.pi f16, f15, f16\n"
			"fmsub.ps f28, f29, f1, f1, rne\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_0_LOOP_SEQ_HID_9\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_HID_9:\n"
			"li x27, 0x8098297010\n"
			"amoswapl.w x14, x23, (x27)\n" 
			"fmul.ps f1, f1, f16, rne\n"
			"beq x3, x6, 1f\n"
			"1:\n"
			"fmv.x.s x19, f7\n"
			"addiw x20, x28, 0x4\n"
			"bge x6, x31, 1f\n"
			"1:\n"
			"fadd.pi f30, f12, f7\n"
			"flt.ps f15, f16, f19\n"
			"bltu x25, x0, 1f\n"
			"1:\n"
			"auipc x19, 0x6\n"
			"fcvt.ps.pw f1, f16, rdn\n"
			"addw x20, x21, x20\n"
			"li x20, 0x809b50800c\n"
			"amomaxl.w x27, x23, (x20)\n" 
			"remw x17, x24, x17\n"
			"blt x15, x13, 1f\n"
			"1:\n"
			"fsgnj.s f30, f28, f28\n"
			"bge x23, x5, 1f\n"
			"1:\n"
			"fslli.pi f15, f29, 0xa\n"
			"fltm.ps m4, f11, f29\n"
			"fmadd.ps f2, f29, f29, f29, rup\n"
			"xori x28, x21, 0x4\n"
			"fmvs.x.ps x24, f11, 0\n"
			"masknot m3, m3\n"
			"li x17, 0x808e74f030\n"
			"la x28, mem_ptr_offset\n"
			"flw.ps f15, 960(x28)\n"
			"famoming.pi f12, f15 (x17)\n" 
			"fsub.ps f8, f12, f8, rdn\n"
			"fsrl.pi f16, f16, f2\n"
			"bge x12, x16, 1f\n"
			"1:\n"
			"fsgnj.s f27, f8, f3\n"
			"fmul.ps f12, f29, f30, rdn\n"
			"fmax.pi f11, f8, f14\n"
			"beq x31, x13, 1f\n"
			"1:\n"
			"fmadd.s f1, f16, f28, f27, rmm\n"
			"fxor.pi f3, f1, f1\n"
			"fmin.ps f27, f27, f16\n"
			"li x19, 0x809b926000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f19, (x19)\n" 
			"slt x0, x0, x27\n"
			"fnot.pi f3, f2\n"
			"bne x9, x12, 1f\n"
			"1:\n"
			"fmin.pi f2, f30, f15\n"
			"blt x0, x30, 1f\n"
			"1:\n"
			"fslli.pi f16, f2, 0xb\n"
			"srli x24, x21, 3\n"
			"fclass.ps f27, f29\n"
			"fmul.s f16, f13, f14, rmm\n"
			"beq x10, x8, 1f\n"
			"1:\n"
			"flt.pi f8, f14, f2\n"
			"la x22, rand_int_32\n"
			"flw.ps f27, 800(x22)\n"
			"fsatu8.pi f7, f27\n" 
			"fround.ps f16, f12, rtz\n"
			"faddi.pi f3, f30, 0xee\n"
			"flt.pi f15, f1, f15\n"
			"fcmovm.ps f12, f2, f12\n"
			"fnmsub.ps f30, f3, f3, f13, rup\n"
			"fpackreph.pi f29, f27\n"
			"fsgnjn.ps f19, f3, f3\n"
			"fmaxu.pi f13, f27, f1\n"
			"li x7, 0x809ac1e008\n"
			"la x28, mem_ptr_offset\n"
			"flw.ps f11, 288(x28)\n"
			"famominul.pi f27, f11 (x7)\n" 
			"rem x23, x7, x23\n"
			"xor x7, x7, x7\n"
			"fcvt.ps.pwu f2, f30, rup\n"
			"slli x20, x23, 1\n"
			"fsetm.pi m2, f2\n"
			"ecall\n"
			"flem.ps m2, f30, f15\n"
			"fcvt.ps.pwu f2, f2, rne\n"
			"bltu x9, x1, 1f\n"
			"1:\n"
			"li x23, 0x805a5b422e\n"
			"sd x27, 512(x23)\n"
			"ld x7, 512(x23)\n" 
			"fround.ps f12, f12, rup\n"
			"fclass.ps f12, f29\n"
			"beq x1, x28, 1f\n"
			"1:\n"
			"fnmsub.s f13, f13, f16, f13, rne\n"
			"fcvt.ps.pw f13, f28, rup\n"
			"fpackrepb.pi f7, f29\n"
			"fsgnjx.ps f30, f15, f8\n"
			"bge x16, x13, 1f\n"
			"1:\n"
			"fsgnjn.s f28, f2, f13\n"
			"fcmovm.ps f2, f2, f13\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f16, 288(x27)\n"
			"flw.ps f11, 352(x27)\n"
			"fmax.s f2, f16, f11\n" 
			"fmul.pi f8, f27, f13\n"
			"fmax.s f16, f16, f3\n"
			"flt.pi f7, f27, f27\n"
			"fminu.pi f27, f27, f27\n"
			"fmax.pi f15, f11, f3\n"
			"fpackrepb.pi f12, f3\n"
			"fxor.pi f15, f13, f3\n"
			"fcvt.f16.ps f8, f30\n"
			"li x28, 0x805a80cb59\n"
			"sh x14, 128(x28)\n" 
			"fle.pi f30, f3, f14\n"
			"bge x4, x2, 1f\n"
			"1:\n"
			"fnot.pi f15, f7\n"
			"for.pi f30, f13, f12\n"
			"fcmov.ps f14, f7, f3, f30\n"
			"fmax.s f7, f3, f2\n"
			"fmax.pi f16, f3, f13\n"
			"fsat8.pi f29, f2\n"
			"fmin.ps f14, f14, f30\n"
			"li x28, 0x808f2ac028\n"
			"amoorg.w x20, x24, (x28)\n" 
			"fmadd.ps f28, f2, f7, f29, rtz\n"
			"fnmsub.s f16, f8, f16, f19, rup\n"
			"fsgnjx.s f29, f16, f27\n"
			"fcvt.pwu.ps f30, f19, rdn\n"
			"faddi.pi f1, f7, 0xcc\n"
			"bge x16, x16, 1f\n"
			"1:\n"
			"fcvt.pw.ps f28, f28, rtz\n"
			"fcmovm.ps f12, f15, f14\n"
			"fnmadd.s f8, f13, f3, f30, rmm\n"
			"li x19, 0x80981bd030\n"
			"amoxorl.d x21, x27, (x19)\n" 
			"fmul.s f11, f2, f12, rdn\n"
			"bge x13, x11, 1f\n"
			"1:\n"
			"fle.ps f12, f27, f12\n"
			"blt x12, x21, 1f\n"
			"1:\n"
			"fmin.ps f7, f29, f28\n"
			"bltu x6, x30, 1f\n"
			"1:\n"
			"fsgnjx.ps f29, f29, f28\n"
			"fsub.pi f28, f3, f28\n"
			"fnmsub.ps f16, f7, f14, f29, rtz\n"
			"feq.pi f3, f3, f29\n"
			"fnot.pi f8, f28\n"
			"li x27, 0xdfbc90d1cee6be8e\n"
			"li x7, 0xfe7b58da8f44e666\n"
			"sllw x20, x27, x7\n" 
			"packb x23, x23, x22\n"
			"frcp.ps f13, f3\n"
			"addw x14, x20, x14\n"
			"bgeu x18, x11, 1f\n"
			"1:\n"
			"or x14, x27, x19\n"
			"fsub.pi f7, f1, f1\n"
			"srliw x14, x24, 0\n"
			"fle.ps f29, f1, f29\n"
			"fmulh.pi f15, f13, f3\n"
			"bge x13, x25, 1f\n"
			"1:\n"
			"li x22, 0x808b98c038\n"
			"la x17, mem_ptr_offset\n"
			"flw.ps f29, 640(x17)\n"
			"famomaxg.ps f8, f29 (x22)\n" 
			"fcvt.w.s x23, f28, rne\n"
			"fadd.pi f2, f1, f1\n"
			"fpackrepb.pi f3, f12\n"
			"masknot m2, m5\n"
			"fbcx.ps f30, x27\n"
			"addw x28, x28, x0\n"
			"bltu x2, x28, 1f\n"
			"1:\n"
			"fcvt.ps.pw f13, f28, rup\n"
			"andi x17, x17, 0xe\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 480(x27)\n"
			"fexp.ps f14, f28\n" 
			"fmul.s f14, f27, f14, rtz\n"
			"beq x9, x20, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f12, f30\n"
			"fmin.pi f19, f14, f1\n"
			"srai x21, x24, 2\n"
			"addw x21, x7, x27\n"
			"bge x16, x19, 1f\n"
			"1:\n"
			"ffrc.ps f12, f14\n"
			"fsatu8.pi f14, f14\n"
			"bne x2, x15, 1f\n"
			"1:\n"
			"addw x22, x27, x24\n"
			"li x31, 0x8030004a00\n"
			"li x20, 0x80000000000000f\n"
			"add x20, x31, x20\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrwi tensor_wait, 6\n" 
			"csrwi tensor_wait, 5\n" 
			"fmsub.s f2, f13, f8, f13, rtz\n"
			"blt x3, x6, 1f\n"
			"1:\n"
			"faddi.pi f16, f13, 0x18e\n"
			"fcmov.ps f3, f7, f30, f16\n"
			"fsgnjn.ps f13, f30, f2\n"
			"fcmov.ps f29, f29, f7, f30\n"
			"fmin.ps f27, f27, f12\n"
			"fxor.pi f13, f14, f13\n"
			"fswizz.ps f14, f27, 0x10\n"
			"li x17, 0x808da09008\n"
			"amomaxg.w x20, x14, (x17)\n" 
			"flt.ps f29, f11, f29\n"
			"mulw x22, x21, x22\n"
			"bgeu x17, x22, 1f\n"
			"1:\n"
			"fle.ps f2, f27, f12\n"
			"fandi.pi f19, f30, 0x146\n"
			"ffrc.ps f1, f27\n"
			"mulhsu x7, x20, x7\n"
			"lui x28, 0x5\n"
			"fadd.ps f3, f2, f3, rmm\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x40\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f2, 0(x31)\n"
			"flw.ps f29, 2(x31)\n"
			"flw.ps f30, 0(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x17, 1(x31)\n"
			"lw x24, 2(x31)\n"
			"lw x28, 3(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_9:\n"
			"fmvs.x.ps x17, f30, 7\n"
			"subw x22, x28, x24\n"
			"mulw x22, x24, x28\n"
			"feq.pi f13, f30, f30\n"
			"sraw x28, x28, x24\n"
			"divu x28, x24, x28\n"
			"flem.ps m4, f2, f30\n"
			"packb x22, x17, x0\n"
			"fsra.pi f28, f30, f2\n"
			"mulh x0, x28, x0\n"
			"fsgnj.s f13, f30, f30\n"
			"mov.m.x m3, x17, 0x4\n"
			"fswizz.ps f28, f30, 0x12\n"
			"bgeu x8, x15, 1f\n"
			"1:\n"
			"frcp.ps f2, f2\n"
			"fmv.s.x f27, x28\n"
			"fmul.ps f27, f2, f29, rdn\n"
			"fle.ps f30, f30, f2\n"
			"slli x0, x0, 0\n"
			"fcvt.ps.pw f27, f30, rmm\n"
			"fsrl.pi f13, f30, f2\n"
			"fmax.ps f28, f30, f29\n"
			"ori x28, x0, 0x18\n"
			"fminu.pi f28, f30, f30\n"
			"addi x17, x28, 0x6\n"
			"blt x6, x5, 1f\n"
			"1:\n"
			"maskpopcz x23, m4\n"
			"mova.m.x x17\n"
			"xor x0, x0, x0\n"
			"fmin.pi f27, f30, f29\n"
			"fsll.pi f27, f29, f29\n"
			"fmsub.ps f30, f2, f30, f2, rup\n"
			"blt x26, x0, 1f\n"
			"1:\n"
			"fxor.pi f13, f29, f29\n"
			"slli x0, x0, 1\n"
			"fsgnjx.s f30, f29, f2\n"
			"andi x28, x28, 0x1e\n"
			"remuw x7, x17, x0\n"
			"sltu x7, x0, x24\n"
			"xor x28, x24, x28\n"
			"fcmov.ps f27, f30, f29, f30\n"
			"fpackreph.pi f27, f30\n"
			"add x23, x17, x17\n"
			"fcvt.ps.pw f28, f29, rmm\n"
			"remu x17, x17, x28\n"
			"flt.ps f28, f29, f2\n"
			"fadd.s f30, f29, f2, rtz\n"
			"bgeu x30, x19, 1f\n"
			"1:\n"
			"sllw x7, x17, x17\n"
			"fcvt.f16.ps f30, f29\n"
			"fmvz.x.ps x0, f2, 0\n"
			"blt x16, x7, 1f\n"
			"1:\n"
			"fsll.pi f28, f30, f30\n"
			"masknot m2, m4\n"
			"bltu x10, x18, 1f\n"
			"1:\n"
			"fmax.s f30, f30, f30\n"
			"fclass.ps f2, f2\n"
			"auipc x17, 0xb\n"
			"bgeu x16, x4, 1f\n"
			"1:\n"
			"flt.ps f13, f2, f29\n"
			"remw x0, x17, x0\n"
			"add x22, x0, x24\n"
			"bne x26, x19, 1f\n"
			"1:\n"
			"srlw x28, x24, x24\n"
			"beq x5, x24, 1f\n"
			"1:\n"
			"packb x28, x28, x24\n"
			"blt x19, x5, 1f\n"
			"1:\n"
			"fmin.pi f27, f29, f29\n"
			"fclass.s x28, f30\n"
			"mulhu x7, x17, x24\n"
			"fxor.pi f28, f2, f2\n"
			"fle.pi f2, f30, f2\n"
			"fmadd.s f27, f30, f2, f30, rdn\n"
			"flem.ps m2, f30, f30\n"
			"sll x22, x24, x0\n"
			"bge x6, x31, 1f\n"
			"1:\n"
			"sllw x17, x28, x17\n"
			"faddi.pi f28, f2, 0xc2\n"
			"fpackreph.pi f2, f2\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_1_LOOP_SEQ_HID_9\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_HID_9:\n"
			"li x20, 0xe592008944669ca8\n"
			"li x22, 0xd2c6b1be69cb6f9\n"
			"remuw x20, x20, x22\n" 
			"fsgnjx.s f8, f19, f12\n"
			"sll x0, x0, x23\n"
			"bltu x17, x5, 1f\n"
			"1:\n"
			"mul x21, x7, x27\n"
			"beq x12, x6, 1f\n"
			"1:\n"
			"li x7, 0x805c2416f0\n"
			"fsw.ps f13, 256(x7)\n"
			"fbc.ps f2, 256(x7)\n" 
			"fltm.pi m3, f16, f8\n"
			"fcvt.s.w f1, x20, rup\n"
			"remuw x23, x27, x14\n"
			"bne x0, x19, 1f\n"
			"1:\n"
			"li x22, 0x80896de000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f1, (x22)\n"
			"flwg.ps f29, (x22)\n" 
			"fsub.ps f13, f12, f7, rne\n"
			"fcvt.f16.ps f28, f13\n"
			"fmul.s f1, f16, f14, rne\n"
			"bltu x16, x20, 1f\n"
			"1:\n"
			"li x14, 0x809e5c300c\n"
			"amoxorl.w x23, x20, (x14)\n" 
			"maskpopc x14, m2\n"
			"mulhsu x14, x28, x28\n"
			"fmsub.ps f15, f15, f8, f19, rmm\n"
			"li x19, 0x808b4ae030\n"
			"la x24, mem_ptr_offset\n"
			"flw.ps f7, 768(x24)\n"
			"fschg.ps f3, f7 (x19)\n" 
			"sraiw x21, x14, 0\n"
			"fcmov.ps f11, f1, f2, f1\n"
			"sltu x24, x14, x24\n"
			"li x17, 0x805ff21a00\n"
			"fsw.ps f8, 512(x17)\n"
			"flw.ps f14, 512(x17)\n" 
			"remu x7, x23, x7\n"
			"sltiu x19, x19, 0x1c\n"
			"fmul.ps f3, f7, f19, rtz\n"
			"li x20, 0x805ee529a8\n"
			"li x17, 772073177\n"
			"fsc32b.ps f19, x19 (x20)\n" 
			"fmsub.ps f1, f13, f28, f11, rne\n"
			"add x22, x19, x22\n"
			"maskor m3, m4, m5\n"
			"bne x25, x22, 1f\n"
			"1:\n"
			"la x24, rand_ieee754_32\n"
			"flw.ps f14, 832(x24)\n"
			"flw.ps f1, 704(x24)\n"
			"flw.ps f12, 320(x24)\n"
			"fnmsub.s f2, f14, f1, f12, rdn\n" 
			"masknot m3, m3\n"
			"fcvt.wu.s x23, f12, rup\n"
			"ecall\n"
			"li x22, 0x809fab0028\n"
			"amoxorl.w x23, x14, (x22)\n" 
			"fnmsub.s f30, f7, f3, f1, rne\n"
			"fcvt.f16.ps f7, f1\n"
			"fsgnjn.ps f14, f29, f3\n"
			"li x23, 0x6b2abe8086e33f4c\n"
			"sraiw x22, x23, 4\n" 
			"ffrc.ps f7, f16\n"
			"or x21, x19, x20\n"
			"packb x23, x22, x23\n"
			"maskor m2, m5, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fslli.pi f27, f12, 0xe\n"
			"blt x5, x13, 1f\n"
			"1:\n"
			"fsgnj.s f16, f2, f16\n"
			"fmul.pi f19, f14, f7\n"
			"li x24, 0x8089807000\n"
			"amoming.d x22, x27, (x24)\n" 
			"fandi.pi f3, f3, 0xae\n"
			"bge x29, x21, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f7, f12, rdn\n"
			"for.pi f29, f19, f14\n"
			"li x27, 0x809dff0010\n"
			"la x23, mem_ptr_offset\n"
			"flw.ps f16, 640(x23)\n"
			"famoswapl.pi f28, f16 (x27)\n" 
			"srlw x20, x28, x14\n"
			"fmvz.x.ps x14, f8, 7\n"
			"bgeu x22, x19, 1f\n"
			"1:\n"
			"fle.pi f1, f16, f1\n"
			"la x22, rand_int_32\n"
			"flw.ps f14, 512(x22)\n"
			"flw.ps f3, 800(x22)\n"
			"fsrl.pi f11, f14, f3\n" 
			"fmax.ps f16, f30, f30\n"
			"fmul.s f15, f29, f13, rmm\n"
			"fence\n"
			"li x28, 0x808fc81018\n"
			"amoaddg.w x20, x28, (x28)\n" 
			"feq.ps f16, f11, f8\n"
			"sllw x22, x7, x19\n"
			"sraw x22, x19, x0\n"
			"csrr x0, mhartid\n"
			"fswizz.ps f12, f7, 0x1d\n"
			"fcvt.f16.ps f1, f28\n"
			"fbci.pi f2, 0x526b5\n"
			"beq x8, x14, 1f\n"
			"1:\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x60\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f12, 0(x31)\n"
			"flw.ps f13, 1(x31)\n"
			"flw.ps f16, 1(x31)\n"
			"flw.ps f19, 9(x31)\n"
			"flw.ps f27, 14(x31)\n"
			"flw.ps f28, 6(x31)\n"
			"flw.ps f3, 4(x31)\n"
			"flw.ps f30, 0(x31)\n"
			"flw.ps f7, 25(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x14, 4(x31)\n"
			"lw x17, 2(x31)\n"
			"lw x19, 5(x31)\n"
			"lw x20, 8(x31)\n"
			"lw x21, 18(x31)\n"
			"lw x22, 2(x31)\n"
			"lw x23, 28(x31)\n"
			"lw x24, 11(x31)\n"
			"lw x27, 29(x31)\n"
			"lw x28, 2(x31)\n"
			"lw x7, 21(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_9:\n"
			"fmax.s f13, f13, f3\n"
			"maskxor m2, m2, m4\n"
			"ebreak\n"
			"fence\n"
			"fsgnjx.s f16, f16, f19\n"
			"fbcx.ps f2, x24\n"
			"bltu x15, x0, 1f\n"
			"1:\n"
			"ebreak\n"
			"fmul.pi f12, f30, f12\n"
			"feq.pi f19, f30, f27\n"
			"andi x14, x7, 0xd\n"
			"fmadd.ps f27, f27, f30, f30, rdn\n"
			"bne x12, x31, 1f\n"
			"1:\n"
			"fsrli.pi f7, f7, 0xb\n"
			"fcvt.pwu.ps f19, f16, rne\n"
			"fmulhu.pi f30, f19, f19\n"
			"bge x27, x8, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f7, f7\n"
			"fmulhu.pi f11, f16, f16\n"
			"addi x17, x19, 0x6\n"
			"fnmsub.ps f2, f13, f30, f16, rtz\n"
			"fmadd.s f11, f7, f27, f19, rne\n"
			"ecall\n"
			"fbci.pi f8, 0x48351\n"
			"srlw x14, x28, x21\n"
			"fand.pi f7, f16, f16\n"
			"slt x27, x0, x21\n"
			"fmul.pi f30, f27, f19\n"
			"fsgnjx.s f2, f3, f30\n"
			"bltu x23, x17, 1f\n"
			"1:\n"
			"fswizz.ps f16, f7, 0xb\n"
			"bltu x7, x27, 1f\n"
			"1:\n"
			"flt.s x24, f16, f27\n"
			"bltu x7, x8, 1f\n"
			"1:\n"
			"srlw x24, x0, x14\n"
			"bne x27, x18, 1f\n"
			"1:\n"
			"and x21, x22, x28\n"
			"fround.ps f12, f13, rne\n"
			"beq x16, x22, 1f\n"
			"1:\n"
			"fsgnjn.s f27, f28, f27\n"
			"fcvt.pw.ps f19, f19, rtz\n"
			"fsll.pi f28, f28, f27\n"
			"bgeu x9, x11, 1f\n"
			"1:\n"
			"maskxor m5, m3, m4\n"
			"blt x9, x4, 1f\n"
			"1:\n"
			"fmulhu.pi f8, f28, f28\n"
			"sra x14, x14, x23\n"
			"srlw x27, x24, x7\n"
			"flt.pi f3, f3, f3\n"
			"flt.ps f30, f12, f30\n"
			"sraiw x19, x17, 4\n"
			"ecall\n"
			"fmul.ps f27, f13, f16, rup\n"
			"fcvt.pw.ps f19, f7, rup\n"
			"maskpopcz x27, m5\n"
			"fmax.s f28, f28, f12\n"
			"mova.x.m x21\n"
			"fbcx.ps f30, x21\n"
			"beq x28, x23, 1f\n"
			"1:\n"
			"divu x19, x19, x20\n"
			"ecall\n"
			"fcmovm.ps f30, f27, f27\n"
			"bltu x13, x17, 1f\n"
			"1:\n"
			"auipc x21, 0x16\n"
			"blt x6, x17, 1f\n"
			"1:\n"
			"fadd.ps f27, f13, f28, rdn\n"
			"fsat8.pi f28, f28\n"
			"fnot.pi f16, f16\n"
			"bne x22, x22, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f12, f30\n"
			"subw x20, x27, x28\n"
			"blt x4, x0, 1f\n"
			"1:\n"
			"feq.ps f16, f19, f12\n"
			"bgeu x29, x5, 1f\n"
			"1:\n"
			"fmsub.s f28, f16, f13, f19, rne\n"
			"fmvz.x.ps x23, f27, 7\n"
			"bltu x19, x3, 1f\n"
			"1:\n"
			"fclass.ps f12, f16\n"
			"beq x20, x3, 1f\n"
			"1:\n"
			"mul x21, x20, x28\n"
			"bge x6, x12, 1f\n"
			"1:\n"
			"fsra.pi f28, f30, f16\n"
			"slt x17, x17, x24\n"
			"bne x21, x5, 1f\n"
			"1:\n"
			"slliw x28, x28, 2\n"
			"bne x19, x20, 1f\n"
			"1:\n"
			"mulh x23, x23, x7\n"
			"fmin.pi f30, f30, f27\n"
			"fsrl.pi f30, f12, f30\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_2_LOOP_SEQ_HID_9\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f15,  32 (x5)\n"
			"flw.ps f2,  224 (x5)\n"
			"flw.ps f17,  256 (x5)\n"
			"flw.ps f13,  352 (x5)\n"
			"flw.ps f3,  768 (x5)\n"
			"flw.ps f28,  480 (x5)\n"
			"flw.ps f19,  928 (x5)\n"
			"flw.ps f14,  64 (x5)\n"
			"flw.ps f7,  800 (x5)\n"
			"flw.ps f4,  288 (x5)\n"
			"flw.ps f21,  640 (x5)\n"
			"flw.ps f16,  896 (x5)\n"
			"flw.ps f31,  416 (x5)\n"
			"flw.ps f30,  992 (x5)\n"
			"flw.ps f10,  448 (x5)\n"
			"flw.ps f29,  96 (x5)\n"
			"flw.ps f6,  512 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f23,  320 (x5)\n"
			"flw.ps f9,  704 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f24,  160 (x5)\n"
			"flw.ps f22,  608 (x5)\n"
			"flw.ps f11,  128 (x5)\n"
			"flw.ps f26,  544 (x5)\n"
			"flw.ps f1,  672 (x5)\n"
			"flw.ps f0,  384 (x5)\n"
			"flw.ps f18,  832 (x5)\n"
			"flw.ps f5,  736 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f20,  0 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_3_HID_9:\n"
			"csrr x0, mtval\n"
			"fcvt.f16.ps f1, f2\n"
			"fmadd.ps f2, f28, f16, f12, rup\n"
			"for.pi f11, f12, f27\n"
			"bne x29, x7, 1f\n"
			"1:\n"
			"fmax.s f3, f12, f12\n"
			"fcvt.ps.pwu f27, f29, rdn\n"
			"fnot.pi f29, f29\n"
			"bne x29, x5, 1f\n"
			"1:\n"
			"csrr x0, matp\n"
			"fmulh.pi f3, f8, f14\n"
			"fcvt.ps.pwu f1, f19, rup\n"
			"fadd.s f12, f12, f19, rup\n"
			"fxor.pi f28, f8, f16\n"
			"fminu.pi f13, f14, f11\n"
			"fltu.pi f12, f14, f8\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f2, 0(x22)\n"
			"fcvt.w.s x22, f2, rne\n" 
			"fnot.pi f8, f12\n"
			"bne x3, x1, 1f\n"
			"1:\n"
			"addiw x0, x19, 0x12\n"
			"divw x20, x7, x19\n"
			"fswizz.ps f8, f16, 0x19\n"
			"fmsub.s f8, f16, f2, f16, rne\n"
			"fxor.pi f2, f12, f16\n"
			"bge x31, x26, 1f\n"
			"1:\n"
			"li x17, 0x8088205030\n"
			"la x28, mem_ptr_offset\n"
			"flw.ps f27, 928(x28)\n"
			"famoming.ps f14, f27 (x17)\n" 
			"fnot.pi f7, f27\n"
			"bge x31, x3, 1f\n"
			"1:\n"
			"fmvs.x.ps x24, f11, 6\n"
			"maskxor m2, m3, m5\n"
			"mova.m.x x20\n"
			"auipc x0, 0x1f\n"
			"fmulh.pi f7, f15, f8\n"
			"li x27, 0x8059b1b388\n"
			"li x24, 79283017\n"
			"fg32w.ps f12, x14 (x27)\n" 
			"fround.ps f15, f13, rne\n"
			"bne x13, x11, 1f\n"
			"1:\n"
			"sllw x22, x22, x0\n"
			"fsatu8.pi f19, f30\n"
			"bgeu x15, x21, 1f\n"
			"1:\n"
			"fmin.pi f15, f30, f13\n"
			"fmin.ps f19, f19, f14\n"
			"fmsub.ps f15, f19, f19, f1, rne\n"
			"li x28, 0x808b107010\n"
			"la x27, mem_ptr_offset\n"
			"flw.ps f7, 448(x27)\n"
			"fghg.ps f8, f7 (x28)\n" 
			"fmul.ps f2, f16, f11, rup\n"
			"fnmadd.s f8, f12, f8, f1, rup\n"
			"fmsub.s f11, f8, f1, f1, rdn\n"
			"fcvt.pwu.ps f19, f2, rtz\n"
			"fclass.ps f12, f2\n"
			"fadd.s f7, f7, f3, rtz\n"
			"li x22, 0x359ae17e83fbbff3\n"
			"li x20, 0x89ab07099db49807\n"
			"remu x20, x22, x20\n" 
			"fmsub.ps f7, f7, f7, f29, rmm\n"
			"fsgnjx.s f27, f7, f16\n"
			"fnot.pi f16, f7\n"
			"bltu x8, x31, 1f\n"
			"1:\n"
			"fcvt.ps.pw f7, f7, rmm\n"
			"fmin.s f27, f2, f2\n"
			"fadd.pi f1, f16, f16\n"
			"li x28, 0x805a4086f8\n"
			"la x27, mem_ptr_offset\n"
			"flw.ps f1, 256(x27)\n"
			"fsch.ps f11, f1 (x28)\n" 
			"fmul.s f2, f16, f30, rup\n"
			"bltu x4, x25, 1f\n"
			"1:\n"
			"fmsub.s f30, f3, f30, f27, rup\n"
			"fmvz.x.ps x17, f1, 5\n"
			"fbci.ps f1, 0x50474\n"
			"sraiw x19, x19, 5\n"
			"fexp.ps f16, f27\n"
			"li x21, 0x805e6abfa5\n"
			"sd x27, 256(x21)\n"
			"lhu x24, 256(x21)\n" 
			"fsatu8.pi f16, f13\n"
			"fmulhu.pi f29, f29, f30\n"
			"faddi.pi f15, f19, 0x138\n"
			"fslli.pi f8, f15, 0x5\n"
			"blt x27, x28, 1f\n"
			"1:\n"
			"fslli.pi f16, f29, 0xe\n"
			"fcvt.ps.pwu f2, f15, rmm\n"
			"la x24, rand_ieee754_32\n"
			"flw.ps f1, 384(x24)\n"
			"flw.ps f3, 224(x24)\n"
			"fcmovm.ps f12, f1, f3\n" 
			"fsrli.pi f12, f1, 0xe\n"
			"fxor.pi f3, f1, f28\n"
			"bgeu x2, x13, 1f\n"
			"1:\n"
			"fle.ps f1, f14, f2\n"
			"slt x14, x22, x17\n"
			"sllw x24, x23, x22\n"
			"sra x20, x20, x20\n"
			"li x14, 0x809c6a8018\n"
			"amoswapl.d x21, x19, (x14)\n" 
			"fminu.pi f12, f28, f12\n"
			"ffrc.ps f2, f29\n"
			"fmin.ps f30, f29, f3\n"
			"fsgnj.ps f11, f11, f29\n"
			"fsgnjx.s f14, f14, f3\n"
			"fandi.pi f2, f2, 0x1bd\n"
			"la x20, rand_int_32\n"
			"flw.ps f2, 96(x20)\n"
			"fsrli.pi f27, f2, 0x3\n" 
			"fround.ps f3, f29, rdn\n"
			"fadd.s f30, f19, f30, rtz\n"
			"feq.pi f14, f3, f30\n"
			"fle.pi f30, f29, f19\n"
			"flt.ps f27, f2, f19\n"
			"beq x29, x10, 1f\n"
			"1:\n"
			"fmul.pi f30, f29, f29\n"
			"li x31, 0x8030004a00\n"
			"li x7, 0x8400000000000004\n"
			"add x7, x31, x7\n"
			"li x31, (0x3 << 6) | 0x1\n"
			"csrwi tensor_wait, 6\n" 
			"csrwi tensor_wait, 5\n" 
			"fpackrepb.pi f27, f2\n"
			"fclass.s x7, f19\n"
			"andi x20, x20, 0x1c\n"
			"divw x14, x19, x23\n"
			"andi x20, x0, 0x0\n"
			"mova.m.x x7\n"
			"li x28, 0x808a55203c\n"
			"amoxorg.w x14, x27, (x28)\n" 
			"andi x21, x24, 0x3\n"
			"ecall\n"
			"fcvt.ps.pwu f30, f14, rmm\n"
			"bltu x15, x25, 1f\n"
			"1:\n"
			"addiw x23, x24, 0x10\n"
			"fcvt.ps.pw f15, f13, rdn\n"
			"mov.m.x m3, x20, 0x18\n"
			"li x28, 0x664926cc71d40002\n"
			"li x19, 0xb3989d87e5334ff8\n"
			"rem x7, x28, x19\n" 
			"fmv.s.x f14, x0\n"
			"mova.m.x x23\n"
			"sraiw x27, x27, 1\n"
			"fcvt.pw.ps f16, f8, rtz\n"
			"add x23, x23, x27\n"
			"fsgnjn.ps f28, f1, f1\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f3, 256(x20)\n"
			"fcvt.lu.s x19, f3, rtz\n" 
			"fmul.s f12, f7, f16, rdn\n"
			"maskxor m5, m2, m5\n"
			"remu x20, x0, x20\n"
			"fsra.pi f1, f3, f16\n"
			"flt.ps f8, f29, f14\n"
			"fbci.ps f14, 0x1a3d7\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x40\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f14, 0(x31)\n"
			"flw.ps f15, 4(x31)\n"
			"flw.ps f16, 1(x31)\n"
			"flw.ps f2, 2(x31)\n"
			"flw.ps f27, 8(x31)\n"
			"flw.ps f7, 16(x31)\n"
			"flw.ps f8, 19(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x21, 2(x31)\n"
			"lw x23, 5(x31)\n"
			"li x31, 0x2\n"
			"LBL_SEQID_3_LOOP_SEQ_HID_9:\n"
			"fmv.s.x f13, x21\n"
			"flt.pi f30, f16, f15\n"
			"blt x19, x6, 1f\n"
			"1:\n"
			"mulhsu x21, x21, x23\n"
			"xori x14, x21, 0x1f\n"
			"fclass.s x14, f8\n"
			"fsub.ps f2, f2, f7, rdn\n"
			"bltu x7, x26, 1f\n"
			"1:\n"
			"flog.ps f30, f8\n"
			"fround.ps f8, f8, rdn\n"
			"fcvt.s.wu f14, x0, rmm\n"
			"beq x26, x31, 1f\n"
			"1:\n"
			"maskpopc x0, m2\n"
			"fcvt.f16.ps f2, f2\n"
			"bltu x27, x2, 1f\n"
			"1:\n"
			"addw x28, x0, x21\n"
			"fsra.pi f30, f14, f8\n"
			"bge x16, x4, 1f\n"
			"1:\n"
			"fsrli.pi f2, f16, 0xa\n"
			"mov.m.x m3, x21, 0x6f\n"
			"fmin.ps f8, f8, f7\n"
			"fnmsub.s f16, f16, f14, f15, rmm\n"
			"div x27, x0, x21\n"
			"addi x14, x23, 0x15\n"
			"fsrai.pi f13, f7, 0x8\n"
			"feq.pi f7, f15, f7\n"
			"lui x21, 0xe\n"
			"bne x7, x11, 1f\n"
			"1:\n"
			"fltm.ps m5, f7, f15\n"
			"fmsub.s f19, f14, f2, f27, rtz\n"
			"fclass.s x0, f8\n"
			"feq.pi f29, f8, f27\n"
			"bge x24, x12, 1f\n"
			"1:\n"
			"flt.ps f27, f14, f8\n"
			"mov.m.x m3, x23, 0x4d\n"
			"and x0, x0, x23\n"
			"flt.pi f19, f14, f15\n"
			"maskxor m4, m4, m2\n"
			"fcmovm.ps f19, f27, f27\n"
			"bltu x8, x28, 1f\n"
			"1:\n"
			"fsgnj.s f19, f2, f16\n"
			"fsll.pi f13, f27, f7\n"
			"fsrai.pi f14, f16, 0x4\n"
			"fcvt.wu.s x24, f14, rup\n"
			"bne x9, x12, 1f\n"
			"1:\n"
			"feq.pi f13, f27, f2\n"
			"blt x29, x4, 1f\n"
			"1:\n"
			"fcmovm.ps f14, f14, f2\n"
			"bne x13, x21, 1f\n"
			"1:\n"
			"ffrc.ps f29, f14\n"
			"divuw x21, x21, x23\n"
			"andi x23, x23, 0xd\n"
			"fcvt.ps.pw f19, f14, rdn\n"
			"fmaxu.pi f19, f15, f14\n"
			"fsat8.pi f29, f7\n"
			"bgeu x25, x10, 1f\n"
			"1:\n"
			"fnmadd.ps f27, f16, f14, f16, rup\n"
			"fmax.s f30, f8, f15\n"
			"fxor.pi f13, f2, f2\n"
			"fsgnj.ps f2, f8, f7\n"
			"fadd.pi f14, f15, f14\n"
			"fcvt.ps.f16 f16, f27\n"
			"fnmadd.ps f8, f8, f16, f2, rtz\n"
			"slt x28, x23, x21\n"
			"maskxor m2, m5, m3\n"
			"blt x12, x8, 1f\n"
			"1:\n"
			"auipc x21, 0x2\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_3_LOOP_SEQ_HID_9\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_4_HID_9:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f28, 64(x20)\n"
			"flw.ps f7, 704(x20)\n"
			"fnmadd.s f28, f28, f7, f28, rup\n" 
			"divuw x14, x17, x23\n"
			"flt.ps f30, f1, f3\n"
			"maskpopcz x14, m4\n"
			"fmsub.ps f30, f12, f8, f7, rtz\n"
			"bge x11, x10, 1f\n"
			"1:\n"
			"la x24, rand_ieee754_32\n"
			"flw.ps f8, 736(x24)\n"
			"fround.ps f27, f8, rup\n" 
			"fltu.pi f2, f19, f27\n"
			"blt x2, x2, 1f\n"
			"1:\n"
			"fsll.pi f11, f27, f28\n"
			"andi x7, x21, 0x16\n"
			"bgeu x17, x27, 1f\n"
			"1:\n"
			"fence\n"
			"bge x10, x22, 1f\n"
			"1:\n"
			"li x19, 0xd62447399d8bc71a\n"
			"li x7, 0x378bf89b04b464f6\n"
			"mul x22, x19, x7\n" 
			"fltu.pi f27, f1, f13\n"
			"fexp.ps f28, f7\n"
			"fmax.pi f30, f1, f16\n"
			"fmsub.s f2, f13, f13, f7, rup\n"
			"li x27, 0x95ad05694dde4802\n"
			"addi x28, x27, 0x1b\n" 
			"fmax.pi f7, f15, f7\n"
			"fsetm.pi m3, f1\n"
			"bne x10, x21, 1f\n"
			"1:\n"
			"fmaxu.pi f15, f8, f8\n"
			"sll x23, x23, x28\n"
			"bltu x20, x2, 1f\n"
			"1:\n"
			"li x7, 0x656909b8eff34752\n"
			"li x21, 0xfe15ea5468414722\n"
			"remu x17, x7, x21\n" 
			"fnot.pi f8, f11\n"
			"fcmovm.ps f30, f19, f3\n"
			"fcvt.pw.ps f2, f13, rmm\n"
			"fmul.s f13, f29, f30, rmm\n"
			"li x24, 0x805ce37990\n"
			"fsw.ps f1, 256(x24)\n"
			"flw f12, 256(x24)\n" 
			"fandi.pi f12, f12, 0x18\n"
			"fnmsub.s f2, f2, f7, f19, rne\n"
			"fexp.ps f30, f19\n"
			"faddi.pi f2, f12, 0x144\n"
			"li x14, 0x809ebef018\n"
			"amomaxl.d x27, x27, (x14)\n" 
			"fmin.pi f27, f27, f3\n"
			"bne x10, x7, 1f\n"
			"1:\n"
			"feq.pi f30, f29, f30\n"
			"fsat8.pi f13, f12\n"
			"fcvt.pwu.ps f27, f28, rtz\n"
			"li x17, 0x808f345008\n"
			"amomaxug.d x28, x23, (x17)\n" 
			"faddi.pi f28, f7, 0x15a\n"
			"fpackrepb.pi f12, f30\n"
			"fmul.s f1, f15, f8, rtz\n"
			"fsrai.pi f13, f12, 0xb\n"
			"li x21, 0x805bdf4528\n"
			"sb x17, -512(x21)\n" 
			"fxor.pi f14, f13, f15\n"
			"fcvt.ps.pwu f2, f13, rne\n"
			"bge x14, x1, 1f\n"
			"1:\n"
			"fnmsub.s f7, f7, f7, f29, rtz\n"
			"fle.pi f30, f13, f12\n"
			"beq x10, x15, 1f\n"
			"1:\n"
			"li x14, 0x809e0ed018\n"
			"la x22, mem_ptr_offset\n"
			"flw.ps f2, 480(x22)\n"
			"famoxorl.pi f1, f2 (x14)\n" 
			"fpackrepb.pi f14, f13\n"
			"fsgnjx.ps f16, f16, f8\n"
			"fsgnjn.ps f19, f3, f13\n"
			"fcvt.pw.ps f7, f11, rup\n"
			"li x24, 0x808a453020\n"
			"amoandg.w x27, x23, (x24)\n" 
			"fsgnjn.ps f2, f2, f1\n"
			"faddi.pi f27, f12, 0x13f\n"
			"bne x8, x12, 1f\n"
			"1:\n"
			"fltu.pi f2, f27, f2\n"
			"fpackrepb.pi f3, f28\n"
			"la x24, rand_int_32\n"
			"flw.ps f11, 544(x24)\n"
			"fnot.pi f27, f11\n" 
			"fcvt.ps.f16 f27, f27\n"
			"subw x21, x23, x21\n"
			"fand.pi f8, f27, f27\n"
			"flt.ps f27, f8, f8\n"
			"bltu x1, x10, 1f\n"
			"1:\n"
			"li x24, 0x8098289030\n"
			"la x28, mem_ptr_offset\n"
			"flw.ps f15, 320(x28)\n"
			"famomaxl.pi f12, f15 (x24)\n" 
			"sra x20, x23, x24\n"
			"fcvt.ps.pwu f16, f7, rmm\n"
			"mulhsu x19, x23, x17\n"
			"fmul.s f12, f8, f7, rtz\n"
			"li x23, 0x805ada59f8\n"
			"sh x22, -256(x23)\n" 
			"flt.pi f19, f8, f1\n"
			"fmax.pi f1, f1, f12\n"
			"fmvz.x.ps x20, f30, 4\n"
			"fnmadd.s f30, f30, f1, f29, rne\n"
			"li x7, 0x21c6f2e5a0fc5db5\n"
			"li x28, 0x4725c77f61572e81\n"
			"sll x19, x7, x28\n" 
			"fcvt.ps.f16 f28, f11\n"
			"addiw x28, x21, 0x1c\n"
			"bge x11, x4, 1f\n"
			"1:\n"
			"mulhsu x7, x7, x28\n"
			"fbci.ps f15, 0x53a8d\n"
			"beq x10, x0, 1f\n"
			"1:\n"
			"li x24, 0x808829c008\n"
			"amoorg.w x22, x20, (x24)\n" 
			"fsgnj.s f13, f29, f15\n"
			"fnot.pi f2, f14\n"
			"fmin.s f3, f12, f14\n"
			"fsgnjn.s f16, f16, f1\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x28\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f11, 3(x31)\n"
			"flw.ps f12, 3(x31)\n"
			"flw.ps f13, 5(x31)\n"
			"flw.ps f14, 12(x31)\n"
			"flw.ps f15, 6(x31)\n"
			"flw.ps f16, 15(x31)\n"
			"flw.ps f19, 1(x31)\n"
			"flw.ps f2, 19(x31)\n"
			"flw.ps f27, 11(x31)\n"
			"flw.ps f28, 17(x31)\n"
			"flw.ps f29, 18(x31)\n"
			"flw.ps f3, 26(x31)\n"
			"flw.ps f30, 11(x31)\n"
			"flw.ps f7, 42(x31)\n"
			"flw.ps f8, 57(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x14, 2(x31)\n"
			"lw x17, 2(x31)\n"
			"lw x19, 5(x31)\n"
			"lw x20, 9(x31)\n"
			"lw x21, 14(x31)\n"
			"lw x22, 9(x31)\n"
			"lw x23, 7(x31)\n"
			"lw x24, 13(x31)\n"
			"lw x27, 31(x31)\n"
			"lw x28, 37(x31)\n"
			"lw x7, 14(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_4_LOOP_SEQ_HID_9:\n"
			"fle.pi f7, f7, f19\n"
			"divu x23, x20, x27\n"
			"bgeu x1, x31, 1f\n"
			"1:\n"
			"fsrai.pi f30, f14, 0x0\n"
			"fmin.s f3, f11, f30\n"
			"addiw x23, x23, 0x5\n"
			"sraw x28, x17, x17\n"
			"divw x22, x27, x14\n"
			"bne x31, x22, 1f\n"
			"1:\n"
			"slti x23, x24, 0x1f\n"
			"bltu x26, x24, 1f\n"
			"1:\n"
			"maskor m5, m5, m5\n"
			"sltu x7, x23, x19\n"
			"maskand m2, m5, m2\n"
			"fmulh.pi f8, f7, f14\n"
			"fbci.ps f3, 0x484cd\n"
			"feq.pi f1, f2, f1\n"
			"beq x18, x20, 1f\n"
			"1:\n"
			"addw x23, x23, x24\n"
			"fcvt.w.s x24, f29, rdn\n"
			"fcvt.s.wu f27, x17, rtz\n"
			"bltu x10, x22, 1f\n"
			"1:\n"
			"flem.ps m5, f8, f19\n"
			"bgeu x23, x29, 1f\n"
			"1:\n"
			"xori x19, x28, 0x18\n"
			"auipc x23, 0x1f\n"
			"fcvt.s.wu f8, x28, rmm\n"
			"bne x30, x20, 1f\n"
			"1:\n"
			"fexp.ps f30, f30\n"
			"blt x12, x23, 1f\n"
			"1:\n"
			"fnmsub.s f3, f13, f3, f30, rne\n"
			"fmin.pi f8, f8, f7\n"
			"fsrli.pi f15, f14, 0xd\n"
			"fcvt.s.w f28, x14, rne\n"
			"add x21, x7, x0\n"
			"fpackreph.pi f16, f8\n"
			"fbci.pi f19, 0x7071c\n"
			"beq x11, x11, 1f\n"
			"1:\n"
			"fmulh.pi f28, f28, f15\n"
			"fround.ps f3, f15, rtz\n"
			"bgeu x1, x8, 1f\n"
			"1:\n"
			"ori x17, x24, 0x1c\n"
			"fsra.pi f29, f13, f1\n"
			"fmv.s.x f29, x17\n"
			"slliw x19, x19, 5\n"
			"bgeu x15, x0, 1f\n"
			"1:\n"
			"fadd.pi f28, f1, f28\n"
			"bne x13, x21, 1f\n"
			"1:\n"
			"fsgnj.ps f8, f8, f16\n"
			"bne x7, x5, 1f\n"
			"1:\n"
			"fsll.pi f11, f28, f27\n"
			"fbci.ps f12, 0x66384\n"
			"fsgnjx.ps f30, f27, f8\n"
			"maskand m4, m4, m2\n"
			"srli x28, x19, 2\n"
			"beq x19, x13, 1f\n"
			"1:\n"
			"or x0, x22, x0\n"
			"fnmadd.ps f19, f13, f7, f28, rtz\n"
			"bne x17, x22, 1f\n"
			"1:\n"
			"fnmsub.s f29, f30, f12, f29, rdn\n"
			"bgeu x8, x30, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f16, f3\n"
			"fltm.pi m5, f29, f1\n"
			"mulh x20, x23, x14\n"
			"flt.ps f27, f28, f27\n"
			"fltu.pi f29, f13, f30\n"
			"flt.pi f11, f16, f14\n"
			"sra x20, x20, x14\n"
			"fadd.pi f3, f27, f3\n"
			"fsub.pi f16, f15, f16\n"
			"add x17, x0, x22\n"
			"slti x28, x7, 0xf\n"
			"fslli.pi f19, f14, 0x4\n"
			"remuw x22, x7, x27\n"
			"bgeu x10, x7, 1f\n"
			"1:\n"
			"sra x21, x21, x17\n"
			"fsra.pi f27, f14, f27\n"
			"mova.m.x x28\n"
			"fnmadd.s f1, f3, f11, f13, rtz\n"
			"srlw x0, x23, x27\n"
			"srli x21, x21, 1\n"
			"fsatu8.pi f14, f13\n"
			"ori x17, x17, 0x6\n"
			"fcvt.s.wu f8, x21, rtz\n"
			"beq x9, x1, 1f\n"
			"1:\n"
			"fsra.pi f29, f13, f27\n"
			"ori x28, x28, 0x14\n"
			"feq.s x17, f29, f14\n"
			"fandi.pi f3, f15, 0x1b7\n"
			"fle.ps f16, f27, f28\n"
			"bge x26, x17, 1f\n"
			"1:\n"
			"addw x7, x23, x21\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_4_LOOP_SEQ_HID_9\n"
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f2,  288 (x5)\n"
			"flw.ps f28,  928 (x5)\n"
			"flw.ps f9,  256 (x5)\n"
			"flw.ps f21,  544 (x5)\n"
			"flw.ps f20,  768 (x5)\n"
			"flw.ps f12,  864 (x5)\n"
			"flw.ps f4,  480 (x5)\n"
			"flw.ps f11,  160 (x5)\n"
			"flw.ps f22,  416 (x5)\n"
			"flw.ps f10,  512 (x5)\n"
			"flw.ps f1,  736 (x5)\n"
			"flw.ps f18,  896 (x5)\n"
			"flw.ps f7,  576 (x5)\n"
			"flw.ps f31,  352 (x5)\n"
			"flw.ps f3,  800 (x5)\n"
			"flw.ps f6,  320 (x5)\n"
			"flw.ps f8,  608 (x5)\n"
			"flw.ps f26,  832 (x5)\n"
			"flw.ps f23,  960 (x5)\n"
			"flw.ps f16,  128 (x5)\n"
			"flw.ps f0,  64 (x5)\n"
			"flw.ps f27,  224 (x5)\n"
			"flw.ps f19,  384 (x5)\n"
			"flw.ps f13,  640 (x5)\n"
			"flw.ps f5,  32 (x5)\n"
			"flw.ps f15,  96 (x5)\n"
			"flw.ps f25,  992 (x5)\n"
			"flw.ps f24,  672 (x5)\n"
			"flw.ps f17,  448 (x5)\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f14,  0 (x5)\n"
			"flw.ps f29,  192 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_5_HID_9:\n"
			"li x27, 0x808aade028\n"
			"la x20, mem_ptr_offset\n"
			"flw.ps f28, 96(x20)\n"
			"famomaxg.pi f28, f28 (x27)\n" 
			"for.pi f8, f1, f11\n"
			"fclass.ps f7, f11\n"
			"or x22, x0, x22\n"
			"add x7, x19, x0\n"
			"fmsub.ps f28, f15, f30, f11, rtz\n"
			"fcvt.wu.s x7, f8, rtz\n"
			"bne x23, x25, 1f\n"
			"1:\n"
			"fmadd.ps f11, f8, f8, f3, rdn\n"
			"srai x20, x22, 1\n"
			"fcvt.w.s x23, f1, rne\n"
			"fsetm.pi m4, f12\n"
			"li x24, 0x809ae8f030\n"
			"amoandl.d x14, x20, (x24)\n" 
			"fsll.pi f14, f15, f14\n"
			"srliw x27, x0, 1\n"
			"fsll.pi f12, f13, f14\n"
			"fmul.pi f16, f14, f7\n"
			"fmax.ps f11, f16, f29\n"
			"sra x7, x19, x17\n"
			"bne x8, x17, 1f\n"
			"1:\n"
			"mulw x22, x22, x7\n"
			"frcp.ps f19, f19\n"
			"fcvt.wu.s x22, f16, rne\n"
			"ffrc.ps f30, f30\n"
			"blt x30, x3, 1f\n"
			"1:\n"
			"li x24, 0x3937b2665d7f6716\n"
			"li x19, 0x290455d48b2fffaf\n"
			"mulw x22, x24, x19\n" 
			"fadd.pi f8, f1, f8\n"
			"flt.pi f7, f3, f19\n"
			"fsra.pi f7, f15, f14\n"
			"fsgnjn.s f27, f19, f27\n"
			"xori x0, x0, 0x1e\n"
			"bge x23, x5, 1f\n"
			"1:\n"
			"slt x27, x19, x22\n"
			"bne x29, x25, 1f\n"
			"1:\n"
			"fadd.ps f14, f14, f30, rdn\n"
			"bge x30, x24, 1f\n"
			"1:\n"
			"remuw x22, x19, x23\n"
			"srliw x20, x22, 3\n"
			"bne x10, x7, 1f\n"
			"1:\n"
			"fmulhu.pi f27, f27, f27\n"
			"la x27, rand_int_32\n"
			"flw.ps f16, 256(x27)\n"
			"fcvt.ps.pwu f15, f16, rup\n" 
			"fadd.pi f28, f30, f3\n"
			"bltu x6, x8, 1f\n"
			"1:\n"
			"fmin.pi f15, f8, f15\n"
			"fsgnjn.s f1, f7, f8\n"
			"fminu.pi f15, f3, f8\n"
			"beq x31, x15, 1f\n"
			"1:\n"
			"frcp.ps f29, f15\n"
			"feq.ps f16, f16, f7\n"
			"bltu x5, x15, 1f\n"
			"1:\n"
			"ffrc.ps f15, f8\n"
			"fsgnjn.s f12, f7, f14\n"
			"fltu.pi f11, f27, f14\n"
			"beq x25, x3, 1f\n"
			"1:\n"
			"fminu.pi f3, f7, f30\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f8, 544(x20)\n"
			"fswizz.ps f13, f8, 0x5\n" 
			"flog.ps f8, f11\n"
			"fmin.s f7, f13, f19\n"
			"fnmadd.s f15, f28, f28, f19, rmm\n"
			"beq x17, x31, 1f\n"
			"1:\n"
			"fmsub.s f27, f29, f16, f19, rdn\n"
			"fcvt.pw.ps f8, f29, rtz\n"
			"frcp.ps f8, f7\n"
			"fcvt.pwu.ps f7, f8, rne\n"
			"fcvt.f16.ps f1, f28\n"
			"blt x0, x8, 1f\n"
			"1:\n"
			"fmin.ps f15, f27, f11\n"
			"fcmov.ps f1, f28, f28, f8\n"
			"li x7, 0x808a968038\n"
			"la x19, mem_ptr_offset\n"
			"flw.ps f2, 288(x19)\n"
			"famoandg.pi f8, f2 (x7)\n" 
			"fcvt.ps.pw f11, f14, rtz\n"
			"fadd.pi f1, f19, f1\n"
			"fadd.pi f27, f27, f7\n"
			"feq.ps f14, f16, f29\n"
			"bltu x31, x1, 1f\n"
			"1:\n"
			"fswizz.ps f16, f27, 0xc\n"
			"bne x24, x22, 1f\n"
			"1:\n"
			"fclass.ps f7, f29\n"
			"flog.ps f2, f7\n"
			"fcvt.ps.pw f2, f14, rup\n"
			"flt.ps f27, f28, f16\n"
			"bltu x29, x14, 1f\n"
			"1:\n"
			"faddi.pi f30, f30, 0x141\n"
			"bltu x7, x25, 1f\n"
			"1:\n"
			"li x23, 0x808fce5008\n"
			"amoswapg.w x24, x24, (x23)\n" 
			"fnmadd.s f19, f19, f15, f12, rdn\n"
			"fle.ps f11, f11, f8\n"
			"beq x6, x6, 1f\n"
			"1:\n"
			"flog.ps f30, f15\n"
			"fsub.ps f3, f3, f12, rne\n"
			"fmin.pi f7, f13, f27\n"
			"fandi.pi f16, f12, 0xc5\n"
			"fsub.pi f7, f1, f16\n"
			"fsatu8.pi f27, f30\n"
			"fxor.pi f1, f8, f16\n"
			"fslli.pi f8, f19, 0xa\n"
			"csrr x0, mhpmevent3\n"
			"fmax.ps f27, f28, f30\n"
			"beq x6, x6, 1f\n"
			"1:\n"
			"fandi.pi f7, f27, 0x26\n"
			"fsub.s f11, f2, f11, rtz\n"
			"fmadd.s f11, f11, f30, f19, rtz\n"
			"fmulh.pi f12, f2, f28\n"
			"fsrl.pi f12, f7, f19\n"
			"fsatu8.pi f30, f2\n"
			"fnmsub.ps f19, f27, f19, f2, rtz\n"
			"bltu x18, x25, 1f\n"
			"1:\n"
			"flt.ps f30, f11, f28\n"
			"fnmsub.ps f12, f30, f11, f30, rtz\n"
			"li x28, 0x805afe1ad0\n"
			"la x19, mem_ptr_offset\n"
			"flw.ps f12, 320(x19)\n"
			"fgw.ps f8, f12 (x28)\n" 
			"srli x28, x24, 0\n"
			"fsrli.pi f28, f16, 0xe\n"
			"fmul.ps f8, f16, f8, rdn\n"
			"fmadd.s f27, f27, f27, f8, rne\n"
			"mulh x21, x20, x28\n"
			"fsrl.pi f3, f3, f8\n"
			"fsgnjx.s f28, f3, f28\n"
			"slli x17, x7, 2\n"
			"addi x0, x0, 0xa\n"
			"fadd.pi f13, f13, f16\n"
			"li x31, 0x8058006340\n"
			"li x23, 0x8800000000000006\n"
			"add x23, x31, x23\n"
			"li x31, (0x1 << 6) | 0x0\n"
			"csrwi tensor_wait, 6\n" 
			"csrwi tensor_wait, 4\n" 
			"ffrc.ps f13, f30\n"
			"fsgnjx.s f14, f30, f30\n"
			"mulhsu x27, x20, x22\n"
			"blt x24, x27, 1f\n"
			"1:\n"
			"fmulhu.pi f3, f3, f3\n"
			"fnmadd.ps f19, f30, f13, f13, rup\n"
			"bge x2, x21, 1f\n"
			"1:\n"
			"fadd.ps f19, f30, f28, rtz\n"
			"fsetm.pi m2, f30\n"
			"fsub.ps f13, f13, f19, rdn\n"
			"bltu x22, x12, 1f\n"
			"1:\n"
			"fmulhu.pi f13, f3, f13\n"
			"packb x27, x24, x22\n"
			"li x23, 0x808d055028\n"
			"la x21, mem_ptr_offset\n"
			"flw.ps f28, 224(x21)\n"
			"famomaxug.pi f16, f28 (x23)\n" 
			"fcvt.ps.pw f19, f3, rdn\n"
			"fmax.ps f16, f16, f19\n"
			"fsll.pi f27, f19, f12\n"
			"fnmsub.ps f15, f11, f1, f16, rmm\n"
			"fmin.ps f27, f27, f12\n"
			"fsgnjx.ps f19, f15, f11\n"
			"fmax.pi f13, f15, f1\n"
			"fmaxu.pi f7, f3, f12\n"
			"fswizz.ps f16, f27, 0x1d\n"
			"fnmsub.s f15, f19, f15, f15, rmm\n"
			"li x23, 0x809deb1018\n"
			"amominl.w x24, x27, (x23)\n" 
			"fsgnjx.s f8, f14, f8\n"
			"fsra.pi f28, f3, f28\n"
			"fsat8.pi f14, f16\n"
			"fcvt.pw.ps f28, f3, rdn\n"
			"fmulh.pi f11, f12, f19\n"
			"fsub.pi f8, f16, f8\n"
			"fpackreph.pi f15, f13\n"
			"fcvt.f16.ps f15, f13\n"
			"fle.pi f19, f27, f3\n"
			"fmul.pi f19, f11, f7\n"
			"li x23, 0x808b807038\n"
			"la x27, mem_ptr_offset\n"
			"flw.ps f19, 256(x27)\n"
			"famoswapg.pi f14, f19 (x23)\n" 
			"fmaxu.pi f2, f1, f2\n"
			"fmsub.s f16, f29, f13, f12, rup\n"
			"fnmadd.s f7, f27, f19, f27, rne\n"
			"feqm.ps m4, f16, f14\n"
			"and x22, x27, x22\n"
			"rem x7, x24, x20\n"
			"srlw x22, x14, x19\n"
			"sltu x17, x7, x17\n"
			"beq x13, x22, 1f\n"
			"1:\n"
			"for.pi f19, f19, f3\n"
			"fsat8.pi f29, f16\n"
			"li x17, 0x805db6181b\n"
			"sd x22, -512(x17)\n"
			"lhu x19, -512(x17)\n" 
			"flt.s x27, f1, f3\n"
			"div x27, x21, x27\n"
			"beq x19, x24, 1f\n"
			"1:\n"
			"fnmsub.s f7, f19, f2, f3, rmm\n"
			"fcvt.f16.ps f19, f2\n"
			"rem x17, x21, x21\n"
			"fsrli.pi f12, f12, 0x7\n"
			"bltu x23, x30, 1f\n"
			"1:\n"
			"fcvt.f16.ps f11, f16\n"
			"fcvt.w.s x17, f16, rdn\n"
			"sltiu x27, x20, 0xc\n"
			"maskpopcz x20, m2\n"
			"beq x20, x8, 1f\n"
			"1:\n"
			"csrr x0, minstret\n"
			"fcmov.ps f8, f8, f11, f8\n"
			"fmsub.ps f28, f11, f29, f8, rdn\n"
			"fnmadd.ps f7, f7, f7, f3, rne\n"
			"ffrc.ps f3, f11\n"
			"fmadd.ps f7, f7, f2, f29, rtz\n"
			"fcmovm.ps f8, f1, f29\n"
			"fand.pi f28, f8, f11\n"
			"blt x5, x25, 1f\n"
			"1:\n"
			"fmin.ps f27, f19, f19\n"
			"fcmov.ps f3, f7, f2, f8\n"
			"fsrai.pi f14, f8, 0x7\n"
			"bge x21, x0, 1f\n"
			"1:\n"
			"csrr x0, stvec\n"
			"fminu.pi f14, f16, f7\n"
			"fcvt.pwu.ps f14, f29, rtz\n"
			"fexp.ps f12, f14\n"
			"fmvs.x.ps x28, f1, 0\n"
			"frcp.ps f2, f16\n"
			"bltu x4, x12, 1f\n"
			"1:\n"
			"fcmov.ps f16, f27, f14, f7\n"
			"fsub.pi f13, f29, f3\n"
			"bgeu x10, x7, 1f\n"
			"1:\n"
			"xori x14, x14, 0x2\n"
			"srai x7, x24, 3\n"
			"bltu x4, x9, 1f\n"
			"1:\n"
			"divu x0, x17, x0\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x30\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f14, 0(x31)\n"
			"flw.ps f15, 0(x31)\n"
			"flw.ps f28, 10(x31)\n"
			"flw.ps f3, 15(x31)\n"
			"flw.ps f30, 16(x31)\n"
			"flw.ps f7, 14(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_5_LOOP_SEQ_HID_9:\n"
			"fsub.s f8, f3, f15, rup\n"
			"fmin.s f8, f28, f28\n"
			"fmin.pi f1, f14, f14\n"
			"fle.pi f1, f15, f15\n"
			"fsub.pi f28, f1, f14\n"
			"for.pi f3, f1, f7\n"
			"fnot.pi f3, f1\n"
			"fsgnjn.s f8, f14, f30\n"
			"fsgnj.s f28, f28, f1\n"
			"fnot.pi f28, f30\n"
			"fltu.pi f8, f3, f7\n"
			"bgeu x30, x24, 1f\n"
			"1:\n"
			"fminu.pi f16, f14, f15\n"
			"fnmadd.s f8, f28, f1, f3, rup\n"
			"fpackreph.pi f15, f1\n"
			"fmin.s f30, f28, f28\n"
			"fle.ps f28, f28, f15\n"
			"flt.ps f14, f30, f15\n"
			"feq.pi f3, f1, f3\n"
			"bne x19, x25, 1f\n"
			"1:\n"
			"fsgnjn.s f3, f3, f7\n"
			"bgeu x8, x13, 1f\n"
			"1:\n"
			"flt.pi f3, f7, f3\n"
			"bltu x24, x30, 1f\n"
			"1:\n"
			"fminu.pi f16, f7, f28\n"
			"ffrc.ps f16, f1\n"
			"fmin.pi f3, f3, f14\n"
			"fmin.ps f30, f15, f28\n"
			"beq x4, x26, 1f\n"
			"1:\n"
			"fround.ps f30, f30, rtz\n"
			"flt.ps f1, f30, f30\n"
			"fnot.pi f30, f28\n"
			"bltu x18, x7, 1f\n"
			"1:\n"
			"fadd.s f16, f28, f14, rne\n"
			"fsub.pi f14, f30, f14\n"
			"fmax.s f30, f1, f30\n"
			"fmadd.s f3, f14, f3, f3, rne\n"
			"fmax.ps f30, f15, f28\n"
			"bge x23, x19, 1f\n"
			"1:\n"
			"fclass.ps f3, f28\n"
			"bge x14, x3, 1f\n"
			"1:\n"
			"fexp.ps f3, f1\n"
			"fsgnj.ps f1, f1, f3\n"
			"fxor.pi f16, f28, f28\n"
			"blt x4, x9, 1f\n"
			"1:\n"
			"fsgnj.s f1, f30, f28\n"
			"fminu.pi f14, f30, f14\n"
			"fmul.pi f14, f3, f3\n"
			"bge x9, x20, 1f\n"
			"1:\n"
			"fmin.s f3, f14, f14\n"
			"fand.pi f28, f7, f15\n"
			"fmulhu.pi f30, f3, f14\n"
			"beq x24, x6, 1f\n"
			"1:\n"
			"fclass.ps f3, f1\n"
			"fsub.s f3, f15, f3, rmm\n"
			"blt x30, x13, 1f\n"
			"1:\n"
			"fmin.ps f28, f30, f28\n"
			"fmul.pi f8, f1, f7\n"
			"fle.pi f30, f30, f30\n"
			"fnmsub.ps f28, f14, f14, f14, rtz\n"
			"bltu x24, x29, 1f\n"
			"1:\n"
			"fsgnj.s f30, f1, f7\n"
			"faddi.pi f1, f28, 0x5b\n"
			"bne x7, x26, 1f\n"
			"1:\n"
			"fmulh.pi f30, f28, f15\n"
			"fclass.ps f1, f30\n"
			"bge x29, x2, 1f\n"
			"1:\n"
			"fxor.pi f30, f28, f7\n"
			"fsub.pi f1, f1, f14\n"
			"bltu x12, x3, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f8, f14, rmm\n"
			"fmsub.ps f15, f15, f15, f28, rne\n"
			"bne x29, x18, 1f\n"
			"1:\n"
			"fminu.pi f28, f14, f14\n"
			"fnmsub.ps f30, f3, f15, f30, rdn\n"
			"fcvt.ps.f16 f3, f3\n"
			"fmul.ps f28, f3, f7, rup\n"
			"fcmovm.ps f30, f15, f30\n"
			"fcvt.pw.ps f8, f3, rmm\n"
			"blt x21, x6, 1f\n"
			"1:\n"
			"fadd.pi f28, f15, f28\n"
			"fnmadd.s f8, f15, f30, f15, rne\n"
			"fmax.pi f3, f3, f15\n"
			"fmul.s f28, f14, f30, rtz\n"
			"fsra.pi f30, f1, f3\n"
			"fmadd.ps f14, f14, f15, f28, rdn\n"
			"for.pi f3, f30, f15\n"
			"fmul.s f7, f15, f7, rne\n"
			"fsgnjx.s f30, f14, f15\n"
			"fle.pi f16, f1, f3\n"
			"fsll.pi f30, f28, f15\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_5_LOOP_SEQ_HID_9\n"
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"la x31, check_sum_hid_9\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"LBL_POSTAMBLE_HID_9:\n"          
			"csrwi   tensor_wait, 0x0\n"
			"csrwi   tensor_wait, 0x1\n"
			"csrwi   tensor_wait, 0x2\n"
			"csrwi   tensor_wait, 0x3\n"
			"csrwi   tensor_wait, 0x4\n"
			"csrwi   tensor_wait, 0x5\n"
			"csrwi   tensor_wait, 0x6\n"
			"csrwi   tensor_wait, 0x7\n"
			"csrwi   tensor_wait, 0x8\n"
			"csrwi   tensor_wait, 0x9\n"
			"csrwi   tensor_wait, 0xa\n"
			"fence\n"
			"LBL_SYNC_POINT_1_HID_9:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_1_WAIT_HID_9\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_1_EXIT_HID_9\n"
			"LBL_SYNC_POINT_1_WAIT_HID_9:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_1_EXIT_HID_9:\n"          
			"nop\n"
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_9\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_9:\n"          
			".global LBL_FAIL_HID_9\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_9\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_9:\n"          
			".global LBL_PASS_HID_9\n"          
			VSNIP_RSV
		);
		c_test_pass();
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_9:\n"          
			".global LBL_HALT_HID_9\n"          
			"wfi\n"
			"j LBL_HALT_HID_9\n"          
			VSNIP_RSV
		);
		return 0;       
	};
	if ( hid == 10 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_10:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_10\n"
			"csrw mscratch, x5\n"
			"csrwi menable_shadows, 0xf\n"
			"li x5, -1\n"
			"csrw mcounteren, x5\n"
			"csrw scounteren, x5\n"
			"csrwi mhpmcounter3, 0\n"
			"csrwi mhpmcounter4, 0\n"
			"csrwi mhpmcounter5, 0\n"
			"csrwi mhpmcounter6, 0\n"
			"csrwi mhpmcounter7, 0\n"
			"csrwi mhpmcounter8, 0\n"
			"csrwi mhpmevent3, 21\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 28\n"   
			"csrwi mhpmevent6, 20\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 6\n"   
			"mov.m.x m0, x0, 0xff\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f12,  480 (x5)\n"
			"flw.ps f14,  352 (x5)\n"
			"flw.ps f17,  448 (x5)\n"
			"flw.ps f26,  64 (x5)\n"
			"flw.ps f7,  128 (x5)\n"
			"flw.ps f0,  832 (x5)\n"
			"flw.ps f13,  192 (x5)\n"
			"flw.ps f22,  224 (x5)\n"
			"flw.ps f18,  320 (x5)\n"
			"flw.ps f15,  928 (x5)\n"
			"flw.ps f10,  960 (x5)\n"
			"flw.ps f31,  768 (x5)\n"
			"flw.ps f4,  0 (x5)\n"
			"flw.ps f30,  96 (x5)\n"
			"flw.ps f25,  896 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f6,  32 (x5)\n"
			"flw.ps f5,  864 (x5)\n"
			"flw.ps f24,  416 (x5)\n"
			"flw.ps f9,  640 (x5)\n"
			"flw.ps f16,  576 (x5)\n"
			"flw.ps f3,  608 (x5)\n"
			"flw.ps f11,  160 (x5)\n"
			"flw.ps f20,  704 (x5)\n"
			"flw.ps f2,  992 (x5)\n"
			"flw.ps f28,  512 (x5)\n"
			"flw.ps f8,  672 (x5)\n"
			"flw.ps f27,  256 (x5)\n"
			"flw.ps f19,  544 (x5)\n"
			"flw.ps f1,  288 (x5)\n"
			"flw.ps f29,  384 (x5)\n"
			"flw.ps f23,  736 (x5)\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_HID_10:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_0_WAIT_HID_10\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_0_EXIT_HID_10\n"
			"LBL_SYNC_POINT_0_WAIT_HID_10:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_EXIT_HID_10:\n"          
			"nop\n"
			"LBL_KERNEL_HID_10:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_10\n"
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_HID_10:\n"
			"li x6, 0xe1ec59a3f389b4ac\n"
			"li x24, 0xd855f7a60f6d8281\n"
			"remu x28, x6, x24\n" 
			"mulhsu x26, x15, x6\n"
			"fmax.pi f1, f14, f24\n"
			"feq.ps f15, f14, f2\n"
			"fmin.s f21, f0, f7\n"
			"slti x19, x19, 0xf\n"
			"fclass.ps f0, f11\n"
			"fnmsub.s f20, f14, f13, f13, rtz\n"
			"addw x19, x19, x26\n"
			"li x29, 0x80637eaa7d\n"
			"sd x6, 512(x29)\n"
			"lw x18, 512(x29)\n" 
			"fcvt.s.w f13, x10, rne\n"
			"sra x19, x24, x26\n"
			"fpackrepb.pi f18, f18\n"
			"fadd.pi f18, f18, f14\n"
			"mulh x10, x10, x25\n"
			"for.pi f15, f20, f7\n"
			"lui x6, 0x1f\n"
			"fsub.pi f0, f0, f14\n"
			"mova.x.m x25\n" 
			"fadd.pi f11, f0, f11\n"
			"fmin.s f18, f14, f26\n"
			"bgeu x2, x24, 1f\n"
			"1:\n"
			"fcmovm.ps f11, f13, f13\n"
			"feq.ps f18, f18, f17\n"
			"fmax.s f1, f0, f24\n"
			"blt x4, x26, 1f\n"
			"1:\n"
			"fsrli.pi f26, f22, 0x1\n"
			"fmax.s f22, f22, f24\n"
			"fsgnjn.ps f11, f20, f22\n"
			"li x19, 0x80669b7648\n"
			"fsw.ps f18, -128(x19)\n" 
			"fle.ps f7, f13, f7\n"
			"bge x9, x9, 1f\n"
			"1:\n"
			"fnmsub.s f15, f7, f17, f15, rmm\n"
			"bltu x25, x13, 1f\n"
			"1:\n"
			"fmadd.ps f7, f7, f13, f18, rtz\n"
			"fsub.s f7, f7, f26, rne\n"
			"bgeu x28, x4, 1f\n"
			"1:\n"
			"fle.pi f22, f15, f0\n"
			"fminu.pi f14, f20, f20\n"
			"fmsub.ps f13, f18, f19, f0, rne\n"
			"bne x5, x25, 1f\n"
			"1:\n"
			"fexp.ps f2, f13\n"
			"li x18, 0x80a4369008\n"
			"la x11, mem_ptr_offset\n"
			"flw.ps f18, 160(x11)\n"
			"famoorl.pi f19, f18 (x18)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fcvt.pw.ps f7, f26, rne\n"
			"fsgnjx.s f20, f17, f0\n"
			"fmul.s f2, f15, f14, rup\n"
			"mulhsu x10, x15, x6\n"
			"fmsub.ps f26, f22, f22, f20, rtz\n"
			"bne x31, x2, 1f\n"
			"1:\n"
			"fnot.pi f7, f7\n"
			"slli x17, x28, 2\n"
			"addi x26, x15, 0x2\n"
			"li x15, 0x8062041428\n"
			"li x28, 1671084795\n"
			"fg32b.ps f11, x10 (x15)\n" 
			"fnmadd.s f15, f18, f18, f26, rup\n"
			"srli x11, x17, 1\n"
			"fcvt.wu.s x15, f14, rdn\n"
			"fmadd.ps f0, f21, f26, f0, rup\n"
			"fslli.pi f15, f22, 0x4\n"
			"mov.m.x m7, x25, 0x21\n"
			"srli x11, x11, 3\n"
			"fsrli.pi f26, f20, 0x0\n"
			"la x26, rand_int_32\n"
			"flw.ps f24, 640(x26)\n"
			"fpackrepb.pi f26, f24\n" 
			"fmax.pi f22, f22, f2\n"
			"slt x29, x29, x25\n"
			"fswizz.ps f7, f7, 0x15\n"
			"beq x9, x21, 1f\n"
			"1:\n"
			"slli x6, x6, 3\n"
			"fcvt.pw.ps f24, f20, rtz\n"
			"sub x6, x24, x25\n"
			"slli x24, x25, 4\n"
			"fsgnjn.s f11, f15, f0\n"
			"li x29, 0x80a59ff008\n"
			"la x11, mem_ptr_offset\n"
			"flw.ps f24, 832(x11)\n"
			"famomaxl.pi f2, f24 (x29)\n" 
			"fmv.x.s x6, f17\n"
			"fcvt.w.s x18, f1, rmm\n"
			"remu x26, x18, x26\n"
			"fle.ps f1, f18, f26\n"
			"fsgnjn.ps f1, f26, f18\n"
			"fand.pi f17, f7, f17\n"
			"fbcx.ps f26, x24\n"
			"bge x19, x16, 1f\n"
			"1:\n"
			"andi x6, x10, 0x11\n"
			"la x24, rand_ieee754_32\n"
			"flw.ps f22, 384(x24)\n"
			"fexp.ps f20, f22\n" 
			"maskpopc x24, m3\n"
			"sub x11, x28, x15\n"
			"fmul.s f0, f17, f7, rne\n"
			"bne x21, x16, 1f\n"
			"1:\n"
			"fsetm.pi m7, f22\n"
			"fmul.pi f18, f2, f14\n"
			"fmul.ps f24, f13, f24, rdn\n"
			"fmaxu.pi f1, f7, f11\n"
			"fcmov.ps f15, f22, f26, f26\n"
			"li x26, 0x80a30c7000\n"
			"amoorl.d x25, x18, (x26)\n" 
			"fclass.s x6, f0\n"
			"fsll.pi f11, f1, f24\n"
			"fle.ps f2, f2, f1\n"
			"mulh x11, x11, x26\n"
			"bge x25, x13, 1f\n"
			"1:\n"
			"srai x28, x10, 4\n"
			"slt x17, x10, x25\n"
			"sll x15, x15, x10\n"
			"fnmsub.ps f21, f7, f1, f20, rdn\n"
			"csrr x0, mip\n"
			"mul x11, x25, x28\n"
			"fswizz.ps f14, f21, 0x1e\n"
			"ebreak\n"
			"fnmadd.ps f19, f20, f19, f18, rmm\n"
			"bltu x9, x11, 1f\n"
			"1:\n"
			"fltm.pi m3, f18, f0\n"
			"fsub.pi f19, f19, f14\n"
			"remw x29, x29, x6\n"
			"fmul.pi f13, f7, f13\n"
			"li x11, 0x8094959030\n"
			"la x17, mem_ptr_offset\n"
			"flw.ps f19, 544(x17)\n"
			"famoorg.pi f0, f19 (x11)\n" 
			"srl x18, x18, x17\n"
			"fsgnjn.ps f17, f13, f19\n"
			"bge x11, x2, 1f\n"
			"1:\n"
			"or x15, x17, x26\n"
			"fsrai.pi f14, f15, 0x1\n"
			"maskand m5, m3, m1\n"
			"ebreak\n"
			"fmul.s f14, f18, f13, rtz\n"
			"flt.pi f15, f0, f26\n"
			"li x24, 0x8092d43008\n"
			"amoorg.d x25, x29, (x24)\n" 
			"li x5, 0x7b3e00c2dff9e970\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xfd0e07276c14e0ea\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"flt.pi f17, f18, f1\n"
			"fcvt.pw.ps f19, f1, rdn\n"
			"fcvt.ps.f16 f0, f0\n"
			"bge x16, x16, 1f\n"
			"1:\n"
			"fmsub.s f15, f13, f1, f15, rtz\n"
			"fnot.pi f22, f15\n"
			"fsll.pi f24, f18, f26\n"
			"fswizz.ps f11, f26, 0xf\n"
			"fmax.s f20, f7, f21\n"
			"li x25, 0x80a4650010\n"
			"la x28, mem_ptr_offset\n"
			"flw.ps f19, 576(x28)\n"
			"famoorl.pi f21, f19 (x25)\n" 
			"fsgnj.s f20, f0, f26\n"
			"fsrli.pi f17, f19, 0x2\n"
			"beq x18, x13, 1f\n"
			"1:\n"
			"fle.pi f22, f0, f22\n"
			"bgeu x6, x17, 1f\n"
			"1:\n"
			"fadd.ps f22, f2, f22, rne\n"
			"bne x15, x21, 1f\n"
			"1:\n"
			"fand.pi f19, f24, f24\n"
			"fmax.s f15, f15, f18\n"
			"fadd.pi f0, f14, f1\n"
			"feq.ps f0, f7, f2\n"
			"li x24, 0x80a3333020\n"
			"la x18, mem_ptr_offset\n"
			"flw.ps f0, 672(x18)\n"
			"famomaxul.pi f26, f0 (x24)\n" 
			"mulw x28, x24, x11\n"
			"fand.pi f2, f7, f2\n"
			"feq.ps f2, f2, f19\n"
			"ecall\n"
			"fsgnj.s f18, f2, f11\n"
			"beq x26, x19, 1f\n"
			"1:\n"
			"ecall\n"
			"fmvz.x.ps x29, f0, 5\n"
			"blt x27, x28, 1f\n"
			"1:\n"
			"fmadd.ps f2, f2, f1, f18, rtz\n"
			"li x6, 0x80621c5ff1\n"
			"sd x18, -512(x6)\n" 
			"fsgnj.s f0, f11, f0\n"
			"xori x28, x6, 0x1b\n"
			"div x10, x6, x17\n"
			"add x15, x18, x26\n"
			"fmin.s f26, f26, f0\n"
			"fadd.pi f11, f24, f11\n"
			"divuw x18, x26, x18\n"
			"flem.ps m5, f24, f26\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_0_TLD_ID_0_NMASK_0x1_CMASK_0xfb_SHIRE_0_HID_10:\n"
			"li x31, 0x1fb1e\n"
			"csrw tensor_coop, x31\n" 
			"li x31, 0x01FF000001FF0000\n"
			"csrw tensor_mask, x31\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"li x11, 0xd440008020006b28\n"
			"csrw tensor_load, x11\n" 
			"csrwi tensor_wait, 1\n" 
			"LBL_SYNC_POINT_1_HID_10:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_1_WAIT_HID_10\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_1_EXIT_HID_10\n"
			"LBL_SYNC_POINT_1_WAIT_HID_10:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_1_EXIT_HID_10:\n"          
			"nop\n"
			"LBL_SYNC_POINT_2_HID_10:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_2_WAIT_HID_10\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_2_EXIT_HID_10\n"
			"LBL_SYNC_POINT_2_WAIT_HID_10:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_2_EXIT_HID_10:\n"          
			"nop\n"
			"LBL_SYNC_POINT_3_HID_10:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_3_WAIT_HID_10\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_3_EXIT_HID_10\n"
			"LBL_SYNC_POINT_3_WAIT_HID_10:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_3_EXIT_HID_10:\n"          
			"nop\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, 0x1a96000000000007\n"
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 2\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f1, 4(x31)\n"
			"flw.ps f11, 3(x31)\n"
			"flw.ps f13, 2(x31)\n"
			"flw.ps f14, 3(x31)\n"
			"flw.ps f15, 10(x31)\n"
			"flw.ps f17, 7(x31)\n"
			"flw.ps f18, 27(x31)\n"
			"flw.ps f19, 27(x31)\n"
			"flw.ps f2, 35(x31)\n"
			"flw.ps f20, 22(x31)\n"
			"flw.ps f21, 22(x31)\n"
			"flw.ps f22, 33(x31)\n"
			"flw.ps f24, 9(x31)\n"
			"flw.ps f26, 6(x31)\n"
			"flw.ps f7, 5(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x10, 0(x31)\n"
			"lw x11, 0(x31)\n"
			"lw x15, 6(x31)\n"
			"lw x17, 12(x31)\n"
			"lw x18, 13(x31)\n"
			"lw x24, 6(x31)\n"
			"lw x25, 20(x31)\n"
			"lw x28, 14(x31)\n"
			"lw x29, 22(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_10:\n"
			"sraiw x6, x18, 0\n"
			"bltu x31, x16, 1f\n"
			"1:\n"
			"addw x18, x24, x15\n"
			"fadd.s f1, f18, f1, rne\n"
			"blt x19, x22, 1f\n"
			"1:\n"
			"sllw x11, x28, x29\n"
			"fbci.pi f1, 0x9dcc\n"
			"bgeu x30, x17, 1f\n"
			"1:\n"
			"fsrli.pi f18, f18, 0x2\n"
			"bge x20, x28, 1f\n"
			"1:\n"
			"mulhsu x25, x25, x10\n"
			"slliw x28, x10, 2\n"
			"slti x24, x24, 0x13\n"
			"bne x28, x12, 1f\n"
			"1:\n"
			"addiw x18, x24, 0x18\n"
			"fbci.ps f2, 0x23bc2\n"
			"fmin.s f13, f2, f24\n"
			"fcvt.ps.pwu f2, f22, rne\n"
			"flog.ps f19, f15\n"
			"mulh x18, x29, x18\n"
			"bne x16, x6, 1f\n"
			"1:\n"
			"maskpopcz x11, m5\n"
			"fcmovm.ps f14, f1, f22\n"
			"addw x29, x24, x15\n"
			"fltu.pi f1, f20, f20\n"
			"fnmadd.s f17, f26, f17, f17, rne\n"
			"bgeu x2, x17, 1f\n"
			"1:\n"
			"fmv.x.s x15, f19\n"
			"mulhsu x17, x17, x25\n"
			"ffrc.ps f26, f24\n"
			"blt x11, x1, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f14, f0, rtz\n"
			"sllw x11, x11, x18\n"
			"fsrli.pi f21, f11, 0x3\n"
			"fmul.ps f24, f15, f13, rdn\n"
			"fswizz.ps f2, f11, 0x1c\n"
			"feq.ps f24, f2, f21\n"
			"fsll.pi f22, f11, f24\n"
			"fcvt.ps.pw f7, f26, rmm\n"
			"fnmadd.s f19, f0, f11, f21, rne\n"
			"faddi.pi f0, f24, 0x8d\n"
			"fslli.pi f19, f14, 0x3\n"
			"fcvt.f16.ps f2, f2\n"
			"andi x24, x24, 0x12\n"
			"fadd.pi f17, f13, f18\n"
			"fsgnj.ps f20, f22, f20\n"
			"fadd.s f11, f15, f11, rtz\n"
			"slliw x6, x24, 5\n"
			"fence\n"
			"srai x10, x29, 3\n"
			"fexp.ps f26, f0\n"
			"bltu x13, x23, 1f\n"
			"1:\n"
			"fsat8.pi f2, f21\n"
			"fpackreph.pi f17, f7\n"
			"bgeu x30, x25, 1f\n"
			"1:\n"
			"flt.s x6, f2, f1\n"
			"fmsub.ps f18, f18, f18, f7, rup\n"
			"slt x15, x25, x15\n"
			"bge x30, x15, 1f\n"
			"1:\n"
			"srlw x24, x18, x24\n"
			"fbcx.ps f22, x24\n"
			"fmv.x.s x10, f2\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_0_LOOP_SEQ_HID_10\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_HID_10:\n"
			"li x11, 0x809188f008\n"
			"amoswapg.d x26, x6, (x11)\n" 
			"fcmov.ps f18, f22, f22, f26\n"
			"and x10, x28, x28\n"
			"remuw x29, x28, x10\n"
			"li x26, 0x80906b8020\n"
			"amoandg.d x18, x6, (x26)\n" 
			"feq.pi f21, f14, f15\n"
			"fmvz.x.ps x6, f15, 2\n"
			"fmsub.ps f20, f7, f2, f13, rup\n"
			"bgeu x1, x27, 1f\n"
			"1:\n"
			"li x19, 0x8061a32d20\n"
			"fsq2 f11, 512(x19)\n" 
			"fle.ps f20, f26, f20\n"
			"bne x29, x20, 1f\n"
			"1:\n"
			"fround.ps f17, f18, rtz\n"
			"fsrai.pi f17, f22, 0x3\n"
			"li x28, 0x80a50f7038\n"
			"la x24, mem_ptr_offset\n"
			"flw.ps f0, 640(x24)\n"
			"famomaxul.pi f20, f0 (x28)\n" 
			"fcvt.w.s x19, f26, rmm\n"
			"slli x11, x11, 0\n"
			"fexp.ps f21, f21\n"
			"csrr x0, stvec\n"
			"li x5, 0xe4a2\n"
			"csrw tensor_mask, x5\n"	
			"fmsub.s f0, f17, f24, f14, rne\n"
			"fcmov.ps f21, f0, f18, f21\n"
			"flt.pi f21, f19, f21\n"
			"lui x15, 0x1e\n" 
			"fnmadd.s f18, f20, f18, f20, rmm\n"
			"sraw x26, x28, x26\n"
			"fmadd.ps f21, f18, f18, f20, rtz\n"
			"li x26, 0x8094346020\n"
			"la x28, mem_ptr_offset\n"
			"flw.ps f15, 64(x28)\n"
			"famoming.pi f20, f15 (x26)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fnmsub.s f13, f13, f20, f26, rdn\n"
			"frcp.ps f11, f17\n"
			"fnmadd.s f20, f19, f20, f20, rup\n"
			"li x10, 0x8091ec7028\n"
			"amoandg.w x10, x29, (x10)\n" 
			"sltu x6, x6, x26\n"
			"fsetm.pi m1, f15\n"
			"mulhsu x17, x11, x11\n"
			"li x26, 0x80947ec030\n"
			"amomaxg.d x26, x15, (x26)\n" 
			"masknot m3, m3\n"
			"packb x6, x15, x10\n"
			"div x10, x10, x26\n"
			"maskxor m1, m5, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x66417ed675f890e9\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x16844bdd78df833\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"fadd.s f18, f18, f17, rup\n"
			"mov.m.x m3, x18, 0x37\n"
			"for.pi f20, f19, f15\n"
			"la x29, rand_int_32\n"
			"flw.ps f0, 384(x29)\n"
			"fpackreph.pi f24, f0\n" 
			"fcvt.pwu.ps f18, f11, rne\n"
			"fsub.s f21, f1, f26, rdn\n"
			"flog.ps f20, f18\n"
			"bgeu x20, x3, 1f\n"
			"1:\n"
			"la x26, rand_int_32\n"
			"flw.ps f15, 480(x26)\n"
			"fminu.pi f15, f15, f15\n" 
			"fpackrepb.pi f0, f13\n"
			"for.pi f17, f0, f7\n"
			"fcvt.ps.f16 f2, f13\n"
			"li x11, 0x80652ca530\n"
			"fsw.ps f1, -512(x11)\n"
			"flw.ps f24, -512(x11)\n" 
			"li x5, 0xcd3f128e56e4969f\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x74c36bbf4295bed0\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"slli x19, x10, 5\n"
			"maskand m1, m5, m3\n"
			"bge x7, x10, 1f\n"
			"1:\n"
			"or x25, x26, x28\n"
			"beq x24, x21, 1f\n"
			"1:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 256(x17)\n"
			"flw.ps f15, 992(x17)\n"
			"fsub.ps f24, f7, f15, rdn\n" 
			"fsub.pi f7, f15, f15\n"
			"mulh x29, x18, x15\n"
			"fmin.s f22, f7, f15\n"
			"li x26, 0x806740b450\n"
			"li x25, 4109461221\n"
			"fsc32w.ps f22, x18 (x26)\n" 
			"maskor m3, m3, m5\n"
			"remuw x15, x19, x15\n"
			"fpackrepb.pi f1, f2\n"
			"li x11, 0x8065e21070\n"
			"la x28, mem_ptr_offset\n"
			"flw.ps f15, 608(x28)\n"
			"fscw.ps f0, f15 (x11)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fmul.ps f14, f26, f21, rne\n"
			"fltu.pi f1, f18, f13\n"
			"fsll.pi f22, f26, f20\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_1_TLD_ID_1_NMASK_0x1_CMASK_0xbf_SHIRE_0_HID_10:\n"
			"li x31, 0x1bf0e\n"
			"csrw tensor_coop, x31\n" 
			"li x31, 0x01FF000001FF0000\n"
			"csrw tensor_mask, x31\n" 
			"li x31, (0x3 << 6) | 0x1\n"
			"li x15, 0x4010008020006b0b\n"
			"csrw tensor_load, x15\n" 
			"li x7, 0x2d800000118002\n"
			"csrw tensor_fma, x7\n" 
			"LBL_SYNC_POINT_4_HID_10:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_4_WAIT_HID_10\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_4_EXIT_HID_10\n"
			"LBL_SYNC_POINT_4_WAIT_HID_10:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_4_EXIT_HID_10:\n"          
			"nop\n"
			"LBL_SYNC_POINT_5_HID_10:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_5_WAIT_HID_10\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_5_EXIT_HID_10\n"
			"LBL_SYNC_POINT_5_WAIT_HID_10:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_5_EXIT_HID_10:\n"          
			"nop\n"
			"LBL_SYNC_POINT_6_HID_10:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_6_WAIT_HID_10\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_6_EXIT_HID_10\n"
			"LBL_SYNC_POINT_6_WAIT_HID_10:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_6_EXIT_HID_10:\n"          
			"nop\n"
			"li x5, 0xf1f9\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, 0x38000080600003c3\n"
			"li x6, 0x1380008060002681\n"
			"li x7, 0x11880000001c002\n"
			"li x9, 0x251de00006666666\n"
			"li x10, 0x8fb00080600003c0\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"li x31, (0x40 << 4)\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrwi tensor_wait, 8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 3\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x40\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f1, 3(x31)\n"
			"flw.ps f11, 7(x31)\n"
			"flw.ps f13, 0(x31)\n"
			"flw.ps f14, 11(x31)\n"
			"flw.ps f15, 7(x31)\n"
			"flw.ps f17, 5(x31)\n"
			"flw.ps f18, 25(x31)\n"
			"flw.ps f19, 2(x31)\n"
			"flw.ps f2, 2(x31)\n"
			"flw.ps f20, 36(x31)\n"
			"flw.ps f21, 43(x31)\n"
			"flw.ps f22, 16(x31)\n"
			"flw.ps f24, 7(x31)\n"
			"flw.ps f26, 30(x31)\n"
			"flw.ps f7, 47(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_10:\n"
			"fmax.s f11, f7, f24\n"
			"fcmov.ps f20, f15, f7, f15\n"
			"fmul.ps f22, f1, f14, rup\n"
			"fmin.ps f15, f14, f15\n"
			"fmin.ps f0, f14, f7\n"
			"fcvt.f16.ps f11, f11\n"
			"fmulhu.pi f24, f7, f15\n"
			"fsat8.pi f20, f14\n"
			"fmin.ps f0, f14, f17\n"
			"fexp.ps f21, f21\n"
			"fsub.pi f14, f14, f0\n"
			"fle.ps f0, f24, f21\n"
			"bge x10, x2, 1f\n"
			"1:\n"
			"fsgnjn.ps f13, f17, f19\n"
			"bltu x22, x21, 1f\n"
			"1:\n"
			"fpackrepb.pi f21, f18\n"
			"fadd.s f14, f0, f14, rup\n"
			"beq x23, x12, 1f\n"
			"1:\n"
			"fmadd.s f20, f17, f0, f15, rdn\n"
			"fmulhu.pi f7, f26, f21\n"
			"flt.pi f11, f20, f7\n"
			"blt x28, x10, 1f\n"
			"1:\n"
			"for.pi f15, f26, f22\n"
			"feq.ps f11, f7, f20\n"
			"fclass.ps f18, f18\n"
			"fmax.s f14, f15, f22\n"
			"fmulhu.pi f11, f11, f11\n"
			"feq.pi f22, f21, f7\n"
			"bge x28, x23, 1f\n"
			"1:\n"
			"fcvt.f16.ps f26, f18\n"
			"fmsub.s f7, f17, f24, f0, rdn\n"
			"fadd.ps f2, f20, f7, rup\n"
			"fmaxu.pi f26, f0, f14\n"
			"fcmov.ps f14, f20, f24, f11\n"
			"fcvt.ps.pw f20, f2, rtz\n"
			"bltu x27, x15, 1f\n"
			"1:\n"
			"flog.ps f17, f21\n"
			"fsra.pi f21, f11, f1\n"
			"bgeu x19, x23, 1f\n"
			"1:\n"
			"fsra.pi f14, f11, f21\n"
			"feq.pi f18, f15, f21\n"
			"bge x22, x8, 1f\n"
			"1:\n"
			"fmax.pi f11, f19, f11\n"
			"faddi.pi f7, f14, 0x13d\n"
			"fadd.pi f19, f2, f18\n"
			"fround.ps f22, f0, rmm\n"
			"fmul.s f2, f15, f1, rdn\n"
			"flog.ps f26, f26\n"
			"fltu.pi f20, f0, f18\n"
			"fle.ps f0, f20, f20\n"
			"fsgnjx.ps f2, f24, f2\n"
			"fadd.pi f18, f18, f20\n"
			"fmax.ps f17, f1, f14\n"
			"fmsub.s f18, f17, f22, f17, rtz\n"
			"fnmadd.s f1, f7, f0, f1, rtz\n"
			"fnmsub.ps f21, f14, f0, f22, rup\n"
			"fsra.pi f18, f13, f17\n"
			"fmadd.ps f7, f20, f7, f26, rmm\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_1_LOOP_SEQ_HID_10\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_HID_10:\n"
			"li x17, 0x80a0d4e030\n"
			"amominul.d x17, x11, (x17)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"ffrc.ps f17, f20\n"
			"fround.ps f0, f21, rup\n"
			"fsgnjn.ps f21, f14, f21\n"
			"blt x11, x6, 1f\n"
			"1:\n"
			"fcmovm.ps f26, f1, f0\n"
			"flt.pi f22, f24, f22\n"
			"bne x1, x16, 1f\n"
			"1:\n"
			"fmax.s f17, f1, f11\n"
			"fmsub.s f20, f20, f0, f24, rup\n"
			"fcmov.ps f22, f21, f21, f22\n"
			"bgeu x20, x20, 1f\n"
			"1:\n"
			"fsatu8.pi f26, f7\n"
			"li x10, 0x80a70a9028\n"
			"la x18, mem_ptr_offset\n"
			"flw.ps f22, 832(x18)\n"
			"fscbl.ps f2, f22 (x10)\n" 
			"fnmadd.s f22, f7, f7, f24, rup\n"
			"fslli.pi f11, f24, 0x9\n"
			"fnmadd.s f22, f1, f2, f11, rup\n"
			"fsgnj.s f7, f2, f1\n"
			"bge x2, x4, 1f\n"
			"1:\n"
			"fmax.ps f22, f1, f24\n"
			"fsgnj.ps f22, f2, f7\n"
			"fmul.s f1, f1, f1, rdn\n"
			"fpackrepb.pi f7, f2\n"
			"bne x31, x16, 1f\n"
			"1:\n"
			"fswizz.ps f2, f7, 0x11\n"
			"bgeu x23, x18, 1f\n"
			"1:\n"
			"li x17, 0x8060559868\n"
			"fsq2 f17, -256(x17)\n" 
			"li x5, 0xae98\n"
			"csrw tensor_mask, x5\n"	
			"fsat8.pi f18, f13\n"
			"fmax.s f22, f24, f26\n"
			"fnmadd.ps f22, f24, f24, f18, rne\n"
			"fmin.pi f18, f13, f18\n"
			"fmin.pi f24, f26, f24\n"
			"bge x2, x23, 1f\n"
			"1:\n"
			"fmin.pi f17, f18, f24\n"
			"fsub.pi f24, f13, f24\n"
			"beq x18, x31, 1f\n"
			"1:\n"
			"fmin.ps f18, f24, f24\n"
			"fmulhu.pi f13, f13, f13\n"
			"blt x16, x20, 1f\n"
			"1:\n"
			"feqm.ps m5, f15, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fminu.pi f1, f26, f15\n"
			"bltu x20, x6, 1f\n"
			"1:\n"
			"fcmovm.ps f1, f26, f11\n"
			"bltu x9, x19, 1f\n"
			"1:\n"
			"fsatu8.pi f14, f1\n"
			"faddi.pi f15, f15, 0x166\n"
			"fsrai.pi f18, f18, 0xf\n"
			"fsgnjx.s f11, f26, f11\n"
			"fmax.pi f24, f15, f18\n"
			"blt x0, x24, 1f\n"
			"1:\n"
			"fpackrepb.pi f1, f2\n"
			"fsra.pi f14, f19, f15\n"
			"bge x31, x18, 1f\n"
			"1:\n"
			"li x6, 0x8096f9c028\n"
			"amominug.d x6, x28, (x6)\n" 
			"fsatu8.pi f13, f17\n"
			"bge x25, x29, 1f\n"
			"1:\n"
			"feq.ps f7, f15, f1\n"
			"bne x8, x12, 1f\n"
			"1:\n"
			"fsub.s f7, f15, f1, rup\n"
			"fnmsub.ps f21, f2, f22, f22, rdn\n"
			"fmaxu.pi f15, f15, f14\n"
			"bgeu x19, x15, 1f\n"
			"1:\n"
			"fpackreph.pi f22, f18\n"
			"fsra.pi f22, f24, f19\n"
			"bltu x0, x14, 1f\n"
			"1:\n"
			"fmul.s f13, f18, f17, rne\n"
			"fsgnjn.s f24, f18, f0\n"
			"beq x14, x16, 1f\n"
			"1:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f20, 32(x15)\n"
			"fcvt.lu.s x15, f20, rdn\n" 
			"frcp.ps f2, f24\n"
			"fpackrepb.pi f24, f0\n"
			"fmadd.ps f0, f24, f17, f0, rup\n"
			"fcvt.ps.pw f0, f24, rdn\n"
			"for.pi f2, f2, f17\n"
			"fminu.pi f15, f2, f17\n"
			"fsub.ps f0, f2, f24, rtz\n"
			"fmax.ps f2, f24, f2\n"
			"fsra.pi f20, f2, f17\n"
			"la x15, rand_int_32\n"
			"flw.ps f24, 672(x15)\n"
			"fnot.pi f19, f24\n" 
			"li x5, 0x4dfb\n"
			"csrw tensor_mask, x5\n"	
			"divw x18, x10, x6\n"
			"fcmov.ps f17, f14, f26, f17\n"
			"mulhsu x29, x29, x29\n"
			"bne x22, x15, 1f\n"
			"1:\n"
			"maskor m5, m7, m1\n"
			"ebreak\n"
			"addw x26, x19, x25\n"
			"fmax.pi f13, f26, f26\n"
			"fsgnjx.s f18, f14, f14\n"
			"feq.ps f2, f14, f17\n"
			"beq x24, x1, 1f\n"
			"1:\n"
			"li x24, 0x8060694f90\n"
			"fsw.ps f26, -512(x24)\n"
			"flw.ps f14, -512(x24)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fand.pi f20, f20, f24\n"
			"addiw x17, x17, 0xb\n"
			"blt x2, x15, 1f\n"
			"1:\n"
			"fxor.pi f13, f15, f17\n"
			"sllw x18, x18, x18\n"
			"bltu x16, x12, 1f\n"
			"1:\n"
			"divu x28, x15, x10\n"
			"fsra.pi f17, f19, f17\n"
			"fsetm.pi m7, f15\n"
			"feqm.ps m7, f24, f0\n"
			"mova.x.m x25\n"
			"li x6, 0x8094f59020\n"
			"la x18, mem_ptr_offset\n"
			"flw.ps f20, 544(x18)\n"
			"famoandg.pi f26, f20 (x6)\n" 
			"fpackreph.pi f21, f0\n"
			"fmin.s f18, f15, f1\n"
			"fmin.s f22, f0, f2\n"
			"bltu x20, x22, 1f\n"
			"1:\n"
			"fsrai.pi f14, f1, 0xb\n"
			"fand.pi f22, f22, f2\n"
			"fmaxu.pi f18, f26, f0\n"
			"blt x6, x16, 1f\n"
			"1:\n"
			"feq.pi f20, f26, f14\n"
			"beq x26, x28, 1f\n"
			"1:\n"
			"fsgnjn.ps f15, f18, f2\n"
			"fsat8.pi f22, f21\n"
			"li x25, 0x80948df018\n"
			"amoorg.d x11, x29, (x25)\n" 
			"fandi.pi f20, f19, 0x3c\n"
			"fmaxu.pi f24, f22, f15\n"
			"fand.pi f15, f15, f20\n"
			"fclass.ps f22, f24\n"
			"mova.m.x x15\n"
			"bge x5, x11, 1f\n"
			"1:\n"
			"fsrai.pi f0, f2, 0xa\n"
			"ffrc.ps f13, f14\n"
			"fsgnj.ps f18, f14, f2\n"
			"fmax.ps f19, f0, f18\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f0, 64(x11)\n"
			"flw.ps f21, 864(x11)\n"
			"fsub.s f14, f0, f21, rmm\n" 
			"fsgnjx.ps f22, f24, f20\n"
			"auipc x28, 0x13\n"
			"fmv.x.s x18, f18\n"
			"auipc x6, 0x14\n"
			"fmulh.pi f17, f14, f1\n"
			"fpackreph.pi f21, f24\n"
			"slli x26, x17, 5\n"
			"fmadd.s f15, f7, f18, f11, rtz\n"
			"fcvt.ps.pw f19, f19, rdn\n"
			"li x19, 0x80949df000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f24, (x19)\n" 
			"fsub.ps f2, f18, f18, rup\n"
			"fence\n"
			"fbci.ps f22, 0x2381a\n"
			"fcvt.ps.pw f2, f11, rmm\n"
			"slt x29, x6, x24\n"
			"fnot.pi f17, f18\n"
			"flt.pi f22, f22, f18\n"
			"mulhu x6, x6, x6\n"
			"fmin.pi f17, f22, f11\n"
			"la x15, rand_ieee754_16\n"
			"flw.ps f26, 736(x15)\n"
			"fcvt.ps.f16 f17, f26\n" 
			"fmv.x.s x15, f24\n"
			"fswizz.ps f21, f14, 0x1f\n"
			"maskor m5, m7, m5\n"
			"fnot.pi f0, f1\n"
			"addi x28, x29, 0x1\n"
			"blt x6, x31, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f7, f13\n"
			"blt x21, x13, 1f\n"
			"1:\n"
			"ebreak\n"
			"fsrai.pi f19, f1, 0x5\n"
			"srai x29, x29, 3\n"
			"beq x11, x13, 1f\n"
			"1:\n"
			"li x10, 0x806204ca38\n"
			"li x17, 324169977\n"
			"fg32h.ps f14, x10 (x10)\n" 
			"fcvt.pw.ps f21, f21, rmm\n"
			"fnmadd.ps f21, f18, f21, f21, rmm\n"
			"fsgnjn.s f18, f18, f20\n"
			"slliw x28, x19, 0\n"
			"fexp.ps f14, f20\n"
			"bltu x1, x25, 1f\n"
			"1:\n"
			"fsatu8.pi f20, f21\n"
			"fsra.pi f18, f21, f21\n"
			"fsgnj.s f14, f21, f21\n"
			"fsrl.pi f14, f21, f18\n"
			"bgeu x27, x17, 1f\n"
			"1:\n"
			"li x18, 0x8091e72020\n"
			"amoandg.d x19, x17, (x18)\n" 
			"frcp.ps f24, f1\n"
			"sraw x29, x29, x11\n"
			"subw x24, x24, x11\n"
			"maskxor m5, m7, m5\n"
			"div x19, x10, x11\n"
			"fcvt.ps.pw f14, f15, rup\n"
			"fcvt.w.s x19, f15, rne\n"
			"ori x18, x24, 0x18\n"
			"fmsub.s f11, f11, f14, f11, rtz\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f20, 736(x26)\n"
			"fcvt.pwu.ps f19, f20, rmm\n" 
			"maskpopc x18, m1\n"
			"fmaxu.pi f20, f20, f7\n"
			"flt.pi f19, f11, f26\n"
			"bne x12, x22, 1f\n"
			"1:\n"
			"fmin.pi f26, f7, f22\n"
			"bltu x1, x26, 1f\n"
			"1:\n"
			"fclass.ps f15, f11\n"
			"fle.s x17, f2, f22\n"
			"masknot m3, m7\n"
			"maskpopc x28, m7\n"
			"bne x20, x17, 1f\n"
			"1:\n"
			"fmax.pi f21, f15, f7\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"li x5, 0xae99\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, 0x1427000002b7027\n"
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 3\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x18\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f1, 4(x31)\n"
			"flw.ps f11, 7(x31)\n"
			"flw.ps f13, 4(x31)\n"
			"flw.ps f14, 6(x31)\n"
			"flw.ps f15, 3(x31)\n"
			"flw.ps f17, 16(x31)\n"
			"flw.ps f18, 2(x31)\n"
			"flw.ps f19, 26(x31)\n"
			"flw.ps f2, 17(x31)\n"
			"flw.ps f20, 10(x31)\n"
			"flw.ps f21, 11(x31)\n"
			"flw.ps f22, 4(x31)\n"
			"flw.ps f24, 20(x31)\n"
			"flw.ps f26, 5(x31)\n"
			"flw.ps f7, 7(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x10, 0(x31)\n"
			"lw x11, 1(x31)\n"
			"lw x15, 5(x31)\n"
			"lw x17, 8(x31)\n"
			"lw x18, 9(x31)\n"
			"lw x24, 11(x31)\n"
			"lw x25, 22(x31)\n"
			"lw x26, 10(x31)\n"
			"lw x28, 28(x31)\n"
			"lw x29, 27(x31)\n"
			"lw x6, 30(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_10:\n"
			"fsub.pi f20, f11, f2\n"
			"flt.ps f22, f22, f13\n"
			"flt.pi f21, f17, f21\n"
			"fcvt.f16.ps f14, f11\n"
			"addiw x26, x18, 0x4\n"
			"srai x6, x6, 3\n"
			"xor x18, x26, x26\n"
			"fsgnjn.s f22, f26, f2\n"
			"fcvt.f16.ps f14, f19\n"
			"fsub.s f22, f15, f24, rdn\n"
			"fcvt.pw.ps f0, f20, rne\n"
			"fround.ps f15, f26, rmm\n"
			"srai x28, x29, 1\n"
			"slliw x19, x28, 5\n"
			"bgeu x24, x27, 1f\n"
			"1:\n"
			"fmax.pi f14, f13, f11\n"
			"fmul.pi f26, f11, f7\n"
			"mul x6, x11, x28\n"
			"fcmovm.ps f11, f7, f14\n"
			"fmaxu.pi f18, f20, f1\n"
			"fmulh.pi f26, f20, f7\n"
			"sraiw x26, x25, 1\n"
			"mul x17, x11, x6\n"
			"beq x25, x7, 1f\n"
			"1:\n"
			"fsrli.pi f14, f15, 0xa\n"
			"remu x10, x10, x26\n"
			"bge x17, x6, 1f\n"
			"1:\n"
			"fsub.pi f17, f19, f21\n"
			"fcvt.pwu.ps f13, f13, rmm\n"
			"bge x17, x0, 1f\n"
			"1:\n"
			"fandi.pi f11, f11, 0x1c4\n"
			"fle.pi f17, f22, f13\n"
			"fmadd.s f18, f0, f19, f18, rtz\n"
			"fsgnjn.ps f22, f24, f22\n"
			"fcvt.ps.pwu f20, f1, rtz\n"
			"feq.pi f0, f0, f7\n"
			"srl x11, x10, x17\n"
			"fmin.ps f19, f24, f19\n"
			"mulw x17, x15, x17\n"
			"fmsub.s f22, f20, f2, f22, rne\n"
			"divw x15, x28, x15\n"
			"bge x7, x15, 1f\n"
			"1:\n"
			"divu x15, x28, x10\n"
			"fadd.ps f22, f20, f26, rtz\n"
			"feq.ps f18, f14, f19\n"
			"mulhu x17, x17, x28\n"
			"addi x6, x24, 0x16\n"
			"bgeu x19, x14, 1f\n"
			"1:\n"
			"slt x24, x17, x25\n"
			"fsgnjx.s f15, f14, f13\n"
			"fsatu8.pi f26, f17\n"
			"fadd.pi f1, f14, f1\n"
			"srl x18, x26, x18\n"
			"fsll.pi f26, f20, f18\n"
			"fsatu8.pi f26, f2\n"
			"fnmadd.ps f26, f17, f20, f7, rtz\n"
			"fmul.ps f1, f14, f24, rtz\n"
			"bne x16, x23, 1f\n"
			"1:\n"
			"fmulh.pi f1, f17, f26\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_2_LOOP_SEQ_HID_10\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f24,  0 (x5)\n"
			"flw.ps f28,  800 (x5)\n"
			"flw.ps f23,  544 (x5)\n"
			"flw.ps f20,  608 (x5)\n"
			"flw.ps f30,  960 (x5)\n"
			"flw.ps f2,  512 (x5)\n"
			"flw.ps f11,  32 (x5)\n"
			"flw.ps f5,  352 (x5)\n"
			"flw.ps f15,  320 (x5)\n"
			"flw.ps f27,  928 (x5)\n"
			"flw.ps f3,  896 (x5)\n"
			"flw.ps f10,  224 (x5)\n"
			"flw.ps f26,  736 (x5)\n"
			"flw.ps f16,  384 (x5)\n"
			"flw.ps f7,  416 (x5)\n"
			"flw.ps f12,  992 (x5)\n"
			"flw.ps f13,  672 (x5)\n"
			"flw.ps f29,  448 (x5)\n"
			"flw.ps f17,  288 (x5)\n"
			"flw.ps f8,  640 (x5)\n"
			"flw.ps f14,  480 (x5)\n"
			"flw.ps f21,  128 (x5)\n"
			"flw.ps f19,  64 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f4,  704 (x5)\n"
			"flw.ps f0,  96 (x5)\n"
			"flw.ps f1,  576 (x5)\n"
			"flw.ps f22,  768 (x5)\n"
			"flw.ps f31,  832 (x5)\n"
			"flw.ps f9,  864 (x5)\n"
			"flw.ps f18,  256 (x5)\n"
			"flw.ps f6,  160 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_3_HID_10:\n"
			"li x6, 0x8096d3c030\n"
			"la x24, mem_ptr_offset\n"
			"flw.ps f2, 896(x24)\n"
			"fghg.ps f22, f2 (x6)\n" 
			"rem x26, x26, x19\n"
			"bgeu x29, x4, 1f\n"
			"1:\n"
			"fsat8.pi f14, f1\n"
			"andi x26, x29, 0x1e\n"
			"fbcx.ps f1, x11\n"
			"maskpopcz x26, m5\n"
			"sll x25, x24, x24\n"
			"li x10, 0x80a2e55008\n"
			"la x19, mem_ptr_offset\n"
			"flw.ps f19, 0(x19)\n"
			"famomaxul.pi f24, f19 (x10)\n" 
			"divu x15, x15, x10\n"
			"fsgnjx.s f17, f15, f22\n"
			"fence\n"
			"fsub.ps f17, f2, f13, rne\n"
			"fsgnjx.ps f11, f20, f21\n"
			"divuw x26, x6, x26\n"
			"li x26, 0x95c772186e5d08fb\n"
			"srai x10, x26, 4\n" 
			"srliw x26, x11, 1\n"
			"sll x6, x10, x6\n"
			"bne x27, x11, 1f\n"
			"1:\n"
			"div x11, x11, x19\n"
			"and x18, x18, x25\n"
			"slliw x15, x19, 5\n"
			"fence\n"
			"li x6, 0x8094032018\n"
			"amomaxg.d x29, x19, (x6)\n" 
			"packb x11, x26, x24\n"
			"mova.x.m x6\n"
			"sra x29, x26, x24\n"
			"fbcx.ps f22, x18\n"
			"lui x29, 0x4\n"
			"bltu x2, x23, 1f\n"
			"1:\n"
			"fbcx.ps f21, x24\n"
			"li x15, 0x8096c17008\n"
			"la x19, mem_ptr_offset\n"
			"flw.ps f14, 992(x19)\n"
			"famoswapg.pi f13, f14 (x15)\n" 
			"fmul.s f20, f15, f20, rtz\n"
			"fmax.pi f26, f15, f2\n"
			"fltm.pi m7, f2, f20\n"
			"bltu x3, x11, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f7, f26, rmm\n"
			"beq x21, x7, 1f\n"
			"1:\n"
			"auipc x26, 0x9\n"
			"fcvt.f16.ps f13, f15\n"
			"li x24, 0x3587396a42427bac\n"
			"li x6, 0x55dd5ad3cbb769fb\n"
			"sub x15, x24, x6\n" 
			"mul x26, x26, x28\n"
			"maskand m7, m7, m1\n"
			"feq.s x15, f26, f22\n"
			"sraw x25, x10, x6\n"
			"remuw x17, x26, x17\n"
			"fswizz.ps f26, f18, 0x17\n"
			"li x19, 0x80968db004\n"
			"amoaddg.w x11, x26, (x19)\n" 
			"fmaxu.pi f11, f17, f13\n"
			"fnmsub.ps f14, f0, f26, f14, rup\n"
			"fcvt.f16.ps f15, f24\n"
			"fpackreph.pi f17, f17\n"
			"flog.ps f21, f22\n"
			"fslli.pi f14, f14, 0xc\n"
			"maskor m7, m1, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fcvt.ps.pwu f14, f14, rup\n"
			"bgeu x16, x21, 1f\n"
			"1:\n"
			"fltu.pi f13, f15, f14\n"
			"frcp.ps f26, f15\n"
			"fnmsub.ps f17, f19, f11, f14, rup\n"
			"fnot.pi f26, f26\n"
			"fcvt.pw.ps f17, f14, rmm\n"
			"beq x22, x23, 1f\n"
			"1:\n"
			"li x15, 0x8094bb801c\n"
			"amomaxug.w x10, x15, (x15)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"sub x28, x10, x15\n"
			"masknot m5, m5\n"
			"fmax.ps f2, f1, f0\n"
			"auipc x29, 0xf\n"
			"sraw x10, x19, x26\n"
			"fcvt.s.wu f17, x10, rmm\n"
			"li x25, 0x8094655038\n"
			"la x10, mem_ptr_offset\n"
			"flw.ps f18, 32(x10)\n"
			"famominug.pi f11, f18 (x25)\n" 
			"fmadd.ps f15, f15, f13, f21, rmm\n"
			"fsrai.pi f17, f13, 0x1\n"
			"fmin.ps f0, f19, f0\n"
			"fsrli.pi f13, f13, 0xa\n"
			"faddi.pi f7, f0, 0x1be\n"
			"fmulhu.pi f13, f13, f22\n"
			"la x24, rand_ieee754_32\n"
			"flw.ps f14, 704(x24)\n"
			"flw.ps f2, 576(x24)\n"
			"flw.ps f0, 128(x24)\n"
			"fnmsub.s f18, f14, f2, f0, rmm\n" 
			"li x5, 0xd31843881619bd61\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xd7a03e68f0731789\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"fmsub.s f19, f13, f21, f0, rtz\n"
			"fmul.s f15, f22, f13, rmm\n"
			"bne x21, x17, 1f\n"
			"1:\n"
			"fsgnjn.ps f7, f7, f19\n"
			"bltu x7, x2, 1f\n"
			"1:\n"
			"sll x10, x11, x15\n"
			"fmax.s f19, f24, f18\n"
			"fcvt.ps.pw f18, f24, rne\n"
			"li x25, 0x80a3b36038\n"
			"amoandl.d x6, x26, (x25)\n" 
			"fsatu8.pi f18, f26\n"
			"fsub.s f13, f24, f13, rmm\n"
			"fnmsub.ps f21, f11, f15, f14, rup\n"
			"flt.ps f17, f17, f14\n"
			"fcmovm.ps f26, f7, f15\n"
			"fsat8.pi f24, f11\n"
			"li x17, 0x80914e7038\n"
			"amomaxg.w x6, x15, (x17)\n" 
			"fbcx.ps f7, x18\n"
			"fcvt.s.w f7, x10, rdn\n"
			"slt x29, x18, x10\n"
			"fcvt.s.wu f11, x18, rne\n"
			"or x28, x18, x29\n"
			"ecall\n"
			"maskpopcz x25, m3\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"ecall\n"
			"mulhsu x6, x17, x25\n"
			"fmul.ps f2, f2, f2, rmm\n"
			"fltu.pi f21, f21, f21\n"
			"fmulh.pi f19, f19, f22\n"
			"fbci.ps f18, 0x3f7c0\n"
			"fbci.pi f20, 0x2e333\n" 
			"fcvt.ps.pwu f2, f7, rmm\n"
			"fmax.pi f21, f1, f1\n"
			"slliw x6, x25, 3\n"
			"fadd.pi f1, f2, f20\n"
			"feq.ps f7, f14, f21\n"
			"sraiw x28, x15, 0\n"
			"la x25, rand_int_32\n"
			"flw.ps f0, 128(x25)\n"
			"flw.ps f7, 640(x25)\n"
			"fmulh.pi f26, f0, f7\n" 
			"remuw x10, x29, x29\n"
			"bge x8, x27, 1f\n"
			"1:\n"
			"fmaxu.pi f14, f20, f17\n"
			"maskand m3, m1, m3\n"
			"bltu x14, x10, 1f\n"
			"1:\n"
			"fle.s x29, f11, f17\n"
			"fpackrepb.pi f14, f17\n"
			"bltu x28, x12, 1f\n"
			"1:\n"
			"mov.m.x m5, x24, 0x7c\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, 0x8e000806000868d\n"
			"li x6, 0x3be0008060004c4b\n"
			"li x7, 0xed80000041f076\n"
			"li x9, 0x36ef800895844aa8\n"
			"li x10, 0xe109008060002680\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"li x31, (0x1 << 6)\n"
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_1_HID_10:\n"
			"li x5, 0x38a0008060004c41\n"
			"li x6, 0x10008060004c45\n"
			"li x7, 0x18a80000051c057\n"
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_2_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0x60000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x38000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, 0x1d00000001c031\n"
			"li x9, 0x2a1c200000002776\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xb\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 3\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x40\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f11, 1(x31)\n"
			"flw.ps f13, 1(x31)\n"
			"flw.ps f14, 3(x31)\n"
			"flw.ps f15, 4(x31)\n"
			"flw.ps f17, 17(x31)\n"
			"flw.ps f18, 12(x31)\n"
			"flw.ps f19, 17(x31)\n"
			"flw.ps f2, 15(x31)\n"
			"flw.ps f20, 0(x31)\n"
			"flw.ps f21, 39(x31)\n"
			"flw.ps f22, 10(x31)\n"
			"flw.ps f24, 38(x31)\n"
			"flw.ps f26, 9(x31)\n"
			"flw.ps f7, 35(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x10, 0(x31)\n"
			"lw x11, 2(x31)\n"
			"lw x15, 2(x31)\n"
			"lw x17, 3(x31)\n"
			"lw x18, 11(x31)\n"
			"lw x19, 5(x31)\n"
			"lw x24, 15(x31)\n"
			"lw x25, 10(x31)\n"
			"lw x26, 9(x31)\n"
			"lw x28, 22(x31)\n"
			"lw x6, 11(x31)\n"
			"li x31, 0x2\n"
			"LBL_SEQID_3_LOOP_SEQ_HID_10:\n"
			"slti x29, x10, 0x1c\n"
			"ffrc.ps f22, f2\n"
			"sub x17, x11, x17\n"
			"fand.pi f26, f1, f13\n"
			"remw x19, x15, x19\n"
			"ori x18, x15, 0x9\n"
			"srli x11, x24, 1\n"
			"fadd.s f24, f24, f17, rtz\n"
			"remuw x10, x17, x26\n"
			"remuw x18, x18, x28\n"
			"bgeu x1, x31, 1f\n"
			"1:\n"
			"fnot.pi f2, f14\n"
			"flt.ps f18, f15, f7\n"
			"andi x15, x10, 0x1b\n"
			"fsub.ps f21, f2, f19, rup\n"
			"flog.ps f19, f15\n"
			"for.pi f22, f13, f17\n"
			"blt x25, x16, 1f\n"
			"1:\n"
			"fexp.ps f13, f17\n"
			"slt x25, x25, x19\n"
			"fmin.ps f24, f17, f24\n"
			"fltu.pi f18, f24, f24\n"
			"xori x10, x26, 0x18\n"
			"fcvt.pw.ps f24, f2, rne\n"
			"mul x15, x24, x15\n"
			"bne x4, x11, 1f\n"
			"1:\n"
			"fmul.pi f14, f7, f1\n"
			"bltu x1, x29, 1f\n"
			"1:\n"
			"addi x10, x17, 0x1f\n"
			"fandi.pi f24, f17, 0xc5\n"
			"bltu x23, x20, 1f\n"
			"1:\n"
			"fnmsub.s f13, f17, f15, f1, rdn\n"
			"fnot.pi f7, f18\n"
			"fpackrepb.pi f2, f1\n"
			"ori x10, x10, 0x17\n"
			"fsll.pi f15, f15, f22\n"
			"bltu x17, x8, 1f\n"
			"1:\n"
			"fcvt.ps.pw f20, f18, rdn\n"
			"fmsub.s f19, f19, f1, f26, rdn\n"
			"bge x5, x24, 1f\n"
			"1:\n"
			"fand.pi f22, f2, f13\n"
			"fexp.ps f17, f22\n"
			"fsll.pi f20, f18, f20\n"
			"fsub.ps f19, f17, f26, rtz\n"
			"fnot.pi f17, f18\n"
			"fexp.ps f0, f17\n"
			"bgeu x15, x12, 1f\n"
			"1:\n"
			"fcmov.ps f21, f7, f18, f21\n"
			"fsat8.pi f20, f1\n"
			"packb x29, x18, x18\n"
			"fsgnjx.ps f22, f15, f15\n"
			"fslli.pi f26, f20, 0x5\n"
			"fnmadd.ps f13, f22, f1, f11, rtz\n"
			"mul x11, x24, x11\n"
			"bltu x16, x29, 1f\n"
			"1:\n"
			"fmulhu.pi f26, f26, f11\n"
			"bltu x0, x3, 1f\n"
			"1:\n"
			"mulhsu x18, x6, x18\n"
			"xor x6, x26, x18\n"
			"bge x22, x21, 1f\n"
			"1:\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_3_LOOP_SEQ_HID_10\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"la x31, check_sum_hid_10\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"LBL_POSTAMBLE_HID_10:\n"          
			"csrwi   tensor_wait, 0x0\n"
			"csrwi   tensor_wait, 0x1\n"
			"csrwi   tensor_wait, 0x2\n"
			"csrwi   tensor_wait, 0x3\n"
			"csrwi   tensor_wait, 0x4\n"
			"csrwi   tensor_wait, 0x5\n"
			"csrwi   tensor_wait, 0x6\n"
			"csrwi   tensor_wait, 0x7\n"
			"csrwi   tensor_wait, 0x8\n"
			"csrwi   tensor_wait, 0x9\n"
			"csrwi   tensor_wait, 0xa\n"
			"fence\n"
			"LBL_SYNC_POINT_7_HID_10:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_7_WAIT_HID_10\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_7_EXIT_HID_10\n"
			"LBL_SYNC_POINT_7_WAIT_HID_10:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_7_EXIT_HID_10:\n"          
			"nop\n"
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_10\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_10:\n"          
			".global LBL_FAIL_HID_10\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_10\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_10:\n"          
			".global LBL_PASS_HID_10\n"          
			VSNIP_RSV
		);
		c_test_pass();
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_10:\n"          
			".global LBL_HALT_HID_10\n"          
			"wfi\n"
			"j LBL_HALT_HID_10\n"          
			VSNIP_RSV
		);
		return 0;       
	};
	if ( hid == 11 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_11:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_11\n"
			"csrw mscratch, x5\n"
			"csrwi menable_shadows, 0xf\n"
			"li x5, -1\n"
			"csrw mcounteren, x5\n"
			"csrw scounteren, x5\n"
			"csrwi mhpmcounter3, 0\n"
			"csrwi mhpmcounter4, 0\n"
			"csrwi mhpmcounter5, 0\n"
			"csrwi mhpmcounter6, 0\n"
			"csrwi mhpmcounter7, 0\n"
			"csrwi mhpmcounter8, 0\n"
			"csrwi mhpmevent3, 7\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 9\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 23\n"   
			"mov.m.x m0, x0, 0xff\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f5,  288 (x5)\n"
			"flw.ps f31,  416 (x5)\n"
			"flw.ps f3,  160 (x5)\n"
			"flw.ps f13,  192 (x5)\n"
			"flw.ps f0,  928 (x5)\n"
			"flw.ps f19,  896 (x5)\n"
			"flw.ps f26,  320 (x5)\n"
			"flw.ps f21,  0 (x5)\n"
			"flw.ps f9,  448 (x5)\n"
			"flw.ps f7,  512 (x5)\n"
			"flw.ps f10,  608 (x5)\n"
			"flw.ps f15,  32 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f28,  544 (x5)\n"
			"flw.ps f27,  704 (x5)\n"
			"flw.ps f30,  128 (x5)\n"
			"flw.ps f11,  992 (x5)\n"
			"flw.ps f4,  800 (x5)\n"
			"flw.ps f20,  96 (x5)\n"
			"flw.ps f1,  736 (x5)\n"
			"flw.ps f18,  832 (x5)\n"
			"flw.ps f14,  768 (x5)\n"
			"flw.ps f6,  256 (x5)\n"
			"flw.ps f2,  384 (x5)\n"
			"flw.ps f23,  224 (x5)\n"
			"flw.ps f25,  64 (x5)\n"
			"flw.ps f22,  352 (x5)\n"
			"flw.ps f29,  480 (x5)\n"
			"flw.ps f12,  672 (x5)\n"
			"flw.ps f17,  576 (x5)\n"
			"flw.ps f16,  640 (x5)\n"
			"flw.ps f24,  960 (x5)\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_HID_11:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_0_WAIT_HID_11\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_0_EXIT_HID_11\n"
			"LBL_SYNC_POINT_0_WAIT_HID_11:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_EXIT_HID_11:\n"          
			"nop\n"
			"LBL_KERNEL_HID_11:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_11\n"
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_HID_11:\n"
			"li x29, 0x806eaf70d0\n"
			"fsw.ps f3, 128(x29)\n"
			"fbc.ps f12, 128(x29)\n" 
			"or x19, x19, x9\n"
			"beq x9, x25, 1f\n"
			"1:\n"
			"fmax.s f6, f8, f23\n"
			"fmvs.x.ps x29, f3, 1\n"
			"fsgnjx.s f21, f12, f17\n"
			"divw x24, x9, x24\n"
			"fmax.pi f3, f3, f23\n"
			"fnot.pi f20, f20\n"
			"bne x9, x14, 1f\n"
			"1:\n"
			"fcvt.ps.pw f31, f17, rmm\n"
			"fcvt.ps.pw f23, f20, rdn\n"
			"auipc x11, 0x9\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 352(x20)\n"
			"fcvt.pwu.ps f22, f25, rdn\n" 
			"fadd.s f22, f31, f22, rne\n"
			"fcvt.ps.f16 f3, f17\n"
			"fmin.pi f15, f15, f20\n"
			"fsgnjn.s f15, f22, f15\n"
			"bne x1, x31, 1f\n"
			"1:\n"
			"fsgnjn.ps f17, f17, f16\n"
			"fcvt.pw.ps f21, f22, rmm\n"
			"frcp.ps f8, f31\n"
			"beq x30, x28, 1f\n"
			"1:\n"
			"fsatu8.pi f25, f31\n"
			"fexp.ps f12, f12\n"
			"fcmovm.ps f12, f8, f17\n"
			"li x6, 0x809f70a008\n"
			"amominug.d x6, x20, (x6)\n" 
			"fmin.pi f6, f6, f16\n"
			"fmadd.ps f17, f16, f14, f9, rne\n"
			"fle.pi f10, f21, f25\n"
			"fmax.ps f9, f31, f31\n"
			"bltu x27, x26, 1f\n"
			"1:\n"
			"feq.ps f31, f23, f6\n"
			"fmulhu.pi f14, f9, f14\n"
			"fround.ps f10, f21, rdn\n"
			"bne x21, x30, 1f\n"
			"1:\n"
			"ffrc.ps f21, f31\n"
			"bne x4, x11, 1f\n"
			"1:\n"
			"fandi.pi f25, f25, 0x14e\n"
			"for.pi f3, f6, f3\n"
			"li x9, 0x8099ecf000\n"
			"amominug.d x25, x11, (x9)\n" 
			"flog.ps f14, f14\n"
			"fsgnjn.s f8, f8, f14\n"
			"fsatu8.pi f14, f14\n"
			"fbci.ps f3, 0x689b5\n"
			"srlw x20, x17, x14\n"
			"beq x19, x2, 1f\n"
			"1:\n"
			"srliw x14, x14, 4\n"
			"sra x29, x17, x14\n"
			"bgeu x0, x13, 1f\n"
			"1:\n"
			"maskpopcz x20, m4\n"
			"feq.ps f10, f31, f14\n"
			"sraiw x6, x17, 5\n"
			"li x24, 0x8099699028\n"
			"amoorg.w x19, x29, (x24)\n" 
			"fand.pi f17, f17, f15\n"
			"faddi.pi f10, f9, 0xaf\n"
			"flt.pi f22, f10, f25\n"
			"fmax.pi f9, f12, f12\n"
			"fmax.pi f15, f31, f31\n"
			"fnmsub.s f9, f6, f21, f6, rup\n"
			"fmsub.ps f3, f20, f31, f3, rdn\n"
			"fexp.ps f8, f12\n"
			"fpackrepb.pi f9, f9\n"
			"fadd.s f9, f9, f22, rne\n"
			"csrr x0, mhartid\n"
			"auipc x11, 0x5\n"
			"fcvt.f16.ps f23, f12\n"
			"divw x9, x24, x19\n"
			"fence\n"
			"slt x25, x13, x24\n"
			"fcvt.ps.f16 f12, f17\n"
			"fbcx.ps f23, x24\n"
			"fsub.pi f23, f17, f12\n"
			"fltm.pi m4, f23, f23\n"
			"bgeu x6, x22, 1f\n"
			"1:\n"
			"fandi.pi f10, f23, 0x17e\n"
			"li x29, 0x806bbd8a03\n"
			"sh x17, 512(x29)\n" 
			"faddi.pi f20, f20, 0x18c\n"
			"fmulhu.pi f9, f12, f6\n"
			"fadd.s f21, f25, f20, rdn\n"
			"bge x28, x1, 1f\n"
			"1:\n"
			"fle.pi f10, f14, f12\n"
			"fle.pi f14, f14, f6\n"
			"fsgnjx.ps f17, f31, f14\n"
			"fle.pi f9, f3, f17\n"
			"fsatu8.pi f14, f31\n"
			"fmul.ps f6, f25, f6, rtz\n"
			"fcmovm.ps f15, f21, f15\n"
			"bne x9, x20, 1f\n"
			"1:\n"
			"flem.ps m6, f12, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"sraw x17, x14, x6\n"
			"bgeu x23, x25, 1f\n"
			"1:\n"
			"fmax.ps f15, f16, f23\n"
			"ffrc.ps f20, f16\n"
			"sll x22, x11, x22\n"
			"blt x15, x29, 1f\n"
			"1:\n"
			"fmul.ps f22, f20, f22, rmm\n"
			"bgeu x14, x4, 1f\n"
			"1:\n"
			"fle.ps f25, f15, f10\n"
			"ori x6, x25, 0x18\n"
			"andi x9, x24, 0x11\n"
			"blt x0, x0, 1f\n"
			"1:\n"
			"fnmsub.s f15, f17, f8, f10, rup\n"
			"bltu x27, x27, 1f\n"
			"1:\n"
			"fsatu8.pi f25, f15\n"
			"li x19, 0x80ac726030\n"
			"amomaxul.d x29, x9, (x19)\n" 
			"fmax.s f14, f20, f6\n"
			"flt.ps f20, f12, f15\n"
			"fnmadd.ps f16, f17, f14, f15, rup\n"
			"flog.ps f3, f8\n"
			"fle.ps f6, f6, f6\n"
			"fmulhu.pi f23, f8, f17\n"
			"fsgnjn.ps f20, f20, f16\n"
			"beq x14, x15, 1f\n"
			"1:\n"
			"fclass.ps f12, f10\n"
			"fcvt.ps.f16 f12, f8\n"
			"feq.ps f6, f16, f3\n"
			"li x9, 0x5c4ae42b508f1d71\n"
			"li x13, 0xa39da5f5178b10bc\n"
			"mul x11, x9, x13\n" 
			"ffrc.ps f3, f3\n"
			"fsgnjn.ps f25, f31, f22\n"
			"flt.ps f23, f6, f10\n"
			"fnot.pi f3, f23\n"
			"fnmadd.ps f23, f9, f23, f20, rtz\n"
			"fltu.pi f16, f21, f16\n"
			"fltu.pi f17, f15, f6\n"
			"fmadd.s f14, f25, f14, f16, rtz\n"
			"fsub.s f22, f22, f22, rdn\n"
			"fmadd.s f9, f10, f14, f25, rne\n"
			"li x13, 0x809d6f303c\n"
			"amominug.w x22, x29, (x13)\n" 
			"fmsub.ps f6, f6, f25, f6, rtz\n"
			"flt.pi f21, f23, f21\n"
			"faddi.pi f31, f21, 0x1dd\n"
			"slt x9, x13, x20\n"
			"bgeu x3, x10, 1f\n"
			"1:\n"
			"slliw x19, x19, 4\n"
			"fmul.pi f20, f31, f21\n"
			"beq x25, x9, 1f\n"
			"1:\n"
			"sllw x6, x13, x13\n"
			"fand.pi f20, f6, f31\n"
			"bne x3, x14, 1f\n"
			"1:\n"
			"srl x19, x13, x17\n"
			"fandi.pi f16, f17, 0x78\n"
			"feqm.ps m6, f20, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"srl x25, x25, x11\n"
			"fcvt.s.wu f9, x6, rmm\n"
			"bgeu x12, x11, 1f\n"
			"1:\n"
			"fadd.s f14, f9, f8, rdn\n"
			"fmsub.ps f6, f15, f17, f17, rtz\n"
			"fmadd.ps f17, f20, f25, f25, rmm\n"
			"mul x9, x19, x13\n"
			"srlw x19, x13, x14\n"
			"maskpopc x24, m5\n"
			"fsgnjx.ps f12, f6, f23\n"
			"andi x25, x22, 0x8\n"
			"li x6, 0x806d8b7288\n"
			"li x13, 1373486614\n"
			"fsc32h.ps f8, x19 (x6)\n" 
			"fcmov.ps f16, f16, f21, f14\n"
			"feq.ps f6, f9, f16\n"
			"beq x12, x12, 1f\n"
			"1:\n"
			"fmul.ps f14, f10, f16, rup\n"
			"fcvt.pwu.ps f10, f12, rtz\n"
			"fnot.pi f23, f23\n"
			"fsrai.pi f12, f12, 0xf\n"
			"fcvt.ps.f16 f6, f6\n"
			"fle.ps f9, f16, f22\n"
			"fsgnj.s f16, f21, f16\n"
			"fmadd.s f16, f10, f9, f16, rmm\n"
			"bgeu x26, x25, 1f\n"
			"1:\n"
			"li x19, 0x80aac9a020\n"
			"la x13, mem_ptr_offset\n"
			"flw.ps f12, 928(x13)\n"
			"famominul.pi f17, f12 (x19)\n" 
			"fadd.s f22, f9, f6, rmm\n"
			"fand.pi f22, f17, f31\n"
			"fsll.pi f6, f6, f15\n"
			"bne x22, x12, 1f\n"
			"1:\n"
			"flog.ps f31, f31\n"
			"fcmov.ps f8, f22, f23, f20\n"
			"fmadd.ps f25, f21, f9, f22, rmm\n"
			"fadd.pi f20, f25, f20\n"
			"fxor.pi f14, f22, f20\n"
			"fmul.ps f8, f3, f8, rdn\n"
			"fmul.ps f21, f12, f9, rtz\n"
			"li x31, 0x8068008c40\n"
			"li x9, 0x4\n"
			"add x9, x31, x9\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrwi tensor_wait, 6\n" 
			"fsgnjn.ps f22, f15, f15\n"
			"bgeu x6, x13, 1f\n"
			"1:\n"
			"fmadd.ps f31, f14, f31, f15, rtz\n"
			"bgeu x4, x0, 1f\n"
			"1:\n"
			"frcp.ps f3, f3\n"
			"fmulhu.pi f3, f3, f31\n"
			"bltu x26, x28, 1f\n"
			"1:\n"
			"faddi.pi f31, f23, 0x103\n"
			"fcmovm.ps f22, f22, f14\n"
			"fsgnj.ps f14, f14, f3\n"
			"fround.ps f3, f14, rup\n"
			"bge x18, x6, 1f\n"
			"1:\n"
			"fnmsub.ps f25, f3, f22, f3, rup\n"
			"fsra.pi f6, f31, f31\n"
			"li x9, 0x809c3ab038\n"
			"amoming.d x9, x25, (x9)\n" 
			"fmax.ps f23, f12, f23\n"
			"fmul.pi f6, f6, f12\n"
			"fclass.ps f8, f15\n"
			"fcvt.ps.pw f3, f16, rup\n"
			"fmul.ps f14, f6, f17, rmm\n"
			"fmsub.s f20, f21, f3, f20, rup\n"
			"fsll.pi f16, f10, f31\n"
			"fmsub.s f21, f15, f22, f9, rne\n"
			"fadd.pi f17, f10, f20\n"
			"fsgnjx.ps f8, f20, f25\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f12, 0(x31)\n"
			"flw.ps f14, 3(x31)\n"
			"flw.ps f15, 7(x31)\n"
			"flw.ps f16, 0(x31)\n"
			"flw.ps f17, 8(x31)\n"
			"flw.ps f21, 10(x31)\n"
			"flw.ps f22, 23(x31)\n"
			"flw.ps f3, 17(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_11:\n"
			"fswizz.ps f12, f12, 0x15\n"
			"bne x22, x26, 1f\n"
			"1:\n"
			"fcvt.pw.ps f9, f3, rdn\n"
			"fpackrepb.pi f15, f15\n"
			"fsgnjn.s f12, f3, f3\n"
			"fsgnjn.s f17, f14, f17\n"
			"fcvt.ps.pw f16, f16, rtz\n"
			"bltu x1, x19, 1f\n"
			"1:\n"
			"fsub.s f10, f3, f14, rne\n"
			"fsgnjx.ps f25, f14, f22\n"
			"fsgnjx.ps f21, f17, f21\n"
			"bltu x8, x30, 1f\n"
			"1:\n"
			"faddi.pi f21, f3, 0xf8\n"
			"bgeu x27, x21, 1f\n"
			"1:\n"
			"fsll.pi f12, f12, f17\n"
			"fandi.pi f22, f21, 0x13f\n"
			"fnmsub.ps f17, f15, f17, f16, rtz\n"
			"fadd.s f14, f16, f21, rmm\n"
			"fandi.pi f25, f12, 0x12a\n"
			"fmulh.pi f15, f15, f12\n"
			"fadd.pi f3, f12, f21\n"
			"bgeu x28, x5, 1f\n"
			"1:\n"
			"fnmadd.s f22, f22, f15, f3, rtz\n"
			"fcvt.pw.ps f3, f3, rmm\n"
			"fadd.ps f31, f12, f12, rmm\n"
			"fnmadd.ps f14, f14, f17, f16, rtz\n"
			"fmadd.ps f16, f3, f22, f16, rdn\n"
			"fsub.s f15, f15, f22, rtz\n"
			"fmax.ps f31, f21, f16\n"
			"fsrai.pi f22, f22, 0x7\n"
			"frcp.ps f21, f21\n"
			"fmadd.ps f10, f21, f22, f15, rdn\n"
			"beq x8, x11, 1f\n"
			"1:\n"
			"fmul.ps f21, f21, f14, rmm\n"
			"fsrl.pi f16, f16, f16\n"
			"fmul.pi f6, f14, f21\n"
			"beq x29, x8, 1f\n"
			"1:\n"
			"fnmadd.ps f17, f17, f17, f17, rdn\n"
			"fsat8.pi f12, f16\n"
			"fmulhu.pi f25, f21, f22\n"
			"fmaxu.pi f16, f22, f17\n"
			"fmadd.s f17, f17, f3, f21, rtz\n"
			"fnmsub.ps f15, f16, f12, f16, rmm\n"
			"faddi.pi f31, f21, 0x199\n"
			"faddi.pi f16, f17, 0xba\n"
			"fnmsub.ps f23, f16, f17, f3, rup\n"
			"fcvt.ps.pw f15, f12, rup\n"
			"blt x14, x14, 1f\n"
			"1:\n"
			"fmax.pi f17, f17, f3\n"
			"fminu.pi f16, f16, f22\n"
			"fand.pi f16, f17, f22\n"
			"fmulhu.pi f21, f15, f3\n"
			"bltu x15, x7, 1f\n"
			"1:\n"
			"frcp.ps f6, f22\n"
			"fcmovm.ps f10, f22, f16\n"
			"fsll.pi f21, f15, f16\n"
			"fmsub.ps f12, f21, f17, f12, rne\n"
			"fexp.ps f21, f22\n"
			"fsgnjx.ps f3, f3, f22\n"
			"fsgnjx.ps f10, f12, f14\n"
			"fsll.pi f9, f16, f12\n"
			"fmin.ps f17, f22, f17\n"
			"bne x1, x15, 1f\n"
			"1:\n"
			"fadd.s f3, f22, f17, rne\n"
			"fmax.pi f17, f21, f17\n"
			"bge x26, x13, 1f\n"
			"1:\n"
			"fsrli.pi f21, f21, 0x1\n"
			"fmul.pi f15, f14, f15\n"
			"blt x26, x25, 1f\n"
			"1:\n"
			"fmul.ps f31, f21, f21, rmm\n"
			"fle.pi f10, f21, f15\n"
			"fswizz.ps f22, f15, 0x1f\n"
			"fadd.ps f9, f16, f21, rup\n"
			"fsll.pi f14, f3, f22\n"
			"fmulhu.pi f3, f12, f15\n"
			"fmax.ps f17, f12, f17\n"
			"fnot.pi f31, f12\n"
			"fmaxu.pi f17, f14, f17\n"
			"fcvt.pwu.ps f16, f16, rdn\n"
			"fsgnjx.s f3, f16, f3\n"
			"fmsub.s f23, f15, f16, f21, rne\n"
			"fsub.ps f25, f17, f22, rmm\n"
			"fnmadd.s f3, f17, f12, f14, rtz\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_0_LOOP_SEQ_HID_11\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_HID_11:\n"
			"masknot m4, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"sltiu x13, x22, 0x19\n"
			"remu x11, x14, x19\n"
			"fround.ps f20, f22, rmm\n"
			"fclass.ps f15, f17\n"
			"fsgnjx.s f14, f6, f9\n"
			"ffrc.ps f20, f10\n"
			"flt.s x11, f6, f31\n"
			"fcvt.pwu.ps f25, f21, rdn\n"
			"li x20, 0xcda1d82e148c82e\n"
			"xori x20, x20, 0xd\n" 
			"feq.s x19, f8, f8\n"
			"bgeu x6, x25, 1f\n"
			"1:\n"
			"fmul.ps f21, f25, f12, rmm\n"
			"feq.ps f8, f17, f6\n"
			"xor x20, x19, x19\n"
			"addiw x29, x22, 0x12\n"
			"fmul.ps f12, f6, f12, rmm\n"
			"slliw x25, x19, 2\n"
			"blt x1, x7, 1f\n"
			"1:\n"
			"fltu.pi f12, f6, f17\n"
			"li x31, 0x80300062c0\n"
			"li x24, 0x800000000000008\n"
			"add x24, x31, x24\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrwi tensor_wait, 6\n" 
			"fcmovm.ps f21, f8, f15\n"
			"bltu x22, x8, 1f\n"
			"1:\n"
			"flog.ps f31, f31\n"
			"blt x8, x9, 1f\n"
			"1:\n"
			"fclass.ps f31, f10\n"
			"fsat8.pi f22, f15\n"
			"fsub.pi f3, f8, f14\n"
			"fsrai.pi f10, f10, 0x2\n"
			"fandi.pi f22, f14, 0xcd\n"
			"fmin.s f10, f16, f31\n"
			"bltu x30, x12, 1f\n"
			"1:\n"
			"li x6, 0x806b79aa30\n"
			"la x24, mem_ptr_offset\n"
			"flw.ps f9, 512(x24)\n"
			"fscw.ps f16, f9 (x6)\n" 
			"fmin.ps f6, f25, f15\n"
			"blt x16, x11, 1f\n"
			"1:\n"
			"frcp.ps f14, f23\n"
			"fcvt.ps.pw f15, f8, rne\n"
			"fsra.pi f8, f6, f8\n"
			"fsgnjn.ps f25, f8, f25\n"
			"fcvt.ps.pw f3, f12, rdn\n"
			"fminu.pi f3, f23, f23\n"
			"fcvt.ps.pwu f8, f8, rmm\n"
			"li x13, 0x8098908038\n"
			"la x14, mem_ptr_offset\n"
			"flw.ps f21, 736(x14)\n"
			"famoxorg.pi f6, f21 (x13)\n" 
			"frcp.ps f22, f21\n"
			"fpackreph.pi f15, f16\n"
			"fcvt.pw.ps f9, f16, rtz\n"
			"fmulhu.pi f9, f17, f22\n"
			"fsgnjn.ps f3, f17, f21\n"
			"fslli.pi f16, f16, 0xd\n"
			"feq.ps f22, f14, f15\n"
			"bge x26, x3, 1f\n"
			"1:\n"
			"fmulhu.pi f6, f25, f15\n"
			"la x20, rand_int_32\n"
			"flw.ps f31, 992(x20)\n"
			"flw.ps f12, 832(x20)\n"
			"fmin.pi f14, f31, f12\n" 
			"for.pi f8, f8, f15\n"
			"fmax.ps f16, f31, f10\n"
			"sllw x29, x29, x29\n"
			"fcmovm.ps f21, f8, f3\n"
			"fsgnj.s f9, f23, f15\n"
			"fcvt.pwu.ps f25, f10, rdn\n"
			"fpackrepb.pi f20, f31\n"
			"beq x26, x8, 1f\n"
			"1:\n"
			"slli x22, x20, 4\n"
			"li x24, 0x80685490a0\n"
			"li x9, 3504371229\n"
			"fsc32w.ps f12, x19 (x24)\n" 
			"fmin.s f21, f20, f22\n"
			"fmadd.s f8, f16, f8, f20, rmm\n"
			"fclass.ps f9, f15\n"
			"fmax.s f17, f8, f31\n"
			"bge x15, x1, 1f\n"
			"1:\n"
			"fsgnjx.ps f21, f14, f3\n"
			"fround.ps f9, f22, rdn\n"
			"fmax.s f20, f6, f9\n"
			"fmax.ps f17, f10, f8\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f15, 672(x6)\n"
			"fround.ps f31, f15, rmm\n" 
			"fnmsub.ps f14, f17, f14, f25, rdn\n"
			"fsll.pi f9, f8, f15\n"
			"bgeu x25, x0, 1f\n"
			"1:\n"
			"fsrai.pi f21, f22, 0x9\n"
			"fsatu8.pi f17, f3\n"
			"fsgnjx.ps f21, f10, f25\n"
			"fsra.pi f20, f20, f23\n"
			"fmsub.ps f8, f16, f20, f16, rtz\n"
			"fnmsub.ps f6, f9, f6, f20, rdn\n"
			"li x13, 0xc90c3199d393b1ef\n"
			"li x25, 0x53e676b4dd6a3b5e\n"
			"mulh x17, x13, x25\n" 
			"fround.ps f25, f6, rmm\n"
			"bltu x17, x21, 1f\n"
			"1:\n"
			"fmax.pi f3, f6, f16\n"
			"fcmovm.ps f21, f10, f15\n"
			"for.pi f31, f17, f31\n"
			"for.pi f10, f15, f31\n"
			"blt x12, x9, 1f\n"
			"1:\n"
			"or x17, x25, x25\n"
			"fsrli.pi f17, f10, 0x1\n"
			"masknot m7, m5\n"
			"bne x0, x4, 1f\n"
			"1:\n"
			"li x29, 0x80688ba2e8\n"
			"la x11, mem_ptr_offset\n"
			"flw.ps f31, 160(x11)\n"
			"fsch.ps f10, f31 (x29)\n" 
			"fmin.s f17, f12, f17\n"
			"fmin.s f21, f23, f17\n"
			"fcmov.ps f17, f15, f17, f12\n"
			"feq.pi f21, f21, f14\n"
			"blt x0, x19, 1f\n"
			"1:\n"
			"fsatu8.pi f8, f31\n"
			"fnmadd.ps f17, f9, f9, f12, rtz\n"
			"fcvt.ps.pwu f25, f9, rne\n"
			"fnmsub.s f22, f31, f22, f3, rne\n"
			"li x11, 0x80af8a5028\n"
			"la x25, mem_ptr_offset\n"
			"flw.ps f3, 864(x25)\n"
			"famominl.pi f8, f3 (x11)\n" 
			"fsgnjn.ps f16, f16, f9\n"
			"fadd.s f31, f3, f31, rne\n"
			"fnmadd.s f14, f14, f9, f20, rmm\n"
			"fminu.pi f20, f16, f23\n"
			"fltu.pi f16, f3, f21\n"
			"bne x28, x21, 1f\n"
			"1:\n"
			"fsra.pi f25, f22, f15\n"
			"fadd.s f25, f16, f3, rup\n"
			"fround.ps f15, f25, rup\n"
			"li x9, 0x809939f000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f23, (x9)\n"
			"flwg.ps f14, (x9)\n" 
			"fnmsub.s f22, f22, f8, f22, rup\n"
			"fsgnj.s f8, f8, f22\n"
			"fminu.pi f15, f15, f15\n"
			"fnmadd.ps f8, f8, f22, f22, rup\n"
			"fcvt.pwu.ps f16, f8, rne\n"
			"fmulhu.pi f15, f8, f15\n"
			"fsub.s f16, f15, f15, rup\n"
			"bgeu x14, x12, 1f\n"
			"1:\n"
			"fadd.pi f22, f22, f15\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f17, 512(x17)\n"
			"fcvt.f16.ps f14, f17\n" 
			"fclass.ps f23, f6\n"
			"fmadd.s f14, f23, f10, f9, rtz\n"
			"bge x1, x25, 1f\n"
			"1:\n"
			"fcmovm.ps f9, f16, f3\n"
			"faddi.pi f15, f10, 0x18f\n"
			"fcvt.ps.pwu f22, f23, rtz\n"
			"fcvt.ps.pwu f22, f25, rmm\n"
			"fswizz.ps f16, f15, 0x1a\n"
			"fadd.s f21, f8, f3, rdn\n"
			"blt x30, x26, 1f\n"
			"1:\n"
			"fence\n" 
			"fsgnjn.s f12, f15, f17\n"
			"flog.ps f21, f20\n"
			"fsra.pi f23, f25, f25\n"
			"fcvt.ps.pwu f9, f8, rup\n"
			"fadd.s f25, f25, f9, rup\n"
			"fnot.pi f17, f6\n"
			"beq x24, x23, 1f\n"
			"1:\n"
			"fswizz.ps f16, f20, 0x1b\n"
			"fmadd.ps f20, f20, f20, f20, rne\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f9, 544(x13)\n"
			"fmvs.x.ps x22, f9, 3\n" 
			"fsrli.pi f20, f9, 0x0\n"
			"fmulhu.pi f20, f16, f15\n"
			"fandi.pi f9, f9, 0x9a\n"
			"fpackrepb.pi f16, f15\n"
			"bne x10, x18, 1f\n"
			"1:\n"
			"fpackrepb.pi f8, f22\n"
			"ffrc.ps f16, f16\n"
			"fclass.ps f8, f15\n"
			"fsrl.pi f22, f16, f22\n"
			"li x14, 0x80aa239038\n"
			"la x19, mem_ptr_offset\n"
			"flw.ps f14, 192(x19)\n"
			"famomaxl.ps f3, f14 (x14)\n" 
			"fslli.pi f12, f6, 0xb\n"
			"fmax.pi f22, f6, f22\n"
			"fmul.s f6, f16, f8, rne\n"
			"fmax.s f21, f10, f10\n"
			"fsgnjn.s f22, f22, f16\n"
			"fmul.s f6, f14, f10, rmm\n"
			"fmax.pi f20, f12, f20\n"
			"fmaxu.pi f3, f17, f31\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x10\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f12, 0(x31)\n"
			"flw.ps f16, 2(x31)\n"
			"flw.ps f20, 3(x31)\n"
			"flw.ps f23, 5(x31)\n"
			"flw.ps f25, 4(x31)\n"
			"flw.ps f3, 12(x31)\n"
			"flw.ps f31, 22(x31)\n"
			"flw.ps f6, 4(x31)\n"
			"flw.ps f8, 16(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_11:\n"
			"fmaxu.pi f25, f25, f25\n"
			"fminu.pi f3, f16, f3\n"
			"fsgnj.ps f14, f20, f3\n"
			"fcvt.pwu.ps f14, f6, rtz\n"
			"fclass.ps f12, f12\n"
			"fcvt.pwu.ps f12, f6, rdn\n"
			"frcp.ps f8, f23\n"
			"fcvt.ps.pwu f25, f31, rup\n"
			"beq x5, x24, 1f\n"
			"1:\n"
			"fsgnj.ps f3, f8, f20\n"
			"fswizz.ps f16, f16, 0xe\n"
			"fcmovm.ps f20, f20, f31\n"
			"fsat8.pi f12, f16\n"
			"beq x28, x22, 1f\n"
			"1:\n"
			"fmul.pi f12, f12, f25\n"
			"fand.pi f25, f8, f25\n"
			"fsgnj.s f17, f12, f23\n"
			"bltu x30, x10, 1f\n"
			"1:\n"
			"fcmovm.ps f31, f25, f31\n"
			"fpackreph.pi f12, f3\n"
			"bgeu x10, x16, 1f\n"
			"1:\n"
			"for.pi f23, f25, f8\n"
			"fmin.pi f31, f20, f16\n"
			"fsat8.pi f17, f12\n"
			"bltu x13, x17, 1f\n"
			"1:\n"
			"flt.ps f21, f12, f20\n"
			"fslli.pi f21, f20, 0x9\n"
			"fmul.pi f12, f8, f20\n"
			"fcmov.ps f21, f16, f8, f8\n"
			"fadd.ps f21, f3, f3, rdn\n"
			"bgeu x26, x27, 1f\n"
			"1:\n"
			"fnmsub.ps f17, f3, f12, f23, rmm\n"
			"bgeu x31, x24, 1f\n"
			"1:\n"
			"flt.pi f20, f20, f6\n"
			"fpackrepb.pi f23, f16\n"
			"beq x11, x24, 1f\n"
			"1:\n"
			"fnmadd.s f20, f8, f20, f6, rup\n"
			"bltu x27, x6, 1f\n"
			"1:\n"
			"fnot.pi f12, f16\n"
			"fnmadd.ps f23, f23, f12, f6, rne\n"
			"fminu.pi f3, f3, f3\n"
			"fmsub.s f12, f8, f3, f6, rup\n"
			"fsgnjx.s f17, f8, f23\n"
			"fmsub.ps f25, f31, f23, f16, rmm\n"
			"fsgnjx.ps f20, f25, f25\n"
			"fmin.pi f31, f31, f20\n"
			"fadd.s f31, f8, f31, rdn\n"
			"fadd.ps f25, f8, f3, rtz\n"
			"fmin.pi f17, f23, f31\n"
			"fpackreph.pi f8, f3\n"
			"fsgnj.s f25, f25, f3\n"
			"fmul.ps f8, f31, f8, rmm\n"
			"fmsub.s f20, f23, f20, f16, rdn\n"
			"bltu x7, x13, 1f\n"
			"1:\n"
			"fsll.pi f3, f16, f12\n"
			"fsgnj.s f25, f23, f25\n"
			"flog.ps f12, f3\n"
			"fcvt.pwu.ps f31, f20, rup\n"
			"fltu.pi f23, f23, f6\n"
			"fnmsub.s f31, f31, f8, f6, rup\n"
			"bltu x12, x17, 1f\n"
			"1:\n"
			"fadd.pi f25, f16, f23\n"
			"beq x8, x11, 1f\n"
			"1:\n"
			"fnot.pi f12, f25\n"
			"for.pi f20, f25, f23\n"
			"fnot.pi f6, f31\n"
			"fmin.ps f20, f20, f12\n"
			"fadd.pi f3, f3, f31\n"
			"fnot.pi f20, f16\n"
			"fpackreph.pi f3, f16\n"
			"fand.pi f15, f16, f3\n"
			"for.pi f17, f25, f31\n"
			"fsgnjx.s f12, f12, f3\n"
			"bge x2, x13, 1f\n"
			"1:\n"
			"fsgnjn.ps f16, f16, f20\n"
			"bgeu x26, x17, 1f\n"
			"1:\n"
			"fexp.ps f25, f25\n"
			"fnmadd.s f15, f16, f12, f12, rtz\n"
			"bgeu x28, x18, 1f\n"
			"1:\n"
			"fand.pi f20, f3, f6\n"
			"fmax.s f12, f3, f12\n"
			"fmin.ps f12, f3, f25\n"
			"fsgnj.s f12, f20, f12\n"
			"fsrli.pi f17, f20, 0x6\n"
			"blt x23, x26, 1f\n"
			"1:\n"
			"fcvt.ps.pw f21, f12, rtz\n"
			"fmadd.s f25, f23, f16, f25, rdn\n"
			"fsgnjn.s f8, f8, f23\n"
			"bge x27, x25, 1f\n"
			"1:\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_1_LOOP_SEQ_HID_11\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_HID_11:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 224(x19)\n"
			"flw.ps f22, 512(x19)\n"
			"fsub.ps f9, f21, f22, rup\n" 
			"li x14, 0x80995f2000\n"
			"amoswapg.d x9, x22, (x14)\n" 
			"li x31, 0x80300062c0\n"
			"li x17, 0x800000000000004\n"
			"add x17, x31, x17\n"
			"li x31, (0x1 << 6) | 0x0\n"
			"csrwi tensor_wait, 6\n" 
			"li x19, 0x80adab4000\n"
			"la x17, mem_ptr_offset\n"
			"flw.ps f25, 448(x17)\n"
			"famominl.pi f10, f25 (x19)\n" 
			"li x29, 0x982de7027a85efa6\n"
			"slti x29, x29, 0xd\n" 
			"li x22, 0x806a3557ba\n"
			"sd x25, 512(x22)\n"
			"lbu x19, 512(x22)\n" 
			"li x6, 0x80acec5020\n"
			"amoxorl.w x9, x6, (x6)\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f22, 384(x25)\n"
			"flw.ps f25, 704(x25)\n"
			"feq.s x22, f22, f25\n" 
			"li x22, 0x80993c1018\n"
			"la x24, mem_ptr_offset\n"
			"flw.ps f12, 512(x24)\n"
			"fgbg.ps f8, f12 (x22)\n" 
			"la x6, rand_int_32\n"
			"flw.ps f3, 96(x6)\n"
			"flw.ps f10, 896(x6)\n"
			"fdivu.pi f12, f3, f10\n" 
			"li x24, 0x809b6ce008\n"
			"amoorg.d x6, x11, (x24)\n" 
			"li x25, 0x80ac329028\n"
			"amominul.w x6, x20, (x25)\n" 
			"li x29, 0x80ad386028\n"
			"la x19, mem_ptr_offset\n"
			"flw.ps f9, 128(x19)\n"
			"famoswapl.pi f8, f9 (x29)\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 800(x17)\n"
			"fclass.s x17, f9\n" 
			"li x9, 0x8098454038\n"
			"la x24, mem_ptr_offset\n"
			"flw.ps f8, 544(x24)\n"
			"fscbg.ps f6, f8 (x9)\n" 
			"li x6, 0x809b1ac018\n"
			"la x22, mem_ptr_offset\n"
			"flw.ps f25, 64(x22)\n"
			"famoandg.pi f12, f25 (x6)\n" 
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x18\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f14, 4(x31)\n"
			"flw.ps f20, 1(x31)\n"
			"flw.ps f22, 1(x31)\n"
			"flw.ps f23, 10(x31)\n"
			"flw.ps f25, 1(x31)\n"
			"flw.ps f3, 24(x31)\n"
			"flw.ps f31, 4(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x11, 0(x31)\n"
			"lw x13, 2(x31)\n"
			"lw x14, 6(x31)\n"
			"lw x17, 4(x31)\n"
			"lw x20, 13(x31)\n"
			"lw x22, 11(x31)\n"
			"lw x24, 4(x31)\n"
			"lw x25, 3(x31)\n"
			"lw x29, 3(x31)\n"
			"lw x6, 32(x31)\n"
			"lw x9, 40(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_11:\n"
			"fminu.pi f31, f31, f20\n"
			"remuw x22, x20, x22\n"
			"mova.x.m x22\n"
			"bne x2, x5, 1f\n"
			"1:\n"
			"fmul.pi f6, f25, f25\n"
			"addiw x17, x22, 0x3\n"
			"sra x17, x25, x17\n"
			"fcvt.wu.s x29, f22, rup\n"
			"fclass.s x29, f3\n"
			"fcvt.ps.pw f12, f20, rmm\n"
			"mul x22, x14, x13\n"
			"fsub.s f3, f14, f3, rtz\n"
			"srli x24, x25, 0\n"
			"beq x15, x31, 1f\n"
			"1:\n"
			"mul x14, x14, x29\n"
			"srliw x19, x6, 2\n"
			"feqm.ps m5, f3, f25\n"
			"bltu x24, x11, 1f\n"
			"1:\n"
			"ffrc.ps f3, f3\n"
			"mova.m.x x13\n"
			"fltm.ps m5, f20, f10\n"
			"and x13, x22, x9\n"
			"maskpopcz x25, m7\n"
			"mova.m.x x22\n"
			"fsra.pi f31, f20, f14\n"
			"flog.ps f25, f25\n"
			"blt x16, x4, 1f\n"
			"1:\n"
			"remu x11, x11, x11\n"
			"fnmadd.ps f31, f3, f3, f23, rup\n"
			"bne x31, x18, 1f\n"
			"1:\n"
			"fcvt.w.s x17, f25, rdn\n"
			"fnmsub.s f6, f10, f22, f3, rdn\n"
			"add x13, x25, x17\n"
			"fbcx.ps f3, x24\n"
			"fadd.s f10, f31, f31, rne\n"
			"packb x25, x9, x6\n"
			"fltm.ps m6, f25, f31\n"
			"feq.s x20, f10, f22\n"
			"sraiw x29, x14, 3\n"
			"fmsub.ps f12, f22, f22, f3, rmm\n"
			"fmax.ps f31, f14, f3\n"
			"bltu x31, x16, 1f\n"
			"1:\n"
			"fmin.ps f21, f20, f22\n"
			"bge x26, x2, 1f\n"
			"1:\n"
			"fsatu8.pi f22, f14\n"
			"beq x4, x31, 1f\n"
			"1:\n"
			"fmin.ps f14, f23, f14\n"
			"fle.ps f23, f20, f31\n"
			"divuw x29, x24, x11\n"
			"fcvt.w.s x14, f31, rup\n"
			"bne x27, x8, 1f\n"
			"1:\n"
			"subw x9, x9, x17\n"
			"feq.pi f6, f23, f23\n"
			"fle.s x22, f10, f20\n"
			"bne x17, x10, 1f\n"
			"1:\n"
			"ffrc.ps f20, f25\n"
			"fand.pi f20, f20, f20\n"
			"fsat8.pi f25, f20\n"
			"ori x22, x22, 0xb\n"
			"maskand m5, m5, m4\n"
			"fle.ps f25, f25, f14\n"
			"fltm.ps m4, f10, f3\n"
			"fswizz.ps f23, f23, 0x1b\n"
			"feq.s x11, f20, f22\n"
			"beq x0, x13, 1f\n"
			"1:\n"
			"fsat8.pi f31, f20\n"
			"bne x4, x25, 1f\n"
			"1:\n"
			"maskand m4, m5, m5\n"
			"or x13, x29, x13\n"
			"srliw x20, x13, 1\n"
			"fence\n"
			"xor x11, x11, x13\n"
			"fcmov.ps f23, f31, f23, f23\n"
			"maskpopcz x20, m6\n"
			"fclass.ps f10, f3\n"
			"flog.ps f14, f25\n"
			"fcvt.pwu.ps f14, f31, rtz\n"
			"fcvt.ps.pw f10, f22, rne\n"
			"beq x14, x5, 1f\n"
			"1:\n"
			"faddi.pi f12, f25, 0x115\n"
			"ffrc.ps f31, f31\n"
			"andi x29, x29, 0x0\n"
			"blt x24, x2, 1f\n"
			"1:\n"
			"mul x24, x24, x20\n"
			"fcvt.w.s x13, f10, rne\n"
			"blt x23, x27, 1f\n"
			"1:\n"
			"flt.ps f10, f10, f23\n"
			"fsgnjn.s f15, f25, f23\n"
			"fcvt.ps.pwu f16, f25, rmm\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_2_LOOP_SEQ_HID_11\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f5,  640 (x5)\n"
			"flw.ps f23,  992 (x5)\n"
			"flw.ps f0,  864 (x5)\n"
			"flw.ps f28,  288 (x5)\n"
			"flw.ps f1,  800 (x5)\n"
			"flw.ps f21,  448 (x5)\n"
			"flw.ps f7,  736 (x5)\n"
			"flw.ps f22,  96 (x5)\n"
			"flw.ps f8,  32 (x5)\n"
			"flw.ps f11,  416 (x5)\n"
			"flw.ps f6,  128 (x5)\n"
			"flw.ps f12,  192 (x5)\n"
			"flw.ps f10,  0 (x5)\n"
			"flw.ps f2,  256 (x5)\n"
			"flw.ps f15,  480 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
			"flw.ps f18,  832 (x5)\n"
			"flw.ps f9,  608 (x5)\n"
			"flw.ps f19,  704 (x5)\n"
			"flw.ps f25,  64 (x5)\n"
			"flw.ps f24,  928 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f26,  320 (x5)\n"
			"flw.ps f29,  160 (x5)\n"
			"flw.ps f3,  576 (x5)\n"
			"flw.ps f30,  224 (x5)\n"
			"flw.ps f16,  352 (x5)\n"
			"flw.ps f17,  384 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f13,  512 (x5)\n"
			"flw.ps f20,  896 (x5)\n"
			"flw.ps f31,  544 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_3_HID_11:\n"
			"li x25, 0x80ab686020\n"
			"la x13, mem_ptr_offset\n"
			"flw.ps f8, 96(x13)\n"
			"famomaxl.ps f31, f8 (x25)\n" 
			"fexp.ps f10, f23\n"
			"fsrai.pi f17, f10, 0x3\n"
			"fcmov.ps f20, f20, f15, f17\n"
			"fmul.pi f17, f17, f12\n"
			"frcp.ps f14, f10\n"
			"bgeu x13, x17, 1f\n"
			"1:\n"
			"fslli.pi f25, f10, 0xc\n"
			"fbci.ps f6, 0x66dd5\n" 
			"sllw x14, x20, x11\n"
			"fsgnjn.s f9, f25, f15\n"
			"bne x5, x3, 1f\n"
			"1:\n"
			"fsat8.pi f12, f15\n"
			"mulh x29, x29, x22\n"
			"mulh x17, x19, x24\n"
			"fmsub.s f14, f16, f9, f20, rup\n"
			"la x17, rand_int_32\n"
			"flw.ps f3, 672(x17)\n"
			"faddi.pi f10, f3, 0x7a\n" 
			"fmax.s f23, f23, f20\n"
			"bltu x17, x7, 1f\n"
			"1:\n"
			"slliw x6, x14, 0\n"
			"bge x15, x25, 1f\n"
			"1:\n"
			"packb x25, x25, x13\n"
			"bgeu x25, x31, 1f\n"
			"1:\n"
			"fmin.ps f8, f21, f8\n"
			"feq.pi f25, f8, f8\n"
			"beq x23, x4, 1f\n"
			"1:\n"
			"fltu.pi f23, f25, f23\n"
			"li x19, 0xb62b7446dc5c1bd2\n"
			"xori x9, x19, 0x1c\n" 
			"fnmsub.s f3, f10, f10, f8, rmm\n"
			"fmadd.ps f12, f20, f16, f22, rtz\n"
			"fmadd.s f8, f20, f9, f22, rup\n"
			"fmsub.ps f8, f22, f25, f8, rup\n"
			"divw x19, x19, x25\n"
			"bne x7, x29, 1f\n"
			"1:\n"
			"remw x25, x6, x6\n"
			"li x19, 0x809d85f038\n"
			"amoorg.d x9, x19, (x19)\n" 
			"fsetm.pi m6, f12\n"
			"packb x13, x17, x25\n"
			"mulh x9, x24, x14\n"
			"fmul.pi f23, f14, f12\n"
			"fexp.ps f12, f14\n"
			"remw x11, x9, x14\n"
			"bltu x6, x4, 1f\n"
			"1:\n"
			"li x19, 0x809cfc7038\n"
			"amominug.d x9, x11, (x19)\n" 
			"fsgnjn.ps f25, f17, f25\n"
			"blt x13, x6, 1f\n"
			"1:\n"
			"fsat8.pi f6, f17\n"
			"mulhsu x24, x17, x17\n"
			"bge x15, x4, 1f\n"
			"1:\n"
			"sub x6, x25, x6\n"
			"maskxor m5, m7, m5\n"
			"bltu x21, x1, 1f\n"
			"1:\n"
			"fandi.pi f6, f25, 0x58\n"
			"li x31, 0x8020002d80\n"
			"li x24, 0xc0000000000000a\n"
			"add x24, x31, x24\n"
			"li x31, (0x3 << 6) | 0x1\n"
			"csrwi tensor_wait, 6\n" 
			"divuw x13, x13, x19\n"
			"frcp.ps f23, f22\n"
			"auipc x20, 0x1e\n"
			"andi x13, x13, 0x16\n"
			"fsub.pi f15, f3, f16\n"
			"blt x2, x14, 1f\n"
			"1:\n"
			"fpackrepb.pi f10, f16\n"
			"bgeu x13, x19, 1f\n"
			"1:\n"
			"li x19, 0x80ac600028\n"
			"la x9, mem_ptr_offset\n"
			"flw.ps f17, 288(x9)\n"
			"famoswapl.pi f20, f17 (x19)\n" 
			"sltiu x14, x11, 0x8\n"
			"fcvt.w.s x19, f8, rup\n"
			"fmul.s f17, f23, f17, rup\n"
			"srl x19, x14, x11\n"
			"maskor m5, m6, m7\n"
			"remu x11, x13, x11\n"
			"li x14, 0x80acf08010\n"
			"amoswapl.d x13, x11, (x14)\n" 
			"fclass.ps f20, f17\n"
			"flt.s x22, f17, f23\n"
			"fnot.pi f6, f15\n"
			"bne x28, x9, 1f\n"
			"1:\n"
			"fmulhu.pi f10, f31, f6\n"
			"fmadd.s f15, f9, f15, f10, rne\n"
			"bne x13, x15, 1f\n"
			"1:\n"
			"fclass.ps f23, f9\n"
			"li x17, 0x8068998208\n"
			"li x6, 2783948163\n"
			"fg32h.ps f23, x13 (x17)\n" 
			"feq.ps f12, f21, f21\n"
			"beq x20, x2, 1f\n"
			"1:\n"
			"fmulh.pi f6, f6, f21\n"
			"flog.ps f10, f10\n"
			"fand.pi f12, f3, f16\n"
			"fcvt.ps.pw f23, f9, rtz\n"
			"fcvt.ps.f16 f20, f12\n"
			"bne x12, x3, 1f\n"
			"1:\n"
			"li x31, 0x80680023c0\n"
			"li x9, 0x8000000000000001\n"
			"add x9, x31, x9\n"
			"li x31, (0x2 << 6) | 0x1\n"
			"csrwi tensor_wait, 6\n" 
			"fcvt.ps.pw f8, f14, rne\n"
			"bltu x16, x20, 1f\n"
			"1:\n"
			"fsgnj.s f12, f9, f16\n"
			"bne x8, x12, 1f\n"
			"1:\n"
			"fsatu8.pi f8, f8\n"
			"or x17, x17, x20\n"
			"auipc x24, 0x12\n"
			"mova.x.m x29\n"
			"li x13, 0x809a37d000\n"
			"la x24, mem_ptr_offset\n"
			"flw.ps f16, 384(x24)\n"
			"famomaxug.pi f23, f16 (x13)\n" 
			"fnmsub.s f31, f14, f3, f6, rup\n"
			"flt.pi f22, f23, f22\n"
			"flt.pi f23, f8, f23\n"
			"fmin.pi f14, f6, f12\n"
			"fcvt.pwu.ps f8, f10, rdn\n"
			"fcvt.ps.f16 f22, f22\n"
			"beq x31, x24, 1f\n"
			"1:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f23, 480(x9)\n"
			"fround.ps f20, f23, rmm\n" 
			"sraiw x25, x6, 0\n"
			"mul x9, x20, x9\n"
			"fswizz.ps f22, f22, 0x15\n"
			"bge x17, x13, 1f\n"
			"1:\n"
			"sraw x17, x9, x6\n"
			"beq x15, x25, 1f\n"
			"1:\n"
			"flem.ps m5, f6, f23\n"
			"bne x6, x6, 1f\n"
			"1:\n"
			"fclass.ps f20, f20\n"
			"la x14, rand_int_32\n"
			"flw.ps f17, 192(x14)\n"
			"fnot.pi f12, f17\n" 
			"fsub.ps f31, f14, f25, rtz\n"
			"fsgnj.ps f17, f12, f25\n"
			"remw x25, x29, x25\n"
			"divu x14, x14, x14\n"
			"bge x27, x25, 1f\n"
			"1:\n"
			"slti x20, x9, 0x1b\n"
			"fsra.pi f9, f9, f9\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 416(x22)\n"
			"flw.ps f22, 96(x22)\n"
			"fcmovm.ps f17, f10, f22\n" 
			"sll x11, x11, x19\n"
			"fsgnjn.s f22, f9, f16\n"
			"fsatu8.pi f22, f9\n"
			"fcvt.w.s x13, f6, rmm\n"
			"xori x29, x9, 0x15\n"
			"beq x5, x23, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f12, f10, rup\n"
			"li x22, 0x80aa66e028\n"
			"amominul.d x29, x22, (x22)\n" 
			"fnmadd.ps f12, f9, f9, f9, rup\n"
			"fltm.pi m6, f21, f16\n"
			"sra x14, x24, x14\n"
			"fadd.ps f17, f22, f9, rne\n"
			"fmadd.s f17, f20, f17, f20, rdn\n"
			"subw x17, x13, x17\n"
			"bge x20, x28, 1f\n"
			"1:\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x40\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f17, 0(x31)\n"
			"flw.ps f21, 0(x31)\n"
			"flw.ps f23, 2(x31)\n"
			"flw.ps f25, 11(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x11, 0(x31)\n"
			"lw x13, 0(x31)\n"
			"lw x17, 5(x31)\n"
			"lw x25, 9(x31)\n"
			"lw x29, 15(x31)\n"
			"lw x6, 19(x31)\n"
			"li x31, 0x2\n"
			"LBL_SEQID_3_LOOP_SEQ_HID_11:\n"
			"fsrl.pi f10, f17, f17\n"
			"sllw x29, x29, x17\n"
			"fmax.s f21, f23, f21\n"
			"fslli.pi f21, f21, 0xc\n"
			"fsub.pi f17, f23, f17\n"
			"fcmov.ps f25, f23, f25, f25\n"
			"fence\n"
			"remw x6, x17, x25\n"
			"fsub.ps f23, f25, f23, rmm\n"
			"bltu x7, x5, 1f\n"
			"1:\n"
			"fsetm.pi m5, f21\n"
			"slli x17, x11, 0\n"
			"fandi.pi f3, f17, 0x79\n"
			"fmv.x.s x13, f25\n"
			"for.pi f3, f17, f23\n"
			"fsat8.pi f17, f21\n"
			"fcvt.ps.pw f3, f23, rne\n"
			"fsat8.pi f3, f21\n"
			"ffrc.ps f21, f21\n"
			"fsub.pi f21, f17, f25\n"
			"bgeu x23, x20, 1f\n"
			"1:\n"
			"fsgnjx.s f3, f17, f21\n"
			"mulw x6, x13, x6\n"
			"fmvz.x.ps x20, f21, 2\n"
			"fmaxu.pi f3, f23, f17\n"
			"bltu x26, x8, 1f\n"
			"1:\n"
			"fmul.ps f3, f17, f25, rup\n"
			"fsgnjn.s f21, f17, f21\n"
			"or x6, x17, x11\n"
			"bltu x31, x18, 1f\n"
			"1:\n"
			"fcvt.wu.s x13, f21, rdn\n"
			"mova.m.x x17\n"
			"fmulhu.pi f3, f25, f17\n"
			"divw x6, x11, x29\n"
			"remuw x25, x25, x25\n"
			"or x19, x13, x6\n"
			"fnmsub.s f3, f23, f17, f21, rdn\n"
			"fround.ps f21, f23, rne\n"
			"fmsub.ps f17, f25, f21, f17, rdn\n"
			"fbci.ps f3, 0x825\n"
			"fmax.pi f23, f17, f23\n"
			"maskpopc x6, m6\n"
			"bltu x5, x9, 1f\n"
			"1:\n"
			"fsub.pi f10, f21, f23\n"
			"maskxor m5, m5, m5\n"
			"fcvt.ps.pw f3, f17, rup\n"
			"bge x8, x26, 1f\n"
			"1:\n"
			"fcvt.f16.ps f21, f25\n"
			"ecall\n"
			"fcvt.s.wu f21, x6, rdn\n"
			"lui x20, 0x6\n"
			"feqm.ps m5, f25, f21\n"
			"maskpopcz x25, m4\n"
			"fmvz.x.ps x17, f21, 3\n"
			"maskor m4, m4, m6\n"
			"fltu.pi f21, f23, f17\n"
			"fsgnj.ps f21, f23, f17\n"
			"fence\n"
			"sltu x19, x6, x25\n"
			"blt x30, x26, 1f\n"
			"1:\n"
			"mov.m.x m7, x29, 0x68\n"
			"bge x18, x3, 1f\n"
			"1:\n"
			"flt.s x19, f23, f25\n"
			"bgeu x11, x1, 1f\n"
			"1:\n"
			"faddi.pi f23, f23, 0xdb\n"
			"rem x19, x25, x17\n"
			"mova.m.x x11\n"
			"fsgnj.s f17, f17, f23\n"
			"mova.m.x x29\n"
			"blt x31, x1, 1f\n"
			"1:\n"
			"fsgnjn.ps f21, f21, f23\n"
			"feq.ps f10, f25, f17\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_3_LOOP_SEQ_HID_11\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"la x31, check_sum_hid_11\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"LBL_POSTAMBLE_HID_11:\n"          
			"csrwi   tensor_wait, 0x0\n"
			"csrwi   tensor_wait, 0x1\n"
			"csrwi   tensor_wait, 0x2\n"
			"csrwi   tensor_wait, 0x3\n"
			"csrwi   tensor_wait, 0x4\n"
			"csrwi   tensor_wait, 0x5\n"
			"csrwi   tensor_wait, 0x6\n"
			"csrwi   tensor_wait, 0x7\n"
			"csrwi   tensor_wait, 0x8\n"
			"csrwi   tensor_wait, 0x9\n"
			"csrwi   tensor_wait, 0xa\n"
			"fence\n"
			"LBL_SYNC_POINT_1_HID_11:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_1_WAIT_HID_11\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_1_EXIT_HID_11\n"
			"LBL_SYNC_POINT_1_WAIT_HID_11:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_1_EXIT_HID_11:\n"          
			"nop\n"
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_11\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_11:\n"          
			".global LBL_FAIL_HID_11\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_11\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_11:\n"          
			".global LBL_PASS_HID_11\n"          
			VSNIP_RSV
		);
		c_test_pass();
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_11:\n"          
			".global LBL_HALT_HID_11\n"          
			"wfi\n"
			"j LBL_HALT_HID_11\n"          
			VSNIP_RSV
		);
		return 0;       
	};
	if ( hid == 12 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_12:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_12\n"
			"csrw mscratch, x5\n"
			"csrwi menable_shadows, 0xf\n"
			"li x5, -1\n"
			"csrw mcounteren, x5\n"
			"csrw scounteren, x5\n"
			"csrwi mhpmcounter3, 0\n"
			"csrwi mhpmcounter4, 0\n"
			"csrwi mhpmcounter5, 0\n"
			"csrwi mhpmcounter6, 0\n"
			"csrwi mhpmcounter7, 0\n"
			"csrwi mhpmcounter8, 0\n"
			"csrwi mhpmevent3, 21\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 28\n"   
			"csrwi mhpmevent6, 20\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 6\n"   
			"mov.m.x m0, x0, 0xff\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f29,  224 (x5)\n"
			"flw.ps f25,  832 (x5)\n"
			"flw.ps f22,  736 (x5)\n"
			"flw.ps f11,  256 (x5)\n"
			"flw.ps f4,  576 (x5)\n"
			"flw.ps f6,  608 (x5)\n"
			"flw.ps f0,  896 (x5)\n"
			"flw.ps f8,  992 (x5)\n"
			"flw.ps f27,  544 (x5)\n"
			"flw.ps f12,  192 (x5)\n"
			"flw.ps f9,  800 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f21,  960 (x5)\n"
			"flw.ps f13,  352 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f7,  864 (x5)\n"
			"flw.ps f30,  768 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f24,  416 (x5)\n"
			"flw.ps f17,  64 (x5)\n"
			"flw.ps f19,  320 (x5)\n"
			"flw.ps f31,  288 (x5)\n"
			"flw.ps f1,  0 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"flw.ps f28,  96 (x5)\n"
			"flw.ps f26,  480 (x5)\n"
			"flw.ps f20,  512 (x5)\n"
			"flw.ps f5,  384 (x5)\n"
			"flw.ps f18,  640 (x5)\n"
			"flw.ps f3,  128 (x5)\n"
			"flw.ps f16,  672 (x5)\n"
			"flw.ps f15,  928 (x5)\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_HID_12:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_0_WAIT_HID_12\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_0_EXIT_HID_12\n"
			"LBL_SYNC_POINT_0_WAIT_HID_12:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_EXIT_HID_12:\n"          
			"nop\n"
			"LBL_KERNEL_HID_12:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_12\n"
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_HID_12:\n"
			"li x16, 0x80a740c010\n"
			"amoandg.d x5, x15, (x16)\n" 
			"fsrli.pi f25, f25, 0x5\n"
			"beq x30, x8, 1f\n"
			"1:\n"
			"li x5, 0x80a7090008\n"
			"la x26, mem_ptr_offset\n"
			"flw.ps f1, 768(x26)\n"
			"famominug.pi f26, f1 (x5)\n" 
			"slliw x28, x16, 5\n"
			"csrr x0, mie\n"
			"fcvt.ps.f16 f28, f0\n"
			"li x24, 0x80a3862008\n"
			"amomaxug.d x24, x5, (x24)\n" 
			"li x5, 0xc4f2877af1efe6de\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x494d43d233f356b\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"fsetm.pi m6, f12\n"
			"maskpopcz x7, m5\n" 
			"fsrli.pi f31, f3, 0xd\n"
			"li x25, 0x80b163a010\n"
			"la x26, mem_ptr_offset\n"
			"flw.ps f16, 192(x26)\n"
			"fgwl.ps f31, f16 (x25)\n" 
			"fmv.s.x f26, x28\n"
			"li x27, 0xf7c0c01aaddde85b\n"
			"srliw x24, x27, 2\n" 
			"fsatu8.pi f1, f1\n"
			"li x24, 0x80b4e35008\n"
			"amomaxl.d x7, x19, (x24)\n" 
			"fsgnjn.s f14, f14, f16\n"
			"beq x16, x21, 1f\n"
			"1:\n"
			"li x5, 0x807162e818\n"
			"fsq2 f7, 512(x5)\n" 
			"fsub.pi f3, f21, f3\n"
			"li x15, 0x80a4258020\n"
			"amoandg.w x16, x16, (x15)\n" 
			"li x5, 0xed15\n"
			"csrw tensor_mask, x5\n"	
			"div x7, x7, x19\n"
			"li x27, 0x80b01d9028\n"
			"amomaxul.d x26, x28, (x27)\n" 
			"mov.m.x m5, x15, 0x54\n"
			"li x15, 0x80a0fe1000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f21, (x15)\n" 
			"maskpopc x26, m6\n"
			"li x25, 0x9e773a02c23d7f47\n"
			"li x15, 0xefffae94571592df\n"
			"slt x27, x25, x15\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fmin.s f10, f14, f1\n"
			"li x16, 0x80a234e038\n"
			"la x28, mem_ptr_offset\n"
			"flw.ps f16, 544(x28)\n"
			"famoaddg.pi f29, f16 (x16)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fmul.s f25, f21, f21, rdn\n"
			"la x5, rand_int_32\n"
			"flw.ps f27, 992(x5)\n"
			"flw.ps f3, 544(x5)\n"
			"fsrl.pi f10, f27, f3\n" 
			"fround.ps f21, f28, rmm\n"
			"li x27, 0xebf1538701108f47\n"
			"andi x19, x27, 0x18\n" 
			"li x5, 0xb8d7\n"
			"csrw tensor_mask, x5\n"	
			"fcvt.ps.pwu f25, f25, rdn\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_0_TLD_ID_0_NMASK_0x1_CMASK_0xfb_SHIRE_0_HID_12:\n"
			"li x31, 0x1fb1e\n"
			"csrw tensor_coop, x31\n" 
			"li x31, 0x01FF000001FF0000\n"
			"csrw tensor_mask, x31\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"li x27, 0xd440008020006b28\n"
			"csrw tensor_load, x27\n" 
			"csrwi tensor_wait, 1\n" 
			"LBL_SYNC_POINT_1_HID_12:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_1_WAIT_HID_12\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_1_EXIT_HID_12\n"
			"LBL_SYNC_POINT_1_WAIT_HID_12:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_1_EXIT_HID_12:\n"          
			"nop\n"
			"LBL_SYNC_POINT_2_HID_12:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_2_WAIT_HID_12\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_2_EXIT_HID_12\n"
			"LBL_SYNC_POINT_2_WAIT_HID_12:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_2_EXIT_HID_12:\n"          
			"nop\n"
			"LBL_SYNC_POINT_3_HID_12:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_3_WAIT_HID_12\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_3_EXIT_HID_12\n"
			"LBL_SYNC_POINT_3_WAIT_HID_12:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_3_EXIT_HID_12:\n"          
			"nop\n"
			"li x5, 0xe71023223da3b0\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xdf755cfc7c4257af\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x6, 0x3d88c0001a345333\n"
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_12:\n"
			"li x6, 0x3964c00000001449\n"
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 2\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrci x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x8\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f1, 4(x31)\n"
			"flw.ps f10, 1(x31)\n"
			"flw.ps f12, 8(x31)\n"
			"flw.ps f14, 0(x31)\n"
			"flw.ps f15, 16(x31)\n"
			"flw.ps f16, 12(x31)\n"
			"flw.ps f21, 5(x31)\n"
			"flw.ps f25, 21(x31)\n"
			"flw.ps f26, 14(x31)\n"
			"flw.ps f27, 7(x31)\n"
			"flw.ps f28, 2(x31)\n"
			"flw.ps f29, 34(x31)\n"
			"flw.ps f3, 39(x31)\n"
			"flw.ps f31, 12(x31)\n"
			"flw.ps f7, 38(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x15, 2(x31)\n"
			"lw x16, 1(x31)\n"
			"lw x19, 11(x31)\n"
			"lw x24, 8(x31)\n"
			"lw x25, 15(x31)\n"
			"lw x26, 5(x31)\n"
			"lw x27, 22(x31)\n"
			"lw x28, 14(x31)\n"
			"lw x5, 36(x31)\n"
			"lw x7, 11(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_12:\n"
			"slt x24, x27, x5\n"
			"maskor m7, m7, m5\n"
			"fsgnj.s f28, f21, f1\n"
			"bgeu x26, x29, 1f\n"
			"1:\n"
			"fcvt.s.w f25, x19, rtz\n"
			"fadd.s f15, f26, f21, rup\n"
			"fmadd.ps f7, f1, f25, f7, rup\n"
			"bgeu x21, x30, 1f\n"
			"1:\n"
			"fcvt.ps.pw f25, f16, rdn\n"
			"feqm.ps m0, f1, f7\n"
			"remu x7, x7, x28\n"
			"sra x27, x26, x0\n"
			"maskxor m7, m7, m7\n"
			"fadd.pi f7, f26, f0\n"
			"fcvt.pw.ps f29, f27, rup\n"
			"addiw x5, x26, 0x1\n"
			"bltu x20, x28, 1f\n"
			"1:\n"
			"fswizz.ps f1, f27, 0xc\n"
			"fsgnjx.ps f21, f29, f31\n"
			"sub x16, x25, x24\n"
			"blt x2, x17, 1f\n"
			"1:\n"
			"maskor m7, m7, m6\n"
			"fmul.pi f27, f31, f15\n"
			"flem.ps m5, f31, f3\n"
			"fsgnjx.ps f26, f25, f7\n"
			"xori x26, x16, 0x18\n"
			"fsetm.pi m0, f31\n"
			"maskxor m5, m0, m0\n"
			"bne x31, x25, 1f\n"
			"1:\n"
			"fmax.pi f1, f1, f16\n"
			"srli x16, x16, 5\n"
			"fsub.s f14, f27, f31, rdn\n"
			"bgeu x8, x0, 1f\n"
			"1:\n"
			"sraw x15, x7, x15\n"
			"mova.m.x x24\n"
			"maskpopc x24, m5\n"
			"packb x25, x16, x27\n"
			"fmin.ps f14, f21, f1\n"
			"fmsub.ps f28, f25, f14, f25, rup\n"
			"srliw x5, x16, 5\n"
			"fsub.s f7, f7, f10, rdn\n"
			"fmv.x.s x27, f27\n"
			"fsgnj.ps f10, f10, f15\n"
			"mul x27, x7, x28\n"
			"fsat8.pi f0, f25\n"
			"fsgnj.ps f0, f0, f27\n"
			"srliw x27, x26, 5\n"
			"fmul.pi f15, f15, f14\n"
			"fmadd.ps f1, f21, f1, f10, rdn\n"
			"addw x15, x19, x27\n"
			"bne x29, x12, 1f\n"
			"1:\n"
			"fswizz.ps f3, f31, 0x1e\n"
			"fbci.ps f14, 0x5ec6\n"
			"fadd.s f16, f16, f27, rup\n"
			"fmulhu.pi f31, f0, f10\n"
			"fsrl.pi f1, f10, f28\n"
			"feq.pi f12, f31, f12\n"
			"fslli.pi f27, f12, 0x2\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_0_LOOP_SEQ_HID_12\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_HID_12:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f25, 96(x7)\n"
			"fclass.ps f31, f25\n" 
			"mulw x14, x0, x24\n"
			"bge x10, x29, 1f\n"
			"1:\n"
			"divuw x19, x19, x16\n"
			"fmv.s.x f10, x26\n"
			"fmin.s f31, f31, f0\n"
			"mova.m.x x0\n"
			"flt.s x19, f1, f31\n"
			"fmin.s f21, f1, f12\n"
			"blt x8, x11, 1f\n"
			"1:\n"
			"fpackrepb.pi f21, f0\n"
			"remu x26, x7, x15\n"
			"li x25, 0x80b54fc010\n"
			"la x27, mem_ptr_offset\n"
			"flw.ps f12, 256(x27)\n"
			"famoswapl.pi f31, f12 (x25)\n" 
			"slli x24, x24, 3\n"
			"fnmadd.s f0, f3, f10, f7, rup\n"
			"bgeu x20, x19, 1f\n"
			"1:\n"
			"fpackrepb.pi f26, f26\n"
			"flt.pi f28, f1, f10\n"
			"fsat8.pi f3, f3\n"
			"fnmadd.ps f16, f15, f16, f31, rne\n"
			"fmax.pi f3, f1, f26\n"
			"beq x22, x28, 1f\n"
			"1:\n"
			"fmvz.x.ps x24, f0, 4\n"
			"fround.ps f16, f3, rmm\n"
			"li x16, 0x80b4814018\n"
			"amoandl.d x26, x19, (x16)\n" 
			"fclass.ps f15, f26\n"
			"fence\n"
			"fnmadd.ps f7, f7, f27, f0, rne\n"
			"maskpopcz x26, m7\n"
			"fmv.x.s x7, f0\n"
			"remuw x0, x25, x19\n"
			"fsatu8.pi f31, f1\n"
			"bltu x11, x28, 1f\n"
			"1:\n"
			"mul x7, x25, x19\n"
			"fsat8.pi f0, f1\n"
			"li x28, 0x80726fc438\n"
			"sd x7, -256(x28)\n"
			"lh x15, -256(x28)\n" 
			"fsgnjx.s f14, f29, f16\n"
			"fmax.pi f14, f27, f31\n"
			"fpackrepb.pi f16, f16\n"
			"blt x22, x3, 1f\n"
			"1:\n"
			"fnmadd.s f14, f1, f25, f10, rup\n"
			"fmul.pi f15, f21, f7\n"
			"bge x5, x13, 1f\n"
			"1:\n"
			"fsra.pi f1, f31, f0\n"
			"fsub.pi f25, f25, f28\n"
			"fsgnj.s f3, f3, f15\n"
			"bltu x0, x30, 1f\n"
			"1:\n"
			"flog.ps f27, f21\n"
			"li x14, 0x8076888e70\n"
			"fsw.ps f21, -512(x14)\n"
			"fbc.ps f10, -512(x14)\n" 
			"fround.ps f27, f26, rmm\n"
			"fmsub.ps f31, f27, f14, f15, rup\n"
			"fmul.pi f3, f16, f3\n"
			"beq x7, x14, 1f\n"
			"1:\n"
			"fandi.pi f16, f28, 0x5c\n"
			"fmul.s f7, f26, f26, rne\n"
			"fmin.pi f0, f10, f28\n"
			"fcvt.ps.f16 f31, f16\n"
			"fmulhu.pi f3, f3, f29\n"
			"fmin.s f0, f27, f10\n"
			"li x5, 0x8077ad09d8\n"
			"fsw f28, -128(x5)\n" 
			"fadd.s f0, f0, f1, rtz\n"
			"fmin.pi f21, f15, f1\n"
			"fnmadd.ps f26, f1, f15, f25, rup\n"
			"bltu x15, x16, 1f\n"
			"1:\n"
			"fcmov.ps f0, f0, f15, f1\n"
			"bltu x0, x20, 1f\n"
			"1:\n"
			"faddi.pi f28, f14, 0x1a0\n"
			"fcvt.ps.f16 f10, f14\n"
			"fmadd.s f21, f28, f14, f14, rtz\n"
			"fsub.pi f26, f25, f15\n"
			"feq.pi f16, f25, f15\n"
			"bne x16, x4, 1f\n"
			"1:\n"
			"li x7, 0x80b19de038\n"
			"la x5, mem_ptr_offset\n"
			"flw.ps f12, 32(x5)\n"
			"fgwl.ps f27, f12 (x7)\n" 
			"fround.ps f16, f16, rdn\n"
			"fcvt.ps.pw f29, f16, rmm\n"
			"maskand m7, m6, m7\n"
			"slli x15, x27, 0\n"
			"sll x16, x16, x19\n"
			"masknot m0, m7\n"
			"mov.m.x m0, x0, 0xff\n"
			"andi x5, x24, 0x13\n"
			"fsll.pi f10, f26, f31\n"
			"sraiw x28, x19, 1\n"
			"bge x7, x24, 1f\n"
			"1:\n"
			"li x16, 0x80a457d03c\n"
			"amominug.w x24, x15, (x16)\n" 
			"slliw x19, x16, 4\n"
			"fsrl.pi f7, f0, f0\n"
			"fmsub.s f31, f7, f12, f3, rmm\n"
			"addi x5, x5, 0x5\n"
			"fsgnj.ps f10, f10, f10\n"
			"fcvt.pw.ps f7, f0, rtz\n"
			"feq.ps f7, f0, f0\n"
			"slt x19, x5, x5\n"
			"fpackrepb.pi f31, f0\n"
			"bge x0, x30, 1f\n"
			"1:\n"
			"li x25, 0x80712db850\n"
			"la x16, mem_ptr_offset\n"
			"flw.ps f25, 544(x16)\n"
			"fscb.ps f25, f25 (x25)\n" 
			"fadd.s f16, f27, f16, rdn\n"
			"fandi.pi f27, f27, 0x77\n"
			"fnmadd.s f16, f28, f16, f28, rmm\n"
			"fsub.s f21, f21, f28, rne\n"
			"bge x23, x24, 1f\n"
			"1:\n"
			"fmadd.ps f27, f28, f28, f27, rup\n"
			"fmin.pi f25, f28, f27\n"
			"fcmov.ps f26, f16, f25, f25\n"
			"fmax.ps f10, f25, f16\n"
			"frcp.ps f12, f25\n"
			"li x25, 0x80b5a64028\n"
			"amominul.w x5, x16, (x25)\n" 
			"remuw x19, x19, x5\n"
			"fmin.pi f12, f26, f12\n"
			"blt x8, x17, 1f\n"
			"1:\n"
			"fadd.s f26, f12, f26, rne\n"
			"fcvt.f16.ps f25, f21\n"
			"ecall\n"
			"srai x14, x27, 1\n"
			"srlw x7, x7, x26\n"
			"bge x22, x0, 1f\n"
			"1:\n"
			"masknot m7, m7\n"
			"fnmadd.s f21, f21, f31, f12, rne\n"
			"li x14, 0x80b1271000\n"
			"amomaxl.d x27, x24, (x14)\n" 
			"fbci.pi f15, 0x56239\n"
			"srl x28, x24, x7\n"
			"sll x15, x24, x7\n"
			"ecall\n"
			"fmin.ps f25, f25, f1\n"
			"fclass.ps f14, f14\n"
			"fadd.s f25, f1, f25, rmm\n"
			"fnmadd.ps f3, f14, f25, f1, rdn\n"
			"xor x26, x0, x0\n"
			"li x14, 0x80a6dd4020\n"
			"la x27, mem_ptr_offset\n"
			"flw.ps f28, 64(x27)\n"
			"famoxorg.pi f0, f28 (x14)\n" 
			"lui x5, 0xf\n"
			"fence\n"
			"bne x24, x30, 1f\n"
			"1:\n"
			"fmadd.s f3, f12, f15, f12, rdn\n"
			"xori x5, x25, 0x1c\n"
			"fence\n"
			"bge x2, x24, 1f\n"
			"1:\n"
			"fcvt.pw.ps f29, f28, rmm\n"
			"srl x5, x7, x15\n"
			"fnmsub.ps f0, f0, f28, f1, rne\n"
			"auipc x14, 0x3\n"
			"li x28, 0x12b91e2df611246c\n"
			"sraiw x7, x28, 3\n" 
			"li x5, 0xceab\n"
			"csrw tensor_mask, x5\n"	
			"maskxor m6, m6, m5\n"
			"beq x18, x6, 1f\n"
			"1:\n"
			"fbci.ps f27, 0x10e2c\n"
			"fsgnjn.ps f10, f10, f0\n"
			"flt.pi f12, f27, f14\n"
			"fcvt.wu.s x26, f25, rmm\n"
			"fbcx.ps f10, x28\n"
			"fmv.s.x f15, x25\n"
			"fandi.pi f29, f29, 0x66\n"
			"maskand m5, m5, m5\n"
			"bge x23, x27, 1f\n"
			"1:\n"
			"li x7, 0x80b6bb3000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f26, (x7)\n" 
			"fsrl.pi f27, f16, f29\n"
			"fsub.ps f25, f0, f10, rmm\n"
			"fadd.pi f31, f14, f28\n"
			"fmin.s f0, f1, f0\n"
			"fmaxu.pi f1, f29, f31\n"
			"fmadd.ps f10, f29, f16, f27, rup\n"
			"fmax.s f1, f29, f1\n"
			"feq.pi f15, f15, f28\n"
			"fswizz.ps f1, f0, 0xa\n"
			"li x19, 0x8077953480\n"
			"li x7, 1423538047\n"
			"fsc32w.ps f12, x26 (x19)\n" 
			"fsrl.pi f21, f21, f12\n"
			"fcmov.ps f15, f26, f10, f7\n"
			"bltu x3, x2, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f14, f10, rmm\n"
			"blt x8, x28, 1f\n"
			"1:\n"
			"for.pi f3, f21, f21\n"
			"fround.ps f3, f16, rdn\n"
			"fmul.ps f26, f0, f7, rmm\n"
			"fsgnjx.s f16, f12, f16\n"
			"bne x18, x7, 1f\n"
			"1:\n"
			"fsgnjx.ps f14, f21, f12\n"
			"fsub.pi f3, f7, f12\n"
			"li x5, 0x807783b0f7\n"
			"sd x25, -512(x5)\n"
			"ld x26, -512(x5)\n" 
			"fmin.ps f1, f1, f12\n"
			"fsgnjn.ps f29, f7, f28\n"
			"fnmadd.s f7, f7, f1, f21, rdn\n"
			"fmin.pi f3, f26, f29\n"
			"bgeu x6, x12, 1f\n"
			"1:\n"
			"fnmsub.s f3, f28, f29, f12, rdn\n"
			"fsgnj.s f27, f26, f3\n"
			"flt.pi f12, f16, f27\n"
			"fcvt.f16.ps f29, f10\n"
			"fle.pi f3, f16, f10\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SYNC_POINT_4_HID_12:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_4_WAIT_HID_12\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_4_EXIT_HID_12\n"
			"LBL_SYNC_POINT_4_WAIT_HID_12:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_4_EXIT_HID_12:\n"          
			"nop\n"
			"LBL_SYNC_POINT_5_HID_12:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_5_WAIT_HID_12\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_5_EXIT_HID_12\n"
			"LBL_SYNC_POINT_5_WAIT_HID_12:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_5_EXIT_HID_12:\n"          
			"nop\n"
			"LBL_SYNC_POINT_6_HID_12:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_6_WAIT_HID_12\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_6_EXIT_HID_12\n"
			"LBL_SYNC_POINT_6_WAIT_HID_12:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_6_EXIT_HID_12:\n"          
			"nop\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, 0x1206000000a7a92\n"
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 3\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x40\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f12, 2(x31)\n"
			"flw.ps f25, 0(x31)\n"
			"flw.ps f26, 1(x31)\n"
			"flw.ps f27, 14(x31)\n"
			"flw.ps f28, 1(x31)\n"
			"flw.ps f29, 4(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x14, 0(x31)\n"
			"lw x15, 2(x31)\n"
			"lw x16, 2(x31)\n"
			"lw x24, 2(x31)\n"
			"lw x28, 11(x31)\n"
			"lw x7, 8(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_12:\n"
			"fsat8.pi f7, f25\n"
			"fmax.s f12, f27, f1\n"
			"fslli.pi f26, f26, 0xd\n"
			"bge x10, x15, 1f\n"
			"1:\n"
			"fadd.pi f7, f12, f1\n"
			"xori x14, x7, 0x13\n"
			"srliw x26, x15, 4\n"
			"addw x24, x24, x7\n"
			"remw x28, x15, x28\n"
			"fadd.s f14, f28, f25, rtz\n"
			"bgeu x25, x7, 1f\n"
			"1:\n"
			"auipc x0, 0x1e\n"
			"fadd.ps f29, f27, f29, rne\n"
			"bltu x26, x5, 1f\n"
			"1:\n"
			"maskpopcz x14, m5\n"
			"fmin.s f25, f25, f1\n"
			"fadd.s f28, f28, f1, rmm\n"
			"maskxor m5, m6, m6\n"
			"bne x20, x30, 1f\n"
			"1:\n"
			"fsgnjn.s f27, f29, f27\n"
			"bgeu x12, x14, 1f\n"
			"1:\n"
			"fence\n"
			"fmadd.s f29, f27, f29, f27, rmm\n"
			"bne x1, x20, 1f\n"
			"1:\n"
			"fmv.x.s x26, f25\n"
			"fcvt.w.s x24, f12, rup\n"
			"mul x0, x24, x24\n"
			"bge x24, x23, 1f\n"
			"1:\n"
			"fslli.pi f28, f25, 0x0\n"
			"bgeu x5, x26, 1f\n"
			"1:\n"
			"fsetm.pi m6, f29\n"
			"bne x14, x19, 1f\n"
			"1:\n"
			"maskxor m7, m7, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"feqm.ps m7, f27, f26\n"
			"fnmsub.ps f29, f29, f26, f28, rne\n"
			"fmul.pi f27, f27, f29\n"
			"bne x29, x27, 1f\n"
			"1:\n"
			"fbci.ps f12, 0x1e1d6\n"
			"srlw x26, x28, x7\n"
			"maskand m0, m5, m6\n"
			"mov.m.x m0, x0, 0xff\n"
			"bne x14, x2, 1f\n"
			"1:\n"
			"fbcx.ps f27, x28\n"
			"feq.pi f27, f27, f28\n"
			"fadd.pi f15, f28, f26\n"
			"maskxor m7, m6, m7\n"
			"fsgnj.ps f7, f1, f12\n"
			"beq x24, x7, 1f\n"
			"1:\n"
			"fsra.pi f29, f27, f25\n"
			"maskxor m0, m0, m7\n"
			"mov.m.x m0, x0, 0xff\n"
			"fand.pi f12, f29, f29\n"
			"blt x17, x15, 1f\n"
			"1:\n"
			"fmin.s f29, f29, f26\n"
			"masknot m5, m6\n"
			"blt x13, x8, 1f\n"
			"1:\n"
			"ffrc.ps f12, f12\n"
			"packb x25, x15, x16\n"
			"fnot.pi f29, f25\n"
			"frcp.ps f14, f28\n"
			"fxor.pi f14, f29, f12\n"
			"divu x0, x14, x16\n"
			"sub x0, x7, x28\n"
			"bltu x27, x21, 1f\n"
			"1:\n"
			"fandi.pi f15, f27, 0x3e\n"
			"fle.s x0, f25, f1\n"
			"fmv.x.s x25, f26\n"
			"bgeu x30, x9, 1f\n"
			"1:\n"
			"fandi.pi f15, f1, 0x134\n"
			"fand.pi f7, f1, f26\n"
			"beq x19, x24, 1f\n"
			"1:\n"
			"fexp.ps f12, f27\n"
			"divu x0, x16, x24\n"
			"fadd.pi f14, f29, f27\n"
			"fsrli.pi f26, f26, 0xc\n"
			"blt x4, x9, 1f\n"
			"1:\n"
			"fle.s x26, f12, f12\n"
			"slliw x24, x24, 5\n"
			"maskor m6, m7, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"ecall\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_1_LOOP_SEQ_HID_12\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_HID_12:\n"
			"li x15, 0x80a0940008\n"
			"la x14, mem_ptr_offset\n"
			"flw.ps f1, 32(x14)\n"
			"famoming.pi f29, f1 (x15)\n" 
			"fence\n"
			"fnmadd.s f28, f31, f29, f26, rmm\n"
			"fmv.s.x f16, x28\n"
			"li x19, 0x80b715f020\n"
			"la x15, mem_ptr_offset\n"
			"flw.ps f15, 960(x15)\n"
			"fscbl.ps f15, f15 (x19)\n" 
			"fcmovm.ps f12, f21, f7\n"
			"mul x0, x28, x26\n"
			"fcmov.ps f27, f3, f10, f21\n"
			"li x16, 0x80a33e3028\n"
			"la x27, mem_ptr_offset\n"
			"flw.ps f0, 576(x27)\n"
			"fschg.ps f29, f0 (x16)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fsgnjn.s f10, f16, f0\n"
			"bltu x19, x4, 1f\n"
			"1:\n"
			"fadd.pi f16, f16, f0\n"
			"flog.ps f0, f0\n"
			"li x26, 0x8bd9d436cbefb5af\n"
			"add x19, x26, x26\n" 
			"fcmov.ps f3, f27, f3, f12\n"
			"fsrl.pi f25, f21, f25\n"
			"flog.ps f16, f29\n"
			"bltu x25, x4, 1f\n"
			"1:\n"
			"li x5, 0x80b6c7a038\n"
			"la x7, mem_ptr_offset\n"
			"flw.ps f7, 160(x7)\n"
			"famoorl.pi f31, f7 (x5)\n" 
			"for.pi f12, f12, f12\n"
			"srai x14, x7, 5\n"
			"fround.ps f12, f12, rup\n"
			"la x24, rand_ieee754_32\n"
			"flw.ps f28, 192(x24)\n"
			"fcvt.f16.ps f15, f28\n" 
			"fmul.s f27, f0, f27, rmm\n"
			"fsrli.pi f1, f3, 0xa\n"
			"bne x18, x18, 1f\n"
			"1:\n"
			"maskpopc x25, m6\n"
			"li x28, 0x80726a3dab\n"
			"sd x15, 128(x28)\n"
			"lwu x14, 128(x28)\n" 
			"fsub.ps f12, f12, f10, rdn\n"
			"flog.ps f28, f0\n"
			"fswizz.ps f7, f12, 0x5\n"
			"li x5, 0x8071a4cc85\n"
			"sh x7, -512(x5)\n" 
			"fcvt.w.s x14, f14, rne\n"
			"packb x0, x16, x19\n"
			"fadd.s f12, f28, f0, rtz\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 704(x26)\n"
			"flw.ps f16, 160(x26)\n"
			"flw.ps f15, 960(x26)\n"
			"fnmsub.ps f3, f21, f16, f15, rtz\n" 
			"mulhsu x14, x5, x14\n"
			"fcvt.s.wu f0, x16, rup\n"
			"fcvt.f16.ps f0, f21\n"
			"bge x6, x13, 1f\n"
			"1:\n"
			"li x7, 0x80b5926000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f28, (x7)\n" 
			"fmv.x.s x25, f15\n"
			"blt x15, x18, 1f\n"
			"1:\n"
			"masknot m0, m0\n"
			"beq x28, x22, 1f\n"
			"1:\n"
			"remu x26, x26, x28\n"
			"blt x29, x16, 1f\n"
			"1:\n"
			"li x15, 0x80a067d000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f26, (x15)\n" 
			"fcvt.ps.f16 f25, f12\n"
			"fcmov.ps f28, f15, f3, f0\n"
			"fand.pi f25, f1, f21\n"
			"bgeu x6, x19, 1f\n"
			"1:\n"
			"li x16, 0x80a3fc203c\n"
			"amoxorg.w x16, x27, (x16)\n" 
			"fsra.pi f31, f25, f3\n"
			"sll x16, x15, x16\n"
			"sltu x16, x27, x16\n"
			"la x15, rand_int_32\n"
			"flw.ps f26, 320(x15)\n"
			"fandi.pi f15, f26, 0x50\n" 
			"fltu.pi f7, f12, f27\n"
			"fcvt.ps.pw f10, f15, rne\n"
			"ori x0, x25, 0x5\n"
			"li x14, 0x807511e558\n"
			"li x7, 3147996678\n"
			"fg32b.ps f12, x24 (x14)\n" 
			"fsgnj.ps f10, f29, f28\n"
			"fsgnjn.s f12, f1, f31\n"
			"bgeu x7, x16, 1f\n"
			"1:\n"
			"fsrl.pi f12, f12, f31\n"
			"li x14, 0x8073762700\n"
			"fsq2 f15, 128(x14)\n" 
			"fswizz.ps f3, f1, 0x1e\n"
			"frcp.ps f25, f12\n"
			"fmulh.pi f25, f1, f28\n"
			"li x26, 0x8071550530\n"
			"la x19, mem_ptr_offset\n"
			"flw.ps f27, 704(x19)\n"
			"fscb.ps f31, f27 (x26)\n" 
			"fmadd.s f26, f15, f25, f26, rmm\n"
			"feq.pi f31, f29, f28\n"
			"blt x26, x11, 1f\n"
			"1:\n"
			"fpackrepb.pi f29, f10\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0xc0000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000007\n"
			"add x6, x6, x31\n"
			"li x7, 0x10380000001a063\n"
			"li x9, 0x1f00e00000000067\n"
			"li x10, 0xe0f8008070008f80\n"
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x10\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrw tensor_quant, x9\n"	
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 10\n"
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0x20000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000004\n"
			"add x6, x6, x31\n"
			"li x7, 0x1ca00000011d013\n"
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xa\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 3\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x18\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f1, 1(x31)\n"
			"flw.ps f10, 2(x31)\n"
			"flw.ps f14, 8(x31)\n"
			"flw.ps f15, 12(x31)\n"
			"flw.ps f16, 5(x31)\n"
			"flw.ps f21, 22(x31)\n"
			"flw.ps f25, 13(x31)\n"
			"flw.ps f26, 21(x31)\n"
			"flw.ps f27, 2(x31)\n"
			"flw.ps f29, 14(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x14, 1(x31)\n"
			"lw x15, 2(x31)\n"
			"lw x16, 0(x31)\n"
			"lw x19, 15(x31)\n"
			"lw x24, 13(x31)\n"
			"lw x25, 16(x31)\n"
			"lw x26, 14(x31)\n"
			"lw x27, 32(x31)\n"
			"lw x28, 13(x31)\n"
			"lw x5, 37(x31)\n"
			"lw x7, 16(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_12:\n"
			"srl x25, x19, x27\n"
			"fmax.pi f10, f27, f1\n"
			"fmulh.pi f3, f1, f10\n"
			"bltu x4, x17, 1f\n"
			"1:\n"
			"ebreak\n"
			"fltu.pi f26, f1, f15\n"
			"flt.ps f21, f21, f14\n"
			"flog.ps f7, f14\n"
			"fmin.pi f12, f0, f0\n"
			"fnmadd.s f21, f1, f16, f14, rne\n"
			"fsrl.pi f26, f29, f15\n"
			"bge x4, x16, 1f\n"
			"1:\n"
			"fsgnjx.ps f3, f0, f16\n"
			"sraw x27, x27, x24\n"
			"andi x14, x28, 0xe\n"
			"srli x25, x28, 0\n"
			"fcmov.ps f15, f21, f25, f0\n"
			"fcvt.s.wu f26, x19, rmm\n"
			"fsll.pi f27, f27, f10\n"
			"and x27, x27, x0\n"
			"ffrc.ps f15, f26\n"
			"xori x0, x28, 0x1c\n"
			"for.pi f0, f10, f21\n"
			"div x27, x27, x5\n"
			"fslli.pi f21, f1, 0x1\n"
			"fmvs.x.ps x7, f21, 1\n"
			"mova.m.x x7\n"
			"ffrc.ps f12, f26\n"
			"srai x27, x7, 1\n"
			"fsatu8.pi f15, f14\n"
			"bltu x10, x26, 1f\n"
			"1:\n"
			"fmvz.x.ps x27, f25, 3\n"
			"fle.pi f27, f15, f27\n"
			"fcvt.ps.f16 f7, f0\n"
			"fcmovm.ps f21, f25, f10\n"
			"feq.pi f3, f14, f0\n"
			"divw x26, x26, x28\n"
			"xori x28, x28, 0x1\n"
			"fand.pi f15, f25, f29\n"
			"for.pi f21, f16, f29\n"
			"fexp.ps f21, f1\n"
			"flt.s x5, f1, f27\n"
			"fsgnjn.s f10, f10, f10\n"
			"blt x30, x12, 1f\n"
			"1:\n"
			"sra x0, x0, x7\n"
			"fsub.pi f3, f26, f14\n"
			"fmvz.x.ps x26, f21, 3\n"
			"bge x25, x19, 1f\n"
			"1:\n"
			"sraw x15, x25, x16\n"
			"sll x26, x28, x19\n"
			"bgeu x26, x26, 1f\n"
			"1:\n"
			"auipc x5, 0x8\n"
			"div x15, x15, x26\n"
			"subw x0, x26, x0\n"
			"bge x18, x19, 1f\n"
			"1:\n"
			"fbci.ps f12, 0x22793\n"
			"fltu.pi f10, f26, f10\n"
			"fswizz.ps f1, f29, 0x9\n"
			"slti x0, x5, 0x4\n"
			"srliw x28, x19, 4\n"
			"beq x31, x18, 1f\n"
			"1:\n"
			"masknot m0, m7\n"
			"mov.m.x m0, x0, 0xff\n"
			"slli x19, x0, 2\n"
			"or x19, x14, x5\n"
			"fsrli.pi f12, f10, 0x3\n"
			"mova.x.m x26\n"
			"fsub.pi f0, f0, f29\n"
			"fbci.ps f0, 0x77c37\n"
			"addi x24, x0, 0x9\n"
			"mulh x27, x27, x5\n"
			"bltu x17, x0, 1f\n"
			"1:\n"
			"andi x27, x7, 0xd\n"
			"fsgnj.s f0, f15, f16\n"
			"srl x27, x24, x14\n"
			"maskand m5, m6, m0\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_2_LOOP_SEQ_HID_12\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f30,  128 (x5)\n"
			"flw.ps f16,  992 (x5)\n"
			"flw.ps f8,  960 (x5)\n"
			"flw.ps f27,  864 (x5)\n"
			"flw.ps f20,  480 (x5)\n"
			"flw.ps f3,  96 (x5)\n"
			"flw.ps f26,  800 (x5)\n"
			"flw.ps f13,  0 (x5)\n"
			"flw.ps f4,  704 (x5)\n"
			"flw.ps f6,  320 (x5)\n"
			"flw.ps f18,  192 (x5)\n"
			"flw.ps f22,  544 (x5)\n"
			"flw.ps f12,  64 (x5)\n"
			"flw.ps f2,  576 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f9,  512 (x5)\n"
			"flw.ps f10,  608 (x5)\n"
			"flw.ps f25,  736 (x5)\n"
			"flw.ps f11,  672 (x5)\n"
			"flw.ps f15,  896 (x5)\n"
			"flw.ps f28,  256 (x5)\n"
			"flw.ps f17,  832 (x5)\n"
			"flw.ps f23,  352 (x5)\n"
			"flw.ps f1,  448 (x5)\n"
			"flw.ps f31,  640 (x5)\n"
			"flw.ps f21,  224 (x5)\n"
			"flw.ps f24,  384 (x5)\n"
			"flw.ps f14,  160 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f7,  928 (x5)\n"
			"flw.ps f5,  32 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_3_HID_12:\n"
			"li x27, 0x80a728f028\n"
			"la x7, mem_ptr_offset\n"
			"flw.ps f26, 640(x7)\n"
			"fgbg.ps f21, f26 (x27)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fmulh.pi f3, f27, f0\n"
			"bge x8, x27, 1f\n"
			"1:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f7, 128(x25)\n"
			"flw.ps f15, 128(x25)\n"
			"fsub.ps f31, f7, f15, rmm\n" 
			"fclass.s x7, f7\n"
			"li x24, 0xddbc178bf66cfcd8\n"
			"li x19, 0x63e129c8ed7eb706\n"
			"add x24, x24, x19\n" 
			"fcvt.ps.pw f31, f12, rmm\n"
			"bge x19, x1, 1f\n"
			"1:\n"
			"li x16, 0x80b26d3018\n"
			"la x25, mem_ptr_offset\n"
			"flw.ps f10, 352(x25)\n"
			"fscbl.ps f31, f10 (x16)\n" 
			"fsrl.pi f1, f1, f1\n"
			"beq x18, x16, 1f\n"
			"1:\n"
			"li x24, 0x80716ed940\n"
			"li x7, 3783398319\n"
			"fg32w.ps f7, x15 (x24)\n" 
			"li x5, 0x7b65\n"
			"csrw tensor_mask, x5\n"	
			"fcvt.pw.ps f28, f31, rup\n"
			"auipc x5, 0x14\n" 
			"li x5, 0xc2ec\n"
			"csrw tensor_mask, x5\n"	
			"fmsub.ps f28, f21, f28, f3, rtz\n"
			"li x15, 0x80a1da7000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f25, (x15)\n" 
			"fand.pi f28, f28, f27\n"
			"li x5, 0xe96f0e1fb6f01934\n"
			"sra x5, x5, x5\n" 
			"li x5, 0xed7d\n"
			"csrw tensor_mask, x5\n"	
			"frcp.ps f7, f29\n"
			"li x15, 0x80a668b018\n"
			"la x24, mem_ptr_offset\n"
			"flw.ps f12, 800(x24)\n"
			"famoandg.pi f29, f12 (x15)\n" 
			"fsgnjx.s f21, f25, f26\n"
			"beq x21, x7, 1f\n"
			"1:\n"
			"li x24, 0x8070fd0ee8\n"
			"la x16, mem_ptr_offset\n"
			"flw.ps f28, 736(x16)\n"
			"fgb.ps f10, f28 (x24)\n" 
			"fnmsub.s f10, f10, f3, f21, rmm\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f21, 96(x27)\n"
			"flw.ps f25, 576(x27)\n"
			"flw.ps f29, 352(x27)\n"
			"fmsub.s f14, f21, f25, f29, rmm\n" 
			"fpackreph.pi f25, f1\n"
			"li x19, 0x8072933d70\n"
			"li x16, 565433088\n"
			"fsc32h.ps f10, x7 (x19)\n" 
			"fnmsub.s f14, f14, f7, f21, rtz\n"
			"li x27, 0x80b51b5028\n"
			"amoswapl.d x16, x15, (x27)\n" 
			"srli x7, x16, 1\n"
			"li x28, 0x80a465d018\n"
			"la x5, mem_ptr_offset\n"
			"flw.ps f21, 672(x5)\n"
			"famoandg.pi f15, f21 (x28)\n" 
			"li x5, 0xfbddd5713b6b6f55\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xb920e6bfc2865d17\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"fmulhu.pi f7, f7, f7\n"
			"li x15, 0x80a180d038\n"
			"amoorg.d x26, x25, (x15)\n" 
			"li x5, 0xa349\n"
			"csrw tensor_mask, x5\n"	
			"fadd.pi f0, f12, f15\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f14, 960(x14)\n"
			"flt.ps f16, f14, f14\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"xor x7, x7, x26\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, 0x30800080700022c7\n"
			"li x6, 0x10008070000c03\n"
			"li x7, 0x1b980000011f041\n"
			"li x9, 0x1fbf200027676667\n"
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 2\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x80\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f1, 2(x31)\n"
			"flw.ps f12, 3(x31)\n"
			"flw.ps f15, 0(x31)\n"
			"flw.ps f16, 15(x31)\n"
			"flw.ps f21, 11(x31)\n"
			"flw.ps f25, 21(x31)\n"
			"flw.ps f26, 4(x31)\n"
			"flw.ps f27, 3(x31)\n"
			"flw.ps f31, 9(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_3_LOOP_SEQ_HID_12:\n"
			"fmin.pi f12, f12, f27\n"
			"fsgnjn.s f28, f0, f12\n"
			"fsub.s f31, f0, f1, rup\n"
			"fsra.pi f15, f31, f12\n"
			"fmul.pi f15, f12, f26\n"
			"fsatu8.pi f1, f12\n"
			"fmadd.s f31, f25, f25, f27, rne\n"
			"fcvt.f16.ps f21, f27\n"
			"fsgnjn.ps f16, f25, f16\n"
			"fmul.pi f25, f12, f31\n"
			"bne x11, x2, 1f\n"
			"1:\n"
			"fsrai.pi f25, f16, 0x0\n"
			"fmin.ps f15, f0, f15\n"
			"fnot.pi f28, f26\n"
			"fmul.pi f21, f21, f31\n"
			"fsgnjn.ps f26, f16, f1\n"
			"fcvt.pw.ps f12, f12, rtz\n"
			"bne x0, x23, 1f\n"
			"1:\n"
			"fsub.pi f31, f21, f15\n"
			"feq.pi f25, f15, f0\n"
			"bne x29, x19, 1f\n"
			"1:\n"
			"fpackreph.pi f31, f21\n"
			"fpackreph.pi f1, f1\n"
			"fexp.ps f31, f12\n"
			"fmadd.s f16, f25, f16, f27, rdn\n"
			"frcp.ps f12, f21\n"
			"fmax.ps f31, f21, f12\n"
			"fsrai.pi f7, f1, 0x0\n"
			"fsgnj.s f7, f1, f26\n"
			"fltu.pi f3, f12, f27\n"
			"bltu x11, x21, 1f\n"
			"1:\n"
			"fmax.pi f26, f26, f25\n"
			"blt x5, x18, 1f\n"
			"1:\n"
			"fcmov.ps f7, f16, f21, f26\n"
			"fsub.s f25, f15, f27, rmm\n"
			"fnmsub.ps f10, f15, f27, f25, rmm\n"
			"fmsub.s f26, f0, f21, f16, rdn\n"
			"fnmsub.s f15, f16, f15, f1, rmm\n"
			"fsgnj.ps f27, f26, f0\n"
			"fmsub.ps f28, f26, f0, f12, rne\n"
			"fmsub.s f28, f0, f15, f15, rdn\n"
			"fmax.pi f28, f25, f1\n"
			"beq x29, x13, 1f\n"
			"1:\n"
			"fminu.pi f3, f21, f0\n"
			"fcmov.ps f16, f21, f16, f27\n"
			"fsrli.pi f21, f16, 0xa\n"
			"fmax.ps f31, f31, f0\n"
			"fsat8.pi f21, f1\n"
			"fmin.s f12, f12, f21\n"
			"fsgnj.s f31, f31, f16\n"
			"flt.pi f16, f25, f16\n"
			"fand.pi f26, f0, f0\n"
			"bge x29, x27, 1f\n"
			"1:\n"
			"fsub.s f15, f25, f27, rdn\n"
			"fmaxu.pi f26, f31, f31\n"
			"fsatu8.pi f26, f25\n"
			"fmulhu.pi f7, f12, f16\n"
			"fcvt.pw.ps f7, f16, rdn\n"
			"fmax.ps f12, f1, f12\n"
			"fmsub.ps f1, f27, f1, f16, rne\n"
			"fadd.s f21, f16, f0, rup\n"
			"fsra.pi f27, f16, f12\n"
			"fsgnj.s f16, f16, f12\n"
			"bltu x6, x18, 1f\n"
			"1:\n"
			"fxor.pi f27, f21, f27\n"
			"flt.pi f25, f31, f0\n"
			"fmadd.ps f25, f25, f27, f12, rtz\n"
			"fadd.ps f25, f12, f25, rtz\n"
			"fsgnjx.s f15, f16, f15\n"
			"bge x19, x1, 1f\n"
			"1:\n"
			"fmin.s f25, f0, f15\n"
			"bne x27, x31, 1f\n"
			"1:\n"
			"fsgnjx.s f25, f25, f15\n"
			"fadd.s f26, f12, f15, rup\n"
			"beq x9, x13, 1f\n"
			"1:\n"
			"fsgnj.ps f12, f15, f12\n"
			"fexp.ps f31, f31\n"
			"fand.pi f27, f15, f25\n"
			"fpackrepb.pi f28, f12\n"
			"feq.pi f15, f26, f31\n"
			"fslli.pi f26, f25, 0x1\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_3_LOOP_SEQ_HID_12\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_4_HID_12:\n"
			"divw x28, x15, x15\n" 
			"addiw x16, x0, 0x3\n"
			"fmv.x.s x7, f28\n"
			"fadd.pi f31, f14, f12\n"
			"fmin.s f28, f15, f12\n"
			"rem x16, x16, x0\n"
			"sltu x16, x26, x16\n"
			"fcvt.s.w f14, x16, rne\n"
			"bgeu x7, x25, 1f\n"
			"1:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f1, 736(x7)\n"
			"fcvt.l.s x16, f1, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fmax.s f21, f31, f27\n"
			"fsgnjn.ps f26, f26, f14\n"
			"fslli.pi f27, f28, 0x7\n"
			"fxor.pi f14, f16, f1\n"
			"fsgnjn.ps f14, f12, f1\n"
			"bge x7, x21, 1f\n"
			"1:\n"
			"fsub.s f29, f16, f29, rne\n"
			"fpackreph.pi f0, f7\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f3, 832(x14)\n"
			"flw.ps f26, 192(x14)\n"
			"fle.ps f16, f3, f26\n" 
			"fmax.s f21, f21, f15\n"
			"fmin.s f15, f26, f12\n"
			"faddi.pi f14, f10, 0x56\n"
			"fbci.ps f15, 0x14525\n"
			"blt x5, x28, 1f\n"
			"1:\n"
			"lui x16, 0x12\n"
			"fnmadd.s f15, f25, f0, f0, rdn\n"
			"bgeu x6, x31, 1f\n"
			"1:\n"
			"fbcx.ps f21, x14\n"
			"bne x27, x28, 1f\n"
			"1:\n"
			"li x24, 0x8072ae8c18\n"
			"li x25, 1097685634\n"
			"fg32b.ps f15, x24 (x24)\n" 
			"fandi.pi f3, f31, 0x1ff\n"
			"beq x31, x6, 1f\n"
			"1:\n"
			"fandi.pi f31, f26, 0xb6\n"
			"fcvt.pw.ps f1, f21, rne\n"
			"fsgnjn.ps f21, f21, f26\n"
			"blt x22, x20, 1f\n"
			"1:\n"
			"fsat8.pi f31, f31\n"
			"fle.ps f31, f31, f31\n"
			"fsgnjx.s f12, f26, f26\n"
			"li x25, 0x80a040c000\n"
			"la x27, mem_ptr_offset\n"
			"flw.ps f28, 384(x27)\n"
			"famoandg.pi f12, f28 (x25)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fsetm.pi m7, f7\n"
			"fand.pi f12, f25, f25\n"
			"fround.ps f15, f21, rne\n"
			"fltm.pi m0, f21, f27\n"
			"mov.m.x m0, x0, 0xff\n"
			"fpackreph.pi f12, f12\n"
			"fle.s x16, f29, f7\n"
			"fbci.ps f31, 0x60d4a\n"
			"li x25, 0x80a2d0d000\n"
			"amoandg.w x19, x14, (x25)\n" 
			"fmax.ps f0, f28, f7\n"
			"fclass.ps f27, f3\n"
			"bne x4, x20, 1f\n"
			"1:\n"
			"fsub.ps f7, f10, f21, rne\n"
			"bgeu x19, x21, 1f\n"
			"1:\n"
			"fnmsub.ps f1, f27, f28, f10, rdn\n"
			"ffrc.ps f16, f28\n"
			"fsrli.pi f27, f31, 0x2\n"
			"fmaxu.pi f27, f3, f7\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f7, 800(x5)\n"
			"flw.ps f10, 64(x5)\n"
			"flw.ps f3, 96(x5)\n"
			"fnmadd.ps f27, f7, f10, f3, rup\n" 
			"slliw x0, x0, 5\n"
			"fminu.pi f25, f25, f10\n"
			"srai x0, x0, 1\n"
			"fmv.s.x f10, x16\n"
			"sll x16, x16, x14\n"
			"blt x21, x23, 1f\n"
			"1:\n"
			"fmvs.x.ps x16, f14, 2\n"
			"fence\n"
			"li x15, 0x80b6dc5018\n"
			"la x19, mem_ptr_offset\n"
			"flw.ps f25, 448(x19)\n"
			"fscbl.ps f25, f25 (x15)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fsub.ps f31, f31, f31, rmm\n"
			"fsgnjx.s f0, f15, f12\n"
			"fandi.pi f1, f12, 0x1da\n"
			"fsrai.pi f3, f31, 0x6\n"
			"fandi.pi f15, f12, 0x23\n"
			"fsrai.pi f16, f31, 0x6\n"
			"blt x25, x8, 1f\n"
			"1:\n"
			"fsra.pi f12, f12, f15\n"
			"la x24, rand_int_32\n"
			"flw.ps f15, 160(x24)\n"
			"flw.ps f26, 160(x24)\n"
			"fmaxu.pi f21, f15, f26\n" 
			"fsrli.pi f26, f1, 0x7\n"
			"sra x5, x25, x25\n"
			"fsub.s f1, f1, f0, rdn\n"
			"fmul.ps f12, f0, f12, rdn\n"
			"fxor.pi f3, f3, f12\n"
			"fsub.s f1, f3, f29, rtz\n"
			"fcvt.pwu.ps f12, f10, rtz\n"
			"lui x15, 0x1b\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"feqm.ps m0, f1, f26\n"
			"mov.m.x m0, x0, 0xff\n"
			"rem x25, x16, x19\n"
			"fand.pi f31, f28, f27\n"
			"fcmov.ps f7, f7, f10, f10\n"
			"fmvs.x.ps x27, f27, 7\n"
			"fnmadd.ps f26, f0, f25, f31, rdn\n"
			"bltu x15, x10, 1f\n"
			"1:\n"
			"fsgnj.ps f3, f3, f27\n"
			"li x24, 0x8071564118\n"
			"li x5, 872024862\n"
			"fsc32b.ps f29, x28 (x24)\n" 
			"fmul.s f28, f21, f28, rmm\n"
			"fround.ps f21, f29, rup\n"
			"fnmsub.ps f28, f16, f7, f10, rdn\n"
			"beq x6, x23, 1f\n"
			"1:\n"
			"fcvt.w.s x24, f28, rne\n"
			"sltu x14, x24, x14\n"
			"fmin.ps f16, f16, f16\n"
			"fnmadd.s f21, f27, f29, f14, rmm\n"
			"maskor m5, m5, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"sra x28, x28, x5\n"
			"packb x14, x14, x5\n"
			"fadd.pi f21, f27, f26\n"
			"fmaxu.pi f15, f12, f28\n"
			"bltu x25, x2, 1f\n"
			"1:\n"
			"sll x26, x14, x19\n"
			"beq x3, x24, 1f\n"
			"1:\n"
			"fminu.pi f16, f27, f21\n"
			"beq x24, x23, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f1, f29, rne\n"
			"li x7, 0x80a761d030\n"
			"amominug.w x28, x24, (x7)\n" 
			"fmax.s f28, f14, f29\n"
			"flt.ps f26, f29, f25\n"
			"frcp.ps f12, f29\n"
			"fxor.pi f3, f25, f28\n"
			"bne x12, x29, 1f\n"
			"1:\n"
			"fnmsub.ps f27, f7, f7, f15, rtz\n"
			"flt.ps f25, f7, f10\n"
			"fslli.pi f12, f25, 0x0\n"
			"li x28, 0x80a2ec6004\n"
			"amoorg.w x16, x24, (x28)\n" 
			"srli x14, x14, 2\n"
			"and x28, x7, x27\n"
			"srlw x15, x15, x16\n"
			"bgeu x25, x16, 1f\n"
			"1:\n"
			"fsetm.pi m0, f1\n"
			"fmul.s f21, f29, f26, rne\n"
			"fmul.ps f27, f15, f27, rne\n"
			"lui x16, 0x12\n"
			"li x15, 0x80a331402c\n"
			"amoaddg.w x27, x7, (x15)\n" 
			"feq.pi f15, f21, f21\n"
			"fsub.ps f15, f15, f28, rtz\n"
			"beq x8, x9, 1f\n"
			"1:\n"
			"fsub.ps f26, f31, f25, rmm\n"
			"blt x20, x29, 1f\n"
			"1:\n"
			"fsrli.pi f10, f3, 0x0\n"
			"fadd.pi f7, f27, f7\n"
			"fpackreph.pi f0, f21\n"
			"bgeu x18, x23, 1f\n"
			"1:\n"
			"fand.pi f0, f25, f25\n"
			"li x24, 0x8074c4311b\n"
			"sd x28, 512(x24)\n" 
			"fsll.pi f15, f1, f3\n"
			"subw x28, x24, x15\n"
			"bge x25, x24, 1f\n"
			"1:\n"
			"fpackreph.pi f26, f16\n"
			"bne x9, x0, 1f\n"
			"1:\n"
			"xori x28, x14, 0x10\n"
			"bne x11, x16, 1f\n"
			"1:\n"
			"fcvt.s.wu f29, x14, rup\n"
			"fmadd.ps f14, f3, f27, f25, rup\n"
			"feq.pi f12, f26, f10\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x6, 0x3928800667667667\n"
			"li x7, 0x3aa0008070004b40\n"
			"li x31, (0x80 << 4)\n"
			"csrw tensor_quant, x6\n"	
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_12:\n"
			"li x5, 0x9b4800000086a1\n"
			"li x6, 0x2a9f400000000002\n"
			"li x7, 0x9a8008070008f80\n"
			"li x31, (0x80 << 4)\n"
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrwi tensor_wait, 8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 2\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x28\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f14, 2(x31)\n"
			"flw.ps f15, 1(x31)\n"
			"flw.ps f21, 4(x31)\n"
			"flw.ps f25, 16(x31)\n"
			"flw.ps f26, 20(x31)\n"
			"flw.ps f28, 15(x31)\n"
			"flw.ps f31, 21(x31)\n"
			"flw.ps f7, 32(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_4_LOOP_SEQ_HID_12:\n"
			"fround.ps f31, f10, rne\n"
			"fnmsub.ps f26, f26, f25, f14, rne\n"
			"fmul.ps f26, f28, f7, rmm\n"
			"fsrli.pi f7, f21, 0xe\n"
			"fsgnjn.ps f31, f21, f31\n"
			"fmul.pi f7, f7, f15\n"
			"fsatu8.pi f31, f28\n"
			"fmadd.s f27, f14, f21, f14, rne\n"
			"fsat8.pi f7, f14\n"
			"beq x27, x29, 1f\n"
			"1:\n"
			"fmax.pi f3, f26, f28\n"
			"fsat8.pi f10, f10\n"
			"fcvt.pw.ps f7, f14, rdn\n"
			"fsub.pi f31, f14, f14\n"
			"fclass.ps f26, f26\n"
			"bge x8, x14, 1f\n"
			"1:\n"
			"fsgnjn.s f29, f7, f7\n"
			"fmul.ps f14, f14, f28, rne\n"
			"fnmadd.ps f31, f21, f15, f26, rne\n"
			"fpackrepb.pi f28, f7\n"
			"fsgnjx.ps f27, f31, f25\n"
			"fsra.pi f10, f14, f10\n"
			"fnmadd.ps f3, f28, f7, f21, rne\n"
			"feq.pi f29, f26, f7\n"
			"beq x14, x16, 1f\n"
			"1:\n"
			"fadd.ps f21, f10, f21, rup\n"
			"fmax.ps f31, f31, f14\n"
			"fmul.ps f29, f31, f15, rtz\n"
			"fsgnjx.s f28, f15, f31\n"
			"fround.ps f28, f31, rtz\n"
			"flog.ps f10, f25\n"
			"fsat8.pi f25, f25\n"
			"fandi.pi f14, f14, 0x23\n"
			"bgeu x26, x9, 1f\n"
			"1:\n"
			"fsra.pi f15, f26, f15\n"
			"fle.pi f25, f14, f28\n"
			"bgeu x18, x22, 1f\n"
			"1:\n"
			"fnmsub.ps f27, f10, f21, f14, rne\n"
			"fmulhu.pi f10, f25, f25\n"
			"fsgnjx.ps f21, f14, f14\n"
			"blt x8, x1, 1f\n"
			"1:\n"
			"fcvt.f16.ps f1, f31\n"
			"fsrai.pi f27, f21, 0x4\n"
			"fnmadd.s f10, f14, f26, f7, rdn\n"
			"fsub.pi f31, f28, f10\n"
			"bne x20, x24, 1f\n"
			"1:\n"
			"ffrc.ps f7, f25\n"
			"fsgnj.s f26, f26, f26\n"
			"bne x26, x25, 1f\n"
			"1:\n"
			"fsatu8.pi f1, f14\n"
			"for.pi f29, f21, f28\n"
			"fsgnjn.s f10, f10, f28\n"
			"fadd.s f26, f25, f26, rdn\n"
			"fmulh.pi f25, f25, f31\n"
			"fcvt.pwu.ps f25, f7, rmm\n"
			"fsgnjn.ps f25, f10, f28\n"
			"fmin.pi f26, f26, f26\n"
			"bgeu x15, x20, 1f\n"
			"1:\n"
			"fand.pi f27, f26, f15\n"
			"fmin.pi f10, f25, f26\n"
			"fcvt.ps.pw f21, f21, rmm\n"
			"for.pi f7, f26, f7\n"
			"fsrai.pi f26, f26, 0x0\n"
			"fle.pi f10, f7, f15\n"
			"flog.ps f7, f15\n"
			"fexp.ps f29, f26\n"
			"fadd.pi f27, f28, f14\n"
			"faddi.pi f29, f10, 0x1d7\n"
			"bltu x13, x26, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f3, f15\n"
			"bgeu x16, x9, 1f\n"
			"1:\n"
			"fmsub.ps f7, f28, f15, f26, rup\n"
			"fcvt.f16.ps f28, f31\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_4_LOOP_SEQ_HID_12\n"
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f26,  416 (x5)\n"
			"flw.ps f11,  672 (x5)\n"
			"flw.ps f5,  192 (x5)\n"
			"flw.ps f7,  864 (x5)\n"
			"flw.ps f3,  480 (x5)\n"
			"flw.ps f2,  768 (x5)\n"
			"flw.ps f17,  448 (x5)\n"
			"flw.ps f12,  128 (x5)\n"
			"flw.ps f18,  96 (x5)\n"
			"flw.ps f28,  608 (x5)\n"
			"flw.ps f9,  640 (x5)\n"
			"flw.ps f13,  288 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f8,  576 (x5)\n"
			"flw.ps f10,  960 (x5)\n"
			"flw.ps f4,  320 (x5)\n"
			"flw.ps f6,  256 (x5)\n"
			"flw.ps f21,  704 (x5)\n"
			"flw.ps f20,  896 (x5)\n"
			"flw.ps f31,  928 (x5)\n"
			"flw.ps f19,  64 (x5)\n"
			"flw.ps f0,  832 (x5)\n"
			"flw.ps f14,  224 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"flw.ps f30,  32 (x5)\n"
			"flw.ps f1,  384 (x5)\n"
			"flw.ps f22,  352 (x5)\n"
			"flw.ps f15,  544 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f27,  800 (x5)\n"
			"flw.ps f16,  0 (x5)\n"
			"la x31, check_sum_hid_12\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"LBL_POSTAMBLE_HID_12:\n"          
			"csrwi   tensor_wait, 0x0\n"
			"csrwi   tensor_wait, 0x1\n"
			"csrwi   tensor_wait, 0x2\n"
			"csrwi   tensor_wait, 0x3\n"
			"csrwi   tensor_wait, 0x4\n"
			"csrwi   tensor_wait, 0x5\n"
			"csrwi   tensor_wait, 0x6\n"
			"csrwi   tensor_wait, 0x7\n"
			"csrwi   tensor_wait, 0x8\n"
			"csrwi   tensor_wait, 0x9\n"
			"csrwi   tensor_wait, 0xa\n"
			"fence\n"
			"LBL_SYNC_POINT_7_HID_12:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_7_WAIT_HID_12\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_7_EXIT_HID_12\n"
			"LBL_SYNC_POINT_7_WAIT_HID_12:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_7_EXIT_HID_12:\n"          
			"nop\n"
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_12\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_12:\n"          
			".global LBL_FAIL_HID_12\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_12\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_12:\n"          
			".global LBL_PASS_HID_12\n"          
			VSNIP_RSV
		);
		c_test_pass();
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_12:\n"          
			".global LBL_HALT_HID_12\n"          
			"wfi\n"
			"j LBL_HALT_HID_12\n"          
			VSNIP_RSV
		);
		return 0;       
	};
	if ( hid == 13 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_13:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_13\n"
			"csrw mscratch, x5\n"
			"csrwi menable_shadows, 0xf\n"
			"li x5, -1\n"
			"csrw mcounteren, x5\n"
			"csrw scounteren, x5\n"
			"csrwi mhpmcounter3, 0\n"
			"csrwi mhpmcounter4, 0\n"
			"csrwi mhpmcounter5, 0\n"
			"csrwi mhpmcounter6, 0\n"
			"csrwi mhpmcounter7, 0\n"
			"csrwi mhpmcounter8, 0\n"
			"csrwi mhpmevent3, 7\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 9\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 23\n"   
			"mov.m.x m0, x0, 0xff\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f6,  32 (x5)\n"
			"flw.ps f5,  192 (x5)\n"
			"flw.ps f14,  96 (x5)\n"
			"flw.ps f31,  992 (x5)\n"
			"flw.ps f15,  128 (x5)\n"
			"flw.ps f28,  320 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f16,  640 (x5)\n"
			"flw.ps f7,  64 (x5)\n"
			"flw.ps f12,  704 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f13,  608 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f3,  224 (x5)\n"
			"flw.ps f9,  928 (x5)\n"
			"flw.ps f18,  576 (x5)\n"
			"flw.ps f24,  384 (x5)\n"
			"flw.ps f27,  544 (x5)\n"
			"flw.ps f4,  896 (x5)\n"
			"flw.ps f0,  800 (x5)\n"
			"flw.ps f26,  864 (x5)\n"
			"flw.ps f22,  288 (x5)\n"
			"flw.ps f21,  672 (x5)\n"
			"flw.ps f17,  512 (x5)\n"
			"flw.ps f1,  448 (x5)\n"
			"flw.ps f29,  416 (x5)\n"
			"flw.ps f8,  160 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f11,  352 (x5)\n"
			"flw.ps f23,  960 (x5)\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_HID_13:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_0_WAIT_HID_13\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_0_EXIT_HID_13\n"
			"LBL_SYNC_POINT_0_WAIT_HID_13:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_EXIT_HID_13:\n"          
			"nop\n"
			"LBL_KERNEL_HID_13:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_13\n"
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_HID_13:\n"
			"la x27, rand_int_32\n"
			"flw.ps f9, 256(x27)\n"
			"flw.ps f27, 800(x27)\n"
			"fmul.pi f20, f9, f27\n" 
			"faddi.pi f7, f15, 0x122\n"
			"li x19, 0x80aa2f5030\n"
			"amomaxug.d x27, x10, (x19)\n" 
			"fsra.pi f24, f15, f15\n"
			"li x15, 0x80adccc020\n"
			"amoxorg.d x22, x19, (x15)\n" 
			"fmulhu.pi f9, f28, f2\n"
			"li x29, 0x807e07eb57\n"
			"sd x6, -128(x29)\n"
			"lh x6, -128(x29)\n" 
			"fandi.pi f15, f24, 0x9c\n"
			"bne x30, x11, 1f\n"
			"1:\n"
			"csrr x0, mhpmcounter7\n"
			"masknot m2, m2\n"
			"li x15, 0x80ad0fb010\n"
			"la x26, mem_ptr_offset\n"
			"flw.ps f24, 480(x26)\n"
			"famoorg.pi f30, f24 (x15)\n" 
			"divuw x0, x0, x10\n"
			"beq x13, x29, 1f\n"
			"1:\n"
			"li x11, 0x80a97b4004\n"
			"amomaxug.w x10, x19, (x11)\n" 
			"fle.s x19, f21, f19\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f1, 320(x6)\n"
			"flw.ps f21, 288(x6)\n"
			"fmin.s f9, f1, f21\n" 
			"fle.s x15, f24, f28\n"
			"li x15, 0x80a9d7e038\n"
			"amomaxg.d x6, x26, (x15)\n" 
			"fsub.s f30, f30, f14, rdn\n"
			"la x10, rand_int_32\n"
			"flw.ps f19, 832(x10)\n"
			"fsatu8.pi f15, f19\n" 
			"maskpopc x27, m3\n"
			"li x15, 0x80adf9d020\n"
			"la x26, mem_ptr_offset\n"
			"flw.ps f30, 992(x26)\n"
			"famoxorg.pi f2, f30 (x15)\n" 
			"fcvt.f16.ps f27, f21\n"
			"bge x13, x30, 1f\n"
			"1:\n"
			"li x9, 0xdd3cd007d11a0862\n"
			"srli x14, x9, 3\n" 
			"fsrai.pi f18, f18, 0x9\n"
			"li x10, 0x80ae657018\n"
			"la x11, mem_ptr_offset\n"
			"flw.ps f2, 320(x11)\n"
			"fghg.ps f30, f2 (x10)\n" 
			"fsrl.pi f15, f28, f28\n"
			"csrr x0, gsc_progress\n"
			"fle.pi f14, f14, f1\n"
			"bltu x24, x8, 1f\n"
			"1:\n"
			"la x6, rand_int_32\n"
			"flw.ps f19, 928(x6)\n"
			"flw.ps f2, 256(x6)\n"
			"frem.pi f20, f19, f2\n" 
			"and x22, x14, x19\n"
			"li x22, 0x80adaa1000\n"
			"la x19, mem_ptr_offset\n"
			"flw.ps f7, 448(x19)\n"
			"fgbg.ps f15, f7 (x22)\n" 
			"fle.pi f19, f17, f17\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f14, 0(x31)\n"
			"flw.ps f15, 3(x31)\n"
			"flw.ps f18, 2(x31)\n"
			"flw.ps f19, 8(x31)\n"
			"flw.ps f20, 7(x31)\n"
			"flw.ps f24, 10(x31)\n"
			"flw.ps f27, 10(x31)\n"
			"flw.ps f28, 27(x31)\n"
			"flw.ps f7, 7(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x10, 2(x31)\n"
			"lw x11, 8(x31)\n"
			"lw x14, 2(x31)\n"
			"lw x15, 4(x31)\n"
			"lw x19, 10(x31)\n"
			"lw x26, 13(x31)\n"
			"lw x27, 27(x31)\n"
			"lw x29, 15(x31)\n"
			"lw x9, 10(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_13:\n"
			"ori x26, x29, 0x13\n"
			"slliw x26, x29, 3\n"
			"fcvt.s.wu f28, x11, rup\n"
			"fcmovm.ps f20, f20, f20\n"
			"fmvz.x.ps x22, f14, 5\n"
			"lui x26, 0x4\n"
			"fmin.s f14, f14, f28\n"
			"fnot.pi f18, f18\n"
			"fmin.pi f24, f19, f28\n"
			"fnot.pi f18, f7\n"
			"fle.s x14, f27, f27\n"
			"fsgnjx.ps f24, f27, f19\n"
			"blt x9, x19, 1f\n"
			"1:\n"
			"flt.pi f15, f28, f15\n"
			"for.pi f21, f18, f28\n"
			"fclass.s x15, f20\n"
			"mulhu x14, x14, x0\n"
			"bgeu x8, x29, 1f\n"
			"1:\n"
			"fclass.s x29, f14\n"
			"ecall\n"
			"maskpopcz x10, m7\n"
			"fltm.pi m2, f15, f27\n"
			"srai x27, x14, 4\n"
			"bne x16, x27, 1f\n"
			"1:\n"
			"or x14, x9, x10\n"
			"feq.pi f20, f20, f24\n"
			"sllw x15, x10, x15\n"
			"fandi.pi f30, f28, 0x1b9\n"
			"fcvt.ps.pw f2, f18, rdn\n"
			"fmul.ps f9, f28, f18, rdn\n"
			"fround.ps f9, f24, rtz\n"
			"fence\n"
			"fbci.pi f15, 0x6d4bb\n"
			"sub x9, x9, x9\n"
			"fmadd.s f14, f15, f24, f14, rne\n"
			"blt x26, x0, 1f\n"
			"1:\n"
			"fsub.ps f24, f24, f7, rup\n"
			"fnmadd.s f15, f7, f28, f28, rtz\n"
			"fandi.pi f9, f28, 0xdc\n"
			"bltu x5, x21, 1f\n"
			"1:\n"
			"fpackreph.pi f20, f14\n"
			"maskand m3, m3, m7\n"
			"fmin.ps f2, f15, f18\n"
			"fmv.x.s x14, f7\n"
			"fmin.s f21, f24, f7\n"
			"mul x19, x9, x19\n"
			"mulhsu x29, x29, x19\n"
			"bgeu x15, x21, 1f\n"
			"1:\n"
			"fadd.pi f9, f27, f18\n"
			"subw x0, x0, x14\n"
			"slliw x15, x10, 3\n"
			"flt.s x27, f27, f14\n"
			"fnmadd.ps f19, f14, f19, f7, rmm\n"
			"fmax.s f20, f20, f27\n"
			"addiw x15, x15, 0x6\n"
			"bgeu x4, x24, 1f\n"
			"1:\n"
			"sra x27, x10, x14\n"
			"div x15, x11, x15\n"
			"sltu x11, x27, x10\n"
			"fmsub.ps f15, f15, f20, f19, rmm\n"
			"ecall\n"
			"bltu x4, x16, 1f\n"
			"1:\n"
			"remuw x6, x0, x26\n"
			"fltm.pi m3, f24, f28\n"
			"fcmov.ps f15, f24, f24, f20\n"
			"andi x9, x29, 0x19\n"
			"slliw x6, x26, 0\n"
			"feq.s x19, f20, f27\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_0_LOOP_SEQ_HID_13\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_HID_13:\n"
			"li x10, 0x80b8620028\n"
			"la x29, mem_ptr_offset\n"
			"flw.ps f21, 608(x29)\n"
			"fschl.ps f15, f21 (x10)\n" 
			"fsll.pi f28, f1, f28\n"
			"bgeu x1, x9, 1f\n"
			"1:\n"
			"fmulh.pi f9, f20, f1\n"
			"fcvt.ps.pwu f1, f1, rne\n"
			"fmulhu.pi f9, f1, f21\n"
			"fcvt.ps.pwu f28, f1, rup\n"
			"fsatu8.pi f15, f1\n"
			"bltu x28, x10, 1f\n"
			"1:\n"
			"flog.ps f1, f1\n"
			"fexp.ps f20, f1\n"
			"fadd.s f1, f9, f9, rdn\n"
			"li x11, 0x80adafd038\n"
			"amoandg.d x22, x9, (x11)\n" 
			"fslli.pi f21, f30, 0xa\n"
			"fadd.ps f20, f21, f30, rdn\n"
			"fsrai.pi f18, f17, 0x5\n"
			"fandi.pi f27, f17, 0x1cd\n"
			"fsub.s f7, f14, f28, rne\n"
			"fmul.s f1, f7, f30, rdn\n"
			"fsat8.pi f17, f30\n"
			"fmax.ps f7, f18, f18\n"
			"fsgnjn.ps f17, f2, f9\n"
			"bltu x20, x3, 1f\n"
			"1:\n"
			"la x19, rand_int_32\n"
			"flw.ps f7, 320(x19)\n"
			"flw.ps f2, 800(x19)\n"
			"fand.pi f30, f7, f2\n" 
			"ebreak\n"
			"slliw x10, x9, 5\n"
			"fmv.s.x f19, x11\n"
			"fcvt.wu.s x15, f14, rtz\n"
			"fmul.s f24, f18, f1, rdn\n"
			"bltu x8, x9, 1f\n"
			"1:\n"
			"subw x15, x6, x15\n"
			"maskpopc x11, m2\n"
			"fmax.ps f18, f19, f19\n"
			"blt x12, x31, 1f\n"
			"1:\n"
			"srliw x15, x22, 5\n"
			"li x27, 0x80a98dc014\n"
			"amomaxg.w x27, x10, (x27)\n" 
			"fpackrepb.pi f28, f2\n"
			"fltm.ps m7, f15, f27\n"
			"fcvt.w.s x11, f19, rtz\n"
			"fadd.s f19, f19, f27, rmm\n"
			"fsgnjx.s f7, f9, f7\n"
			"flt.s x29, f20, f14\n"
			"fnmadd.ps f2, f14, f24, f24, rne\n"
			"fsatu8.pi f24, f14\n"
			"ffrc.ps f20, f27\n"
			"li x19, 0x80ba8d8018\n"
			"amomaxul.d x29, x6, (x19)\n" 
			"frcp.ps f13, f7\n"
			"feq.pi f2, f2, f9\n"
			"fsgnjx.s f7, f9, f7\n"
			"fcvt.ps.pwu f13, f15, rne\n"
			"fnmsub.ps f21, f18, f21, f19, rdn\n"
			"blt x5, x14, 1f\n"
			"1:\n"
			"fmin.ps f30, f17, f18\n"
			"for.pi f27, f21, f18\n"
			"fnmsub.ps f7, f15, f27, f7, rdn\n"
			"blt x7, x13, 1f\n"
			"1:\n"
			"fcmovm.ps f9, f9, f14\n"
			"li x22, 0x80bfb3e024\n"
			"amominul.w x27, x27, (x22)\n" 
			"fmvz.x.ps x22, f14, 7\n"
			"beq x16, x8, 1f\n"
			"1:\n"
			"fcvt.w.s x11, f19, rne\n"
			"sllw x15, x26, x15\n"
			"fexp.ps f7, f13\n"
			"bltu x20, x26, 1f\n"
			"1:\n"
			"fcvt.ps.pw f24, f7, rtz\n"
			"fmul.ps f1, f1, f24, rdn\n"
			"sub x9, x9, x9\n"
			"fnot.pi f9, f17\n"
			"fxor.pi f13, f2, f7\n"
			"la x11, rand_int_32\n"
			"flw.ps f28, 800(x11)\n"
			"flw.ps f9, 736(x11)\n"
			"fxor.pi f27, f28, f9\n" 
			"fadd.pi f18, f18, f15\n"
			"bge x13, x5, 1f\n"
			"1:\n"
			"addw x14, x14, x26\n"
			"remw x29, x14, x26\n"
			"fsgnjx.s f2, f30, f1\n"
			"fsll.pi f30, f30, f14\n"
			"fmin.ps f18, f30, f1\n"
			"fle.s x6, f30, f18\n"
			"ecall\n"
			"fmin.s f18, f15, f1\n"
			"li x19, 0x807d16977e\n"
			"sw x14, 512(x19)\n" 
			"fnmsub.ps f24, f24, f24, f20, rne\n"
			"fexp.ps f24, f28\n"
			"flt.ps f27, f24, f28\n"
			"flog.ps f20, f20\n"
			"feq.pi f13, f24, f28\n"
			"bltu x24, x25, 1f\n"
			"1:\n"
			"for.pi f13, f20, f24\n"
			"fle.pi f13, f24, f24\n"
			"fsub.s f24, f20, f20, rne\n"
			"fsgnjn.s f18, f28, f28\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f18, 640(x15)\n"
			"flw.ps f20, 416(x15)\n"
			"feq.s x29, f18, f20\n" 
			"fmul.s f24, f28, f21, rne\n"
			"fexp.ps f28, f24\n"
			"bltu x19, x22, 1f\n"
			"1:\n"
			"fmin.pi f28, f21, f9\n"
			"fnmsub.s f28, f15, f30, f7, rup\n"
			"fsgnj.s f18, f28, f18\n"
			"fmax.s f7, f17, f30\n"
			"for.pi f17, f20, f30\n"
			"fnmadd.s f7, f30, f15, f17, rtz\n"
			"fnmadd.s f14, f1, f24, f15, rup\n"
			"li x29, 0x80b9761020\n"
			"la x19, mem_ptr_offset\n"
			"flw.ps f28, 384(x19)\n"
			"famoxorl.pi f28, f28 (x29)\n" 
			"feq.ps f1, f30, f27\n"
			"fle.ps f21, f30, f27\n"
			"fmadd.s f24, f24, f30, f27, rne\n"
			"fcvt.ps.f16 f1, f27\n"
			"beq x9, x12, 1f\n"
			"1:\n"
			"fmadd.ps f21, f24, f30, f21, rne\n"
			"fslli.pi f24, f30, 0x2\n"
			"bgeu x31, x24, 1f\n"
			"1:\n"
			"fnmsub.s f21, f24, f21, f21, rdn\n"
			"fmul.s f24, f27, f21, rdn\n"
			"fmin.s f21, f21, f30\n"
			"li x27, 0x807ccb6038\n"
			"la x9, mem_ptr_offset\n"
			"flw.ps f30, 288(x9)\n"
			"fgh.ps f17, f30 (x27)\n" 
			"for.pi f24, f15, f30\n"
			"fmax.s f30, f28, f7\n"
			"fsgnjn.s f30, f30, f7\n"
			"add x26, x26, x14\n"
			"maskpopcz x11, m4\n"
			"div x22, x26, x22\n"
			"fmvz.x.ps x0, f20, 0\n"
			"bgeu x27, x1, 1f\n"
			"1:\n"
			"frcp.ps f20, f28\n"
			"ecall\n"
			"blt x0, x1, 1f\n"
			"1:\n"
			"li x22, 0x80be4e4038\n"
			"amoorl.d x22, x15, (x22)\n" 
			"fsgnjx.s f24, f30, f24\n"
			"bgeu x11, x12, 1f\n"
			"1:\n"
			"ffrc.ps f27, f15\n"
			"flt.ps f21, f27, f7\n"
			"fmax.ps f28, f28, f18\n"
			"fmaxu.pi f19, f2, f24\n"
			"fsgnjn.ps f19, f15, f28\n"
			"fsgnj.ps f27, f27, f14\n"
			"fsgnjx.ps f2, f2, f20\n"
			"bne x9, x26, 1f\n"
			"1:\n"
			"fandi.pi f15, f27, 0x193\n"
			"bge x4, x15, 1f\n"
			"1:\n"
			"li x10, 0x8078485158\n"
			"fsw f19, -256(x10)\n" 
			"fandi.pi f30, f30, 0x108\n"
			"bne x24, x29, 1f\n"
			"1:\n"
			"fmaxu.pi f13, f27, f7\n"
			"fsrli.pi f30, f30, 0x3\n"
			"fle.pi f2, f7, f7\n"
			"fcvt.f16.ps f19, f27\n"
			"fsgnjx.ps f7, f7, f30\n"
			"fsgnj.s f19, f30, f30\n"
			"flt.ps f13, f30, f30\n"
			"fnmsub.s f13, f27, f27, f7, rtz\n"
			"li x26, 0x807b2e2e82\n"
			"sh x9, 256(x26)\n" 
			"fcvt.pw.ps f19, f13, rtz\n"
			"fnmsub.s f24, f2, f1, f24, rmm\n"
			"bne x26, x22, 1f\n"
			"1:\n"
			"fsgnj.s f9, f17, f30\n"
			"fround.ps f13, f14, rtz\n"
			"fmax.ps f27, f20, f30\n"
			"bltu x24, x24, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f15, f18, rne\n"
			"fnmsub.ps f24, f28, f30, f19, rmm\n"
			"fsub.ps f13, f18, f21, rup\n"
			"fpackreph.pi f24, f2\n"
			"bge x6, x3, 1f\n"
			"1:\n"
			"la x29, rand_ieee754_32\n"
			"flw.ps f24, 256(x29)\n"
			"flw.ps f1, 832(x29)\n"
			"fsgnj.s f1, f24, f1\n" 
			"fsgnjn.ps f27, f27, f18\n"
			"fcvt.ps.f16 f27, f18\n"
			"fsgnjn.ps f18, f20, f1\n"
			"fround.ps f18, f13, rdn\n"
			"fmul.ps f18, f13, f1, rup\n"
			"fadd.pi f9, f15, f30\n"
			"beq x24, x19, 1f\n"
			"1:\n"
			"fmul.ps f20, f19, f21, rne\n"
			"fnot.pi f15, f30\n"
			"fle.pi f18, f7, f13\n"
			"bge x27, x9, 1f\n"
			"1:\n"
			"li x19, 0x80be6f1020\n"
			"la x14, mem_ptr_offset\n"
			"flw.ps f7, 736(x14)\n"
			"famoswapl.pi f17, f7 (x19)\n" 
			"fcvt.pw.ps f9, f2, rdn\n"
			"fbci.pi f15, 0x7bfdd\n"
			"slli x19, x19, 1\n"
			"fslli.pi f13, f21, 0xb\n"
			"mulw x27, x0, x6\n"
			"feq.s x19, f20, f1\n"
			"fmulh.pi f1, f1, f9\n"
			"bne x28, x25, 1f\n"
			"1:\n"
			"fmaxu.pi f28, f20, f21\n"
			"remu x6, x15, x22\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f13, 0(x31)\n"
			"flw.ps f14, 4(x31)\n"
			"flw.ps f15, 9(x31)\n"
			"flw.ps f17, 6(x31)\n"
			"flw.ps f18, 11(x31)\n"
			"flw.ps f19, 0(x31)\n"
			"flw.ps f2, 13(x31)\n"
			"flw.ps f20, 12(x31)\n"
			"flw.ps f21, 14(x31)\n"
			"flw.ps f24, 37(x31)\n"
			"flw.ps f27, 41(x31)\n"
			"flw.ps f28, 32(x31)\n"
			"flw.ps f30, 15(x31)\n"
			"flw.ps f7, 37(x31)\n"
			"flw.ps f9, 51(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x10, 2(x31)\n"
			"lw x11, 2(x31)\n"
			"lw x14, 12(x31)\n"
			"lw x15, 5(x31)\n"
			"lw x19, 4(x31)\n"
			"lw x22, 15(x31)\n"
			"lw x26, 7(x31)\n"
			"lw x27, 25(x31)\n"
			"lw x29, 16(x31)\n"
			"lw x6, 25(x31)\n"
			"lw x9, 29(x31)\n"
			"li x31, 0x2\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_13:\n"
			"fnmsub.ps f2, f18, f21, f1, rmm\n"
			"fcmov.ps f9, f2, f14, f28\n"
			"lui x15, 0x5\n"
			"fcvt.wu.s x19, f28, rmm\n"
			"packb x9, x14, x14\n"
			"fmin.ps f18, f24, f18\n"
			"sraiw x22, x27, 4\n"
			"and x9, x29, x26\n"
			"fpackrepb.pi f28, f13\n"
			"fmaxu.pi f2, f9, f2\n"
			"mov.m.x m3, x15, 0xb\n"
			"and x29, x9, x11\n"
			"fadd.ps f20, f15, f15, rtz\n"
			"fmvs.x.ps x26, f27, 5\n"
			"feqm.ps m3, f9, f14\n"
			"flem.ps m2, f14, f27\n"
			"sltiu x10, x19, 0x1e\n"
			"fcvt.ps.f16 f17, f18\n"
			"addi x26, x10, 0x14\n"
			"fsll.pi f18, f24, f21\n"
			"fandi.pi f27, f15, 0x1a0\n"
			"mova.m.x x10\n"
			"frcp.ps f9, f9\n"
			"blt x31, x20, 1f\n"
			"1:\n"
			"fmul.ps f1, f1, f30, rtz\n"
			"fmulhu.pi f1, f9, f1\n"
			"fpackreph.pi f18, f7\n"
			"andi x11, x10, 0xe\n"
			"for.pi f17, f20, f19\n"
			"maskpopcz x15, m3\n"
			"faddi.pi f1, f1, 0x2\n"
			"flt.ps f30, f7, f30\n"
			"fsra.pi f1, f20, f9\n"
			"andi x19, x0, 0x1f\n"
			"fmax.ps f30, f30, f15\n"
			"srai x27, x0, 5\n"
			"fle.pi f18, f7, f17\n"
			"packb x9, x22, x22\n"
			"masknot m4, m3\n"
			"sltiu x0, x0, 0x15\n"
			"maskxor m3, m4, m7\n"
			"sub x19, x15, x22\n"
			"mul x9, x9, x6\n"
			"fle.s x27, f20, f21\n"
			"bge x0, x27, 1f\n"
			"1:\n"
			"ffrc.ps f30, f28\n"
			"fcvt.pw.ps f15, f28, rup\n"
			"for.pi f27, f27, f15\n"
			"add x22, x29, x15\n"
			"or x11, x11, x6\n"
			"packb x26, x14, x26\n"
			"fcmovm.ps f15, f9, f1\n"
			"fmul.s f13, f15, f17, rmm\n"
			"srl x29, x26, x19\n"
			"fnmadd.ps f1, f20, f19, f27, rne\n"
			"sltiu x26, x22, 0x1b\n"
			"mov.m.x m2, x11, 0x7f\n"
			"fmul.pi f9, f9, f17\n"
			"fsrli.pi f2, f7, 0x6\n"
			"srlw x29, x29, x29\n"
			"fsra.pi f27, f18, f9\n"
			"beq x9, x24, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f15, f2, rdn\n"
			"fcvt.pw.ps f28, f27, rtz\n"
			"fmsub.s f13, f30, f18, f7, rmm\n"
			"bge x20, x8, 1f\n"
			"1:\n"
			"ori x11, x11, 0x2\n"
			"fand.pi f2, f17, f19\n"
			"maskor m7, m7, m7\n"
			"blt x30, x17, 1f\n"
			"1:\n"
			"fcvt.f16.ps f20, f9\n"
			"fnmadd.s f21, f28, f14, f9, rne\n"
			"bgeu x8, x22, 1f\n"
			"1:\n"
			"sraw x6, x6, x26\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_1_LOOP_SEQ_HID_13\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_HID_13:\n"
			"li x26, 0x80ae4e9030\n"
			"amomaxg.d x9, x22, (x26)\n" 
			"fle.pi f1, f14, f15\n"
			"fle.ps f1, f15, f7\n"
			"fswizz.ps f30, f17, 0x9\n"
			"fmv.s.x f28, x14\n"
			"bne x7, x0, 1f\n"
			"1:\n"
			"fmv.s.x f27, x15\n"
			"slli x10, x29, 0\n"
			"li x15, 0x80ae516008\n"
			"la x14, mem_ptr_offset\n"
			"flw.ps f13, 256(x14)\n"
			"famoming.pi f18, f13 (x15)\n" 
			"fsub.s f13, f28, f30, rmm\n"
			"fswizz.ps f20, f7, 0x3\n"
			"bge x2, x10, 1f\n"
			"1:\n"
			"fle.ps f19, f17, f17\n"
			"fmin.s f19, f1, f19\n"
			"bgeu x30, x1, 1f\n"
			"1:\n"
			"faddi.pi f14, f21, 0x13d\n"
			"fmadd.ps f27, f17, f14, f1, rup\n"
			"la x22, rand_int_32\n"
			"flw.ps f30, 64(x22)\n"
			"fpackreph.pi f17, f30\n" 
			"ori x15, x15, 0x1c\n"
			"srli x15, x15, 4\n"
			"fswizz.ps f17, f17, 0x15\n"
			"srai x27, x29, 0\n"
			"slliw x19, x0, 1\n"
			"fnmsub.ps f20, f1, f24, f19, rdn\n"
			"li x6, 0x1721c92581477222\n"
			"addiw x11, x6, 0x0\n" 
			"ecall\n"
			"feqm.ps m3, f20, f24\n"
			"ori x19, x15, 0xf\n"
			"fmsub.s f27, f7, f18, f24, rtz\n"
			"bge x28, x7, 1f\n"
			"1:\n"
			"masknot m4, m4\n"
			"sraiw x19, x9, 1\n"
			"li x22, 0x807edb5b69\n"
			"sb x29, 256(x22)\n" 
			"fcvt.pwu.ps f27, f30, rtz\n"
			"fmaxu.pi f19, f15, f19\n"
			"fcvt.f16.ps f27, f1\n"
			"fsra.pi f20, f27, f19\n"
			"ffrc.ps f1, f9\n"
			"fswizz.ps f28, f15, 0x11\n"
			"li x14, 0x80ae7fb018\n"
			"la x6, mem_ptr_offset\n"
			"flw.ps f24, 96(x6)\n"
			"famoming.ps f21, f24 (x14)\n" 
			"fand.pi f24, f2, f30\n"
			"fand.pi f24, f30, f13\n"
			"fsgnjx.ps f2, f9, f9\n"
			"fsgnjn.ps f24, f9, f9\n"
			"bne x23, x14, 1f\n"
			"1:\n"
			"fsrli.pi f2, f30, 0x1\n"
			"fmul.s f24, f13, f30, rtz\n"
			"li x10, 0x807b5a5230\n"
			"fsw.ps f27, 128(x10)\n" 
			"fsub.s f20, f21, f13, rmm\n"
			"for.pi f18, f24, f18\n"
			"fmul.s f19, f19, f24, rup\n"
			"fcmov.ps f13, f13, f13, f28\n"
			"fsgnjx.ps f21, f21, f21\n"
			"fminu.pi f28, f28, f18\n"
			"li x15, 0xb45f05dfbbd00d15\n"
			"fcvt.s.w f17, x15, rtz\n" 
			"addi x26, x26, 0x1b\n"
			"frcp.ps f21, f27\n"
			"maskpopc x10, m2\n"
			"bne x20, x23, 1f\n"
			"1:\n"
			"auipc x26, 0x6\n"
			"fslli.pi f19, f18, 0xe\n"
			"and x6, x0, x6\n"
			"li x6, 0x807c8d1710\n"
			"li x27, 3118095407\n"
			"fg32b.ps f1, x26 (x6)\n" 
			"fadd.ps f13, f27, f24, rtz\n"
			"flog.ps f19, f7\n"
			"blt x11, x11, 1f\n"
			"1:\n"
			"fsrli.pi f19, f20, 0x7\n"
			"fcvt.f16.ps f21, f21\n"
			"flt.pi f30, f2, f20\n"
			"beq x17, x9, 1f\n"
			"1:\n"
			"fcvt.pw.ps f19, f21, rmm\n"
			"maskand m2, m7, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fand.pi f9, f20, f13\n"
			"fmul.s f24, f30, f20, rmm\n"
			"fmin.pi f28, f1, f1\n"
			"fltm.ps m2, f18, f20\n"
			"beq x31, x5, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f21, f2, rtz\n"
			"fmul.ps f20, f20, f18, rdn\n"
			"li x19, 0x80bf88d008\n"
			"amomaxul.d x19, x19, (x19)\n" 
			"fandi.pi f2, f9, 0x154\n"
			"blt x16, x17, 1f\n"
			"1:\n"
			"fmul.s f20, f20, f20, rne\n"
			"fnmsub.ps f24, f9, f27, f24, rdn\n"
			"bgeu x30, x8, 1f\n"
			"1:\n"
			"fmadd.s f15, f27, f15, f7, rne\n"
			"fcvt.ps.pw f2, f21, rdn\n"
			"fmulhu.pi f24, f19, f24\n"
			"bne x30, x3, 1f\n"
			"1:\n"
			"li x22, 0xe4b82f96572f59d8\n"
			"li x26, 0xc771331fe9af06d5\n"
			"remuw x10, x22, x26\n" 
			"fcmovm.ps f9, f21, f19\n"
			"fmulhu.pi f15, f21, f19\n"
			"fle.pi f19, f19, f21\n"
			"fle.ps f15, f17, f17\n"
			"fsatu8.pi f7, f19\n"
			"blt x12, x1, 1f\n"
			"1:\n"
			"fsgnjx.ps f21, f21, f13\n"
			"li x15, 0x80af676030\n"
			"la x27, mem_ptr_offset\n"
			"flw.ps f21, 928(x27)\n"
			"fghg.ps f21, f21 (x15)\n" 
			"flt.ps f24, f18, f15\n"
			"fmin.ps f27, f13, f27\n"
			"fnmsub.s f17, f19, f2, f2, rup\n"
			"fxor.pi f9, f9, f13\n"
			"bge x4, x13, 1f\n"
			"1:\n"
			"fcvt.f16.ps f1, f19\n"
			"fmin.ps f9, f1, f9\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f28, 0(x11)\n"
			"flw.ps f20, 320(x11)\n"
			"fmin.s f21, f28, f20\n" 
			"fmin.pi f2, f7, f28\n"
			"bgeu x22, x4, 1f\n"
			"1:\n"
			"fmaxu.pi f9, f9, f9\n"
			"fminu.pi f15, f19, f15\n"
			"bltu x4, x14, 1f\n"
			"1:\n"
			"slli x26, x26, 1\n"
			"and x29, x6, x14\n"
			"bne x29, x18, 1f\n"
			"1:\n"
			"fandi.pi f17, f15, 0xc3\n"
			"maskpopc x11, m4\n" 
			"fminu.pi f18, f7, f21\n"
			"fmul.s f27, f18, f2, rne\n"
			"fnmsub.s f27, f18, f1, f20, rmm\n"
			"mulh x6, x15, x10\n"
			"fslli.pi f1, f2, 0xa\n"
			"beq x1, x15, 1f\n"
			"1:\n"
			"fsgnjx.s f27, f27, f7\n"
			"li x27, 0x80adce9008\n"
			"amoorg.w x11, x10, (x27)\n" 
			"fmin.ps f21, f9, f2\n"
			"bgeu x5, x13, 1f\n"
			"1:\n"
			"fround.ps f19, f1, rup\n"
			"fclass.ps f1, f2\n"
			"fandi.pi f15, f2, 0x1ef\n"
			"fsll.pi f9, f13, f7\n"
			"blt x15, x31, 1f\n"
			"1:\n"
			"fand.pi f17, f27, f17\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x18\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f13, 1(x31)\n"
			"flw.ps f14, 1(x31)\n"
			"flw.ps f15, 10(x31)\n"
			"flw.ps f17, 3(x31)\n"
			"flw.ps f18, 2(x31)\n"
			"flw.ps f19, 10(x31)\n"
			"flw.ps f2, 6(x31)\n"
			"flw.ps f20, 14(x31)\n"
			"flw.ps f21, 36(x31)\n"
			"flw.ps f24, 18(x31)\n"
			"flw.ps f27, 42(x31)\n"
			"flw.ps f28, 43(x31)\n"
			"flw.ps f30, 25(x31)\n"
			"flw.ps f7, 55(x31)\n"
			"flw.ps f9, 12(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x10, 3(x31)\n"
			"lw x11, 8(x31)\n"
			"lw x14, 7(x31)\n"
			"lw x15, 5(x31)\n"
			"lw x19, 2(x31)\n"
			"lw x26, 13(x31)\n"
			"lw x27, 25(x31)\n"
			"lw x29, 17(x31)\n"
			"lw x6, 23(x31)\n"
			"lw x9, 39(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_13:\n"
			"fnmadd.ps f20, f15, f1, f14, rmm\n"
			"fcvt.f16.ps f21, f2\n"
			"mov.m.x m7, x6, 0x30\n"
			"fle.pi f28, f15, f17\n"
			"faddi.pi f24, f9, 0x16\n"
			"sra x14, x14, x27\n"
			"slti x22, x9, 0x1\n"
			"fbci.pi f18, 0x66e8f\n"
			"auipc x11, 0x13\n"
			"sllw x0, x14, x14\n"
			"fmaxu.pi f13, f14, f19\n"
			"bgeu x23, x3, 1f\n"
			"1:\n"
			"fandi.pi f30, f30, 0x175\n"
			"fclass.ps f24, f2\n"
			"bne x6, x28, 1f\n"
			"1:\n"
			"fmulh.pi f21, f28, f15\n"
			"remu x14, x29, x11\n"
			"bne x24, x25, 1f\n"
			"1:\n"
			"ffrc.ps f27, f1\n"
			"fmin.ps f13, f1, f21\n"
			"fmul.s f18, f13, f13, rup\n"
			"fandi.pi f1, f1, 0xc5\n"
			"fcvt.wu.s x0, f30, rup\n"
			"beq x19, x7, 1f\n"
			"1:\n"
			"feq.ps f20, f13, f14\n"
			"fmsub.s f17, f13, f2, f1, rmm\n"
			"rem x6, x6, x19\n"
			"fmvz.x.ps x11, f21, 7\n"
			"fadd.ps f27, f27, f2, rne\n"
			"fadd.pi f24, f13, f20\n"
			"or x14, x26, x10\n"
			"fnmsub.ps f19, f19, f15, f2, rne\n"
			"fsgnj.ps f28, f17, f30\n"
			"fnmadd.ps f19, f30, f18, f28, rdn\n"
			"bltu x7, x0, 1f\n"
			"1:\n"
			"fmsub.ps f15, f30, f9, f15, rtz\n"
			"frcp.ps f30, f21\n"
			"flog.ps f1, f9\n"
			"fmax.pi f18, f9, f18\n"
			"sraiw x29, x15, 5\n"
			"mov.m.x m3, x0, 0x22\n"
			"fcmov.ps f7, f17, f7, f24\n"
			"subw x9, x9, x11\n"
			"bge x25, x29, 1f\n"
			"1:\n"
			"fcvt.ps.pw f1, f2, rtz\n"
			"fmsub.ps f28, f13, f7, f1, rmm\n"
			"fmvz.x.ps x0, f17, 7\n"
			"packb x22, x27, x11\n"
			"fround.ps f14, f2, rne\n"
			"bne x13, x20, 1f\n"
			"1:\n"
			"sub x11, x11, x9\n"
			"srli x27, x26, 1\n"
			"fcvt.wu.s x9, f17, rne\n"
			"fmax.pi f24, f21, f9\n"
			"masknot m7, m7\n"
			"srli x6, x6, 1\n"
			"bne x0, x15, 1f\n"
			"1:\n"
			"fclass.ps f19, f15\n"
			"fle.pi f27, f21, f30\n"
			"mov.m.x m2, x15, 0x7c\n"
			"fsll.pi f2, f2, f9\n"
			"mulh x9, x11, x9\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_2_LOOP_SEQ_HID_13\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f3,  832 (x5)\n"
			"flw.ps f22,  0 (x5)\n"
			"flw.ps f4,  384 (x5)\n"
			"flw.ps f28,  288 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f25,  224 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
			"flw.ps f23,  480 (x5)\n"
			"flw.ps f10,  96 (x5)\n"
			"flw.ps f17,  896 (x5)\n"
			"flw.ps f18,  320 (x5)\n"
			"flw.ps f20,  544 (x5)\n"
			"flw.ps f31,  928 (x5)\n"
			"flw.ps f0,  800 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f1,  448 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f6,  192 (x5)\n"
			"flw.ps f16,  128 (x5)\n"
			"flw.ps f13,  32 (x5)\n"
			"flw.ps f2,  512 (x5)\n"
			"flw.ps f14,  416 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
			"flw.ps f24,  992 (x5)\n"
			"flw.ps f9,  864 (x5)\n"
			"flw.ps f21,  736 (x5)\n"
			"flw.ps f5,  160 (x5)\n"
			"flw.ps f30,  608 (x5)\n"
			"flw.ps f26,  704 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f12,  768 (x5)\n"
			"flw.ps f19,  640 (x5)\n"
			"la x31, check_sum_hid_13\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"LBL_POSTAMBLE_HID_13:\n"          
			"csrwi   tensor_wait, 0x0\n"
			"csrwi   tensor_wait, 0x1\n"
			"csrwi   tensor_wait, 0x2\n"
			"csrwi   tensor_wait, 0x3\n"
			"csrwi   tensor_wait, 0x4\n"
			"csrwi   tensor_wait, 0x5\n"
			"csrwi   tensor_wait, 0x6\n"
			"csrwi   tensor_wait, 0x7\n"
			"csrwi   tensor_wait, 0x8\n"
			"csrwi   tensor_wait, 0x9\n"
			"csrwi   tensor_wait, 0xa\n"
			"fence\n"
			"LBL_SYNC_POINT_1_HID_13:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_1_WAIT_HID_13\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_1_EXIT_HID_13\n"
			"LBL_SYNC_POINT_1_WAIT_HID_13:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_1_EXIT_HID_13:\n"          
			"nop\n"
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_13\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_13:\n"          
			".global LBL_FAIL_HID_13\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_13\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_13:\n"          
			".global LBL_PASS_HID_13\n"          
			VSNIP_RSV
		);
		c_test_pass();
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_13:\n"          
			".global LBL_HALT_HID_13\n"          
			"wfi\n"
			"j LBL_HALT_HID_13\n"          
			VSNIP_RSV
		);
		return 0;       
	};
	if ( hid == 14 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_14:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_14\n"
			"csrw mscratch, x5\n"
			"csrwi menable_shadows, 0xf\n"
			"li x5, -1\n"
			"csrw mcounteren, x5\n"
			"csrw scounteren, x5\n"
			"csrwi mhpmcounter3, 0\n"
			"csrwi mhpmcounter4, 0\n"
			"csrwi mhpmcounter5, 0\n"
			"csrwi mhpmcounter6, 0\n"
			"csrwi mhpmcounter7, 0\n"
			"csrwi mhpmcounter8, 0\n"
			"csrwi mhpmevent3, 21\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 28\n"   
			"csrwi mhpmevent6, 20\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 6\n"   
			"mov.m.x m0, x0, 0xff\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f1,  96 (x5)\n"
			"flw.ps f19,  608 (x5)\n"
			"flw.ps f3,  160 (x5)\n"
			"flw.ps f24,  896 (x5)\n"
			"flw.ps f12,  864 (x5)\n"
			"flw.ps f8,  576 (x5)\n"
			"flw.ps f2,  992 (x5)\n"
			"flw.ps f26,  960 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f9,  128 (x5)\n"
			"flw.ps f15,  480 (x5)\n"
			"flw.ps f27,  928 (x5)\n"
			"flw.ps f22,  0 (x5)\n"
			"flw.ps f16,  352 (x5)\n"
			"flw.ps f14,  384 (x5)\n"
			"flw.ps f0,  448 (x5)\n"
			"flw.ps f17,  736 (x5)\n"
			"flw.ps f23,  640 (x5)\n"
			"flw.ps f29,  768 (x5)\n"
			"flw.ps f18,  32 (x5)\n"
			"flw.ps f21,  224 (x5)\n"
			"flw.ps f31,  288 (x5)\n"
			"flw.ps f20,  512 (x5)\n"
			"flw.ps f11,  800 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f13,  832 (x5)\n"
			"flw.ps f6,  416 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f28,  544 (x5)\n"
			"flw.ps f30,  672 (x5)\n"
			"flw.ps f4,  64 (x5)\n"
			"flw.ps f7,  320 (x5)\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_HID_14:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_0_WAIT_HID_14\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_0_EXIT_HID_14\n"
			"LBL_SYNC_POINT_0_WAIT_HID_14:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_EXIT_HID_14:\n"          
			"nop\n"
			"LBL_KERNEL_HID_14:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_14\n"
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_HID_14:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 608(x26)\n"
			"flw.ps f23, 352(x26)\n"
			"fmax.ps f11, f30, f23\n" 
			"frcp.ps f23, f28\n"
			"srli x21, x0, 5\n"
			"fle.pi f22, f22, f11\n"
			"fltu.pi f16, f1, f7\n"
			"sltu x21, x26, x21\n"
			"mulh x14, x19, x14\n"
			"addw x14, x18, x18\n"
			"addw x29, x27, x11\n"
			"mulw x21, x23, x25\n"
			"li x23, 0x808698ce40\n"
			"la x14, mem_ptr_offset\n"
			"flw.ps f15, 672(x14)\n"
			"fscw.ps f29, f15 (x23)\n" 
			"packb x21, x23, x9\n"
			"feq.pi f7, f27, f27\n"
			"bltu x18, x4, 1f\n"
			"1:\n"
			"fsrai.pi f7, f27, 0xd\n"
			"fltu.pi f28, f16, f16\n"
			"fround.ps f7, f15, rup\n"
			"bgeu x1, x23, 1f\n"
			"1:\n"
			"add x19, x29, x29\n"
			"fsra.pi f15, f27, f15\n"
			"and x11, x25, x23\n"
			"fcvt.pwu.ps f30, f30, rdn\n"
			"li x14, 0x80b7fd6038\n"
			"la x25, mem_ptr_offset\n"
			"flw.ps f22, 800(x25)\n"
			"fgwg.ps f28, f22 (x14)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fsll.pi f24, f29, f7\n"
			"subw x26, x11, x14\n"
			"fmulh.pi f23, f23, f28\n"
			"sllw x25, x11, x25\n"
			"fxor.pi f31, f12, f16\n"
			"or x19, x9, x23\n"
			"addw x14, x0, x9\n"
			"auipc x29, 0x2\n"
			"fsrl.pi f24, f27, f22\n"
			"li x27, 0x80b71ce038\n"
			"la x23, mem_ptr_offset\n"
			"flw.ps f1, 576(x23)\n"
			"fgwg.ps f28, f1 (x27)\n" 
			"flem.ps m2, f3, f27\n"
			"sltu x9, x9, x9\n"
			"fmadd.ps f23, f3, f23, f7, rdn\n"
			"beq x12, x22, 1f\n"
			"1:\n"
			"mov.m.x m4, x18, 0x29\n"
			"bgeu x0, x6, 1f\n"
			"1:\n"
			"maskpopc x21, m4\n"
			"bge x18, x13, 1f\n"
			"1:\n"
			"div x9, x9, x11\n"
			"fence\n"
			"fmax.s f1, f27, f1\n"
			"fmax.s f29, f22, f22\n"
			"li x19, 0x80c5c3e008\n"
			"amoorl.d x27, x21, (x19)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fsub.s f31, f12, f31, rne\n"
			"fcmovm.ps f15, f7, f22\n"
			"fcvt.pw.ps f24, f16, rdn\n"
			"fsgnjx.s f15, f1, f28\n"
			"fltu.pi f2, f11, f16\n"
			"for.pi f15, f11, f24\n"
			"fpackrepb.pi f12, f23\n"
			"fmsub.s f22, f2, f22, f1, rdn\n"
			"bgeu x13, x28, 1f\n"
			"1:\n"
			"fmul.ps f7, f3, f28, rne\n"
			"li x19, 0x80b6b32008\n"
			"amoswapg.w x25, x23, (x19)\n" 
			"li x5, 0x415d\n"
			"csrw tensor_mask, x5\n"	
			"remu x14, x0, x29\n"
			"fmvs.x.ps x26, f27, 7\n"
			"bltu x27, x12, 1f\n"
			"1:\n"
			"fsetm.pi m4, f3\n"
			"sraiw x26, x19, 5\n"
			"flt.ps f3, f3, f30\n"
			"ori x23, x21, 0x17\n"
			"sraiw x25, x29, 3\n"
			"addi x11, x0, 0x1a\n"
			"fle.s x18, f29, f3\n"
			"li x19, 0x80b19ab000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f11, (x19)\n" 
			"li x5, 0xe242\n"
			"csrw tensor_mask, x5\n"	
			"fmul.s f24, f11, f27, rdn\n"
			"fandi.pi f1, f7, 0x79\n"
			"blt x5, x21, 1f\n"
			"1:\n"
			"flt.pi f24, f3, f11\n"
			"fcvt.pw.ps f7, f30, rdn\n"
			"fmul.ps f3, f31, f11, rmm\n"
			"fround.ps f1, f1, rtz\n"
			"fnot.pi f15, f7\n"
			"fcvt.ps.pw f1, f31, rup\n"
			"fmaxu.pi f31, f11, f1\n"
			"blt x30, x20, 1f\n"
			"1:\n"
			"li x23, 0x151c810c72b59756\n"
			"slti x19, x23, 0x1d\n" 
			"fmadd.s f7, f7, f30, f15, rne\n"
			"bne x18, x30, 1f\n"
			"1:\n"
			"fsgnjn.s f27, f31, f15\n"
			"subw x27, x27, x27\n"
			"fltu.pi f15, f3, f27\n"
			"beq x25, x26, 1f\n"
			"1:\n"
			"fadd.pi f3, f12, f27\n"
			"fsgnjx.s f11, f16, f31\n"
			"fmulh.pi f11, f27, f1\n"
			"fsgnj.s f30, f16, f30\n"
			"bge x28, x15, 1f\n"
			"1:\n"
			"fsatu8.pi f31, f1\n"
			"li x18, 0x80875d8670\n"
			"la x25, mem_ptr_offset\n"
			"flw.ps f3, 352(x25)\n"
			"fgw.ps f31, f3 (x18)\n" 
			"fsub.s f15, f7, f30, rtz\n"
			"faddi.pi f31, f30, 0x1cd\n"
			"fcvt.pw.ps f29, f15, rup\n"
			"fsub.ps f24, f15, f16, rup\n"
			"fadd.s f15, f3, f15, rne\n"
			"fsat8.pi f22, f29\n"
			"bne x12, x16, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f16, f23, rne\n"
			"fmul.pi f2, f7, f2\n"
			"fpackreph.pi f16, f12\n"
			"li x21, 0x80c633b000\n"
			"amoaddl.w x14, x14, (x21)\n" 
			"maskor m0, m0, m4\n"
			"fmv.s.x f15, x9\n"
			"fmv.x.s x11, f28\n"
			"maskxor m0, m2, m0\n"
			"fmvs.x.ps x0, f28, 5\n"
			"fle.pi f27, f2, f2\n"
			"maskpopcz x29, m4\n"
			"fpackrepb.pi f15, f31\n"
			"masknot m0, m4\n"
			"li x11, 0x80b095d020\n"
			"amoandg.d x27, x18, (x11)\n" 
			"frcp.ps f16, f16\n"
			"ffrc.ps f31, f15\n"
			"fcvt.f16.ps f12, f30\n"
			"fcvt.f16.ps f30, f1\n"
			"fmsub.ps f28, f23, f15, f15, rdn\n"
			"fand.pi f30, f27, f15\n"
			"fnmadd.ps f31, f30, f30, f28, rmm\n"
			"fsub.ps f16, f27, f28, rtz\n"
			"fsrl.pi f27, f15, f1\n"
			"li x25, 0x80b612a008\n"
			"amoorg.d x29, x21, (x25)\n" 
			"li x5, 0xba81da0aab66b6a8\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xebfd286a108f3b52\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"fmulh.pi f22, f30, f24\n"
			"fsgnjx.ps f15, f16, f27\n"
			"fltm.pi m2, f16, f12\n"
			"fmax.s f1, f1, f30\n"
			"fltm.ps m4, f11, f22\n"
			"fmaxu.pi f2, f23, f27\n"
			"blt x19, x15, 1f\n"
			"1:\n"
			"mova.m.x x18\n"
			"bge x30, x16, 1f\n"
			"1:\n"
			"srl x0, x0, x27\n"
			"remw x14, x11, x14\n"
			"li x21, 0x80c120c020\n"
			"amomaxul.d x19, x9, (x21)\n" 
			"fmadd.ps f2, f28, f16, f22, rup\n"
			"fadd.ps f11, f11, f23, rup\n"
			"bge x31, x26, 1f\n"
			"1:\n"
			"fmaxu.pi f2, f22, f27\n"
			"fltu.pi f29, f31, f29\n"
			"bne x26, x31, 1f\n"
			"1:\n"
			"fmsub.s f27, f7, f23, f27, rdn\n"
			"fmin.s f7, f7, f30\n"
			"fnot.pi f15, f15\n"
			"fsatu8.pi f7, f31\n"
			"bne x17, x16, 1f\n"
			"1:\n"
			"fmadd.ps f28, f28, f3, f24, rtz\n"
			"la x23, rand_int_32\n"
			"flw.ps f30, 960(x23)\n"
			"flw.ps f3, 224(x23)\n"
			"fmulhu.pi f7, f30, f3\n" 
			"fsrli.pi f27, f7, 0x6\n"
			"fsatu8.pi f22, f29\n"
			"fnot.pi f24, f2\n"
			"bltu x11, x4, 1f\n"
			"1:\n"
			"fsrai.pi f30, f12, 0x0\n"
			"fadd.s f29, f15, f29, rup\n"
			"fsgnjx.s f11, f15, f16\n"
			"feq.ps f23, f23, f15\n"
			"fandi.pi f7, f30, 0x153\n"
			"fsub.pi f1, f1, f7\n"
			"bltu x29, x17, 1f\n"
			"1:\n"
			"div x27, x26, x29\n" 
			"fcvt.ps.pw f27, f22, rmm\n"
			"fcvt.ps.f16 f3, f7\n"
			"fbci.ps f31, 0xe865\n"
			"fcvt.s.w f28, x18, rdn\n"
			"blt x2, x7, 1f\n"
			"1:\n"
			"feq.ps f27, f12, f12\n"
			"bge x4, x30, 1f\n"
			"1:\n"
			"and x21, x19, x0\n"
			"fsgnjn.ps f27, f11, f27\n"
			"feq.pi f12, f27, f12\n"
			"fslli.pi f23, f12, 0x5\n"
			"li x27, 0xb8ad0765a5695bb9\n"
			"slti x9, x27, 0x2\n" 
			"li x5, 0x9601\n"
			"csrw tensor_mask, x5\n"	
			"fmax.s f7, f24, f11\n"
			"fsra.pi f27, f2, f3\n"
			"fmul.s f1, f11, f1, rmm\n"
			"fnmadd.s f11, f31, f12, f23, rdn\n"
			"fnmadd.ps f3, f15, f1, f28, rmm\n"
			"feq.pi f7, f7, f11\n"
			"fsub.ps f3, f3, f31, rmm\n"
			"fxor.pi f15, f27, f28\n"
			"bltu x1, x0, 1f\n"
			"1:\n"
			"for.pi f11, f11, f31\n"
			"bne x4, x5, 1f\n"
			"1:\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_0_TLD_ID_0_NMASK_0x1_CMASK_0xfb_SHIRE_0_HID_14:\n"
			"li x31, 0x1fb1e\n"
			"csrw tensor_coop, x31\n" 
			"li x31, 0x01FF000001FF0000\n"
			"csrw tensor_mask, x31\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"li x14, 0xd440008020006b28\n"
			"csrw tensor_load, x14\n" 
			"csrwi tensor_wait, 1\n" 
			"LBL_SYNC_POINT_1_HID_14:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_1_WAIT_HID_14\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_1_EXIT_HID_14\n"
			"LBL_SYNC_POINT_1_WAIT_HID_14:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_1_EXIT_HID_14:\n"          
			"nop\n"
			"LBL_SYNC_POINT_2_HID_14:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_2_WAIT_HID_14\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_2_EXIT_HID_14\n"
			"LBL_SYNC_POINT_2_WAIT_HID_14:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_2_EXIT_HID_14:\n"          
			"nop\n"
			"LBL_SYNC_POINT_3_HID_14:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_3_WAIT_HID_14\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_3_EXIT_HID_14\n"
			"LBL_SYNC_POINT_3_WAIT_HID_14:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_3_EXIT_HID_14:\n"          
			"nop\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x5, 0x1e6a800000e2430\n"
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 3\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f11, 3(x31)\n"
			"flw.ps f12, 3(x31)\n"
			"flw.ps f15, 5(x31)\n"
			"flw.ps f2, 2(x31)\n"
			"flw.ps f23, 5(x31)\n"
			"flw.ps f24, 22(x31)\n"
			"flw.ps f27, 14(x31)\n"
			"flw.ps f29, 0(x31)\n"
			"flw.ps f3, 13(x31)\n"
			"flw.ps f31, 2(x31)\n"
			"flw.ps f7, 33(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x11, 3(x31)\n"
			"lw x14, 8(x31)\n"
			"lw x18, 8(x31)\n"
			"lw x19, 1(x31)\n"
			"lw x21, 8(x31)\n"
			"lw x23, 14(x31)\n"
			"lw x25, 11(x31)\n"
			"lw x26, 8(x31)\n"
			"lw x27, 34(x31)\n"
			"lw x29, 30(x31)\n"
			"lw x9, 23(x31)\n"
			"li x31, 0x3\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_14:\n"
			"fand.pi f29, f29, f11\n"
			"addw x21, x21, x27\n"
			"sub x18, x19, x18\n"
			"fsat8.pi f24, f2\n"
			"fsgnj.s f23, f27, f3\n"
			"remu x27, x29, x29\n"
			"fmvs.x.ps x18, f11, 3\n"
			"fle.s x0, f7, f1\n"
			"fsrli.pi f24, f15, 0x5\n"
			"addi x0, x25, 0x19\n"
			"fmaxu.pi f24, f27, f23\n"
			"fcvt.s.w f7, x0, rup\n"
			"fsgnj.s f23, f27, f1\n"
			"fcvt.ps.pwu f24, f7, rtz\n"
			"maskpopc x25, m2\n"
			"for.pi f31, f24, f2\n"
			"mulhu x19, x25, x9\n"
			"fcmovm.ps f1, f27, f29\n"
			"feq.ps f2, f23, f1\n"
			"flt.s x0, f23, f11\n"
			"bge x26, x3, 1f\n"
			"1:\n"
			"div x19, x9, x26\n"
			"fadd.ps f31, f24, f12, rmm\n"
			"fadd.pi f2, f24, f27\n"
			"fadd.ps f31, f3, f23, rmm\n"
			"fsgnjn.ps f11, f7, f7\n"
			"fmul.pi f7, f24, f3\n"
			"fbci.pi f24, 0x7a0b4\n"
			"fmadd.s f24, f15, f2, f24, rtz\n"
			"maskxor m0, m0, m7\n"
			"mov.m.x m0, x0, 0xff\n"
			"maskand m7, m0, m2\n"
			"fnmsub.s f1, f24, f12, f27, rne\n"
			"flt.s x0, f23, f31\n"
			"fmvz.x.ps x18, f3, 2\n"
			"sltu x21, x0, x14\n"
			"sltiu x26, x19, 0x10\n"
			"fsra.pi f15, f29, f15\n"
			"bgeu x2, x22, 1f\n"
			"1:\n"
			"slli x11, x0, 0\n"
			"bgeu x6, x30, 1f\n"
			"1:\n"
			"fmulhu.pi f29, f3, f11\n"
			"fbcx.ps f27, x19\n"
			"for.pi f24, f24, f27\n"
			"bgeu x31, x0, 1f\n"
			"1:\n"
			"maskor m0, m7, m4\n"
			"mov.m.x m0, x0, 0xff\n"
			"sll x26, x27, x27\n"
			"fpackreph.pi f23, f23\n"
			"divw x11, x23, x11\n"
			"fsub.pi f24, f1, f15\n"
			"fcvt.ps.pw f11, f15, rne\n"
			"ebreak\n"
			"fltu.pi f23, f2, f12\n"
			"srl x14, x0, x14\n"
			"flt.pi f30, f27, f1\n"
			"rem x29, x29, x9\n"
			"bne x16, x28, 1f\n"
			"1:\n"
			"auipc x0, 0xa\n"
			"fexp.ps f31, f7\n"
			"beq x22, x22, 1f\n"
			"1:\n"
			"maskxor m4, m7, m4\n"
			"flt.ps f1, f7, f1\n"
			"sltu x23, x23, x23\n"
			"lui x29, 0x7\n"
			"fle.s x14, f7, f2\n"
			"beq x12, x3, 1f\n"
			"1:\n"
			"fmax.s f11, f15, f11\n"
			"flt.s x26, f3, f27\n"
			"blt x23, x0, 1f\n"
			"1:\n"
			"masknot m0, m2\n"
			"mov.m.x m0, x0, 0xff\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_0_LOOP_SEQ_HID_14\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_HID_14:\n"
			"li x23, 0x8082e5f340\n"
			"fsw.ps f28, 128(x23)\n" 
			"fmulhu.pi f11, f31, f30\n"
			"li x26, 0x8083ea7240\n"
			"la x19, mem_ptr_offset\n"
			"flw.ps f3, 704(x19)\n"
			"fscw.ps f16, f3 (x26)\n" 
			"fcvt.pw.ps f11, f24, rtz\n"
			"li x11, 0x617c77bf933fd47b\n"
			"fcvt.s.lu f3, x11, rmm\n" 
			"fcvt.pwu.ps f28, f2, rne\n"
			"li x9, 0x80c7f75018\n"
			"amominl.w x18, x27, (x9)\n" 
			"for.pi f12, f12, f29\n"
			"la x21, rand_int_32\n"
			"flw.ps f28, 512(x21)\n"
			"flw.ps f1, 96(x21)\n"
			"frem.pi f2, f28, f1\n" 
			"fsgnjx.ps f7, f7, f7\n"
			"li x11, 0x80c6fd7008\n"
			"la x25, mem_ptr_offset\n"
			"flw.ps f22, 480(x25)\n"
			"famomaxl.ps f24, f22 (x11)\n" 
			"mov.m.x m7, x14, 0x48\n"
			"la x27, rand_int_32\n"
			"flw.ps f1, 960(x27)\n"
			"flw.ps f23, 928(x27)\n"
			"fremu.pi f12, f1, f23\n" 
			"fandi.pi f30, f7, 0x1fc\n"
			"li x14, 0x12bc4095b098e684\n"
			"slliw x11, x14, 4\n" 
			"fltu.pi f24, f2, f23\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 896(x26)\n"
			"fcvt.pwu.ps f11, f28, rtz\n" 
			"divw x27, x9, x18\n"
			"li x21, 0x80850483c0\n"
			"li x26, 1035288863\n"
			"fg32w.ps f1, x18 (x21)\n" 
			"fcvt.ps.pw f1, f1, rmm\n"
			"li x29, 0x8080c2eec0\n"
			"la x26, mem_ptr_offset\n"
			"flw.ps f2, 320(x26)\n"
			"fgb.ps f24, f2 (x29)\n" 
			"fsrai.pi f12, f12, 0x3\n"
			"li x19, 0x80c0b50018\n"
			"la x23, mem_ptr_offset\n"
			"flw.ps f16, 736(x23)\n"
			"famoswapl.pi f27, f16 (x19)\n" 
			"fmsub.ps f11, f12, f11, f28, rdn\n"
			"blt x6, x6, 1f\n"
			"1:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 160(x26)\n"
			"fclass.ps f15, f30\n" 
			"fclass.ps f1, f11\n"
			"li x14, 0x80b3fa9008\n"
			"amoxorg.d x14, x19, (x14)\n" 
			"fswizz.ps f31, f31, 0x10\n"
			"li x23, 0x80c4d14010\n"
			"la x19, mem_ptr_offset\n"
			"flw.ps f3, 192(x19)\n"
			"famomaxul.pi f29, f3 (x23)\n" 
			"fmadd.s f22, f23, f22, f28, rmm\n"
			"blt x11, x15, 1f\n"
			"1:\n"
			"la x26, rand_int_32\n"
			"flw.ps f7, 288(x26)\n"
			"fsat8.pi f22, f7\n" 
			"fcvt.w.s x26, f23, rne\n"
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_1_TLD_ID_1_NMASK_0x1_CMASK_0xbf_SHIRE_0_HID_14:\n"
			"li x31, 0x1bf0e\n"
			"csrw tensor_coop, x31\n" 
			"li x31, 0x01FF000001FF0000\n"
			"csrw tensor_mask, x31\n" 
			"li x31, (0x3 << 6) | 0x1\n"
			"li x14, 0x4010008020006b0b\n"
			"csrw tensor_load, x14\n" 
			"li x7, 0xdd800000118002\n"
			"csrw tensor_fma, x7\n" 
			"LBL_SYNC_POINT_4_HID_14:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_4_WAIT_HID_14\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_4_EXIT_HID_14\n"
			"LBL_SYNC_POINT_4_WAIT_HID_14:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_4_EXIT_HID_14:\n"          
			"nop\n"
			"LBL_SYNC_POINT_5_HID_14:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_5_WAIT_HID_14\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_5_EXIT_HID_14\n"
			"LBL_SYNC_POINT_5_WAIT_HID_14:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_5_EXIT_HID_14:\n"          
			"nop\n"
			"LBL_SYNC_POINT_6_HID_14:\n"          
			"li x5, 0xe0\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_6_WAIT_HID_14\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_6_EXIT_HID_14\n"
			"LBL_SYNC_POINT_6_WAIT_HID_14:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_6_EXIT_HID_14:\n"          
			"nop\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x7, 0xca40008080002740\n"
			"li x31, (0x20 << 4)\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 3\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f16, 0(x31)\n"
			"flw.ps f23, 2(x31)\n"
			"flw.ps f24, 4(x31)\n"
			"flw.ps f27, 2(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x14, 2(x31)\n"
			"lw x21, 1(x31)\n"
			"li x31, 0x2\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_14:\n"
			"remw x26, x21, x14\n"
			"fence\n"
			"masknot m4, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"srli x26, x21, 4\n"
			"srlw x21, x0, x21\n"
			"fle.ps f7, f16, f27\n"
			"fadd.ps f2, f27, f23, rmm\n"
			"bge x9, x2, 1f\n"
			"1:\n"
			"fcvt.s.wu f11, x14, rdn\n"
			"fle.s x26, f23, f27\n"
			"ffrc.ps f24, f24\n"
			"beq x5, x0, 1f\n"
			"1:\n"
			"fmadd.s f27, f16, f23, f27, rne\n"
			"mul x26, x0, x0\n"
			"flt.s x19, f27, f23\n"
			"blt x8, x4, 1f\n"
			"1:\n"
			"fcvt.ps.pw f11, f16, rup\n"
			"bge x1, x3, 1f\n"
			"1:\n"
			"fle.s x26, f23, f27\n"
			"fcvt.wu.s x14, f24, rmm\n"
			"fround.ps f27, f27, rtz\n"
			"blt x18, x0, 1f\n"
			"1:\n"
			"slli x26, x0, 3\n"
			"bge x15, x8, 1f\n"
			"1:\n"
			"fmul.s f28, f16, f24, rtz\n"
			"flt.pi f30, f24, f24\n"
			"fmul.ps f7, f24, f23, rne\n"
			"mova.m.x x14\n"
			"ecall\n"
			"fsub.ps f24, f24, f24, rdn\n"
			"xori x0, x0, 0x1f\n"
			"divw x14, x14, x21\n"
			"fsra.pi f23, f16, f27\n"
			"fsrl.pi f24, f23, f23\n"
			"packb x26, x14, x14\n"
			"feq.pi f12, f16, f27\n"
			"fsgnjn.s f30, f23, f23\n"
			"feq.pi f24, f27, f27\n"
			"packb x26, x21, x14\n"
			"fand.pi f23, f23, f16\n"
			"andi x0, x0, 0xc\n"
			"bge x18, x14, 1f\n"
			"1:\n"
			"fsetm.pi m0, f27\n"
			"mov.m.x m0, x0, 0xff\n"
			"bgeu x13, x13, 1f\n"
			"1:\n"
			"rem x14, x21, x14\n"
			"beq x29, x25, 1f\n"
			"1:\n"
			"fcvt.ps.pw f16, f27, rup\n"
			"addiw x14, x14, 0x1d\n"
			"fmul.pi f11, f16, f27\n"
			"fsgnjx.s f23, f24, f23\n"
			"fcvt.w.s x26, f24, rup\n"
			"fmul.pi f24, f23, f24\n"
			"and x26, x14, x14\n"
			"fle.s x21, f16, f27\n"
			"xor x14, x0, x14\n"
			"fsrli.pi f24, f24, 0x2\n"
			"subw x0, x0, x0\n"
			"fclass.ps f27, f27\n"
			"fbci.pi f11, 0x63076\n"
			"fcmovm.ps f27, f27, f24\n"
			"fmadd.ps f23, f23, f23, f24, rup\n"
			"fclass.s x19, f16\n"
			"fle.pi f30, f23, f27\n"
			"addi x14, x21, 0x2\n"
			"fxor.pi f30, f23, f27\n"
			"fence\n"
			"fmadd.s f24, f16, f24, f24, rup\n"
			"beq x15, x4, 1f\n"
			"1:\n"
			"maskpopc x14, m2\n"
			"fnmsub.s f12, f24, f24, f24, rne\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_1_LOOP_SEQ_HID_14\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_HID_14:\n"
			"li x18, 0x8084d11b10\n"
			"la x9, mem_ptr_offset\n"
			"flw.ps f11, 928(x9)\n"
			"fgw.ps f11, f11 (x18)\n" 
			"li x18, 0x80c3ea1030\n"
			"la x21, mem_ptr_offset\n"
			"flw.ps f3, 960(x21)\n"
			"fscwl.ps f16, f3 (x18)\n" 
			"li x23, 0x80b3180028\n"
			"la x14, mem_ptr_offset\n"
			"flw.ps f11, 576(x14)\n"
			"famoming.ps f31, f11 (x23)\n" 
			"li x11, 0x8081cb9267\n"
			"sd x9, 512(x11)\n"
			"lb x9, 512(x11)\n" 
			"li x9, 0xd6a6961a0d86bff8\n"
			"andi x27, x9, 0x11\n" 
			"li x23, 0x80b37c3000\n"
			"amoswapg.d x18, x19, (x23)\n" 
			"li x31, 0x80800046c0\n"
			"li x23, 0x800000000000000b\n"
			"add x23, x31, x23\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw lock_va, x23\n"	
			"csrwi tensor_wait, 6\n" 
			"li x29, 0x8087514a10\n"
			"la x19, mem_ptr_offset\n"
			"flw.ps f11, 512(x19)\n"
			"fgw.ps f15, f11 (x29)\n" 
			"li x29, 0x80b5f46018\n"
			"amoorg.d x25, x23, (x29)\n" 
			"li x27, 0x8086018140\n"
			"fsw f23, 256(x27)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"li x27, 0xa95919900428c57\n"
			"sraiw x18, x27, 1\n" 
			"li x26, 0x80b1294000\n"
			"amoandg.w x19, x29, (x26)\n" 
			"li x5, 0xd1b9\n"
			"csrw tensor_mask, x5\n"	
			"li x9, 0x80c7d0c018\n"
			"la x26, mem_ptr_offset\n"
			"flw.ps f3, 544(x26)\n"
			"fscbl.ps f12, f3 (x9)\n" 
			"li x9, 0x8081c4e388\n"
			"li x27, 1807470188\n"
			"fg32b.ps f2, x14 (x9)\n" 
			"li x14, 0xde0081a416005b4d\n"
			"csrw tensor_conv_size, x14\n"	
			"li x19, 0x80b1fc7008\n"
			"la x9, mem_ptr_offset\n"
			"flw.ps f30, 928(x9)\n"
			"famomaxg.ps f15, f30 (x19)\n" 
			"csrw tensor_error, zero\n" 
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrs x0, mcache_control, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi   tensor_wait, 0x0\n"     
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, 0x1025c00000000276\n"
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 2\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			"csrwi excl_mode, 0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x18\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f12, 2(x31)\n"
			"flw.ps f15, 7(x31)\n"
			"flw.ps f16, 2(x31)\n"
			"flw.ps f2, 9(x31)\n"
			"flw.ps f22, 1(x31)\n"
			"flw.ps f23, 21(x31)\n"
			"flw.ps f24, 3(x31)\n"
			"flw.ps f29, 23(x31)\n"
			"flw.ps f3, 35(x31)\n"
			"flw.ps f31, 11(x31)\n"
			"flw.ps f7, 37(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x11, 0(x31)\n"
			"lw x14, 4(x31)\n"
			"lw x18, 6(x31)\n"
			"lw x19, 3(x31)\n"
			"lw x21, 10(x31)\n"
			"lw x23, 6(x31)\n"
			"lw x25, 3(x31)\n"
			"lw x26, 17(x31)\n"
			"lw x27, 21(x31)\n"
			"lw x29, 24(x31)\n"
			"lw x9, 9(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_14:\n"
			"fle.ps f16, f12, f3\n"
			"fsgnj.ps f24, f2, f24\n"
			"fadd.ps f7, f24, f3, rdn\n"
			"fnmsub.s f29, f29, f31, f24, rne\n"
			"fcvt.ps.pwu f16, f7, rtz\n"
			"srli x26, x27, 2\n"
			"slt x25, x14, x25\n"
			"srliw x14, x23, 4\n"
			"bne x22, x8, 1f\n"
			"1:\n"
			"fsatu8.pi f29, f29\n"
			"maskpopcz x18, m0\n"
			"fmul.pi f16, f23, f12\n"
			"divw x0, x18, x11\n"
			"slti x19, x26, 0x12\n"
			"fround.ps f2, f31, rtz\n"
			"bgeu x18, x20, 1f\n"
			"1:\n"
			"mulhsu x29, x29, x27\n"
			"fmin.ps f15, f22, f23\n"
			"fmin.pi f29, f3, f31\n"
			"fcvt.ps.pw f16, f29, rup\n"
			"bltu x29, x21, 1f\n"
			"1:\n"
			"feq.pi f1, f1, f2\n"
			"fslli.pi f28, f23, 0xc\n"
			"fsra.pi f7, f23, f2\n"
			"packb x11, x19, x26\n"
			"flt.s x14, f29, f29\n"
			"beq x27, x27, 1f\n"
			"1:\n"
			"ebreak\n"
			"slti x25, x9, 0x1e\n"
			"fcvt.s.w f29, x19, rup\n"
			"feq.ps f1, f12, f23\n"
			"xori x25, x25, 0x1a\n"
			"beq x4, x8, 1f\n"
			"1:\n"
			"fcmovm.ps f29, f1, f7\n"
			"bltu x3, x11, 1f\n"
			"1:\n"
			"fadd.pi f27, f29, f7\n"
			"fandi.pi f29, f3, 0x1c3\n"
			"fnmsub.ps f29, f12, f15, f2, rup\n"
			"blt x22, x23, 1f\n"
			"1:\n"
			"sltiu x23, x27, 0x5\n"
			"fadd.s f28, f24, f23, rne\n"
			"fle.s x14, f12, f2\n"
			"fpackreph.pi f31, f15\n"
			"fnmsub.ps f16, f31, f16, f22, rne\n"
			"blt x7, x21, 1f\n"
			"1:\n"
			"fle.pi f1, f22, f3\n"
			"fcvt.ps.f16 f11, f3\n"
			"andi x0, x14, 0x15\n"
			"blt x0, x25, 1f\n"
			"1:\n"
			"fround.ps f28, f22, rdn\n"
			"fsatu8.pi f27, f3\n"
			"mulhu x14, x29, x11\n"
			"flt.s x29, f15, f23\n"
			"for.pi f28, f3, f1\n"
			"srli x23, x26, 3\n"
			"mulw x25, x23, x29\n"
			"mova.m.x x14\n"
			"fsrl.pi f16, f24, f23\n"
			"fcvt.wu.s x19, f31, rtz\n"
			"bltu x22, x29, 1f\n"
			"1:\n"
			"subw x21, x26, x21\n"
			"fmv.s.x f27, x14\n"
			"fslli.pi f15, f7, 0xa\n"
			"maskpopcz x29, m7\n"
			"fcvt.w.s x29, f15, rne\n"
			"fmul.s f24, f24, f7, rtz\n"
			"fnmsub.s f28, f3, f24, f1, rne\n"
			"slt x29, x27, x21\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_2_LOOP_SEQ_HID_14\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f24,  992 (x5)\n"
			"flw.ps f7,  160 (x5)\n"
			"flw.ps f5,  224 (x5)\n"
			"flw.ps f28,  864 (x5)\n"
			"flw.ps f9,  736 (x5)\n"
			"flw.ps f18,  288 (x5)\n"
			"flw.ps f19,  0 (x5)\n"
			"flw.ps f12,  192 (x5)\n"
			"flw.ps f17,  384 (x5)\n"
			"flw.ps f10,  832 (x5)\n"
			"flw.ps f27,  960 (x5)\n"
			"flw.ps f30,  256 (x5)\n"
			"flw.ps f4,  128 (x5)\n"
			"flw.ps f25,  704 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f31,  768 (x5)\n"
			"flw.ps f13,  512 (x5)\n"
			"flw.ps f3,  96 (x5)\n"
			"flw.ps f6,  640 (x5)\n"
			"flw.ps f8,  352 (x5)\n"
			"flw.ps f23,  544 (x5)\n"
			"flw.ps f29,  608 (x5)\n"
			"flw.ps f2,  672 (x5)\n"
			"flw.ps f22,  896 (x5)\n"
			"flw.ps f15,  480 (x5)\n"
			"flw.ps f1,  800 (x5)\n"
			"flw.ps f14,  576 (x5)\n"
			"flw.ps f16,  64 (x5)\n"
			"flw.ps f21,  928 (x5)\n"
			"flw.ps f26,  448 (x5)\n"
			"flw.ps f11,  416 (x5)\n"
			"flw.ps f20,  32 (x5)\n"
			"la x31, check_sum_hid_14\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"LBL_POSTAMBLE_HID_14:\n"          
			"csrwi   tensor_wait, 0x0\n"
			"csrwi   tensor_wait, 0x1\n"
			"csrwi   tensor_wait, 0x2\n"
			"csrwi   tensor_wait, 0x3\n"
			"csrwi   tensor_wait, 0x4\n"
			"csrwi   tensor_wait, 0x5\n"
			"csrwi   tensor_wait, 0x6\n"
			"csrwi   tensor_wait, 0x7\n"
			"csrwi   tensor_wait, 0x8\n"
			"csrwi   tensor_wait, 0x9\n"
			"csrwi   tensor_wait, 0xa\n"
			"fence\n"
			"LBL_SYNC_POINT_7_HID_14:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_7_WAIT_HID_14\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_7_EXIT_HID_14\n"
			"LBL_SYNC_POINT_7_WAIT_HID_14:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_7_EXIT_HID_14:\n"          
			"nop\n"
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_14\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_14:\n"          
			".global LBL_FAIL_HID_14\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_14\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_14:\n"          
			".global LBL_PASS_HID_14\n"          
			VSNIP_RSV
		);
		c_test_pass();
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_14:\n"          
			".global LBL_HALT_HID_14\n"          
			"wfi\n"
			"j LBL_HALT_HID_14\n"          
			VSNIP_RSV
		);
		return 0;       
	};
	if ( hid == 15 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_15:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_15\n"
			"csrw mscratch, x5\n"
			"csrwi menable_shadows, 0xf\n"
			"li x5, -1\n"
			"csrw mcounteren, x5\n"
			"csrw scounteren, x5\n"
			"csrwi mhpmcounter3, 0\n"
			"csrwi mhpmcounter4, 0\n"
			"csrwi mhpmcounter5, 0\n"
			"csrwi mhpmcounter6, 0\n"
			"csrwi mhpmcounter7, 0\n"
			"csrwi mhpmcounter8, 0\n"
			"csrwi mhpmevent3, 7\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 9\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 23\n"   
			"mov.m.x m0, x0, 0xff\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f0,  32 (x5)\n"
			"flw.ps f15,  800 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f20,  384 (x5)\n"
			"flw.ps f13,  128 (x5)\n"
			"flw.ps f1,  320 (x5)\n"
			"flw.ps f9,  256 (x5)\n"
			"flw.ps f4,  352 (x5)\n"
			"flw.ps f24,  416 (x5)\n"
			"flw.ps f2,  64 (x5)\n"
			"flw.ps f19,  544 (x5)\n"
			"flw.ps f3,  640 (x5)\n"
			"flw.ps f28,  704 (x5)\n"
			"flw.ps f11,  672 (x5)\n"
			"flw.ps f5,  608 (x5)\n"
			"flw.ps f23,  576 (x5)\n"
			"flw.ps f12,  288 (x5)\n"
			"flw.ps f31,  512 (x5)\n"
			"flw.ps f10,  192 (x5)\n"
			"flw.ps f27,  832 (x5)\n"
			"flw.ps f18,  480 (x5)\n"
			"flw.ps f8,  160 (x5)\n"
			"flw.ps f16,  768 (x5)\n"
			"flw.ps f6,  448 (x5)\n"
			"flw.ps f21,  864 (x5)\n"
			"flw.ps f30,  224 (x5)\n"
			"flw.ps f29,  736 (x5)\n"
			"flw.ps f22,  992 (x5)\n"
			"flw.ps f25,  0 (x5)\n"
			"flw.ps f14,  928 (x5)\n"
			"flw.ps f26,  960 (x5)\n"
			"flw.ps f7,  896 (x5)\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_HID_15:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_0_WAIT_HID_15\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_0_EXIT_HID_15\n"
			"LBL_SYNC_POINT_0_WAIT_HID_15:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_0_EXIT_HID_15:\n"          
			"nop\n"
			"LBL_KERNEL_HID_15:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_15\n"
			"li x5, 0x7\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_HID_15:\n"
			"li x24, 0x80b86ef010\n"
			"la x25, mem_ptr_offset\n"
			"flw.ps f18, 640(x25)\n"
			"famoxorg.pi f21, f18 (x24)\n" 
			"fmax.pi f5, f10, f30\n"
			"li x20, 0x808bd4c0f8\n"
			"la x14, mem_ptr_offset\n"
			"flw.ps f21, 864(x14)\n"
			"fscw.ps f5, f21 (x20)\n" 
			"fmsub.ps f12, f5, f16, f5, rmm\n"
			"li x14, 0x80bfa6e038\n"
			"la x23, mem_ptr_offset\n"
			"flw.ps f28, 800(x23)\n"
			"famoming.ps f12, f28 (x14)\n" 
			"fsrl.pi f24, f8, f5\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f8, 608(x18)\n"
			"flog.ps f20, f8\n" 
			"fsgnjx.s f31, f10, f18\n"
			"li x18, 0x80cc51a030\n"
			"la x11, mem_ptr_offset\n"
			"flw.ps f18, 448(x11)\n"
			"famomaxl.ps f18, f18 (x18)\n" 
			"fnmsub.s f30, f30, f30, f12, rtz\n"
			"li x18, 0x80bcec9000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f12, (x18)\n"
			"flwg.ps f16, (x18)\n" 
			"fltm.pi m6, f24, f24\n"
			"bge x20, x7, 1f\n"
			"1:\n"
			"li x11, 0x1ca8d1bd07e4860f\n"
			"li x25, 0x24e25a99ff6f6f11\n"
			"remu x14, x11, x25\n" 
			"fswizz.ps f28, f21, 0x6\n"
			"li x7, 0x808ea6d578\n"
			"la x18, mem_ptr_offset\n"
			"flw.ps f8, 576(x18)\n"
			"fsch.ps f30, f8 (x7)\n" 
			"fsgnjx.s f9, f26, f12\n"
			"li x31, 0x8088006440\n"
			"li x28, 0x800000000000006\n"
			"add x28, x31, x28\n"
			"li x31, (0x1 << 6) | 0x0\n"
			"csrwi tensor_wait, 6\n" 
			"csrwi tensor_wait, 4\n" 
			"div x14, x14, x23\n"
			"li x27, 0x80c9cd3038\n"
			"amominl.d x23, x20, (x27)\n" 
			"fmulhu.pi f16, f20, f16\n"
			"li x11, 0x1ab2b5873d89c808\n"
			"li x24, 0x6051bdfbf74aa74a\n"
			"packb x27, x11, x24\n" 
			"fmin.ps f21, f10, f2\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f26, 352(x20)\n"
			"flw.ps f24, 256(x20)\n"
			"flw.ps f12, 480(x20)\n"
			"fnmadd.ps f1, f26, f24, f12, rup\n" 
			"flog.ps f8, f8\n"
			"beq x10, x9, 1f\n"
			"1:\n"
			"li x17, 0x808c7bbf19\n"
			"sd x7, 128(x17)\n"
			"lbu x17, 128(x17)\n" 
			"fclass.ps f5, f21\n"
			"bne x27, x19, 1f\n"
			"1:\n"
			"li x25, 0x80bda08000\n"
			"la x23, mem_ptr_offset\n"
			"flw.ps f8, 928(x23)\n"
			"famoming.pi f8, f8 (x25)\n" 
			"flt.s x24, f30, f18\n"
			"li x27, 0x80cb18b038\n"
			"amominul.d x18, x28, (x27)\n" 
			"for.pi f26, f20, f26\n"
			"li x7, 0x80cbe11008\n"
			"amominul.d x20, x20, (x7)\n" 
			"fnot.pi f9, f10\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x10\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f10, 1(x31)\n"
			"flw.ps f12, 1(x31)\n"
			"flw.ps f16, 5(x31)\n"
			"flw.ps f18, 12(x31)\n"
			"flw.ps f2, 15(x31)\n"
			"flw.ps f20, 24(x31)\n"
			"flw.ps f21, 15(x31)\n"
			"flw.ps f24, 22(x31)\n"
			"flw.ps f26, 5(x31)\n"
			"flw.ps f28, 15(x31)\n"
			"flw.ps f30, 26(x31)\n"
			"flw.ps f31, 10(x31)\n"
			"flw.ps f5, 42(x31)\n"
			"flw.ps f8, 13(x31)\n"
			"flw.ps f9, 43(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x11, 4(x31)\n"
			"lw x14, 2(x31)\n"
			"lw x17, 6(x31)\n"
			"lw x20, 11(x31)\n"
			"lw x24, 15(x31)\n"
			"lw x27, 17(x31)\n"
			"lw x28, 28(x31)\n"
			"lw x7, 14(x31)\n"
			"li x31, 0x2\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_15:\n"
			"fle.pi f10, f12, f2\n"
			"slliw x17, x14, 4\n"
			"fltm.ps m2, f1, f30\n"
			"fpackreph.pi f28, f28\n"
			"fpackrepb.pi f30, f2\n"
			"bne x27, x26, 1f\n"
			"1:\n"
			"maskor m2, m2, m5\n"
			"fmin.ps f20, f5, f26\n"
			"and x7, x0, x7\n"
			"divu x28, x28, x11\n"
			"fclass.s x14, f10\n"
			"masknot m3, m5\n"
			"bge x1, x5, 1f\n"
			"1:\n"
			"remuw x18, x27, x27\n"
			"for.pi f8, f24, f12\n"
			"fsat8.pi f9, f9\n"
			"sub x11, x24, x11\n"
			"fcvt.ps.pw f16, f9, rmm\n"
			"fsra.pi f24, f10, f31\n"
			"feq.ps f20, f20, f1\n"
			"fcvt.ps.pw f5, f8, rmm\n"
			"fsrl.pi f24, f10, f1\n"
			"feq.ps f31, f12, f18\n"
			"fsub.pi f1, f28, f20\n"
			"srai x14, x14, 3\n"
			"fmvs.x.ps x23, f2, 1\n"
			"beq x10, x19, 1f\n"
			"1:\n"
			"fsub.pi f28, f26, f1\n"
			"fsgnj.ps f12, f30, f9\n"
			"fsgnj.ps f20, f12, f26\n"
			"lui x24, 0x3\n"
			"fle.s x0, f10, f24\n"
			"feq.ps f30, f10, f30\n"
			"maskand m6, m6, m5\n"
			"bgeu x30, x22, 1f\n"
			"1:\n"
			"fminu.pi f30, f26, f1\n"
			"feqm.ps m5, f10, f18\n"
			"fnmsub.ps f21, f21, f28, f1, rtz\n"
			"mova.m.x x0\n"
			"maskpopc x28, m3\n"
			"fsrli.pi f30, f10, 0x9\n"
			"fmsub.ps f1, f18, f1, f21, rne\n"
			"sltu x27, x27, x0\n"
			"fcvt.f16.ps f18, f10\n"
			"fmul.ps f28, f9, f12, rmm\n"
			"blt x2, x24, 1f\n"
			"1:\n"
			"fmaxu.pi f26, f26, f30\n"
			"fltm.pi m5, f12, f1\n"
			"ffrc.ps f21, f10\n"
			"bge x27, x31, 1f\n"
			"1:\n"
			"subw x23, x7, x28\n"
			"fle.pi f10, f16, f24\n"
			"fsgnjx.s f1, f1, f10\n"
			"srl x14, x11, x17\n"
			"divw x17, x27, x0\n"
			"fsgnjx.s f21, f26, f26\n"
			"fsrai.pi f16, f18, 0x1\n"
			"fadd.s f18, f28, f16, rne\n"
			"fsrl.pi f8, f28, f5\n"
			"fmadd.s f18, f1, f9, f26, rne\n"
			"remw x25, x24, x20\n"
			"bgeu x18, x27, 1f\n"
			"1:\n"
			"fltm.ps m3, f5, f2\n"
			"fcvt.ps.f16 f31, f18\n"
			"fcmovm.ps f20, f12, f31\n"
			"slt x28, x14, x27\n"
			"fltm.pi m3, f9, f10\n"
			"mov.m.x m3, x17, 0x53\n"
			"bgeu x15, x7, 1f\n"
			"1:\n"
			"fle.pi f18, f5, f21\n"
			"mulhsu x24, x17, x24\n"
			"fmadd.s f5, f5, f28, f20, rmm\n"
			"fle.s x7, f2, f5\n"
			"fxor.pi f1, f28, f21\n"
			"fmadd.s f10, f28, f2, f28, rmm\n"
			"srai x28, x28, 3\n"
			"fcvt.ps.pw f18, f20, rne\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_0_LOOP_SEQ_HID_15\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_HID_15:\n"
			"li x23, 0x80cc74c034\n"
			"amoandl.w x17, x28, (x23)\n" 
			"fmvs.x.ps x18, f16, 2\n"
			"li x24, 0x808d607b58\n"
			"fsw.ps f1, -256(x24)\n"
			"flq2 f30, -256(x24)\n" 
			"fmsub.ps f2, f18, f2, f31, rmm\n"
			"bne x22, x19, 1f\n"
			"1:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f31, 832(x28)\n"
			"fexp.ps f26, f31\n" 
			"fcvt.ps.pw f10, f10, rdn\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 928(x17)\n"
			"flw.ps f21, 640(x17)\n"
			"fsgnj.s f2, f16, f21\n" 
			"subw x0, x24, x20\n"
			"la x24, rand_ieee754_32\n"
			"flw.ps f26, 672(x24)\n"
			"flw.ps f18, 768(x24)\n"
			"fmax.s f10, f26, f18\n" 
			"fsub.pi f31, f31, f31\n"
			"bge x11, x0, 1f\n"
			"1:\n"
			"li x27, 0x80cebc3000\n"
			"la x11, mem_ptr_offset\n"
			"flw.ps f2, 160(x11)\n"
			"famoswapl.pi f18, f2 (x27)\n" 
			"fsra.pi f31, f12, f8\n"
			"bltu x1, x5, 1f\n"
			"1:\n"
			"li x28, 0x808bc568b0\n"
			"fsw.ps f1, -256(x28)\n" 
			"fsub.ps f8, f18, f24, rmm\n"
			"li x24, 0x80bbe72010\n"
			"la x11, mem_ptr_offset\n"
			"flw.ps f31, 224(x11)\n"
			"famoorg.pi f5, f31 (x24)\n" 
			"fcmovm.ps f8, f8, f10\n"
			"li x14, 0x80bb22a008\n"
			"la x18, mem_ptr_offset\n"
			"flw.ps f16, 544(x18)\n"
			"famomaxg.pi f2, f16 (x14)\n" 
			"fmul.ps f5, f5, f5, rtz\n"
			"li x18, 0x80bfda1024\n"
			"amomaxug.w x7, x23, (x18)\n" 
			"divw x17, x0, x23\n"
			"li x17, 0xec529dfb76bf718f\n"
			"li x18, 0x6a1b304b67f7edff\n"
			"sraw x18, x17, x18\n" 
			"fcvt.f16.ps f28, f21\n"
			"li x24, 0x80bd2cc028\n"
			"la x18, mem_ptr_offset\n"
			"flw.ps f28, 256(x18)\n"
			"fgwg.ps f9, f28 (x24)\n" 
			"fsetm.pi m5, f16\n"
			"li x7, 0x80c9384008\n"
			"la x24, mem_ptr_offset\n"
			"flw.ps f20, 768(x24)\n"
			"fscwl.ps f9, f20 (x7)\n" 
			"fnmadd.ps f21, f10, f24, f9, rup\n"
			"li x28, 0x80cd96b028\n"
			"amoxorl.w x24, x11, (x28)\n" 
			"fbcx.ps f5, x18\n"
			"li x14, 0x78bed6126e3ac45b\n"
			"addi x23, x14, 0x19\n" 
			"rem x25, x17, x27\n"
			"bge x28, x15, 1f\n"
			"1:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 320(x7)\n"
			"flw.ps f30, 512(x7)\n"
			"fadd.ps f26, f5, f30, rmm\n" 
			"ebreak\n"
			"bgeu x3, x11, 1f\n"
			"1:\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x10\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f5, 0(x31)\n"
			"flw.ps f8, 2(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x14, 0(x31)\n"
			"lw x17, 1(x31)\n"
			"lw x23, 1(x31)\n"
			"lw x27, 10(x31)\n"
			"lw x28, 14(x31)\n"
			"lw x7, 19(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_15:\n"
			"sraiw x17, x27, 2\n"
			"blt x30, x31, 1f\n"
			"1:\n"
			"and x28, x17, x28\n"
			"mulw x17, x27, x27\n"
			"fcvt.f16.ps f28, f1\n"
			"xori x25, x7, 0x1b\n"
			"fcvt.pwu.ps f20, f5, rup\n"
			"fsub.ps f1, f1, f1, rmm\n"
			"slliw x25, x7, 0\n"
			"sltiu x7, x14, 0x3\n"
			"or x17, x17, x17\n"
			"fminu.pi f1, f1, f1\n"
			"fmsub.s f18, f8, f5, f1, rtz\n"
			"fsub.s f1, f1, f1, rmm\n"
			"addi x23, x7, 0x4\n"
			"sltu x25, x27, x28\n"
			"subw x27, x28, x27\n"
			"fandi.pi f26, f1, 0xac\n"
			"fnmsub.ps f20, f1, f8, f8, rmm\n"
			"fsatu8.pi f18, f1\n"
			"fmsub.ps f2, f1, f1, f8, rtz\n"
			"fpackreph.pi f1, f5\n"
			"bltu x22, x22, 1f\n"
			"1:\n"
			"xor x17, x7, x27\n"
			"fnot.pi f28, f8\n"
			"fnot.pi f10, f1\n"
			"ffrc.ps f18, f5\n"
			"fcvt.ps.f16 f5, f5\n"
			"sub x7, x7, x7\n"
			"feq.ps f1, f1, f1\n"
			"blt x1, x29, 1f\n"
			"1:\n"
			"fnmsub.s f20, f5, f8, f8, rtz\n"
			"flt.ps f2, f8, f8\n"
			"fmul.pi f30, f1, f5\n"
			"addi x17, x17, 0x19\n"
			"or x25, x7, x27\n"
			"fand.pi f26, f1, f1\n"
			"div x18, x14, x27\n"
			"bgeu x2, x26, 1f\n"
			"1:\n"
			"fswizz.ps f5, f5, 0x15\n"
			"fcvt.f16.ps f8, f1\n"
			"fsgnj.ps f30, f8, f5\n"
			"subw x25, x17, x23\n"
			"bgeu x15, x7, 1f\n"
			"1:\n"
			"addiw x7, x7, 0xc\n"
			"fcmovm.ps f26, f5, f8\n"
			"remu x0, x17, x17\n"
			"bgeu x19, x10, 1f\n"
			"1:\n"
			"for.pi f5, f5, f8\n"
			"fsub.pi f30, f8, f8\n"
			"srl x27, x17, x17\n"
			"fsgnjx.ps f1, f1, f5\n"
			"fslli.pi f8, f5, 0xa\n"
			"beq x15, x22, 1f\n"
			"1:\n"
			"fsub.pi f26, f5, f1\n"
			"fcvt.ps.f16 f5, f8\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_1_LOOP_SEQ_HID_15\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_HID_15:\n"
			"li x17, 0x808b8b8f08\n"
			"li x7, 2274984562\n"
			"fsc32b.ps f24, x23 (x17)\n" 
			"li x7, 0x80c8373000\n"
			"amominl.d x24, x20, (x7)\n" 
			"li x24, 0x808badaad0\n"
			"fsq2 f24, 128(x24)\n" 
			"li x24, 0x80ce99002c\n"
			"amoxorl.w x28, x27, (x24)\n" 
			"csrr x0, sip\n"
			"li x28, 0x80bc877020\n"
			"amomaxg.d x23, x27, (x28)\n" 
			"li x27, 0x80cf982008\n"
			"amominl.d x11, x24, (x27)\n" 
			"li x28, 0x80b81b9018\n"
			"la x27, mem_ptr_offset\n"
			"flw.ps f18, 544(x27)\n"
			"famoxorg.pi f10, f18 (x28)\n" 
			"li x7, 0x80c9a1f010\n"
			"amoaddl.w x18, x23, (x7)\n" 
			"la x23, rand_int_32\n"
			"flw.ps f12, 704(x23)\n"
			"fcvt.ps.pwu f20, f12, rtz\n" 
			"li x7, 0x80cfee7024\n"
			"amomaxl.w x25, x20, (x7)\n" 
			"li x23, 0x80c8443010\n"
			"la x14, mem_ptr_offset\n"
			"flw.ps f18, 736(x14)\n"
			"fgwl.ps f21, f18 (x23)\n" 
			"li x11, 0x80bee6f020\n"
			"la x23, mem_ptr_offset\n"
			"flw.ps f9, 0(x23)\n"
			"famomaxg.ps f8, f9 (x11)\n" 
			"li x25, 0x80ccbe0008\n"
			"la x27, mem_ptr_offset\n"
			"flw.ps f24, 640(x27)\n"
			"famominl.ps f5, f24 (x25)\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f5, 800(x18)\n"
			"frcp.ps f24, f5\n" 
			"li x17, 0x80bd3fa028\n"
			"amomaxg.w x17, x17, (x17)\n" 
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x30\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f12, 1(x31)\n"
			"flw.ps f16, 6(x31)\n"
			"flw.ps f2, 5(x31)\n"
			"flw.ps f20, 11(x31)\n"
			"flw.ps f28, 10(x31)\n"
			"flw.ps f31, 3(x31)\n"
			"flw.ps f5, 14(x31)\n"
			"flw.ps f8, 31(x31)\n"
			"li x31, 0x2\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_15:\n"
			"fclass.ps f18, f2\n"
			"fsat8.pi f12, f5\n"
			"blt x28, x29, 1f\n"
			"1:\n"
			"fpackreph.pi f16, f31\n"
			"feq.pi f9, f12, f10\n"
			"bne x18, x12, 1f\n"
			"1:\n"
			"fcvt.ps.pw f18, f20, rtz\n"
			"blt x15, x27, 1f\n"
			"1:\n"
			"fmin.s f2, f20, f2\n"
			"fsgnjx.s f12, f20, f8\n"
			"fle.pi f20, f28, f20\n"
			"fclass.ps f8, f2\n"
			"fmadd.ps f18, f20, f5, f16, rdn\n"
			"bgeu x23, x17, 1f\n"
			"1:\n"
			"fsub.ps f16, f16, f28, rdn\n"
			"fsgnjn.s f28, f2, f28\n"
			"fmul.ps f24, f10, f12, rdn\n"
			"fmax.s f20, f31, f20\n"
			"fcmovm.ps f8, f8, f31\n"
			"fmul.pi f8, f16, f2\n"
			"fcmov.ps f31, f20, f31, f20\n"
			"fcmovm.ps f20, f2, f20\n"
			"fmul.s f12, f10, f20, rup\n"
			"bge x13, x11, 1f\n"
			"1:\n"
			"fsub.s f28, f5, f28, rup\n"
			"fadd.s f26, f10, f28, rdn\n"
			"fsub.ps f8, f8, f28, rtz\n"
			"fmaxu.pi f18, f5, f8\n"
			"fmulhu.pi f9, f16, f16\n"
			"bne x20, x13, 1f\n"
			"1:\n"
			"fsgnjn.s f28, f28, f20\n"
			"fsub.pi f18, f10, f2\n"
			"bne x24, x2, 1f\n"
			"1:\n"
			"fmax.s f9, f28, f28\n"
			"beq x16, x30, 1f\n"
			"1:\n"
			"fle.ps f31, f31, f31\n"
			"fround.ps f10, f31, rne\n"
			"fmsub.s f8, f12, f31, f16, rne\n"
			"bgeu x5, x28, 1f\n"
			"1:\n"
			"fmax.s f31, f5, f10\n"
			"fsrl.pi f20, f20, f20\n"
			"fpackreph.pi f20, f20\n"
			"fsgnjn.ps f8, f8, f5\n"
			"feq.ps f12, f2, f16\n"
			"bne x5, x20, 1f\n"
			"1:\n"
			"fround.ps f24, f12, rmm\n"
			"fcvt.ps.pwu f24, f12, rmm\n"
			"fmin.s f18, f5, f10\n"
			"beq x26, x9, 1f\n"
			"1:\n"
			"fmsub.s f8, f10, f12, f20, rdn\n"
			"fmul.s f2, f8, f2, rne\n"
			"fround.ps f16, f10, rup\n"
			"fcmov.ps f12, f5, f2, f12\n"
			"fmulh.pi f18, f2, f31\n"
			"fsgnjx.ps f18, f20, f28\n"
			"fsub.ps f16, f31, f31, rne\n"
			"fmax.s f10, f16, f28\n"
			"fsgnjn.ps f24, f5, f12\n"
			"bgeu x29, x8, 1f\n"
			"1:\n"
			"fmax.ps f10, f28, f28\n"
			"fclass.ps f24, f16\n"
			"flt.ps f5, f5, f5\n"
			"fsatu8.pi f20, f20\n"
			"fcmov.ps f26, f5, f20, f2\n"
			"fadd.ps f8, f28, f2, rup\n"
			"bgeu x21, x12, 1f\n"
			"1:\n"
			"fpackreph.pi f10, f8\n"
			"bltu x0, x2, 1f\n"
			"1:\n"
			"fsrai.pi f5, f5, 0x2\n"
			"bgeu x24, x19, 1f\n"
			"1:\n"
			"fsrai.pi f16, f28, 0x3\n"
			"fmul.pi f10, f5, f12\n"
			"fadd.s f18, f31, f16, rmm\n"
			"fand.pi f18, f31, f8\n"
			"fsatu8.pi f12, f12\n"
			"fsgnjn.s f12, f10, f8\n"
			"fmsub.s f9, f12, f28, f5, rne\n"
			"fclass.ps f20, f10\n"
			"bge x11, x31, 1f\n"
			"1:\n"
			"fsll.pi f10, f10, f20\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_2_LOOP_SEQ_HID_15\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f5,  736 (x5)\n"
			"flw.ps f6,  480 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f24,  192 (x5)\n"
			"flw.ps f26,  896 (x5)\n"
			"flw.ps f16,  64 (x5)\n"
			"flw.ps f15,  992 (x5)\n"
			"flw.ps f31,  544 (x5)\n"
			"flw.ps f2,  320 (x5)\n"
			"flw.ps f27,  352 (x5)\n"
			"flw.ps f25,  32 (x5)\n"
			"flw.ps f17,  704 (x5)\n"
			"flw.ps f11,  512 (x5)\n"
			"flw.ps f10,  832 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f29,  640 (x5)\n"
			"flw.ps f13,  416 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f4,  672 (x5)\n"
			"flw.ps f0,  928 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
			"flw.ps f22,  288 (x5)\n"
			"flw.ps f7,  256 (x5)\n"
			"flw.ps f21,  576 (x5)\n"
			"flw.ps f9,  608 (x5)\n"
			"flw.ps f28,  768 (x5)\n"
			"flw.ps f23,  224 (x5)\n"
			"flw.ps f1,  448 (x5)\n"
			"flw.ps f3,  0 (x5)\n"
			"flw.ps f14,  160 (x5)\n"
			"flw.ps f19,  96 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_3_HID_15:\n"
			"li x25, 0xe67de6866cf7fe0d\n"
			"fcvt.s.wu f8, x25, rne\n" 
			"sub x11, x28, x28\n"
			"fmin.s f12, f2, f2\n"
			"remu x7, x28, x18\n"
			"fsgnjx.s f10, f2, f20\n"
			"or x18, x7, x18\n"
			"fsrai.pi f5, f18, 0xd\n"
			"blt x2, x13, 1f\n"
			"1:\n"
			"li x11, 0x80cb910028\n"
			"la x23, mem_ptr_offset\n"
			"flw.ps f5, 256(x23)\n"
			"famoandl.pi f26, f5 (x11)\n" 
			"fsub.ps f28, f10, f20, rmm\n"
			"fcvt.f16.ps f24, f9\n"
			"flt.ps f26, f20, f26\n"
			"fcvt.ps.pwu f31, f5, rtz\n"
			"fmul.ps f9, f9, f9, rne\n"
			"fsat8.pi f2, f5\n"
			"li x27, 0x80cc7e300c\n"
			"amominul.w x17, x24, (x27)\n" 
			"fsgnjx.ps f18, f20, f5\n"
			"fsatu8.pi f1, f24\n"
			"fexp.ps f12, f28\n"
			"fnot.pi f5, f24\n"
			"bgeu x6, x14, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f26, f31, rmm\n"
			"fand.pi f1, f10, f8\n"
			"li x18, 0x80bd2d4030\n"
			"la x20, mem_ptr_offset\n"
			"flw.ps f21, 448(x20)\n"
			"famoandg.pi f20, f21 (x18)\n" 
			"fence\n"
			"bge x10, x2, 1f\n"
			"1:\n"
			"fcmov.ps f30, f30, f26, f9\n"
			"fcmov.ps f30, f30, f28, f20\n"
			"fmax.pi f16, f28, f1\n"
			"slli x20, x27, 2\n"
			"bne x5, x18, 1f\n"
			"1:\n"
			"fnmsub.ps f8, f28, f28, f1, rdn\n"
			"li x23, 0x80ce570010\n"
			"amominul.d x20, x27, (x23)\n" 
			"fsrai.pi f30, f2, 0xc\n"
			"fadd.s f10, f21, f10, rmm\n"
			"fsgnjx.s f8, f1, f12\n"
			"fle.pi f30, f20, f12\n"
			"fsrl.pi f16, f10, f2\n"
			"fsrl.pi f26, f12, f28\n"
			"beq x24, x21, 1f\n"
			"1:\n"
			"div x27, x25, x23\n" 
			"faddi.pi f24, f31, 0x14c\n"
			"packb x0, x24, x18\n"
			"fcvt.ps.pw f24, f2, rtz\n"
			"bge x16, x18, 1f\n"
			"1:\n"
			"flog.ps f28, f31\n"
			"flt.ps f18, f5, f18\n"
			"fsgnj.ps f24, f28, f31\n"
			"bge x13, x5, 1f\n"
			"1:\n"
			"fbci.ps f21, 0x4fbfd\n" 
			"fcvt.ps.pwu f10, f18, rmm\n"
			"beq x19, x22, 1f\n"
			"1:\n"
			"fround.ps f1, f8, rtz\n"
			"fxor.pi f30, f31, f1\n"
			"fclass.ps f28, f21\n"
			"faddi.pi f10, f10, 0xca\n"
			"beq x13, x7, 1f\n"
			"1:\n"
			"fnmadd.ps f2, f28, f1, f1, rtz\n"
			"bltu x8, x16, 1f\n"
			"1:\n"
			"li x20, 0x136b9dcddeba969b\n"
			"srlw x18, x20, x20\n" 
			"fcvt.s.w f9, x28, rup\n"
			"fsetm.pi m3, f26\n"
			"sraw x28, x28, x11\n"
			"fadd.pi f10, f8, f10\n"
			"addi x7, x17, 0x13\n"
			"fcvt.ps.pw f26, f12, rup\n"
			"li x20, 0x80cfee801c\n"
			"amoaddl.w x14, x27, (x20)\n" 
			"fsrai.pi f21, f8, 0xe\n"
			"fmaxu.pi f10, f10, f10\n"
			"fcvt.ps.pw f8, f20, rtz\n"
			"fround.ps f12, f20, rup\n"
			"fsrli.pi f21, f20, 0x9\n"
			"fandi.pi f24, f21, 0x1d0\n"
			"blt x5, x26, 1f\n"
			"1:\n"
			"li x11, 0x80cb3ad038\n"
			"la x28, mem_ptr_offset\n"
			"flw.ps f20, 160(x28)\n"
			"famomaxul.pi f8, f20 (x11)\n" 
			"divu x7, x17, x27\n"
			"fmulh.pi f9, f31, f9\n"
			"remuw x7, x14, x27\n"
			"fle.pi f31, f21, f31\n"
			"add x7, x27, x17\n"
			"for.pi f30, f21, f18\n"
			"li x14, 0x80c8bf8028\n"
			"la x17, mem_ptr_offset\n"
			"flw.ps f28, 992(x17)\n"
			"fghl.ps f8, f28 (x14)\n" 
			"fcvt.ps.pwu f21, f10, rtz\n"
			"fsatu8.pi f9, f12\n"
			"fnmadd.s f5, f31, f10, f8, rdn\n"
			"bgeu x5, x8, 1f\n"
			"1:\n"
			"fnmsub.ps f5, f30, f5, f20, rdn\n"
			"ffrc.ps f12, f12\n"
			"fmaxu.pi f16, f5, f10\n"
			"div x18, x7, x23\n" 
			"addw x27, x27, x27\n"
			"fpackrepb.pi f21, f31\n"
			"fnot.pi f21, f21\n"
			"sub x27, x27, x23\n"
			"ori x14, x14, 0x17\n"
			"fadd.pi f8, f31, f31\n"
			"li x24, 0x362d902a7d33b765\n"
			"srliw x23, x24, 3\n" 
			"maskpopcz x23, m3\n"
			"sltiu x0, x0, 0x2\n"
			"srliw x20, x0, 2\n"
			"fclass.s x20, f24\n"
			"bge x9, x4, 1f\n"
			"1:\n"
			"fltu.pi f16, f16, f16\n"
			"beq x12, x10, 1f\n"
			"1:\n"
			"sllw x23, x23, x18\n"
			"li x7, 0x8088a1b178\n"
			"fsw.ps f10, 256(x7)\n"
			"fbc.ps f1, 256(x7)\n" 
			"faddi.pi f10, f18, 0x152\n"
			"fmin.ps f10, f26, f18\n"
			"ecall\n"
			"fsatu8.pi f10, f18\n"
			"fsgnjx.s f5, f8, f26\n"
			"srli x27, x17, 3\n"
			"li x11, 0x808ca11b5f\n"
			"sd x14, 128(x11)\n"
			"lb x20, 128(x11)\n" 
			"fsrl.pi f18, f20, f21\n"
			"blt x21, x15, 1f\n"
			"1:\n"
			"frcp.ps f20, f5\n"
			"beq x2, x17, 1f\n"
			"1:\n"
			"ffrc.ps f30, f21\n"
			"fnmadd.ps f20, f20, f20, f28, rdn\n"
			"fsat8.pi f9, f26\n"
			"fsrl.pi f5, f8, f30\n"
			"bne x13, x18, 1f\n"
			"1:\n"
			"li x14, 0x80cf035000\n"
			"la x23, mem_ptr_offset\n"
			"flw.ps f10, 864(x23)\n"
			"famoorl.pi f16, f10 (x14)\n" 
			"fmin.ps f10, f18, f10\n"
			"bne x6, x0, 1f\n"
			"1:\n"
			"fle.ps f16, f16, f21\n"
			"beq x13, x0, 1f\n"
			"1:\n"
			"fnmsub.ps f16, f16, f16, f21, rne\n"
			"fmadd.ps f1, f18, f10, f28, rmm\n"
			"feq.ps f16, f1, f1\n"
			"fmin.pi f16, f28, f10\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f10, 1(x31)\n"
			"flw.ps f12, 5(x31)\n"
			"flw.ps f16, 0(x31)\n"
			"flw.ps f18, 15(x31)\n"
			"flw.ps f2, 16(x31)\n"
			"flw.ps f20, 1(x31)\n"
			"flw.ps f21, 24(x31)\n"
			"flw.ps f24, 6(x31)\n"
			"flw.ps f26, 16(x31)\n"
			"flw.ps f28, 5(x31)\n"
			"flw.ps f30, 31(x31)\n"
			"flw.ps f31, 31(x31)\n"
			"flw.ps f5, 25(x31)\n"
			"flw.ps f8, 3(x31)\n"
			"flw.ps f9, 25(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x11, 2(x31)\n"
			"lw x14, 4(x31)\n"
			"lw x17, 6(x31)\n"
			"lw x18, 10(x31)\n"
			"lw x20, 14(x31)\n"
			"lw x23, 17(x31)\n"
			"lw x24, 8(x31)\n"
			"lw x25, 0(x31)\n"
			"lw x27, 22(x31)\n"
			"lw x28, 11(x31)\n"
			"lw x7, 4(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_3_LOOP_SEQ_HID_15:\n"
			"or x7, x18, x24\n"
			"flog.ps f18, f2\n"
			"remuw x7, x20, x0\n"
			"blt x1, x23, 1f\n"
			"1:\n"
			"fsub.ps f20, f1, f20, rup\n"
			"feq.ps f1, f1, f12\n"
			"fmsub.s f5, f18, f28, f9, rne\n"
			"fmv.s.x f9, x17\n"
			"bne x4, x12, 1f\n"
			"1:\n"
			"fcvt.ps.pw f21, f1, rup\n"
			"fnot.pi f1, f10\n"
			"fpackreph.pi f24, f26\n"
			"fexp.ps f21, f8\n"
			"frcp.ps f30, f1\n"
			"fle.ps f5, f10, f24\n"
			"mul x28, x28, x14\n"
			"mulhsu x17, x28, x0\n"
			"fsrli.pi f30, f21, 0x3\n"
			"fmulh.pi f31, f20, f31\n"
			"divw x0, x11, x0\n"
			"fnot.pi f24, f28\n"
			"flt.s x23, f30, f5\n"
			"frcp.ps f28, f1\n"
			"sll x18, x18, x25\n"
			"fcvt.wu.s x14, f30, rmm\n"
			"bge x20, x19, 1f\n"
			"1:\n"
			"fpackrepb.pi f16, f10\n"
			"fxor.pi f24, f24, f12\n"
			"srl x24, x7, x23\n"
			"fsub.pi f21, f12, f1\n"
			"fcvt.wu.s x23, f12, rdn\n"
			"fsetm.pi m6, f8\n"
			"fsgnjn.s f2, f24, f2\n"
			"fexp.ps f18, f20\n"
			"fmul.pi f12, f21, f12\n"
			"fmadd.ps f21, f12, f2, f10, rmm\n"
			"fsgnjn.ps f28, f10, f18\n"
			"fminu.pi f5, f28, f2\n"
			"fbci.ps f26, 0x39a13\n"
			"sra x11, x24, x7\n"
			"fsrai.pi f21, f9, 0x9\n"
			"fmaxu.pi f9, f8, f9\n"
			"ecall\n"
			"fxor.pi f5, f28, f28\n"
			"fle.ps f18, f1, f18\n"
			"fnmsub.ps f9, f30, f9, f18, rmm\n"
			"maskand m2, m6, m3\n"
			"fmulhu.pi f8, f8, f30\n"
			"fclass.ps f1, f10\n"
			"frcp.ps f31, f1\n"
			"blt x2, x10, 1f\n"
			"1:\n"
			"fcmov.ps f8, f24, f28, f18\n"
			"and x18, x14, x0\n"
			"addi x20, x20, 0x10\n"
			"flt.ps f26, f2, f26\n"
			"frcp.ps f1, f26\n"
			"fmul.s f16, f31, f8, rtz\n"
			"mul x20, x14, x20\n"
			"fsgnjn.s f21, f20, f16\n"
			"srliw x14, x25, 5\n"
			"fand.pi f18, f21, f12\n"
			"bltu x19, x30, 1f\n"
			"1:\n"
			"fsgnjx.s f5, f12, f5\n"
			"slliw x25, x11, 5\n"
			"fsrli.pi f28, f31, 0xe\n"
			"fmax.s f20, f20, f18\n"
			"divw x18, x27, x0\n"
			"fsetm.pi m3, f28\n"
			"beq x31, x8, 1f\n"
			"1:\n"
			"fltm.ps m5, f30, f21\n"
			"ori x0, x17, 0x1f\n"
			"fnmadd.ps f16, f8, f21, f16, rdn\n"
			"flt.pi f10, f10, f1\n"
			"fadd.pi f18, f28, f24\n"
			"mov.m.x m2, x27, 0x79\n"
			"fsgnjn.ps f5, f28, f1\n"
			"fmadd.s f12, f20, f16, f31, rup\n"
			"blt x17, x11, 1f\n"
			"1:\n"
			"fltm.ps m5, f30, f9\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_3_LOOP_SEQ_HID_15\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_4_HID_15:\n"
			"li x27, 0x808e8efdd8\n"
			"fsw.ps f28, 128(x27)\n"
			"flw.ps f30, 128(x27)\n" 
			"mov.m.x m2, x25, 0x76\n"
			"li x24, 0x808a4346e7\n"
			"sd x18, 256(x24)\n"
			"lw x11, 256(x24)\n" 
			"fmsub.s f5, f5, f18, f12, rtz\n"
			"li x11, 0x7e65e1f8ddf17f55\n"
			"li x23, 0x23edde1dbfecc46b\n"
			"subw x27, x11, x23\n" 
			"feq.s x7, f18, f18\n"
			"li x24, 0x808cedea90\n"
			"fsw.ps f31, -256(x24)\n"
			"fbc.ps f1, -256(x24)\n" 
			"add x0, x0, x28\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f8, 992(x20)\n"
			"flw.ps f16, 384(x20)\n"
			"fsgnjn.ps f26, f8, f16\n" 
			"andi x20, x20, 0xa\n"
			"li x23, 0x80bacf7000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f16, (x23)\n" 
			"mova.m.x x28\n"
			"li x11, 0x48284eda0a27d13c\n"
			"slliw x24, x11, 5\n" 
			"fsgnjx.ps f21, f31, f1\n"
			"csrr x0, hartid\n"
			"fsgnj.s f26, f1, f21\n"
			"li x28, 0x80ba68a000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f18, (x28)\n"
			"flwg.ps f20, (x28)\n" 
			"fround.ps f1, f30, rdn\n"
			"li x23, 0x808d1048f8\n"
			"fsw.ps f31, -128(x23)\n" 
			"fand.pi f16, f8, f31\n"
			"li x14, 0x80be971030\n"
			"amoaddg.d x28, x28, (x14)\n" 
			"fmul.s f18, f21, f24, rne\n"
			"li x23, 0x80c9fd7038\n"
			"la x24, mem_ptr_offset\n"
			"flw.ps f21, 864(x24)\n"
			"fscbl.ps f9, f21 (x23)\n" 
			"fsra.pi f26, f2, f28\n"
			"li x14, 0x80bf59a008\n"
			"la x17, mem_ptr_offset\n"
			"flw.ps f9, 960(x17)\n"
			"famoswapg.pi f18, f9 (x14)\n" 
			"fmsub.s f2, f16, f28, f10, rdn\n"
			"li x27, 0x808df99acc\n"
			"sb x25, 512(x27)\n" 
			"fmul.s f2, f24, f16, rdn\n"
			"li x18, 0x80cb8b2000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f10, (x18)\n"
			"flwl.ps f31, (x18)\n" 
			"mulhsu x7, x25, x14\n"
			"beq x21, x10, 1f\n"
			"1:\n"
			"li x24, 0x80ba2a7008\n"
			"la x17, mem_ptr_offset\n"
			"flw.ps f21, 160(x17)\n"
			"famoming.ps f9, f21 (x24)\n" 
			"fle.s x0, f18, f31\n"
			"bltu x9, x19, 1f\n"
			"1:\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x28\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f12, 2(x31)\n"
			"flw.ps f16, 3(x31)\n"
			"flw.ps f21, 8(x31)\n"
			"flw.ps f26, 13(x31)\n"
			"flw.ps f31, 10(x31)\n"
			"flw.ps f5, 5(x31)\n"
			"flw.ps f8, 1(x31)\n"
			"flw.ps f9, 27(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x11, 1(x31)\n"
			"lw x14, 2(x31)\n"
			"lw x17, 6(x31)\n"
			"lw x20, 5(x31)\n"
			"lw x23, 17(x31)\n"
			"lw x24, 19(x31)\n"
			"lw x25, 4(x31)\n"
			"lw x27, 2(x31)\n"
			"lw x28, 11(x31)\n"
			"lw x7, 33(x31)\n"
			"li x31, 0x1\n"
			"LBL_SEQID_4_LOOP_SEQ_HID_15:\n"
			"remw x24, x28, x24\n"
			"fbci.ps f12, 0x41eb7\n"
			"fltm.pi m5, f12, f10\n"
			"fmin.ps f28, f12, f8\n"
			"fsetm.pi m2, f12\n"
			"bne x1, x18, 1f\n"
			"1:\n"
			"fcvt.s.w f9, x27, rup\n"
			"fcmov.ps f26, f21, f16, f26\n"
			"fmvz.x.ps x17, f9, 2\n"
			"fmvz.x.ps x18, f9, 3\n"
			"fsat8.pi f2, f8\n"
			"flog.ps f20, f26\n"
			"fle.ps f26, f16, f8\n"
			"mova.m.x x14\n"
			"divuw x23, x23, x25\n"
			"fltm.pi m2, f26, f12\n"
			"fxor.pi f1, f16, f16\n"
			"maskand m6, m6, m6\n"
			"blt x7, x11, 1f\n"
			"1:\n"
			"fmv.x.s x27, f12\n"
			"fsgnj.ps f1, f16, f26\n"
			"fltu.pi f10, f12, f10\n"
			"fmadd.s f26, f12, f10, f21, rmm\n"
			"fadd.ps f2, f10, f16, rmm\n"
			"maskpopc x24, m6\n"
			"blt x29, x19, 1f\n"
			"1:\n"
			"fmsub.s f18, f10, f16, f5, rdn\n"
			"fsgnjx.s f31, f16, f26\n"
			"auipc x25, 0x16\n"
			"fcvt.s.w f28, x14, rne\n"
			"fmax.ps f12, f21, f8\n"
			"fle.ps f31, f16, f31\n"
			"fltm.pi m3, f26, f12\n"
			"fsgnjn.ps f2, f26, f16\n"
			"packb x28, x28, x14\n"
			"bltu x21, x26, 1f\n"
			"1:\n"
			"mulhu x23, x11, x20\n"
			"add x14, x14, x7\n"
			"srli x27, x17, 0\n"
			"feqm.ps m5, f26, f26\n"
			"divu x20, x24, x20\n"
			"feq.ps f9, f26, f5\n"
			"fnmadd.ps f12, f21, f26, f10, rtz\n"
			"beq x21, x4, 1f\n"
			"1:\n"
			"fsrai.pi f10, f16, 0x9\n"
			"srlw x24, x24, x23\n"
			"mova.m.x x24\n"
			"fnmsub.s f8, f10, f10, f5, rup\n"
			"srlw x25, x14, x7\n"
			"fsgnjn.ps f8, f5, f9\n"
			"mul x24, x28, x24\n"
			"fsrl.pi f31, f21, f21\n"
			"fswizz.ps f26, f12, 0x1b\n"
			"remu x27, x28, x28\n"
			"slliw x17, x27, 2\n"
			"fexp.ps f2, f12\n"
			"bltu x16, x23, 1f\n"
			"1:\n"
			"fsatu8.pi f20, f26\n"
			"addw x24, x0, x23\n"
			"remw x7, x7, x0\n"
			"blt x31, x3, 1f\n"
			"1:\n"
			"fle.ps f2, f5, f8\n"
			"remuw x27, x27, x24\n"
			"slt x20, x25, x20\n"
			"bge x25, x4, 1f\n"
			"1:\n"
			"mulhsu x0, x0, x24\n"
			"flem.ps m5, f16, f10\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_4_LOOP_SEQ_HID_15\n"
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f14,  864 (x5)\n"
			"flw.ps f31,  736 (x5)\n"
			"flw.ps f4,  512 (x5)\n"
			"flw.ps f24,  480 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f18,  992 (x5)\n"
			"flw.ps f8,  640 (x5)\n"
			"flw.ps f13,  768 (x5)\n"
			"flw.ps f29,  448 (x5)\n"
			"flw.ps f9,  800 (x5)\n"
			"flw.ps f2,  192 (x5)\n"
			"flw.ps f10,  224 (x5)\n"
			"flw.ps f0,  64 (x5)\n"
			"flw.ps f1,  416 (x5)\n"
			"flw.ps f17,  0 (x5)\n"
			"flw.ps f12,  704 (x5)\n"
			"flw.ps f26,  32 (x5)\n"
			"flw.ps f19,  896 (x5)\n"
			"flw.ps f28,  128 (x5)\n"
			"flw.ps f27,  352 (x5)\n"
			"flw.ps f6,  832 (x5)\n"
			"flw.ps f25,  576 (x5)\n"
			"flw.ps f16,  96 (x5)\n"
			"flw.ps f3,  544 (x5)\n"
			"flw.ps f20,  384 (x5)\n"
			"flw.ps f23,  672 (x5)\n"
			"flw.ps f30,  928 (x5)\n"
			"flw.ps f11,  256 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f5,  288 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f15,  160 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_5_HID_15:\n"
			"li x11, 0x80bfa15000\n"
			"la x20, mem_ptr_offset\n"
			"flw.ps f9, 64(x20)\n"
			"famoaddg.pi f10, f9 (x11)\n" 
			"fmul.ps f1, f20, f8, rne\n"
			"li x14, 0x80895736de\n"
			"sd x17, 512(x14)\n"
			"ld x17, 512(x14)\n" 
			"fsgnjx.ps f2, f18, f28\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f8, 960(x28)\n"
			"flw.ps f9, 0(x28)\n"
			"flt.ps f18, f8, f9\n" 
			"fpackreph.pi f16, f1\n"
			"bltu x15, x20, 1f\n"
			"1:\n"
			"li x17, 0x80c9967030\n"
			"la x18, mem_ptr_offset\n"
			"flw.ps f16, 736(x18)\n"
			"famoswapl.pi f18, f16 (x17)\n" 
			"fsgnjn.s f20, f20, f9\n"
			"li x7, 0x519ecdeff65df845\n"
			"fcvt.s.l f30, x7, rup\n" 
			"fcvt.ps.pw f10, f20, rmm\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f26, 992(x7)\n"
			"frcp.ps f26, f26\n" 
			"fnmadd.s f12, f12, f9, f16, rne\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f10, 96(x27)\n"
			"flw.ps f16, 608(x27)\n"
			"flw.ps f18, 416(x27)\n"
			"fmadd.s f28, f10, f16, f18, rup\n" 
			"fle.pi f16, f12, f20\n"
			"li x28, 0x80cc5a6020\n"
			"la x24, mem_ptr_offset\n"
			"flw.ps f9, 576(x24)\n"
			"famomaxl.ps f31, f9 (x28)\n" 
			"fpackreph.pi f28, f12\n"
			"la x27, rand_int_32\n"
			"flw.ps f24, 800(x27)\n"
			"flw.ps f26, 192(x27)\n"
			"fsra.pi f5, f24, f26\n" 
			"fswizz.ps f16, f21, 0x1f\n"
			"li x18, 0x80bd6a1024\n"
			"amomaxg.w x18, x20, (x18)\n" 
			"fmsub.ps f18, f12, f20, f20, rtz\n"
			"li x11, 0x80bd13f004\n"
			"amomaxug.w x24, x25, (x11)\n" 
			"fmin.ps f30, f26, f16\n"
			"li x14, 0x80be63c000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f21, (x14)\n" 
			"fmul.ps f12, f18, f10, rtz\n"
			"li x7, 0x8088956dc0\n"
			"la x25, mem_ptr_offset\n"
			"flw.ps f30, 640(x25)\n"
			"fsch.ps f5, f30 (x7)\n" 
			"fexp.ps f8, f1\n"
			"li x20, 0x808e767668\n"
			"la x25, mem_ptr_offset\n"
			"flw.ps f5, 192(x25)\n"
			"fgb.ps f16, f5 (x20)\n" 
			"fadd.s f16, f28, f21, rtz\n"
			"li x24, 0x808d47f3ec\n"
			"sd x20, 256(x24)\n"
			"lw x27, 256(x24)\n" 
			"fsgnjx.s f2, f24, f21\n"
			"li x23, 0x4067324ec8a40bc0\n"
			"li x24, 0x6f6bde6606205598\n"
			"sllw x11, x23, x24\n" 
			"rem x20, x20, x20\n"
			"bgeu x17, x28, 1f\n"
			"1:\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x60\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f12, 2(x31)\n"
			"flw.ps f24, 1(x31)\n"
			"flw.ps f28, 0(x31)\n"
			"flw.ps f31, 2(x31)\n"
			"flw.ps f5, 13(x31)\n"
			"flw.ps f8, 0(x31)\n"
			"flw.ps f9, 0(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x11, 3(x31)\n"
			"lw x14, 1(x31)\n"
			"lw x17, 11(x31)\n"
			"lw x18, 13(x31)\n"
			"lw x20, 9(x31)\n"
			"lw x23, 21(x31)\n"
			"lw x25, 21(x31)\n"
			"lw x27, 2(x31)\n"
			"lw x28, 22(x31)\n"
			"lw x7, 28(x31)\n"
			"li x31, 0x2\n"
			"LBL_SEQID_5_LOOP_SEQ_HID_15:\n"
			"fmsub.ps f21, f28, f12, f9, rmm\n"
			"bne x15, x5, 1f\n"
			"1:\n"
			"fsub.pi f1, f8, f12\n"
			"sub x25, x0, x20\n"
			"div x11, x11, x17\n"
			"xor x14, x14, x11\n"
			"bgeu x12, x30, 1f\n"
			"1:\n"
			"fsgnjx.s f30, f10, f5\n"
			"faddi.pi f24, f8, 0x151\n"
			"packb x17, x17, x23\n"
			"xor x7, x20, x18\n"
			"srl x28, x20, x17\n"
			"fxor.pi f8, f5, f8\n"
			"feq.ps f16, f10, f5\n"
			"fence\n"
			"blt x12, x5, 1f\n"
			"1:\n"
			"mul x0, x28, x25\n"
			"bge x23, x14, 1f\n"
			"1:\n"
			"subw x17, x20, x17\n"
			"sllw x28, x23, x18\n"
			"bne x18, x27, 1f\n"
			"1:\n"
			"fnmadd.s f26, f28, f10, f9, rup\n"
			"fmulh.pi f16, f10, f24\n"
			"slliw x11, x20, 1\n"
			"packb x28, x28, x11\n"
			"bgeu x8, x5, 1f\n"
			"1:\n"
			"remu x17, x20, x0\n"
			"fand.pi f9, f9, f12\n"
			"xor x11, x20, x11\n"
			"divw x11, x23, x11\n"
			"fpackrepb.pi f9, f24\n"
			"fand.pi f24, f8, f24\n"
			"bgeu x18, x17, 1f\n"
			"1:\n"
			"fsrai.pi f16, f10, 0x2\n"
			"fslli.pi f9, f28, 0x9\n"
			"maskxor m6, m2, m6\n"
			"fsrl.pi f8, f24, f9\n"
			"fsgnj.ps f21, f9, f5\n"
			"sub x27, x27, x7\n"
			"fswizz.ps f9, f9, 0xe\n"
			"mova.x.m x23\n"
			"slliw x17, x11, 1\n"
			"mulhu x23, x28, x7\n"
			"bgeu x20, x16, 1f\n"
			"1:\n"
			"xori x17, x23, 0xe\n"
			"flem.ps m5, f12, f31\n"
			"faddi.pi f31, f9, 0xe\n"
			"fcvt.f16.ps f28, f28\n"
			"fxor.pi f8, f5, f31\n"
			"rem x28, x18, x0\n"
			"fsub.ps f1, f24, f9, rne\n"
			"sltiu x20, x7, 0x2\n"
			"bgeu x31, x0, 1f\n"
			"1:\n"
			"feq.ps f20, f12, f12\n"
			"srl x23, x0, x23\n"
			"ori x20, x25, 0x8\n"
			"lui x24, 0xa\n"
			"maskpopcz x18, m3\n"
			"auipc x17, 0xc\n"
			"fsub.pi f20, f31, f8\n"
			"bltu x27, x3, 1f\n"
			"1:\n"
			"fadd.s f8, f9, f5, rup\n"
			"fle.ps f20, f5, f10\n"
			"mova.x.m x24\n"
			"sub x23, x20, x25\n"
			"fmadd.ps f2, f12, f9, f31, rtz\n"
			"fcvt.ps.pwu f1, f10, rdn\n"
			"fcmovm.ps f8, f8, f31\n"
			"or x20, x25, x11\n"
			"fle.pi f8, f5, f9\n"
			"bltu x19, x21, 1f\n"
			"1:\n"
			"mov.m.x m6, x27, 0x13\n"
			"rem x25, x23, x7\n"
			"slt x7, x28, x7\n"
			"blt x8, x5, 1f\n"
			"1:\n"
			"ori x25, x18, 0x19\n"
			"fnmsub.ps f31, f31, f9, f10, rne\n"
			"ebreak\n"
			"fmulhu.pi f28, f28, f28\n"
			"fsgnj.ps f18, f12, f10\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_5_LOOP_SEQ_HID_15\n"
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_6_HID_15:\n"
			"li x28, 0x80cf640038\n"
			"la x17, mem_ptr_offset\n"
			"flw.ps f10, 864(x17)\n"
			"famoaddl.pi f21, f10 (x28)\n" 
			"fround.ps f16, f16, rdn\n"
			"fmax.ps f2, f21, f12\n"
			"fsgnjx.ps f30, f20, f30\n"
			"for.pi f9, f12, f21\n"
			"blt x25, x21, 1f\n"
			"1:\n"
			"fswizz.ps f20, f2, 0x1c\n"
			"fle.ps f16, f28, f26\n"
			"fsll.pi f24, f16, f24\n"
			"fsrl.pi f5, f31, f16\n"
			"fswizz.ps f30, f12, 0x0\n"
			"li x24, 0x80bf451010\n"
			"la x20, mem_ptr_offset\n"
			"flw.ps f10, 416(x20)\n"
			"famoming.pi f30, f10 (x24)\n" 
			"fmax.ps f21, f21, f8\n"
			"sub x20, x28, x24\n"
			"bgeu x14, x0, 1f\n"
			"1:\n"
			"fandi.pi f8, f8, 0x1d5\n"
			"fadd.s f16, f21, f28, rdn\n"
			"fsgnj.s f8, f10, f10\n"
			"bgeu x12, x4, 1f\n"
			"1:\n"
			"fle.ps f8, f30, f2\n"
			"flog.ps f16, f21\n"
			"bne x27, x30, 1f\n"
			"1:\n"
			"sll x23, x7, x24\n"
			"blt x27, x12, 1f\n"
			"1:\n"
			"fsub.s f9, f8, f9, rdn\n"
			"li x7, 0x80ba951028\n"
			"la x24, mem_ptr_offset\n"
			"flw.ps f1, 992(x24)\n"
			"famoming.ps f24, f1 (x7)\n" 
			"sltiu x25, x24, 0x7\n"
			"fswizz.ps f18, f12, 0x8\n"
			"fcvt.w.s x25, f1, rtz\n"
			"fcvt.s.wu f5, x28, rmm\n"
			"fmulh.pi f24, f26, f16\n"
			"fltm.ps m5, f5, f8\n"
			"mov.m.x m6, x18, 0x4\n"
			"mov.m.x m6, x11, 0x64\n"
			"feqm.ps m5, f28, f8\n"
			"li x20, 0x80cff94000\n"
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f2, (x20)\n"
			"flwl.ps f28, (x20)\n" 
			"fmul.s f18, f2, f9, rup\n"
			"fsgnjx.ps f1, f9, f10\n"
			"fmul.ps f5, f20, f10, rup\n"
			"fmulh.pi f5, f9, f20\n"
			"fslli.pi f8, f18, 0xf\n"
			"fmin.s f8, f8, f26\n"
			"beq x25, x31, 1f\n"
			"1:\n"
			"fxor.pi f1, f1, f18\n"
			"fexp.ps f31, f31\n"
			"fmulh.pi f28, f2, f16\n"
			"li x18, 0x808f8ab010\n"
			"la x23, mem_ptr_offset\n"
			"flw.ps f2, 448(x23)\n"
			"fgh.ps f20, f2 (x18)\n" 
			"fle.pi f12, f9, f31\n"
			"fnmsub.s f12, f20, f31, f16, rmm\n"
			"for.pi f10, f10, f8\n"
			"fsub.pi f9, f26, f31\n"
			"fadd.s f16, f26, f31, rup\n"
			"bge x26, x15, 1f\n"
			"1:\n"
			"fmin.ps f12, f9, f8\n"
			"bne x9, x19, 1f\n"
			"1:\n"
			"fmsub.s f10, f31, f26, f8, rmm\n"
			"faddi.pi f21, f16, 0x68\n"
			"beq x20, x28, 1f\n"
			"1:\n"
			"fandi.pi f26, f26, 0x102\n"
			"bge x9, x17, 1f\n"
			"1:\n"
			"li x24, 0x80bf23a028\n"
			"la x28, mem_ptr_offset\n"
			"flw.ps f12, 576(x28)\n"
			"fgbg.ps f21, f12 (x24)\n" 
			"fsatu8.pi f30, f16\n"
			"blt x28, x4, 1f\n"
			"1:\n"
			"andi x27, x17, 0x18\n"
			"fbci.ps f10, 0x68d4c\n"
			"addi x24, x0, 0x16\n"
			"maskpopcz x27, m2\n"
			"divw x11, x27, x11\n"
			"subw x20, x20, x17\n"
			"fltm.pi m5, f30, f24\n"
			"fle.pi f28, f24, f24\n"
			"li x24, 0x80cd57b028\n"
			"la x11, mem_ptr_offset\n"
			"flw.ps f30, 640(x11)\n"
			"fscwl.ps f16, f30 (x24)\n" 
			"fcvt.ps.pw f30, f16, rtz\n"
			"fmsub.s f30, f16, f16, f5, rmm\n"
			"fnmsub.ps f5, f20, f8, f8, rup\n"
			"fmul.ps f1, f21, f21, rne\n"
			"bne x13, x3, 1f\n"
			"1:\n"
			"fmul.ps f30, f8, f31, rmm\n"
			"fmadd.s f31, f20, f8, f31, rdn\n"
			"fsub.s f20, f20, f31, rmm\n"
			"bne x17, x25, 1f\n"
			"1:\n"
			"fclass.ps f1, f8\n"
			"fnot.pi f8, f1\n"
			"blt x23, x29, 1f\n"
			"1:\n"
			"li x17, 0x80cc115008\n"
			"la x25, mem_ptr_offset\n"
			"flw.ps f2, 864(x25)\n"
			"famominl.pi f9, f2 (x17)\n" 
			"fcvt.ps.f16 f31, f1\n"
			"bge x31, x15, 1f\n"
			"1:\n"
			"fsgnjx.s f21, f26, f26\n"
			"fmin.ps f18, f10, f10\n"
			"fnmadd.ps f18, f10, f10, f10, rne\n"
			"fand.pi f1, f10, f26\n"
			"flt.ps f10, f10, f10\n"
			"fsgnj.s f21, f26, f1\n"
			"fsrai.pi f1, f1, 0x8\n"
			"fmul.pi f10, f26, f10\n"
			"li x24, 0x80c85c0020\n"
			"amomaxul.w x28, x20, (x24)\n" 
			"fmsub.s f16, f12, f21, f20, rmm\n"
			"fcvt.pwu.ps f18, f28, rtz\n"
			"fmax.ps f18, f18, f28\n"
			"fnot.pi f2, f12\n"
			"bge x18, x31, 1f\n"
			"1:\n"
			"fpackrepb.pi f2, f12\n"
			"ffrc.ps f16, f20\n"
			"fnot.pi f8, f1\n"
			"bge x30, x30, 1f\n"
			"1:\n"
			"ffrc.ps f28, f21\n"
			"fmax.s f16, f20, f20\n"
			"csrr x0, mhpmcounter5\n"
			"fmul.s f21, f12, f28, rdn\n"
			"slti x14, x14, 0x6\n"
			"flt.s x25, f28, f18\n"
			"fmax.ps f12, f24, f18\n"
			"bne x20, x1, 1f\n"
			"1:\n"
			"fmulh.pi f5, f5, f28\n"
			"fnmadd.s f20, f12, f2, f2, rne\n"
			"sraw x17, x17, x20\n"
			"sllw x18, x14, x14\n"
			"bne x10, x6, 1f\n"
			"1:\n"
			"sra x24, x7, x18\n"
			"bltu x24, x30, 1f\n"
			"1:\n"
			"la x23, rand_int_32\n"
			"flw.ps f21, 288(x23)\n"
			"flw.ps f31, 288(x23)\n"
			"fdivu.pi f1, f21, f31\n" 
			"fsub.ps f18, f18, f18, rdn\n"
			"fnmadd.ps f12, f16, f12, f18, rmm\n"
			"fminu.pi f24, f12, f18\n"
			"fnmsub.ps f24, f16, f16, f16, rne\n"
			"fexp.ps f24, f16\n"
			"ffrc.ps f12, f18\n"
			"fmadd.ps f18, f18, f18, f18, rtz\n"
			"fmin.pi f31, f18, f18\n"
			"fsgnj.s f30, f18, f16\n"
			"li x25, 0x287160cf7bc14db\n"
			"fcvt.s.wu f26, x25, rtz\n" 
			"fcvt.s.wu f26, x17, rup\n"
			"frcp.ps f31, f9\n"
			"beq x18, x31, 1f\n"
			"1:\n"
			"fand.pi f18, f21, f31\n"
			"rem x11, x11, x25\n"
			"srliw x18, x18, 0\n"
			"bgeu x15, x0, 1f\n"
			"1:\n"
			"xor x0, x0, x17\n"
			"fmul.pi f20, f10, f31\n"
			"sltu x28, x28, x23\n"
			"fsrli.pi f2, f2, 0xe\n"
			"li x28, 0x80cbc93004\n"
			"amoaddl.w x24, x7, (x28)\n" 
			"faddi.pi f1, f12, 0x10b\n"
			"fmin.s f26, f26, f26\n"
			"fand.pi f30, f2, f28\n"
			"fnmadd.ps f30, f18, f1, f8, rne\n"
			"fcvt.ps.pw f1, f21, rtz\n"
			"fmul.ps f31, f18, f21, rup\n"
			"fnot.pi f21, f31\n"
			"fcvt.f16.ps f30, f1\n"
			"beq x18, x8, 1f\n"
			"1:\n"
			"fsatu8.pi f31, f26\n"
			"bltu x18, x13, 1f\n"
			"1:\n"
			"li x11, 0x808e591c08\n"
			"la x24, mem_ptr_offset\n"
			"flw.ps f20, 672(x24)\n"
			"fscw.ps f5, f20 (x11)\n" 
			"fmin.pi f18, f26, f18\n"
			"blt x1, x4, 1f\n"
			"1:\n"
			"fnmsub.ps f9, f9, f5, f21, rtz\n"
			"fslli.pi f5, f30, 0x7\n"
			"fmul.s f28, f28, f8, rmm\n"
			"for.pi f28, f24, f16\n"
			"fandi.pi f12, f12, 0x1a\n"
			"fmsub.s f20, f20, f20, f20, rne\n"
			"fround.ps f10, f30, rup\n"
			"fmax.ps f9, f30, f12\n"
			"mova.m.x x27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fcvt.wu.s x18, f8, rmm\n"
			"fsrli.pi f18, f31, 0xc\n"
			"fcvt.s.wu f26, x20, rdn\n"
			"bltu x4, x19, 1f\n"
			"1:\n"
			"feqm.ps m3, f9, f2\n"
			"slliw x7, x28, 1\n"
			"fsgnjn.s f1, f31, f1\n"
			"bge x21, x22, 1f\n"
			"1:\n"
			"frcp.ps f20, f5\n"
			"fcvt.s.wu f1, x14, rdn\n"
			"fcvt.wu.s x11, f10, rne\n"
			"bltu x9, x22, 1f\n"
			"1:\n"
			"li x24, 0x808a6ab154\n"
			"sd x18, 512(x24)\n"
			"lwu x17, 512(x24)\n" 
			"ffrc.ps f1, f21\n"
			"fclass.ps f30, f5\n"
			"fmulh.pi f21, f30, f12\n"
			"fsatu8.pi f12, f8\n"
			"fnot.pi f9, f9\n"
			"fmul.s f24, f9, f30, rne\n"
			"fsgnj.ps f30, f5, f28\n"
			"fcvt.ps.pwu f20, f21, rdn\n"
			"fmul.ps f1, f12, f5, rdn\n"
			"csrw tensor_error, zero\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x70\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f16, 2(x31)\n"
			"flw.ps f18, 0(x31)\n"
			"flw.ps f20, 3(x31)\n"
			"flw.ps f21, 11(x31)\n"
			"flw.ps f24, 15(x31)\n"
			"flw.ps f26, 2(x31)\n"
			"flw.ps f31, 7(x31)\n"
			"flw.ps f5, 29(x31)\n"
			"flw.ps f8, 5(x31)\n"
			"flw.ps f9, 9(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x11, 0(x31)\n"
			"lw x14, 7(x31)\n"
			"lw x18, 1(x31)\n"
			"lw x20, 12(x31)\n"
			"lw x23, 8(x31)\n"
			"lw x24, 11(x31)\n"
			"lw x25, 13(x31)\n"
			"lw x27, 15(x31)\n"
			"lw x28, 1(x31)\n"
			"lw x7, 28(x31)\n"
			"li x31, 0x2\n"
			"LBL_SEQID_6_LOOP_SEQ_HID_15:\n"
			"fmaxu.pi f8, f8, f18\n"
			"fcvt.ps.pwu f21, f21, rne\n"
			"bne x23, x23, 1f\n"
			"1:\n"
			"fcvt.s.wu f26, x27, rup\n"
			"fsgnjn.s f28, f10, f21\n"
			"mulhsu x23, x23, x14\n"
			"bltu x7, x19, 1f\n"
			"1:\n"
			"mulhu x14, x14, x25\n"
			"mulw x27, x24, x27\n"
			"mov.m.x m2, x11, 0x12\n"
			"srl x25, x11, x25\n"
			"sll x24, x24, x0\n"
			"fcvt.pwu.ps f26, f31, rup\n"
			"maskpopcz x27, m2\n"
			"fcmov.ps f21, f21, f24, f26\n"
			"addi x28, x24, 0x4\n"
			"fcmovm.ps f26, f16, f26\n"
			"fcvt.pw.ps f1, f8, rmm\n"
			"masknot m5, m3\n"
			"fltm.ps m3, f21, f10\n"
			"bgeu x17, x8, 1f\n"
			"1:\n"
			"fsgnj.ps f8, f8, f9\n"
			"sllw x28, x24, x7\n"
			"remuw x27, x18, x18\n"
			"fmul.s f10, f10, f24, rmm\n"
			"rem x0, x25, x28\n"
			"fsatu8.pi f9, f9\n"
			"bne x22, x4, 1f\n"
			"1:\n"
			"fsub.s f18, f18, f10, rdn\n"
			"feq.pi f30, f26, f24\n"
			"bge x8, x10, 1f\n"
			"1:\n"
			"fadd.s f2, f18, f18, rtz\n"
			"fsgnjn.s f5, f21, f21\n"
			"fcvt.f16.ps f9, f9\n"
			"fminu.pi f1, f8, f9\n"
			"sltiu x11, x14, 0xd\n"
			"fsra.pi f18, f18, f21\n"
			"fsgnjx.ps f26, f9, f20\n"
			"fle.ps f26, f5, f10\n"
			"fmsub.ps f5, f8, f16, f16, rne\n"
			"mul x25, x25, x14\n"
			"lui x14, 0x8\n"
			"fadd.pi f26, f24, f9\n"
			"fsub.s f18, f31, f5, rup\n"
			"fmul.ps f28, f9, f16, rmm\n"
			"maskpopcz x14, m3\n"
			"fadd.s f12, f18, f26, rdn\n"
			"fcvt.s.wu f5, x0, rup\n"
			"maskpopcz x24, m5\n"
			"fsub.ps f8, f31, f8, rtz\n"
			"fsrli.pi f2, f16, 0x4\n"
			"flem.ps m5, f5, f20\n"
			"fcmov.ps f2, f20, f24, f16\n"
			"divu x11, x25, x11\n"
			"bge x2, x8, 1f\n"
			"1:\n"
			"maskxor m3, m3, m3\n"
			"fmvz.x.ps x28, f18, 0\n"
			"addiw x25, x7, 0xd\n"
			"sllw x20, x24, x20\n"
			"bne x5, x5, 1f\n"
			"1:\n"
			"feq.ps f18, f18, f31\n"
			"bne x22, x5, 1f\n"
			"1:\n"
			"addi x31, x31, -1\n"
			"bne x31, x0, LBL_SEQID_6_LOOP_SEQ_HID_15\n"
			"li x5, 7\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f13,  800 (x5)\n"
			"flw.ps f8,  256 (x5)\n"
			"flw.ps f18,  896 (x5)\n"
			"flw.ps f17,  416 (x5)\n"
			"flw.ps f6,  704 (x5)\n"
			"flw.ps f2,  960 (x5)\n"
			"flw.ps f23,  0 (x5)\n"
			"flw.ps f15,  608 (x5)\n"
			"flw.ps f28,  992 (x5)\n"
			"flw.ps f5,  736 (x5)\n"
			"flw.ps f29,  384 (x5)\n"
			"flw.ps f21,  64 (x5)\n"
			"flw.ps f4,  864 (x5)\n"
			"flw.ps f30,  352 (x5)\n"
			"flw.ps f24,  832 (x5)\n"
			"flw.ps f31,  928 (x5)\n"
			"flw.ps f25,  224 (x5)\n"
			"flw.ps f22,  160 (x5)\n"
			"flw.ps f12,  512 (x5)\n"
			"flw.ps f19,  288 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f16,  128 (x5)\n"
			"flw.ps f3,  768 (x5)\n"
			"flw.ps f9,  32 (x5)\n"
			"flw.ps f11,  480 (x5)\n"
			"flw.ps f26,  96 (x5)\n"
			"flw.ps f7,  640 (x5)\n"
			"flw.ps f1,  576 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f10,  192 (x5)\n"
			"flw.ps f20,  544 (x5)\n"
			"la x31, check_sum_hid_15\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"LBL_POSTAMBLE_HID_15:\n"          
			"csrwi   tensor_wait, 0x0\n"
			"csrwi   tensor_wait, 0x1\n"
			"csrwi   tensor_wait, 0x2\n"
			"csrwi   tensor_wait, 0x3\n"
			"csrwi   tensor_wait, 0x4\n"
			"csrwi   tensor_wait, 0x5\n"
			"csrwi   tensor_wait, 0x6\n"
			"csrwi   tensor_wait, 0x7\n"
			"csrwi   tensor_wait, 0x8\n"
			"csrwi   tensor_wait, 0x9\n"
			"csrwi   tensor_wait, 0xa\n"
			"fence\n"
			"LBL_SYNC_POINT_1_HID_15:\n"          
			"li x5, 0x1e1\n"
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, LBL_SYNC_POINT_1_WAIT_HID_15\n" 
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"csrw 0x821, x0\n" 
			"j LBL_SYNC_POINT_1_EXIT_HID_15\n"
			"LBL_SYNC_POINT_1_WAIT_HID_15:\n"          
			"csrw 0x821, x0\n" 
			"LBL_SYNC_POINT_1_EXIT_HID_15:\n"          
			"nop\n"
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_15\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_15:\n"          
			".global LBL_FAIL_HID_15\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_15\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_15:\n"          
			".global LBL_PASS_HID_15\n"          
			VSNIP_RSV
		);
		c_test_pass();
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_15:\n"          
			".global LBL_HALT_HID_15\n"          
			"wfi\n"
			"j LBL_HALT_HID_15\n"          
			VSNIP_RSV
		);
		return 0;       
	};
}
volatile uint32_t rand_int_32[256] __attribute__ ((aligned (2048))) = {
		0x3871ac28, 
		0xd9b5dae1, 
		0x7af062d3, 
		0x07023abb, 
		0x20772f65, 
		0x2b9a18b7, 
		0x6cf9ba90, 
		0xec754d51, 
		0x3d972155, 
		0xead3ab89, 
		0xd6f277be, 
		0x753001c4, 
		0xa3eb5b9b, 
		0x68a40cfa, 
		0x5613b8e2, 
		0x616634fb, 
		0xe2194d80, 
		0x5d545205, 
		0x6bb8a5c2, 
		0x44c5e737, 
		0x89aad8e3, 
		0x15609fd3, 
		0xa67210ac, 
		0x692aa066, 
		0x3714f645, 
		0x7f782b40, 
		0xdddf93aa, 
		0x2cd7ecd5, 
		0xbbe33b8c, 
		0x7eabb5c5, 
		0x17fc6ad6, 
		0x5417389e, 
		0xad87f60a, 
		0x447956b8, 
		0x6885e224, 
		0x0f8ab4bc, 
		0x8c57d58a, 
		0x5ee2bfa0, 
		0xc499f972, 
		0xd8afa023, 
		0x63a0bada, 
		0x2bf1410f, 
		0xd08256aa, 
		0x0b4664d9, 
		0x237fa600, 
		0x8b11f136, 
		0x74504f7e, 
		0x742e3108, 
		0x825ae486, 
		0x70b471e7, 
		0x2de1e23f, 
		0x7d5985ab, 
		0x39c48bca, 
		0x2d266d73, 
		0x85522af0, 
		0x6ee4c2b0, 
		0x92d0feb4, 
		0xb0771464, 
		0x23f11732, 
		0x6c5e3c60, 
		0x4b22ef91, 
		0xf44cd014, 
		0x4f16c20d, 
		0xf6234737, 
		0x7649ea70, 
		0x36ae48b7, 
		0xbe886dea, 
		0xb3a1cd03, 
		0x967f73a4, 
		0x22cbef78, 
		0x017032d9, 
		0x74f9eaf7, 
		0xa7f4226a, 
		0x8bd4d1b7, 
		0x5e205cf8, 
		0x5490663e, 
		0xad915af1, 
		0x7f376e4d, 
		0x89e461f9, 
		0x11c53ef7, 
		0xfc9e22a2, 
		0x6fceb777, 
		0x4a33a656, 
		0xcbdf0c7e, 
		0x8565228b, 
		0xc26a4488, 
		0x487217cd, 
		0xedafbb38, 
		0x4c9c533a, 
		0xd2153ad9, 
		0xd8f91bcf, 
		0x6a615ba5, 
		0x7c4ec70d, 
		0xfb07b71d, 
		0xaaecac18, 
		0x993d4d13, 
		0x483fb556, 
		0x7597bbf7, 
		0xc8cfd18b, 
		0xf6b7399a, 
		0x94a3806c, 
		0x7f9487a6, 
		0x31f71f4f, 
		0x64b4481b, 
		0x3f8d7b8c, 
		0x0bb0de7b, 
		0x62725c76, 
		0xac5ecc4d, 
		0x5fbe34d9, 
		0x940a202c, 
		0xc6067d63, 
		0xdb1feb96, 
		0x4f2d4af5, 
		0xad705828, 
		0xfec17bb4, 
		0x419105ef, 
		0x808d16a3, 
		0xe0da0447, 
		0x118367a1, 
		0x16f02b47, 
		0x535fe602, 
		0x37699de1, 
		0xfbdc1db2, 
		0x0b25c0d3, 
		0xe73bb757, 
		0x989478b6, 
		0xfcea8f71, 
		0x8ddeb337, 
		0x4ee03a12, 
		0x9fc39241, 
		0xc061117d, 
		0x79445139, 
		0x5bcfa7c1, 
		0xcdb5487d, 
		0xa600a783, 
		0xa21525b3, 
		0x8e49b692, 
		0x5a0585c6, 
		0x7a194998, 
		0x283eee55, 
		0x3170165d, 
		0xaca534fd, 
		0x08140203, 
		0xb28e15d2, 
		0x95afb063, 
		0xa9611ea4, 
		0x5fa5f889, 
		0x34f364d7, 
		0xcfb7d8fc, 
		0x1596b11a, 
		0x7af58ea3, 
		0xee771c48, 
		0x24f452d8, 
		0x49cb9e22, 
		0xa5991a57, 
		0xbfb23bfd, 
		0x2ce21412, 
		0x34674a69, 
		0xe49e4b53, 
		0xb9f42230, 
		0x15137c55, 
		0x88695b67, 
		0xfa3f5aee, 
		0xffd7e7e0, 
		0x65b1e2ba, 
		0x61cf15a1, 
		0x1bbc6271, 
		0xd52f607a, 
		0x06c458f8, 
		0x0689eb97, 
		0xd8c51a0b, 
		0x9e483811, 
		0x6201c84c, 
		0x65abbf54, 
		0x40410d20, 
		0x447414d2, 
		0x1dc05468, 
		0x207cc295, 
		0xa8a862c8, 
		0x7704f091, 
		0x60960de1, 
		0x1cf289c0, 
		0xbfc90bcb, 
		0xeeb4c0fa, 
		0xdedd3526, 
		0x1d24edee, 
		0x046460c9, 
		0x998a7fae, 
		0xab08aebc, 
		0x05700731, 
		0x2a0764ba, 
		0x2239e176, 
		0x53bfb2e4, 
		0x7e1ba15e, 
		0xe67ffa5c, 
		0x3debb40e, 
		0xf2c41d11, 
		0x39ac694b, 
		0x2251f1ca, 
		0x8a176d1f, 
		0x43f25c7e, 
		0xdb30a3e3, 
		0xd885dea6, 
		0x1aa4e0a0, 
		0xf0cf1e43, 
		0x544521ff, 
		0x543fa005, 
		0xb9017b32, 
		0x72f4863e, 
		0xeaa6f6d1, 
		0xd37f5382, 
		0x4b712af6, 
		0x6c964191, 
		0x84d239fd, 
		0x036be572, 
		0xb5c22cca, 
		0x3a2cf00b, 
		0x5628443c, 
		0xedec0355, 
		0xdeb3adab, 
		0x1e2f7644, 
		0xad50e70b, 
		0x962d3d97, 
		0x58bba881, 
		0x53a1c536, 
		0x88b119e3, 
		0x7b65763c, 
		0x43f391f7, 
		0x5a2fb914, 
		0x66e4885a, 
		0x99b83c78, 
		0x08503a4b, 
		0xe0527dc0, 
		0x4f1edd16, 
		0x6eb1d7f1, 
		0xb54ebe90, 
		0xa01c2785, 
		0x11e6dd04, 
		0x5eef5354, 
		0xc9268dcb, 
		0x4c67f45f, 
		0x850e1a3d, 
		0xcb07ec4f, 
		0xc1db4cc4, 
		0xa197661a, 
		0x0ab58dad, 
		0xf0a9b903, 
		0xa8b18fd3, 
		0xaeac0139, 
		0x54c56604, 
		0xfdbfc6ea, 
		0x0abd6ee6, 
		0x81889bba, 
		0xe2a0b09c, 
		0x26f9ed91, 
		0xd7b932e0  
};
volatile uint16_t rand_ieee754_16[1024] __attribute__ ((aligned (2048))) = {
		0xd3c0, 
		0x51a0, 
		0xf93b, 
		0x2810, 
		0x034f, 
		0xa7b1, 
		0x4f40, 
		0x4f80, 
		0x5140, 
		0x5180, 
		0x4b80, 
		0xcb00, 
		0x5360, 
		0xca00, 
		0xd040, 
		0x7c00, 
		0x4800, 
		0x0a79, 
		0xccd1, 
		0x8256, 
		0xc500, 
		0x992f, 
		0x4d80, 
		0xd380, 
		0x5120, 
		0xfd42, 
		0x2c2f, 
		0xfc00, 
		0x4fc0, 
		0xd1a0, 
		0xd160, 
		0x5380, 
		0xcd08, 
		0x46ea, 
		0xd220, 
		0xe5d6, 
		0x4f00, 
		0x4f80, 
		0x8000, 
		0x4e00, 
		0xfcd6, 
		0xd260, 
		0x4e40, 
		0x0000, 
		0x51b7, 
		0xd2e0, 
		0x0ed6, 
		0x5180, 
		0x5380, 
		0xcd00, 
		0x5d65, 
		0x5340, 
		0x53e0, 
		0x123b, 
		0x8000, 
		0xab32, 
		0x4cc0, 
		0xcc00, 
		0x5280, 
		0xc523, 
		0x5280, 
		0xc400, 
		0x5000, 
		0xcd21, 
		0x9a11, 
		0xd060, 
		0x8000, 
		0x4800, 
		0xc200, 
		0x269e, 
		0x4d40, 
		0x1ef1, 
		0x5871, 
		0x5060, 
		0x8000, 
		0x4980, 
		0x5020, 
		0xbd8f, 
		0x4700, 
		0x7f37, 
		0xaca3, 
		0xfc00, 
		0xcec0, 
		0x8000, 
		0xc900, 
		0x53c0, 
		0x8000, 
		0x4700, 
		0x4445, 
		0x4b80, 
		0xd120, 
		0xd300, 
		0xccc0, 
		0xfc00, 
		0xa0b5, 
		0x8000, 
		0x4900, 
		0x82ba, 
		0x5260, 
		0xd280, 
		0xca80, 
		0x5340, 
		0x8000, 
		0x4e80, 
		0xd200, 
		0x5000, 
		0xce00, 
		0x4200, 
		0xdfad, 
		0x5160, 
		0x67cd, 
		0x5340, 
		0x4dc0, 
		0xd000, 
		0x4c80, 
		0xc5be, 
		0x53c0, 
		0xc500, 
		0xc400, 
		0x5120, 
		0xce40, 
		0xbc00, 
		0xce40, 
		0x5080, 
		0x4700, 
		0x4c40, 
		0x9f45, 
		0xd000, 
		0x4c80, 
		0xd08b, 
		0xf7c6, 
		0x4e40, 
		0xd380, 
		0xd220, 
		0xb438, 
		0xd300, 
		0x89dd, 
		0x5100, 
		0xc500, 
		0x3ddb, 
		0x4e4a, 
		0xcf00, 
		0x5320, 
		0x4200, 
		0x3bcf, 
		0x8000, 
		0x282a, 
		0xd0a0, 
		0x5040, 
		0x5060, 
		0xd200, 
		0x2f6e, 
		0x2153, 
		0xd240, 
		0xa892, 
		0x14da, 
		0xcfc0, 
		0x1e55, 
		0x4b00, 
		0x4600, 
		0xe7a0, 
		0x8000, 
		0xfc00, 
		0xe196, 
		0xfc00, 
		0x4800, 
		0x683e, 
		0x4e80, 
		0x8a0a, 
		0xc800, 
		0x604b, 
		0x9273, 
		0x5040, 
		0x3c00, 
		0x51c0, 
		0xcb80, 
		0x0267, 
		0xd140, 
		0x52c0, 
		0xc900, 
		0xd1c0, 
		0x4900, 
		0x5200, 
		0xcfd7, 
		0x14fc, 
		0xad31, 
		0x57da, 
		0xd3e0, 
		0x064a, 
		0x4ec0, 
		0x04a3, 
		0xee28, 
		0xcfc0, 
		0xcff5, 
		0x50e0, 
		0x5273, 
		0x8874, 
		0xd0c0, 
		0xd2c0, 
		0xd2c0, 
		0x4e40, 
		0xd65a, 
		0xcae1, 
		0xd260, 
		0xd160, 
		0xd280, 
		0xca00, 
		0x4900, 
		0x5b30, 
		0xcf40, 
		0x66c7, 
		0xfc68, 
		0xd220, 
		0x746e, 
		0x4800, 
		0xd100, 
		0x8000, 
		0x19fc, 
		0x4800, 
		0x5120, 
		0xfc00, 
		0x8000, 
		0x33cf, 
		0xc400, 
		0xd160, 
		0xcd00, 
		0x1ea7, 
		0x51a0, 
		0x4700, 
		0x3c00, 
		0x4500, 
		0x138c, 
		0x3c00, 
		0x50c0, 
		0x0217, 
		0x8000, 
		0x7c42, 
		0xd080, 
		0x51c0, 
		0x5140, 
		0xcec0, 
		0xd1e0, 
		0x3322, 
		0x52a0, 
		0x52c0, 
		0xdda5, 
		0x4700, 
		0x7c00, 
		0x320a, 
		0xad23, 
		0x53c0, 
		0x2388, 
		0x4c40, 
		0x4a80, 
		0x6cbd, 
		0xd040, 
		0x7c00, 
		0x4c00, 
		0x4c00, 
		0x3dbd, 
		0xd3e0, 
		0x5100, 
		0xd1a0, 
		0xd220, 
		0x8000, 
		0xcec0, 
		0xd0a0, 
		0xb290, 
		0xbb2c, 
		0xd180, 
		0xfba2, 
		0x7396, 
		0x4d80, 
		0x50c0, 
		0x4ec0, 
		0xd220, 
		0xcb00, 
		0x066c, 
		0xc700, 
		0xd180, 
		0xd1e0, 
		0xd0e0, 
		0xd2a0, 
		0x0374, 
		0xb972, 
		0x4d00, 
		0x188c, 
		0x74d5, 
		0xeb9a, 
		0x8000, 
		0x7c00, 
		0x4700, 
		0x4880, 
		0x85e6, 
		0x52a0, 
		0x5961, 
		0xd0e0, 
		0x5340, 
		0xd000, 
		0x9a1d, 
		0xd113, 
		0x53e0, 
		0xfc00, 
		0x4700, 
		0xd340, 
		0xc980, 
		0x4a00, 
		0x0000, 
		0xb306, 
		0xb04b, 
		0x7c00, 
		0xf402, 
		0x2974, 
		0x4e00, 
		0xb762, 
		0xc8fc, 
		0x4900, 
		0x7c00, 
		0x4200, 
		0xd0c2, 
		0xd7eb, 
		0x4880, 
		0xfc3f, 
		0xd180, 
		0xad9b, 
		0xcfc0, 
		0x0000, 
		0x5300, 
		0x7f77, 
		0x51e0, 
		0x4dc0, 
		0xf339, 
		0x4504, 
		0x8000, 
		0xa52d, 
		0x53c0, 
		0xa222, 
		0x4fc0, 
		0x4ec0, 
		0xa89f, 
		0xcf00, 
		0xc200, 
		0xc400, 
		0xd5e8, 
		0x42f3, 
		0xdaa9, 
		0x02b2, 
		0x460a, 
		0xd1e0, 
		0x0000, 
		0x9657, 
		0xd280, 
		0xccc0, 
		0x8000, 
		0x5340, 
		0x52a0, 
		0xc200, 
		0x4fc0, 
		0xcf00, 
		0xc800, 
		0x65ce, 
		0xb542, 
		0xd2c0, 
		0xd180, 
		0xd140, 
		0x4880, 
		0x0000, 
		0xab22, 
		0x7c00, 
		0xd200, 
		0xcb00, 
		0x4880, 
		0xce40, 
		0x7c00, 
		0xd2a0, 
		0xfcaa, 
		0x47f9, 
		0x4400, 
		0x51e0, 
		0xfcd2, 
		0x4900, 
		0x9f04, 
		0xd0c0, 
		0x4cc0, 
		0x5340, 
		0x8000, 
		0x6aa5, 
		0x4c00, 
		0x5320, 
		0x5100, 
		0x4400, 
		0xe13a, 
		0x5e32, 
		0xcc80, 
		0x3684, 
		0xe1b1, 
		0x4000, 
		0x2241, 
		0x5180, 
		0x4700, 
		0x2c60, 
		0xd471, 
		0x4800, 
		0x4c40, 
		0xd000, 
		0xcd80, 
		0x439d, 
		0x8000, 
		0xd220, 
		0x5000, 
		0xcfc0, 
		0xccc0, 
		0xfc8d, 
		0x5340, 
		0xc880, 
		0xd360, 
		0x53c0, 
		0xd1c0, 
		0xd3c0, 
		0x5120, 
		0x8000, 
		0x8000, 
		0xcdc0, 
		0x836f, 
		0x882b, 
		0xc400, 
		0x83d6, 
		0x8a81, 
		0xd040, 
		0x5bc5, 
		0x53c0, 
		0x0000, 
		0xcd00, 
		0xa386, 
		0xcb80, 
		0x52e0, 
		0x8000, 
		0x8000, 
		0x51a0, 
		0xd060, 
		0xd260, 
		0xd280, 
		0x51a0, 
		0xffd6, 
		0x4ae8, 
		0xc800, 
		0xc200, 
		0x8000, 
		0xce40, 
		0xd380, 
		0x0138, 
		0x7c00, 
		0x8000, 
		0xcd40, 
		0x52e0, 
		0xd5f8, 
		0x4980, 
		0x4000, 
		0x8000, 
		0x2080, 
		0x4500, 
		0xf0ca, 
		0x5100, 
		0x02ec, 
		0xfc00, 
		0xb086, 
		0x52e0, 
		0x9190, 
		0x8000, 
		0x4000, 
		0xf3ee, 
		0x53c0, 
		0x50e0, 
		0x5244, 
		0x0000, 
		0xd280, 
		0x52a0, 
		0x4d3c, 
		0xd020, 
		0xce88, 
		0xd1e0, 
		0xce40, 
		0xe668, 
		0x001d, 
		0x4d00, 
		0x8000, 
		0x4f40, 
		0xecf9, 
		0xc600, 
		0x5140, 
		0xb798, 
		0xe035, 
		0x4c40, 
		0xce00, 
		0xf225, 
		0xfe5b, 
		0x5602, 
		0xd340, 
		0x8000, 
		0x5280, 
		0x1047, 
		0xce40, 
		0xa002, 
		0xd280, 
		0xcd40, 
		0xdc19, 
		0x80f3, 
		0x0fbf, 
		0x4500, 
		0x53c0, 
		0x8000, 
		0xfec5, 
		0x1d89, 
		0xc787, 
		0xcc8d, 
		0xcc40, 
		0x9750, 
		0xd340, 
		0x51c0, 
		0xd120, 
		0xce80, 
		0x3228, 
		0xd040, 
		0x8000, 
		0x4f80, 
		0x4e80, 
		0xcf80, 
		0xd5ee, 
		0x7574, 
		0x0000, 
		0xfc00, 
		0x4f40, 
		0x9872, 
		0xc900, 
		0x9d4f, 
		0x79cc, 
		0x7c00, 
		0x7866, 
		0x4c00, 
		0xd220, 
		0x4a80, 
		0x8000, 
		0x8a69, 
		0xcf80, 
		0xd260, 
		0x51a0, 
		0x1001, 
		0x5280, 
		0x4c80, 
		0xd080, 
		0x0000, 
		0x71b9, 
		0xd100, 
		0xd16d, 
		0x4500, 
		0xd380, 
		0xd495, 
		0x0000, 
		0xcf40, 
		0xcf80, 
		0xbc00, 
		0xc050, 
		0x3ddf, 
		0x51e0, 
		0x4880, 
		0x5240, 
		0xcd80, 
		0x8000, 
		0x7c00, 
		0xfc00, 
		0xd260, 
		0x8f7b, 
		0x4dc0, 
		0x920b, 
		0x0a88, 
		0xd360, 
		0xf3e3, 
		0x51c0, 
		0x4fc0, 
		0x4fc0, 
		0x7c00, 
		0x8000, 
		0x4d80, 
		0xd0e0, 
		0x4980, 
		0x5340, 
		0x41b9, 
		0xd200, 
		0xc880, 
		0x77b0, 
		0xd100, 
		0x5160, 
		0x4200, 
		0x501b, 
		0x4c80, 
		0xcc00, 
		0xa004, 
		0x5120, 
		0x4c80, 
		0xcfc0, 
		0x7c00, 
		0x50e0, 
		0x5100, 
		0x4634, 
		0x52c0, 
		0x53c0, 
		0x7f0b, 
		0xfc00, 
		0xcc80, 
		0x8000, 
		0xc400, 
		0x4fc0, 
		0x226a, 
		0x4400, 
		0xd040, 
		0xce40, 
		0xfc00, 
		0x5630, 
		0x4d80, 
		0xc880, 
		0x4ec0, 
		0xe45c, 
		0x4b00, 
		0xf7ee, 
		0xd020, 
		0x5260, 
		0x5320, 
		0x5080, 
		0x53e0, 
		0xd320, 
		0xcc82, 
		0xc600, 
		0xd080, 
		0x50e0, 
		0x7c00, 
		0x0000, 
		0xce00, 
		0x4000, 
		0xcd80, 
		0xfa66, 
		0xc5e6, 
		0xd080, 
		0xd2ea, 
		0xd080, 
		0x4725, 
		0xd060, 
		0x79ce, 
		0xce00, 
		0x7c00, 
		0xc000, 
		0xca80, 
		0x5380, 
		0x4e80, 
		0xfd31, 
		0xd060, 
		0xd240, 
		0x5140, 
		0xccc0, 
		0x987b, 
		0xcc00, 
		0x4c00, 
		0x4d00, 
		0x8da0, 
		0x3c00, 
		0x4f00, 
		0x0fe1, 
		0x4400, 
		0x4000, 
		0x4880, 
		0x7c00, 
		0xcd00, 
		0xf580, 
		0xfc00, 
		0xd200, 
		0x46db, 
		0x0000, 
		0xc900, 
		0xd2c0, 
		0x0000, 
		0x5100, 
		0x5160, 
		0xdd4e, 
		0xca80, 
		0xc980, 
		0x3d4e, 
		0x5220, 
		0x4200, 
		0x52c0, 
		0x6f41, 
		0xd0a0, 
		0x8000, 
		0xd3e0, 
		0x317c, 
		0x16e6, 
		0xfc00, 
		0x4200, 
		0xbc00, 
		0x2859, 
		0x50e0, 
		0xcec0, 
		0xf57c, 
		0xd000, 
		0xd2c0, 
		0x0000, 
		0xce80, 
		0xd657, 
		0x4a00, 
		0xccc0, 
		0x4e80, 
		0xcfc0, 
		0x5340, 
		0xd1e0, 
		0x8000, 
		0xc400, 
		0xce80, 
		0xca80, 
		0xd220, 
		0x4184, 
		0xeba6, 
		0x4980, 
		0x4a00, 
		0x4c00, 
		0xd1c0, 
		0x0000, 
		0xd2c0, 
		0xd180, 
		0x8000, 
		0x5320, 
		0xfde6, 
		0xca00, 
		0xcb80, 
		0x4700, 
		0x53a0, 
		0x8000, 
		0x0133, 
		0x6347, 
		0xfdd7, 
		0xce40, 
		0x4d80, 
		0x3bb4, 
		0x1e93, 
		0xcfc0, 
		0xc582, 
		0x7285, 
		0x5300, 
		0x0000, 
		0x4800, 
		0x8000, 
		0x5e11, 
		0x4cc0, 
		0xfc25, 
		0x4b80, 
		0x4e00, 
		0xccc0, 
		0x5320, 
		0xd220, 
		0xd000, 
		0x736e, 
		0x39af, 
		0x5120, 
		0xce40, 
		0xa148, 
		0xcc00, 
		0xc880, 
		0xd000, 
		0x5300, 
		0xcfc0, 
		0x4f80, 
		0x4800, 
		0x5160, 
		0x8000, 
		0x7c00, 
		0x50a0, 
		0xcf80, 
		0x4880, 
		0xc900, 
		0x8000, 
		0x4500, 
		0xcd3e, 
		0x6e2b, 
		0x5060, 
		0xcf40, 
		0xd2a0, 
		0xbc3f, 
		0x4c80, 
		0xcec0, 
		0x51e0, 
		0x4e00, 
		0xd8cc, 
		0x4c00, 
		0x8000, 
		0xcf80, 
		0x0000, 
		0xc600, 
		0x2384, 
		0xfc00, 
		0xccc0, 
		0x8423, 
		0x7a14, 
		0x6b01, 
		0xa916, 
		0xc200, 
		0xd140, 
		0x4f80, 
		0x77e4, 
		0x426c, 
		0xd347, 
		0x3b5c, 
		0x53e0, 
		0xd020, 
		0xd1e0, 
		0x7844, 
		0x2a43, 
		0x5120, 
		0xd340, 
		0x4d40, 
		0x4980, 
		0x7c42, 
		0x7fe5, 
		0x50c0, 
		0x6008, 
		0x5180, 
		0x0000, 
		0x5200, 
		0x619b, 
		0xdaec, 
		0x8000, 
		0x52e0, 
		0x5200, 
		0xd1e0, 
		0x4f80, 
		0x5100, 
		0x4d40, 
		0x4900, 
		0x8000, 
		0xce80, 
		0xca00, 
		0xd3c0, 
		0xd8c1, 
		0x196c, 
		0xd0c0, 
		0x0000, 
		0x8000, 
		0x7dbb, 
		0x4200, 
		0x9652, 
		0xcec0, 
		0xd2c0, 
		0x0159, 
		0xd060, 
		0x3974, 
		0xf857, 
		0x4d80, 
		0x8000, 
		0xdbdc, 
		0x52c0, 
		0x7d9e, 
		0x4e80, 
		0x50c0, 
		0x4d40, 
		0x4980, 
		0xfc00, 
		0xd020, 
		0xea0e, 
		0xcc80, 
		0x4980, 
		0xcfc0, 
		0x5120, 
		0xc000, 
		0x7c00, 
		0x8717, 
		0xcf00, 
		0x0000, 
		0x4d80, 
		0x5140, 
		0xfdce, 
		0xcf40, 
		0x5340, 
		0x4a80, 
		0xfd3d, 
		0xcc40, 
		0x0000, 
		0x4ce2, 
		0xfc00, 
		0xed49, 
		0x4600, 
		0x4cc2, 
		0x5240, 
		0xcd00, 
		0x4a80, 
		0xe67e, 
		0x97eb, 
		0xd3c0, 
		0x0000, 
		0x4d80, 
		0xbeaf, 
		0xc228, 
		0x5060, 
		0x5060, 
		0x53c0, 
		0x4f00, 
		0xa504, 
		0xd140, 
		0xd080, 
		0xd040, 
		0xb6eb, 
		0xd9be, 
		0x5240, 
		0x8000, 
		0xd1c0, 
		0x8000, 
		0x4600, 
		0x4000, 
		0x2388, 
		0x4a80, 
		0xbd31, 
		0xc993, 
		0x5020, 
		0x6857, 
		0x53e0, 
		0x5220, 
		0xd320, 
		0x9836, 
		0x4700, 
		0x5380, 
		0xd300, 
		0x50ef, 
		0x4a80, 
		0xcb1d, 
		0x4f80, 
		0xcc40, 
		0x4d00, 
		0x4ec0, 
		0x425f, 
		0x5160, 
		0xd220, 
		0xf5e1, 
		0x8912, 
		0xce80, 
		0xd120, 
		0xcf80, 
		0x29e9, 
		0x4a80, 
		0x0000, 
		0xa668, 
		0x4fa3, 
		0x5280, 
		0x94af, 
		0x0c37, 
		0xcc80, 
		0x775e, 
		0x4c00, 
		0x941c, 
		0x55a9, 
		0xb264, 
		0xd100, 
		0x5260, 
		0xd0a0, 
		0xfcc8, 
		0xc800, 
		0x4900, 
		0x57b0, 
		0xd360, 
		0xd1e0, 
		0x654c, 
		0xd360, 
		0x8000, 
		0xcb00, 
		0x4d40, 
		0x4500, 
		0xce39, 
		0x50c0, 
		0x4a80, 
		0x4700, 
		0x51e0, 
		0x8000, 
		0xb65e, 
		0xb035, 
		0x4903, 
		0xc000, 
		0xb43f, 
		0xd280, 
		0xd240, 
		0x53e0, 
		0x970a, 
		0x5000, 
		0x0fb3, 
		0xbc88, 
		0x8bb2, 
		0xfba4, 
		0x4dc0, 
		0x815f, 
		0xccc0, 
		0x8eb6, 
		0xbab7, 
		0xd2a0, 
		0xc800, 
		0x2b60, 
		0xd100, 
		0x1f71, 
		0x4d00, 
		0xfc00, 
		0x5100, 
		0xc758, 
		0x4000, 
		0xc980, 
		0xfc00, 
		0xcec0, 
		0xc900, 
		0x4f00, 
		0x5240, 
		0xa225, 
		0x2a87, 
		0xc800, 
		0xce80, 
		0x50a0, 
		0x8fb4, 
		0xd240, 
		0xc44e, 
		0x4600, 
		0x8000, 
		0xfc00, 
		0x3370, 
		0x5020  
};
volatile uint32_t rand_ieee754_32[1024] __attribute__ ((aligned (2048))) = {
		0x42680000, 
		0xf92f37aa, 
		0x7f7ffffe, 
		0x0c7fff00, 
		0x42280000, 
		0x00000008, 
		0x0c7f8000, 
		0xc2040000, 
		0x00800001, 
		0x80000000, 
		0xff800000, 
		0x41c00000, 
		0x7f800000, 
		0x7f8ba784, 
		0x647923d7, 
		0x98307fe4, 
		0xff7ffffe, 
		0x80800002, 
		0x80008000, 
		0xbf800000, 
		0x41c00000, 
		0x8043ff72, 
		0x7fc00000, 
		0x80000001, 
		0x8f7ffffd, 
		0x80000020, 
		0x9f2a39d9, 
		0x00040000, 
		0x41e00000, 
		0x0e0007ff, 
		0x41600000, 
		0xc2e7d86e, 
		0x0e00ffff, 
		0x806fc149, 
		0x504513cc, 
		0x00000000, 
		0x80000000, 
		0x80800003, 
		0xcb000000, 
		0x1f424acf, 
		0x4160b1e9, 
		0xffbaeeb0, 
		0x32fc72b9, 
		0xacbcd9a7, 
		0x41600000, 
		0x80002000, 
		0x42780000, 
		0x8f7ffffe, 
		0x07cb0a52, 
		0x801ed7a9, 
		0x0f7ffffd, 
		0x237ac604, 
		0x7f800001, 
		0x41b80000, 
		0xc2600000, 
		0x42c138f1, 
		0xc254e05a, 
		0x1261f57f, 
		0xc1700000, 
		0x095d6024, 
		0x80800000, 
		0x1496c8dc, 
		0xffc00000, 
		0x802317ff, 
		0xc0a00000, 
		0x80000000, 
		0x0c7f0000, 
		0x4fe50783, 
		0xa0fafc5b, 
		0x7f800000, 
		0x80000020, 
		0x80000000, 
		0x8d7dd1b6, 
		0x80800001, 
		0x80020000, 
		0x0e000007, 
		0x75b1e96d, 
		0xc1200000, 
		0x807fffff, 
		0x0e7fffff, 
		0x00000002, 
		0x7f7ffffe, 
		0x0c7ffe00, 
		0x007ffffe, 
		0x0e01ffff, 
		0xcb000000, 
		0x80000000, 
		0xc2400000, 
		0xc2080000, 
		0x7d94fe11, 
		0xbf028f5c, 
		0x41a00000, 
		0xc1700000, 
		0x41700000, 
		0x80000002, 
		0x7f7fffff, 
		0x3826329c, 
		0x76adea27, 
		0x80000000, 
		0x00004000, 
		0xaaaaaaaa, 
		0x41500000, 
		0xe864812a, 
		0x00000001, 
		0x00080000, 
		0x7f800000, 
		0xe4bbc317, 
		0x2a15b099, 
		0xe5139c81, 
		0xc0400000, 
		0x41200000, 
		0x0c7ff000, 
		0x918e8f07, 
		0xc2080000, 
		0x10852eb1, 
		0xff7ffffe, 
		0x7149e3c8, 
		0xfdd2eafb, 
		0x42200000, 
		0x55555555, 
		0x421c0000, 
		0xff800000, 
		0x638431f3, 
		0x00800002, 
		0x8f7ffffd, 
		0xebc7ff73, 
		0x33476f8e, 
		0x00318caa, 
		0xf123e0f8, 
		0x0c780000, 
		0x0c7f8000, 
		0xc13641e8, 
		0x00800002, 
		0x1b467469, 
		0xc1c80000, 
		0xea4bf6f6, 
		0x80800000, 
		0x0c7fff80, 
		0xd84a899d, 
		0x0c7c0000, 
		0x00020000, 
		0x72370c29, 
		0x42580000, 
		0x8f7ffffe, 
		0x0000a756, 
		0x8c59a6fb, 
		0xff800000, 
		0x873f9bb0, 
		0x55555555, 
		0x7f800000, 
		0xc1e00000, 
		0x00000002, 
		0xff800000, 
		0x7f800000, 
		0x8abb8125, 
		0x807ffffe, 
		0xaaaaaaaa, 
		0x0c7e0000, 
		0x0c7fffe0, 
		0xc2140000, 
		0x808a6bf7, 
		0xdcb24bd5, 
		0xc2440000, 
		0x8f7ffffd, 
		0x41a80000, 
		0x807f1aa8, 
		0x42140000, 
		0x40000000, 
		0xff7fffff, 
		0x80000000, 
		0xa5e26eb4, 
		0x7f7ffffe, 
		0xc20c0000, 
		0xc27c0000, 
		0x7f7ffffe, 
		0xc2240000, 
		0x00100000, 
		0x00100000, 
		0x80080000, 
		0x00000000, 
		0xc2700000, 
		0x5f6202b5, 
		0x00000000, 
		0x4c17cc37, 
		0xff7ffffe, 
		0x8f7ffffc, 
		0xaa78e410, 
		0x038a96a1, 
		0x0c7ffc00, 
		0x00800002, 
		0x0c7ff800, 
		0x81da2dd5, 
		0x0e0fffff, 
		0xe38aa214, 
		0xc0c00000, 
		0x80000000, 
		0x42180000, 
		0x64c59d4c, 
		0x42180000, 
		0x7f800000, 
		0x80040000, 
		0x00000000, 
		0xc1000000, 
		0x62ed9618, 
		0x0f7fffff, 
		0xcb8c4b40, 
		0xff800000, 
		0xffbfffff, 
		0xf391d9bc, 
		0xc381b183, 
		0x0f7ffffe, 
		0x00002000, 
		0x16368656, 
		0x0ba5e231, 
		0x7f7ffffe, 
		0x0c7fc000, 
		0x00000800, 
		0x41a80000, 
		0x00200000, 
		0x80000000, 
		0x4c8b81e0, 
		0x7f7ffffe, 
		0xf153444f, 
		0x41300000, 
		0xf7a21ffb, 
		0x80000000, 
		0xf3c06b70, 
		0xb2eb46ca, 
		0x401fd3de, 
		0xb7b9593c, 
		0x0e000fff, 
		0x00000010, 
		0xbd8a541b, 
		0xc2440000, 
		0x80000010, 
		0x42280000, 
		0x42300000, 
		0x40e00000, 
		0x62985c1a, 
		0x6d82b596, 
		0xc2340000, 
		0x40c00000, 
		0x0d00fff0, 
		0x257914a2, 
		0xbf800001, 
		0x80010000, 
		0x0c700000, 
		0x22c93723, 
		0x80000400, 
		0x7f7ffffe, 
		0xa7401c9b, 
		0x29084fd3, 
		0x0c400000, 
		0x00800000, 
		0xb1b40262, 
		0x80001000, 
		0x7fc00001, 
		0x7fc00000, 
		0xb392d83c, 
		0x007ffffe, 
		0x42180000, 
		0x0c7fc000, 
		0x29212a3f, 
		0x23b5b7b3, 
		0x81cc8c2f, 
		0x4514395e, 
		0xb85a7aa6, 
		0x007fffff, 
		0x00000000, 
		0xcccccccc, 
		0x41600000, 
		0x41d00000, 
		0x0c7ffff0, 
		0x90a14687, 
		0xffc00001, 
		0x80008000, 
		0xffbfffff, 
		0xc1900000, 
		0xe54b9f1e, 
		0x4949a5d0, 
		0x0c400000, 
		0xa26feb4e, 
		0x7f000000, 
		0xb5dbfa1a, 
		0x42000000, 
		0xf880c517, 
		0x00000400, 
		0x77efd9df, 
		0x80ffffff, 
		0x00000008, 
		0x41700000, 
		0x00000008, 
		0xf410026f, 
		0x00000000, 
		0xe0131688, 
		0xc1800000, 
		0x0d15f729, 
		0x00800000, 
		0x42140000, 
		0xae2beff1, 
		0xd7a0a9e8, 
		0x7fc00000, 
		0x42140000, 
		0x00000000, 
		0xc26c0000, 
		0x48d0631c, 
		0xffa93fae, 
		0x00800002, 
		0xaec51ab3, 
		0x80800000, 
		0x67a4b576, 
		0xc1c00000, 
		0x30d0d69c, 
		0xc283fe8f, 
		0xc0000000, 
		0xfa82a2ff, 
		0x80002000, 
		0x0e07ffff, 
		0x0c7f8000, 
		0xc1b80000, 
		0x80000004, 
		0x7f800000, 
		0x00400000, 
		0x007fffff, 
		0x0e00000f, 
		0x00000000, 
		0x0e00000f, 
		0x00800001, 
		0x40a20335, 
		0x9ab73bfa, 
		0x8f7ffffe, 
		0xc1600000, 
		0x88ff76e7, 
		0xc2440000, 
		0x0e7fffff, 
		0x42040000, 
		0xc1b00000, 
		0x80000000, 
		0x80000000, 
		0x80000004, 
		0xff800000, 
		0x42300000, 
		0x00000008, 
		0x803006e0, 
		0xc1000000, 
		0x1617e48b, 
		0x21eb094a, 
		0x5d378415, 
		0xff800000, 
		0xff800000, 
		0x8762cecf, 
		0x00dbb793, 
		0xbb0ead42, 
		0xc1c80000, 
		0x00800002, 
		0x8f7ffffd, 
		0xff800000, 
		0x41500000, 
		0x41880000, 
		0x00000002, 
		0x8059c41c, 
		0x0e0003ff, 
		0x2e5e91ac, 
		0xc2580000, 
		0x40e00000, 
		0x7fc00001, 
		0x949c6ba7, 
		0x80000000, 
		0x000b158c, 
		0x7f800000, 
		0x00000000, 
		0x7fc00001, 
		0x00000400, 
		0xcccccccc, 
		0x7f8efc58, 
		0xc0800000, 
		0xffabbd52, 
		0x42640000, 
		0xaece9f42, 
		0x00000080, 
		0x41600000, 
		0x42040000, 
		0x422c0000, 
		0x41400000, 
		0xbf028f5c, 
		0x8ae20365, 
		0x00800000, 
		0x007fffff, 
		0x42580000, 
		0x00000800, 
		0x00008000, 
		0x80000002, 
		0x3ae95b5e, 
		0x00400000, 
		0x80800001, 
		0x00000000, 
		0x4946d5a2, 
		0x0c7fff80, 
		0x00000000, 
		0xb8835bcb, 
		0x25560439, 
		0xff80b1f9, 
		0xa734991a, 
		0x7f7ffffe, 
		0x8f7ffffd, 
		0x00000000, 
		0x424c0000, 
		0x49c7c2bc, 
		0x41f00000, 
		0xffbfffff, 
		0x80000020, 
		0x40a00000, 
		0x274482f7, 
		0x0c7ffff0, 
		0x12e2550f, 
		0x40311c57, 
		0x056875f2, 
		0x3f028f5c, 
		0x04b6af28, 
		0xc2780000, 
		0x89c8a22f, 
		0x3b46da97, 
		0xc1f00000, 
		0x80011111, 
		0x41e00000, 
		0x3f800001, 
		0x13d800bb, 
		0xc1a80000, 
		0xc1200000, 
		0x9e6ba344, 
		0x80040000, 
		0x74c11c5a, 
		0x3a7e2d9e, 
		0x03b37050, 
		0x424c0000, 
		0x7f7ffffe, 
		0xc2000000, 
		0x00000000, 
		0x41a80000, 
		0x00004000, 
		0x2081a764, 
		0xcb8c4b40, 
		0x3f800001, 
		0x94a559fd, 
		0x7f7ffffe, 
		0x42700000, 
		0x00000010, 
		0x0e00000f, 
		0x427dd332, 
		0xc25c0000, 
		0x0e000007, 
		0x80000001, 
		0x4acdd5e2, 
		0x0d53d7b0, 
		0x0e0007ff, 
		0x42780000, 
		0x0284cdf4, 
		0xbf800000, 
		0xff7ffffe, 
		0xc2180000, 
		0x00001000, 
		0xc2000000, 
		0x7f7fffff, 
		0x4e8f15de, 
		0x41c00000, 
		0x80000000, 
		0xa263f994, 
		0x80000020, 
		0x80080000, 
		0x42600000, 
		0xc2040000, 
		0xc2180000, 
		0x41e00000, 
		0xae991f69, 
		0x0c7c0000, 
		0x0e007fff, 
		0x02ab0147, 
		0x7f800000, 
		0x427c0000, 
		0x4a1be071, 
		0x0f7fffff, 
		0xb9a97b6b, 
		0xff800000, 
		0x0e000001, 
		0xc2400000, 
		0xc1e00000, 
		0x7f7ffffe, 
		0x01304b8f, 
		0xfff60e21, 
		0x532bd468, 
		0xc1d80000, 
		0xc0800000, 
		0xe11ebfdb, 
		0xc2600000, 
		0x80020000, 
		0x1ddddc1c, 
		0x42040000, 
		0x91e1ad5c, 
		0x80778947, 
		0x7f7ffffe, 
		0x80200000, 
		0x0c4d4185, 
		0x007ffffe, 
		0x00000010, 
		0xc0800000, 
		0x42480000, 
		0x76936560, 
		0x42200000, 
		0x0c7f8000, 
		0xed35fc58, 
		0x948b42e4, 
		0x8006cb45, 
		0x648ac072, 
		0x41000000, 
		0xf8ef5429, 
		0x0e00ffff, 
		0x00001000, 
		0x0c7ff800, 
		0x80200000, 
		0xd0d2f3d9, 
		0x3f800000, 
		0x58a111ee, 
		0x8f7ffffd, 
		0x80000000, 
		0xc2500000, 
		0x3f800000, 
		0x00000000, 
		0x9c2559f8, 
		0x41200000, 
		0x41a80000, 
		0x00000000, 
		0x00800000, 
		0xc1c80000, 
		0xc2440000, 
		0x55202fe0, 
		0x41b00000, 
		0x80800000, 
		0x4190432f, 
		0xcccccccc, 
		0x0e0fffff, 
		0x4b89ebf1, 
		0x0c700000, 
		0xbbc48560, 
		0x00800000, 
		0x60d521e4, 
		0x1aad118b, 
		0x42780000, 
		0x0c7ffe00, 
		0x7fc00000, 
		0xb3bbbe61, 
		0x10878f26, 
		0x4d694a8a, 
		0xff000000, 
		0x80182e28, 
		0x0e3fffff, 
		0xbf028f5c, 
		0x801f617d, 
		0x4bf17d80, 
		0xff979063, 
		0x0e1fffff, 
		0x41a80000, 
		0x80800000, 
		0xa836f609, 
		0x51a63b73, 
		0x418db8be, 
		0x004d69e0, 
		0x0f7ffffe, 
		0x0e00001f, 
		0xc0e00000, 
		0x60ddb574, 
		0x36528eb1, 
		0x80000100, 
		0x00800003, 
		0x7fc00001, 
		0x80ffffff, 
		0x7f7fffff, 
		0x5575236c, 
		0xf48d9dda, 
		0x00080000, 
		0xcccccccc, 
		0x80080000, 
		0x7fffffff, 
		0x4b8c4b40, 
		0xf948a2db, 
		0xc2480000, 
		0xc1900000, 
		0x42300000, 
		0xc20c0000, 
		0x00800000, 
		0x00000000, 
		0x0e03ffff, 
		0x0f7ffffe, 
		0xf65e9b2b, 
		0x80800001, 
		0xc1f80000, 
		0x93f76fdb, 
		0x6e54cd82, 
		0x0c7f8000, 
		0x7f800000, 
		0x00010000, 
		0x41600000, 
		0xc7b5680b, 
		0xff7fffff, 
		0x41800000, 
		0x00000000, 
		0x00000000, 
		0x2d1f98ec, 
		0x09153da4, 
		0xc1600000, 
		0xe38d3f4d, 
		0x80800001, 
		0x00000100, 
		0x081cf910, 
		0x7f800000, 
		0x60c0d8a9, 
		0x33fcb2b0, 
		0x93833ad7, 
		0x035dcb2e, 
		0xf7b31bfa, 
		0x0c7ffe00, 
		0x80000002, 
		0x80000040, 
		0x41f80000, 
		0xfdfcc0cb, 
		0x42300000, 
		0x80000000, 
		0xa85dc95f, 
		0x08b7b0b8, 
		0x40c00000, 
		0xc24c0000, 
		0xbb08a2fb, 
		0x00800002, 
		0x80010000, 
		0x80800001, 
		0x41f80000, 
		0x306a3b21, 
		0x7f800000, 
		0x00000080, 
		0x252a9fb0, 
		0x80800003, 
		0x80000000, 
		0x42000000, 
		0x41f00000, 
		0x6b452a93, 
		0x7f800000, 
		0x41a80000, 
		0x41880000, 
		0x42300000, 
		0x00000400, 
		0x7f800000, 
		0xff7ffffe, 
		0x9d6c9133, 
		0xff18d8c2, 
		0x80000000, 
		0x00000200, 
		0x42400000, 
		0xff800000, 
		0x7f7ffffe, 
		0x56f7a941, 
		0xc20c0000, 
		0xc1b00000, 
		0x80800000, 
		0xc20c0000, 
		0x6df47586, 
		0x80800000, 
		0x6c46d243, 
		0x42080000, 
		0x7fc00000, 
		0x0e000001, 
		0x002d2191, 
		0x49770109, 
		0x80800003, 
		0x80800003, 
		0x6ce51422, 
		0xc2280000, 
		0x0d74054d, 
		0x593a5dcc, 
		0x7f800001, 
		0xff7ffffe, 
		0xc1d80000, 
		0xec5f0ea2, 
		0x0e000007, 
		0x0c7ffffc, 
		0xc1f80000, 
		0xc2700000, 
		0x423c0000, 
		0x80411590, 
		0x4b8c4b40, 
		0x0c7ffe00, 
		0x80000100, 
		0x80000000, 
		0x4b000000, 
		0xe3652e57, 
		0x40000000, 
		0x80000080, 
		0xef9644cf, 
		0xb59548d3, 
		0x3d9eaaa1, 
		0x00000000, 
		0x543b4cf4, 
		0x4fd22e16, 
		0x00000000, 
		0x55555555, 
		0x95a1bfd2, 
		0x80080000, 
		0x42200000, 
		0xc47213ce, 
		0xffffffff, 
		0xc1300000, 
		0x44297a0f, 
		0x80000000, 
		0x80000000, 
		0x7fc00000, 
		0x7f800000, 
		0x85fad34c, 
		0xc24c0000, 
		0x00000020, 
		0x80000000, 
		0xc2680000, 
		0x80000000, 
		0x80000000, 
		0x42100000, 
		0xc2380000, 
		0x0f7fffff, 
		0x00800000, 
		0x42140000, 
		0x27199b31, 
		0x1421741f, 
		0xcbd32221, 
		0xea3d7abc, 
		0xc2140000, 
		0x9eebdeb2, 
		0x6f92d4c9, 
		0x7cf5a182, 
		0x422c0000, 
		0x7f800000, 
		0x80000002, 
		0x0e00ffff, 
		0x80000000, 
		0x00000000, 
		0x42600000, 
		0xc7e84f85, 
		0x00100000, 
		0x0c780000, 
		0x0c7f0000, 
		0xff800000, 
		0x66b43b5f, 
		0x7f7ffffe, 
		0x42640000, 
		0x80ffffff, 
		0xbf000257, 
		0xbf028f5c, 
		0x00000000, 
		0xc1e80000, 
		0x143d47d3, 
		0xd6abd802, 
		0xffacb11e, 
		0x2805cb86, 
		0x0c7e0000, 
		0x0c7ffff8, 
		0x00400000, 
		0x00040000, 
		0x8058d911, 
		0xbf800001, 
		0xffa817d5, 
		0x02e897d5, 
		0xd3c5f721, 
		0x0e0fffff, 
		0xff000000, 
		0x0c7c0000, 
		0xf4a86763, 
		0xff65e02c, 
		0x0c7f0000, 
		0x80000002, 
		0x41880000, 
		0x42300000, 
		0x422c0000, 
		0x00000000, 
		0x7fc00000, 
		0xc1300000, 
		0x00001000, 
		0x0e0000ff, 
		0x0c7ff800, 
		0x0c7e0000, 
		0xc1f00000, 
		0x0c7fff80, 
		0x42300000, 
		0x0c7ffffe, 
		0x1499888f, 
		0x820fa27d, 
		0x0c7fe000, 
		0x0c780000, 
		0xffbd90f2, 
		0x80000000, 
		0xff800000, 
		0xc2200000, 
		0x007ffffe, 
		0x4d3b562c, 
		0xa9ca1f27, 
		0xffc00001, 
		0xbf800001, 
		0xbc744a5c, 
		0x0f1a8abb, 
		0x807ffffe, 
		0x00000000, 
		0xaab37989, 
		0x80000000, 
		0x0e000007, 
		0x7ed926d6, 
		0x00200000, 
		0x00000000, 
		0x80000000, 
		0x00000000, 
		0xb47e2960, 
		0x40a00000, 
		0xbb752ad9, 
		0xacd8cca3, 
		0x41880000, 
		0x421c0000, 
		0x7fc00000, 
		0x6f7c695a, 
		0x00000004, 
		0xc1800000, 
		0x80040000, 
		0x0e000001, 
		0x42340000, 
		0x7f7fffff, 
		0x7fbe04a1, 
		0x7161f76f, 
		0x8221cfcd, 
		0x80000000, 
		0x00000000, 
		0xc2780000, 
		0x42280000, 
		0x501a0cf8, 
		0x00000000, 
		0x00400000, 
		0x8d217027, 
		0x80000010, 
		0xc1d80000, 
		0x7de9a4ee, 
		0x830bfe8a, 
		0x00011111, 
		0x0c600000, 
		0x7f800000, 
		0x00000002, 
		0xb80621d3, 
		0x302c58c2, 
		0x0c400000, 
		0xaa82fb7e, 
		0xff800000, 
		0xc1e80000, 
		0x3f028f5c, 
		0x80000000, 
		0x0c400000, 
		0x00000200, 
		0x00800001, 
		0x7f800000, 
		0xff7fffff, 
		0x30778636, 
		0xc2680000, 
		0x0d00fff0, 
		0x7f7ffffe, 
		0x7f000000, 
		0x0e0007ff, 
		0xc2740000, 
		0x330464fc, 
		0x80000200, 
		0x689f4d5f, 
		0xc31f8d80, 
		0x80000000, 
		0x7f800000, 
		0x42300000, 
		0x00000002, 
		0x8f7fffff, 
		0x7f7ffffe, 
		0x80040000, 
		0xf113b8b2, 
		0x054d6e9b, 
		0xdff105f2, 
		0x00000000, 
		0x00010000, 
		0xc1d80000, 
		0x0c7c0000, 
		0x0af6ec71, 
		0xc1500000, 
		0xae8cc408, 
		0x2f5646ad, 
		0xcccccccc, 
		0x00007e20, 
		0x5255afa4, 
		0x7f800000, 
		0x80020000, 
		0x67c8e1fb, 
		0xc2380000, 
		0xc2380000, 
		0x7f800001, 
		0xff800000, 
		0x364f18c8, 
		0x7f800000, 
		0x7f800000, 
		0x41f00000, 
		0xc1300000, 
		0xb543899c, 
		0x41200000, 
		0x80741f27, 
		0x00000000, 
		0xc5fd2268, 
		0x80000800, 
		0xc2080000, 
		0xc1e80000, 
		0x59ae177d, 
		0x0c400000, 
		0xa1e697ff, 
		0x482cc9fa, 
		0xc20b7441, 
		0xd3be0167, 
		0x00800000, 
		0xc2380000, 
		0xc35757a3, 
		0x7f7fffff, 
		0x0c780000, 
		0x80000000, 
		0x00010000, 
		0xc1f80000, 
		0xc2640000, 
		0x807fffff, 
		0x94853586, 
		0x78063f80, 
		0x99fde573, 
		0xf471d46c, 
		0xf545436e, 
		0xc2700000, 
		0xba136246, 
		0xff800000, 
		0xd5091100, 
		0x42380000, 
		0xec870494, 
		0x7f000000, 
		0x7f800000, 
		0x0c7fffe0, 
		0x0f7ffffd, 
		0x26a7f7f3, 
		0xa73925a8, 
		0x00800000, 
		0x807ffffe, 
		0x5fa45463, 
		0x807ffffe, 
		0x13cafa8d, 
		0x80000001, 
		0xc1880000, 
		0xc1980000, 
		0x00000000, 
		0x0e7fffff, 
		0x424c0000, 
		0x425c0000, 
		0x42780000, 
		0x75e6005c, 
		0x0e0fffff, 
		0x0f7ffffd, 
		0xa566190e, 
		0xed67d906, 
		0x80000040, 
		0xc2340000, 
		0x8d799d4c, 
		0xff800000, 
		0x0c7ffff0, 
		0x00000020, 
		0x031d0a38, 
		0x41c80000, 
		0xc1d80000, 
		0x7f7fffff, 
		0x80000800, 
		0x1fc2d2ea, 
		0x5e780cce, 
		0x805eaad6, 
		0x80000000, 
		0x80010000, 
		0x0c7ff800, 
		0x61fab4d4, 
		0x0c7fffff, 
		0x363b78a4, 
		0x42680000, 
		0xd6f50220, 
		0x80ffffff, 
		0x40400000, 
		0x42580000, 
		0x41980000, 
		0x80000001, 
		0x80000000, 
		0xc2440000, 
		0x0f7ffffe, 
		0x80000800, 
		0x80011111, 
		0x41900000, 
		0x0d00fff0, 
		0x00000010, 
		0x0c7c0000, 
		0x41880000, 
		0xcd8651cc, 
		0x0f7ffffd, 
		0x40a00000, 
		0x41100000, 
		0x0d000ff0, 
		0x37c0dd10, 
		0xaecc0eec, 
		0xffa53e42, 
		0x8f7ffffe, 
		0x01abe989, 
		0x0e00001f, 
		0xffb1f25b, 
		0x67cf80a0, 
		0x80000100, 
		0x50c3456a, 
		0x0c7fffff, 
		0xa3264dc7, 
		0x5926175b, 
		0x0c7fffc0, 
		0xc1a00000, 
		0x55555555, 
		0xa242ef52, 
		0x002eca58, 
		0xc0a00000, 
		0x426c0000  
};
volatile uint8_t self_check_8[1024] __attribute__ ((aligned (2048))) = {
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01  
};
volatile uint16_t self_check_16[1024] __attribute__ ((aligned (2048))) = {
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00  
};
volatile uint32_t self_check_32[1024] __attribute__ ((aligned (2048))) = {
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000  
};
volatile uint64_t check_sum_hid_0[16] __attribute__ ((aligned (1024))) = {
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff  
};
volatile uint64_t check_sum_hid_1[16] __attribute__ ((aligned (1024))) = {
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff  
};
volatile uint64_t check_sum_hid_2[16] __attribute__ ((aligned (1024))) = {
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff  
};
volatile uint64_t check_sum_hid_3[16] __attribute__ ((aligned (1024))) = {
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff  
};
volatile uint64_t check_sum_hid_4[16] __attribute__ ((aligned (1024))) = {
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff  
};
volatile uint64_t check_sum_hid_5[16] __attribute__ ((aligned (1024))) = {
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff  
};
volatile uint64_t check_sum_hid_6[16] __attribute__ ((aligned (1024))) = {
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff  
};
volatile uint64_t check_sum_hid_7[16] __attribute__ ((aligned (1024))) = {
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff  
};
volatile uint64_t check_sum_hid_8[16] __attribute__ ((aligned (1024))) = {
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff  
};
volatile uint64_t check_sum_hid_9[16] __attribute__ ((aligned (1024))) = {
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff  
};
volatile uint64_t check_sum_hid_10[16] __attribute__ ((aligned (1024))) = {
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff  
};
volatile uint64_t check_sum_hid_11[16] __attribute__ ((aligned (1024))) = {
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff  
};
volatile uint64_t check_sum_hid_12[16] __attribute__ ((aligned (1024))) = {
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff  
};
volatile uint64_t check_sum_hid_13[16] __attribute__ ((aligned (1024))) = {
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff  
};
volatile uint64_t check_sum_hid_14[16] __attribute__ ((aligned (1024))) = {
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff  
};
volatile uint64_t check_sum_hid_15[16] __attribute__ ((aligned (1024))) = {
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff  
};
volatile uint32_t mem_ptr_offset[256] __attribute__ ((aligned (1024))) = {
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c  
};
