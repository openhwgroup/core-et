#!/usr/bin/python
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import os
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--status","-x", help="set hex number in format 0x<NUM>")
args=parser.parse_args()
decode_number_str=args.status
decode_number=int(decode_number_str, 16)
  
MASK_MOVA_MX = 0x1
MASK_FPTOINT = 0x2
MASK_GATHER_SCATTER = 0x4
MASK_CSR = 0x8
MASK_DCACHE_NOT_READY = 0x10
MASK_FENCE = 0x20
MASK_IDIV = 0x40
MASK_DEP_TX_RCV_F = 0x80
MASK_DEP_CACHE_F = 0x100
MASK_DEP_LONG_F = 0x200
MASK_DEP_LONG_I =0x400
MASK_DEP_4 = 0x800
MASK_DEP_3 = 0x1000
MASK_DEP_2= 0x2000
MASK_DEP_1= 0x4000
MASK_IC = 0x8000
MASK_MISPREDICT = 0x10000
MASK_REPLAY = 0x20000
MASK_LONGF = 0xC0000
MASK_LOAD_STORE = 0x300000
MASK_L1 = 0xC00000
MASK_TX = 0x3000000
MASK_VPU = 0x7C000000
MASK_L1MISS = 0x1F80000000
MASK_GTSC = 0x2000000000
MASK_FMA_CSR = 0xC000000000

mova_mx = decode_number & MASK_MOVA_MX
fptoint = decode_number & MASK_FPTOINT
gather_scatter = decode_number & MASK_GATHER_SCATTER
csr = decode_number & MASK_CSR
dcache_not_ready = decode_number & MASK_DCACHE_NOT_READY
fence = decode_number & MASK_FENCE
idiv = decode_number & MASK_IDIV
dep_tx_rcv_f = decode_number & MASK_DEP_TX_RCV_F
dep_dcache_f = decode_number & MASK_DEP_CACHE_F
dep_long_f = decode_number & MASK_DEP_LONG_F
dep_long_i = decode_number & MASK_DEP_LONG_I
dep_4 = decode_number & MASK_DEP_4
dep_3 = decode_number & MASK_DEP_3
dep_2 = decode_number & MASK_DEP_2
dep_1 = decode_number & MASK_DEP_1
ic = decode_number & MASK_IC
mispredict = decode_number & MASK_MISPREDICT
replay = decode_number & MASK_REPLAY
longf = decode_number & MASK_LONGF
load_store = decode_number & MASK_LOAD_STORE
l1 = decode_number & MASK_L1
tx = decode_number & MASK_TX
vpu = decode_number & MASK_VPU
l1_miss = decode_number & MASK_L1MISS
gtsc = decode_number & MASK_GTSC
fma_csr = decode_number & MASK_FMA_CSR
 
status=["MOVA_MX", "FPToInt","Gather_Scatter","CSR", "D$_not_ready", "Fence", "iDiv", "Dep_TX_RCV_F","Dep_Dcache_F", "Dep_Long_F", "Dep_Long_I", "Dep_4", "Dep_3", "Dep_2", "Dep_1", "IC", "Mis_Predict",
        "Replay", "Busy_Long_F", "WR_Long_F", "Store", "Load", "L1_Miss", "L1_Evict", "TX_Send", "TX_RCV", "VPU-Tensor_FMA", "VPU-Transcend","VPU-Tensor_LD", "VPU-Tensor_Load_Op", "VPU-Tensor_Store_Op",
        "L1_Miss_Load", "L1_Miss_Store", "L1_Miss_Prefetch_RO", "L1_Miss_Prefetch_R1", "GTSC_Gather-Scatter", "FMA_CSR"]

writeback = ["M_Read", "FPTOINT_In_Progress", "Not_Implemented_Yet(Always_0)", "CSR_Stall", "D$_Not_Ready", "Fence_Operation", "DIV_Not_Ready", "TX_RCV_Depend", "D$_Depend", "VPU_Stall", "Scoreboard",
             "WB_Stage", "Men_Stage", "Tag_stage", "EX_stage", "I$_Fetch_Valid", "Tag_mis-predict", "Reply_exception_ERET", "Trans_Busy-Not_Implemented", "FP_Inst",
             "Mem_WB(!Wfd/Wxd-Float_Single,Atomics,Packed_Single)", "Mem_WB(Wfd/Wxd-Float_Single,Atomics,Packed_Single)", "L1->L2_Miss", "L1_Evict", "Tx_Msg_send_data", "Not_Implemented",
             "Tensor(TIMA||TXFMA)", "Trans_Div(Not_Implemented)", "FSM_Busy", "L2_Req", "L2_Req&Reduce", "PMH_0_Miss", "PMH_1_Miss", "Memory_Type_Access_GS-{8,16,32}", "CSR:Tensor_FMA",                    
             "CSR:Tensor_FMA_Control_Disable","CSR:Tensor_FMA_FP16*FP32", "CSR:Tensor_FMA_FP16", "CSR:Tensor_FMA_Int8*Int32"]


#f=open("decoded_wbstatus.txt","w+")

def str_to_write(wstr):
    data = wstr.split()
    ajust_data = '{0[0]:<7}{0[1]:<35}{0[2]:<6}{0[3]:<11}{0[4]:<53}{0[5]:<3}{0[6]:<9}'.format(data)
    print(ajust_data + "\n")
#    f.write(ajust_data + "\n")
    return

def str_to_write_all_dis(wstrd):
#    line = "Write back status for [" + status[0] + "] are all disabled"
    data = wstrd.split()
    ajust_data = '{0[0]:<6}{0[1]:<5}{0[2]:<7}{0[3]:<4}{0[4]:<20}{0[5]:<4}{0[6]:<4}{0[7]:<9}'.format(data)
    print(ajust_data + "\n")
#    f.write(ajust_data + "\n")
    return

def str_to_write_all_enab(wstre):
    data = wstre.split()
    ajust_data = '{0[0]:<9}{0[1]:<6}{0[2]:<5}{0[3]:<7}{0[4]:<4}{0[5]:<20}{0[6]:<9}{0[7]:<7}{0[8]:<9}'.format(data)
    print(ajust_data + "\n")
#    f.write(ajust_data + "\n")
    return

if(mova_mx):
     str_to_write("Status [" + status[0] + "] with writeback [" +writeback[0]+ "] is enabled")
else:
     str_to_write("Status [" + status[0] + "] with writeback [" + writeback[0] + "] is disabled")
#    f.write("Status [" + status[0] + "] with writeback [" + writeback[0] + "] is disabled \n")

if(fptoint):
     str_to_write("Status [" + status[1] + "] with writeback [" + writeback[1] + "] is enabled")
else:
    str_to_write("Status [" + status[1] + "] with writeback [" + writeback[1] + "] is disabled")

if(gather_scatter):
    str_to_write("Status [" + status[2] + "] with writeback [" + writeback[2] + "] is enabled")
else:
    str_to_write("Status [" + status[2] + "] with writeback [" + writeback[2] + "] is disabled")

if(csr):
    str_to_write("Status [" + status[3] + "] with writeback [" + writeback[3] + "] is enabled")
else:
    str_to_write("Status [" + status[3] + "] with writeback [" + writeback[3] + "] is disabled")
    
if(dcache_not_ready):
    str_to_write("Status [" + status[4] + "] with writeback [" + writeback[4] + "] is enabled")
else:
    str_to_write("Status [" + status[4] + "] with writeback [" + writeback[4] + "] is disabled")

if(fence):
    str_to_write("Status [" + status[5] + "] with writeback [" + writeback[5] + "] is enabled")
else:
    str_to_write("Status [" + status[5] + "] with writeback [" + writeback[5] + "] is disabled")

if(idiv):
    str_to_write("Status [" + status[6] + "] with writeback [" + writeback[6] + "] is enabled")
else:
    str_to_write("Status [" + status[6] + "] with writeback [" + writeback[6] + "] is disabled")

if(dep_tx_rcv_f):
    str_to_write("Status [" + status[7] + "] with writeback [" + writeback[7] + "] is enabled")
else:
    str_to_write("Status [" + status[7] + "] with writeback [" + writeback[7] + "] is disabled")

if(dep_dcache_f):
    str_to_write("Status [" + status[8] + "] with writeback [" + writeback[8] + "] is enabled")
else:
    str_to_write("Status [" + status[8] + "] with writeback [" + writeback[8] + "] is disabled")

if(dep_long_f):
    str_to_write("Status [" + status[9] + "] with writeback [" + writeback[9] + "] is enabled")
else:
    str_to_write("Status [" + status[9] + "] with writeback [" + writeback[9] + "] is disabled")

if(dep_long_i):
    str_to_write("Status [" + status[10] + "] with writeback [" + writeback[10] + "] is enabled")
else:
    str_to_write("Status [" + status[10] + "] with writeback [" + writeback[10] + "] is disabled")

if(dep_4):
    str_to_write("Status [" + status[11] + "] with writeback [" + writeback[11] + "] is enabled")
else:
    str_to_write("Status [" + status[11] + "] with writeback [" + writeback[11] + "] is disabled")

if(dep_3):
    str_to_write("Status [" + status[12] + "] with writeback [" + writeback[12] + "] is enabled")
else:
    str_to_write("Status [" + status[12] + "] with writeback [" + writeback[12] + "] is disabled")

if(dep_2):
    str_to_write("Status [" + status[13] + "] with writeback [" + writeback[13] + "] is enabled")
else:
    str_to_write("Status [" + status[13] + "] with writeback [" + writeback[13] + "] is disabled")

if(dep_1):
    str_to_write("Status [" + status[14] + "] with writeback [" + writeback[14] + "] is enabled")
else:
    str_to_write("Status [" + status[14] + "] with writeback [" + writeback[14] + "] is disabled")
 
if(ic):
    str_to_write("Status [" + status[15] + "] with writeback [" + writeback[15] + "] is enabled")
else:
    str_to_write("Status [" + status[15] + "] with writeback [" + writeback[15] + "] is disabled")

if(mispredict):
    str_to_write("Status [" + status[16] + "] with writeback [" + writeback[16] + "] is enabled")
else:
    str_to_write("Status [" + status[16] + "] with writeback [" + writeback[16] + "] is disabled")

if(replay):
    str_to_write("Status [" + status[17] + "] with writeback [" + writeback[17] + "] is enabled")
else:
    str_to_write("Status [" + status[17] + "] with writeback [" + writeback[17] + "] is disabled")   

if(longf == 0x40000):
    str_to_write("Status [" + status[18] + "] with writeback [" + writeback[18] + "] is enabled")
elif(longf == 0x80000):
    str_to_write("Status [" + status[19] + "] with writeback [" + writeback[19] + "] is enabled")
elif(longf == 0x00000):
    str_to_write_all_dis("Write back status for [LONG_F_Busy||WR] are all disabled")
else:
    str_to_write_all_enab("Warning: Write back status for [LONG_F_Busy||WR] multiple status enabled")
#    f.write("Status [" + status[18] + "] with writeback [" +writeback[18]+ "] is disabled \nStatus [" + status[19] + "] with writeback ["+ writeback[19]+ "] is disabled \n")


if(load_store == 0x100000):
    str_to_write("Status [" + status[20] + "] with writeback [" + writeback[20] + "] is enabled")
elif(load_store == 0x200000):
    str_to_write("Status [" + status[21] + "] with writeback [" + writeback[21] + "] is enabled")
elif(load_store == 0x000000):
    str_to_write_all_dis("Write back status for [Load_Store] are all disabled")
else:
    str_to_write_all_enab("Warning: Write back status for [Load_Store] multiple status enabled")

if(l1 == 0x400000):
    str_to_write("Status [" + status[22] + "] with writeback [" + writeback[22] + "] is enabled")
elif(l1 == 0x800000):
    str_to_write("Status [" + status[23] + "] with writeback [" + writeback[23] + "] is enabled")
elif(l1 == 0x000000):
    str_to_write_all_dis("Write back status for [L1_Evict||Miss] are all disabled")
else:
    str_to_write_all_enab("Warning: Write back status for [L1_Evict||Miss] multiple status enabled")

if(tx == 0x1000000):
    str_to_write("Status [" + status[24] + "] with writeback [" + writeback[24] + "] is enabled")
elif(tx == 0x2000000):
    str_to_write("Status [" + status[25] + "] with writeback [" + writeback[25] + "] is enabled")
elif(tx == 0x0000000):
    str_to_write_all_dis("Write back status for [TX_Send||RCV] are all disabled")
else:
    str_to_write_all_enab("Warning: Write back status for [TX_Send||RCV] multiple status enabled")

if(vpu == 0x4000000):
    str_to_write("Status [" + status[26] + "] with writeback ["+ writeback[26] + "] is enabled")
elif(vpu == 0x8000000):
    str_to_write("Status [" + status[27] + "] with writeback [" + writeback[27] + "] is enabled")
elif(vpu == 0x10000000):
    str_to_write("Status [" + status[28] + "] with writeback [" + writeback[28] + "] is enabled")
elif(vpu == 0x20000000):
    str_to_write("Status [" + status[29] + "] with writeback [" + writeback[29] + "] is enabled")
elif(vpu == 0x40000000):
    str_to_write("Status [" + status[30] + "] with writeback [" + writeback[30] + "] is enabled")
elif(vpu == 0x00000000):
    str_to_write_all_dis("Write back status for [VPU] are all disabled")
else:
    str_to_write_all_enab("Warning: Write back status for [VPU] multiple status enabled")

if(l1_miss == 0x80000000):
    str_to_write("Status ["+ status[31] + "] with writeback [" + writeback[31] + "] is enabled")
elif(l1_miss == 0x100000000):
    str_to_write("Status [" + status[31] + "] with writeback [" + writeback[32] + "] is enabled")
elif(l1_miss == 0x200000000):
    str_to_write("Status [" + status[32] + "] with writeback [" + writeback[31] + "] is enabled")
elif(l1_miss == 0x400000000):
    str_to_write("Status [" + status[32] + "] with writeback [" + writeback[32] + "] is enabled")
elif(l1_miss == 0x800000000):
    str_to_write("Status [" + status[33] + "] with writeback [" + writeback[31] + "] is enabled")
elif(l1_miss == 0x1000000000):
    str_to_write("Status [" + status[34] + "] with writeback [" + writeback[32] + "] is enabled")
elif(l1_miss == 0x0000000000):
    str_to_write_all_dis("Write back status for [" + status[22] + "] are all disabled")
else:
    str_to_write_all_enab("Warning: Write back status for [" + status[22] + "] multiple status enabled")

if(gtsc):
    str_to_write("Status [" + status[35] + "] with writeback ["+ writeback[33] + "] is enabled")
else:
    str_to_write("Status [" + status[35] + "] with writeback [" + writeback[33] + "] is disabled")

if(fma_csr == 0x4000000000):
    str_to_write("Status [" + status[36] + "] with writeback [" + writeback[36] + "] is enabled")
elif(fma_csr == 0x8000000000):
    str_to_write("Status [" + status[36] + "] with writeback [" + writeback[37] + "] is enabled")
elif(fma_csr == 0xC000000000):
    str_to_write("Status [" + status[36] + "] with writeback [" + writeback[38] + "] is enabled")
else:
    str_to_write("Status [" + status[36] + "] with writeback [" + writeback[35] + "] is enabled")
#  str_to_write_all_dis("Write back status for [" + status[36] + "] are all disabled") 

    
#f.close()    
