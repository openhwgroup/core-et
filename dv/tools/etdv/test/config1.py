# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

print('DBG1: config1:sequence')

# print(f'DBG4: {globals()}')

import os
from datetime import date

today = date.today()
print(f'DBG5: today={today}')


def cat():
    print('cat: meow')


def foo():
    print('DBG2: foo')
    cat()


def bar():
    def foo():
        if True:
            CWD = os.path.dirname(get_config_filename())
        print('DBG3: bar::foo')

    foo()


#[globals().update({k: v}) for k, v in locals().items() if str(type(v)) == "<class 'function'>"]
#[print(f'DBG:GLOBAL: name={key}') for key, val in globals().items()]
#[print(f'DBG:LOCAL: name={key} is {type(val)}') for key, val in locals().items()]


foo()
bar()

# exec(open('config2.py').read())
#import config2
#config2.reuse()
