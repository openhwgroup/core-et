# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

for i in range(10):
    print ("\n	csrw tensor_load, x0 // xcpt")
    print ("	// fill the pipeline with instructions")
    for j in range(i+1):
        print ("	fadd.ps f%d,f1,f2" % (j+10))
