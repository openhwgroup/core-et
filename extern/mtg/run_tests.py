#!/usr/bin/env python3
"""
MTG Test Runner - runs multiple randomized tests and reports results.

Usage:
    ./run_tests.py           # Run 1 test with random seed (8 minions × 2 threads = 16 HARTs)
    ./run_tests.py -n 10     # Run 10 tests with random seeds
    ./run_tests.py -s 42     # Run 1 test with specific seed
    ./run_tests.py -n 10 -s 42  # Run 10 tests starting from seed 42
    ./run_tests.py -min 0x3 -thr 0x3  # Run with 2 minions × 2 threads = 4 HARTs
"""

import argparse
import os
import random
import subprocess
import sys
from datetime import datetime

def run_single_test(seed: int, test_dir: str, config: str, ncomps: int,
                    minions: str, threads: str, shires: str,
                    verbose: bool = False) -> tuple[str, str]:
    """Run a single test and return (status, error_summary).

    Status is one of: 'pass', 'gen_fail' (generation failed), 'test_fail' (ELF ran but failed)
    """

    env = os.environ.copy()
    env['PATH'] = f"/opt/et/bin:{env.get('PATH', '')}"
    env['MINION_DIAGS'] = os.path.expanduser('~/nekkoai/cosim/dv/tests/minion_core/c_asm_tests/')
    env['REPOROOT'] = os.path.expanduser('~/nekkoai/cosim/')
    env['MTG_TARGET'] = 'erbium'

    # Create unique subdirectory with timestamp_seed for each test run
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    run_dir = f"{test_dir}/{timestamp}_{seed}"

    cmd = [
        'python', 'mtg.py', run_dir,
        '-c', config,
        '-ncomps', str(ncomps),
        '-s', str(seed),
        '-min', minions,
        '-thr', threads,
        '-shi', shires
    ]

    result = subprocess.run(
        cmd,
        env=env,
        capture_output=True,
        text=True
    )

    if verbose:
        print(result.stdout)
        if result.stderr:
            print(result.stderr)

    # Check if test passed by looking at return code
    if result.returncode == 0:
        return 'pass', None

    # Determine failure type by looking at output
    error_summary = None
    is_gen_fail = False

    combined_output = result.stdout + result.stderr

    # Check for generation-phase errors (before ELF runs)
    for line in combined_output.split('\n'):
        if 'AssertionError' in line:
            error_summary = line.strip()
            # Resource allocation errors are generation phase
            if 'soft-allocate' in line or 'allocate the resource' in line:
                is_gen_fail = True
            else:
                is_gen_fail = True  # Other assertions are also gen failures
            break
        # readelf error means ELF wasn't created (gen failure)
        if 'readelf: Error' in line and 'No such file' in line:
            error_summary = line.strip()
            is_gen_fail = True
            break
        # Compilation errors are gen failures
        if 'error:' in line.lower() and ('gcc' in line.lower() or 'ld' in line.lower()):
            error_summary = line.strip()
            is_gen_fail = True
            break

    # Check for SYS_EMU failure (test phase) - only if not already gen_fail
    if not is_gen_fail:
        for line in combined_output.split('\n'):
            if 'SYS_EMU execution failed' in line:
                error_summary = line.strip()
                break
            if 'Signal end test with FAIL' in line:
                error_summary = line.strip()
                break

    status = 'gen_fail' if is_gen_fail else 'test_fail'
    return status, error_summary or "Unknown error"

def main():
    parser = argparse.ArgumentParser(description='Run MTG tests')
    parser.add_argument('-n', '--num-tests', type=int, default=1,
                        help='Number of tests to run (default: 1)')
    parser.add_argument('-s', '--seed', type=int, default=None,
                        help='Starting seed (default: random). For multiple tests, seeds increment from this value.')
    parser.add_argument('-c', '--config', type=str, default='erbium_rand.py',
                        help='Config file (default: erbium_rand.py)')
    parser.add_argument('--ncomps', type=int, default=10,
                        help='Number of components per test (default: 10)')
    parser.add_argument('--test-dir', type=str, default='./test_out',
                        help='Test output directory (default: ./test_out)')
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='Show full test output')
    parser.add_argument('-min', '--minions', type=str, default='0xff',
                        help='Minion mask (default: 0xff = 8 minions)')
    parser.add_argument('-thr', '--threads', type=str, default='0x3',
                        help='Thread mask (default: 0x3 = 2 threads)')
    parser.add_argument('-shi', '--shires', type=str, default='0x1',
                        help='Shire mask (default: 0x1 = 1 shire)')
    args = parser.parse_args()

    # Create test directory
    os.makedirs(args.test_dir, exist_ok=True)

    # Generate seeds
    if args.seed is not None:
        seeds = [args.seed + i for i in range(args.num_tests)]
    else:
        seeds = [random.getrandbits(64) for _ in range(args.num_tests)]

    passed = 0
    gen_failed = 0
    test_failed = 0

    # Calculate number of HARTs
    num_minions = bin(int(args.minions, 16)).count('1')
    num_threads = bin(int(args.threads, 16)).count('1')
    num_shires = bin(int(args.shires, 16)).count('1')
    num_harts = num_minions * num_threads * num_shires

    print(f"Running {args.num_tests} test(s) with {num_harts} HARTs "
          f"({num_minions} minions × {num_threads} threads × {num_shires} shires)...\n", flush=True)

    for i, seed in enumerate(seeds):
        status, error_summary = run_single_test(
            seed, args.test_dir, args.config, args.ncomps,
            args.minions, args.threads, args.shires, args.verbose
        )

        if status == 'pass':
            passed += 1
            status_str = "\033[32mPASS\033[0m"  # Green
            print(f"[{i+1}/{args.num_tests}] {status_str} (seed={seed})", flush=True)
        elif status == 'gen_fail':
            gen_failed += 1
            status_str = "\033[33mGEN_FAIL\033[0m"  # Yellow
            print(f"[{i+1}/{args.num_tests}] {status_str} (seed={seed}): {error_summary}", flush=True)
        else:  # test_fail
            test_failed += 1
            status_str = "\033[31mTEST_FAIL\033[0m"  # Red
            print(f"[{i+1}/{args.num_tests}] {status_str} (seed={seed}): {error_summary}", flush=True)

    # Tally
    print(f"\n{'='*60}")
    print(f"Results: {passed} PASS, {gen_failed} GEN_FAIL, {test_failed} TEST_FAIL, {args.num_tests} total")

    if test_failed > 0:
        sys.exit(1)  # Real test failures
    elif gen_failed > 0:
        sys.exit(2)  # Only generation failures (may be acceptable)
    else:
        sys.exit(0)

if __name__ == '__main__':
    main()
