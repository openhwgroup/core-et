#!/usr/bin/python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0


import os
import re
import sys
import csv

def file_match(fname, pat, data_offset, table_min, table_avg, table_max):
    try:
        f = open(fname, "rt")
    except IOError:
        return
    file_name = os.path.basename(fname)
    if file_name == "testme.log":
        for i, line in enumerate(f):
            if pat.search(line):
                print ("%s: %i: %s" % (fname, i+1, line))
                modes = []
                for t in line.split():
                    try:
                        modes.append(int(t))
                    except ValueError:
                        pass
                print (modes)
                lock_times = []
                for t in line.split():
                    try:
                        lock_times.append(round(float(t), 3))
                    except ValueError:
                        pass
                print (lock_times)
                if(table_min[modes[0]][modes[2]] == "/"):
                    table_min[modes[0]][modes[2]] = lock_times[data_offset]
                else:
                    if(table_min[modes[0]][modes[2]] > lock_times[data_offset]):
                	    table_min[modes[0]][modes[2]] = lock_times[data_offset]
                if(table_avg[modes[0]][modes[2]] == "/"):
                    table_avg[modes[0]][modes[2]] = lock_times[data_offset]
                else:
                    table_avg[modes[0]][modes[2]] = round(((table_avg[modes[0]][modes[2]] + lock_times[data_offset])/2), 3)
                if(table_max[modes[0]][modes[2]] == "/"):
                    table_max[modes[0]][modes[2]] = lock_times[data_offset]
                else:
                    if(table_max[modes[0]][modes[2]] < lock_times[data_offset]):
                        table_max[modes[0]][modes[2]] = lock_times[data_offset]
    f.close()

def grep(dir_name, s_pat, data_offset, table_min, table_avg, table_max):
    pat = re.compile(s_pat)
    for dirpath, dirnames, filenames in os.walk(dir_name):
        for fname in filenames:
            fullname = os.path.join(dirpath, fname)
            file_match(fullname, pat, data_offset, table_min, table_avg, table_max)

if len(sys.argv) != 2:
    u = "Usage: [parse_pll_logs] <dir_name> \n"
    sys.stderr.write(u)
    sys.exit(1)

def parse_prog_logs(s_pat, data_offset, table_min, table_avg, table_max):
    grep(sys.argv[1], s_pat, data_offset, table_min, table_avg, table_max)

def grep_logs(s_pat, data_offset, size, table_min, table_avg, table_max):
    for x in range(size):
        freq = 300 + x*25
        table_min[0][x+1] = str(freq)+"MHz"
        table_min[x+1][0] = str(freq)+"MHz"
        table_avg[0][x+1] = str(freq)+"MHz"
        table_avg[x+1][0] = str(freq)+"MHz"
        table_max[0][x+1] = str(freq)+"MHz"
        table_max[x+1][0] = str(freq)+"MHz"
    parse_prog_logs(s_pat, data_offset, table_min, table_avg, table_max)

print ("Script started")

a_min = [["/" for x in range(46)] for x in range(46)]
a_avg = [["/" for x in range(46)] for x in range(46)]
a_max = [["/" for x in range(46)] for x in range(46)]
grep_logs("MODE From mode", 8, 45, a_min, a_avg, a_max)
with open("full_prog_min.csv", 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(a_min)
with open("full_prog_avg.csv", 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(a_avg)
with open("full_prog_max.csv", 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(a_max)

b_min = [["/" for x in range(30)] for x in range(30)]
b_avg = [["/" for x in range(30)] for x in range(30)]
b_max = [["/" for x in range(30)] for x in range(30)]
grep_logs("FCW NO WAIT LOCK From mode", 6, 29, b_min, b_avg, b_max)
with open("fcw_prog_min.csv", 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(b_min)
with open("fcw_prog_avg.csv", 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(b_avg)
with open("fcw_prog_max.csv", 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(b_max)

c_delta = [["/" for x in range(30)] for x in range(30)]
for x in range(29):
    freq = 300 + x*25
    c_delta[0][x+1] = str(freq)+"MHz"
    c_delta[x+1][0] = str(freq)+"MHz"
for x in range(29):
    for y in range(29):
        if(a_avg[x+1][y+1] != "/" and b_avg[x+1][y+1] != "/"):
            c_delta[x+1][y+1] = a_avg[x+1][y+1] - b_avg[x+1][y+1]
with open("full_vs_fcw_prog_delta.csv", 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(c_delta)

a_min = [["/" for x in range(30)] for x in range(30)]
a_avg = [["/" for x in range(30)] for x in range(30)]
a_max = [["/" for x in range(30)] for x in range(30)]
grep_logs("FCW NO WAIT LOCK From mode", 5, 29, a_min, a_avg, a_max)
with open("fcw_prog_time_min.csv", 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(a_min)
with open("fcw_prog_time_avg.csv", 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(a_avg)
with open("fcw_prog_time_max.csv", 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(a_max)

a_min = [["/" for x in range(30)] for x in range(30)]
a_avg = [["/" for x in range(30)] for x in range(30)]
a_max = [["/" for x in range(30)] for x in range(30)]
grep_logs("FCW NO WAIT LOCK MVLS clock deviation", 4, 29, a_min, a_avg, a_max)
with open("fcw_overshoot_min.csv", 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(a_min)
with open("fcw_overshoot_avg.csv", 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(a_avg)
with open("fcw_overshoot_max.csv", 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(a_max)

a_min = [["/" for x in range(30)] for x in range(30)]
a_avg = [["/" for x in range(30)] for x in range(30)]
a_max = [["/" for x in range(30)] for x in range(30)]
grep_logs("FCW NO WAIT LOCK MVLS clock deviation", 6, 29, a_min, a_avg, a_max)
with open("fcw_undershoot_min.csv", 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(a_min)
with open("fcw_undershoot_avg.csv", 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(a_avg)
with open("fcw_undershoot_max.csv", 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(a_max)

print ("Script ended")
