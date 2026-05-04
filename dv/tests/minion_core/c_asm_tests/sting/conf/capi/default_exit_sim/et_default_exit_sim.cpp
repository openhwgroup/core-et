#include <stdint.h>

#include "generic_asm_helpers.h"
#include "conf_sc.h"

extern RunTimeData run_time_data[];

#define INFINITE_LOOP       \
   __asm__ __volatile__ ( \
         "1:wfi\n" \
         "j 1b\n" \
         : : : );

// This code is a copy of the Sting default_exit_simv() function (v1.8.46)
// but with some extra instructions (fence and INFINITE_LOOP)
void default_exit_sim() {
    uint32_t code;
    uint32_t cpu_id = get_cpu_id();
    if (run_time_data[cpu_id].test_status == TestStatus::TEST_PASS)
    {
        code = 0x1FEED000;
        asm volatile ("fence iorw, iorw");
        asm volatile ("slti x0,x0,0x7fe");
    }
    else
    {
        code = 0x50BAD000;
        asm volatile ("fence iorw, iorw");
        asm volatile ("slti x0,x0,0x7ff");
    }
    asm volatile ("fence iorw, iorw");
    asm volatile ("csrw 0x8D0, %[ec]" : : [ec] "r" ((uint32_t)code));
    INFINITE_LOOP;
    return;
}
