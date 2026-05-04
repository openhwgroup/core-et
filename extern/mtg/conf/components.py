
# t(<component_name>, <component_path>)
# <component_name> must be the class name
# <component_path> must be the relative path to the component file

BASE_DIR = os.path.dirname(os.path.realpath(__file__))

# Import from other files
####################################################################
exec(open(BASE_DIR+"/conf/randinst.py").read())
exec(open(BASE_DIR+"/conf/tensor.py").read())
exec(open(BASE_DIR+"/conf/trap.py").read())

# Target randomization
####################################################################
t("TargetRandomization", "components/tgt_rand/tgt_rand.py")

# Preamble Components
####################################################################
t("SetStackPointer",  "components/preamble/set_stack_pointer.py")
t("SetDRAMSize",      "components/preamble/set_dram_size.py")
t("SetThreadPointer", "components/preamble/set_thread_pointer.py")
t("SetGlobalPointer", "components/preamble/set_global_pointer.py")
t("SetGAMOThreadPointer", "components/preamble/set_gamo_tp.py")
t("SetLAMOThreadPointer", "components/preamble/set_lamo_tp.py")
t("ClearTenC", "components/preamble/clear_tenc.py")
t("FillL1SCP", "components/preamble/fill_l1scp.py")
t("InitCSRs", "components/preamble/init_csrs.py")
t("InitUART", "components/preamble/init_uart.py")
t("SetCoopMode", "components/preamble/set_coop_mode.py")
t("EnableSWInterrupts", "components/preamble/enable_sw_interrupts.py")
t("MEnableShadows", "components/preamble/menable_shadows.py")
t("SCL3YieldPriority", "components/sc_esr/sc_l3_yield_priority.py")
t("InitZero", "components/preamble/init_zero.py")
t("RepeatTestTop", "components/preamble/repeat_test.py")
t("RepeatTestBottom", "components/preamble/repeat_test.py")
t("BackupMinionDiagsABI", "components/preamble/minion_diags_abi.py")
t("RestoreMinionDiagsSP", "components/preamble/minion_diags_abi.py")
t("RestoreMinionDiagsGP", "components/preamble/minion_diags_abi.py")
t("RestoreMinionDiagsTP", "components/preamble/minion_diags_abi.py")
t("InitMstatus", "components/preamble/init_mstatus.py")


# Exception handler Components
####################################################################
t("MTrapVector", "components/handler/excpt_handler.py")
t("STrapVector", "components/handler/excpt_handler.py")
t("UTrapVector", "components/handler/excpt_handler.py")
t("SetupMTrapVector", "components/handler/excpt_handler.py")
t("SetupSTrapVector", "components/handler/excpt_handler.py")
t("SetupUTrapVector", "components/handler/excpt_handler.py")


# Self-checking Components
####################################################################
t("GenSignature", "components/checking/gen_signature.py")
t("BackupVPUTenc", "components/checking/gen_signature.py")
t("RestoreVPUTenc", "components/checking/gen_signature.py")


# Common Components
####################################################################
t("ConstrainRegs", "core/constrain_regs.py")
t("BackupToSP",    "components/common.py")
t("RestoreFromSP", "components/common.py")
t("BackupMregs",   "components/common.py")
t("RestoreMregs",  "components/common.py")


# Resource Hazard Components (Int, FP and Mask RF)
####################################################################
t("IntReadAfterRead",    "components/resrc_haz/int_deps.py")
t("IntReadAfterWrite",   "components/resrc_haz/int_deps.py")
t("IntWriteAfterWrite",  "components/resrc_haz/int_deps.py")
t("IntWriteAfterRead",   "components/resrc_haz/int_deps.py")

t("FPReadAfterRead",     "components/resrc_haz/fp_deps.py")
t("FPReadAfterWrite",    "components/resrc_haz/fp_deps.py")
t("FPWriteAfterWrite",   "components/resrc_haz/fp_deps.py")
t("FPWriteAfterRead",    "components/resrc_haz/fp_deps.py")

t("MaskReadAfterRead",   "components/resrc_haz/mask_deps.py")
t("MaskReadAfterWrite",  "components/resrc_haz/mask_deps.py")
t("MaskWriteAfterWrite", "components/resrc_haz/mask_deps.py")
t("MaskWriteAfterRead",  "components/resrc_haz/mask_deps.py")

t("X31ReadAfterRead",    "components/resrc_haz/impl_x31.py")
t("X31ReadAfterWrite",   "components/resrc_haz/impl_x31.py")
t("X31WriteAfterRead",   "components/resrc_haz/impl_x31.py")


# Shire Cache Cache Ops
####################################################################
t("EvictAllL2", "components/cops/sc_cops.py")
t("EvictAllL3", "components/cops/sc_cops.py")
t("ClearL2SCP", "components/cops/sc_cops.py")
t("EvictCoalescingBuffer", "components/cops/sc_cops.py")
t("CoherencyBarrier", "components/cops/coherency_barrier.py")


# Components
####################################################################
# AMO
t("FswLGPs", "components/amo/fsw_lg_ps.py")

# Cacheops
t("SET_DCACHE_SHARED", "components/cacheops.py")
t("SET_D1SPLIT",       "components/cacheops.py")
t("SET_SCPENABLE",     "components/cacheops.py")

t("UnlockSW",          "components/cops/unlock_sw.py")
t("CacheInvalidate",   "components/add_csrs/cache_invalidate.py")
t("EvictOrFlushSW",    "components/cops/evict_or_flush_sw.py")
t("EvictOrFlushVA",    "components/cops/evict_or_flush_va.py")
t("LockOrUnlockSW",    "components/cops/lock_or_unlock_sw.py")
t("LockOrUnlockVA",    "components/cops/lock_or_unlock_va.py")
t("PrefetchVA",        "components/cops/prefetch_va.py")
t("CacheOpsWait",      "components/cops/cacheops_wait.py")

# FLB / FCC
t("FCC", "components/flb_fcc/fcc.py")
t("FCCBarrier", "components/flb_fcc/fcc_barrier.py")
t("FCCSyncThreads", "components/flb_fcc/fcc_sync_threads.py")
t("FLBFCCSyncShireThreads", "components/flb_fcc/flb_fcc_sync_threads.py")
t("AMOFLBFCCSyncThreads", "components/flb_fcc/flb_fcc_sync_threads.py")
t("FCCSyncMinion", "components/flb_fcc/fcc_sync_minion.py")
t("FCCSyncNeigh", "components/flb_fcc/fcc_sync_minion.py")
t("FCCSyncShire", "components/flb_fcc/fcc_sync_minion.py")

# PRM-4
t("MovaMX", "components/mask/mova_m_x.py")
t("MovaXM", "components/mask/mova_x_m.py")

# PRM-10
t("TENSOR_WAIT",    "components/tensor_wait.py")

t("WAIT_TENSOR_LOAD_0",    "components/tensor_wait.py")
t("WAIT_TENSOR_LOAD_1",    "components/tensor_wait.py")
t("WAIT_TENSOR_LOAD_L2_0", "components/tensor_wait.py")
t("WAIT_TENSOR_LOAD_L2_1", "components/tensor_wait.py")
t("WAIT_PREFETCH_0",       "components/tensor_wait.py")
t("WAIT_PREFETCH_1",       "components/tensor_wait.py")
t("WAIT_CACHEOPS",         "components/tensor_wait.py")
t("WAIT_TENSOR_FMA",       "components/tensor_wait.py")
t("WAIT_TENSOR_STORE",     "components/tensor_wait.py")
t("WAIT_TENSOR_REDUCE",    "components/tensor_wait.py")
t("WAIT_TENSOR_QUANT",     "components/tensor_wait.py")

# IPI
t("IPITrigger", "components/ipi/ipi_trigger.py")
t("IPIRedirectFilter", "components/ipi/ipi_redirect_filter.py")
t("IPIRedirect", "components/ipi/ipi_redirect.py")

#ET CSR
t("EXCL_MODE",       "components/et_csr.py")
t("SET_EXCL_MODE",   "components/et_csr.py")
t("UNSET_EXCL_MODE", "components/et_csr.py")

t("RandomCSRRead",      "components/random_csr_read.py")
t("ClearTensorError",   "components/clear_tensor_error.py")

t("EvictAllL1D", "components/evict_all_l1d.py")
t("EnableL1SCP", "components/enable_l1scp.py")
t("InitFPABI",   "components/init_fp_abi.py")

t("SetLHID", "components/preamble/set_lhid.py")
t("RandXregs",    "components/preamble/rand_xregs.py")
t("RandFPregs",    "components/preamble/rand_fpregs.py")

t("ExclusiveMode", "components/exclusive_mode/exclusive_mode.py")
t("SCTraffic", "components/stress/sc_traffic.py")
t("EOTChecks", "components/eot_chk.py")

t("SetRandPriv", "components/set_rand_priv.py")
t("JumpToMMode", "components/set_rand_priv.py")
t("JumpToSMode", "components/set_rand_priv.py")
t("JumpToUMode", "components/set_rand_priv.py")

t("WaitForFccRelay", "components/flb_fcc/fcc_chain.py")
t("PassFccRelay", "components/flb_fcc/fcc_chain.py")

t("PowerSequence", "components/power.py")
t("RandSC", "components/rand_sc/rand_sc.py")

