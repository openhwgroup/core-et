
from core.component_factory import register, mtg_dataclass, get_component
from core.component import Component
from core.snippet.asm import AsmCode
from dataclasses import dataclass, field
from core.models.model import *

@mtg_dataclass
class MHARTID(Component, AsmCode):
   rd: Xreg = field(default_factory=Xreg)
   link = None
   condition = None
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append('csrr {rd}, mhartid\n')

   def render_if(self, model: Model) -> bool:
      if self.link == None:
         if model.curr_priv_mode == "m":
            self.condition = model.test_helper.pick_one([True, True, False])
         else:
            self.condition = False
      else:
         self.condition = self.link.condition == False
      return self.condition

@mtg_dataclass
class HARTID(Component, AsmCode):
   rd: Xreg = field(default_factory=Xreg)
   link = None
   condition = None
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append('csrr {rd}, hartid\n')

   def render_if(self, model: Model) -> bool:
      if self.link == None:
         self.condition = True
      else:
         self.condition = self.link.condition == False
      return self.condition

@register
class ReadHartID(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None, rd: Xreg = None) -> None:
      if rd is None:
         self.rd = Xreg()
         assert False
      else:
         self.rd = rd

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      self.mhartid = MHARTID(test_helper=test_helper, rd=self.rd)
      self.hartid  = HARTID(test_helper=test_helper, rd=self.rd)
      self.hartid.link = self.mhartid

      content_list += [self.mhartid, self.hartid]
