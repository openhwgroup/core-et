#include "mock.h"

#include <cassert>
#include <cstdlib>

std::vector<CpuResource> mock::setup(unum_t num_neighs, unum_t num_threads) {
    static constexpr unum_t threads_per_neigh = 16;
    static constexpr auto rand_warmup = 10000;

    assert(num_neighs > 0);
    assert(num_threads > 0);
    assert(num_threads <= threads_per_neigh);

    const auto num_cpus = num_neighs * num_threads;

    const auto glbl_seed = std::rand();

    BOARD_CONF.NUM_CPUS = num_cpus;
    BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP = num_cpus;
    auto& cpu_map = BOARD_CONF.LCPUID_TO_PCPUID_MAP;
    cpu_map.resize(num_cpus);

    std::vector<CpuResource> cpus(num_cpus);

    for (unum_t neigh = 0; neigh < num_neighs; ++neigh) {
        for (unum_t thread = 0; thread < num_threads; ++thread) {
            const unum_t lid = neigh * num_threads + thread;
            const unum_t pid = neigh * threads_per_neigh + thread;

            // Map ID's
            cpu_map[lid] = pid;

            // Setup CPU
            auto& cpu = cpus[lid];
            cpu.id = lid;
            cpu.rand.seed(std::rand());
            cpu.rand_glbl.seed(glbl_seed);

            // Warmup RNG
            cpu.rand.discard(rand_warmup);
            cpu.rand_glbl.discard(rand_warmup);
        }
    }

    return cpus;
}
