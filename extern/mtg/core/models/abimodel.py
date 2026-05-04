
from core.models.resource import Resource, VirtualResource

class ABIreg(Resource):

   # Remember to update BackupABI and RestoreABI Components
   # if new ABI registers are added
   regs = {'zero': (0, 'Hard-wired zero'),
           'ra'  : (1, 'Return address'),
           'sp'  : (2, 'Stack pointer'),
           'gp'  : (3, 'Global pointer'),
           'tp'  : (4, 'Thread pointer'),
           'gamotp' : (18, 'Global AMO Thread pointer'),
           'lamotp' : (19, 'Local AMO Thread pointer'),
           'mhandler': (20, 'Machine exception handler pointer'),
           'shandler': (21, 'Supervisor exception handler pointer'),
           'uhandler': (22, 'User exception handler pointer'),
           'lhid': (27,'Logical HART ID'),
          }

   def __init__(self, name: str, ignore_check = False):
      super().__init__()
      self.name = name
      self.reserve()
      abi_reg = self.regs[name]
      self.reg_num = abi_reg[0]
      self.reg_info = abi_reg[1]
      self.ignore_check = ignore_check

   def sanity_check(self, model: 'Model') -> None:
      if not self.ignore_check:
         if self.name == 'gp':
            assert model.gp_is_set == True
         elif self.name == 'tp':
            assert model.tp_is_set == True
         elif self.name == 'gamotp':
            assert model.gamotp_is_set == True
         elif self.name == 'lamotp':
            assert model.lamotp_is_set == True
         elif self.name == 'lhid':
            assert model.lhid_is_set == True

   def __repr__(self) -> str:
      return f"x{self.reg_num}: {self.reg_info}"

   def __str__(self) -> str:
      return f'x{self.reg_num}'

class FPABIreg(ABIreg):

   # Remember to update BackupABI and RestoreABI Components
   # if new ABI registers are added
   regs = {'amo' : (26, 'PRM-7 AMO aligned to 32b'),
           'off' : (27, 'PRM-5, PRM-6 mem offset'),
          }

   def sanity_check(self, model: 'Model') -> None:
      if not self.ignore_check:
         if self.name == 'amo':
            assert model.fp_amo_is_set == True
         elif self.name == 'off':
            assert model.fp_off_is_set == True

   def __repr__(self) -> str:
      return f"f{self.reg_num}: {self.reg_info}"

   def __str__(self) -> str:
      return f'f{self.reg_num}'
