# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import random
for i in range(400):
    if random.randint(0,15) != 0:
        a = random.randint(0,7)
        b = random.randint(0,31)
        c = random.randint(0,31)
        print ("	fadd.ps f%d, f%d, f%d" %(a,b,c))
    else:
        what = random.randint(0,5)
        if what == 0:
            print ("	divw x10, t1, t1")
            print ("	addi x0, x10, 0")
        elif what == 1:
            print ("	csrr x0,hpmcounter3")
            print ("	csrr x0,hpmcounter4")
        else:
            print ("	j 1f\n1:")
