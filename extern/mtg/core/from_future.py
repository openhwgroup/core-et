
# This class is a Place Holder
# It behaves like an Int, however, its value is set in the future
#
# One use-case is tensors, where the memory size is not known until the render stage
# however we need to pass something to the Malloc class, which is instantiated before the
# render stage.

class FromFuture:
   value = None

   def set_value(self, val: int):
      self.value = val
   def __add__(self,other):
      return self.value + other
   def __radd__(self,other):
      return self.value + other
   def __sub__(self,other):
      return self.value - other
   def __mul__(self,other):
      return self.value * other
   def __div__(self,other):
      return self.value / other
   def __mod__(self,other):
      return self.value % other
   def __lt__(self,other):
      return self.value < other
   def __le__(self,other):
      return self.value <= other
   def __gt__(self,other):
      return self.value > other
   def __ge__(self,other):
      return self.value >= other
   def __eq__(self,other):
      return self.value == other
   def __ne__(self,other):
      return self.value != other
   def __repr__(self):
      return str(self.value)
   def __str__(self):
      return str(self.value)
