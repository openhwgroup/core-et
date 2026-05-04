# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
Python and its json writer and even packages (jsonlines) are so lame, we need to write our own to
get something that works.
"""


class JsonWriter:
    def __init__(self, filename, obj):
        self.__filename = filename
        self.__ofs = open(filename, "w+t")
        self.__obj = obj

    def write(self):
        self.__write(self.__obj)
        self.__ofs.close()

    def __write(self, obj):
        if isinstance(obj, str):
            nobj = obj.replace('\\',r'\\').replace('\n', '\\n').replace('\"',r'\\\"');
            self.__ofs.write('"' + nobj + '"');
        elif isinstance(obj, list):
            self.__ofs.write('[')
            n = len(obj)
            for e in obj:
                self.__write(e)
                n -= 1
                if 0 < n: self.__ofs.write(',')
            self.__ofs.write(']')
        elif isinstance(obj, dict):
            self.__ofs.write('{')
            n = len(obj)
            for k, v in obj.items():
                self.__ofs.write('"' + k + '": ')
                self.__write(v)
                n -= 1
                if 0 < n: self.__ofs.write(',')
                self.__ofs.write('\n')
            self.__ofs.write('}')
        else:
            if not isinstance(obj, str): obj = str(obj)
            self.__ofs.write(obj)
