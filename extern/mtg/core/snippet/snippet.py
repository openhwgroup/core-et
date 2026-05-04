
class Snippet:

   _snip_head = ""
   _snip_tail = ""

   def __init__(self, name: str, content: str = "") -> None:
      # Instance Attributes
      self.__name = name
      if not content:
         self._content = []
      else:
         self._content = [content]

   @property
   def name(self) -> str:
      return self.__name

   def format_comment(self, comment: str) -> str:
      return f"#{comment}"

   def set_content(self, content: str) -> None:
      self._content = [content]

   def get_content(self, prev = type(None), prefix = "", last_snip = True) -> str:
      title = self.format_comment(self.name)
      body = ""
      for i in self._content:

         if isinstance(i, str):
            my_type = type(self)
         else:
            my_type = type(i)

         if my_type != prev:
            if prev is not type(None):
               body += f"{prev._snip_tail}"
            body += f"{my_type._snip_head}\n"

         prev = my_type

         if isinstance(i, Snippet):
            body += f"{i.get_content(prev = prev, prefix = '   ', last_snip = False)}\n"
         else:
            ll = filter(None, i.split("\n"))
            l = list()
            for line in ll:
               if line[0] == "#":
                  l.append(line)
               else:
                  l.append(f"   \"{line}\\n\"")
            l = "\n".join(l)
            body += f"{l}\n"

      if last_snip and prev is not type(None):
         body += prev._snip_tail

      ret = f"\n{prefix}{title}\n{body}"
      return ret

   def get_export_format(self, path, prefix = None, suffix = None):
      if prefix is None:
         prefix = ""
      else:
         prefix = f'{prefix}_'

      if suffix is None:
         suffix = ""
      else:
         suffix = f'_{suffix}'

      return f"{path}/{prefix}{self.name}{suffix}.h"

   def _post_processing(self, content: str, path: str, prefix: str = None, suffix: str = None):
      return content

   def get_comp_phases(self, content: list):
      return content, None

   def export_content(self, path: str, prefix: str = None, suffix: str = None):

      content = self.get_content()
      content = self._post_processing(content, path, prefix=prefix, suffix=suffix)

      run_code, mhandler_code, shandler_code, uhandler_code  = self.get_comp_phases(content)

      output_filename = self.get_export_format(path, prefix=prefix, suffix=suffix)
      f = open(output_filename,"w+")
      f.write(str(run_code))

      def export_handler_code(handler_code, hsuffix):
         if handler_code != None:
            output_filename = self.get_export_format(path, prefix=prefix, suffix=suffix+"_"+hsuffix)
            f = open(output_filename,"w+")
            f.write(str(handler_code))

      export_handler_code(mhandler_code, "mhandler")
      export_handler_code(shandler_code, "shandler")
      export_handler_code(uhandler_code, "uhandler")
      f.close()

   def __iadd__(self, other: 'Snippet') -> None:
      self._content.append(other)
      return self

   def __repr__(self) -> str:
      return str((self.name, self._content))

   def __str__(self) -> str:
      return str((self.name, self._content))
