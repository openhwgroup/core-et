#!/usr/bin/env python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

from openpyxl import load_workbook

range_columns = ["min", "max", "inc"]
columns = ["loop unroll", "nops_before_gsc", "nops_after_gsc", "nops_second_thread", "shared_res", "gsc_insn"]


class TestCase:
    def __init__(self):
        empty = { i: None for i in range_columns }
        self.__testcase = { i: empty.copy() for i in columns}
        
    def add(self, c, val):
        field_idx = int((c -3) / 3)
        range_idx = (c-3) % 3
        self.__testcase[columns[field_idx]][range_columns[range_idx]] = val


    def enumVal(self, v):
        # these are the same enums as the c++
        enum_shared_resource = ["DIV", "DIVW", "MUL", "MULW", "EXCL", "LD"]
        if v in enum_shared_resource:
            return enum_shared_resource.index(v)
        
        enum_gsc = ["G_MEM", "S_MEM", "G_ESR", "S_ESR"]
        if v in enum_gsc:
            return enum_gsc.index(v)

        return v
        
        
    def dimSize(self, mn, mx, inc):
        mx = self.enumVal(mx)
        mn = self.enumVal(mn)
        
        n = int ((mx - mn) / inc + 1)
        
        if n == 0 or mx < mn:
            raise Exception("bad dim size")
        return n

    def table(self, randseed, randcount):
        count = 1
        lines = ["        make_tests< gsc_starvation_test, run_func_t" ]
        for i in columns:
            
            mx = self.__testcase[i]["max"]
            mn = self.__testcase[i]["min"]
            inc = self.__testcase[i]["inc"]
            if mn == None:
                mn = 0
            if inc == None:
                inc = 1

            count *= self.dimSize(mn, mx, inc)
            lines.append("                        range<%s,%s,%s> /*%s*/" % (str(mx), str(mn), str(inc), i))

        tab = ',\n'.join(lines) + "\n                  > ::"
        if randseed:
            return randcount, tab+"random<%d,%d>()" % (randcount, randseed)
        else:
            return count, tab+"sweep()"




    
class TestGen:
    def __init__(self, spreadsheet = 'testCases.xlsx'):
        wb = load_workbook(spreadsheet)
        self.__cases_ws = wb['testcases']
        self.__tests_ws = wb['tests']
        self.__testCaseTables = {}
        self.__tests = {}
        self.__countPerTest = {}
        self.__countPerCase = {}

    def run(self):
        self.readTestCases()
        self.readTests()
        self.output()
        
    def readTestCases(self):
        for r, row in enumerate(self.__cases_ws.rows):
            if r >= 2: # skip header rows
                testcase = TestCase()
                for c, cell in enumerate(row):
                    if c == 0: # name
                        name = cell.value
                    elif c == 1: #rand seed
                        seed = cell.value
                    elif c == 2: # rand count
                        randCount = cell.value
                    elif cell.value != None: # range field
                        testcase.add(c, cell.value)
                self.__countPerCase[name], self.__testCaseTables[name] = testcase.table(seed, randCount)
                

    def readTests(self):
        for r, row in enumerate(self.__tests_ws.rows):
            if r >= 1: # skip header row
                cases = []
                for c, cell in enumerate(row):
                    if c == 0: # name
                        name=cell.value
                    elif cell.value != None:
                        cases.append(cell.value)
                if len(cases) > 0:
                    self.genTestcode(cases, name)

    def genTestcode(self, cases, name):
        lines = []
        self.__countPerTest[name] = 0
        for c in cases:
            if c not in self.__testCaseTables:
                raise Exception("missing testcase %s" % c)
            lines.append(self.__testCaseTables[c])
            self.__countPerTest[name] +=self.__countPerCase[c]
    
        if len(lines) == 1:
            self.__tests[name] = "auto table = \n%s;" % lines[0]
        else:
            self.__tests[name] = "auto table = concat(\n" + ",\n\n".join(lines) + ");"


    def output(self):
        # and generate the tests
        for name in self.__tests:
            print ("Creating %s.cc => %d tests" % (name, self.__countPerTest[name]))
            with open('%s.cc' % name, "w") as f:
                   f.write("""
#include "starvation.h"
#include "rtlmin_6068.h"

class gsc_starve_test : public starvation <gsc_starve_test> {
public:
  static void run_tests(unsigned id) {
    %s
     
    for (auto test: table)
      test(id);
  }
};

int main() {
  gsc_starve_test::run();
}
"""
                           % self.__tests[name])
                   

t = TestGen()
t.run()
