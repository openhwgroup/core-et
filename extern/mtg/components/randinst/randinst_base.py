
from core.mtg import *

@register
class RandinstBase(Component, AsmCode):

   subcls = []

   def __init__(self, test_helper: TestHelper = None):
      self.subcls = [x for x in self.subcls if x.exclude == False]
      self.insn = test_helper.pick_one(self.subcls)(test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn)
