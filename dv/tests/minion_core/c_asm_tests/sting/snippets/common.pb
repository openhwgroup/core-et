import "$MINION_DIAGS/sting/snippets/zebu.pb"

pb_start snippet_start(random_num, rega, regb)
   li   rega, random_num
   slli rega, rega, 16
   li   regb, 0xC0DE
   add  rega, rega, regb
pb_end


pb_start snippet_end(random_num, rega, regb)
   li   rega, random_num
   slli rega, rega, 16
   li   regb, 0xD3AD
   add  rega, rega, regb
pb_end

;
; Param Block: Get HART Id
;
pb_start get_hart_id(reg_hartid_o)
   csrr reg_hartid_o, mhartid
pb_end

;
; Param Block: Get Shire Id
;
pb_start get_shire_id(reg_shireid_o)
   get_hart_id(reg_hartid_o = reg_shireid_o)
   srli reg_shireid_o, reg_shireid_o, 7
pb_end

;
; Param Block: Test Fail
;
pb_start test_fail(regz)
   ; The following two param blocks contain exactly 3 instructions
if ET_TEST_ENV.EOT_ZEBU_HINTS == 1:
   ; Zebu uses RISC-V hints to stop the execution
   zebu_test_fail(regx = regz)
else:
   ; EVL uses 'validation' registers to stop VCS simulations
   evl_test_fail(regx = regz)
endif
pb_end

pb_start evl_test_fail(regx)
   ; Be careful with adding or deleting instructions of this param block
   ; Some snippets are assuming that this block contains 3 instructions
   ;
   ; For example:
   ;    Some mhandlers will mret to a PC that skips this param block therefore
   ;    is necessary to know the number of instructions contained in this block
   fence iorw, iorw
   lui   regx, 0x50BAD
evl_testfail:
   csrw  validation0, regx
pb_end

pb_start set_m0(value)
       mov.m.x m0, x0, value
pb_end

pb_start mv_imm_to_fd1(lane_num, imm, regx1, fregx1)
   li regx1, imm
   if   ((lane_num%8) == 0):
       set_m0(value = 1)
   elif ((lane_num%8) == 1):
       set_m0(value = 2)
   elif ((lane_num%8) == 2):
       set_m0(value = 4)
   elif ((lane_num%8) == 3):
       set_m0(value = 8)
   elif ((lane_num%8) == 4):
       set_m0(value = 16)
   elif ((lane_num%8) == 5):
       set_m0(value = 32)
   elif ((lane_num%8) == 6):
       set_m0(value = 64)
   else :
       set_m0(value = 128)
   endif
   fbcx.ps fregx1, regx1
pb_end

pb_start load_rand_fp(loop_index, num_var, reg_mem, rega, regb, reg_fp)
   ; Load the Immediate value to a register
   li   rega, num_var
   li   regb, num_var
for loop_index in (0, 32, 4):
   sw   rega, loop_index(reg_mem)
   srli regb, regb, 1
   xor  rega, rega, regb
endfor

   ; Load the data from memory to a FP reg
   flq2 reg_fp, 0(mem_reg)
pb_end

pb_start store_rand_32bytes(loop_index, num_var, reg_mem, rega, regb)
   ; Load the Immediate value to a register
   li   rega, num_var
   li   regb, num_var
for loop_index in (0, 32, 4):
   sw   rega, loop_index(reg_mem)
   srli regb, regb, 1
   xor  rega, rega, regb
endfor

   ; Mask out negative values
   li regb, 0x000007c0
for loop_index in (0, 32, 4):
   lw   rega, loop_index(reg_mem)
   and rega, rega, regb
   sw   rega, loop_index(reg_mem)
endfor
pb_end

pb_start load_mem_to_vpu_lanes(reg_mem, imm_fp)
   ; Load the data from memory to a FP reg
   if (imm_fp == 0):
      flq2 f0, 0(reg_mem)
   elif (imm_fp == 1):
      flq2 f1, 0(reg_mem)
   elif (imm_fp == 2):
      flq2 f2, 0(reg_mem)
   elif (imm_fp == 3):
      flq2 f3, 0(reg_mem)
   elif (imm_fp == 4):
      flq2 f4, 0(reg_mem)
   elif (imm_fp == 5):
      flq2 f5, 0(reg_mem)
   elif (imm_fp == 6):
      flq2 f6, 0(reg_mem)
   elif (imm_fp == 7):
      flq2 f7, 0(reg_mem)
   elif (imm_fp == 8):
      flq2 f8, 0(reg_mem)
   elif (imm_fp == 9):
      flq2 f9, 0(reg_mem)
   elif (imm_fp == 10):
      flq2 f10, 0(reg_mem)
   elif (imm_fp == 11):
      flq2 f11, 0(reg_mem)
   elif (imm_fp == 12):
      flq2 f12, 0(reg_mem)
   elif (imm_fp == 13):
      flq2 f13, 0(reg_mem)
   elif (imm_fp == 14):
      flq2 f14, 0(reg_mem)
   elif (imm_fp == 15):
      flq2 f15, 0(reg_mem)
   elif (imm_fp == 16):
      flq2 f16, 0(reg_mem)
   elif (imm_fp == 17):
      flq2 f17, 0(reg_mem)
   elif (imm_fp == 18):
      flq2 f18, 0(reg_mem)
   elif (imm_fp == 19):
      flq2 f19, 0(reg_mem)
   elif (imm_fp == 20):
      flq2 f20, 0(reg_mem)
   elif (imm_fp == 21):
      flq2 f21, 0(reg_mem)
   elif (imm_fp == 22):
      flq2 f22, 0(reg_mem)
   elif (imm_fp == 23):
      flq2 f23, 0(reg_mem)
   elif (imm_fp == 24):
      flq2 f24, 0(reg_mem)
   elif (imm_fp == 25):
      flq2 f25, 0(reg_mem)
   elif (imm_fp == 26):
      flq2 f26, 0(reg_mem)
   elif (imm_fp == 27):
      flq2 f27, 0(reg_mem)
   elif (imm_fp == 28):
      flq2 f28, 0(reg_mem)
   elif (imm_fp == 29):
      flq2 f29, 0(reg_mem)
   elif (imm_fp == 30):
      flq2 f30, 0(reg_mem)
   elif (imm_fp == 31):
      flq2 f31, 0(reg_mem)
   else:
      test_fail(regz = x31)
   endif

pb_end


pb_start store_vpu_lanes_to_mem(reg_mem, imm_fp)
   ; Load the data from memory to a FP reg
   if (imm_fp == 0):
      fsq2 f0, 0(reg_mem)
   elif (imm_fp == 1):
      fsq2 f1, 0(reg_mem)
   elif (imm_fp == 2):
      fsq2 f2, 0(reg_mem)
   elif (imm_fp == 3):
      fsq2 f3, 0(reg_mem)
   elif (imm_fp == 4):
      fsq2 f4, 0(reg_mem)
   elif (imm_fp == 5):
      fsq2 f5, 0(reg_mem)
   elif (imm_fp == 6):
      fsq2 f6, 0(reg_mem)
   elif (imm_fp == 7):
      fsq2 f7, 0(reg_mem)
   elif (imm_fp == 8):
      fsq2 f8, 0(reg_mem)
   elif (imm_fp == 9):
      fsq2 f9, 0(reg_mem)
   elif (imm_fp == 10):
      fsq2 f10, 0(reg_mem)
   elif (imm_fp == 11):
      fsq2 f11, 0(reg_mem)
   elif (imm_fp == 12):
      fsq2 f12, 0(reg_mem)
   elif (imm_fp == 13):
      fsq2 f13, 0(reg_mem)
   elif (imm_fp == 14):
      fsq2 f14, 0(reg_mem)
   elif (imm_fp == 15):
      fsq2 f15, 0(reg_mem)
   elif (imm_fp == 16):
      fsq2 f16, 0(reg_mem)
   elif (imm_fp == 17):
      fsq2 f17, 0(reg_mem)
   elif (imm_fp == 18):
      fsq2 f18, 0(reg_mem)
   elif (imm_fp == 19):
      fsq2 f19, 0(reg_mem)
   elif (imm_fp == 20):
      fsq2 f20, 0(reg_mem)
   elif (imm_fp == 21):
      fsq2 f21, 0(reg_mem)
   elif (imm_fp == 22):
      fsq2 f22, 0(reg_mem)
   elif (imm_fp == 23):
      fsq2 f23, 0(reg_mem)
   elif (imm_fp == 24):
      fsq2 f24, 0(reg_mem)
   elif (imm_fp == 25):
      fsq2 f25, 0(reg_mem)
   elif (imm_fp == 26):
      fsq2 f26, 0(reg_mem)
   elif (imm_fp == 27):
      fsq2 f27, 0(reg_mem)
   elif (imm_fp == 28):
      fsq2 f28, 0(reg_mem)
   elif (imm_fp == 29):
      fsq2 f29, 0(reg_mem)
   elif (imm_fp == 30):
      fsq2 f30, 0(reg_mem)
   elif (imm_fp == 31):
      fsq2 f31, 0(reg_mem)
   else:
      test_fail(regz = x31)
   endif

pb_end
;
; Param Block: Evict All L1 Cache Lines
;
; value : Initial random value to be written into the CSR
; regb  : Tmp register
; regc  : Tmp register

pb_start evict_all_l1(value, rega, regb, regc)
   li   regb, 0x73fffffffffc3f30
   li   regc, value
   and  regc, regc, regb ; regc contains the masked csr valueue

   li   regb, 0x040000000000000F ; TensorMask(0), DestLvl(01), set(0), way(0), NumLines(15)
   or   rega, regc, regb
   csrw evict_sw, rega
   li   regb, 0x040000000000004F ; TensorMask(0), DestLvl(01), set(0), way(1), NumLines(15)
   or   rega, regc, regb
   csrw evict_sw, rega
   li   regb, 0x040000000000008F ; TensorMask(0), DestLvl(01), set(0), way(2), NumLines(15)
   or   rega, regc, regb
   csrw evict_sw, rega
   li   regb, 0x04000000000000CF ; TensorMask(0), DestLvl(01), set(0), way(3), NumLines(15)
   or   rega, regc, regb
   csrw evict_sw, rega

   wait_cacheops()
   fence iorw, iorw
pb_end

;
; Param Block: Flush All L1 Cache Lines
;
; value : Initial random value to be written into the CSR
; regb  : Tmp register
; regc  : Tmp register

pb_start flush_all_l1(value, rega, regb, regc)
   li   regb, 0x73fffffffffc3f30
   li   regc, value
   and  regc, regc, regb ; regc contains the masked csr valueue

   li   regb, 0x040000000000000F ; TensorMask(0), DestLvl(01), set(0), way(0), NumLines(15)
   or   rega, regc, regb
   csrw flush_sw, rega
   li   regb, 0x040000000000004F ; TensorMask(0), DestLvl(01), set(0), way(1), NumLines(15)
   or   rega, regc, regb
   csrw flush_sw, rega
   li   regb, 0x040000000000008F ; TensorMask(0), DestLvl(01), set(0), way(2), NumLines(15)
   or   rega, regc, regb
   csrw flush_sw, rega
   li   regb, 0x04000000000000CF ; TensorMask(0), DestLvl(01), set(0), way(3), NumLines(15)
   or   rega, regc, regb
   csrw flush_sw, rega

   wait_cacheops()
   fence iorw, iorw
pb_end

;
; Param Block: Wait Cache Ops
;

pb_start wait_cacheops()
   csrwi tensor_wait, 6
pb_end

;
; Param Block: Exclusive Mode
;

pb_start exclusive_mode(set_it, previous)
if (set_it == 0):
   SELECT 1 {
      1: csrrwi previous, excl_mode, 0
      1: csrrci previous, excl_mode, 1
      1: csrrw  previous, excl_mode, x0
   }
else:
   SELECT 1 {
      1: csrrwi previous, excl_mode, 1
      1: csrrsi previous, excl_mode, 1
   }
endif
pb_end

;
; Param Block: Set L1 Cache to work as SCP
;
; value : Initial random value to be written into the CSR
; rega  : CSR Reg
; regb  : Tmp register
; regc  : Tmp register

pb_start set_l1_split(value, rega, regb)
   ; Cache Control masking register
   li regb, 0xfffffffffffffffc

   ; unknown -> 00
   li    rega, value
   and   rega, rega, regb
   csrw  mcache_control, rega
   fence iorw, iorw
   wait_cacheops()

   ; 00 -> 01
   addi  rega, rega, 1
   csrw  mcache_control, rega
   fence iorw, iorw
   wait_cacheops()
pb_end

pb_start set_l1_scp(value, rega, regb)
   ; Cache Control masking register
   li regb, 0xfffffffffffffffc

   ; unknown -> 00
   li    rega, value
   and   rega, rega, regb
   csrw  mcache_control, rega
   fence iorw, iorw
   wait_cacheops()

   ; 00 -> 01
   addi  rega, rega, 1
   csrw  mcache_control, rega
   fence iorw, iorw
   wait_cacheops()

   ; 01 -> 11
   addi  rega, rega, 2
   csrw  mcache_control, rega
   fence iorw, iorw
   wait_cacheops()
pb_end

;
; Param Block: Randomize mprot bits related to the PMA minion R/W
;
; rega  : Tmp register
; regb  : ESR address register to set
; rand_val : Random value to write in mprot from 0 to 6
;

pb_start set_random_mprot(rega, regb, rand_val)

  ;31-30 PP for mprot 3
  ;39-32   1 mandatory
  ;29-22 255 local minion shire
  ;21-20   1 host neighborhood specific
  ;19-16  15 All neigh in shire
  li rega, 0x80DF0000
  ;15-03 esr register --> mprot 0x4 << 3 = 0x20
  slli regb, regb, 3
  or rega, rega, regb    ;rega has @esr

  li regb, rand_val

  sd regb, 0(rega)  ;write it value to @esr

  fence iorw, iorw

pb_end


;
; Param Block: Backup mprot bits related to the PMA minion R/W
;
; rega  : Tmp register
; regb  : ESR address register to set
; regc  : Register to store original mprot value
;

pb_start backup_mprot(rega, regb, regc, memory)

  ;31-30 PP for mprot 3
  ;39-32   1 mandatory
  ;29-22 255 local minion shire
  ;21-20   1 host neighborhood specific
  ;19-16  15 All neigh in shire
  li rega, 0x80D00000
  ;15-03 esr register --> mprot 0x4 << 3 = 0x20
  slli regb, regb, 3
  or rega, rega, regb    ;rega has @esr

  ld regc, 0(rega)  ;store mprot value in register

  la rega, memory
  sd regc, 0(rega)

  srli regb, regb, 3

  fence iorw, iorw

pb_end


;
; Param Block: Restore mprot bits related to the PMA minion R/W
;
; rega  : Tmp register
; regb  : ESR address register to set
; regc  : Register with original mprot value
;

pb_start restore_mprot(rega, regb, regc, memory)

  la rega, memory
  ld regc, 0(rega)
  ;31-30 PP for mprot 3
  ;39-32   1 mandatory
  ;29-22 255 local minion shire
  ;21-20   1 host neighborhood specific
  ;19-16  15 All neigh in shire
  li rega, 0x80DF0000
  ;15-03 esr register --> mprot 0x4 << 3 = 0x20
  slli regb, regb, 3
  or rega, rega, regb    ;rega has @esr

  sd regc, 0(rega)  ;write it value to @esr

  fence iorw, iorw

pb_end


;
; Param Block: Is this current @ in this memory area
;
; reg_addr      : @ address to check
; reg_addr_base : Base memory area @ pass a value and assure you define r1 register
; reg_addr_top  : Top memory area @  pass a value and assure you define r2 register
; reg_area_val  : Register r3 which has the area belong to (user define each area with 0 1 2 ...)
; val_area      : Value for the current area under inspection
; jumpOnfind    : label to Go if @ is in this area (skip pbs bellow)
;

pb_start is_in_region(reg_addr, addr_base, addr_top, reg_area_val, val_area, jumpOnfind)

    li r1, addr_base
    addi r1, r1, -1   ; slt < but addr_base would be == reg_addr
    li r2, addr_top


    sltu reg_area_val, r1, reg_addr
    sltu r1, reg_addr, r2

    add r1, r1, reg_area_val
    li r2, 2
    li reg_area_val, val_area
    beq r1, r2, jumpOnfind

pb_end

;
; Param Block: Write FCC to specific cnt 0
;
; credinc     :  fcc credit counter to choose
; regtmp      :  temporary register
; regtmp2     :  temporary register
; valhartmask :  Mask of minions which has to receive the credit
;
pb_start writeFCC(credinc, regtemp, regtemp2, valhartmask)

  if credinc == 0 :
    li regtemp, 0x803400C0
  elif credinc == 1 :
    li regtemp, 0x803400C8
  elif credinc == 2 :
    li regtemp, 0x803400D0
  elif credinc == 3 :
    li regtemp, 0x803400D8
  endif

  li regtemp2, valhartmask
  sd  regtemp2, 0(regtemp)
  fence iorw, iorw

pb_end

;
; Param Block: Wait for FCC
;
; cnt    : Fcc Counter to inspect
;

pb_start wait_fcc(cnt)

  csrwi fcc, cnt

pb_end


pb_start backup_fp_regs(memory, rega)
   ; Load the Memory addres to a register
   la rega, memory

   ; Registers used to store base addresses
   fsq2 f26,   0(rega)
   fsq2 f27,  32(rega)
   fsq2 f30,  64(rega)
   fsq2 f31,  96(rega)
pb_end


pb_start restore_fp_regs(memory, rega)
   ; Load the Memory addres to a register
   la rega, memory

   ; Registers used to store base addresses
   flq2 f26,   0(rega)
   flq2 f27,  32(rega)
   flq2 f30,  64(rega)
   flq2 f31,  96(rega)
pb_end


pb_start clear_SCP_values(rega, regb, regc, shireid)
   li regb, 0x00000901
   li regc, shireid
   slli regc, regc, 22

   li rega, 0x80F1E030
   add rega, rega, regc
   sd regb, 0(rega)

   fence iorw, iorw
pb_end


pb_start write_ESR_value(rega, regb, regc, shireid, esr_addr, esr_value)
   li regb, esr_value
   li regc, shireid
   slli regc, regc, 22

   li rega, esr_addr
   add rega, rega, regc
   sd regb, 0(rega)

   ;fence iorw, iorw
pb_end


pb_start enable_l1scp(regx1, regx2, en_l1scp_pass_label)
   exclusive_mode(set_it = 1, previous = x0)
   evict_all_l1(value = evict_csr_val, rega = csr_reg, regb = regx1, regc = regx2)
   set_l1_scp(value = l1_scp_csr_val, rega = csr_reg, regb = regx1)
   exclusive_mode(set_it = 0, previous = x0)

   ; self check
   csrr regx1, mcache_control
   li   regx2, 0x3
   beq  regx1, regx2, pass_labled_enable_l1scp
   test_fail(regz = regx1)

pass_labled_enable_l1scp:
pb_end

pb_start restore_dcache_mode(prev_val_reg, regx1, regx2)

   exclusive_mode(set_it = 1, previous = x0)
   evict_all_l1(value = 0, rega = csr_reg, regb = regx1, regc = regx2)

   ; unknown -> 00
   csrw  mcache_control, x0
   fence iorw, iorw
   wait_cacheops()

   beq prev_val_reg, x0, restore_done

   ; 00 -> 01
   addi  regx1, regx1, 1
   csrw  mcache_control, regx1
   fence iorw, iorw
   wait_cacheops()

   li regx2, 1
   beq prev_val_reg, regx2, restore_done

   ; 01 -> 11
   addi  regx1, regx1, 2
   csrw  mcache_control, regx1
   fence iorw, iorw
   wait_cacheops()

restore_done:
   exclusive_mode(set_it = 0, previous = x0)

pb_end


pb_start ecall_handler(rega, regb, m_label, s_label, u_label, ilegal_label)
   csrr rega, mcause
   li regb, 0xB ; m_ecall
   beq rega, regb, m_label
   li regb, 0x8 ; u_ecall
   beq rega, regb, u_label
   li regb, 0x9 ; s_ecall
   beq rega, regb, s_label
   li regb, 0x2 ; illegal_inst
   beq rega, regb, ilegal_label
pb_end


pb_start set_shire_coop_mode(rega, regc, esr_addr, esr_value)
   li regc, esr_addr

   ; Load the random value
   li rega, esr_value
   ; Write it to the ESR
   sd rega, 0(regc)
pb_end


pb_start set_dcache_bypass(loop_index, tmp_reg0, tmp_reg1, tmp_reg2)
   li  tmp_reg0, 0x80D00070
   
   for loop_index in (0, 4, 1):
      li    tmp_reg1, loop_index
      slli  tmp_reg1, tmp_reg1, 16
      add   tmp_reg2, tmp_reg0, tmp_reg1
      fence iorw, iorw
      ld    tmp_reg1, 0(tmp_reg2)
      ori   tmp_reg1, tmp_reg1, 0x2
      sd    tmp_reg1, 0(tmp_reg2)
      fence iorw, iorw
   endfor
pb_end

pb_start clr_dcache_bypass(tmp_reg0, tmp_reg1, tmp_reg2)
   li  tmp_reg0, 0x80D00070

   ; loop for each neighbourhood
   for loop_index in (0, 4, 1):
      li    tmp_reg1, loop_index
      slli  tmp_reg1, tmp_reg1, 16
      add   tmp_reg2, tmp_reg0, tmp_reg1

      fence iorw, iorw
      ld    tmp_reg1, 0(tmp_reg2)
      andi  tmp_reg1, tmp_reg1, 0x3d
      sd    tmp_reg1, 0(tmp_reg2)
      fence iorw, iorw
   endfor
pb_end

pb_start backup_x31()
   addi    x2, x2, -8
   sd      x31, 0(x2)
pb_end

pb_start restore_x31()
   ld      x31, 0(x2)
   addi    x2, x2, 8
pb_end

pb_start backup_rx(imm_rsx)
   ; Increase Stack Pointer
   addi x2, x2, -4
   if (imm_rsx == 0):
      sd x0, 0(x2)
   elif (imm_rsx == 1):
      sd x1, 0(x2)
   elif (imm_rsx == 2):
      sd x2, 0(x2)
   elif (imm_rsx == 3):
      sd x3, 0(x2)
   elif (imm_rsx == 4):
      sd x4, 0(x2)
   elif (imm_rsx == 5):
      sd x5, 0(x2)
   elif (imm_rsx == 6):
      sd x6, 0(x2)
   elif (imm_rsx == 7):
      sd x7, 0(x2)
   elif (imm_rsx == 8):
      sd x8, 0(x2)
   elif (imm_rsx == 9):
      sd x9, 0(x2)
   elif (imm_rsx == 10):
      sd x10, 0(x2)
   elif (imm_rsx == 11):
      sd x11, 0(x2)
   elif (imm_rsx == 12):
      sd x12, 0(x2)
   elif (imm_rsx == 13):
      sd x13, 0(x2)
   elif (imm_rsx == 14):
      sd x14, 0(x2)
   elif (imm_rsx == 15):
      sd x15, 0(x2)
   elif (imm_rsx == 16):
      sd x16, 0(x2)
   elif (imm_rsx == 17):
      sd x17, 0(x2)
   elif (imm_rsx == 18):
      sd x18, 0(x2)
   elif (imm_rsx == 19):
      sd x19, 0(x2)
   elif (imm_rsx == 20):
      sd x20, 0(x2)
   elif (imm_rsx == 21):
      sd x21, 0(x2)
   elif (imm_rsx == 22):
      sd x22, 0(x2)
   elif (imm_rsx == 23):
      sd x23, 0(x2)
   elif (imm_rsx == 24):
      sd x24, 0(x2)
   elif (imm_rsx == 25):
      sd x25, 0(x2)
   elif (imm_rsx == 26):
      sd x26, 0(x2)
   elif (imm_rsx == 27):
      sd x27, 0(x2)
   elif (imm_rsx == 28):
      sd x28, 0(x2)
   elif (imm_rsx == 29):
      sd x29, 0(x2)
   elif (imm_rsx == 30):
      sd x30, 0(x2)
   elif (imm_rsx == 31):
      sd x31, 0(x2)
   else:
      test_fail(regz = x31)
   endif
pb_end

pb_start restore_rx(imm_rsx)
   if (imm_rsx == 0):
      ld x0, 0(x2)
   elif (imm_rsx == 1):
      ld x1, 0(x2)
   elif (imm_rsx == 2):
      ld x2, 0(x2)
   elif (imm_rsx == 3):
      ld x3, 0(x2)
   elif (imm_rsx == 4):
      ld x4, 0(x2)
   elif (imm_rsx == 5):
      ld x5, 0(x2)
   elif (imm_rsx == 6):
      ld x6, 0(x2)
   elif (imm_rsx == 7):
      ld x7, 0(x2)
   elif (imm_rsx == 8):
      ld x8, 0(x2)
   elif (imm_rsx == 9):
      ld x9, 0(x2)
   elif (imm_rsx == 10):
      ld x10, 0(x2)
   elif (imm_rsx == 11):
      ld x11, 0(x2)
   elif (imm_rsx == 12):
      ld x12, 0(x2)
   elif (imm_rsx == 13):
      ld x13, 0(x2)
   elif (imm_rsx == 14):
      ld x14, 0(x2)
   elif (imm_rsx == 15):
      ld x15, 0(x2)
   elif (imm_rsx == 16):
      ld x16, 0(x2)
   elif (imm_rsx == 17):
      ld x17, 0(x2)
   elif (imm_rsx == 18):
      ld x18, 0(x2)
   elif (imm_rsx == 19):
      ld x19, 0(x2)
   elif (imm_rsx == 20):
      ld x20, 0(x2)
   elif (imm_rsx == 21):
      ld x21, 0(x2)
   elif (imm_rsx == 22):
      ld x22, 0(x2)
   elif (imm_rsx == 23):
      ld x23, 0(x2)
   elif (imm_rsx == 24):
      ld x24, 0(x2)
   elif (imm_rsx == 25):
      ld x25, 0(x2)
   elif (imm_rsx == 26):
      ld x26, 0(x2)
   elif (imm_rsx == 27):
      ld x27, 0(x2)
   elif (imm_rsx == 28):
      ld x28, 0(x2)
   elif (imm_rsx == 29):
      ld x29, 0(x2)
   elif (imm_rsx == 30):
      ld x30, 0(x2)
   elif (imm_rsx == 31):
      ld x31, 0(x2)
   else:
      test_fail(regz = x31)
   endif
   ; Recover Stack Pointer
   addi x2, x2, 4
pb_end

pb_start load_address_imm_rsx(imm_rsx, imm_addr)
   if (imm_rsx == 0):
      la x0, imm_addr
   elif (imm_rsx == 1):
      la x1, imm_addr
   elif (imm_rsx == 2):
      la x2, imm_addr
   elif (imm_rsx == 3):
      la x3, imm_addr
   elif (imm_rsx == 4):
      la x4, imm_addr
   elif (imm_rsx == 5):
      la x5, imm_addr
   elif (imm_rsx == 6):
      la x6, imm_addr
   elif (imm_rsx == 7):
      la x7, imm_addr
   elif (imm_rsx == 8):
      la x8, imm_addr
   elif (imm_rsx == 9):
      la x9, imm_addr
   elif (imm_rsx == 10):
      la x10, imm_addr
   elif (imm_rsx == 11):
      la x11, imm_addr
   elif (imm_rsx == 12):
      la x12, imm_addr
   elif (imm_rsx == 13):
      la x13, imm_addr
   elif (imm_rsx == 14):
      la x14, imm_addr
   elif (imm_rsx == 15):
      la x15, imm_addr
   elif (imm_rsx == 16):
      la x16, imm_addr
   elif (imm_rsx == 17):
      la x17, imm_addr
   elif (imm_rsx == 18):
      la x18, imm_addr
   elif (imm_rsx == 19):
      la x19, imm_addr
   elif (imm_rsx == 20):
      la x20, imm_addr
   elif (imm_rsx == 21):
      la x21, imm_addr
   elif (imm_rsx == 22):
      la x22, imm_addr
   elif (imm_rsx == 23):
      la x23, imm_addr
   elif (imm_rsx == 24):
      la x24, imm_addr
   elif (imm_rsx == 25):
      la x25, imm_addr
   elif (imm_rsx == 26):
      la x26, imm_addr
   elif (imm_rsx == 27):
      la x27, imm_addr
   elif (imm_rsx == 28):
      la x28, imm_addr
   elif (imm_rsx == 29):
      la x29, imm_addr
   elif (imm_rsx == 30):
      la x30, imm_addr
   elif (imm_rsx == 31):
      la x31, imm_addr
   endif
pb_end

pb_start et_jump_to_m_mode()
   ; x2 == stack pointer
   addi x2, x2, -8
   sd x10, 0(x2)
   jump_to_machine_mode(tm = TEST_PROPS.TEST_MODE)
   ld x10, 0(x2)
   addi x2, x2, 8
pb_end

pb_start wait_for_sim_end()
if (ET_TEST_ENV.SILICON_MODE == 0):
   wait_for_end_of_sim:
   if (ET_TEST_ENV.EOT_ZEBU_HINTS == 1):
      if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE || TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.SUPERVISOR):
            ; WFI is only valid in MACHINE/SUPERVISOR
            wfi
      else: ; Call mhandler
            et_jump_to_m_mode()
      endif
   else:
        csrwi stall, 0x1
   endif
endif
pb_end

pb_start wait_iprefetch(pb_rx1, pb_rx2)
if TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.USER:
    li pb_rx1, 0x803402F8
endif
if TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.SUPERVISOR:
    li pb_rx1, 0x80740300
endif
if TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE:
    li pb_rx1, 0x80F40308
endif
wait_any_prev_iprefetch:
    ld pb_rx2, 0(pb_rx1)
    beq pb_rx2, x0, wait_any_prev_iprefetch
pb_end
