
pb_start set_ipi_redirect_pc(mem_redirect_pc, ipi_esr_addr, regx1, regx2)
   ; User Mode accesible
   ; Load the address where IPI is going to redirect
   la regx2, mem_redirect_pc
   ; Write into the ESR
   li regx1, ipi_esr_addr
   sd regx2, 0(regx1)
pb_end

pb_start set_ipi_redirect_filter(redirect_filter, ipi_esr_addr, regx1, regx2)
    ; IPI Redirect Filter can only be set by Machine Mode
    li rx1, ipi_esr_addr
    li rx2, redirect_filter
    sd rx2, 0(rx1)
pb_end

pb_start set_ipi_redirect_trigger(redirect_trigger_val, ipi_esr_addr, regx1, regx2)
   ; User Mode accesible
   li regx1, ipi_esr_addr
   li regx2, ipi_redirect_trigger_val
   sd regx2, 0(regx1)
pb_end

pb_start backup_tvec(bak_mem, regx1, regx2)
   la regx1, bak_mem
   csrr regx2, mtvec
   sd regx2, 0(regx1)
pb_end

pb_start set_tvec(handle_interrupt, regx1)
   ; Set interrupt-enable register
   la regx1, handle_interrupt
   csrw mtvec, regx1
pb_end

pb_start restore_tvec(bak_mem, regx1, regx2)
  la regx1, bak_mem
  ld regx2, 0(regx1)
  csrw mtvec, regx2
pb_end

pb_start backup_sw_interrupt(bak_mem, regx1, regx2)
   la regx1, bak_mem
   csrr regx2, mie
   sd regx2, 0(regx1)
pb_end

pb_start enable_sw_interrupt(regx1)
   ; Enable software interrupts in machine mode
   li  regx1, 0x8
   csrs mie, regx1
   csrs mstatus, regx1
pb_end

pb_start restore_sw_interrupt(bak_mem, regx1, regx2)
  la regx1, bak_mem
  ld regx2, 0(regx1)
  csrw mie, regx2
pb_end

pb_start set_ipi_trigger(trigger_val, ipi_esr_addr, regx1, regx2)
    ; IPI Redirect Filter can only be set by Machine Mode
    li rx1, ipi_esr_addr
    li rx2, trigger_val
    sd rx2, 0(rx1)
pb_end

pb_start set_ipi_trigger_clear(trigger_clear_val, ipi_esr_addr, regx1, regx2)
    set_ipi_trigger(trigger_val = trigger_clear_val, ipi_esr_addr = ipi_esr_addr, regx1 = regx1, regx2 = regx2)
pb_end

pb_start ipi_stall()
   ; Currently, in a WFI, IPI trigger will set mepc=pc_stall+4
   ; for this reason we need to add an extra NOP (RTLMIN-4367)
   csrwi stall, 0x1
   add x0, x0, x0 ; nop
pb_end

pb_start ipi_wfi()
   ; Currently, in a WFI, IPI trigger will set mepc=pc_wfi+4
   ; for this reason we need to add an extra NOP (RTLMIN-4367)
   wfi
   add x0, x0, x0 ; nop
pb_end
