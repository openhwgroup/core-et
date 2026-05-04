/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "et_test_common.h"
#include "macros.h"
#include <inttypes.h>
#include <stdlib.h>
#include "minion.h"
#include "gsc_bp.h"


// The test setups the breakpoint unit to trigger exceptions
// if can test either FETCH or DATA breakpoints (with an scatter, to check gsc progress as well)
// The assembly does some basic checking, but most of the checking relies on the BEMU
//
// FETCH:
//   Sets the bp to a PC expected to be executed, sets mscratch to 9
//   In the trap handler, disables the bp, error if mscratch!=9, sets mscratch back to 8 and returns to the same
//   PC to continue executing where it left.. after the breakpoint, error if mscratch != 8
//
// DATA:
//   It randomizes the offsets for the 8 lanes and the PS mask
//   It configures the tdata2 to match one of the addresses used in one of the lanes
//   mscratch CSR is set to the expected lane to trigger the breakpoint (or 8 if no lane has to trigger)
//   If the breakpoint triggers ( it may not if the lane with the matching address is masked) it will enter the trap handler
//   In the trap handler:
//       - Error if exceptions cause is not breakpoint
//       - Error if gsc_progress doesn't match the mscratch (trapped for an incorrect lane, or trapped when not expected)
//       - mscracht is set to 8, tdata2 to an invalid address (so that there are no more traps for this iteration)
//       - mret with mepc set to the same gather (will restaret in the lane that trapped). No more traps expected.
//   After the gather (it either did not trap or returned from a trap):
//       - Error if mscracth != 8


//////////////////////////////////////////////////////////////////////
// buffer where to store/load data to and from
//////////////////////////////////////////////////////////////////////
uint8_t buffer[1024];

//////////////////////////////////////////////////////////////////////
// precomputed arrays with random numbers
// use macro RANDOM(offset), RANDOM(lane), RANDOM(int0_to_255) to retrieve next rand value
//////////////////////////////////////////////////////////////////////
uint64_t random_offset[] = { 448, 640, 128, 64, 448, 896, 704, 64, 256, 128, 704, 448, 320, 832, 832, 960, 448, 960, 128, 448, 640, 960, 896, 128, 832, 384, 704, 896,
                             256, 384, 512, 576, 768, 0, 704, 256, 0, 64, 576, 448, 832, 512, 64, 448, 576, 64, 0, 896, 640, 768, 320, 896, 64, 192, 512, 320, 768, 512,
                             64, 704, 576, 960, 64, 512, 192, 704, 384, 128, 0, 0, 832, 128, 704, 384, 128, 768, 192, 768, 128, 0, 512, 448, 256, 384, 832, 960, 768,
                             640, 832, 64, 576, 448, 832, 640, 64, 576, 384, 512, 640, 960};


uint64_t random_lane[] = { 2, 5, 1, 6, 3, 2, 2, 5, 3, 1, 2, 7, 0, 6, 4, 3, 7, 4, 7, 2, 4, 0, 6, 5, 7, 0, 4, 3, 1, 2, 0, 4, 6, 6, 4, 5, 3, 1, 7, 3, 0, 6, 2, 7, 6, 6,
                           3, 6, 3, 6, 5, 5, 7, 1, 1, 6, 6, 3, 7, 6, 4, 3, 1, 7, 5, 7, 2, 3, 2, 7, 4, 3, 0, 4, 4, 6, 6, 5, 7, 3, 2, 0, 0, 1, 7, 6, 0, 0, 5, 1, 7, 7,
                           5, 6, 4, 1, 2, 4, 4, 3};

uint64_t random_int0_to_255[] = { 10, 9, 1, 0, 15, 11, 12, 12, 11, 15, 4, 2, 14, 9, 7, 3, 11, 1, 14, 6, 0, 5, 15, 6, 11, 13, 4, 12, 9, 14, 14, 9, 11, 5, 14, 6, 9, 6, 15, 15,
                         4, 1, 4, 1, 13, 11, 11, 10, 5, 9, 14, 8, 11, 3, 9, 4, 1, 15, 14, 7, 10, 9, 7, 1, 5, 1, 8, 13, 5, 15, 2, 9, 4, 7, 3, 13, 0, 10, 9, 7, 10, 0,
                         11, 10, 4, 4, 8, 12, 6, 6, 9, 4, 13, 8, 5, 1, 1, 6, 13, 9};

uint64_t random_mcontrol_mask[] = {94, 27,  118, 253, 112, 199, 56,  181, 145, 30,
                                   74, 83,  122, 65,  179, 16,  157, 170, 157, 113,
                                   174, 231, 107, 181, 234, 195, 207, 226, 204, 226,
                                   115,  20,  70,  206, 116, 25,  2, 228, 184, 124,
                                   207, 125, 200, 112, 249, 190, 160, 25,  41,  162,
                                   33,  106, 144, 204, 190, 172, 96,  204, 109, 206,
                                   167, 252, 244, 72,  226, 33,  212, 178, 173, 166,
                                   146, 107, 156, 100, 238, 228, 30,  162, 49,  110,
                                   225, 159, 207, 196, 172, 211, 94,  156, 178, 79,
                                   250, 216, 252, 122, 185, 64,  180, 145, 66,  183};

/*uint64_t random_mcontrol_mask[] = {46, 127,  47,  106,  13,  50,  84,  27,  49,  54,
                                   30,  46,  101, 51,  14,  42,  52,  7, 73,  9,
                                   56,  42,  112,  2, 50,  25,  63,  95,  28,  13,
                                   60,  8, 23,  38,  71,  59,  25,  98,  9, 16,
                                   49,  47,  54,  19,  54,  127,  50,  45,  3, 2,
                                   18,  54,  109,  58,  22,  27,  62,  116,  46,  23,
                                   57,  22,  109, 86, 27,  18,  16,  17,  57,  124,
                                   13,  21,  60,  20,  115,  61,  16,  22,  17,  62,
                                   53,  95,  15,  45,  28,  60,  38,  38,  61,  38,
                                   111,  24,  31,  21,  58,  78,  48,  32,  41,  40};*/


//GLOBAL VARIABLES

hart_data_t hart_data[2048];

//////////////////////////////////////////////////////////////////////
// configure trap vector and setup tdata1 CSR
// set tdata2 and mscratch to initial values
//////////////////////////////////////////////////////////////////////

void random_ld_or_st(uint64_t base, uint64_t trigger_addr, uint64_t select, bool &is_load, bool &is_sc_or_g, uint64_t exp_lane){

  uint64_t dbg_info = 0;

  switch(select){
    /*
     * The debug expects certain parameters on x31
     * The result is always returned on mscratch
     */
    case FSCB_INST:
      is_load = false;
      is_sc_or_g = true;
      dbg_info = is_sc_or_g;
      dbg_info = dbg_info << 32 | exp_lane;
      __asm__ __volatile__
        (
         "mv x31, %[dbg_info]\n"
         "fscb.ps f0,f0(%[base])"
         :
         : [base] "r" (base), [dbg_info] "r" (dbg_info)
         : "f0", "x31"
         );
      break;
    case FSCH_INST:
      is_load = false;
      is_sc_or_g = true;
      dbg_info = is_sc_or_g;
      dbg_info = dbg_info << 32 | exp_lane;
      __asm__ __volatile__
        (
         "mv x31, %[dbg_info]\n"
         "fsch.ps f0,f0(%[base])"
         :
         : [base] "r" (base), [dbg_info] "r" (dbg_info)
         : "f0", "x31"
         );
      break;
    case FSCW_INST:
      is_load = false;
      is_sc_or_g = true;
      dbg_info = is_sc_or_g;
      dbg_info = dbg_info << 32 | exp_lane;
      __asm__ __volatile__
        (
         "mv x31, %[dbg_info]\n"
         "fscw.ps f0,f0(%[base])"
         :
         : [base] "r" (base), [dbg_info] "r" (dbg_info)
         : "f0", "x31"
         );
      break;
    case FGB_INST:
      is_load = true;
      is_sc_or_g = true;
      dbg_info = is_sc_or_g;
      dbg_info = dbg_info << 32 | exp_lane;
      __asm__ __volatile__
        (
         "mv x31, %[dbg_info]\n"
         "fgb.ps f0,f0(%[base])"
         :
         : [base] "r" (base), [dbg_info] "r" (dbg_info)
         : "f0", "x31"
         );
      break;
    case FGH_INST:
      is_load = true;
      is_sc_or_g = true;
      dbg_info = is_sc_or_g;
      dbg_info = dbg_info << 32 | exp_lane;
      __asm__ __volatile__
        (
         "mv x31, %[dbg_info]\n"
         "fgh.ps f0,f0(%[base])"
         :
         : [base] "r" (base), [dbg_info] "r" (dbg_info)
         : "f0", "x31"
         );

      break;
    case FGW_INST:
      is_load = true;
      is_sc_or_g = true;
      dbg_info = is_sc_or_g;
      dbg_info = dbg_info << 32 | exp_lane;
      __asm__ __volatile__
        (
         "mv x31, %[dbg_info]\n"
         "fgw.ps f0,f0(%[base])"
         :
         : [base] "r" (base), [dbg_info] "r" (dbg_info)
         : "f0", "x31"
         );
      break;
    case LB_INST:
      is_load = true;
      is_sc_or_g = false;
      dbg_info = is_sc_or_g;
      dbg_info = dbg_info << 32 | exp_lane;
      __asm__ __volatile__
        (
         "mv x31, %[dbg_info]\n"
         "mv x30, %[trigger_addr]\n"
         "lb x0,0(%[trigger_addr])"
         :
         : [trigger_addr] "r" (trigger_addr), [dbg_info] "r" (dbg_info)
         : "x0", "x31"
         );
      break;
    case LH_INST:
      is_load = true;
      is_sc_or_g = false;
      dbg_info = is_sc_or_g;
      dbg_info = dbg_info << 32 | exp_lane;
      __asm__ __volatile__
        (
         "mv x31, %[dbg_info]\n"
         "mv x30, %[trigger_addr]\n"
         "lh x0,0(%[trigger_addr])"
         :
         : [trigger_addr] "r" (trigger_addr), [dbg_info] "r" (dbg_info)
         : "x0", "x31"
         );
      break;
    case LW_INST:
      is_load = true;
      is_sc_or_g = false;
      dbg_info = is_sc_or_g;
      dbg_info = dbg_info << 32 | exp_lane;
      __asm__ __volatile__
        (
         "mv x31, %[dbg_info]\n"
         "mv x30, %[trigger_addr]\n"
         "lw x0,0(%[trigger_addr])"
         :
         : [trigger_addr] "r" (trigger_addr), [dbg_info] "r" (dbg_info)
         : "x0", "x31"
         );
      break;

    case SB_INST:
      is_load = false;
      is_sc_or_g = false;
      dbg_info = is_sc_or_g;
      dbg_info = dbg_info << 32 | exp_lane;
      __asm__ __volatile__
        (
         "mv x31, %[dbg_info]\n"
         "mv x30, %[trigger_addr]\n"
         "sb x0,0(%[trigger_addr])"
         :
         : [trigger_addr] "r" (trigger_addr), [dbg_info] "r" (dbg_info)
         : "x0", "x31"
         );
      break;
    case SH_INST:
      is_load = false;
      is_sc_or_g = false;
      dbg_info = is_sc_or_g;
      dbg_info = dbg_info << 32 | exp_lane;
      __asm__ __volatile__
        (
         "mv x31, %[dbg_info]\n"
         "mv x30, %[trigger_addr]\n"
         "sh x0,0(%[trigger_addr])"
         :
         : [trigger_addr] "r" (trigger_addr), [dbg_info] "r" (dbg_info)
         : "x0", "x31"
         );
      break;
    case SW_INST:
      is_load = false;
      is_sc_or_g = false;
      dbg_info = is_sc_or_g;
      dbg_info = dbg_info << 32 | exp_lane;
      __asm__ __volatile__
        (
         "mv x31, %[dbg_info]\n"
         "mv x30, %[trigger_addr]\n"
         "sw x0,0(%[trigger_addr])"
         :
         : [trigger_addr] "r" (trigger_addr), [dbg_info] "r" (dbg_info)
         : "x0", "x31"
         );
      break;
    default:
      is_load = false;
      is_sc_or_g = false;
      // We should not reach this points.
      __asm__ __volatile__ ("unimp");
   }
}

bool check_ecall(uint64_t cause, priv_t priv_mod){

    return (((priv_mod == priv_t::umode) && (cause == CAUSE_USER_ECALL)) ||
            ((priv_mod == priv_t::smode) && (cause == CAUSE_SERVICE_ECALL)) ||
            ((priv_mod == priv_t::mmode) && (cause == CAUSE_MACHINE_ECALL)));
}

void random_exec_mode(priv_t &priv_mod, uint64_t int0_to_255_ptr ){

   uint64_t select = RANDOM(int0_to_255) % 3;// 0, 1 or 2
   priv_t priv_list[3] = { priv_t::umode, priv_t::smode, priv_t::mmode};
   priv_t selected_mod = priv_list[select];

   if(selected_mod != priv_mod){
      switch(selected_mod){
      case priv_t::umode:
          m_to_u_mode();
          break;
      case priv_t::smode:
          m_to_s_mode();
          break;
      case priv_t::mmode:
          break;
          //We don't need to do anything cause we always return to M-Mode to execute CSR
        default:
          break;
      }
      priv_mod = selected_mod;
   }

}
void setup_trap_vector() {
     //configure trap vector
       __asm__ __volatile__
           (
             "la t0, test_mtrap_vector\n"
             "csrw mtvec, t0\n"
             : : : "t0");
}

__attribute__ ((always_inline))
inline void setup_debug() {
  // Setup DCSR so ebreaks traps to debug
  // We will use them to communicate with the debuger

  /*
   * The debug expects certain parameters on x31
   * The result is always returned on mscratch
   */
  /*__asm__ __volatile__
    (
     "csrrwi x0, fcc, 0\n" //read from CREDIT COUNTER 0
     );
  */

  /* Long delay until the debugger halts the thread
   * A better way could be using the FCC from the debugger
   * but BEMU expects the FCC from the minion (instr retired)
   * That's not a big deal since the checker is disabled on debug test
   * but makes more complicate to debug since there will be not log for the
   * retired insturctions
   */
  uint64_t d = 3*512;
  while(d-- > 0){
    __asm__ __volatile__("nop\n");
  }


}

uint64_t build_tdata1(uint64_t mcontrol_mask) {

  uint64_t tdata1_val =

    mcontrol_mask;    // trigger on m-s-u / exec-store-load
  return tdata1_val;
}

void setup_tdata1( uint64_t &mcontrol_mask, uint64_t &mcontrol_mask_ptr){

    mcontrol_mask = RANDOM(mcontrol_mask);

    uint64_t tdata1_val = build_tdata1(mcontrol_mask);
    uint64_t hart = get_hart_id();

    __asm__ __volatile__
    (
     "csrw tdata2, x0\n"
     "csrw tdata1, %[tdata1_val]\n"
     "csrr t0,tdata1\n"
     :
     : [tdata1_val] "r" (tdata1_val)
     : "t0"
     );
    if (!hart_data[hart].attr.expects_to_trap_to_debug) {
      __asm__ __volatile__
        (
         "csrwi mscratch, 0x8\n"
         );
    }

 }

bool trigger_fetch_bp(priv_t priv_mod, uint64_t mcontrol_mask){
    bool exec_up = (mcontrol_mask >> 2) & 0x01;
    switch(priv_mod){
    case priv_t::umode:
        return (((mcontrol_mask >> 3) & 0x01) && exec_up);
    case priv_t::smode:
        return (((mcontrol_mask >> 4) & 0x01) && exec_up);
    case priv_t::mmode:
        return (((mcontrol_mask >> 6) & 0x01) && exec_up);
    }
   return false;
}

bool trigger_data_bp(priv_t priv_mod, uint64_t mcontrol_mask, bool is_load){
    bool ld_up = (mcontrol_mask) & 0x01;
    bool st_up = (mcontrol_mask >> 1) & 0x01;
    switch(priv_mod){
    case priv_t::umode:
        if(is_load)
          return (((mcontrol_mask >> 3) & 0x01) && ld_up);
        else
          return (((mcontrol_mask >> 3) & 0x01) && st_up);
    case priv_t::smode:
        if(is_load)
          return (((mcontrol_mask >> 4) & 0x01) && ld_up);
        else
          return (((mcontrol_mask >> 4) & 0x01) && st_up);
    case priv_t::mmode:
        if(is_load)
          return (((mcontrol_mask >> 6) & 0x01) && ld_up);
        else
          return (((mcontrol_mask >> 6) & 0x01) && st_up);
   }
   return true;
}


bool match_mbits(uint64_t addr_provided, uint64_t addr_expected){
  int first_zero = 6;
  const int MASKMASK = 6;
  // build mask
  for (int i = 0 ; i < MASKMASK; ++i) {
    uint64_t lsb = (addr_expected >> i) & 0x1;
    if (lsb == 0) {
      first_zero = i;
      break;
    }
  }

  // mask and compare
  uint64_t mask = ~((1 << first_zero)-1);
  uint64_t masked_addr_provided = addr_provided & mask;
  uint64_t masked_addr_expected = addr_expected & mask;

  return masked_addr_expected == masked_addr_provided;
}

void calc_exp_lane(uint64_t base, int32_t *offsets, uint64_t tdata1, uint64_t tdata2, uint64_t &exp_lane, uint64_t m, bool &addr_eq_bp){
  bool match_mask = (tdata1 >> 7) & 0x1;
  for ( int l = 0; l < 8; l++) {
    //if the ld/st instruction is a scatter/gather we compare with the non_byte aligned addr
    if(((m & 1) == 1)){
      uint64_t addr = base + offsets[l];
      if (!match_mask && (addr == tdata2)) {
        exp_lane = l;
        addr_eq_bp = true;
        break;
      } else if (match_mask && match_mbits(addr, tdata2)) {
        exp_lane = l;
        addr_eq_bp = true;
        break;
      }
    }
    m>>=1;
  }
}
//////////////////////////////////////////////////////////////////////
// TEST ENTRY POINT
//////////////////////////////////////////////////////////////////////

int main(){

  C_TEST_START;

  const uint64_t hart = get_hart_id();
  uint64_t thread_id = hart & 0x1;
  //////////////////////////////////////////////////////////////////////
  // local variables
  //////////////////////////////////////////////////////////////////////
  uint64_t offset_ptr = 0;
  uint64_t lane_ptr = 0;
  uint64_t mask = 0xFF;
  uintptr_t base = (uintptr_t) buffer;
  bool trigger_fetch_bp_val;
  uint64_t mcontrol_mask_ptr = 0;
  bool is_load = 0;

  uint64_t mcontrol_mask = 0x5f;//RANDOM(mcontrol_mask);
  uint64_t int0_to_255_ptr = 0;
  bool trigger_data_bp_val;

  if(CAN_TRAP_TO_DEBUG)
    setup_debug();
  setup_trap_vector();


  hart_data[hart].attr.init();

  //////////////////////////////////////////////////////////////////////
  // test loop
  //////////////////////////////////////////////////////////////////////
  for ( int it = 0; it < ITERATIONS; it++ ){
    // from half of the test onwards, random delay before starting any iteration.. so that threads are misaligned
    if ( it > ITERATIONS / 2 ) {
      if (thread_id == (RANDOM(int0_to_255) & 1) )
        random_delay(RANDOM(int0_to_255) % 32);
    }


    if (CAN_TRAP_TO_DEBUG) {
      // Randomly trap to debug
      hart_data[hart].attr.expects_to_trap_to_debug = (RANDOM(int0_to_255) % 2 ) == 0;
    }

    setup_tdata1(mcontrol_mask, mcontrol_mask_ptr);

    ////////////////////////
    // FETCH BREAKPOINTS
    ////////////////////////

    if ( (RANDOM(int0_to_255) % 4 ) == 0 ) { // 1 out of 4 iterations => add fetch breakpoint

      //////////////////////////////////////////////////////////////////////////
      // DEBUG
      //////////////////////////////////////////////////////////////////////////

      if (hart_data[hart].attr.expects_to_trap_to_debug) {
        // Breakpoint will trap to debug. We need to comunicate with debug mode so it can do the checking
        volatile uint64_t dbg_result = 0;

        // Send tdata1
        uint64_t tdata1 = build_tdata1(mcontrol_mask);

        // Send tdata2
        uint64_t bp_addr = 0;
        __asm__ __volatile__
          (
           "la %[bp_addr], bp_targets_dbg\n"
           : [bp_addr] "=r" (bp_addr)
           :
          );

        uint64_t bp_offset = (RANDOM(int0_to_255) % 32) <<2; // %32 because there are 32 nops in the _asm_ below
        uint64_t tdata2 = bp_addr + bp_offset;


        /*
         * The debug expects certain parameters on x31
         * The result is always returned on mscratch
         */
        __asm__ __volatile__
        (
         "addi x31, zero, %[opcode]\n" // Set x31 to 0. (FETCH_BRK)
         "mv x30, %[tdata1]\n"
         "mv x29, %[tdata2]\n"
         "ebreak\n"            // jump to debug
         :
         : [tdata1] "r" (tdata1), [tdata2] "r" (tdata2), [opcode] "i" (DBG_REQ_FETCH_BRK)
         : "x31", "x30", "x29"
         );

        dbg_result = get_mscratch();
        set_mscratch(0);

        if(dbg_result == DBG_ERROR || dbg_result == DBG_REQ_FETCH_BRK)
          C_TEST_FAIL;


        random_exec_mode(hart_data[hart].attr.priv_mod,int0_to_255_ptr);//change priv mode here
        trigger_fetch_bp_val = trigger_fetch_bp(hart_data[hart].attr.priv_mod,mcontrol_mask);//check if we have to trigger or not;

        //bucle of 16 nops and 16 reminders
        //just to test differnt instructions
        __asm__ __volatile__
          (
	   "addi x28, x0, 7\n"
	   "addi x29, x0, 3\n"
           "bp_targets_dbg:\n"
           ".rept 16\n"
           "nop\n"
           ".endr\n"
           ".rept 16\n"
           "remu x31, x28, x29\n"
           ".endr\n"
           :::"x28","x29","x31");
        to_m_mode(hart_data[hart].attr.priv_mod); // Back to M-mode to be able to execute all the CSR

        // Check if we trapped
        dbg_result = get_mscratch();
        set_mscratch(0);

        et_write_val2(0xFECDBA);
        et_write_val2(trigger_fetch_bp_val);
        et_write_val2(0xFECDBF);

        if ((trigger_fetch_bp_val  && (dbg_result == DBG_ERROR || dbg_result == 0)) ||
            (!trigger_fetch_bp_val && (dbg_result != 0))) {
          C_TEST_FAIL;
        }

        if (!trigger_fetch_bp_val) {
          //We didn't expect to trap, notify debug
          set_mscratch(DBG_REQ_SKIP);
          __asm__ __volatile__
            (
             "ebreak\n"
             :::
             );
          set_mscratch(0);
        }

        // Nothing else to do. Debug will deasert dmode so we can continue our execution

      } else {
        // set mscratch=9, setup breakpoint ( to any of the 32 nops added below)
        uint64_t bp_offset = (RANDOM(int0_to_255) % 32) <<2; // %32 because there are 32 nops in the _asm_ below
        __asm__ __volatile__
          (
           "csrwi mscratch, 9\n"
           "la t0, bp_targets\n"
           "add t0, t0, %[bp_offset]\n"
           "csrw tdata2, t0\n"
           :
           : [bp_offset] "r" (bp_offset)
           : "t0"
           );
        random_exec_mode(hart_data[hart].attr.priv_mod,int0_to_255_ptr);//change priv mode here
        trigger_fetch_bp_val = trigger_fetch_bp(hart_data[hart].attr.priv_mod,mcontrol_mask);//check if we have to trigger or not;
        //bucle of 16 and 16 reminers
        //Test different sets of instructions
        __asm__ __volatile__
          (
	   "addi x28, x0, 8\n"
	   "addi x29, x0, 3\n"
           "bp_targets:\n"
           ".rept 16\n"
           "nop\n"
           ".endr\n"
           ".rept 16\n"
           "remu x31, x28, x29\n"
           ".endr\n"
           ::: "x28", "x29", "x31");
        to_m_mode(hart_data[hart].attr.priv_mod); // Back to M-mode to be able to execute all the CSR

        et_write_val2(0xFEC);
        et_write_val2(trigger_fetch_bp_val);

        // check that mscratch is back to 8 (as in.. we have entered the trap handler)
        uint64_t mscratch = get_mscratch();

        if ((trigger_fetch_bp_val) && (mscratch != 8)) {
          C_TEST_FAIL;
        }
        set_mscratch(0);
      }
    }


    ////////////////////////////////////////////////////////////////////////////////
    // DATA BREAKPOINTS
    ////////////////////////////////////////////////////////////////////////////////

    // random mask
    mask = RANDOM(int0_to_255);

    //random offsets
    int32_t offsets[8] __attribute__((aligned (32)));

    // completely random offsets
    if ( RANDOM(int0_to_255) % 8 > 0 ) {
      for ( int l = 0; l < 8 ; l++)
        offsets[l] = RANDOM(offset);
    }
    else { // all the same offset
      offsets[0] =  RANDOM(offset);
      for ( int l = 0; l < 8 ; l++)
        offsets[l] = offsets[0];
    }

    // breakpoint address
    uint64_t tdata2 = base + offsets[RANDOM(lane)];
    uint64_t byte_offset =  (RANDOM(int0_to_255) % 4);
    tdata2 += byte_offset;//unalign the address randomly

    // get lane that'll trigger ( the first one in case the offset is repeated.. will be copied into mscratch)
    uint64_t exp_lane = 0;
    uint64_t m = mask;
    uint64_t select_ld_st_inst = RANDOM(int0_to_255) % 12;
    bool addr_eq_bp = false; //This variable sets to 1 if on cal_exp_lane function we found that the bp address is going to be consumed

    calc_exp_lane(base, offsets, mcontrol_mask, tdata2, exp_lane, m, addr_eq_bp);

    ////////////////////////////////////////////////////////////////////////////////
    // DEBUG
    ////////////////////////////////////////////////////////////////////////////////

    if (hart_data[hart].attr.expects_to_trap_to_debug) {
       // Breakpoint will trap to debug. We need to comunicate with debug mode so it can do the checking
      volatile uint64_t dbg_result = 0;

      // Get tdata1 and setup mask
      uint64_t tdata1 = build_tdata1(mcontrol_mask);
      __asm__ __volatile__
        (
         "mv x6, %[mask]\n"
         "mova.m.x x6\n"
         :
         : [mask] "r" (mask)
         : "x6"
         );

      /*
       * The debug expects certain parameters on x31-29
       * The result is always returned on mscratch
       */

      __asm__ __volatile__
        (
         "addi x31, zero, %[opcode]\n" // Set x31 to 1. (DATA_BRK)
         "mv x30, %[tdata1]\n"
         "mv x29, %[tdata2]\n"
         "ebreak\n"            // jump to debug
         :
         : [tdata1] "r" (tdata1), [tdata2] "r" (tdata2), [opcode] "i" (DBG_REQ_DATA_BRK)
         : "x31", "x30", "x29"
         );

      dbg_result = get_mscratch();
      set_mscratch(0);

      if(dbg_result == DBG_ERROR)
        C_TEST_FAIL;


      random_exec_mode(hart_data[hart].attr.priv_mod,int0_to_255_ptr);
      // Consume from the address setted for trigger the bp using any mem instr
      __asm__ __volatile__
        (
         "flq2 f0,0(%[offsets])\n" //loads gsc offsets
         //"fscw.ps f0,f0(%[base])"
         :
         : [offsets] "r" (offsets)/*,[base] "r" (base)*/
         : "f0"
         );

      random_ld_or_st(base, tdata2, select_ld_st_inst, is_load, hart_data[hart].attr.is_sc_or_g, exp_lane);

      trigger_data_bp_val = trigger_data_bp(hart_data[hart].attr.priv_mod, mcontrol_mask, is_load); // True if the breakpoint is expected to be triggered
      to_m_mode(hart_data[hart].attr.priv_mod);

      dbg_result = get_mscratch(); // Get result from debugger
      set_mscratch(0);

      bool expect_gsc_jump = hart_data[hart].attr.is_sc_or_g && (trigger_data_bp_val && addr_eq_bp);
      bool expect_ldst_jump = !hart_data[hart].attr.is_sc_or_g && trigger_data_bp_val;
      bool jump_with_error = (expect_gsc_jump || expect_ldst_jump)  && (dbg_result == DBG_ERROR || dbg_result == 0);
      bool unexpected_jump = (!expect_gsc_jump && !expect_ldst_jump) && dbg_result != 0;

      et_write_val2(0xDAFADA);
      et_write_val2(trigger_data_bp_val);
      et_write_val2(addr_eq_bp);
      et_write_val2(expect_gsc_jump);
      et_write_val2(expect_ldst_jump);
      et_write_val2(jump_with_error);
      et_write_val2(unexpected_jump);
      et_write_val2(0xDAFADF);

      if (jump_with_error || unexpected_jump)
        C_TEST_FAIL;


      if (!expect_gsc_jump && !expect_ldst_jump) {
        //We didn't expect to trap, notify debug
        set_mscratch(DBG_REQ_SKIP);
        __asm__ __volatile__
          (
           "ebreak\n"
           :::
           );
        set_mscratch(0);
      }

      // Nothing else to do. Debug will clear tdata[1/2]

    } else {
      // configure bp,
      __asm__ __volatile__
        (
         "csrw mscratch, %[exp_lane]\n"
         "csrw tdata2, %[tdata2]\n"
         "mv x6, %[mask]\n"
         "mova.m.x x6\n"
         :
         : [mask] "r" (mask), [exp_lane] "r" (exp_lane), [tdata2] "r" (tdata2)
         : "f0", "f1", "x6"
         );

      random_exec_mode(hart_data[hart].attr.priv_mod,int0_to_255_ptr);
      // Consume from the address setted for trigger the bp using any mem instr
      __asm__ __volatile__
        (
         "flq2 f0,0(%[offsets])\n"
         //"fscw.ps f0,f0(%[base])"
         :
         : [offsets] "r" (offsets)/*,[base] "r" (base)*/
         : "f0"
         );
      random_ld_or_st(base, tdata2, select_ld_st_inst, is_load, hart_data[hart].attr.is_sc_or_g, exp_lane);
      trigger_data_bp_val = trigger_data_bp(hart_data[hart].attr.priv_mod, mcontrol_mask, is_load);
      to_m_mode(hart_data[hart].attr.priv_mod);
      // at this point, exp_lane has to be 8
      __asm__ __volatile__
        (
         "csrr %[exp_lane], mscratch\n"
         : [exp_lane] "=r" (exp_lane)
         );

      et_write_val2(0xDAFAA);
      et_write_val2(trigger_data_bp_val);
      et_write_val2(addr_eq_bp);
      et_write_val2(hart_data[hart].attr.is_sc_or_g);
      et_write_val2(exp_lane);
      et_write_val2(0xDAFAF);


      //print_boolean(trigger_data_bp_val);
      if (trigger_data_bp_val && addr_eq_bp) {
        if(hart_data[hart].attr.is_sc_or_g && exp_lane != 8) {
          C_TEST_FAIL;
        }
      }

      set_mscratch(0);
    }


  }

  if (CAN_TRAP_TO_DEBUG) {
    /*
     * The debug expects certain parameters on x31
     * The result is always returned on mscratch
     *
     * Here we notify debug this thread is done
     */
    // notify dbg that this thread is done
    __asm__ __volatile__
      (
       "addi x31, zero, %[opcode]\n" // Set x31 to 1. (DBG_REQ_DONE)
       "ebreak\n"            // jump to debug
       :
       : [opcode] "i" (DBG_REQ_DONE)
       : "x31"
       );
    C_TEST_PASS;
    return 0;
  }
  else {
      C_TEST_PASS;
      return 0;
  }
}




uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval, uint64_t hart)
{
  // if cause is ecall, it's a jump to 'm mode'
  // set mepc to +4, and mpp to 3
  if (check_ecall(cause, hart_data[hart].attr.priv_mod)) {
    __asm__ __volatile__
      (
       "li t0, 0x1800\n"
       "csrs mstatus, t0\n"
       :
       :
       : "t0" );

    return epc+4;//erase + 4 tommorrow before push Trap return to epc+4 automatically
  }
  // fail if exception other than breakpoint
  else if (cause != CAUSE_BREAKPOINT) {
    C_TEST_FAIL;
  }
  else {
    // read progress and expected progress
    uint64_t exp_progress, progress;
    __asm__ __volatile__
      (
       "csrr %[exp_progress], mscratch\n"
       "csrr %[progress], gsc_progress\n"
       : [exp_progress] "=r" (exp_progress), [progress] "=r" (progress)
       );

    // set mscratch to >7;
    // clear tdata2 so that there are no more matches
    // clear addr_eq_bp as a part of the current bp state
    __asm__ __volatile__
      (
       "csrwi mscratch, 0x8\n"
       "csrwi tdata2, 0\n"
       );
    // distinguish between data and fetch breakpoints
    // fetch => epc == tval;  data => epc != tval

    // FETCH CASE
    if ( epc == tval ) {
      if (exp_progress !=9 )  {
        C_TEST_FAIL;
      }
      // return to the same PC (breakpoint has been disabled, it should move forward)
      return epc;
    }


    // DATA CASE
    if ((progress != exp_progress) && hart_data[hart].attr.is_sc_or_g ) {
      // note that if mscratch > 7 it will also fail ( case where were we not expecting a breakpoint but received one)
      C_TEST_FAIL;
    }

    // return PC to return... the same one to continue where progress ended
    return epc;
  }
  return epc;
}


void inline random_delay(uint64_t d) {
  while(d-- > 0){
    __asm__ __volatile__("nop\n");
  }
}
