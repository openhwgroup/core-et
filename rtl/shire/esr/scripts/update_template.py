# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import sys
import os
from shutil import copyfile

if "RTLROOT" not in os.environ:
    raise Exception( "RTLROOT not set" )
rtlroot= os.environ['RTLROOT']

sys.path.append(rtlroot + '/shire/scripts')
from XsrFileIO import printToFile, replaceInFile, mkdir_p, getFilePortion


modules=["esr_neigh", "esr_shire_other"]
customLabels = ["PARAMS", "PORTS", "STATEMENTS", "SIGNALS", "POST_DECODE",  "POST_WRITE_COMB"]


with open('esr_slave_template.v',"r") as f:
    template = f.readlines()

for mod in modules:
    filename = rtlroot + '/shire/esr/' + mod + '.v'
    custom = [ getFilePortion(filename, "CUSTOM "+ x + " BEGIN", "CUSTOM " + x + " END", True) for x in customLabels ]
    copyfile( filename, filename + '.old')
    contents = [x.replace('--MOD_NAME--', mod) for x in template ]
    printToFile(filename, ''.join(contents))
    for i,lab in enumerate(customLabels):
        replaceInFile(filename, custom[i],  "CUSTOM " + lab + " BEGIN", "CUSTOM " + lab + " END", False)


