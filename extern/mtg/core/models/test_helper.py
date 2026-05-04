
import random

class TestHelper:

   def __init__(self, seed: int = 0):
      self.__random_gen = random.Random()
      self.__random_gen.seed(seed)

   # Return a random integer N such that a <= N <= b
   def get_rand_range(self, start, stop):
      assert(start <= stop)
      ret = self.__random_gen.randint(start, stop)
      return ret

   # Returns a non-negative Python integer with K random bits.
   def get_rand_kbits(self, k):
      ret = self.__random_gen.getrandbits(k)
      return ret

   # Returns a signed Python integer with K random bits (bit K-1 represents the sign).
   def get_rand_signed_kbits(self, k):
      ret = self.__random_gen.getrandbits(k)
      sign = ret >> (k-1)
      if sign != 0:
         sign_mask = (1 << (k-1)) - 1
         ret = (ret & sign_mask) * -1
      return ret

   # Choose a random item from a sequence. The sequence can be a list, tuple, string, or any iterable like range
   def pick_one(self, iterable_list, exclude=[]):
      assert isinstance(iterable_list, list), f"TestHelper.pick_one() only accepts lists. Provided {type(iterable_list)}"
      avail = [x for x in iterable_list if x not in exclude]
      ret = self.__random_gen.choice(avail)
      return ret

   # Choose a random item from a sequence with weights. The sequence can be a list, tuple, string, or any iterable like range
   def pick_one_weight(self, iterable_list, weights, exclude=[]):
      assert len(iterable_list) == len(weights)
      elems = list()
      elem_weights = list()
      for i in range(len(iterable_list)):
         if iterable_list[i] not in exclude:
            elems.append(iterable_list[i])
            elem_weights.append(weights[i])
      assert len(elems) != 0, "Cannot pick one from an empty list"
      ret = self.__random_gen.choices(elems, weights = elem_weights)
      return ret[0]

   # Returns a Python unsigned integer with 64 random bits.
   def get_urand_num(self):
      ret = self.get_rand_kbits(64)
      return ret

   # Returns a Python signed integer with 64 random bits.
   def get_rand_num(self):
      ret = self.get_rand_signed_kbits(64)
      return ret

   def shuffle(self, elems : list):
      self.__random_gen.shuffle(elems)
