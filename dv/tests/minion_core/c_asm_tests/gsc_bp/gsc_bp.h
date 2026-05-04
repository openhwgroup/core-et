/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _RTLMIN_GSC_BP_H_
#define _RTLMIN_GSC_BP_H_

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#ifdef ALLOW_TRAP_TO_DEBUG
constexpr bool CAN_TRAP_TO_DEBUG = true;
#else
constexpr bool CAN_TRAP_TO_DEBUG = false;
#endif


constexpr uint64_t DBG_REQ_FETCH_BRK       = 0x1;
constexpr uint64_t DBG_REQ_DATA_BRK        = 0x2;
constexpr uint64_t DBG_REQ_DONE            = 0x3;
constexpr uint64_t DBG_REQ_SKIP            = 0x4;

constexpr uint64_t DBG_SUCCESS             = 0x11;
constexpr uint64_t DBG_ERROR               = 0x12;

//LD/ST VALUE DEFINITION FOR SWITCH STATEMENT
#define FSCB_INST 0
#define FSCH_INST 1
#define FSCW_INST 2
#define FGB_INST 3
#define FGH_INST 4
#define FGW_INST 5
#define LB_INST 6
#define LH_INST 7
#define LW_INST 8
#define SB_INST 9
#define SH_INST 10
#define SW_INST 11


#define CAUSE_USER_ECALL         0x8
#define CAUSE_SERVICE_ECALL         0x9
#define CAUSE_MACHINE_ECALL         0xb

#ifdef ALLOW_TRAP_TO_DEBUG
  #define ITERATIONS 50 // The debugger is VERY SLOW. Could take hours. Reducing iterations to something more resonable
#else
  #define ITERATIONS 200
#endif

#define CAUSE_BREAKPOINT 3


enum class priv_t : uint8_t {mmode=3, smode=1, umode=0};

typedef struct {
    bool is_sc_or_g; //The current test expects to trap to debug instead of the trap handler
    bool expects_to_trap_to_debug;  //know if the last ld/st instruction executed is scatter/gather to check or not the gsc_progress
    priv_t priv_mod; //3-m 1-s 0-u

    void init() {
        this->is_sc_or_g = false;
        this->expects_to_trap_to_debug = false;
        this->priv_mod = priv_t::mmode;
    }

} hart_attr_t;

// We use a union with cache line to avoid false sharing
typedef union {
    uint8_t __min_size[L1_CL_SIZE];
    hart_attr_t attr;
} hart_data_t;


EXTERN_C uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval, uint64_t thread);
EXTERN_C void test_mtrap_vector();



static inline uint64_t get_rand(uint64_t *array, uint64_t &ptr, uint64_t size)  {
  uint64_t val = array[ptr++];
  if (ptr == size) ptr = 0;
  return val;
}

#define RANDOM(x) get_rand(random_##x , x##_ptr ,  sizeof(random_##x) / sizeof(uint64_t) )



inline void random_delay(uint64_t d);

void print_boolean(bool val){
 if(val)
      et_write_val2(0x7e5);
    else
      et_write_val2(0x40);
}

uint64_t read_size(uint64_t select_ld_st_inst){

  switch(select_ld_st_inst){

    case FSCB_INST:
      return 1;
    case FSCH_INST:
      return 2;
    case FSCW_INST:
      return 4;
    case FGB_INST:
      return 1;
    case FGH_INST:
      return 2;
    case FGW_INST:
      return 4;
    case LB_INST:
      return 1;
    case LH_INST:
      return 2;
    case LW_INST:
      return 4;
    case SB_INST:
      return 1;
    case SH_INST:
      return 2;
    case SW_INST:
      return 4;
   }
   return 0;

}

void m_to_u_mode() {
  __asm__ __volatile__
    (
     "li t0, 0xffff\n"
     "csrrc x0, mstatus, t0\n"  // clear mstatus 16 lower bits
     "li t0, 0x6000\n"
     "csrrs x0, mstatus, t0\n"  // clear mstatus 16 lower bits
     "la t0, 1f\n"
     "csrw mepc, t0\n"          // set mepc to user-mode entry point
     "mret\n"                   // go to user mod
     "1:\n"                     // label to referr to user-mode
     :
     :
     : "t0"
     );
}

void m_to_s_mode() {
  __asm__ __volatile__
    (
     "li t0, 0xffff\n"
     "csrrc x0, mstatus, t0\n"  // clear mstatus 16 lower bits
     "li t0, 0x6800\n"
     "csrrs x0, mstatus, t0\n"  // clear mstatus.mpp
     "la t0, 1f\n"
     "csrw mepc, t0\n"          // set mepc to user-mode entry point
     "mret\n"                   // go to user mod
     "1:\n"                     // label to referr to user-mode
     :
     :
     : "t0"
     );
}

void to_m_mode(priv_t &priv_mod) {
  __asm__ __volatile__ ("ecall\n"
                        "nop\n");
  priv_mod=priv_t::mmode;
}

inline void ebreak() {
  __asm__ __volatile__ ("ebreak\n");
}

inline void set_mscratch(uint64_t value) {
  __asm__ __volatile__
    (
     "csrrw zero, mscratch, %[v]\n"
     :
     : [v] "r" (value)
     :
     );
}

inline uint64_t get_mscratch() {
  uint64_t value;
    __asm__ __volatile__
    (
     "csrr %[v], mscratch\n"
     : [v] "=r" (value)
     :
     :
     );
    return value;
}



#endif
