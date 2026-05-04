
from core.mtg import *
from components.randinst.mask import MOVA_X_M

@register
class MovaXM(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      self.rx0 = Xreg()

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(MOVA_X_M(test_helper = test_helper))
