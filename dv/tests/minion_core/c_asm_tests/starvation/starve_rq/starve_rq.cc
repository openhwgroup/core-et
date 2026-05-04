/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "starvation.h"
#include "starve_rq.h"
#include "seed.h"

class rq_starve_test : public starvation <rq_starve_test, 4 /*4 harts: 2 active minions*/> {
public:
  static void run_tests(unsigned id) {
    auto table = 
      make_tests< rq_starvation_test, run_func_t
                  ,range<5,5,1>  //loop unroll
                  ,range<32,1,1> // NR_HITS
                  ,range<10,1,1> // NR_NOPS_T0
                  ,range<20,1,1> // NR_ESRS
                  ,range<10,0,1> // NR_NOPS_T1
                  > ::
#ifdef SWEEP
      sweep();
#else
    random<300, SEED>();
#endif

    for (auto test: table)
      test(id);
  }
};

int main() {
  rq_starve_test::run();
}
