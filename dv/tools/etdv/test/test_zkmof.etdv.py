# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

config_name = 'test_zkmof'
build_cmd = "sleep 5"
check_cmd='echo pass > $TARGET'
build = new_build(name='my_build',run_cmd=build_cmd,check_cmd=check_cmd)
add_build(build)

build.add_test(
	new_test(
		name='t1',
		run_cmd='sleep 5',
		check_cmd='echo fail > $TARGET'
	)
)
build.add_test(
	new_test(
		name='t2',
		run_cmd='sleep 300',
		check_cmd='echo pass > $TARGET'
	)
)
