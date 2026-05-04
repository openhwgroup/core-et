# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

config_name = 'foobar'

b = new_build(name='b1',run_cmd='sleep 30')
add_build(b)

def p(build):
    build.add_test(new_test(name='t1',run_cmd='sleep 30'))  

p(b)
