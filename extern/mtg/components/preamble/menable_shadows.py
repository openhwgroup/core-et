
from core.mtg import *

@register
class MEnableShadows(Component, AsmCode):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append("csrwi menable_shadows, 0x1")
