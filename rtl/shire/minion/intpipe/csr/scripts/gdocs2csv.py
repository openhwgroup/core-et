#!/usr/bin/env python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import sys
import os


if "RTLROOT" not in os.environ:
    raise Exception( "RTLROOT not set" )
rtlroot= os.environ['RTLROOT']
sys.path.append(rtlroot + '/shire/scripts')
from CsrFile import CsrFile
from CsvExporter import CsvExporter

# generate code
dest = rtlroot + '/shire/minion/intpipe/csr/scripts/'
exporter=CsvExporter(CsrFile.CSR_SPREADSHEET_ID)
exporter.export(dest+'csr.csv', dest + 'types/')

