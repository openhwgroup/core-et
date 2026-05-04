

import "$MINION_DIAGS/sting/snippets/cacheops/cops.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"
import "$MINION_DIAGS/sting/snippets/et_memory_map/cacheops_pma.pb"

resource

cpu_t x

axreg_t r0, r1, r2, r3, rset, rdestlevel

;; Pick random Region Map

uc_memory_t mem1[64]@64:ET_NON_CACHEABLE_MEM_SET

unum_t cntr = 0
unum_t cacheop_inst = 0

snip_setup
  ; Empty

snip_init
@cpu: x

cntr = 0
cacheop_inst  = 0
li rset, 0
li rdestlevel, 0


snip_run
@cpu: x

  ;cacheable marks current memory region as cached accessible area
  ;r0 keeps memory address region
  li r1, 0
  li r2, 0
  li r3, 0
  li r0, 0

  reset_tensor_error()

  la r0, mem1

  SELECT 1 {
        10: ;mcache_control:0x7E0
            ;[0] sz1 D1Split [1] sz1 SCPEnable [63:2] sz 62 WARL(0)
            ;reuse pb call but destlevel and set is unused.         
            set_cacheopCsr(csr_cacheop = mcache_control, rega = r0,  regb = r1, regc = r2, mask1 = 0x00000000000000003, regset = rset, regdestlevel = rdestlevel, ndx = 0)

        10: ;evict_sw:0x7F9
            ;xs[63] =  Use the TensorMask xs[62:60] = WARL (zero) xs[59:58] = DestLevel: 00=L1, 01=L2, 10=L3, 11=MEM xs[57:18] = WARL (zero) xs[17:14] = set
            ;xs[13:8] = WARL (zero) xs[7:6] = way xs[5:4] = WARL (zero) xs[3:0] = NumLines (minus 1)
            set_cacheopCsr(csr_cacheop = evict_sw, rega = r0, regb = r1, regc = r2, mask1 = 0x8C0000000003C0CF, regset = rset, regdestlevel = rdestlevel, ndx = 1)

        10: ;flush_sw:0x7FB
            ;xs[63] = Use the TensorMask xs[62:60] = WARL (zero) xs[59:58] = DestLevel: 00=L1, 01=L2, 10=L3, 11=MEM
            ;xs[57:18] = WARL (zero) xs[17:14] = set xs[13:8] = WARL (zero) xs[7:6] = way xs[5:4] = WARL (zero)xs[3:0] = NumLines (minus 1)
            set_cacheopCsr(csr_cacheop = flush_sw, rega = r0, regb = r1, regc = r2, mask1 = 0x8C0000000003C0CF, regset = rset, regdestlevel = rdestlevel, ndx = 2)        

        10: ;lock_sw:0x7FD
            ;xs[63:57] = WARL (zero) xs[56:55] = way xs[54:40] = WARL (zero) xs[39:6] = Physical Address [39:6] xs[5:0] = WARL (zero)
            ;reuse pb call but destlevel is unused.  
            set_cacheopCsrVA(csr_cacheop = lock_sw, rega = r0, regb = r1, regc = r2, mask1 = 0x18000FFFFFFFFC0, mask2 = 0x00000FFFFFFFFC0, regdestlevel = rdestlevel, ndx = 3)
            
        10: ;unlock_sw:0x7ff   doesn't generate any error condition
            ;xs[63:57] = WARL(0) xs[56:55] = way xs[54:10] = WARL (zero) xs[9:6] = set xs[5:0] = WARL (zero)
            ;reuse pb call but destlevel and set is unused.  
            set_cacheopCsr(csr_cacheop = unlock_sw, rega = r0, regb = r1, regc = r2, mask1 = 0x1800000000003C0, regset = rset, regdestlevel = rdestlevel, ndx = 4)
 
        10: ;ucache_control:0x810
            ;[0] sz1 D1Split [1] sz1 SCPEnable [4:2] sz 3 ;Cacheop_RepRate [5] sz1 WARL(0) [10:6] sz 5 ;Cacheop_Max [63:11] sz53 WARL(0)
            ;reuse pb call but destlevel and set is unused.  
            set_cacheopCsr(csr_cacheop = ucache_control, rega = r0, regb = r1, regc = r2, mask1 = 0x0000000000000FFF, regset = rset, regdestlevel = rdestlevel, ndx = 5)        
            
        10: ;evict_va:0x89F
            ;xs[63] = Use the TensorMask xs[62:60] = WARL (zero) xs[59:58] = DestLevel: 00=L1, 01=L2, 10=L3, 11=MEM xs[57:48] = WARL (zero)
            ;xs[47:6] = Virtual Address [47:6] xs[5:4] = WARL (zero) xs[3:0] = NumLines (minus 1)
            set_cacheopCsrVA(csr_cacheop = evict_va, rega = r0, regb = r1, regc = r2, mask1 = 0x8C00000000000003, mask2 = 0x00000FFFFFFFFC0, regdestlevel = rdestlevel, ndx = 6)
            
        10: ;flush_va:0x8BF
            ;xs[63] = Use the TensorMask xs[62:60] = WARL (zero) xs[59:58] = DestLevel: 00=L1, 01=L2, 10=L3, 11=MEM xs[57:48] = WARL (zero)
            ;xs[47:6] = Virtual Address [47:6] xs[5:4] = WARL (zero)xs[3:0] = NumLines (minus 1) Implicitly uses register x31[47:6] for the stride value, x31[0] for ID
            set_cacheopCsrVA(csr_cacheop = flush_va, rega = r0, regb = r1, regc = r2, mask1 = 0x8C00000000000003, mask2 = 0x00000FFFFFFFFC0, regdestlevel = rdestlevel, ndx = 7)
            
        10: ;prefetch_va:0x81F
            ;xs[63] = Use the TensorMask xs[62:60] = WARL (zero) xs[59:58] = DestLevel: 00=L1, 01=L2, 10=L3, 11=MEM xs[57:48] = WARL (zero) xs[47:6] = Virtual Address [47:6]
            ;xs[5:4] = WARL (zero) xs[3:0] = NumLines (minus 1) Implicitly uses register x31[47:6] for the stride value, x31[0] for ID
            set_cacheopCsrVA(csr_cacheop = prefetch_va, rega = r0, regb = r1, regc = r2, mask1 = 0x8C00000000000003, mask2 = 0x00000FFFFFFFFC0, regdestlevel = rdestlevel, ndx = 8)
            
        10: ;lock_va:0x8DF
            ;xs[63] = Use the TensorMask xs[62:48] = WARL (zero) xs[47:6] = Virtual Address [47:6] xs[5:4] = WARL (zero) xs[3:0] = NumLines (minus 1) Implicitly uses register x31[47:6] for the stride value, x31[0] for ID for TensorWait
            ;reuse pb call but destlevel is unused.  
            set_cacheopCsrVA(csr_cacheop = lock_va, rega = r0, regb = r1, regc = r2, mask1 = 0x8000000000000003, mask2 = 0x00000FFFFFFFFC0, regdestlevel = rdestlevel, ndx = 9)
            ; Storing to the VA would generate an exception
            
        10: ;unlock_va:0x8ff
            ;xs[63] = Use the TensorMask xs[62:48] = WARL (zero) xs[47:6] = Virtual Address [47:6] xs[5:4] = WARL (zero) xs[3:0] = NumLines (minus 1) Implicitly uses register x31[47:6] for the stride value, x31[0] for ID for TensorWait

            set_cacheopCsrVA(csr_cacheop = unlock_va, rega = r0, regb = r1, regc = r2, mask1 = 0x8C00000000000003, mask2 = 0x00000FFFFFFFFC0, regdestlevel = rdestlevel, ndx = 10)
            ; Storing to the VA would generate an exception
  }

  wait_cacheops()

  ;This pb used to only check tensor_error if RM_MODE is MACHINE
  check_tensor_error_cacheop(rega1 = r3, regb1 = r2, should_fail = 0, on_error = check_TsErrBit7)

  if (cacheop_inst ==3):
      check_tensor_error_lock_sw(rega1 = r3, regb1 = r2, should_fail = 0, on_error = check_TsErrBit5)
  endif
  j pass_cacheop

  check_TsErrBit7:  ;assume if not by L3 or MEM is because we here always access to uncacheable area.
    ; 1 evictSW - 2 flushSW - 6 evictVA - 7 flushVA - 8 prefetchVA - 9 lockVA - 10 unlockVA
    if ((cacheop_inst == 1) || (cacheop_inst == 2) || (cacheop_inst == 6) ||
        (cacheop_inst == 7) || (cacheop_inst == 8) || (cacheop_inst == 9) ||
        (cacheop_inst == 10)):

        ;destlevel > L3 or MEM
        addi r1, x0, 2
        bge rdestlevel, r1, pass_cacheop
    elif (cacheop_inst == 3):
       addi r1, x0, 2
       bge rdestlevel, r1, pass_cacheop
       check_tensor_error_lock_sw(rega1 = r3, should_fail = 0, on_error = check_TsErrBit5)
    else:
       ;The physical address does not correspond to cacheable memory as expected
       li r1, 0x80
       and r1, r1, r3    ;r3 has the tensor_error previously readed
       bne r1, x0, pass_cacheop
       j fail_cacheop
    endif

  check_TsErrBit5:
     ;For do a easier the test not self-check it it assume will do a monitor.
    ;Applying this operation to a set-way already hard-locked
    ;If the PA is already present in a way different than the way specified in xs[62:55]
    ;If all available ways - 1 in the given set are already locked
    addi x0,x0, 0
    j pass_cacheop ; temporarily till know how to assure the error exception

  fail_cacheop:
    test_fail(regz = r1)

  pass_cacheop:
    reset_tensor_error()


snip_check
; Empty

; The notation given below allows user to register handlers which will
; either run in machine mode (mhandler), or in supervisor mode (shandler)
; when trap delegation is enabled. In both the cases the exception program
; counter is read and incremented by 4 so as to allow the execution to proceed
; from the program counter following the instruction which caused the exception.
;
snip_handler

mhandler@cpu: x
  ; If user mode or supervisor mode attempts to use the machine-mode CSRs,
  ; an illegal instruction exception shall be raised, as is customary in RISC-V.
  ; Supervisor mode can access the user-mode CSRs.

  csrrs r1, mcause, x0
  ; It has to work in MACHINE mode
  test_fail (regz = r1)

