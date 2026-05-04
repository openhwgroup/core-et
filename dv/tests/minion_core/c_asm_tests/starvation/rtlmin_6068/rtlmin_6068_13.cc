/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "starvation.h"
#include "rtlmin_6068.h"

class gsc_starve_test : public starvation <gsc_starve_test> {
public:
  static void run_tests(unsigned id) {
    auto table = 
        make_tests< gsc_starvation_test, run_func_t,
                        range<10,1,1> /*loop unroll*/,
                        range<80,0,1> /*nops_before_gsc*/,
                        range<10,0,1> /*nops_after_gsc*/,
                        range<120,0,1> /*nops_second_thread*/,
                        range<LD,DIV,1> /*shared_res*/,
                        range<S_ESR,G_MEM,1> /*gsc_insn*/
                #ifdef POSTSI_UST
                  > ::random<50,10004>(); /*for less Instructions size, thus less UST preloading time*/
                #else
                  > ::random<250,10004>();
                #endif 
     
    for (auto test: table)
      test(id);
  }
};

int main() {
  gsc_starve_test::run();
}
