
# start(<component_name>)
#
# main(<component_name>, <component_bias>)
# <component_name> must be the class name
# <component_bias> must be the bias relative to the number of ops
#
# end(<component_name>)

# Components that Trap
main("TrapTensor",         3)
main("TrapTensorSequence", 1)
main("TrapAnySequence",    1)

# RV32IMF and RV64IMF
main("RV",                10)

# PRM4: Mask
main("Mask",              10)
main("MovaMX",             1)
main("MovaXM",             1)

# PRM5: PackedSingle
main("PackedSingle",      10)

# PRM6: PackedInteger
main("PackedInteger",     10)

# PRM7: Atomic
main("Atomic",            10)

# PRM8: CacheOps
main("CacheOps",          10)

main("EvictAllL1D",        1)

main("RANDINST",          10)

main("IntReadAfterRead", 10)
main("IntReadAfterWrite", 10)
main("IntWriteAfterWrite", 10)
main("IntWriteAfterRead", 10)

main("FPReadAfterRead", 10)
main("FPReadAfterWrite", 10)
main("FPWriteAfterWrite", 10)
main("FPWriteAfterRead", 10)

main("MaskReadAfterRead", 10)
main("MaskReadAfterWrite", 10)
main("MaskWriteAfterWrite", 10)
main("MaskWriteAfterRead", 10)

main("X31ReadAfterRead", 10)
main("X31ReadAfterWrite", 10)
main("X31WriteAfterRead", 10)

main("ExclusiveMode",      10)
main("UnlockSW",           10)
main("CacheInvalidate",    10)
main("EvictOrFlushSW",     10)
main("EvictOrFlushVA",     10)
#main("LockOrUnlockSW",     10)
#main("LockOrUnlockVA",     10)
#main("PrefetchVA",         10)
main("CacheOpsWait",        5)

main("RandomCSRRead",     10)
main("ClearTensorError",  10)

main("RandomTensorWait",   5)
main("TensorLoad",        10)
main("TensorLoadCoop",    10)
main("TensorLoadL2SCP",   10)
main("TensorFMA",         10)
main("TensorLoadFMATenB", 10)
main("TensorStore",       10)
main("TensorStoreCoop",   10)
main("TensorStoreSCP",    10)
main("TensorQuant",       10)
main("TensorReduce",      10)
main("SetRandTensorMask", 10)

main("IllegalTensorLoad",  1)
main("IllegalTensorStore", 1)

main("TensorSequence", 10)
main("AnySequence",    10)
main("TFMATenaTLTenb",  5)
main("TsScpTFMA",       5)
main("TFMATsScp",       5)
main("TsScpTQuant",     5)
main("TQuantTsScp",     5)
main("TensorMixed",    10)
main("TensorCoop",      5)

#main("FswLGPs",         5)
main("SCTraffic", 20)

main("FLBFCCSyncShireThreads",                10)

# Inter-Processor Interrupts
start("IPIRedirectFilter")
main("IPITrigger",  1)
main("IPIRedirect", 1)
