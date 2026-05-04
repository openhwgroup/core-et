#! /bin/python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0


import os
import shutil
import sys

CSR_COMPILE = "csrCompile"
MINION_CSR  = "minion_csr"
REPOROOT = os.getenv("REPOROOT")
RTLROOT  = os.getenv("RTLROOT")

if ((REPOROOT is None) or (RTLROOT is None)):
    print("Expected environment variable REPOROOT and RTLROOT to be set - Did you source the environment file?")
    sys.exit(1)

print(f"Attempting to generate the csr, csv, html files for {MINION_CSR}")
os.system(f"{CSR_COMPILE} -c {REPOROOT}/dv/common/scripts/semifore_css/minion_csr.css -t html -t CSRSpec -t csv {RTLROOT}/shire/minion/intpipe/csr/semifore/xlsx/minion_csr.xlsx")

# Move the csr file to the standard location along with all other CSR files
print(f"Copying {MINION_CSR}.csr file to directory {RTLROOT}/ip/hal/csr")
shutil.copy(f'{MINION_CSR}.csr', f'{RTLROOT}/ip/hal/csr')
