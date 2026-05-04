
inline void amo_write(uint64_t *addr, uint64_t write_data) {
#if defined(MIN_AGNOSTIC_MODE) || defined(NEIGH_AGNOSTIC_MODE) || defined(SHIRE_AGNOSTIC_MODE)
   __asm__ __volatile__(
      "amoswapl.d x0, %[write_data], (%[addr])\n"
      :
      : [write_data] "r" (write_data),
        [addr] "r" (addr)
      :
   );
#else
   __asm__ __volatile__(
      "amoswapg.d x0, %[write_data], (%[addr])\n"
      :
      : [write_data] "r" (write_data),
        [addr] "r" (addr)
      :
   );
#endif
}

inline uint64_t amo_cmpswp(uint64_t *addr, uint64_t write_val, uint64_t cmp_val) {
   uint64_t old_data;
#if defined(MIN_AGNOSTIC_MODE) || defined(NEIGH_AGNOSTIC_MODE) || defined(SHIRE_AGNOSTIC_MODE)
   __asm__ __volatile__(
      "add x31, x0, %[cmp_val]\n"
      "amocmpswapl.d %[old_data], %[write_val], (%[addr])\n"
      : [old_data] "=r" (old_data)
      : [addr] "r" (addr), [write_val] "r" (write_val), [cmp_val] "r" (cmp_val)
      : "x31"
   );
#else
   __asm__ __volatile__(
      "add x31, x0, %[cmp_val]\n"
      "amocmpswapg.d %[old_data], %[write_val], (%[addr])\n"
      : [old_data] "=r" (old_data)
      : [addr] "r" (addr), [write_val] "r" (write_val), [cmp_val] "r" (cmp_val)
      : "x31"
   );
#endif
   return old_data;
}

uint64_t lock[64/sizeof(uint64_t)] __attribute__((aligned(64))) = {0}; // full cache line
#define CMPSWP_LOCK amo_cmpswp(&lock[0], 1, 0)
#define FREE_LOCK amo_write(&lock[0], 0)

inline void SetLock() {
   while (CMPSWP_LOCK == 1); // spin until the lock seems free
}

inline void UnsetLock() {
   FREE_LOCK;
}
