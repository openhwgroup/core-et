
from core.test_conf import TEST_CONF

def check_rtlmin_workarounds(comp: 'Component', snip_content: list):
   if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-2840'] == True:
      check_rtlmin_2840(comp.__class__.__name__, snip_content)
   if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6136'] == True:
      check_rtlmin_6136(comp.__class__.__name__, snip_content)
   if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6398'] == True:
      check_rtlmin_6398(comp.__class__.__name__, snip_content)
   if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6469'] == True:
      check_rtlmin_6469(comp.__class__.__name__, snip_content)
   if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6537'] == True:
      check_rtlmin_6537(comp.__class__.__name__, snip_content)

   for i in snip_content:
      if not isinstance(i, str):
         # Must be a pair of type ('Component', list)
         check_rtlmin_workarounds(i[0], i[1])

def check_rtlmin_2840(component_name: str, component_list: list):
   jira = "RTLMIN-2840"
   for comp in component_list:
      if isinstance(comp, str):
         if "fswl.ps" in comp:
            assert component_name  == "FSWL_PS", f"Error when checking for {jira} workarounds"
         elif "fswg.ps" in comp:
            assert component_name  == "FSWG_PS", f"Error when checking for {jira} workarounds"
      else:
         subcomp_name = comp.__class__.__name__
         if subcomp_name == "FSWL_PS" or  subcomp_name == "FSWG_PS":
            assert component_name == "FswLGPs", f"Error when checking for {jira} workarounds"
         
def check_rtlmin_6136(component_name: str, component_list: list):
   jira = "RTLMIN-6136"
   for comp in component_list:
      if isinstance(comp, str):
         if "tensor_mask" in comp and ("csrrw" in comp or "csrw" in comp):
            if component_name != "BackupVPUTenc" and component_name != "RestoreTensorCSR":
               assert component_name == "TENSOR_MASK", f"Error when checking for {jira} workarounds ({component_name})"

def check_rtlmin_6398(component_name: str, component_list: list):
   jira = "RTLMIN-6398"
   for comp in component_list:
      if isinstance(comp, str):
         if "mret" in comp:
            assert component_name == "MRET", f"Error when checking for {jira} workarounds"
         if "sret" in comp:
            assert component_name == "SRET", f"Error when checking for {jira} workarounds"
         if "uret" in comp:
            assert component_name == "URET", f"Error when checking for {jira} workarounds"

def check_rtlmin_6469(component_name: str, component_list: list):
   jira = "RTLMIN-6469"
   for comp in component_list:
      if isinstance(comp, str):
         if "amocmpswap" in comp:
            assert component_name == "AMOCMPSWAPL_W" or component_name == "AMOCMPSWAPL_D" or component_name == "AMOCMPSWAPG_W" or component_name == "AMOCMPSWAPG_D", f"Error when checking for {jira} workarounds"

def check_rtlmin_6537(component_name: str, component_list: list):
   jira = "RTLMIN-6537"

   def check_cops(comp, op_name):
      if op_name.lower() in comp:
         assert component_name == op_name.capitalize(), f"Error when checking for {jira} workarounds"

   cops = ["evict_sw", 'flush_sw', 'evict_va', 'flush_va', 'prefetch_va', 'lock_va', 'unlock_va']

   for comp in component_list:
      if isinstance(comp, str):
         for cop in cops:
            check_cops(comp, cop)
