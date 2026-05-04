
#pragma once
#include <inttypes.h>

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval, uint64_t thread);
EXTERN_C void test_mtrap_vector();

#define LEN(array) (sizeof(array)/sizeof(array[0]))

enum class priv_lv_t : uint8_t
{
 UMODE = 0,
 SMODE = 1,
 DMODE = 2,
 MMODE = 3
};

enum class xcpt_cause_t 
{
 ILLEGAL_INSTR,
 BREAKPOINT,
 MISALIGNED_LD,
 MISALIGNED_ST,
 LD_AF,
 ST_AF,
 M_ECALL,
 S_ECALL,
 U_ECALL,
 BUS_ERROR,
 //DEBUG,
 M_CODE,
 FETCH_AF,
 FETCH_BUS_ERROR
};

typedef struct 
{
  xcpt_cause_t cause;
  priv_lv_t priv;
} xcpt_gen_t;

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

void to_m_mode() {
  __asm__ __volatile__ ("ecall\n"
                        "nop\n");
}

constexpr uint64_t MAX_HART_ID = ((uint64_t)256) << (2+3+1);

priv_lv_t hart_priv_mode[MAX_HART_ID] = { priv_lv_t::MMODE };

void change_priv_to(uint64_t hart, priv_lv_t priv) {
  if (hart_priv_mode[hart] != priv_lv_t::MMODE ) {
    to_m_mode();
    hart_priv_mode[hart] = priv_lv_t::MMODE;
  }
  if (priv == priv_lv_t::SMODE) {
    m_to_s_mode();
    hart_priv_mode[hart] = priv_lv_t::SMODE;
  }
  else if(priv == priv_lv_t::UMODE) {
    m_to_u_mode();
    hart_priv_mode[hart] = priv_lv_t::UMODE;
  }
}
