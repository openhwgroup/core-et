
import re
from core.snippet.snippet import Snippet
from core.test_conf import TEST_CONF, MTG_HANDLER
from core.models.model import Stack

class Asm(Snippet):

   _snip_head = "__asm__ __volatile__("
   _snip_tail = ");"
   _re_findall = re.compile(r'(?<=")(.*)(?=:)')

   class Code:
      def __init__(self):
         self.__code = ""
         self.__has_code = False
      def add_snip_head(self, snip_head: str) -> None:
         self.__code += f"{snip_head}\n"
      def add_snip_tail(self, snip_tail: str) -> None:
         self.__code += f"{snip_tail}\n"
      def __iadd__(self, other):
         self.__has_code = True
         self.__code += other
         return self
      def __eq__(self, other):
        if self.__has_code:
           return self.__code == other
        else:
           return None == other
      def __repr__(self) -> str:
         return repr(self.__code)
      def __str__(self) -> str:
         return str(self.__code)

   def _get_mtg_handler(self, content: str, hint: str) -> str:
      tmp = content.split(f"   \"start {hint}\\n\"")
      handler_code = None
      if len(tmp) == 2:
         tmp = tmp[1].split(f"   \"end {hint}\\n\"")
         handler_code = self._snip_head + tmp[0] + self._snip_tail
      return handler_code

   def get_comp_phases(self, content: list):
      code = [self.Code(), self.Code(), self.Code(), self.Code()]
      mhandler_code = 0
      shandler_code = 1
      uhandler_code = 2
      run_code = 3
      curr_code = Stack()
      curr_code.push(run_code)

      for line in content.split('\n'):
         if self._snip_head in line:
            for c in range(len(code)):
               code[c].add_snip_head(self._snip_head)
         elif self._snip_tail in line:
            for c in range(len(code)):
               code[c].add_snip_tail(self._snip_tail)
         elif MTG_HANDLER.M_HINT in line:
            if "start " in line:
               curr_code.push(mhandler_code)
            elif "end " in line:
               t = curr_code.pop()
               assert t == mhandler_code
            else:
               raise ValueError(f"Unexpected line: {line}")
         elif MTG_HANDLER.S_HINT in line:
            if "start " in line:
               curr_code.push(shandler_code)
            elif "end " in line:
               t = curr_code.pop()
               assert t == shandler_code
            else:
               raise ValueError(f"Unexpected line: {line}")
         elif MTG_HANDLER.U_HINT in line:
            if "start " in line:
               curr_code.push(uhandler_code)
            elif "end " in line:
               t = curr_code.pop()
               assert t == uhandler_code
            else:
               raise ValueError(f"Unexpected line: {line}")
         else:
            code[curr_code.top()] += f"{line}\n"

      return code[run_code], code[mhandler_code], code[shandler_code], code[uhandler_code]

   def _post_processing(self, content: str, path: str, prefix: str = None, suffix: str = None):
      prefix = f"{prefix}_{self.name}_{suffix}"
      labels = re.findall(self._re_findall, content)
      for i in labels:
         if i not in TEST_CONF.STATIC_LABELS:
            content = re.sub(i, f'{prefix}_{i}', content)
      return content

   def format_comment(self, comment: str) -> str:
      return f"/* {comment} */"

class AsmCode:
   _snip_class = Asm

# This is just a Stub
class CCode:
   pass
