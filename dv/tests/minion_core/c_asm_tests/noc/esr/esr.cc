/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "esr.h"           // :/dv/tests/minion_core/c_asm_tests/include 
#include <stdio.h>
#include <inttypes.h>

#include "minion.h"
#include "et_test_common.h"



/*
    From PRM-19: Memory Map, PMA and PMP:
    2.4 ESR Region (4G-8G):
    The encoding of the PP bits is:
    00 for user mode, 
    01 for supervisor mode,
    11 for machine mode,
    10 is reserved for microarchitectural messages (communications from and to the Tbox, reduce messages, atomic responses, etc.) and shall always be trapped by agents as an "access fault", except the Service Processor when in M-mode (?)
*/   


/*
// PP = address[31:30]
typedef enum
{
    PP_USER       = 0,
    PP_SUPERVISOR = 1,
    PP_MESSAGES   = 2,  // reserved for u-Arch messages, shall always be trapped as "access fault", except for SP in M-mode
    PP_MACHINE    = 3
} esr_protection_t;

// Region = address[21:20]
typedef enum
{
    REGION_MINION        = 0,    // HART ESR
    REGION_NEIGHBOURHOOD = 1,    // Neighbor ESR
    REGION_TBOX          = 2,    // 
    REGION_OTHER         = 3     // Shire Cache ESR and Shire Other ESR
} esr_region_t;
*/




int main()
{
/*
    // suggested hack by Terry: 
    top.soc_top.SHIRE[0].shire_top.shire_top.shire_channel_gen.channel_inst.g_sc.shire_cache.banks[0].bank.pipe.dv_reset_esr_ctl_captured = 1
    top.soc_top.SHIRE[0].shire_top.shire_top.shire_channel_gen.channel_inst.g_sc.shire_cache.banks[1].bank.pipe.dv_reset_esr_ctl_captured = 1
    top.soc_top.SHIRE[0].shire_top.shire_top.shire_channel_gen.channel_inst.g_sc.shire_cache.banks[2].bank.pipe.dv_reset_esr_ctl_captured = 1
    top.soc_top.SHIRE[0].shire_top.shire_top.shire_channel_gen.channel_inst.g_sc.shire_cache.banks[3].bank.pipe.dv_reset_esr_ctl_captured = 1
    delay(5);  // wait a few clocks... 
    top.soc_top.SHIRE[0].shire_top.shire_top.shire_channel_gen.channel_inst.g_sc.shire_cache.banks[0].bank.pipe.dv_reset_esr_ctl_captured = 0
    top.soc_top.SHIRE[0].shire_top.shire_top.shire_channel_gen.channel_inst.g_sc.shire_cache.banks[1].bank.pipe.dv_reset_esr_ctl_captured = 0
    top.soc_top.SHIRE[0].shire_top.shire_top.shire_channel_gen.channel_inst.g_sc.shire_cache.banks[2].bank.pipe.dv_reset_esr_ctl_captured = 0
    top.soc_top.SHIRE[0].shire_top.shire_top.shire_channel_gen.channel_inst.g_sc.shire_cache.banks[3].bank.pipe.dv_reset_esr_ctl_captured = 0
    delay(10);
*/

    C_TEST_START;

/*
    __asm__ __volatile__ (
            "csrr a5, mhartid\n"
            "srli a5, a5, 6\n"
            "slli a5, a5, 22\n"
            "li a3, 0x80340000\n"
            "add a2, a5, a3\n"
            "sd zero, 0(a2)\n"
            "fence\n"
            ::
            : "a4", "a5", "a2", "a3"
    );
*/

 
    // change to use macros functions read_esr, write_esr, etc (from esr.h) 
    // see Minion ESR Registers.xlsx 
    // inline uint64_t __attribute__((always_inline)) read_esr(esr_protection_t pp, uint8_t shire_id, esr_region_t region, uint32_t address) 
    // inline void __attribute__((always_inline)) write_esr(esr_protection_t pp, uint8_t shire_id, esr_region_t region, uint32_t address, uint64_t value) 

    uint64_t  shire_id = get_shire_id();     // [29:22] 
    uint64_t  neigh_id = get_neigh_id();     // [19:16]
  //uint64_t  minion_id = get_minion_id();   // [19:13]
    uint32_t  esr_addr = 0x000;              // valid upto [16:0]

    uint64_t  esr_rd_data;
    //note: if return esr_rd_data, it has to be "used" in later statement, otherwise -> error ...???



    // 1. read_esr Neigh_ESR ([21:20]=2'b01): 
    
    // read Neigh_ESR: minion_boot:
    // et_soc1/rtl/inc/esr_defines.vh:`define ESR_NEIGH_MINION_BOOT_RESET_VAL `VA_SIZE'h8000001000
    //uint32_t  minion_boot_addr = 0x003; 
    esr_addr = 0x003;
    //read_esr_new(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0); 
    esr_rd_data = read_esr_new(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0);
    //printf("esr_rd_data=0x%" PRIx64 "\n", esr_rd_data);     // not working
    //LOG(DEBUG, "esr_rd_data=0x%" PRIx64 "\n", esr_rd_data);     // not working

    // write same value back to this ESR:
    write_esr_new(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, esr_rd_data, 0x0);


    // read Neigh_ESR: mprot
    esr_addr = 0x004; 
    //read_esr_new(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0); 
    esr_rd_data = read_esr_new(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0);

    // write same value back to this ESR:
    write_esr_new(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, esr_rd_data, 0x0);


    // read Neigh_ESR vmspagesize
    esr_addr = 0x007; 
    //read_esr_new(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0);
    esr_rd_data = read_esr_new(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0);

    // write same value back to this ESR:
    write_esr_new(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, esr_rd_data, 0x0);
 

    // read Neigh_ESR: ipi_redirect_pc: user_mode:
    esr_addr = 0x008; 
    //read_esr_new(PP_USER, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0); 
    esr_rd_data = read_esr_new(PP_USER, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0); 

    // write same value back to this ESR:
    write_esr_new(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, esr_rd_data, 0x0);



    // ??? skip Neigh_ESR with PP=0x2 (uArch Messages) ???
    // PP_MESSAGES = 2: reserved for microarchitectural messages, shall always be trapped as "access fault", except for SP in M-mode
/*
    // read Neigh_ESR: hactrl: PP=0x2 
    esr_addr = 0x1ff0; 
    read_esr_new(PP_MESSAGES, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0);    // pass?

    // read Neigh_ESR: hastatus0: PP=0x2 
    esr_addr = 0x1ff1; 
    read_esr_new(PP_MESSAGES, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0);    // pass?

    // read Neigh_ESR: hastatus1: PP=0x2 
    esr_addr = 0x1ff2; 
    read_esr_new(PP_MESSAGES, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0);    // pass?

    // read Neigh_ESR: and_or_treeL0: PP=0x2 
    esr_addr = 0x1ff3; 
    read_esr_new(PP_MESSAGES, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0);    // pass?
*/


    // read Neigh_ESR: pmu_ctrl: 
    esr_addr = 0x00d;  
    read_esr_new(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0); 

    // read Neigh_ESR: neigh_chicken: 
    esr_addr = 0x00e; 
    read_esr_new(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0); 

    // read_esr Neigh_ESR icache_err_log_ctl:
    esr_addr = 0x00f; 
    read_esr_new(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0); 
    // et_soc1/rtl/inc/esr_defines.vh:`define ESR_ICACHE_ERR_LOG_CTL_RESET_VAL '{err_interrupt_enable: 3'b110, default: '0}

    // read Neigh_ESR: icache_err_log_info: 
    esr_addr = 0x010; 
    read_esr_new(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0); 

    // read Neigh_ESR: icache_err_log_addr: 
    esr_addr = 0x011; 
    read_esr_new(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0); 

    // read Neigh_ESR: icache_sbe_dbe_counts: 
    esr_addr = 0x012; 
    read_esr_new(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0); 

    // read Neigh_ESR: texture_control: user_mode 
    esr_addr = 0x1000; 
    read_esr_new(PP_USER, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0); 

    // read Neigh_ESR: texture_status: user_mode 
    esr_addr = 0x1001; 
    read_esr_new(PP_USER, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0); 

    // read Neigh_ESR: texture_image_table_ptr: user_mode 
    esr_addr = 0x1002; 
    read_esr_new(PP_USER, shire_id, REGION_NEIGHBOURHOOD, neigh_id, esr_addr, 0x0); 



    // 2. Shire Cache ESR:  assume REGION_OTHER([21:20]=2'b11)
    uint8_t  shr_cach_sub_region = 0x00;    // Shire_Cache ESR ([19:17]=3'b000)
    //uint8_t  shr_cach_bnk = 0x0;

    // read Shire_Cache_ESR: sc_l3_shire_swizzle_ctl
    esr_addr = 0x000; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);    // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);    // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);    // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);    // sc_bnk=3 

   /*** // Note: writing a different value would trigger a SVA assertion (about Shire-Cache) to fire!!!
    // write some new value to this Shire_Cache_ESR:
    write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0xdeafbabedeaf0001, 0x00);    // sc_bnk=0 
    // then read it back:
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);    // sc_bnk=0 
   ***/

    // read Shire_Cache_ESR: sc_reqq_ctl
    esr_addr = 0x001; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   // sc_bnk=0
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);   // sc_bnk=1
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);   // sc_bnk=2
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);   // sc_bnk=3

    // read Shire_Cache_ESR: sc_pipe_ctl
    esr_addr = 0x002; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);   // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);   // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);   // sc_bnk=3 

    // read Shire_Cache_ESR: sc_l2_cache_ctl
    esr_addr = 0x003; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);   // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);   // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);   // sc_bnk=3 

    // read Shire_Cache_ESR: sc_l3_cache_ctl
    esr_addr = 0x004; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);   // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);   // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);   // sc_bnk=3 

    // read Shire_Cache_ESR: sc_scp_cache_ctl
    esr_addr = 0x005; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 

    // read Shire_Cache_ESR: sc_idx_cop_sm_ctl
    esr_addr = 0x006; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 

    // read Shire_Cache_ESR: sc_idx_cop_sm_phy_idx
    esr_addr = 0x007; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 

    // read Shire_Cache_ESR: sc_idx_cop_sm_data0
    esr_addr = 0x008; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 

    // read Shire_Cache_ESR: sc_idx_cop_sm_data1
    esr_addr = 0x009; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 

    // read Shire_Cache_ESR: sc_idx_cop_sm_ecc
    esr_addr = 0x00a; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 

    // read Shire_Cache_ESR: sc_err_log_ctl
    esr_addr = 0x00b; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 

    // read Shire_Cache_ESR: sc_err_log_info
    esr_addr = 0x00c; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 

    // read Shire_Cache_ESR: sc_err_log_addr
    esr_addr = 0x00d; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 

    // read Shire_Cache_ESR: sc_sbe_dbe_counts
    esr_addr = 0x00e; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 

    // read Shire_Cache_ESR: sc_reqq_debug_ctl
    esr_addr = 0x00f; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 

    // read Shire_Cache_ESR: sc_reqq_debug0
    esr_addr = 0x010; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 

    // read Shire_Cache_ESR: sc_reqq_debug1
    esr_addr = 0x011; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 

    // read Shire_Cache_ESR: sc_reqq_debug2
    esr_addr = 0x012; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 

    // read Shire_Cache_ESR: sc_reqq_debug3
    esr_addr = 0x013; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 


    // read Shire_Cache_ESR: sc_eco_ctl
    esr_addr = 0x014; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 


    // read Shire_Cache_ESR: sc_perfmon_ctl_status
    esr_addr = 0x017; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 

    // read Shire_Cache_ESR: sc_perfmon_cyc_cntr
    esr_addr = 0x018; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 

    // read Shire_Cache_ESR: sc_perfmon_p0_cntr
    esr_addr = 0x019; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 

    // read Shire_Cache_ESR: sc_perfmon_p1_cntr
    esr_addr = 0x01a; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 

    // read Shire_Cache_ESR: sc_perfmon_p0_qual
    esr_addr = 0x01b; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 

    // read Shire_Cache_ESR: sc_perfmon_p1_qual
    esr_addr = 0x01c; 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 




    // ??? skip Shire_Cache_ESR with PP=0x2 (uArch Messages) ???
    // PP_MESSAGES = 2: reserved for microarchitectural messages, shall always be trapped as "access fault", except for SP in M-mode
/*    
    // read Shire_Cache_ESR: sc_trace_addr_enable: PP_Message
    esr_addr = 0x3f0; 
    read_esr_new(PP_MESSAGES, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  // sc_bnk=0 
    read_esr_new(PP_MESSAGES, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x01);  // sc_bnk=1 
    read_esr_new(PP_MESSAGES, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x02);  // sc_bnk=2 
    read_esr_new(PP_MESSAGES, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x03);  // sc_bnk=3 
*/



/*
    // write some (random?) data to above ESR: how many bits? assume 64-bit?

    FENCE;    // make sure all above "Loads" are completed;

    // write_esr: sc_l3_shire_swizzle_ctl:
    //uint64_t  sc_l3_shire_swizzle_ctl_new_val = 0xdeafbabedeaf0001;
    //uint64_t  sc_l3_shire_swizzle_ctl_new_val = 0xdeaf0001;
    //write_esr(PP_MACHINE, shire_id, REGION_OTHER, sc_l3_shire_swizzle_ctl, sc_l3_shire_swizzle_ctl_new_val); 
    write_esr(PP_MACHINE, shire_id, REGION_OTHER, sc_l3_shire_swizzle_ctl, 0xdeafbabedeaf0001); 
    FENCE;
    write_esr(PP_MACHINE, shire_id, REGION_OTHER, sc_l3_shire_swizzle_ctl, 0xdeaf0001); 

    FENCE;    // make sure all above "Stores" are completed;     

    // read back the new data:
    read_esr(PP_MACHINE, shire_id, REGION_OTHER, sc_l3_shire_swizzle_ctl); 
*/


/*
    // read_esr: sc_trace_ctl: PP=2'b10 (PP_MESSAGES): only from "Service Processor" in M-mode!!!
    uint32_t  sc_trace_ctl = 0x3f2;  // valid [16:0] 
    read_esr(PP_MESSAGES, shire_id, REGION_OTHER, sc_trace_ctl);   // failed: RTL-> Exception/Trap
    // 1447000 ns : INFO CoSIM: Cyc: 1309[H0:S0:N0:C0:T0] RTL took Exception  @ PC: 0x8000200138 
    //       Inst: ld x15,0(x15)(0x7b783)  Cause: 5 -->> access_fault: expected, see note below!!!
    // ??? how to access it from our test ???
*/




    // 3. Shire_Other ESR:  assume REGION_OTHER([21:20]=2'b11)
    shr_cach_sub_region = 0x02;    // Shire_Other ESR ([19:17]=3'b010)

    // Shire_Other_ESR: read_esr: minion_feature
    esr_addr = 0x000;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: shire_config
    esr_addr = 0x001;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  
    // POR value: `ESR_SHIRE_CONFIG_RESET_VAL

    // Shire_Other_ESR: read_esr: thread1_disable
    esr_addr = 0x002;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: shire_cache_build_config
    esr_addr = 0x003;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: shire_cache_revision_id
    esr_addr = 0x004;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: ipi_redirect_mask: user_mode: PP=0x0
    esr_addr = 0x010;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: ipi_redirect_filter
    esr_addr = 0x011;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   
    // POR value: `ESR_FILTER_IPI_RESET_VAL

    // Shire_Other_ESR: read_esr: ipi_trigger
    esr_addr = 0x012;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: ipi_trigger_clear
    esr_addr = 0x013;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   


    // Shire_Other_ESR: read_esr: fcc_credinc_0: user_mode
    esr_addr = 0x018;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fcc_credinc_1: user_mode
    esr_addr = 0x019;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fcc_credinc_2: user_mode
    esr_addr = 0x01a;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fcc_credinc_3: user_mode
    esr_addr = 0x01b;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   


    // Shire_Other_ESR: read_esr: fast_local_barrier0: user_mode
    esr_addr = 0x020;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier1: user_mode
    esr_addr = 0x021;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier2: user_mode
    esr_addr = 0x022;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier3: user_mode
    esr_addr = 0x023;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier4: user_mode
    esr_addr = 0x024;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier5: user_mode
    esr_addr = 0x025;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier6: user_mode
    esr_addr = 0x026;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier7: user_mode
    esr_addr = 0x027;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier8: user_mode
    esr_addr = 0x028;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier9: user_mode
    esr_addr = 0x029;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier10: user_mode
    esr_addr = 0x02a;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier11: user_mode
    esr_addr = 0x02b;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier12: user_mode
    esr_addr = 0x02c;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier13: user_mode
    esr_addr = 0x02d;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier14: user_mode
    esr_addr = 0x02e;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier15: user_mode
    esr_addr = 0x02f;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier16: user_mode
    esr_addr = 0x030;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier17: user_mode
    esr_addr = 0x031;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier18: user_mode
    esr_addr = 0x032;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier19: user_mode
    esr_addr = 0x033;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier20: user_mode
    esr_addr = 0x034;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier21: user_mode
    esr_addr = 0x035;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier22: user_mode
    esr_addr = 0x036;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier23: user_mode
    esr_addr = 0x037;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier24: user_mode
    esr_addr = 0x038;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier25: user_mode
    esr_addr = 0x039;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier26: user_mode
    esr_addr = 0x03a;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier27: user_mode
    esr_addr = 0x03b;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier28: user_mode
    esr_addr = 0x03c;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier29: user_mode
    esr_addr = 0x03d;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier30: user_mode
    esr_addr = 0x03e;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: fast_local_barrier31: user_mode
    esr_addr = 0x03f;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   


/*
    // Shire_Other_ESR: read_esr: and_or_tree_1: PP_Message
    esr_addr = 0x3ff0;
    read_esr_new(PP_MESSAGES, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   
*/


    // Shire_Other_ESR: read_esr: mtime_local_target
    esr_addr = 0x043;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: shire_power_ctrl
    esr_addr = 0x044;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: power_ctrl_neigh_nsleepin
    esr_addr = 0x045;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: power_ctrl_neigh_isolation
    esr_addr = 0x046;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: power_ctrl_neigh_nsleepout
    esr_addr = 0x047;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: thread0_disable
    esr_addr = 0x048;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: shire_error_log
    esr_addr = 0x049;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: shire_pll_auto_config
    esr_addr = 0x04a;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: shire_pll_config_data_0
    esr_addr = 0x04b;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: shire_pll_config_data_1
    esr_addr = 0x04c;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: shire_pll_config_data_2
    esr_addr = 0x04d;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: shire_pll_config_data_3
    esr_addr = 0x04e;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00); 
 
/* Removed registers
    // Shire_Other_ESR: read_esr: shire_pll_config_data_4
    esr_addr = 0x04f;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: shire_pll_config_data_5
    esr_addr = 0x050;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  
*/

    // Shire_Other_ESR: read_esr: shire_pll_read_data
    esr_addr = 0x051;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: shire_coop_mode: Supervisor_mode;
    esr_addr = 0x052;
    read_esr_new(PP_SUPERVISOR, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   
    // POR value: `ESR_SHIRE_COOP_MODE_RESET_VAL


    // Shire_Other_ESR: read_esr: shire_ctrl_clockmux
    esr_addr = 0x053;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);   

    // Shire_Other_ESR: read_esr: shire_cache_ram_cfg1
    esr_addr = 0x054;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  
    // POR value: `ESR_SHIRE_CACHE_RAM_CFG1_RESET_VAL
    
    // Shire_Other_ESR: read_esr: shire_cache_ram_cfg2
    esr_addr = 0x055;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  
    // POR value: `ESR_SHIRE_CACHE_RAM_CFG2_RESET_VAL
    
    // Shire_Other_ESR: read_esr: shire_cache_ram_cfg3
    esr_addr = 0x056;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  
    // POR value: `ESR_SHIRE_CACHE_RAM_CFG3_RESET_VAL
    
    // Shire_Other_ESR: read_esr: shire_cache_ram_cfg4
    esr_addr = 0x057;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  
    // POR value: `ESR_SHIRE_CACHE_RAM_CFG4_RESET_VAL


    // Shire_Other_ESR: read_esr: shire_noc_interrupt_status
    esr_addr = 0x058;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: shire_dll_auto_config
    esr_addr = 0x059;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: shire_dll_config_data_0
    esr_addr = 0x05a;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: shire_dll_read_data
    esr_addr = 0x05b;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

/*
    // Shire_Other_ESR: read_esr: tbox_rbox_dbg_rc:  PP=0x2
    esr_addr = 0x3ff1;
    read_esr_new(PP_MESSAGES, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  
*/

    // Shire_Other_ESR: read_esr: uc_config:  PP=0x1
    esr_addr = 0x05d;
    read_esr_new(PP_SUPERVISOR, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

/*
    // Shire_Other_ESR: read_esr: shire_ctrl_reset_dbg:  PP=0x2
    esr_addr = 0x3ff2;
    read_esr_new(PP_MESSAGES, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  
*/

    // Shire_Other_ESR: read_esr: icache_uprefetch:  PP=0x0
    esr_addr = 0x05f;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: icache_sprefetch:  PP=0x1
    esr_addr = 0x060;
    read_esr_new(PP_SUPERVISOR, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: icache_mprefetch:  PP=0x3
    esr_addr = 0x061;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Other_ESR: read_esr: clk_gate_ctrl:  PP=0x3
    esr_addr = 0x062;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

/*
    // Shire_Other_ESR: read_esr: debug_clk_gate_ctrl:  PP=0x2
    esr_addr = 0x3ff4;
    read_esr_new(PP_MESSAGES, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  
*/

    // Shire_Other_ESR: read_esr: shire_channel_eco_ctrl:  PP=0x3
    esr_addr = 0x068;
    read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  





    /////////////////////////////////////////////////////////

/*  // RTL takes exception on these read to Rbox_ESR ???
   
    // Shire_Rbox ESR:  assume REGION_OTHER([21:20]=2'b11); all with PP=0x0 (user_mode) 
    shr_cach_sub_region = 0x01;    // Shire_Rbox ESR ([19:17]=3'b001)
     
    // Shire_Rbox_ESR: read_esr: rbox_config
    esr_addr = 0x000;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  
    // POR value: 0?
    
    // Shire_Rbox_ESR: read_esr: rbox_in_buf_pg
    esr_addr = 0x001;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  
    // POR value: 0?

    // Shire_Rbox_ESR: read_esr: rbox_in_buf_cfg
    esr_addr = 0x002;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  
    // POR value: 0?

    // Shire_Rbox_ESR: read_esr: rbox_out_buf_pg
    esr_addr = 0x003;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Rbox_ESR: read_esr: rbox_out_buf_cfg
    esr_addr = 0x004;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Rbox_ESR: read_esr: rbox_status
    esr_addr = 0x005;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Rbox_ESR: read_esr: rbox_start
    esr_addr = 0x006;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Rbox_ESR: read_esr: rbox_consume
    esr_addr = 0x007;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Rbox_ESR: read_esr: rasterization_counter0_config
    esr_addr = 0x010;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Rbox_ESR: read_esr: rasterization_counter1_config
    esr_addr = 0x011;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Rbox_ESR: read_esr: rasterization_counter2_config
    esr_addr = 0x012;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Rbox_ESR: read_esr: rasterization_counter3_config
    esr_addr = 0x013;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Rbox_ESR: read_esr: rasterization_counter0
    esr_addr = 0x020;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Rbox_ESR: read_esr: rasterization_counter1
    esr_addr = 0x021;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Rbox_ESR: read_esr: rasterization_counter2
    esr_addr = 0x022;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

    // Shire_Rbox_ESR: read_esr: rasterization_counter3
    esr_addr = 0x023;
    read_esr_new(PP_USER, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, 0x00);  

*/


    C_TEST_PASS;
    return 0;
}
