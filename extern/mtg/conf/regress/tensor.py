
# start(<component_name>)
#
# main(<component_name>, <component_bias>)
# <component_name> must be the class name
# <component_bias> must be the bias relative to the number of ops
#
# end(<component_name>)

main("TensorReduce",      10)
main("TensorFMA",         10)
main("TensorQuant",       10)
main("SetRandTensorMask", 10)
main("ReadTensorMask",     5)

main("TensorMixed",       40)

#main("TensorLoadL2SCP",   10)

main("IllegalTensorLoad",  1)
main("IllegalTensorStore", 1)
main("RandomTensorWait",   1)
