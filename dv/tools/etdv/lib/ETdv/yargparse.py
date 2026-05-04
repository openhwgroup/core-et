# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

from collections import OrderedDict
from functools import reduce
from ETdv.util import AnyKeyDict
import re

"""
Another argparse, but better.
We support Verilog-ish things like options '+incdir+d1+d2+...'
"""


class YargParse:
    class ArgException(Exception):
        def __init__(self, msg):
            super().__init__(msg)

    class Rule:
        def __init__(self, opt_nm, help=None, validator=None, default=None, group=None, usage=None):
            """
            Create option rule.
            :param opt_nm: literal option name (like --opt).  Prefix with '^' to indicate that starts-with matching
                           is to be used, as in '^+incdir+'.
                           Specify short and long opt names using |, as in '-s|--long'.
                           Suffix name with one of @?, @+, @* or @!
                           to express repetition: 0 or 1, 1 or more, 0 or more, or required.
                           If option takes arguments, add space separated names, as '-dir name'.
                           If option has default, specify after option using '=default', as in 'option=default'.
                           If option has choices, provide '|' separated, as in: '-sim vcs|verilator'.
            :param help: Optional help message for usage.
            :param validator: Optional function/lambda to validate option.
                              Takes 3 arguments: (rule, option, value),
                              where rule is Rule for option and value as parsed.
                              Lambda returns None if ok, else error message.
            :param group: _Group instance.
            :param usage: verbatim usage (or None to automatically generate).
            """

            def raise_ill_formed(opt_nm):
                raise AssertionError("ill-formed: " + opt_nm)

            xopt_nm = opt_nm
            args = xopt_nm.split()
            choices = None
            if 1 < len(args):
                choices = args[1].split('|')
                if 1 < len(choices):
                    choices = [i for i in filter(None, choices)]  # handle case 'opt|'
                    if 2 < len(args): raise_ill_formed(opt_nm)
                    xopt_nm, args = args[0], re.sub(r'\s*', '', args[1])
                else:
                    choices, set_default = None, False
                    if 2 == len(args):
                        tmp = args[1].split('=')
                        if 2 == len(tmp):
                            xopt_nm, args, default = [args[0]] + tmp
                            set_default = True
                    if not set_default:
                        xopt_nm, args = args[0], args[1:]
            else:
                args = []
            if re.match(r'.*@[\?\+\*\!]$', xopt_nm):
                rep = xopt_nm[-1]
                xopt_nm = xopt_nm[:-2]
            else:
                rep = '?'
            self._starts_with = xopt_nm[0] == '^'
            # if self._starts_with and args is not None: raise_ill_formed(opt_nm)
            if self._starts_with:
                xopt_nm = xopt_nm[1:]
            self._opt_nm = xopt_nm.split('|')
            if type(args) is str: args = [args]
            self._args = args
            self._rep = rep
            self._help = help
            self._validator = validator
            self._choices = choices
            self._default = default
            assert group is None or isinstance(group, YargParse._Group)
            self._group = group
            self._usage = usage

        @property
        def group(self):
            return self._group

        @property
        def opt_nm(self):
            return self._opt_nm

        @property
        def choices(self):
            return self._choices

        @property
        def default(self):
            return self._default

        @property
        def starts_with(self):
            return self._starts_with

        @property
        def args(self):
            return self._args

        @property
        def rep(self):
            return self._rep

        @property
        def help(self):
            return self._help

        @property
        def usage(self):
            return self._usage

        @property
        def validator(self):
            return self._validator

    """
    A proxy with add() method to pass onto YargParse group.
    """

    class _Group:
        def __init__(self, parent, name, tool=None):
            self._name = name
            self._parent = parent
            self._tool = tool

        @property
        def name(self): return self._name

        @property
        def tool(self): return self._tool

        def add(self, rules, **kwargs):
            self._parent.add(rules, group=self, **kwargs)
            return self

    class _RuleVal:
        """
        Wrapper for Rule,val pair.
        """

        def __init__(self, rule):
            self._rule = rule
            self._val = []

        @property
        def rule(self): return self._rule

        @property
        def val(self):
            v = self._val
            if isinstance(v, list) and 1 == len(v): v = v[0]
            return v

        @val.setter
        def val(self, x):
            self._val.append(x)

    def set_option(self, opt_nm, val=[]):
        self._add_value(opt_nm, val)
        return self

    def __init__(self):
        self._group_info = OrderedDict()
        self._rules_by_group = OrderedDict()
        self._rule_by_name = AnyKeyDict()
        self._starts_with_rules = {}
        self._opt_pfx = '-+'
        self._argv = None
        self._found_opts = {}
        self._user_plusargs = []
        self._positional = []

    def __getitem__(self, optnm):
        """
        Implement [] operator for lookup.
        :param optnm: option name.
        :return: values.
        """
        if optnm in self._found_opts:
            val = self._found_opts[optnm].val
        elif optnm in self._user_plusargs:
            val = self._user_plusargs[optnm].val
        else:
            return None
        if isinstance(val, str):
            return val
        if not isinstance(val, list):
            val = [val]
        if 0 == len(val) or not isinstance(val[0], dict):
            return val
        val = [y[0] for y in [[x for x in v.values()] for v in val]]
        if isinstance(val, list) and 1 == len(val): val = val[0]
        return val

    @property
    def positional(self):
        return self._positional

    def usage_options(self):
        def get_nm(xrule):
            return re.sub(r'[^\w\d]', '', '|'.join(xrule.opt_nm)).lower()

        indent = ' ' * 3
        indentx2 = indent * 2
        description_col = 40
        usage = ""
        for group, info in self._group_info.items():
            if 0 < len(usage): usage += '\n'
            usage += info['title'] + ':\n'
            if info['description'] is not None: usage += indent + info['description'] + '\n\n'
            for rule in [self._get_rule(name.opt_nm) for name in sorted(self._rules_by_group[group], key=get_nm)]:
                line = indent
                if rule.usage:
                    line += rule.usage
                else:
                    if rule.rep != '!':
                        line += '['
                    line += '|'.join(rule.opt_nm)
                    if 0 < len(rule.args):
                        if rule.starts_with:
                            assert 1 == len(rule.args)
                            line += rule.args[0]
                        elif rule.choices:
                            line += ' ' + '|'.join(rule.choices)
                        else:
                            n = len(rule.args)
                            fmt = ' <{}>' * n
                            line += fmt.format(*rule.args)
                    if rule.rep != '!':
                        line += ']'
                        if rule.rep in ['*', '+']:
                            line += '...'
                if rule.help or rule.default:
                    n = description_col - len(line)
                    if 0 < n and len(indent) <= n:
                        line += ' ' * n
                    else:
                        line += indentx2
                    if rule.help: line += rule.help + '.  '
                    if rule.default: line += '(default: {})'.format(rule.default)
                line += '\n'
                usage += line
        return usage

    def add(self, rules, group=None, **kwargs):
        if group is None: group = self.add_group()
        assert isinstance(group, YargParse._Group)
        if isinstance(rules, YargParse.Rule):
            rules = [rules]
        elif type(rules) is str:
            return self._add_rule(opt_nm=rules, group=group, **kwargs)
        for rule in rules:
            self._rules_by_group[group.name] = self._rules_by_group.get(group.name, [])
            self._rules_by_group[group.name].append(rule)
            opt_nm = rule.opt_nm
            if rule.starts_with:
                if opt_nm in self._starts_with_rules: raise AssertionError('opt_nm=' + opt_nm)
                self._starts_with_rules.update({opt_nm: rule})
            else:
                if opt_nm in self._rule_by_name: raise AssertionError('opt_nm=' + opt_nm)
                self._rule_by_name[opt_nm] = rule
        return self

    def add_group(self, name='*', title=None, description=None, tool=None):
        if name in self._group_info: raise AssertionError("name '{}' exists".format(name))
        if title is None or 1 > len(title): raise AssertionError('title is required')
        self._group_info[name] = {'title': title, 'description': description}
        return YargParse._Group(self, name, tool)

    def _add_rule(self, opt_nm, group, **kwargs):
        assert group is None or isinstance(group, YargParse._Group)
        rule = YargParse.Rule(opt_nm, group=group, **kwargs)
        return self.add(rule, group)

    def parse(self, argv):
        """
        Parse argv against Option rules.
        :param argv:
        :return: [opts, user_opts, argv]
        where opts is dictionary of parsed arguments; uopts are user-plusargs and argv are remaining positional arguments.
        """
        self._argv = list(argv)  # working copy
        while 0 < len(self._argv):
            option = self._argv.pop(0)
            if option == '--':
                self._argv = self._argv[1:]
                break
            if option[0] not in self._opt_pfx:
                self._positional += [option]
            else:
                self._process(option)
        self._add_defaults()
        self._positional += self._argv
        return [self._found_opts, self._user_plusargs, self._positional]

    def _add_defaults(self):
        """
        Add default values.
        :return: None
        """
        for rule in list(self._rule_by_name.values()) + list(self._starts_with_rules.values()):
            if rule.default and rule.opt_nm[-1] not in self._found_opts:
                self._add_value(rule.opt_nm[-1], rule.default, rule)

    def _add_value(self, opt_nm, val, rule=None):
        """
        Add value to found_opts.
        :param opt_nm: option name (key).
        :param val: value to add.
        :return: self
        """
        if rule is None:
            rule = self._get_rule(opt_nm)
        assert rule
        if opt_nm not in self._found_opts:
            self._found_opts[opt_nm] = YargParse._RuleVal(rule)
        if isinstance(val, list):
            if 0 == len(val):
                val = None
            else:
                assert 1 == len(val)
                val = val[0]
        self._found_opts[opt_nm].val = val
        return self

    def _get_rule(self, opt_nm):
        if opt_nm in self._rule_by_name:
            return self._rule_by_name[opt_nm]
        for key, rule in self._starts_with_rules.items():
            if opt_nm.startswith(key):
                return rule
        return None

    def _process(self, option):
        def raise_arg_exception(fmt, *args):
            raise YargParse.ArgException(fmt.format(*args))

        rule = self._get_rule(option)
        if rule is None:
            if option.startswith('+'):
                self._user_plusargs += [option]
                return
            else:
                raise_arg_exception("'{}': invalid option", option)
        #always just take long option, presumably last
        if rule.opt_nm[-1] in self._found_opts and rule.rep == '?':
            raise_arg_exception("option '{}' was repeated", rule.opt_nm)
        if rule.starts_with and option in self._found_opts:
            # e.g. found: +libext+val ... +libext+val  (exact same starts-with value)
            raise_arg_exception("option '{}' was repeated", option)
        if rule.args is not None and not rule.starts_with:  # we do 'else' if we are a starts_with
            n = len(rule.args)
            args = self._argv[:n]
            del self._argv[:n]
            if len(args) != n:
                raise_arg_exception(
                    "option '{}' expected {} arg(s): found {}", option, n, len(args))
            if rule.choices is None:
                # list of key:value
                val = [{key: args.pop(0)} for key in rule.args]
            else:
                choice = args[0]
                if choice not in rule.choices:
                    raise_arg_exception("option '{}' to '{}' not in {}", choice, option, '|'.join(rule.choices))
                val = choice
        else:
            val = None
        if rule.validator:
            fail_message = rule.validator(rule, option, val)
            if fail_message: raise_arg_exception(fail_message)
        self._add_value(rule.opt_nm[-1], val, rule)

    @staticmethod
    def has_option(argv, options):
        if type(options) is str: options = [options]
        return reduce(lambda a, b: a or b, (map(lambda e: e in options, argv)))


# basic tests
if __name__ == '__main__':
    parser = YargParse()
    g = parser.add_group(name='group', title='title')
    g.add('-s|--long arg', help='a help message') \
    .add('-t', help='explain t option')
    #parsed = parser.parse('hello -opt1 bar +incdir+d1+d2'.split())
    print(parser.usage_options())
    pass
