#!/usr/bin/env python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0


from __future__ import print_function
from collections import namedtuple
import re, os, sys

TOKENS = [
    # Ignored
    ('whitespace', re.compile(r'\s+')),
    ('comment',    re.compile(r'#.*')),
    # Literal
    ('event',      re.compile(r'event')),
    ('const',      re.compile(r'const')),
    ('module',     re.compile(r'module')),
    ('{',          re.compile(r'{')),
    ('}',          re.compile(r'}')),
    ('[',          re.compile(r'\[')),
    (']',          re.compile(r']')),
    (':',          re.compile(r':')),
    (',',          re.compile(r',')),
    ('=',          re.compile(r'=')),
    (';',          re.compile(r';')),
    ('(',          re.compile(r'\(')),
    (')',          re.compile(r'\)')),
    ('mul_op',     re.compile(r'[\*/%]')),
    ('add_op',     re.compile(r'[\+\-]')),
    # Primary
    ('type',       re.compile(r'(u8|u16|u32|u64|i8|i16|i32|i64|f32|f64|bool)\b')),
    ('identifier', re.compile(r'[a-zA-Z_][a-zA-Z0-9_]*')),
    ('number',     re.compile(r'[0-9]+')),
]

IGNORED_TOKENS = ['whitespace', 'comment']

Token = namedtuple('Token', ['name', 'value'])

FilePos = namedtuple('FilePos', ['filename', 'lineno', 'column', 'text'])

RESERVED = ['event_id', 'event', 'const', 'module']

class EventSyntaxError(Exception):
    def __init__(self, filepos, message):
        self.filepos = filepos
        self.message = message
    def __str__(self):
        (filename, lineno, column, text) = self.filepos
        if text == None:
            return '{}:eof: {}'.format(filename, self.message)
        else:
            return '{}:{}:{}: {}\n{}\n{}^'.format(filename, lineno, column, self.message, text, " " * (column-1))

class TokenStream(object):
    def __init__(self, fname):
        def tokenize():
            with open(fname, 'r') as fp:
                def next_line(pos):
                    text = fp.readline()
                    line = None if text == '' else text.strip()
                    return FilePos(fname, pos.lineno+1, 1, line)
                def next_token(pos):
                    for name, pattern in TOKENS:
                        match = pattern.match(pos.text, pos.column - 1)
                        if match:
                            return FilePos(pos.filename, pos.lineno, match.span()[1]+1, pos.text), Token(name, match.group())
                    raise EventSyntaxError(pos, 'unrecognized token')
                pos = next_line(FilePos(fname, 0, 0, ''))
                while pos.text != None:
                    while pos.column <= len(pos.text):
                        new_pos, tkn = next_token(pos)
                        if tkn.name not in IGNORED_TOKENS:
                            yield pos, tkn
                        pos = new_pos
                    pos = next_line(pos)
                yield pos, Token(None, None)
        self.tkns = tokenize()
        self.head = next(self.tkns)
    @property
    def pos(self):
        return self.head[0]
    def consume(self):
        self.head = next(self.tkns)
    def peek(self, tkn_name):
        pos, tkn = self.head
        if tkn.name == tkn_name: return pos, tkn
    def expect(self, tkn_name):
        pos, tkn = self.head
        if tkn.name != tkn_name:
            raise EventSyntaxError(pos, 'expected {}'.format(tkn_name))
        self.consume()
        return pos, tkn

Type = namedtuple('Type', ['name', 'cpp', 'sv'])
TYPES = {
    'u8':   Type('u8',   'uint8_t',   'byte unsigned'),
    'i8':   Type('i8',   'int8_t',    'byte'),
    'u16':  Type('u16',  'uint16_t',  'shortint unsigned'),
    'i16':  Type('i16',  'int16_t',   'shortint'),
    'u32':  Type('u32',  'uint32_t',  'int unsigned'),
    'i32':  Type('i32',  'int32_t',   'int'),
    'u64':  Type('u64',  'uint64_t',  'longint unsigned'),
    'i64':  Type('i64',  'int64_t',   'longint'),
    'f32':  Type('f32',  'float32_t', 'shortreal'),
    'f64':  Type('f64',  'float64_t', 'real'),
    'bool': Type('bool', 'bool',      'bit'),
}

Field = namedtuple('Field', ['name', 'type', 'width'])
Event = namedtuple('Event', ['name', 'id', 'tag', 'fields'])
Doc = namedtuple('Doc', ['module', 'events'])

Const = namedtuple('Const', ['name', 'value'])
Expr = namedtuple('Expr', ['type', 'lhs', 'rhs'])

def evaluate(expr):
    if expr.type == 'value':
        return expr.lhs
    elif expr.type == '+':
        return evaluate(expr.lhs) + evaluate(expr.rhs)
    elif expr.type == '-':
        return evaluate(expr.lhs) - evaluate(expr.rhs)
    elif expr.type == '*':
        return evaluate(expr.lhs) * evaluate(expr.rhs)
    elif expr.type == '/':
        return int(evaluate(expr.lhs) / evaluate(expr.rhs))
    elif expr.type == '%':
        return evaluate(expr.lhs) % evaluate(expr.rhs)

class EventParser(object):
    def parse(self, fname):
        stream = TokenStream(fname)
        doc = self.parse_doc(stream)
        return doc
    def parse_doc(self, stream):
        mname = self.parse_module(stream)
        events = []
        consts = {}
        while not stream.peek(None):
            if stream.peek('const'):
                (name, value) = self.parse_const(stream, consts)
                consts[name] = value
            elif stream.peek('event'):
                evt = self.parse_event(stream, events, consts)
                events.append(evt)
            else:
                pos, tkn = stream.consume()
        return Doc(mname, events)
    def parse_module(self, stream):
        stream.expect('module')
        pos, (_, mname) = stream.expect('identifier')
        stream.expect(';')
        return mname
    def parse_event(self, stream, events, consts):
        stream.expect('event')
        pos, (_, name) = stream.expect('identifier')
        if name in [evt.name for evt in events]:
            raise EventSyntaxError(pos, 'duplicate event {}'.format(name))
        if name in RESERVED:
            raise EventSyntaxError(pos, 'reserved {}'.format(name))
        tag = self.parse_tag(stream) if stream.peek('[') else None
        stream.expect('{')
        fields = []
        while stream.peek('identifier'):
            fld = self.parse_field(stream, fields, consts)
            fields.append(fld)
            if not stream.peek('}'): stream.expect(',')
        stream.expect('}')
        return Event(name, len(events)+1, tag, fields)
    def parse_tag(self, stream):
        stream.expect('[')
        pos, (_, tag) = stream.expect('identifier')
        stream.expect(']')
        return tag
    def parse_field(self, stream, fields, consts):
        pos, (_, name) = stream.expect('identifier')
        if name in [fld.name for fld in fields]:
            raise EventSyntaxError(pos, 'duplicate field {}'.format(name))
        if name in RESERVED:
            raise EventSyntaxError(pos, 'reserved identifier {}'.format(name))
        stream.expect(':')
        typ, width = self.parse_type(stream, consts)
        return Field(name, typ, width)
    def parse_type(self, stream, consts):
        pos, (_, name) = stream.expect('type')
        width = 1
        if stream.peek('['):
            stream.consume()
            width = evaluate(self.parse_expr(stream, consts))
            stream.expect(']')
        return TYPES[name], width
    def parse_const(self, stream, consts):
        stream.expect('const')
        pos, (_, name) = stream.expect('identifier')
        if name in [con.name for con in consts]:
            raise EventSyntaxError(pos, 'duplicate const {}'.format(name))
        if name in RESERVED:
            raise EventSyntaxError(pos, 'reserved identifier {}'.format(name))
        stream.expect('=')
        value = evaluate(self.parse_expr(stream, consts))
        stream.expect(';')
        return Const(name, value)
    def parse_expr(self, stream, consts):
        lhs = self.parse_mul_expr(stream, consts)
        if stream.peek('add_op'):
            _, (_, op) = stream.expect('add_op')
            rhs = self.parse_mul_expr(stream, consts)
            return Expr(op, lhs, rhs)
        return lhs
    def parse_mul_expr(self, stream, consts):
        lhs = self.parse_prim_expr(stream, consts)
        if stream.peek('mul_op'):
            _, (_, op) = stream.expect('mul_op')
            rhs = self.parse_expr(stream, consts)
            return Expr(op, lhs, rhs)
        return lhs
    def parse_prim_expr(self, stream, consts):
        if stream.peek('identifier'):
            pos, (_, name) = stream.expect('identifier')
            if name not in consts.keys():
                raise EventSyntaxError(pos, 'undefined value {}'.format(name))
            return Expr('value', consts[name], None)
        elif stream.peek('number'):
            _, (_, num) = stream.expect('number')
            return Expr('value', int(num), None)
        elif stream.peek('('):
            stream.consume()
            expr = self.parse_expr(stream, consts)
            stream.expect(')')
        else:
            raise EventSyntaxError('expected expression')

if __name__ == '__main__':
    from argparse import ArgumentParser
    from mako.template import Template
    cmdline = ArgumentParser(description='Parse event file to generate C++/SV headers')
    cmdline.add_argument('event_file', help='Definition of events (.evt)')
    cmdline.add_argument('template_file', help='Mako template file')
    args = cmdline.parse_args()
    tmpl = Template(filename=args.template_file)
    parser = EventParser()
    try:
        doc = parser.parse(args.event_file)
        print(tmpl.render(**{'module': doc.module, 'events': doc.events}))
    except EventSyntaxError as err:
        print(err, file=sys.stderr)

