from abc import ABCMeta, abstractmethod
from dataclasses import dataclass, field, InitVar

from core.models.model import *
from core.snippet.asm import Asm, AsmCode
from core.snippet.snippet import Snippet
from core.component import Component, param_block, base_class
from core.component_handler import ComponentHandler, mepc_plus, sepc_plus, uepc_plus, set_mepc_to, set_sepc_to, set_uepc_to
from core.cond_component import CondComponent
from core.skip_harts import only_mid, only_phid, only_thread, only_lhid, only_mod_lhid, only_tensor_coop
from core.test_conf import TEST_CONF, MTG_HANDLER

# Dynamic memory map import based on target
if TEST_CONF.TARGET == 'erbium':
    from conf.mem.erbium_mem import *
else:
    from conf.mem.et_mem import *

from core.component_factory import register, mtg_dataclass, get_component
from components.common import *

from core.priv.decorators import machine_code, supervisor_code, user_code, machine_supervisor_code
