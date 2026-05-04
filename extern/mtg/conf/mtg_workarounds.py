
# IPI Components are generating CRC16 mismatches in FC (34 shires) in the BUB
# the mismatch comes from the X2 (StackPointer) being different to what sys_emu expects
TEST_CONF.EXCL_COMPS.add(IPITrigger)
TEST_CONF.EXCL_COMPS.add(IPIRedirectFilter)
TEST_CONF.EXCL_COMPS.add(IPIRedirect)

# Exclude MTG components that do not apply for Erbium
if TEST_CONF.TARGET == 'erbium':

    shire_cache = [EvictAllL2, EvictAllL3, ClearL2SCP, EvictCoalescingBuffer]
    minion = [TensorLoadL2SCP]

    # TODO: InitUART temporarily disabled - UART not yet implemented in Erbium emulator
    tmp = [InitUART]

    erbium_invalid_comps = shire_cache + minion + tmp

    for inv_comp in erbium_invalid_comps:
        TEST_CONF.EXCL_COMPS.add(inv_comp)   # Register component as excluded
        inv_comp.ignore_excl()               # Disable exclusion checks for this component
        inv_comp.is_nop = True               # Mark component as a no-op so it is not rendered
