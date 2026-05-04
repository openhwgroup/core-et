from core.mtg import *

@register
class ConstrainRegs(Component, AsmCode):

   num_excl_int_regs = 3
   int_opt_out_regs  = [5, 6, 10, 11, 31]
   excl_int_regs     = []

   num_excl_fp_regs  = 6
   fp_opt_out_regs   = []
   excl_fp_regs      = []

   num_excl_m_regs   = 3
   m_opt_out_regs    = [0]
   excl_m_regs       = []

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def excl_regs(self, model, regs) -> None:
      for i in regs:
         model.exclude_reg(i)

   def reset_regs(self, model, regs) -> None:
      for i in regs:
         model.reset_reg(i)

   def get_victim_regs(self, test_helper, model, opt_out_regs: list, num_regs: int) -> list:
      victims = []
      avail_regs = [x.reg_num for x in model.get_avail_regs()]
      assert len(avail_regs) != 0, f"ConstrainRegs: failed to constrain {num_regs} registers ({model.__class__.__name__}). There are no free resources to constrain."
      for _ in range(num_regs):
         try:
            victim = test_helper.pick_one(avail_regs, exclude=opt_out_regs)
            victims.append(victim)
            opt_out_regs.append(victim)
         except IndexError as err:
            model.print_all()
            raise IndexError(f"ConstrainRegs: failed to constrain {num_regs} registers ({model.__class__.__name__}). avail_regs: {avail_regs}, opt_out_regs: {opt_out_regs}") from err
      return victims

   def constrain_regs(self, test_helper, model, regs, opt_out_regs: list, num_regs: int, prev_excl_regs: int) -> list:
      disjoint = test_helper.get_rand_range(0, 1)

      avail_regs = [x.reg_num for x in model.get_avail_regs()]
      num_avail_regs = len(avail_regs)
      if num_avail_regs < num_regs:
         if prev_excl_regs == 0:
            disjoint = -1
         else:
            disjoint = 0

      victims = list()
      if disjoint == 1:
         victims = self.get_victim_regs(test_helper, model, opt_out_regs, num_regs)
         self.reset_regs(model, regs)
         assert set(victims).isdisjoint(regs)
      elif disjoint == 0:
         self.reset_regs(model, regs)
         victims = self.get_victim_regs(test_helper, model, opt_out_regs, num_regs)

      self.excl_regs(model, victims)
      return victims

   def render(self, model: Model) -> Snippet:
      cls = self.__class__
      test_helper = model.test_helper

      ## Integer ##
      opt_out_regs = self.int_opt_out_regs.copy()
      num_regs     = self.num_excl_int_regs
      res_model    = model.integer
      cls.excl_int_regs = self.constrain_regs(test_helper, res_model, cls.excl_int_regs, opt_out_regs, num_regs, len(self.excl_int_regs))

      if TEST_CONF.RTLMIN_WORKAROUNDS['fv-260'] == False:
         ## Floating-Point ##
         opt_out_regs = self.fp_opt_out_regs.copy()
         num_regs     = self.num_excl_fp_regs
         res_model    = model.floatpoint
         cls.excl_fp_regs = self.constrain_regs(test_helper, res_model, cls.excl_fp_regs, opt_out_regs, num_regs, len(self.excl_fp_regs))

      # TODO: do not constrain mask regs due to conflict with MOVA.M.X and MOVA.X.M operations
      ## Mask ##
      #opt_out_regs = self.m_opt_out_regs.copy()
      #num_regs     = self.num_excl_m_regs
      #res_model    = model.mask
      #cls.excl_m_regs = self.constrain_regs(test_helper, res_model, cls.excl_m_regs, opt_out_regs, num_regs)

      return super().render(model)
