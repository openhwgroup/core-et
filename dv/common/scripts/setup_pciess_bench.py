#!/usr/bin/python
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0


import sys
import re
import os


if (os.path.isdir("sim_uvm")):
    sys.exit("ERROR: sim_uvm directory exists. remove it or copy it aside")

os.system("cp -rf $PCIE_IP_DIR/subsystem/sim_uvm .")
my_files = [ "sim_uvm/compile_vip.sh", "sim_uvm/runtest.pl", "sim_uvm/runtest.sh", 
             "sim_uvm/modellist", "sim_uvm/svt_version", "sim_uvm/testbench/DWC_pcie_subsystem.test_list" ]

for thisfile in my_files:
    os.system("chmod +w %s" % thisfile)

snps_ver = "N-2017.12"
with open('sim_uvm/modellist', 'w+') as fileHandle:
    fileHandle.write("pcie_device_agent_svt -v %s\n" % snps_ver)
    fileHandle.write("axi_system_env_svt -v %s\n" % snps_ver)
    fileHandle.write("apb_system_env_svt -v %s\n" % snps_ver)

with open('sim_uvm/svt_version', 'w+') as fileHandle:
    fileHandle.write("svt -v %s\n" % snps_ver)

print_lines = """

# ---------------------SUCCESS---------------------------------------
# % cd sim_uvm
# Create a new testlist that contains only the tests you want to run.
# % cp testbench/DWC_pcie_subsystem.test_list <yourtestlistname>;
# Use the following command to run the testlist
# % ./runtest.sh <yourtestlistname> -NOCOV
# -------------------------------------------------------------------

"""

print (print_lines)
