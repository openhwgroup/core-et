import os

class TEST_CONF:
   # Target chip: "erbium" or "et-soc-1"
   TARGET = os.environ.get('MTG_TARGET', 'erbium')

   UNIQ_PC = False

   # CRC16 Self-checking
   ADD_SELF_CHK = 1
   CHK_FREQ = 50
   NUM_COMPS = 10000

   # These variables will be overwritten based on the provided shire/minion mask
   NUM_HARTS = 2
   THREAD_MASK = 0x3
   MINION_MASK = 0x1
   SHIRE_MASK = 0x1
   SEQ_BASE_HART = 0
   SEQ_NUM_HARTS = 2
   SEQ_MINION_MASK = 0x1
   SEQ_SHIRE_MASK = 0x1
   SEQUENTIAL_MODE = 0x0
   NUM_ACTIVE_HARTS_PER_SHIRE = 2
   ACTIVE_HART_IDS = [0, 1]
   MAX_NUM_MINIONS = 32 * 34 #32 Minions per shire, 34 shires at most

   EXCL_COMPS = set()
   EXCL_INTREGS = []
   EXCL_FPREGS = []
   EXCL_MREGS = []

   EXTRA_CHECKS = 0
   USE_TENSOR_MASK_MODEL = 0
   MINION_AGNOSTIC = 0
   NEIGH_AGNOSTIC = 0
   SHIRE_AGNOSTIC = 0
   POST_SILICON = 0

   MAX_AMO_OFF = (2*1024)-1
   MAX_OFF = (2*1024)-1
   HART_STACK_SIZE = 8*1024 # This only takes effect if POSTSI_L2_ONLY == True (--postsi_l2)

   POSTSI_L2_ONLY = False

   STATIC_LABELS = ["postsi_ust_test_end"]

   DEBUG_PRINTS = False
   RTLMIN_WORKAROUNDS = {
      'rtlmin-2840': True,
      'rtlmin-6136': False,
      'rtlmin-6160': True,
      'rtlmin-6398': False,
      'rtlmin-6469': True,
      'rtlmin-6537': True,
      'fv-260': False
   }

   TARGET_RANDOMIZATION = False
   _RENDER_REM_RAND_DATA = False

   SYS_EMU_MAX_CYCLES = 10000000

   @classmethod
   def agnostic_mode(cls):
      def TrueXor(*args):
         return sum(args) == 1
      return TrueXor(cls.MINION_AGNOSTIC, cls.NEIGH_AGNOSTIC, cls.SHIRE_AGNOSTIC)

   REPEAT_COUNT = -1


class L2SCP_CONF:
   mem_head = None
   mem_tail = None

   tensor_head = None
   tensor_tail = None

   mem_size = None
   tensor_slice_size = None # Size per HART
   nlines = 0xa000 # 0x0280000 Bytes

   bytes_per_line = 64

   @classmethod
   def print_conf(cls):
      print(f"L2 SCP CONF: Memory=[{hex(L2SCP_CONF.mem_head)}, {hex(L2SCP_CONF.mem_tail)}] | Tensor=[{hex(L2SCP_CONF.tensor_head*64)}, {hex(L2SCP_CONF.tensor_tail*64 + 63)}] (slice_size: {cls.tensor_slice_size*64} Bytes)")

   @classmethod
   def set_l2scp_conf(cls, test_helper: 'TestHelper'):
      configs = [(40, 45), (45, 55), (55, 60)]
      weights = [       2,        4,        2]
      config = test_helper.pick_one_weight(configs, weights)
      config = test_helper.get_rand_range(config[0], config[1])

      order = test_helper.get_rand_range(0, 1)
      if order == 0:
         cls.mem_head = 0
         cls.mem_tail = int((cls.nlines/100)*config)
         cls.tensor_head = cls.mem_tail
         cls.tensor_tail = cls.nlines - 1
      else:
         cls.tensor_head = 0
         cls.tensor_tail = int((cls.nlines/100)*config)
         cls.mem_head = cls.tensor_tail + 1
         cls.mem_tail = cls.nlines

      cls.mem_head = cls.mem_head * cls.bytes_per_line
      cls.mem_tail = (cls.mem_tail * cls.bytes_per_line) - 1
      cls.mem_size = cls.mem_tail - cls.mem_head + 1

      if TEST_CONF.THREAD_MASK == 0x3:
         num_harts_per_shire = int(TEST_CONF.NUM_ACTIVE_HARTS_PER_SHIRE / 2)
      else:
         num_harts_per_shire = TEST_CONF.NUM_ACTIVE_HARTS_PER_SHIRE

      cls.tensor_slice_size = int((cls.tensor_tail - cls.tensor_head) / num_harts_per_shire)

      assert cls.mem_head <= cls.mem_tail, f"{cls.mem_head} <= {cls.mem_tail}"
      assert cls.tensor_head <= cls.tensor_tail, f"{cls.tensor_head} <= {cls.tensor_tail}"

      assert cls.mem_size + (cls.tensor_slice_size * num_harts_per_shire) < (cls.nlines * cls.bytes_per_line)
      assert cls.mem_head >= 0 and cls.tensor_head >= 0
      assert cls.mem_tail <= cls.nlines * cls.bytes_per_line and cls.tensor_tail <= cls.nlines

def fv260_a_wa(content_list: list, idx = None):
   if TEST_CONF.RTLMIN_WORKAROUNDS['fv-260'] == True:
      if idx is not None:
         content_list.append(f"fmv.x.w x0, f{idx}")
      else:
         content_list.append("fmv.x.w x0, {fd}")
      content_list.append("addi x0, x0, 0")

def fv260_b_wa(content_list: list):
   if TEST_CONF.RTLMIN_WORKAROUNDS['fv-260'] == True:
      for _ in range(8):
         content_list.append("addi x0, x0, 0")

def fv260_c_wa(content_list: list):
   if TEST_CONF.RTLMIN_WORKAROUNDS['fv-260'] == True:
      for _ in range(8):
         content_list.append("addi x0, x0, 0")

class MTG_HANDLER:
   M_HINT = "mtg-machine-handler"
   S_HINT = "mtg-supervisor-handler"
   U_HINT = "mtg-user-handler"
