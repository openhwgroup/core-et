# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import re
import ast
class ParseDefines:
    instance = None
    fileList = []
    
    def __init__ (self):
        defines = {}
        
        for fileName in ParseDefines.fileList:
            with open(fileName,"r") as f:
                contents = f.readlines()
            # load all constants
            for l in contents:
                m = re.match(r'`define \s*([^\s]+) \s*(.+)$', l)
                if m != None:
                    defines[m.group(1)] = m.group(2)

        # merge constants for cases where some constants depend on other constants
        for name, val in defines.items():
            for repName in defines:
                defines[repName] = re.sub(r'`%s'%name, val, defines[repName])
                
        #eval the results.. if there is an unresolved constant ( there is letter in the value), raise exception
        for name,val in defines.items():
            if  re.search(r"[A-Za-z]", val) != None:
                raise Exception("cannot resolve value for constant %s = %s\n "% (name, val) )
            defines[name] = eval(val)

        self.defines = defines
            


    def getValue(self, name):
        if name.isdigit():
            return int(name)
        else:
            name = re.sub(r'^`','', name)
            if name in self.defines:
                return self.defines[name]
            else:
                raise Exception ("unknown constant %s\n"%name)

    @classmethod
    def getInstance(ParseDefines):
        if ParseDefines.instance == None:
            ParseDefines.instance = ParseDefines()
        return ParseDefines.instance

    @classmethod
    def addDefines(ParseDefines,files):
        if isinstance(files, list):
           ParseDefines.fileList += files
        else:
            ParseDefines.append(files)
