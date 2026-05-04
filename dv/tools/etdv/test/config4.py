# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

config_name = 'config4'

build = new_build(name='build1', run_cmd='sleep 10')
add_build(build)

test = new_test(name='test1', run_cmd='sleep 10')
build.add_test(test)
