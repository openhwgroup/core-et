
from functools import wraps
from core.priv.priv import StoMMode, UtoMMode, MtoSMode, MtoUMode, UtoSMode, StoUMode

def __common_code(self, content_list, test_helper, wrapped_method, pre, post):
      post[0].link = pre[0]
      post[1].link = pre[1]

      content_list += pre
      wrapped_method(self, content_list, test_helper)
      content_list += post

def machine_code(wrapped_method):
   @wraps(wrapped_method)
   def wrapper(self, content_list, test_helper = None):
      pre  = [StoMMode(test_helper = test_helper), UtoMMode(test_helper = test_helper)]
      post = [MtoSMode(test_helper = test_helper), MtoUMode(test_helper = test_helper)]
      __common_code(self, content_list, test_helper, wrapped_method, pre, post)
   return wrapper

def supervisor_code(wrapped_method):
   @wraps(wrapped_method)
   def wrapper(self, content_list, test_helper = None):
      pre  = [MtoSMode(test_helper = test_helper), UtoSMode(test_helper = test_helper)]
      post = [StoMMode(test_helper = test_helper), StoUMode(test_helper = test_helper)]
      __common_code(self, content_list, test_helper, wrapped_method, pre, post)
   return wrapper

def user_code(wrapped_method):
   @wraps(wrapped_method)
   def wrapper(self, content_list, test_helper = None):
      pre  = [MtoUMode(test_helper = test_helper), StoUMode(test_helper = test_helper)]
      post = [UtoMMode(test_helper = test_helper), UtoSMode(test_helper = test_helper)]
      __common_code(self, content_list, test_helper, wrapped_method, pre, post)
   return wrapper

def machine_supervisor_code(wrapped_method):
   @wraps(wrapped_method)
   def wrapper(self, content_list, test_helper = None):
      mode = test_helper.pick_one(['m', 's'])
      if mode == "s":
         pre  = [MtoSMode(test_helper = test_helper), UtoSMode(test_helper = test_helper)]
         post = [StoMMode(test_helper = test_helper), StoUMode(test_helper = test_helper)]
      else: #mode == 'm'
         pre  = [StoMMode(test_helper = test_helper), UtoMMode(test_helper = test_helper)]
         post = [MtoSMode(test_helper = test_helper), MtoUMode(test_helper = test_helper)]
      __common_code(self, content_list, test_helper, wrapped_method, pre, post)
   return wrapper
