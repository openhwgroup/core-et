
# Erbium Base Configuration
# Minimal preamble

preamble("MEnableShadows")
preamble("SetLHID")
preamble("WaitForFccRelay")

# Setup trap vectors early before any components that might cause exceptions
preamble("SetupMTrapVector")
preamble("SetupSTrapVector")

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
# No ClearL2SCP - no L2 Shire Cache on Erbium
preamble("ClearTenC")
preamble("SetStackPointer")
# No SetDRAMSize - MPROT ESR doesn't exist on Erbium
preamble("SetGlobalPointer")
preamble("SetThreadPointer")
preamble("InitUART")
# SetupMTrapVector and SetupSTrapVector moved earlier
preamble("FillL1SCP")
preamble("InitCSRs")
preamble("SetCoopMode")
preamble("DummyTensorLoad")

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
