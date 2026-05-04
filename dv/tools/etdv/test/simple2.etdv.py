# config is imported (automatically) into scope.
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

# The _builds property accesses (r/w) an array of Build objects to be added via add_build.
builds = config._builds
config_name = 'my_config'
build = new_build(name='my_buildA',run_cmd='run command TBD',check_cmd='check command TBD')
assert 0 == len(builds)
add_build(build)
assert 1 == len(builds)
assert 0 == len(builds[0].tests)
test_run_cmd = 'test run command TBD'
build.add_test(
    new_test(
        name='test_nightly',
        run_cmd=test_run_cmd,
        check_cmd='test check command TBD',
        tags=['nightly', 'smoke']
    )
)
assert 1 == len(builds[0].tests)
assert 'test_nightly' == builds[0].tests[0]['name']
assert test_run_cmd == builds[0].tests[0]['run_cmd']
pass
