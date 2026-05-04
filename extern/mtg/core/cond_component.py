from abc import ABCMeta, abstractmethod
from core.component import Component, base_class
from core.models.test_helper import TestHelper

@base_class
class CondComponent(Component, metaclass=ABCMeta):
   skip_assert = False

   @abstractmethod
   def render_if(self, model: 'Model') -> None:
      pass

   @abstractmethod
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def  _alloc_and_free(self, model: 'Model', context: 'Component', snip_content: list, skip: bool = False) -> None:
      super()._alloc_and_free(model, context, snip_content, skip = skip)
      if not skip and not self.skip_assert:
         assert self.render_if(model) == False
