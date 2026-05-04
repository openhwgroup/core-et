#include "catch.hpp"

#include <array>
#include <functional>

#include "mock.h"

#include "lib_snip_transform.h"

// Tests for the mock suite itself
//   setup 1s1n16t : Test setup for 1s1n16t config
//   setup 1s4n4t  : Test setup for 1s4n4t (sparse) config

TEST_CASE("setup 1s1n16t", "[mock]") {

    auto cpus = mock::setup(1, 16);

    // Check CpuResource's
    REQUIRE(cpus.size() == 16);

    REQUIRE(cpus[0].id == 0);
    REQUIRE(cpus[3].id == 3);
    REQUIRE(cpus[7].id == 7);
    REQUIRE(cpus[15].id == 15);

    REQUIRE(snp_cpu_id(&cpus[0]) == 0);
    REQUIRE(snp_cpu_id(&cpus[3]) == 3);
    REQUIRE(snp_cpu_id(&cpus[7]) == 7);
    REQUIRE(snp_cpu_id(&cpus[15]) == 15);

    // Check BOARD_CONF
    REQUIRE(BOARD_CONF.NUM_CPUS == 16);
    REQUIRE(BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP == 16);
    REQUIRE(BOARD_CONF.LCPUID_TO_PCPUID_MAP.size() == 16);

    REQUIRE(BOARD_CONF.LCPUID_TO_PCPUID_MAP[0] == 0);
    REQUIRE(BOARD_CONF.LCPUID_TO_PCPUID_MAP[3] == 3);
    REQUIRE(BOARD_CONF.LCPUID_TO_PCPUID_MAP[7] == 7);
    REQUIRE(BOARD_CONF.LCPUID_TO_PCPUID_MAP[15] == 15);

    SECTION("glbl random values") {
        constexpr size_t num_rands = 100;
        using rand_values = std::array<unum_t, num_rands>;
        std::array<rand_values, 4> rands;

        const auto rnd_glbl = [](CpuResource& cpu, unum_t& val) { val = snp_randnum_glbl(&cpu); };
        using namespace std::placeholders;

        (void)snp_randnum(&cpus[0]);
        (void)snp_randnum(&cpus[0]);
        (void)snp_randnum(&cpus[0]);

        std::for_each(rands[0].begin(), rands[0].end(), std::bind(rnd_glbl, cpus[0], _1));
        std::for_each(rands[1].begin(), rands[1].end(), std::bind(rnd_glbl, cpus[3], _1));
        std::for_each(rands[2].begin(), rands[2].end(), std::bind(rnd_glbl, cpus[7], _1));
        std::for_each(rands[3].begin(), rands[3].end(), std::bind(rnd_glbl, cpus[15], _1));

        for (size_t i = 0; i < num_rands; ++i) {
            REQUIRE(rands[0][i] == rands[1][i]);
            REQUIRE(rands[1][i] == rands[2][i]);
            REQUIRE(rands[2][i] == rands[3][i]);
        }
    }
}

TEST_CASE("setup 1s4n4t", "[mock]") {

    auto cpus = mock::setup(4, 4);

    // Check CpuResource's
    REQUIRE(cpus.size() == 16);

    REQUIRE(cpus[0].id == 0);
    REQUIRE(cpus[3].id == 3);
    REQUIRE(cpus[7].id == 7);
    REQUIRE(cpus[15].id == 15);

    REQUIRE(snp_cpu_id(&cpus[0]) == 0);
    REQUIRE(snp_cpu_id(&cpus[3]) == 3);
    REQUIRE(snp_cpu_id(&cpus[7]) == 7);
    REQUIRE(snp_cpu_id(&cpus[15]) == 15);

    // Check BOARD_CONF
    REQUIRE(BOARD_CONF.NUM_CPUS == 16);
    REQUIRE(BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP == 16);
    REQUIRE(BOARD_CONF.LCPUID_TO_PCPUID_MAP.size() == 16);

    REQUIRE(BOARD_CONF.LCPUID_TO_PCPUID_MAP[0] == 0);
    REQUIRE(BOARD_CONF.LCPUID_TO_PCPUID_MAP[3] == 3);
    REQUIRE(BOARD_CONF.LCPUID_TO_PCPUID_MAP[7] == 19);
    REQUIRE(BOARD_CONF.LCPUID_TO_PCPUID_MAP[15] == 51);

    SECTION("glbl random values") {
        constexpr size_t num_rands = 100;
        using rand_values = std::array<unum_t, num_rands>;
        std::array<rand_values, 4> rands;

        const auto rnd_glbl = [](CpuResource& cpu, unum_t& val) { val = snp_randnum_glbl(&cpu); };
        using namespace std::placeholders;

        (void)snp_randnum(&cpus[0]);
        (void)snp_randnum(&cpus[0]);
        (void)snp_randnum(&cpus[0]);

        std::for_each(rands[0].begin(), rands[0].end(), std::bind(rnd_glbl, cpus[0], _1));
        std::for_each(rands[1].begin(), rands[1].end(), std::bind(rnd_glbl, cpus[3], _1));
        std::for_each(rands[2].begin(), rands[2].end(), std::bind(rnd_glbl, cpus[7], _1));
        std::for_each(rands[3].begin(), rands[3].end(), std::bind(rnd_glbl, cpus[15], _1));

        for (size_t i = 0; i < num_rands; ++i) {
            REQUIRE(rands[0][i] == rands[1][i]);
            REQUIRE(rands[1][i] == rands[2][i]);
            REQUIRE(rands[2][i] == rands[3][i]);
        }
    }
}
