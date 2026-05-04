from core.models.test_helper import TestHelper
from core.models.tensormodel import set_bits, get_bits, randomize_scp_lines, TensorEncoding
from core.test_conf import TEST_CONF
from core.models.model import get_mask_bits, get_mask_from_bits
import itertools

def get_all_combinations(values: list) -> list:
   all_combs = list()
   for r in range(len(values)+1)[1:]:
      all_combs += list(itertools.combinations(values, r))
   return all_combs

def get_active_neigh_minions(mask: int) -> dict:
   active_neigh_minions = dict()
   nid = 0
   for i in range(4):
      active_neigh_minions[i] = mask & 0xff
      nid += 1
      mask >>= 8
   assert mask == 0
   return active_neigh_minions

def get_neigh_combinations(active_neigh_minions: dict) -> list:
   tcoop_neighs = list()
   for k, v in active_neigh_minions.items():
      if v > 0:
         tcoop_neighs.append(k)
   return get_all_combinations(tcoop_neighs)

def get_minion_combinations(minions_mask: int) -> list:
   tcoop_minions = get_mask_bits(minions_mask)
   return get_all_combinations(tcoop_minions)

def get_all_tensor_coop_combinations(shire_minion_mask):
   tcoop_valid_combs = list()
   active_neigh_minions = get_active_neigh_minions(shire_minion_mask)
   all_neigh_combs = get_neigh_combinations(active_neigh_minions)

   for neighs in all_neigh_combs:
      minion_mask = 0xff
      for nid in neighs:
         minion_mask &= active_neigh_minions[nid]

      if minion_mask > 0:
         neigh_mask = get_mask_from_bits(neighs)
         all_minion_combs = get_minion_combinations(minion_mask)
         for minions in all_minion_combs:
            minion_mask = get_mask_from_bits(minions)
            tcoop_valid_combs.append((hex(neigh_mask), hex(minion_mask)))
   return tcoop_valid_combs

tcoop_valid_combs = get_all_tensor_coop_combinations(TEST_CONF.MINION_MASK)

tcoop_valid_coop_ids = set(range(16))

class TensorCoopEncoding(TensorEncoding):

   tensor_coop = 0x0
   excl_id = set()

   def __init__(self, value = None):
      self.name = self.get_name()
      super().__init__(value = value)

   @classmethod
   def get_name(cls) -> str:
      return f'TCOOP'

   @classmethod
   def free_resources(cls, model: 'Model', name: str) -> None:
      pass

   def get_encoding(self, model: 'Model') -> int:
      test_helper = model.test_helper
      tcoop_id = test_helper.pick_one(list(tcoop_valid_coop_ids), exclude=TensorCoopEncoding.excl_id)
      enc = test_helper.get_rand_num()
      (neigh_mask, minion_mask) = model.test_helper.pick_one(tcoop_valid_combs)
      enc = set_bits(enc, 19, 16, int(neigh_mask, 16))
      enc = set_bits(enc, 15,  8, int(minion_mask, 16))
      enc = set_bits(enc,  4,  0, tcoop_id)
      return enc

   def selfcheck_valid_combination(self, tcoop_csr_val: int) -> None:
      neigh_mask  = get_bits(tcoop_csr_val, 19, 16)
      minion_mask = get_bits(tcoop_csr_val, 15,  8)

      active_neighs = get_mask_bits(neigh_mask)
      mask = 0x0
      for nid in active_neighs:
         mask += minion_mask << (8*nid)

      assert (mask | TEST_CONF.MINION_MASK) == TEST_CONF.MINION_MASK

   @classmethod
   def must_reset_ids(cls):
      return cls.excl_id == tcoop_valid_coop_ids

   @classmethod
   def reset_ids(cls):
      return cls.excl_id.clear()

   def post_encoding(self, model: 'Model', tcoop_csr_val: int):
      # Double-check that tensor coop value matches with active minions
      self.selfcheck_valid_combination(tcoop_csr_val)

      # Update the tensor coop id model value
      tcoop_id = get_bits(tcoop_csr_val,  4,  0)
      assert tcoop_id not in TensorCoopEncoding.excl_id
      TensorCoopEncoding.excl_id.add(tcoop_id)

      # Update the tensor coop model value
      neigh_mask  = get_bits(tcoop_csr_val, 19, 16)
      minion_mask = get_bits(tcoop_csr_val, 15,  8)
      TensorCoopEncoding.tensor_coop = (neigh_mask << 8) | minion_mask
