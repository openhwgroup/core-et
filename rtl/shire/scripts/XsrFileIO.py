# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import re
import os
import errno

Indent = '   '

def printToFile (fileName, contents):
    with open(fileName, "w") as f:
        f.write(contents)

def findKey(contents, key):
    for i,c in enumerate(contents):
        if re.search(key,c):
            return i
    return None

def replaceInFile(fileName, newContents, startKey, endKey, addEOL = True):
    with open(fileName,"r") as f:
        contents = f.readlines()
    
    start = findKey(contents, startKey)
    end = findKey(contents, endKey)

    if  start == None or end == None:
        raise Exception("either startKey %s or endKey %s not found in %s" % 
                        ( startKey, endKey, fileName) )
    
    if ( end < start):
        raise Exception("occurrence of startKey %s after endKey %s in %s" %
                        ( startKey, endKey, fileName) )
    else:
        eol = '\n' if addEOL else ''
        printToFile(fileName,  ''.join(contents[:start+1]) + 
                    newContents + eol + ''.join(contents[end:]) )

def getFilePortion(fileName, startKey, endKey, allowNotFound = False):
    with open(fileName,"r") as f:
        contents = f.readlines()
    
    start = findKey(contents, startKey)
    end = findKey(contents, endKey)

    found = (start != None)
    found += (end != None)

    if found == 0 and allowNotFound:
        return ''

    if found < 2:
        raise Exception("either startKey %s or endKey %s not found in %s" % 
                        ( startKey, endKey, fileName) )
    
    if ( end < start):
        raise Exception("occurrence of startKey %s after endKey %s in %s" %
                        ( startKey, endKey, fileName) )
    else:
        return ''.join(contents[start+1:end])


def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc:
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise
