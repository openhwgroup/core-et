#!/usr/bin/python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0


# This script generates randomized input parameters for tensor instructions.
# To invoke it and do something useful with it you need to use one of the existing scenarios
# or define your own scenario. 
# In general each generate_ function creates random input paremeters for an instruction for all the minions
# in all the shires specified in the shires input parameters
# Each scenario_ function calls one or more generate_functions a numebr of times to implement a randomized-directed test scenario
#
# scenario_* and generate_* functions share the following parameters:
#   - shires: Number of shires to unr the test one
#   - target_dir: Where to store the output file
#   - output_type: define parameters using a switch statement or an array
#   - num_iter: Iteration number for which we call this function
#   - total_iter: Total number of iterations

import time
import random
import sys
import argparse
import os

minions_per_shire = 32

def generate_tensorloads(shires, target_dir, output_type, num_iter, total_iter, tl_id, max_num_cooploads_in_shire, min_coop_id, use_tenb = False, min_tl_line=0, max_tl_line=15):
    
    """ 
    Generate input parameters for tensor loads running on all minions in "shires" shires.
    Inputs:
    - max_num_coop_loads_in_shire: Number of coop load groups the function will try to generate. If > 0 then [1..max_num_cooploads_in_shire]
      groups will be created
    - min_coop_id: Coop id from the previous iteration to avoid using the same coop id in different groups
    - use_tenb: Is this a TL to Tenb (1) or to SCP (0). Currently we do not support randomizing this parameter inside the function
    Outputs:
    - coop_id that can be used by the next tensor load
    - which minions use tenb to be passed to the consuming FMA
    - number of lines for the tenb to be passed to the consuming FMA
    """

    assert(tl_id == 0 or tl_id == 1)

    assert(not(total_iter > 1 and output_type == "switch"))

    # Overwrite possible existing file on the 1st iteration
    if num_iter == 0:
        tl_configs_hf = open(target_dir+"/tl"+str(tl_id)+"_configs.h","w")        
        if output_type == "switch":
            def_line = "switch(minion_id) {\n"
        elif output_type =="array":
            def_line = "static uint64_t tl"+str(tl_id)+"_configs["+str(total_iter*shires*minions_per_shire*10)+"] = {\n"
            def_line = def_line + "// COOP_CSR  IS_COOP  TMASK  TRANS  SCP_START  TENB  ADDR_OFFSET  LINE_OFFSET  LINES  STRIDE\n"
        tl_configs_hf.write(def_line)
    else:
        tl_configs_hf = open(target_dir+"/tl"+str(tl_id)+"_configs.h","a+")         
   
    full_tl_tenb_list = []
    full_tl_num_lines_list = []
    
    tl_code_notrans = 0
    tl_code_li8 = 1
    tl_code_li16 = 2
    tl_code_lt8 = 5
    tl_code_lt16 = 6
    tl_code_lt32 = 7
    tl_code_map = { 0: tl_code_notrans,
                    1: tl_code_li8,
                    2: tl_code_li16,
                    3: tl_code_lt8,
                    4: tl_code_lt16,
                    5: tl_code_lt32 }

    for sh in range(shires):
        
        tl_tmask_list = [random.randint(0,1) for i in range(minions_per_shire)]
        tl_coop_list = [0] * minions_per_shire
        
        tl_scp_start_line_list =  [random.randint(0,63) for i in range(minions_per_shire)]

        # TenB will be 0 or 1 for all minions, currently if the user wants to change that he/shge can do it within the test only
        if use_tenb:
            tl_tenb_list = [1] * minions_per_shire
        else:
            tl_tenb_list = [0] * minions_per_shire

        tl_code_list = [tl_code_map[random.randint(0,5)] for i in range(minions_per_shire)]
        for i in range(minions_per_shire):
            if tl_tenb_list[i] == 1:
                tl_code_list[i] = 0

        # The address is the offset from some start address specified in the test
        # The limit is specified by #shires, we give 1KB per minion because that is the size of the VPU RF, so 32KB (0x8000 per shire)
        # SW-1560: This is not an elegant way to specify addresses because we need to know in advance the address
        # range. Here we assume that thew input size is 2MB.
        # The 1st MB is used to for a base address for each shire (each shire points to a 32KB block).
        # The 2nd MB is to accommodate different strides.
        # Ideally we should specify an input size and let this function calculate the tl_addr_list
        # and the tl_stride_list
        tl_addr_list = [random.randint(0, 0x8000 * shires - 1) & 0xFFFFFFFFFFC0 for i in range(minions_per_shire)]
        
        tl_offset_list = [0] * minions_per_shire

        tl_num_lines_list = [random.randint(min_tl_line, max_tl_line) for i in range(minions_per_shire)]

        # Stride here is speficied in lines
        # SW-1560: Set max value to 224 instead of 1024
        tl_stride_list =  [(random.randint(1,224) << 6) for i in range(minions_per_shire)]

        tl_coop_csr_list = [0] * minions_per_shire

        num_cooploads_in_shire = 0
        
        tl_coop_programmed = [0] * minions_per_shire

        # Until this point we have randomized parameters assuming on coops.
        # Adding coops will force us to use the same address, stride, num lines, coop csr, mask csr
        # for all the minions in the group.

        for coop_ld in range(max_num_cooploads_in_shire):
            
            neigh_mask = random.randint(1,15)
            min_mask = random.randint(1,255)

            # Set a mask that shows which minions participate in the coop
            # Then check if minions in that mask are already participating in another coop.
            # If so this coop needs to be aborted
            bitfield_min_mask = [int(x) for x in list('{0:0>8b}'.format(min_mask))]
            bitfield_min_mask.reverse()
            bitfield_shire_mask = []
            for i in range(4):
                if (neigh_mask & (1 << i)):
                    bitfield_shire_mask = bitfield_shire_mask + bitfield_min_mask
                else:
                    bitfield_shire_mask = bitfield_shire_mask + ([0] * 8)
            
            coop_possible = True
            first_min = -1
            for m in range(minions_per_shire):
                if tl_coop_programmed[m] == 0 and bitfield_shire_mask[m] == 1 and first_min == -1:
                    first_min = m
                if tl_coop_programmed[m] == 1 and bitfield_shire_mask[m] == 1:
                    coop_possible = False

            if not coop_possible:
                continue
            else:
                num_cooploads_in_shire = num_cooploads_in_shire + 1
            
            # Now set all the parameters in the coops participating in the coop
            # to be equal the ones of the first minion in the coop
            tl_coop_programmed[first_min] = 1
            tl_coop_list[first_min] = 1
            tl_coop_csr_list[first_min] = tl_coop_csr_list[first_min] | (neigh_mask << 16) + (min_mask << 8) | ((min_coop_id[sh] + num_cooploads_in_shire - 1) % 16)
            for m in range(first_min+1,minions_per_shire):                
                if bitfield_shire_mask[m] == 1:
                    tl_coop_programmed[m] = 1
                    tl_coop_csr_list[m] = tl_coop_csr_list[first_min]
                    tl_tmask_list[m] = tl_tmask_list[first_min]
                    tl_coop_list[m] = 1
                    tl_code_list[m] = tl_code_list[first_min]
                    tl_scp_start_line_list[m] = tl_scp_start_line_list[first_min]
                    tl_tenb_list[m] = tl_tenb_list[first_min]
                    tl_addr_list[m] = tl_addr_list[first_min]
                    if tl_code_list[m] == tl_code_li8 or tl_code_list[m] == tl_code_lt8:
                        tl_offset_list[m] = random.randint(0,3)
                    elif tl_code_list[m] == tl_code_li16 or tl_code_list[m] == tl_code_lt16:
                         tl_offset_list[m] = random.randint(0,1) * 2
                    tl_num_lines_list[m] = tl_num_lines_list[first_min]
                    tl_stride_list[m] = tl_stride_list[first_min]                    


        # All parameters have been set for this iteration, write up the source files
        if (output_type == "switch"):
            for i in range(0,minions_per_shire):
                def_line = " case " + str(sh*minions_per_shire+i) + ":\n   TL" + str(tl_id) + "_COOP_CSR = " + str(hex(tl_coop_csr_list[i])) + ";\n"
                def_line = def_line + "   TL" + str(tl_id) + "_IS_COOP = " + str(tl_coop_list[i]) + ";\n"
                def_line = def_line + "   TL" + str(tl_id) + "_TMASK = " + str(tl_tmask_list[i]) + ";\n"
                def_line = def_line + "   TL" + str(tl_id) + "_CODE = " + str(tl_code_list[i]) + ";\n"
                def_line = def_line + "   TL" + str(tl_id) + "_SCP_START_LINE = " + str(tl_scp_start_line_list[i]) + ";\n"
                def_line = def_line + "   TL" + str(tl_id) + "_TENB = " + str(tl_tenb_list[i]) + ";\n"
                def_line = def_line + "   TL" + str(tl_id) + "_ADDR = " + str(hex(tl_addr_list[i])) + ";\n"
                def_line = def_line + "   TL" + str(tl_id) + "_OFFSET = " + str(hex(tl_offset_list[i])) + ";\n"
                def_line = def_line + "   TL" + str(tl_id) + "_NUM_LINES = " + str(tl_num_lines_list[i]) + ";\n";
                def_line = def_line + "   TL" + str(tl_id) + "_STRIDE = " + str(hex(tl_stride_list[i])) + ";\n";
                def_line = def_line + "   break;\n"
                tl_configs_hf.write(def_line)
        elif output_type == "array":
            for i in range(0,minions_per_shire):
                def_line = str(hex(tl_coop_csr_list[i])).rjust(10) + "," + str(tl_coop_list[i]).rjust(6) + "," + str(tl_tmask_list[i]).rjust(7) + "," + \
                           str(tl_code_list[i]).rjust(6) + "," + str(tl_scp_start_line_list[i]).rjust(9) + "," + str(tl_tenb_list[i]).rjust(7) + "," + \
                           str(hex(tl_addr_list[i])).rjust(11) + "," + str(hex(tl_offset_list[i])).rjust(10) + "," + str(tl_num_lines_list[i]).rjust(8) + "," + \
                           str(hex(tl_stride_list[i])).rjust(9) + ",  //  Min" + str(sh*minions_per_shire + i) + "-Iter"+str(num_iter)+"\n"
                tl_configs_hf.write(def_line)

        full_tl_tenb_list = full_tl_tenb_list + tl_tenb_list
        full_tl_num_lines_list = full_tl_num_lines_list + tl_num_lines_list
        
        min_coop_id[sh] = (min_coop_id[sh] + num_cooploads_in_shire) % 16

    if num_iter == total_iter-1:
            tl_configs_hf.write("};\n")
    
    tl_configs_hf.close()

    return (min_coop_id, full_tl_tenb_list, full_tl_num_lines_list)



def generate_tfmas(shires, target_dir, output_type, num_iter, total_iter, tfma_tenb_list, tfma_acols_list):

    """
    Generate input parameters for tensor loads running on all minions in "shires" shires.
    Inputs:
    - tfma_tenb_list: List of minions that do tenb from previously generated tensor load
    - tfma_acols_list: List of number of a cols (b rows) generated by the previous tensor laod for TenB.    
    """ 

    num_minions = shires * minions_per_shire

    tfma_tmask_list = [random.randint(0,1) for i in range(num_minions)]
    tfma_bcols_list = [random.randint(0,3) for i in range(num_minions)]
    tfma_arows_list = [random.randint(0,15) for i in range(num_minions)]
    tfma_astart_col_list = [random.randint(0,15) for i in range(num_minions)]
    
    # FMA types (FMA32, IMA8A32, FMA16A32)
    tfma_type_fma16a32 = 1
    tfma_type_ima8a32 = 3
    tfma_type_fma32 = 0

    tfma_types_map = {0: tfma_type_fma32,
                      1: tfma_type_fma16a32,
                      2: tfma_type_ima8a32 }

    tfma_type_list = [tfma_types_map[random.randint(0,2)] for i in range(num_minions)]

    tfma_scp_start_linea_list = [random.randint(0,63) for i in range(num_minions)]
    tfma_scp_start_lineb_list = [random.randint(0,63) for i in range(num_minions)]
    
    tfma_clear_rf_list = [random.randint(0,1) for i in range(num_minions)]

    if num_iter == 0:
        tfma_configs_hf = open(target_dir+"/tfma_configs.h", "w")
        if output_type == "switch":
            def_line = "switch (minion_id) {\n"
        else:
            def_line = "static uint64_t tfma_configs["+str(total_iter*num_minions*13)+"] = {\n"
            def_line = def_line + "// TMASK  BCOLS  AROWS  ACOLS  ASTART_COL  TENB  SCP_START_LINEA  SCP_START_LINEB  CLEAR_RF  TYPE  USE_TENC  UNS_A  UNS_B\n"
        tfma_configs_hf.write(def_line)                          
    else:
        tfma_configs_hf = open(target_dir+"/tfma_configs.h", "a+")
    
    if (output_type == "switch"):
        for i in range(num_minions):
        
            def_line = " case " + str(i) + ":\n   TFMA_TMASK = " + str(tfma_tmask_list[i]) + ";\n"
            def_line = def_line + "   TFMA_BCOLS = " + str(tfma_bcols_list[i]) + ";\n"
            def_line = def_line + "   TFMA_AROWS = " + str(tfma_arows_list[i]) + ";\n"
            if tfma_tenb_list[i] == 0:
                tfma_acols_list[i] = random.randint(0,15)
            def_line = def_line + "   TFMA_ACOLS = " + str(tfma_acols_list[i]) + ";\n"
            def_line = def_line + "   TFMA_ASTART_COL = " + str(tfma_astart_col_list[i]) + ";\n"
            def_line = def_line + "   TFMA_TENB = " + str(tfma_tenb_list[i]) + ";\n"
            def_line = def_line + "   TFMA_SCP_START_LINEA = " + str(tfma_scp_start_linea_list[i]) + ";\n"
            def_line = def_line + "   TFMA_SCP_START_LINEB = " + str(tfma_scp_start_lineb_list[i]) + ";\n"
            def_line = def_line + "   TFMA_CLEAR_RF = " + str(tfma_clear_rf_list[i]) + ";\n"
            def_line = def_line + "   TFMA_TYPE = " + str(tfma_type_list[i]) + ";\n"
            tfma_use_tenc = 0
            tfma_unsigneda = 0
            tfma_unsignedb = 0
            if (tfma_type_list[i] == tfma_type_ima8a32):
                tfma_use_tenc = random.randint(0,1)
                tfma_unsigneda = random.randint(0,1)
                tfma_unsignedb = random.randint(0,1)
            def_line = def_line + "   TFMA_USE_TENC = " + str(tfma_use_tenc) + ";\n"
            def_line = def_line + "   TFMA_UNSIGNEDA = " + str(tfma_unsigneda) + ";\n"
            def_line = def_line + "   TFMA_UNSIGNEDB = " + str(tfma_unsignedb) + ";\n"
            def_line = def_line + "   break;\n"
            tfma_configs_hf.write(def_line)
                
        tfma_configs_hf.write("}\n")
    
    elif output_type == "array":
        for i in range(num_minions):
            if tfma_tenb_list[i] == 0:
                tfma_acols_list[i] = random.randint(0,15)
            tfma_use_tenc = 0
            tfma_unsigneda = 0
            tfma_unsignedb = 0
            if (tfma_type_list[i] == tfma_type_ima8a32):
                tfma_use_tenc = random.randint(0,1)
                tfma_unsigneda = random.randint(0,1)
                tfma_unsignedb = random.randint(0,1)
            def_line = str(tfma_tmask_list[i]).rjust(6) + "," + str(tfma_bcols_list[i]).rjust(6) + "," + str(tfma_arows_list[i]).rjust(6) + "," + str(tfma_acols_list[i]).rjust(6) + "," + \
                       str(tfma_astart_col_list[i]).rjust(9) + "," + str(tfma_tenb_list[i]).rjust(7) + "," +  str(tfma_scp_start_linea_list[i]).rjust(12) + "," + \
                       str(tfma_scp_start_lineb_list[i]).rjust(15) + "," + str(tfma_clear_rf_list[i]).rjust(12) + "," + str(tfma_type_list[i]).rjust(7) + "," + \
                       str(tfma_use_tenc).rjust(7) + "," + str(tfma_unsigneda).rjust(8) + "," + str(tfma_unsignedb).rjust(5) + ",   // Min" + str(i) + "-Iter"+str(num_iter)+"\n"
            tfma_configs_hf.write(def_line)

        if num_iter == total_iter - 1:
            tfma_configs_hf.write("};")            
            


def generate_reduces(shires, target_dir, output_type, num_iter, total_iter, new_list = "new", previous_reduce_list = []):

    """
    Generate random pairs of reduces across "shires" shires
    Inputs:
    - new_list: If "new" then a new set of pairs is created, if "same" then we use previous_reduce_list, if "inverse" then it inverts senders and receivers in the list
    - previous_reduce_list: Same above
    Outputs:
    - reduce_order: Randomized list of pairs (each even element is sender and the next odd is the receiver). To be optionally passed to the next reduce
    Currently only send/receive pairs are generated. One reason is that to stress test the NOC only the send/receive modes can be used,
    as only 1 minion per shire (minion 0) can participate in cross-shire auto-reduce and broadcast traffic. The other reason is that no tests have been developed for that yet

    """

    assert(not(total_iter > 1 and output_type == "switch"))

    num_minions = shires * minions_per_shire
    
    reduce_op_fadd = 0
    reduce_op_fmax = 2
    reduce_op_fmin = 3
    reduce_op_iadd = 4
    reduce_op_imax = 6
    reduce_op_imin = 7
    reduce_op_fget = 8
    
    reduce_op_map = { 0: reduce_op_fadd,
                      1: reduce_op_fmax,
                      2: reduce_op_fmin,
                      3: reduce_op_iadd,
                      4: reduce_op_imax,
                      5: reduce_op_imin,
                      6: reduce_op_fget}

    reduce_send = 0
    reduce_receive = 1

    if new_list == "new":
        minion_list  = list(range(0,num_minions))
        reduce_order = random.sample(minion_list, num_minions)
    elif new_list == "same":
        reduce_order = previous_reduce_list
    elif new_list == "inverse":
        reduce_order = previous_reduce_list
        for i in range(0,len(reduce_order),2):
            reduce_order[i], reduce_order[i+1] = reduce_order[i+1], reduce_order[i]

    reduce_start_reg =  [random.randint(0,31) for i in range(num_minions)]
    reduce_op = [reduce_op_map[random.randint(0,6)] for i in range(num_minions)]
    reduce_config = [0] * num_minions
    reduce_num_regs = [0] * num_minions
    reduce_partner = [0] * num_minions
        
    for m in reduce_order:
        reduce_idx = reduce_order.index(m)
        # Even number is sender and odd is receiver
        if (reduce_idx % 2) == 0:
            reduce_partner[m] = reduce_order[reduce_idx+1]    
            reduce_num_regs[m] = random.randint(0,127)
            reduce_config[m] = reduce_send
            
        else:
            reduce_partner[m] = reduce_order[reduce_idx-1]        
            reduce_num_regs[m] = reduce_num_regs[reduce_partner[m]]
            reduce_config[m] = reduce_receive            

    # Pairs are ready, now print out the code
    if num_iter == 0:
        reduce_configs_hf = open(target_dir+"/reduce_configs.h", "w")
        if output_type == "switch":
            def_line = "switch (minion_id) {\n"
        else:
            def_line = "static uint64_t reduce_configs["+str(total_iter*num_minions*13)+"] = {\n"
            def_line = def_line + "// START_REG  OP  NUM_REGS  PARTNER/DEPTH  ACTION\n"
        reduce_configs_hf.write(def_line)                          
    else:
        reduce_configs_hf = open(target_dir+"/reduce_configs.h", "a+")

    if output_type == "switch":
        for i in range(0,num_minions):
            def_line = " case " + str(i) + ":\n";
            def_line = def_line + "   REDUCE_ACTION = " + str(reduce_config[i]) + ";\n"
            def_line = def_line + "   REDUCE_PARTNER = " + str(reduce_partner[i]) + ";\n"
            def_line = def_line + "   REDUCE_START_REG = " + str(reduce_start_reg[i]) + ";\n"
            def_line = def_line + "   REDUCE_OP = " + str(reduce_op[i]) + ";\n"
            def_line = def_line + "   REDUCE_NUM_REGS = " + str(reduce_num_regs[i]) + ";\n"
            tfma_configs_hf.write(def_line);

    elif output_type == "array":
        for i in range(0,num_minions):
            def_line = str(reduce_start_reg[i]).rjust(8) + "," + str(reduce_op[i]).rjust(6) + "," + str(reduce_num_regs[i]).rjust(8) + "," + \
                       str(reduce_partner[i]).rjust(10) + "," + str(reduce_config[i]).rjust(10) + "," + \
                       "  // Min" + str(i) + "-Iter"+ str(num_iter)+ "\n"
            reduce_configs_hf.write(def_line);

    if num_iter == total_iter-1:
        reduce_configs_hf.write("};");

    return reduce_order


def generate_tensorstores(shires, target_dir, output_type, num_iter, total_iter, max_num_coops_in_shire, addr_type="split", buf_size="1K"):
    """
    Generate tensor stores. Split addr type means each minion will store in a separate buffer
    Random it means that addresses are random. Buf size is the buffer size per minion.
    """

    no_coop = 0
    pair_128 = 1
    pair_256 = 2
    quad_128 = 3
    
    quarter_line = 0
    half_line = 1
    full_line = 3
    row_size_map = {0:quarter_line,
                    1:half_line,
                    2:full_line};
    use_rf = 0
    use_scp = 1

    shire_list = list(range(0,shires))
    # If you use shire_addr_lsit instead of shire list to generate the tstore addesses
    # you need to synchronize across shirs because on one iteration ons shire will overwrite the data
    # of another shire and there will be races
    # shire_addr_list = random.sample(shire_list, shires)

    if num_iter == 0:
        tstore_configs_hf = open(target_dir+"/tstore_configs.h", "w")
        if output_type == "switch":
            def_line = "switch (minion_id) {\n"
        else:
            def_line = "static uint64_t tstore_configs["+str(total_iter*shires*minions_per_shire*8)+"] = {\n"
            def_line = def_line + "// USE_SCP  COOP_MASK  START_REG_LINE  REG_LINE_STRIDE  NUM_ROWS  ROW_SIZE   ADDR   STRIDE  \n"
        tstore_configs_hf.write(def_line)
    else:
        tstore_configs_hf = open(target_dir+"/tstore_configs.h", "a+")

    for sh in range(shires):
            
        coop_list = [no_coop] * minions_per_shire
        use_scp_list = [random.randint(0,1) for i in range(minions_per_shire)]
        row_size_list = [row_size_map[random.randint(0,2)] for i in range(minions_per_shire)]
        num_rows_list = [random.randint(0,15) for i in range(minions_per_shire)]
        start_reg_list = [random.randint(0,31) for i in range(minions_per_shire)]
        start_line_list = [random.randint(0,47) for i in range(minions_per_shire)]
        reg_line_stride_list = [random.randint(0,3) for i in range(minions_per_shire)]
        # Each minion in the shire can access any 1KB block within a
        minion_list = list(range(0,minions_per_shire))
        minion_addr_list = random.sample(minion_list, minions_per_shire)
        addr_list = [0] * minions_per_shire        
        for m in range(minions_per_shire):
            addr_list[m] = 0x8000 * shire_list[sh] + 0x400 * minion_addr_list[m]
        # Still need to find a way to randomize strides w/o doing conflict computation
        stride_list = [ 0x40 ] * minions_per_shire 

        num_coops_in_shire = 0
        # First see if you are going to use coop        
        for i in range(minions_per_shire):
            if num_coops_in_shire == max_num_coops_in_shire:
                break
            if coop_list[i] != no_coop:
                continue
            coop_val = random.randint(0,3)
            if coop_val == no_coop:
                continue
            elif coop_val == pair_128 and (i % 2) == 0:
                coop_list[i:i+1] = [pair_128] * 2
                use_scp_list[i:i+1] = [use_rf] * 2
                row_size_list[i:i+1] = [quarter_line] * 2
                num_rows_list[i+1] = num_rows_list[i]
                addr_list[i+1] = addr_list[i] + 0x10
                num_coops_in_shire = num_coops_in_shire + 1
            elif coop_val == pair_256 and (i % 2) == 0:
                coop_list[i:i+1] = [pair_128] * 2
                use_scp_list[i:i+1] = [use_rf] * 2
                row_size_list[i:i+1] = [half_line] * 2
                num_rows_list[i+1] = num_rows_list[i]
                addr_list[i+1] = addr_list[i] + 0x20
                num_coops_in_shire = num_coops_in_shire + 1
            elif coop_val == quad_128 and i % 4 == 0:
                coop_list[i:i+3] = [quad_128] * 4
                use_scp_list[i:i+3] = [use_rf] * 4
                row_size_list[i:i+3] = [quarter_line] * 4
                num_rows_list[i+1:i+3] = [num_rows_list[i]] * 4
                addr_list[i+1] = addr_list[i] + 0x10
                addr_list[i+2] = addr_list[i] + 0x20
                addr_list[i+3] = addr_list[i] + 0x30
                num_coops_in_shire = num_coops_in_shire + 1
        
        for m in range(minions_per_shire):
            if output_type == "switch":
                def_line = " case " + str(m) + ":\n";
                def_line = def_line + "   USE_SCP = " + use_scp_list[m] + ";\n"
                def_line = def_line + "   COOP_MASK = " + coop_list[m] + ";\n"
                if use_scp_list[m] == use_rf:
                    def_line = def_line + "   START_REG = " + start_reg_list[m] + ";\n"
                else:
                    def_line = def_line + "   START_LINE = " + start_line_list[m] + ";\n"
                def_line = def_line + "   REG_LINE_STRIDE = " + reg_line_stride_list[m] + ";\n"
                def_line = def_line + "   NUM_ROWS = " + num_rows_list[m] + ";\n"
                def_line = def_line + "   ROW_SIZE = " + num_rows_list[m] + ";\n"
                def_line = def_line + "   ADDR = " + _rows_list[m] + ";\n"
                def_line = def_line + "   STRIDE = " + coop_list[m] + ";\n"
                tstore_configs.write(def_line)
            
            elif output_type == "array":
                def_line = str(use_scp_list[m]).rjust(8) + "," + str(coop_list[m]).rjust(8) + ","
                if use_scp_list[m] == use_rf:
                    def_line = def_line + str(start_reg_list[m]).rjust(13) + ","
                else:
                    def_line = def_line + str(start_line_list[m]).rjust(13) + ","
                def_line = def_line + str(reg_line_stride_list[m]).rjust(15) + "," + str(num_rows_list[m]).rjust(12) + "," + \
                           str(row_size_list[m]).rjust(9) + "," + str(hex(addr_list[m])).rjust(11) + "," + str(hex(stride_list[m])).rjust(7) + ","\
                           "  // Min" + str(m + sh * minions_per_shire) + "-Iter"+ str(num_iter)+ "\n"
                tstore_configs_hf.write(def_line)
        
    if num_iter == total_iter-1:
        tstore_configs_hf.write("};")
            
    

def generate_tload_tstore_pairs(shires, target_dir, output_type, num_iter, total_iter):
    """
    Generate parameters for producer-consumer scenario
    Minions write to a shared buffer. Writesrs use tensor stores, readers use tensor loads.
    Readers and Writers are rnadomly paired. The index variable show the buffer locations where data 
    is read / written and the pair variable for each writer shows its respective reader
    """
    
    num_minions = shires * minions_per_shire
    minion_list  = list(range(0,num_minions))

    tstore_list = random.sample(minion_list,num_minions)
    tload_list = random.sample(minion_list,num_minions)

    if num_iter == 0:
        mappings_hf = open(target_dir+"/tload_tstore_mappings.h","w")
        if output_type == "switch":
            def_line = "switch(minion_id) {\n"
        elif output_type == "array":
            def_line = "static uint64_t tload_tstore_mappings[" + str(num_minions * total_iter * 3) + "] = {\n"
            def_line = def_line + "// STORE_IDX   LOAD_IDX   PAIR  \n"
        mappings_hf.write(def_line)
    else:
        mappings_hf = open(target_dir+"/tload_tstore_mappings.h","a+")
        
    for i in range(num_minions):
        pair = -1
        for j in range(0,num_minions):
            if tload_list[j] == tstore_list[i]:
                pair = j
                
                if output_type == "switch":
                    def_line = " case " + str(i) + ":\n";
                    def_line = def_line + "   INDEX_STORE = " + str(tstore_list[i]) + ";\n"
                    def_line = def_line + "   INDEX_LOAD = " + str(tload_list[i]) + ";\n"
                    def_line = def_line + "   PAIR = " + str(j) + ";\n   break;\n"
                else:
                    def_line =  str(tstore_list[i]).rjust(8) + "," + str(tload_list[i]).rjust(10) + ", " + str(pair).rjust(9) +  "," \
                                "  // Min" + str(i) + "-Iter"+ str(num_iter)+ "\n"
                
                mappings_hf.write(def_line)
                break
            
    if num_iter == total_iter-1:
        mappings_hf.write("};\n")


def scenario_tload_tfma(shires, repeat, target_dir, output_type):

    """
    Generate parameters for "repeat" iterations of 2 tensor loads + FMA.
    Tensor loads are not coops. TL0 goes to SCP, TL1 goes to TenB.
    """
    max_coop_id = [0] * shires
    for it in range(repeat):
        max_coop_id, _, _ = generate_tensorloads(shires, target_dir, output_type, it, repeat, 0, 0, max_coop_id, False, 11, 15)
        max_coop_id, tenb_list, tfma_acols_list = generate_tensorloads(shires, target_dir, output_type, it, repeat, 1, 0, max_coop_id, True, 11, 15)
                
        generate_tfmas(shires, target_dir, output_type, it, repeat, tenb_list, tfma_acols_list)


def scenario_coop_tload_tfma(shires, repeat, target_dir, output_type):
    
    """
    Generate parameters for "repeat" iterations of 2 tensor loads + FMA.
    Tensor loads may be coops (max 2 groups per shire). TL0 goes to SCP, TL1 goes to TenB.
    """

    max_coop_id = [0] * shires
    for it in range(repeat):
        max_coop_id, _, _ = generate_tensorloads(shires, target_dir, output_type, it, repeat, 0, 2, max_coop_id, False, 11, 15)
        max_coop_id, tenb_list, tfma_acols_list = generate_tensorloads(shires, target_dir, output_type, it, repeat, 1, 2, max_coop_id, True, 11, 15)
                
        generate_tfmas(shires, target_dir, output_type, it, repeat, tenb_list, tfma_acols_list)


def scenario_tload_tfma_reduce(shires, repeat, target_dir, output_type):

    """
    Generate parameters for "repeat" iterations of 2 tensor loads + FMA + Reduce
    Tensor loads are not coops. TL0 goes to SCP, TL1 goes to TenB.
    There are 2 reduces per loop, and in each one the pair ordering is reversed.
    """

    max_coop_id = [0] * shires
    reduce_order = []
    for it in range(repeat):
        max_coop_id, _, _ = generate_tensorloads(shires, target_dir, output_type, it, repeat, 0, 0, max_coop_id, False, 1, 15)
        max_coop_id, tenb_list, tfma_acols_list = generate_tensorloads(shires, target_dir, output_type, it, repeat, 1, 0, max_coop_id, True, 1, 15)
                
        generate_tfmas(shires, target_dir, output_type, it, repeat, tenb_list, tfma_acols_list)
        
        if it == 0:
            reduce_order = generate_reduces(shires, target_dir, output_type, it, repeat, "new")

        else:
            reduce_order = generate_reduces(shires, target_dir, output_type, it, repeat, "inverse", reduce_order)


def scenario_producer_consumer(shires, repeat, target_dir, output_type):

    max_coop_id = [0] * shires
    reduce_order = []
    tenb_list = [0] * shires * minions_per_shire
    tfma_acols_list = [0] * shires * minions_per_shire

    # Before the 1st FMA test should make sure that the SCP has valid data
    for it in range(repeat):
                    
        generate_tfmas(shires, target_dir, output_type, it, repeat, tenb_list, tfma_acols_list)
        
        if it == 0:
            reduce_order = generate_reduces(shires, target_dir, output_type, it, repeat, "new")

        else:
            reduce_order = generate_reduces(shires, target_dir, output_type, it, repeat, "same", reduce_order)
        
        generate_tload_tstore_pairs(shires, target_dir, output_type, it, repeat)


def scenario_tload_tfma_tstore(shires, repeat, target_dir, output_type):
    
    max_tload_coop_id = [2] * shires
    
    for it in range(repeat):
        max_tload_coop_id, _, _ = generate_tensorloads(shires, target_dir, output_type, it, repeat, 0, 2, max_tload_coop_id, False, 1, 15)
        max_tload_coop_id, tenb_list, tfma_acols_list = generate_tensorloads(shires, target_dir, output_type, it, repeat, 1, 2, max_tload_coop_id, True, 1, 15)
        
        generate_tfmas(shires, target_dir, output_type, it, repeat, tenb_list, tfma_acols_list)
        
        generate_tensorstores(shires, target_dir, output_type, it, repeat, 4)


parser = argparse.ArgumentParser()
parser.add_argument("--seed", type=int, help="Use seed for random generation if provided, otherwise create your own seed", default=-1)
parser.add_argument("--shires", type=int, help="Now many shires", default = 32)
parser.add_argument("--scenario", type=str, help="What scenario are you trying to generate ?", default = "tload_tstore")
parser.add_argument("--repeat", type=int, help="How many tiems do you want to repeat this scenario ?", default = 1)
parser.add_argument("--target_dir", type=str, help="Target directory where output files are store", default = ".")
parser.add_argument("--output_type", type=str, help="What will be the output type to be fed into test (array / switch)", default = "switch")

args = parser.parse_args()

if args.seed == -1:
    initial_seed = int(time.time() * 100000)
    random.seed(initial_seed)
    seed_file = open("tensor_rand_seed","w")
    seed_file.write(str(initial_seed))
else:
    random.seed(args.seed)
    
if args.scenario == "tload_tfma":
    scenario_tload_tfma(args.shires, args.repeat, args.target_dir, args.output_type)
elif args.scenario == "coop_tload_tfma":
    scenario_coop_tload_tfma(args.shires, args.repeat, args.target_dir, args.output_type)
elif args.scenario == "tload_tfma_reduce":
    scenario_tload_tfma_reduce(args.shires, args.repeat, args.target_dir, args.output_type)
elif args.scenario == "producer_consumer":
    scenario_producer_consumer(args.shires, args.repeat, args.target_dir, args.output_type)
elif args.scenario == "tload_tfma_tstore":
    scenario_tload_tfma_tstore(args.shires, args.repeat, args.target_dir, args.output_type)
else:
    sys.exit("Unknown scenario, valid scenarios are: tload_tfma, coop_tload_tfma, tload_tfma_reduce, tload_tfma_tstore, producer_consumer")

