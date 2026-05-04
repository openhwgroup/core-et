import re
from abc import ABCMeta, abstractmethod
from typing import Callable, ClassVar
from dataclasses import dataclass, InitVar

from core.models.model import *
from core.snippet.snippet import Snippet
from functools import wraps

from core.rtl_workarounds_chk import check_rtlmin_workarounds


def var_name(self, p):
   for k, v in self.__dict__.items():
      if id(p) == id(v):
         return k

def add_post_init(wrapped_class: Callable):
   try:
      # If it does not fail, it means that the __init__() already calls __component_post_init__()
      wrapped_class.__init__.__mtg_post_init__
   except AttributeError as exc:
      # If it fails, we must decorate the __init__() function to call __component_post_init__()
      dflt_init = wrapped_class.__init__

      @wraps(dflt_init)
      def __init__(self, *args, **kwargs):
         dflt_init(self, *args, **kwargs)
         self.__component_post_init__(*args, **kwargs)
      setattr(wrapped_class, '__init__', __init__)
      setattr(wrapped_class.__init__, '__mtg_post_init__', True)
   return wrapped_class

def param_block(wrapped_class: Callable):
   return add_post_init(wrapped_class)

def base_class(wrapped_class: Callable):
   return add_post_init(wrapped_class)

@base_class
@dataclass
class Component(metaclass=ABCMeta):

   soft_alloc = False
   _tensor = False
   exclude: ClassVar = False
   is_nop: ClassVar = False
   _ignore_excl: ClassVar = False
   _glbl_cntr: ClassVar = 0

   # Class attribute (shared by all the Components)
   __snake_case = re.compile('((?<=[a-z0-9])[A-Z]|(?!^)[A-Z](?=[a-z]))')
   __get_resrc = re.compile("{(.*?)}")
   __get_li = re.compile(r"(?<!{)\bli\b(?!})")
   __get_tm = re.compile(r"(?<!{)\btensor_mask\b(?!})")
   __get_tcs = re.compile(r"(?<!{)\btensor_conv_size\b(?!})")
   __get_tcc = re.compile(r"(?<!{)\btensor_conv_ctrl\b(?!})")
   test_helper: InitVar[TestHelper] = None

   def __init__(self, test_helper: TestHelper = None):
      pass

   def __component_post_init__(self, *args, **kwargs):
      self.__name = self.__snake_case.sub(r'_\1', self.__class__.__name__).lower()
      try:
         print(self._component_list)
         raise ValueError(f"{self.__class__.__name__} is initializing self._component_list twice")
      except AttributeError:
         self._component_list = list()
      test_helper = None
      # Try to find a TestHelper instance in args
      for i in args:
         if isinstance(i, TestHelper):
            test_helper = i
            break
      if test_helper is None:
         # Try to find a TestHelper instance in keyword args
         for i in kwargs:
            if isinstance(kwargs[i], TestHelper):
               test_helper = kwargs[i]
               break
      try:
         # Try call _run with test_helper keyword argument
         self._run(self._component_list, test_helper = test_helper)
      except TypeError:
         # If the class does not define the test_helper keyword argument, just not pass it
         self._run(self._component_list)

   @property
   def name(self):
      return self.__name

   @classmethod
   def ignore_excl(cls):
      cls._ignore_excl = True

   @classmethod
   def _incr_glbl_cntr(cls):
      cls._glbl_cntr += 1

   @classmethod
   def _get_glbl_cntr(cls):
      return cls._glbl_cntr

   @abstractmethod
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def _render_if(self, model: Model) -> bool:
      return not self.is_nop and self.render_if(model)

   def render_if(self, model: Model) -> bool:
      return True

   def _assert_load_addr(self, i: str, name: str):
      # TODO: this checking could be done in a CI regression
      lines = i.splitlines()
      for line in lines:

         if (self.name == "li") or (self.__get_li.findall(line)):
            resources = self.__get_resrc.findall(line)
            if len(resources) != 0:
               res_name = resources[-1]
               resource = self.__dict__[res_name]
               assert not isinstance(resource, Malloc), f"Memory allocation for '{res_name}' must be done using the 'MTG_LOAD_ADDR' Component"

   def _assert_tensor_mask(self, i: str, name: str):
      # TODO: this checking could be done in a CI regression
      lines = i.splitlines()
      for line in lines:
         tm = self.__get_tm.findall(line)
         assert len(tm) == 0, "tensor_mask can only be set using the TensorMask Component"

         tcs = self.__get_tcs.findall(line)
         assert len(tcs) == 0, "tensor_conv_size can only be set using the TensorConvSize Component"

         tcc = self.__get_tcc.findall(line)
         assert len(tcc) == 0, "tensor_conv_ctrl can only be set using the TensorConvCtrl Component"

   def _gen_new_labels(self, i: str) -> None:
      assert isinstance(i, str)
      _re_findall = re.compile(r'\w+:')
      labels = re.findall(_re_findall, i)

      for l in labels:
         l = l[:-1]
         if l not in TEST_CONF.STATIC_LABELS:
            assert l not in self._labels_dict, f"{l}"
            self._labels_dict[l] = f'{self.name}_{l}_{self._lcl_cntr}'

   def _gen_snip_labels(self, pre="") -> None:
      if not self._ignore_excl:
         assert self.exclude is False, f"Trying to render an excluded Component ('{self.__class__.__name__}')"
      self._lcl_cntr = self._get_glbl_cntr()
      self._incr_glbl_cntr()
      self._labels_dict = dict()

      for i in self._component_list:
         if isinstance(i, Component):
            try:
               i._gen_snip_labels(pre=pre+"  ")
            except Exception as exc:
               raise Exception(f"The '{self.__class__.__name__}' Component failed to generate the labels") from exc
         elif isinstance(i, str):
            self._gen_new_labels(i)
         else:
            assert False, f"Snippet content can only be 'str' or 'Component'. Found {type(i)}."

   def _rename_labels(self, i: str) -> None:
      for key, value in self._labels_dict.items():
         i = re.sub(key, value, i)
      return i

   def _gen_snip_content(self, pre="") -> ('Component', list):
      if not self._ignore_excl:
         assert self.exclude is False, f"Trying to render an excluded Component ('{self.__class__.__name__}')"
      content_list = list()

      def assert_comps(i, method, excl_comps):
         if self.name not in excl_comps:
            try:
               method(i, self.name)
            except Exception as exc:
               raise Exception(f"The '{self.__class__.__name__}' Component failed to generate the Component content") from exc

      for i in self._component_list:
         if isinstance(i, Component):
            try:
               content_list.append(i._gen_snip_content(pre=pre+"  "))
            except Exception as exc:
               raise Exception(f"The '{self.__class__.__name__}' Component failed to generate the Component content") from exc
         elif isinstance(i, str):
            assert_comps(i, self._assert_load_addr, ['mtg_load_addr'])
            assert_comps(i, self._assert_tensor_mask, ['tensor_mask', 'tensor_conv_size', 'tensor_conv_ctrl', 'backup_tensor_csr', 'backup_vpu_tenc', 'restore_tensor_csr', 'read_tensor_mask'])
            i = self._rename_labels(i)
            content_list.append(i)
         else:
            assert False, f"Snippet content can only be 'str' or 'Component'. Found {type(i)}."
      return (self, content_list)

   def _compute_refs(self, snip_content: list) -> None:
      for i in snip_content:
         if isinstance(i, str):
            resources = self.__get_resrc.findall(i)
            for res_name in resources:
               resource = self.__dict__[res_name]
               if isinstance(resource, VirtualResource):
                  resource.inc_refs()
               elif isinstance(resource, Imm) or isinstance(resource, Frm) or isinstance(resource, Malloc):
                  resource.inc_refs()
         else:
            # Must be a pair of type ('Component', list)
            i[0]._compute_refs(i[1])

   def  _alloc_predefined_regs(self, model: Model, snip_content: list) -> None:
      for i in snip_content:
         if isinstance(i, str):
            for line in i.splitlines():
               resources = self.__get_resrc.findall(line)
               for res_name in reversed(resources):
                  resource = self.__dict__[res_name]
                  if isinstance(resource, VirtualResource):
                     if resource.reg_num is not None:
                        try:
                           resource.allocate(model)
                        except Exception as exc:
                           raise Exception(f"{self.__class__.__name__} failed to allocate {resource.__class__.__name__}({resource.reg_num}) for '{var_name(self, resource)}'") from exc
         else:
            # Must be a pair of type ('Component', list)
            try:
               if i[0].soft_alloc is True:
                  i[0]._soft_alloc_predefined_regs(model, i[1])
               else:
                  i[0]._alloc_predefined_regs(model, i[1])
            except Exception as exc:
               raise Exception(f"{self.__class__.__name__} failed to allocate resources for the sub Component '{var_name(self, i[0])}' of type '{i[0].__class__.__name__}'") from exc

   def  _soft_alloc_predefined_regs(self, model: Model, snip_content: list) -> None:
      for i in snip_content:
         if isinstance(i, str):
            for line in i.splitlines():
               resources = self.__get_resrc.findall(line)
               for res_name in reversed(resources):
                  resource = self.__dict__[res_name]
                  if isinstance(resource, VirtualResource):
                     if resource.reg_num is not None:
                        try:
                           resource.soft_allocate(model)
                        except Exception as exc:
                           raise Exception(f"{self.__class__.__name__} failed to allocate {resource.__class__.__name__}({resource.reg_num}) for '{var_name(self, resource)}'") from exc
         else:
            # Must be a pair of type ('Component', list)
            try:
               i[0]._soft_alloc_predefined_regs(model, i[1])
            except Exception as exc:
               raise Exception(f"{self.__class__.__name__} failed to allocate resources for the sub Component '{var_name(self, i[0])}' of type '{i[0].__class__.__name__}'") from exc

   def  _Malloc_and_free(self, model: Model, snip_content: list) -> None:
      for i in snip_content:
         if isinstance(i, str):
            for line in i.splitlines():
               resources = self.__get_resrc.findall(line)
               for res_name in reversed(resources):
                  resource = self.__dict__[res_name]
                  if isinstance(resource, Malloc):
                     try:
                        resource.allocate(model)
                        resource.free()
                     except Exception as exc:
                        raise Exception(f"{self.__class__.__name__} failed to allocate memory for '{var_name(self, resource)}'") from exc
         else:
            # Must be a pair of type ('Component', list)
            try:
               i[0]._Malloc_and_free(model, i[1])
            except Exception as exc:
               raise Exception(f"{self.__class__.__name__} failed to allocate resources for the sub Component '{var_name(self, i[0])}' of type '{i[0].__class__.__name__}'") from exc

   def  _alloc_and_free(self, model: Model, context: 'Component', snip_content: list, skip: bool = False) -> None:
      for i in snip_content:
         if isinstance(i, str):
            for line in i.splitlines():
               resources = self.__get_resrc.findall(line)
               for res_name in reversed(resources):
                  resource = self.__dict__[res_name]
                  if isinstance(resource, VirtualResource):
                     try:
                        if not skip:
                           resource.allocate(model)
                     except Exception as exc:
                        raise Exception(f"{self.__class__.__name__} failed to allocate VirtualResource '{var_name(self, resource)}'") from exc
                     try:
                        resource.free()
                     except AssertionError as exc:
                        if not skip:
                           raise Exception(f"{self.__class__.__name__} failed to free VirtualResource '{var_name(self, resource)}'") from exc
                        else:
                           # When you free a resource coming from a skipped Component, typically, this resource has not been allocated
                           # and an assertion in the VirtualResource (e.g. Xreg) class will fail, however, we still have to decrement the
                           # reference counter. This is fine only in skipped Components
                           resource._count -= 1
                  elif isinstance(resource, TensorEncoding):
                     if not skip and context._tensor:
                        resource.allocate(model)
                     resource.free() # This free does not actually free the HW resources used by the Tensor operation
                  elif isinstance(resource, Imm) or isinstance(resource, Frm):
                     if not skip:
                        resource.set_val(model)
                     resource.free()
                  elif isinstance(resource, ABIreg):
                     resource.sanity_check(model)
         else:
            # Must be a pair of type ('Component', list)
            try:
               if skip is True:
                  i[0]._alloc_and_free(model, i[0], i[1], skip = True)
               else:
                  skip_next = not i[0]._render_if(model)
                  i[0]._alloc_and_free(model, i[0], i[1], skip = skip_next)
            except Exception as exc:
               raise Exception(f"{self.__class__.__name__} failed to allocate resources for the sub Component '{var_name(self, i[0])}' of type '{i[0].__class__.__name__}'") from exc

      if skip is True:
         snip_content.clear()

   def _gen_snippet(self, snip_content: list) -> Snippet:
      try:
         snip = self._snip_class(name = self.name)
         for i in snip_content:
            if isinstance(i, str):
               content = i.format(**self.__dict__)
               snip += content
            else:
               # Must be a pair of type ('Component', list)
               sub_snip = i[0]._gen_snippet(i[1])
               snip += sub_snip
         return snip
      except:
         raise Exception(f"{self.__class__.__name__} must extend from AsmCode or CCode")

   def _override_UXregs(self, model: Model, snip_content) -> list:
      if len(model.integer.unpredictable) != 0:
         snip_content[1].append("/* Overriding unpredictable registers */")
         for i in model.integer.unpredictable:
            snip_content[1].append(f"li x{i.reg_num}, {model.test_helper.get_rand_num()}")
         model.integer.unpredictable.clear()
      return snip_content

   def render(self, model: Model) -> Snippet:
      self._gen_snip_labels()
      snip_content = self._gen_snip_content()
      check_rtlmin_workarounds(snip_content[0], snip_content[1])
      self._compute_refs(snip_content[1])
      if self.soft_alloc is True:
        self._soft_alloc_predefined_regs(model, snip_content[1])
      else:
        self._alloc_predefined_regs(model, snip_content[1])
      self._alloc_and_free(model, snip_content[0], snip_content[1], skip = not self._render_if(model))
      self._Malloc_and_free(model, snip_content[1])
      snip_content = self._override_UXregs(model, snip_content)
      snip = self._gen_snippet(snip_content[1])
      return snip
