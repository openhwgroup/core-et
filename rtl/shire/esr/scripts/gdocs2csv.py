#!/usr/bin/env python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import sys
import os


if "RTLROOT" not in os.environ:
    raise Exception( "RTLROOT not set" )
rtlroot= os.environ['RTLROOT']
sys.path.append(rtlroot + '/shire/scripts')
from EsrFile import EsrFile
from CsvExporter import CsvExporter

# generate code
dest = rtlroot + '/shire/esr/scripts/'
exporter=CsvExporter(EsrFile.ESR_SPREADSHEET_ID)

exporter.export(dest+'esr.csv', dest + 'types/')
    

