#!/usr/bin/env python3

from mako.template import Template
from os import listdir, path
import argparse
import glob

def gen_src_file(output_filename, post_mako):
   f = open(output_filename,"w+")
   f.write(post_mako)
   f.close()

cwd = path.dirname(path.realpath(__file__))

#################################
# Python Argparse code
#################################
seed = 0
parser = argparse.ArgumentParser()
parser.add_argument("-l", "--lhid_abi_reg", type=int, help="The integer ABI reg that stores the Logical HART ID. Default value is register '27'", action="store")
parser.add_argument("-s", "--seed", type=int, help="Initial seed for the random number generator. If not specified, the default seed is "+str(seed), action="store")
parser.add_argument("-t", "--template", type=str, help="Path to the test mako template. If not specified, the default one is 'base.cc.mako'", action="store")
parser.add_argument("-l2", "--postsi_l2", help="Avoid generating any request the L2", action="store_true")
parser.add_argument("-rc", "--repeat_count", help="Test repeat count", action="store")
parser.add_argument("-sc", "--self_check", help="Include self-checking code", action="store")
parser.add_argument("snip_dir", type=str, help="Path to the snippets (*.h) directory")
args = parser.parse_args()

rc = -1
if args.repeat_count:
   rc = args.repeat_count

if args.lhid_abi_reg:
   lhid_abi_reg = "x"+str(args.lhid_abi_reg)
else:
   lhid_abi_reg = "x27"

if args.seed:
   seed = args.seed

if args.template:
   tmp = args.template.split("/")
   tmp = tmp[-1].split(".")
   assert tmp[-1] == "mako", "Provided template is not a '*.mako' file"
   filename = path.abspath(args.template)
else:
   filename = cwd + "/templates/base.cc.mako"

sc = 1
if args.self_check:
    sc = args.self_check

output_filename = ((path.basename(filename)).split("."))[0] + ".cc"
prefix = args.snip_dir

l2_only = False
if args.postsi_l2:
   l2_only = True

#################################

class TestInfo:
   seed = seed
   num_harts = 1
   snip_dir = prefix
   lhid_reg = lhid_abi_reg
   l2_only = l2_only
   repeat_count = int(rc)
   self_check = sc

info = TestInfo()

mem_map_file = prefix+"/mtg_memory_map.h"

with open(prefix+'/PREAMBLE.out') as f:
    content = f.readlines()
preamble_list = [x.strip() for x in content]

with open(prefix+'/START.out') as f:
    content = f.readlines()
start_list = [x.strip() for x in content]

with open(prefix+'/MAIN.out') as f:
    content = f.readlines()
main_list = [x.strip() for x in content]

with open(prefix+'/END.out') as f:
    content = f.readlines()
end_list = [x.strip() for x in content]

with open(prefix+'/HANDLERS.out') as f:
    content = f.readlines()
handler_list = [x.strip() for x in content]

mtg_dir = cwd+'/../..'
crc16_file = cwd+'/../crc16/crc16.cpp'

tmp = Template(filename=filename)
post_mako = tmp.render(MTG_DIR = mtg_dir, crc16_file = crc16_file, mem_map_file = mem_map_file, preamble_list = preamble_list, start_list = start_list, main_list = main_list, end_list = end_list, handler_list = handler_list, test_info = info)

print("Generating test main file: "+output_filename)
gen_src_file(output_filename, post_mako)
