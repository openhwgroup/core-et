# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

config_name = 'rof_test'

CHECK_CMD = """
    [[ $(fgrep PASS $LOG) ]] && echo pass > $TARGET || echo 'fail bad_signature' > $TARGET
"""
TEST_CMD = """
    [[  0 == $((RANDOM % 2)) ]] && echo PASS || echo FAIL
"""

build = new_build(name='b1')
build['run_cmd'] = "echo PASS"
build['rof_run_cmd'] = build['run_cmd']
build['check_cmd'] = CHECK_CMD
add_build(build)

for i in [1,2,3]:
    test = new_test(name=f"t{i}", run_cmd=TEST_CMD, check_cmd=CHECK_CMD)    
    test['rof_run_cmd'] = test['run_cmd']
    build.add_test(test)

build = new_build(name='b2')
build['run_cmd'] = "echo PASS"
build['rof_run_cmd'] = build['run_cmd']
build['check_cmd'] = CHECK_CMD
add_build(build)

for i in [1,2,3]:
    test = new_test(name=f"t{i}", run_cmd=TEST_CMD, check_cmd=CHECK_CMD)    
    test['rof_run_cmd'] = test['run_cmd']
    build.add_test(test)
