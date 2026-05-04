/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "minion.h"
#include "macros.h"

/// Macros

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
			"csrr x5, mcause\n"      // read mcause
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
			"lui	x5,0x2\n"      // mstatus[12:11] = mpp = 2'b11
			"addiw	x5,x5,-2048\n" 
			"csrrs   zero, mstatus, x5\n" // mpp = m-mode
			"j LBL_RET_TO_CALLER\n"

			"LBL_EXCP_ECALL_FROM_SMODE_9:\n"
			"lui	x5,0x2\n"      // mstatus[12:11] = mpp = 2'b11
			"addiw	x5,x5,-2048\n" 
			"csrrs   zero, mstatus, x5\n" // mpp = m-mode
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
			"csrw mepc, x5\n"       // mepc = x5
			"li x5, 0xDEADDEAD\n"            
			"mret\n"                // ret to caller
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
			"mov.m.x m0, x0, 0xff\n"

			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"

			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0x162e\n" // 5678
			"slti x0, x0, 0x7f2\n"

			//{{{ //MAIN_SEQID_0_HID_0 num_seqs=8 approximate_num_gen_instrs = 6328
			"LBL_SEQID_0_HID_0:\n"
			"csrw tensor_error, zero\n" // clear the tensor error, may have been written due to unexpected pairing of tenb and tfma
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" // way0
			"csrw   evict_sw, x5\n" //  evict_way0, all sets
			"li x5,  0xc0000000000004f\n" // way1
			"csrw   evict_sw, x5\n" //  evict_way1, all sets
			"li x5,  0xc0000000000008f\n" // way2
			"csrw   evict_sw, x5\n" //  evict_way2, all sets
			"li x5,  0xc000000000000cf\n" // way3
			"csrw   evict_sw, x5\n" //  evict_way3, all sets
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
			"li x5,  0x1ff800000610007\n" // to reset tenc
			"csrw tensor_fma, x5\n" // reset_tenc
			"csrwi   tensor_wait, 7\n"

			"li x5,  0x1ff800000610007\n" // to exhaust any outstanding tenb ops
			"csrw tensor_fma, x5\n" // exhaust any outstanding tenb ops
			"csrwi   tensor_wait, 7\n"
			"csrwi   tensor_wait, 0x6\n"
			"li x5,  0xc0000000000000f\n" // way0
			"csrw   evict_sw, x5\n" //  evict_way0, all sets
			"li x5,  0xc0000000000004f\n" // way1
			"csrw   evict_sw, x5\n" //  evict_way1, all sets
			"li x5,  0xc0000000000008f\n" // way2
			"csrw   evict_sw, x5\n" //  evict_way2, all sets
			"li x5,  0xc000000000000cf\n" // way3
			"csrw   evict_sw, x5\n" //  evict_way3, all sets
			"csrwi   tensor_wait, 0x6\n"
			"csrrci x0, mcache_control, 3\n"
			"csrwi   tensor_wait, 0x6\n"
			"fence\n"
			//}}} //MAIN_SEQID_0_HID_0

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
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_0\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_0:\n"          
			".global LBL_FAIL_HID_0\n"          
			"li x5, 0xDEADDEADDEADDEAD\n"
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
			"li x5, 0xACEDACEDACEDACED\n"
			"slti x0, x0, 0x7fe\n"
			VSNIP_RSV
		);

		C_TEST_PASS;

		__asm__ __volatile__ ( 
			"LBL_HALT_HID_0:\n"          
			".global LBL_HALT_HID_0\n"          
			"wfi\n"
			"j LBL_HALT_HID_0\n"          
			VSNIP_RSV
		);
		return 0;       
	};
}
volatile uint32_t rand_ieee754_32[1024] __attribute__ ((aligned (2048))) = {
		0x41e00000, //    0
		0x00000000, //    1
		0x0c7fffe0, //    2
		0x41a00000, //    3
		0x00001000, //    4
		0x00010000, //    5
		0x00000000, //    6
		0x0f5b2dde, //    7
		0xff800000, //    8
		0x41e00000, //    9
		0xff800000, //   10
		0x00000400, //   11
		0xa0d976ef, //   12
		0x7f7ffffe, //   13
		0xf089e700, //   14
		0xbafef303, //   15
		0x0c7fffe0, //   16
		0x00000002, //   17
		0x3f028f5c, //   18
		0x80800000, //   19
		0x42740000, //   20
		0x41a80000, //   21
		0x0f7ffffe, //   22
		0x80000200, //   23
		0xc1900000, //   24
		0x00000000, //   25
		0xb6141421, //   26
		0x80800000, //   27
		0xcb000000, //   28
		0xc84e26b8, //   29
		0x7f898678, //   30
		0x0e00003f, //   31
		0x7f800000, //   32
		0xe2eb9000, //   33
		0x00800002, //   34
		0x00000000, //   35
		0x00800003, //   36
		0x0c7c0000, //   37
		0x41700000, //   38
		0xc1400000, //   39
		0x80000001, //   40
		0x0c400000, //   41
		0xffb114fc, //   42
		0x08a0c0e8, //   43
		0x80000004, //   44
		0x007fffff, //   45
		0x41d00000, //   46
		0x7f800000, //   47
		0x42680000, //   48
		0x45fe1a99, //   49
		0xc20c0000, //   50
		0x6fe2992a, //   51
		0x08ad82ce, //   52
		0x34f54858, //   53
		0xcb000000, //   54
		0x7fa18e44, //   55
		0x0c7fffe0, //   56
		0x0e3fffff, //   57
		0x42480000, //   58
		0x0c7ffff0, //   59
		0x42580000, //   60
		0xea5bd9e3, //   61
		0x8afa10a6, //   62
		0xa25c3c00, //   63
		0x80000000, //   64
		0x80008000, //   65
		0x00000001, //   66
		0xdf94f026, //   67
		0x42240000, //   68
		0x00040000, //   69
		0x8ad90777, //   70
		0xff800000, //   71
		0x00000080, //   72
		0x00400000, //   73
		0xc23c0000, //   74
		0x40c00000, //   75
		0x807fffff, //   76
		0x00000080, //   77
		0x0e00ffff, //   78
		0x3adbcd6f, //   79
		0x5431f337, //   80
		0xff800000, //   81
		0x80000000, //   82
		0x68076fe5, //   83
		0xb87fa7b9, //   84
		0x982b58d9, //   85
		0x7f800000, //   86
		0xff800000, //   87
		0x0f7ffffd, //   88
		0x3a96f09b, //   89
		0xc1b80000, //   90
		0x80000000, //   91
		0x00800003, //   92
		0xc0800000, //   93
		0x1cb3fbc8, //   94
		0x8f7ffffd, //   95
		0x80800001, //   96
		0x338d01f3, //   97
		0x80000008, //   98
		0x00000800, //   99
		0x40a00000, //  100
		0x0a1fdf3b, //  101
		0x0e07ffff, //  102
		0x23d2278a, //  103
		0xbf028f5c, //  104
		0x40e00000, //  105
		0x00000000, //  106
		0x00000000, //  107
		0x40a00000, //  108
		0x4b8c4b40, //  109
		0xc1980000, //  110
		0x42300000, //  111
		0x00571b2d, //  112
		0x42300000, //  113
		0xc2000000, //  114
		0x42200000, //  115
		0xc2780000, //  116
		0xa4e80bb7, //  117
		0x7fa4656d, //  118
		0x0e000fff, //  119
		0x0e00003f, //  120
		0x80200000, //  121
		0xc1a00000, //  122
		0xb5a24016, //  123
		0xff800000, //  124
		0x425c0000, //  125
		0xc1f00000, //  126
		0x00800001, //  127
		0xbf800001, //  128
		0x6aeefacf, //  129
		0x80000200, //  130
		0x7f800000, //  131
		0x41300000, //  132
		0xff7ffffe, //  133
		0x00000004, //  134
		0x31c39077, //  135
		0x14c0263d, //  136
		0x80800000, //  137
		0x00008000, //  138
		0x80000002, //  139
		0xdb4797c6, //  140
		0xc33a94f1, //  141
		0xc25c0000, //  142
		0x00000200, //  143
		0x0e007fff, //  144
		0x007ffffe, //  145
		0x7cdceed2, //  146
		0x00000000, //  147
		0xba637e61, //  148
		0x7f800000, //  149
		0x80000002, //  150
		0x42440000, //  151
		0x00800003, //  152
		0x0c7f8000, //  153
		0x0c7ffff0, //  154
		0x40400000, //  155
		0x803f7fdb, //  156
		0x80004000, //  157
		0x7f800001, //  158
		0x00800003, //  159
		0x00000000, //  160
		0xc2200000, //  161
		0xea1334e2, //  162
		0x41e80000, //  163
		0x0e07ffff, //  164
		0xc20c0000, //  165
		0x42040000, //  166
		0xb7af1b0c, //  167
		0x41d80000, //  168
		0xc1b80000, //  169
		0x80001000, //  170
		0x80100000, //  171
		0x45861c97, //  172
		0xc1c80000, //  173
		0x1dbcf6ad, //  174
		0xc0800000, //  175
		0xc2680000, //  176
		0xffffffff, //  177
		0x7f800000, //  178
		0x00010000, //  179
		0xffffffff, //  180
		0x7f7ffffe, //  181
		0x807ffffe, //  182
		0x80008000, //  183
		0x40000000, //  184
		0x00000001, //  185
		0xc1300000, //  186
		0x0c7ffff0, //  187
		0x80000000, //  188
		0x710c9875, //  189
		0xff7ffffe, //  190
		0x0c7ffffe, //  191
		0xc2200000, //  192
		0x00200000, //  193
		0x42700000, //  194
		0xc0000000, //  195
		0xbcbc0515, //  196
		0x7f53c9b1, //  197
		0x0e000001, //  198
		0xc2000000, //  199
		0x517425eb, //  200
		0xe859ba97, //  201
		0xb02787fc, //  202
		0xdc28119d, //  203
		0xc11c31b9, //  204
		0xc0e00000, //  205
		0xcccccccc, //  206
		0xf0d00d1b, //  207
		0x41800000, //  208
		0x80000040, //  209
		0x41200000, //  210
		0x424c0000, //  211
		0x8ada5940, //  212
		0x00800000, //  213
		0x807ffffe, //  214
		0xff800000, //  215
		0x422c0000, //  216
		0xa086d661, //  217
		0x80800002, //  218
		0xc1c80000, //  219
		0x42640000, //  220
		0x39f18689, //  221
		0x0e01ffff, //  222
		0x1b261a9f, //  223
		0x68824afa, //  224
		0x80000001, //  225
		0x989dc56c, //  226
		0x067b0f6d, //  227
		0x3f800000, //  228
		0xc8c47d77, //  229
		0xc2500000, //  230
		0x00400000, //  231
		0xbf028f5c, //  232
		0xc43aa18d, //  233
		0xc773abdb, //  234
		0x0048cb9c, //  235
		0x7fffffff, //  236
		0xc1c00000, //  237
		0xc0a00000, //  238
		0x41000000, //  239
		0x33333333, //  240
		0x1e42a661, //  241
		0xfff81a3e, //  242
		0xf74607c4, //  243
		0x00002000, //  244
		0x0e0001ff, //  245
		0xc1300000, //  246
		0xcb000000, //  247
		0x40400000, //  248
		0xb67803be, //  249
		0x41f00000, //  250
		0x42540000, //  251
		0x00001000, //  252
		0x00800003, //  253
		0x7fa24acc, //  254
		0x13f731f9, //  255
		0xc0800000, //  256
		0x0876335d, //  257
		0x40400000, //  258
		0xc1c80000, //  259
		0x00000000, //  260
		0xa555fae6, //  261
		0x0e00007f, //  262
		0xc1200000, //  263
		0xc2240000, //  264
		0x4734733f, //  265
		0xb67a6d84, //  266
		0x0c7ffff0, //  267
		0x00100000, //  268
		0x41980000, //  269
		0xf7ee0a0f, //  270
		0x007ffffe, //  271
		0x80000001, //  272
		0xc1880000, //  273
		0x2b5e0773, //  274
		0x80000000, //  275
		0x80001000, //  276
		0x80000000, //  277
		0x7f7fffff, //  278
		0x51cbc9ef, //  279
		0xc1700000, //  280
		0x0e00001f, //  281
		0x42340000, //  282
		0xff800000, //  283
		0x7fbfffff, //  284
		0x0d2edc85, //  285
		0x0858f550, //  286
		0x80100000, //  287
		0xc1b80000, //  288
		0x00200000, //  289
		0x80000000, //  290
		0x00800003, //  291
		0x7f800000, //  292
		0x80001000, //  293
		0x42740000, //  294
		0xff7ffffe, //  295
		0x007fffff, //  296
		0xc24c0000, //  297
		0x57741663, //  298
		0x7493cc34, //  299
		0x47a4bf61, //  300
		0xc2440000, //  301
		0x41980000, //  302
		0x42700000, //  303
		0x0e00001f, //  304
		0xbe7c1013, //  305
		0x7f800000, //  306
		0xffe4d6a0, //  307
		0x586488ee, //  308
		0x80000000, //  309
		0x758539d9, //  310
		0xc58edd21, //  311
		0xc2580000, //  312
		0x80800000, //  313
		0x00004000, //  314
		0xc1880000, //  315
		0x00000004, //  316
		0x0c7ffffe, //  317
		0xd5c4277e, //  318
		0x8f7ffffc, //  319
		0x7fbfffff, //  320
		0x7f800000, //  321
		0x6d30f549, //  322
		0x43961c16, //  323
		0x0c7ffffe, //  324
		0x80100000, //  325
		0xff800000, //  326
		0x42534e3e, //  327
		0x5f539602, //  328
		0x41500000, //  329
		0x00008000, //  330
		0x15a73e28, //  331
		0x00000002, //  332
		0x96a59cec, //  333
		0xffbfffff, //  334
		0x33333333, //  335
		0x42380000, //  336
		0xff800000, //  337
		0xc1f80000, //  338
		0x204cb461, //  339
		0xc1980000, //  340
		0x8f7ffffe, //  341
		0x5cf49e7c, //  342
		0xf769cdbb, //  343
		0x1401ffa7, //  344
		0x0c7ffff0, //  345
		0xc1d80000, //  346
		0x42200000, //  347
		0x42080000, //  348
		0x42280000, //  349
		0x41a80000, //  350
		0x4b000000, //  351
		0xc2680000, //  352
		0xc1800000, //  353
		0xf2157932, //  354
		0x42240000, //  355
		0x0c7fe000, //  356
		0x41e00000, //  357
		0x00800001, //  358
		0xd0cd7507, //  359
		0xb73d415c, //  360
		0xc2080000, //  361
		0x5587ce91, //  362
		0x7f800000, //  363
		0x7f800000, //  364
		0xff7ffffe, //  365
		0xc6fea19e, //  366
		0x00000000, //  367
		0x00000200, //  368
		0x9de62b6e, //  369
		0x00800001, //  370
		0x806e037a, //  371
		0xc5b1cb66, //  372
		0x40c00000, //  373
		0x0e6d6615, //  374
		0x11737b0d, //  375
		0x429908ae, //  376
		0x80800003, //  377
		0xc2300000, //  378
		0x41e80000, //  379
		0xc2180000, //  380
		0x80004000, //  381
		0xc2100000, //  382
		0xa87b5d17, //  383
		0x0e1fffff, //  384
		0x0e7fffff, //  385
		0x94a1f350, //  386
		0x0e0003ff, //  387
		0x99062b3b, //  388
		0xeec240d0, //  389
		0x53325677, //  390
		0x6cef1056, //  391
		0x80000001, //  392
		0xc2600000, //  393
		0x424c0000, //  394
		0x42580000, //  395
		0x80010000, //  396
		0x2ad2089c, //  397
		0x0e00ffff, //  398
		0xff800000, //  399
		0xff800000, //  400
		0x7f800000, //  401
		0x80ee8b70, //  402
		0x1b7eecb9, //  403
		0xb5f82aab, //  404
		0x41900000, //  405
		0x0f1da59f, //  406
		0x424c0000, //  407
		0x7fc00000, //  408
		0x80800001, //  409
		0xc153c3ef, //  410
		0xa2c39626, //  411
		0x41a80000, //  412
		0x80000000, //  413
		0x4abf9b72, //  414
		0x0c7ffffe, //  415
		0xcccccccc, //  416
		0x8f7ffffe, //  417
		0x427c0000, //  418
		0x41c00000, //  419
		0x7fa58d92, //  420
		0x00000000, //  421
		0x41b80000, //  422
		0xc2100000, //  423
		0x06a4a54d, //  424
		0xc90b040f, //  425
		0x42240000, //  426
		0x0c400000, //  427
		0xff800000, //  428
		0x33333333, //  429
		0xc2280000, //  430
		0xbf800000, //  431
		0xff9d096f, //  432
		0x83e0c581, //  433
		0x417d1e09, //  434
		0x80011111, //  435
		0xcb000000, //  436
		0x2a6248aa, //  437
		0xc1980000, //  438
		0x80000000, //  439
		0x00000000, //  440
		0xc1100000, //  441
		0xc21c0000, //  442
		0xd9e1f3f4, //  443
		0x0e00000f, //  444
		0x9b48744a, //  445
		0x0c7ff800, //  446
		0x3b97e289, //  447
		0x00010000, //  448
		0xff7fffff, //  449
		0xc1f80000, //  450
		0x00000000, //  451
		0xa83189ac, //  452
		0xc1d80000, //  453
		0x42440000, //  454
		0x42240000, //  455
		0x00000200, //  456
		0xc1c80000, //  457
		0xc1800000, //  458
		0xc306dd46, //  459
		0x0f7ffffc, //  460
		0x0c7fffc0, //  461
		0xc21c0000, //  462
		0x0c7fffff, //  463
		0x5b89adb0, //  464
		0xff800000, //  465
		0xc1f80000, //  466
		0x80000000, //  467
		0x0e007fff, //  468
		0x1f9adb98, //  469
		0xfd1dc626, //  470
		0xc20c0000, //  471
		0x0e07ffff, //  472
		0x0c7fffff, //  473
		0x0b0c6a70, //  474
		0x0c7ffff8, //  475
		0x80000000, //  476
		0x0e3fffff, //  477
		0x41c00000, //  478
		0x80010000, //  479
		0x7f800000, //  480
		0x80000001, //  481
		0x0e00001f, //  482
		0x80000000, //  483
		0x684e3f4d, //  484
		0x7fc00001, //  485
		0xa3d3f220, //  486
		0x42180000, //  487
		0x00000020, //  488
		0x42500000, //  489
		0x0c7fc000, //  490
		0xc2200000, //  491
		0xea82556f, //  492
		0x42040000, //  493
		0x80000000, //  494
		0xc23c0000, //  495
		0x98389a91, //  496
		0x41600000, //  497
		0x96de3b86, //  498
		0x0e0007ff, //  499
		0x42540000, //  500
		0x80800002, //  501
		0xa5731c16, //  502
		0x0e00003f, //  503
		0x0e000003, //  504
		0x80000001, //  505
		0xc1e80000, //  506
		0x00000000, //  507
		0x4f76de36, //  508
		0xff800000, //  509
		0x41000000, //  510
		0x80000080, //  511
		0xe9c0e7d6, //  512
		0x25b59fe9, //  513
		0x7c526b54, //  514
		0x80aaae5d, //  515
		0x42100000, //  516
		0x9e4f8873, //  517
		0x80800001, //  518
		0xc2000000, //  519
		0x8f7ffffd, //  520
		0xc2080000, //  521
		0x7fffffff, //  522
		0x082ec3c4, //  523
		0x0d000ff0, //  524
		0xa215542e, //  525
		0x00400000, //  526
		0xffc00000, //  527
		0xe7c1589e, //  528
		0x00800003, //  529
		0xc0c00000, //  530
		0x80000000, //  531
		0x41700000, //  532
		0xf9b676f9, //  533
		0x0957d3f7, //  534
		0x0c7fff80, //  535
		0x176a5fde, //  536
		0x7f800000, //  537
		0xc29f15da, //  538
		0x80002000, //  539
		0x40e00000, //  540
		0x4f7cbbcb, //  541
		0x80800000, //  542
		0xb5af63ae, //  543
		0x31d9a996, //  544
		0x42600000, //  545
		0xc24c0000, //  546
		0xbf800000, //  547
		0xbc8f220b, //  548
		0x7f369d29, //  549
		0xc2100000, //  550
		0x42280000, //  551
		0x42140000, //  552
		0x75ece10c, //  553
		0x41e00000, //  554
		0xff800000, //  555
		0x7f7ffffe, //  556
		0x00000100, //  557
		0x680aebfe, //  558
		0x42400000, //  559
		0x00400000, //  560
		0x7ec792c1, //  561
		0x00000001, //  562
		0xe6e8a6c1, //  563
		0x42080000, //  564
		0x7f800000, //  565
		0x0c7ff800, //  566
		0x80008000, //  567
		0x42540000, //  568
		0xc1d00000, //  569
		0x80200000, //  570
		0xc1900000, //  571
		0x7f800000, //  572
		0x00800000, //  573
		0xc1a80000, //  574
		0x23601752, //  575
		0x853989a3, //  576
		0x20659425, //  577
		0xffc00000, //  578
		0xc2540000, //  579
		0x7ef59140, //  580
		0xc2580000, //  581
		0xbf028f5c, //  582
		0x59e8df82, //  583
		0x338b70a2, //  584
		0x80000040, //  585
		0xd75f283b, //  586
		0xa7bfac9a, //  587
		0x80800003, //  588
		0x8051433a, //  589
		0x8f7ffffc, //  590
		0xd965ac9f, //  591
		0x0e07ffff, //  592
		0x41200000, //  593
		0x2211ee24, //  594
		0x80000200, //  595
		0x80001000, //  596
		0xc2440000, //  597
		0x7f800000, //  598
		0x42740000, //  599
		0xcb000000, //  600
		0x20c0a0ad, //  601
		0x23d36c6f, //  602
		0x51e32844, //  603
		0x0d000ff0, //  604
		0x0e000001, //  605
		0x00000004, //  606
		0x41000000, //  607
		0xc2040000, //  608
		0xcab9d331, //  609
		0x007fffff, //  610
		0xa77ea16a, //  611
		0x3c9683cf, //  612
		0xc2740000, //  613
		0x380d6cd6, //  614
		0xff7ffffe, //  615
		0x42480000, //  616
		0x00000008, //  617
		0xff8a558d, //  618
		0x85179130, //  619
		0x00800000, //  620
		0x427c0000, //  621
		0x00000000, //  622
		0x42500000, //  623
		0xff800000, //  624
		0x80800002, //  625
		0x75852c9d, //  626
		0x7f800000, //  627
		0x42340000, //  628
		0x421c0000, //  629
		0xc2240000, //  630
		0xff800000, //  631
		0xc22c0000, //  632
		0xffc00000, //  633
		0x80000000, //  634
		0x00200000, //  635
		0x00000000, //  636
		0x0c7fff80, //  637
		0x00000000, //  638
		0x40400000, //  639
		0x0f7ffffd, //  640
		0x41880000, //  641
		0xeb139d04, //  642
		0xc1b00000, //  643
		0xf0685985, //  644
		0x41d80000, //  645
		0x807fffff, //  646
		0x7f800000, //  647
		0xc1c00000, //  648
		0x007fffff, //  649
		0xde310d9a, //  650
		0xff800000, //  651
		0xcb8c4b40, //  652
		0x7fc00000, //  653
		0xff8945cb, //  654
		0xff800000, //  655
		0x80000000, //  656
		0x0c7fffc0, //  657
		0x0e00ffff, //  658
		0xd39d4d28, //  659
		0xff7ffffe, //  660
		0xc1b80000, //  661
		0x7f800000, //  662
		0x80020000, //  663
		0x00000400, //  664
		0x00100000, //  665
		0x00100000, //  666
		0x6215a8d9, //  667
		0x80800003, //  668
		0x0e0001ff, //  669
		0x780e0ee5, //  670
		0x42640000, //  671
		0x00010000, //  672
		0x0c7ffffe, //  673
		0x42080000, //  674
		0x40400000, //  675
		0xfc437a0a, //  676
		0xc20c0000, //  677
		0x00000000, //  678
		0x80000004, //  679
		0x3f800001, //  680
		0x80000000, //  681
		0x33f9bcbb, //  682
		0x01ca44de, //  683
		0x80800003, //  684
		0x42540000, //  685
		0x00000080, //  686
		0xaaaaaaaa, //  687
		0xff000000, //  688
		0xc1f80000, //  689
		0xc15f69dd, //  690
		0x7f7ffffe, //  691
		0xbce03840, //  692
		0xc1980000, //  693
		0xffffffff, //  694
		0x80777a30, //  695
		0xff800000, //  696
		0xc2400000, //  697
		0x00000002, //  698
		0x1d33c584, //  699
		0x9463b81c, //  700
		0x00000000, //  701
		0x9103daa2, //  702
		0x8f7ffffd, //  703
		0x80000000, //  704
		0x80000800, //  705
		0xff000000, //  706
		0x00008000, //  707
		0x0e7fffff, //  708
		0x0e0003ff, //  709
		0x286d8499, //  710
		0x80008000, //  711
		0xc2000000, //  712
		0xf1509150, //  713
		0xd188c9e2, //  714
		0x80000000, //  715
		0x00008000, //  716
		0x00000000, //  717
		0x00800002, //  718
		0x41c00000, //  719
		0x0c780000, //  720
		0x41f80000, //  721
		0x5b8dac14, //  722
		0x2b8f0152, //  723
		0xc2340000, //  724
		0x7be19bbf, //  725
		0x00010000, //  726
		0x41300000, //  727
		0x00000000, //  728
		0x0e00ffff, //  729
		0x80000000, //  730
		0x00800003, //  731
		0xfff7f3a4, //  732
		0x0e00001f, //  733
		0xb4e00796, //  734
		0x0e7fffff, //  735
		0x83b1abc2, //  736
		0x80200000, //  737
		0x00000800, //  738
		0xff7fffff, //  739
		0x41d50f17, //  740
		0x422c0000, //  741
		0x3a5cfe4c, //  742
		0x007fffff, //  743
		0x45601775, //  744
		0x40400000, //  745
		0x41e00000, //  746
		0x80800001, //  747
		0xa7300843, //  748
		0xfa371979, //  749
		0x7fbfffff, //  750
		0x7f8c956f, //  751
		0xff7fffff, //  752
		0xc21c0000, //  753
		0xcaffc923, //  754
		0xc0800000, //  755
		0x41e00000, //  756
		0x42740000, //  757
		0x00000000, //  758
		0x0e7fffff, //  759
		0x785e69a8, //  760
		0x42680000, //  761
		0x02dd563e, //  762
		0x014481c5, //  763
		0x7f800000, //  764
		0x426c0000, //  765
		0x8f6ebd8a, //  766
		0x41f00000, //  767
		0x42400000, //  768
		0x00ffffff, //  769
		0xc1600000, //  770
		0x0e007fff, //  771
		0x60d4a0d2, //  772
		0x42400000, //  773
		0x0e00ffff, //  774
		0xc26c0000, //  775
		0x42000000, //  776
		0x981ff124, //  777
		0x41b80000, //  778
		0x7f800000, //  779
		0x0e0001ff, //  780
		0x7f800000, //  781
		0xc2000000, //  782
		0x41b80000, //  783
		0x80800002, //  784
		0xff800000, //  785
		0x0821a350, //  786
		0xc24c0000, //  787
		0x80000000, //  788
		0xc1a00000, //  789
		0xc272f4da, //  790
		0x80020000, //  791
		0x80001000, //  792
		0x8f7ffffc, //  793
		0xaa793ba1, //  794
		0x7f7ffffe, //  795
		0x0c7ffff8, //  796
		0x0c600000, //  797
		0x41f00000, //  798
		0x0c7fffc0, //  799
		0x0c600000, //  800
		0x41c80000, //  801
		0xf7914946, //  802
		0xff800000, //  803
		0x7820319d, //  804
		0x0c2dd4d0, //  805
		0x55555555, //  806
		0x00800003, //  807
		0x8fd98a0d, //  808
		0xc0800000, //  809
		0x80020000, //  810
		0xb0dd0851, //  811
		0x426c0000, //  812
		0xc0000000, //  813
		0x80800001, //  814
		0x0c7ffff8, //  815
		0xb7d07bd3, //  816
		0x43815c10, //  817
		0x521c7e55, //  818
		0x40a00000, //  819
		0x80000000, //  820
		0x41d80000, //  821
		0x0e00003f, //  822
		0x025e483c, //  823
		0x8f7ffffe, //  824
		0x0e07ffff, //  825
		0x2b33e7e5, //  826
		0x09f0b5b9, //  827
		0x80000000, //  828
		0x90801589, //  829
		0x0e000001, //  830
		0x0a831754, //  831
		0x7f800000, //  832
		0x41e00000, //  833
		0x00100000, //  834
		0xa4181be5, //  835
		0x99d19a92, //  836
		0xc2040000, //  837
		0x42140000, //  838
		0x7f800000, //  839
		0x1d8cd119, //  840
		0xcc631d0c, //  841
		0x00000000, //  842
		0x80000000, //  843
		0x3870f536, //  844
		0x80800001, //  845
		0xc2080000, //  846
		0x40c00000, //  847
		0x00400000, //  848
		0x80000800, //  849
		0xb23b7c67, //  850
		0xc6372c9b, //  851
		0x1a5223a4, //  852
		0x0e000003, //  853
		0x001683f2, //  854
		0x7f800000, //  855
		0xaee0dfcb, //  856
		0x00001000, //  857
		0xc1e00000, //  858
		0x2e51b18f, //  859
		0x00000000, //  860
		0x1994b46a, //  861
		0x8c3c0c8d, //  862
		0x0e0000ff, //  863
		0x00000002, //  864
		0xfe9ea270, //  865
		0x760c6f85, //  866
		0xff7ffffe, //  867
		0x67172ed8, //  868
		0xc1900000, //  869
		0xe1f25218, //  870
		0x41d00000, //  871
		0xc2180fcd, //  872
		0x7a03b98d, //  873
		0xc1800000, //  874
		0x41f00000, //  875
		0x7f800000, //  876
		0x5691d921, //  877
		0x80000000, //  878
		0x006fdc3e, //  879
		0xeddeb9b9, //  880
		0x00000000, //  881
		0x00000020, //  882
		0xff800000, //  883
		0x40a00000, //  884
		0x8a72462c, //  885
		0x41980000, //  886
		0x6950e1fd, //  887
		0xa1336b52, //  888
		0x42200000, //  889
		0x7f7ffffe, //  890
		0xc2200000, //  891
		0xff7ffffe, //  892
		0x00000080, //  893
		0x00002000, //  894
		0x41800000, //  895
		0xc21c0000, //  896
		0xa38eaebf, //  897
		0xff800000, //  898
		0xc1000000, //  899
		0xc2300000, //  900
		0xff7ffffe, //  901
		0x80000000, //  902
		0xc25c0000, //  903
		0x7fdcab90, //  904
		0xc2080000, //  905
		0xb3717886, //  906
		0x40a00000, //  907
		0x0c7ffffe, //  908
		0x0e00001f, //  909
		0x0c7e0000, //  910
		0xc1300000, //  911
		0x5dffa546, //  912
		0x41000000, //  913
		0xc1200000, //  914
		0x80000004, //  915
		0x0e00000f, //  916
		0x00000080, //  917
		0x00800002, //  918
		0x80000040, //  919
		0x41300000, //  920
		0xffc00001, //  921
		0xc1c80000, //  922
		0xc1b80000, //  923
		0x2e98c0d7, //  924
		0x7fa895e0, //  925
		0x7f7ffffe, //  926
		0x7ee914fd, //  927
		0x8012a9d8, //  928
		0xc1100000, //  929
		0x00000001, //  930
		0x80100000, //  931
		0x0c7fc000, //  932
		0x00040000, //  933
		0xc1e80000, //  934
		0xf6ae783a, //  935
		0x00000000, //  936
		0x41e00000, //  937
		0x0c7c0000, //  938
		0xf5871e7c, //  939
		0x0c780000, //  940
		0x00040000, //  941
		0x80800003, //  942
		0x1e264d10, //  943
		0x410201ae, //  944
		0xadd37df4, //  945
		0xa7743684, //  946
		0xffffffff, //  947
		0x58f3af26, //  948
		0xc2680000, //  949
		0x80000000, //  950
		0x0e00ffff, //  951
		0x42140000, //  952
		0x41a80000, //  953
		0xff800000, //  954
		0x42480000, //  955
		0x00000000, //  956
		0xe7c8e1a0, //  957
		0xc0a00000, //  958
		0xff7ffffe, //  959
		0x00000000, //  960
		0x42480000, //  961
		0x41000000, //  962
		0x80000000, //  963
		0x74b8a2bc, //  964
		0x72e290ea, //  965
		0x41f00000, //  966
		0x00800000, //  967
		0x7fadc6d6, //  968
		0x41600000, //  969
		0x2563937f, //  970
		0x00000100, //  971
		0x80000001, //  972
		0x0c7fffff, //  973
		0x40400000, //  974
		0x7fbcae63, //  975
		0x311c5c1f, //  976
		0x41500000, //  977
		0x80000040, //  978
		0x7f800000, //  979
		0x00000010, //  980
		0xffbfffff, //  981
		0x41900000, //  982
		0x00100000, //  983
		0xaf26b4e8, //  984
		0xff800000, //  985
		0x0c7f8000, //  986
		0x42480000, //  987
		0xc0a00000, //  988
		0xb9fed9c6, //  989
		0x00000001, //  990
		0x0c700000, //  991
		0x80000800, //  992
		0x3f800000, //  993
		0x7f8d80ce, //  994
		0xc1a80000, //  995
		0x007ffffe, //  996
		0x00080000, //  997
		0xc2400000, //  998
		0xb75d6fed, //  999
		0x1293390c, // 1000
		0x00000001, // 1001
		0x0c7ffe00, // 1002
		0x80000000, // 1003
		0x7f800001, // 1004
		0x80000000, // 1005
		0x100c718d, // 1006
		0xc1400000, // 1007
		0x7fc00001, // 1008
		0x424c0000, // 1009
		0xff7ffffe, // 1010
		0x0c7c0000, // 1011
		0xdb41ae16, // 1012
		0x0c7ffc00, // 1013
		0xc1100000, // 1014
		0x806b593f, // 1015
		0xff800000, // 1016
		0x3f800000, // 1017
		0x41d00000, // 1018
		0x80000040, // 1019
		0x80200000, // 1020
		0x00800000, // 1021
		0x00000000, // 1022
		0xc2480000  // 1023
};
