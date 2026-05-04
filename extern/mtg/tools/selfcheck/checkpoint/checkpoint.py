#!/usr/bin/env python3

from mako.template import Template
from os import listdir, path, system, popen
import argparse
import glob
import shutil
import sys

# Add parent paths to find core modules
sys.path.insert(0, path.join(path.dirname(path.realpath(__file__)), '../../..'))
from core.test_conf import TEST_CONF

def gen_src_file(output_filename, post_mako):
   f = open(output_filename,"w+")
   f.write(post_mako)
   f.close()

cwd = path.dirname(path.realpath(__file__))

#################################
# Python Argparse code
#################################
parser = argparse.ArgumentParser()
parser.add_argument("snip_dir", type=str, help="Path to the snippets (*.h) directory")
parser.add_argument("crc_mem_addr", type=int, help="Base memory address where the CRC signatures are stored")
parser.add_argument("crc_mem_size", type=int, help="Size of the memory region where the CRC signatures are stored")
parser.add_argument("-ec",  "--extra-checks", help="Add sys_emu extra checks (e.g. memory checker)", action="store_true")
parser.add_argument("-t", "--test", type=str, help="Path to the C test file. If not specified, the default one is 'base.cc'", action="store")
parser.add_argument("-min", "--minions", type=str, help="A mask of Minions that should be enabled in each Shire (default: 1 Minion/Shire)", action="store")
parser.add_argument("-shi", "--shires", type=str, help="A mask of Shires that should be enabled. (default: 1 Shire)", action="store")
args = parser.parse_args()

prefix = args.snip_dir
template = cwd + "/mtg_export_checkpoints.h.mako"
output_filename = prefix+"/mtg_export_checkpoints.h"

if args.minions:
   assert "0x" in str(args.minions), "Minions mask must be in hex format (including '0x')"
   min_mask = str(args.minions)
else:
   min_mask = "0x1"

if args.shires:
   assert "0x" in str(args.shires), "Shires mask must be in hex format (including '0x')"
   shi_mask = str(args.shires)
else:
   shi_mask = "0x1"

active_harts_per_minion = 2
active_minions_per_shire = bin(int(min_mask, 0)).count('1')
active_shires = bin(int(shi_mask, 0)).count('1')
NUM_ACTIVE_HARTS = active_shires*active_minions_per_shire*active_harts_per_minion

if args.test:
   tmp = args.test.split(".")
   assert tmp[-1] == "cc", "Provided template ("+str(args.test)+") is not a '*.cc' file"
   test_name = args.test
else:
   test_name = "base.cc"

if args.extra_checks:
   extra_checks = " -vpurf_check -mem_check -mem_check_minion -1 -l1_scp_check -l1_scp_check_minion -1 -l2_scp_check -l2_scp_check_shire -1 -flb_check -flb_check_shire -1 -tstore_check -tstore_check_thread -1"
else:
   extra_checks = ""

elf_name = path.splitext(test_name)[0]

tmp = Template(filename=template)
post_mako = tmp.render(addr=args.crc_mem_addr, size=args.crc_mem_size, CRC_REGION="CRC16_REGION")

gen_src_file(output_filename, post_mako)

# Compile the intermediate ELF that contains the prints for the CRC16 memory region
compile_cmd = "EXTRA_CFLAGS+=' -DGEN_CHECKPOINT' "+str(cwd)+"/../../compile/compile.sh "+str(prefix)+" "+str(elf_name)
print("Compiling with: "+compile_cmd)
system(compile_cmd)

# Select emulator based on target
if TEST_CONF.TARGET == 'erbium':
   emu_bin = str(cwd)+"/../../sys_emu/erbium_emu"
   sys_emu_cmd = emu_bin+" -elf_load "+str(elf_name)+" -reset_pc 0x40000000 -minions "+min_mask+" -max_cycles 10000000000 > sysemu.log.crc16"
else:
   emu_bin = str(cwd)+"/../../sys_emu/sys_emu"
   sys_emu_cmd = emu_bin+" -max_cycles 10000000000 -minions "+min_mask+" -shires "+shi_mask+" -elf "+str(elf_name)+extra_checks+"  -pu_uart1_tx_file crc16.out  > sysemu.log.crc16"

print("Running in emulator using: "+sys_emu_cmd)
system(sys_emu_cmd)
system("cp "+str(elf_name)+" gen_chkpts_"+str(elf_name))

pass_marks = popen('grep -c "PASS" sysemu.log.crc16').read()
assert int(pass_marks) == NUM_ACTIVE_HARTS, "MTG: Failed to generate intermediate CRC16 signatures"

# Filter the output prints and create a file called 'mtg_crc16_signatures.h'
# For Erbium, CRC16 output goes to sysemu.log.crc16 via validation1 CSR (format: "[H0] CRC16: ...")
# For ET-SoC-1, CRC16 output goes to crc16.out via UART (format: "CRC16: ...")
if TEST_CONF.TARGET == 'erbium':
   system('grep "CRC16: " sysemu.log.crc16 | awk \'{print $2}\' FS=\': \' > '+str(prefix)+'/mtg_crc16_signatures.h')
else:
   system('grep "CRC16: " crc16.out | awk \'{print $2}\' FS=\': \' > '+str(prefix)+'/mtg_crc16_signatures.h')
system('sed -i \'s@.*CRC16.*@#include "mtg_crc16_signatures.h"@g\' '+str(prefix)+'/mtg_memory_map.h')

# Replace the CRC16 Components with the ones that actually contain the self-checking
components_prefix = str(prefix)+'/components'
files = glob.glob(str(components_prefix)+'/*_gen_signature_*')
assert len(files) != 0
for i in files:
   basename = path.basename(i)
   shutil.move(str(components_prefix)+'/.'+basename, str(components_prefix)+'/'+basename)
