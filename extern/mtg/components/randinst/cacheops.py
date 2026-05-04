
from core.mtg import *
from components.encoding.enc import *
from components.randinst.randinst_base import RandinstBase
from components.tensor.tensor_mask import ReadTensorMask

@register
class MCACHE_CONTROL(CSRRWI): pass

@register
class UCACHE_CONTROL(CSRRWI): pass

@register
class EVICT_SW(CSRRW):
   def _run(self, content_list: list, test_helper: TestHelper = None):
      if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6537'] == True:
         content_list.append(ReadTensorMask(rd=ABIreg('zero')))
      super()._run(content_list, test_helper=test_helper)

@register
class FLUSH_SW(CSRRW):
   def _run(self, content_list: list, test_helper: TestHelper = None):
      if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6537'] == True:
         content_list.append(ReadTensorMask(rd=ABIreg('zero')))
      super()._run(content_list, test_helper=test_helper)

if TEST_CONF.EXTRA_CHECKS == 1:
   TEST_CONF.EXCL_COMPS.add(FLUSH_SW)

@register
class LOCK_SW(CSRRW): pass

@register
class UNLOCK_SW(CSRRW): pass

@register
class EVICT_VA(CSRRW_X31):
   def _run(self, content_list: list, test_helper: TestHelper = None):
      if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6537'] == True:
         content_list.append(ReadTensorMask(rd=ABIreg('zero')))
      super()._run(content_list, test_helper=test_helper)

@register
class FLUSH_VA(CSRRW_X31):
   def _run(self, content_list: list, test_helper: TestHelper = None):
      if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6537'] == True:
         content_list.append(ReadTensorMask(rd=ABIreg('zero')))
      super()._run(content_list, test_helper=test_helper)

if TEST_CONF.EXTRA_CHECKS == 1:
   TEST_CONF.EXCL_COMPS.add(FLUSH_VA)

@register
class PREFETCH_VA(CSRRW_X31):
   def _run(self, content_list: list, test_helper: TestHelper = None):
      if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6537'] == True:
         content_list.append(ReadTensorMask(rd=ABIreg('zero')))
      super()._run(content_list, test_helper=test_helper)

@register
class LOCK_VA(CSRRW_X31):
   def _run(self, content_list: list, test_helper: TestHelper = None):
      if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6537'] == True:
         content_list.append(ReadTensorMask(rd=ABIreg('zero')))
      super()._run(content_list, test_helper=test_helper)

@register
class UNLOCK_VA(CSRRW_X31):
   def _run(self, content_list: list, test_helper: TestHelper = None):
      if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6537'] == True:
         content_list.append(ReadTensorMask(rd=ABIreg('zero')))
      super()._run(content_list, test_helper=test_helper)

@register
class CacheOps(RandinstBase):

   subcls = [EVICT_SW, FLUSH_SW, UNLOCK_SW]

   # Exclusions: PREFETCH_VA, LOCK_VA, UNLOCK_VA, LOCK_SW
   # Excluded due to X31 conflicts: EVICT_VA, FLUSH_VA

   @machine_code
   def _run(self, content_list: list, test_helper: TestHelper = None):
      super()._run(content_list, test_helper=test_helper)
