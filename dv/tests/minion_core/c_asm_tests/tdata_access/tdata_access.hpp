
#pragma once

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif


#define LENGHT(a) (sizeof(a)/sizeof(a[0]))
#define EBREAK __asm__ volatile ("ebreak\n");

constexpr uint64_t DBG_SUCCESS             = 0x11;
constexpr uint64_t DBG_ERROR               = 0x12;


constexpr uint64_t ENABLE_DMODE  = 1 << 0;
constexpr uint64_t ENABLE_ACTION = 1 << 1;
constexpr uint64_t HART_DONE = 1 << 2;


EXTERN_C uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval, uint64_t thread);
EXTERN_C void test_mtrap_vector();

struct tdata1_t {
  bool load;
  bool store;
  bool execute;
  bool u;
  bool s;
  bool m;
  bool match;
  bool action;
  bool timing;
  uint8_t maskmax;
  bool dmode;
  uint8_t ttype;

  bool operator ==(const tdata1_t & other) {
    return this->load == other.load &&
      this->store == other.store &&
      this->execute == other.execute &&
      this->u == other.u &&
      this->s == other.s &&
      this->m == other.m &&
      this->match == other.match &&
      this->action == other.action &&
      //this.timing == other.timing &&
      //this.maskmax == other.maskmax &&
      this->dmode == other.dmode;
    //this.ttype == other.ttype;
  }

  bool operator !=(const tdata1_t & other) {
    return not ((*this) == other);
  }

};

tdata1_t read_tdata1() {
  uint64_t tdata1_bits;
  tdata1_t tdata1;
  __asm__ volatile ("csrr %[VAR], tdata1\n"
		    : [VAR] "=r" (tdata1_bits)
		    :
		    :
                    );
  tdata1.load = tdata1_bits & 0x1ULL;
  tdata1.store = (tdata1_bits >> 1ULL) & 0x1ULL;
  tdata1.execute = (tdata1_bits >> 2ULL) & 0x1ULL;
  tdata1.u = (tdata1_bits >> 3ULL) & 0x1ULL;
  tdata1.s = (tdata1_bits >> 4ULL) & 0x1ULL;
  tdata1.m = (tdata1_bits >> 6ULL) & 0x1ULL;
  tdata1.match = (tdata1_bits >> 7ULL) & 0x1ULL;
  tdata1.action = (tdata1_bits >> 12ULL) & 0x1ULL;
  tdata1.timing = (tdata1_bits >> 18ULL) & 0x1ULL;
  tdata1.maskmax = (tdata1_bits >> 53ULL) & 0x3FULL;
  tdata1.dmode = (tdata1_bits >> 59ULL) & 0x1ULL;
  tdata1.ttype = (tdata1_bits >> 60ULL) & 0xFULL;
  return tdata1;
}


void write_tdata1(tdata1_t tdata1) {
  uint64_t tdata1_bits = 0;
  tdata1_bits |= static_cast<uint64_t>(tdata1.load);
  tdata1_bits |= static_cast<uint64_t>(tdata1.store) << 1ULL;
  tdata1_bits |= static_cast<uint64_t>(tdata1.execute) << 2ULL;
  tdata1_bits |= static_cast<uint64_t>(tdata1.u) << 3ULL;
  tdata1_bits |= static_cast<uint64_t>(tdata1.s) << 4ULL;
  tdata1_bits |= static_cast<uint64_t>(tdata1.m) << 6ULL;
  tdata1_bits |= static_cast<uint64_t>(tdata1.match) << 7ULL;
  tdata1_bits |= static_cast<uint64_t>(tdata1.action) << 12ULL;
  tdata1_bits |= static_cast<uint64_t>(tdata1.timing) << 18ULL;
  tdata1_bits |= static_cast<uint64_t>(tdata1.maskmax) << 53ULL;
  tdata1_bits |= static_cast<uint64_t>(tdata1.dmode) << 59ULL;
  tdata1_bits |= static_cast<uint64_t>(tdata1.ttype) << 60ULL;

  __asm__ volatile ("csrw tdata1, %[VAR]\n"
		    : 
		    : [VAR] "r" (tdata1_bits)
		    :
                    );
}

uint64_t read_tdata2() {
  uint64_t tdata2;
  __asm__ volatile ("csrr %[VAR], tdata2\n"
		    : [VAR] "=r" (tdata2)
		    :
		    :
                    );
  return tdata2;
}


void write_tdata2(uint64_t tdata2_data) {

  __asm__ volatile ("csrw tdata2, %[VAR]\n"
		    : 
		    : [VAR] "r" (tdata2_data)
		    :
                    );
}

__attribute__((always_inline))
inline void hint(uint64_t val) {
  /*
   * Wrapper for et_write_val
   * sets validation2 to a value to help
   * debugging
   */
#ifdef DEBUG
  et_write_val2(val);
#endif
}
