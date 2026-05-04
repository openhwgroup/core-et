
from core.mtg import *
from core.priv.priv import MtoSMode, MtoUMode, StoMMode, StoUMode, UtoMMode, UtoSMode

@param_block
class SetRandPrivFromM(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None):
      comps = [MtoSMode, MtoUMode]
      self.rand_priv = test_helper.pick_one(comps)(test_helper=test_helper)
      self.condition = None
      self.link = None
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.rand_priv)
   def render_if(self, model: Model) -> bool:
      if self.link is None or self.link.condition is False:
         self.condition = model.curr_priv_mode == "m"
      else:
         self.condition = False
      return self.condition

@param_block
class SetRandPrivFromS(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None):
      comps = [StoMMode, StoUMode]
      self.rand_priv = test_helper.pick_one(comps)(test_helper=test_helper)
      self.condition = None
      self.link = None
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.rand_priv)
   def render_if(self, model: Model) -> bool:
      if self.link is None or self.link.condition is False:
         self.condition = model.curr_priv_mode == "s"
      else:
         self.condition = False
      return self.condition

@param_block
class SetRandPrivFromU(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None):
      comps = [UtoMMode, UtoSMode]
      self.rand_priv = test_helper.pick_one(comps)(test_helper=test_helper)
      self.condition = None
      self.link = None
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.rand_priv)
   def render_if(self, model: Model) -> bool:
      if self.link is None or self.link.condition is False:
         self.condition = model.curr_priv_mode == "u"
      else:
         self.condition = False
      return self.condition

@register
class SetRandPriv(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None):
      self.from_modes = [SetRandPrivFromU(test_helper=test_helper), SetRandPrivFromS(test_helper=test_helper), SetRandPrivFromM(test_helper=test_helper)]
      test_helper.shuffle(self.from_modes)
      self.from_modes[1].link = self.from_modes[0]
      self.from_modes[2].link = self.from_modes[1]

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      for i in self.from_modes:
         content_list.append(i)

if TEST_CONF.DEBUG_PRINTS:
   TEST_CONF.EXCL_COMPS.add(SetRandPrivFromM)
   TEST_CONF.EXCL_COMPS.add(SetRandPrivFromS)
   TEST_CONF.EXCL_COMPS.add(SetRandPrivFromU)
   TEST_CONF.EXCL_COMPS.add(SetRandPriv)

@register
class JumpToMMode(Component, AsmCode):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append("jump_to_m_mode:")
      content_list.append(StoMMode(test_helper=test_helper))
      content_list.append(UtoMMode(test_helper=test_helper))

@register
class JumpToSMode(Component, AsmCode):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append("jump_to_s_mode:")
      content_list.append(MtoSMode(test_helper=test_helper))
      content_list.append(UtoSMode(test_helper=test_helper))

@register
class JumpToUMode(Component, AsmCode):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append("jump_to_u_mode:")
      content_list.append(MtoUMode(test_helper=test_helper))
      content_list.append(StoUMode(test_helper=test_helper))
