
# SingleInstruction Components
t("TENSOR_LOAD",      "components/tensor/tensor.py")
t("TENSOR_LOAD_L2",   "components/tensor/tensor.py")
t("TENSOR_FMA",       "components/tensor/tensor.py")
t("TENSOR_STORE",     "components/tensor/tensor.py")
t("TENSOR_STORE_SCP", "components/tensor/tensor.py")
t("TENSOR_QUANT",     "components/tensor/tensor.py")
t("TENSOR_REDUCE",    "components/tensor/tensor.py")
t("TENSOR_MASK",      "components/tensor/tensor_mask.py")
t("TENSOR_CONV_SIZE", "components/tensor/tensor_mask.py")
t("TENSOR_CONV_CTRL", "components/tensor/tensor_mask.py")
t("TENSOR_COOP",      "components/tensor/tensor.py")

t("TensorMask",     "components/tensor/tensor_mask.py")
t("TensorConvSize", "components/tensor/tensor_mask.py")
t("TensorConvCtrl", "components/tensor/tensor_mask.py")
t("SetRandTensorMask", "components/tensor/tensor_mask.py")

# Tensor Components
t("TensorLoad",      "components/tensor/tensor_load.py")
t("TensorLoadCoop",  "components/tensor/tensor_load_coop.py")
t("TensorLoadL2SCP", "components/tensor/tensor_load_l2scp.py")
t("TensorFMA",       "components/tensor/tensor_fma.py")
t("TensorLoadFMATenB", "components/tensor/tensor_load_fma_tenb.py")
t("TensorStore",     "components/tensor/tensor_store.py")
t("TensorStoreCoop", "components/tensor/tensor_store_coop.py")
t("TensorQuant",     "components/tensor/tensor_quant.py")
t("TensorStoreSCP",  "components/tensor/tensor_store_scp.py")
t("TensorReduce",    "components/tensor/tensor_reduce.py")
t("TensorCoop",      "components/tensor/tensor_coop.py")

# Tensor Sequences
t("DummyTensorLoad", "components/tensor/dummy_tensor_load.py")
t("TensorSequence", "components/tensor/tensor_sequences.py")
t("AnySequence", "components/tensor/tensor_sequences.py")
t("TFMATenaTLTenb", "components/tensor/tensor_sequences.py")
t("TsScpTFMA", "components/tensor/tensor_sequences.py")
t("TsScpTQuant", "components/tensor/tensor_sequences.py")
t("TFMATsScp", "components/tensor/tensor_sequences.py")
t("TQuantTsScp", "components/tensor/tensor_sequences.py")

# Illegal tensors (tensor_error)
t("IllegalTensorLoad",  "components/tensor/illegal_tensor_load.py")
t("IllegalTensorStore", "components/tensor/illegal_tensor_store.py")

# Tensors sharing same memory region
t("TensorMixed", "components/tensor/tensor_mixed.py")

# Tensor Waits
t("RandomTensorWait", "components/tensor/random_tensor_wait.py")
