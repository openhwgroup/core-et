
from core.mtg import *

@register
class InitZero(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      pass

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append("add x0, x0, x0")
