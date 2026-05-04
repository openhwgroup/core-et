from components.randinst.randinst_base import RandinstBase
from components.randinst.rv32i import *
from components.randinst.rv64i import *
from components.randinst.rv32m import *
from components.randinst.rv64m import *
from components.randinst.rv32f import *
from components.randinst.rv64f import *

_rv_components = list()
_rv_components += RV32I.subcls
_rv_components += RV64I.subcls
_rv_components += RV32M.subcls
_rv_components += RV64M.subcls
_rv_components += RV32F.subcls
_rv_components += RV64F.subcls

@register
class RV(RandinstBase):

   subcls = _rv_components
