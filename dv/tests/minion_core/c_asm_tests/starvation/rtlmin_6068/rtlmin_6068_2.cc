/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "starvation.h"
#include "rtlmin_6068.h"

class gsc_starve_test : public starvation <gsc_starve_test> {
public:
  static void run_tests(unsigned id) {
    auto table = 
        make_tests< gsc_starvation_test, run_func_t,
                        range<5,5,1> /*loop unroll*/,
                        range<40,30,1> /*nops_before_gsc*/,
                        range<0,0,1> /*nops_after_gsc*/,
                        range<50,25,2> /*nops_second_thread*/,
                        range<DIV,DIV,1> /*shared_res*/,
                        range<S_MEM,S_MEM,1> /*gsc_insn*/
                  > ::sweep();
     
    for (auto test: table)
      test(id);
  }
};

int main() {
  gsc_starve_test::run();
}
