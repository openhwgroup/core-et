#!/usr/bin/env python3

from mako.template import Template
from os import listdir, path, environ
import argparse

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
parser.add_argument("-a", "--active_hart_ids", nargs="+", help="List of the active HART ID's. The default value is '[0,1]'", action="store")
parser.add_argument("-am", "--agnostic_mode", help="If sweep mode is enabled, logical HID will not be binded to physical HID", action="store_true")
parser.add_argument("snip_dir", type=str, help="Path to the snippets (*.h) directory")
args = parser.parse_args()

filename = cwd + "/lhid_map.h.mako"

if args.active_hart_ids:
   active_hids = list(map(int, args.active_hart_ids))
else:
   active_hids = [0,1]

output_filename = "mtg_" + ((path.basename(filename)).split("."))[0] + ".h"
prefix = args.snip_dir

if args.agnostic_mode:
   agnostic_mode = True
else:
   agnostic_mode = False

#################################

target = environ.get('MTG_TARGET', 'erbium')

if target == 'erbium':
   MAX_HARTS_IN_SOC = 8 * 2
else:
   MAX_HARTS_PER_MINION = 2
   MAX_MINIONS_PER_NEIGH = 8
   MAX_NEIGHS_PER_SHIRE = 4
   MAX_SHIRES_IN_SOC = 34
   MAX_HARTS_IN_SOC = MAX_HARTS_PER_MINION*MAX_MINIONS_PER_NEIGH*MAX_NEIGHS_PER_SHIRE*MAX_SHIRES_IN_SOC

if agnostic_mode:
   active_hids = list(range(0, len(active_hids)))
   lhid_map = active_hids*int((MAX_HARTS_IN_SOC/(len(active_hids))))
else:
   lhid_map = [-1]*MAX_HARTS_IN_SOC

   lhid = 0
   for i in active_hids:
      lhid_map[i] = lhid
      lhid = lhid + 1

#################################


tmp = Template(filename=filename)
post_mako = tmp.render(lhid_map=lhid_map)

print("Generating Physical HART ID to Logical HART ID mapping: "+output_filename)
gen_src_file(prefix+"/"+output_filename, post_mako)
