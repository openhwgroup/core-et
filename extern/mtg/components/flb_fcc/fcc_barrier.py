
from core.mtg import *
from components.flb_fcc.flb_fcc_sync_threads import AMOFLBFCCSyncThreads, FLBFCCSyncShireThreads

if TEST_CONF.MINION_AGNOSTIC == 1 or TEST_CONF.NEIGH_AGNOSTIC == 1 or TEST_CONF.SHIRE_AGNOSTIC == 1:
   @register
   class FCCBarrier(FLBFCCSyncShireThreads):
      pass
else:
   @register
   class FCCBarrier(AMOFLBFCCSyncThreads):
      pass
