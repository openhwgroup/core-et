#include <iostream>
#include <algorithm>

#include "mock.h"
#include "lib_snip_transform.h"

int main(int argc, const char** argv) {
    srand(time(NULL));

    const auto& cpu_map = BOARD_CONF.LCPUID_TO_PCPUID_MAP;

    auto cpus = mock::setup(4, 4);

    const auto debug_cpu = [&](CpuResource& cpu) {
        const auto id = snp_cpu_id(&cpu);
        const auto hart = cpu_map[cpu.id];

        constexpr auto num_rand = 10;

        std::cout << "CPU " << id << " (H" << hart << ")\n";

        std::cout << "  rand:     ";
        for (auto i = 0; i < num_rand; ++i) {
            std::cout << "  " << snp_urandrange(&cpu, 10, 99);
        }
        std::cout << '\n';

        std::cout << "  rand_glbl:";
        for (auto i = 0; i < num_rand; ++i) {
            std::cout << "  " << snp_urandrange_glbl(&cpu, 10, 99);
        }
        std::cout << '\n';
    };

    std::for_each(cpus.begin(), cpus.end(), debug_cpu);
}
