# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

config_name = 'my_config'
build_cmd = ""
check_cmd='echo pass > $TARGET'

build = new_build(name='my_buildA',run_cmd=build_cmd,check_cmd=check_cmd)
add_build(build)
for i in [2,5,10]:
    run_cmd = ""
    test = new_test(name=f'my_test_{i}', run_cmd=run_cmd,check_cmd=check_cmd)
    build.add_test(test)

