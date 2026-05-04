
# start(<component_name>)
#
# main(<component_name>, <component_bias>)
# <component_name> must be the class name
# <component_bias> must be the bias relative to the number of ops
#
# end(<component_name>)

preamble("MEnableShadows")
preamble("SetLHID")
preamble("WaitForFccRelay")

if TEST_CONF.TARGET_RANDOMIZATION == True:
   preamble("BackupMinionDiagsABI")

preamble("RepeatTestTop")
preamble("InitMstatus")

if TEST_CONF.TARGET_RANDOMIZATION == True:
   preamble("RestoreMinionDiagsSP")

preamble("InitFPABI")
preamble("SetGAMOThreadPointer")
preamble("SetLAMOThreadPointer")

if TEST_CONF.TARGET_RANDOMIZATION == True:
   preamble("TargetRandomization")

preamble("RandXregs")
preamble("RandFPregs")
preamble("EnableL1SCP")
preamble("ClearL2SCP")
preamble("ClearTenC")
preamble("SetStackPointer")
preamble("SetDRAMSize")
preamble("SetGlobalPointer")
preamble("SetThreadPointer")
preamble("InitUART")
preamble("SetupMTrapVector")
preamble("SetupSTrapVector")
#preamble("SetupUTrapVector")
preamble("FillL1SCP")
preamble("InitCSRs")
preamble("SetCoopMode")
preamble("DummyTensorLoad")
#preamble("SCL3YieldPriority")

start("RandXregs")

main("InitFPABI", 3)
main("ConstrainRegs", 25)
main("SetRandPriv", 2)

end("JumpToMMode")
end("EOTChecks")
end("RepeatTestBottom")
end("MTrapVector")
end("STrapVector")
end("UTrapVector")
