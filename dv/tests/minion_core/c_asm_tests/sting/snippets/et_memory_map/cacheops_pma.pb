

;  ParamBlock: Get set from address
;
;  set    : Register has the set value from @
;  xaddr  : @ from set value is getting
;
pb_start get_set(r_set, r_addr)

  srli r_set, r_addr, 6
  andi r_set, r_set, 15

pb_end


;  ParamBlock: Get set from address
;
;  level  : Register has the level value from @
;  addr   : @ from level value is getting
;
pb_start get_destlevel(r_destlevel, r_addr)

  srli r_destlevel, r_addr, 58
  addi r_destlevel, r_destlevel, 3

pb_end


;  Param Block: Set cacheop with random csr_val
;
;  csr_cacheop : cacheop csr to be use
;  rega        : register with address memory
;  regb        : Tmp register
;  regc        : Tmp register
;  mask1       : csr mask
;  ndx         : Define wich csr_cacheop is using
;  cacheop_inst is getting from the pb call file
pb_start set_cacheopCsr(csr_cacheop, rega, regb, regc, mask1, regset, regdestlevel, ndx)

  cacheop_inst = ndx

  ; 16 sets x 4 way x64 bytes = 4096 byte = 4kb
  ; Fill all 4 ways of the current set with addresses of mem region in rega

  add regb, x0, rega

  get_set(r_set = regset, r_addr = rega)

  get_destlevel(r_destlevel = regdestlevel, r_addr = rega)

  add regb, x0, rega
  li  regc, mask1
  and regb, regb, regc 
  
  csrw csr_cacheop, regb
  
pb_end


;  Param Block: Set cacheop with random csr_val which use VA
;
;  csr_cacheop : cacheop csr to be use
;  rega        : register with address loaded
;  regb        : Tmp register
;  regc        : Tmp register
;  mask1       : Mask applied to rega
;  mask2       : Mask applied to regb if cacheop use VA address
;  ndx         : Define wich csr_cacheop instruction is using
pb_start set_cacheopCsrVA(csr_cacheop, rega, regb, regc, mask1, mask2, regdestlevel, ndx)

  cacheop_inst = ndx

  get_destlevel(r_destlevel = regdestlevel, r_addr = rega)

  li regc, mask1
  add regb, x0, rega  
  and regb, regb, regc

  li regc, mask2
  and regc, rega, regc

  or regb, regb, regc
  
  csrw csr_cacheop, regb
  
pb_end


;  Param Block: Set cacheop with random csr_val which use lockSW before evict all way of the set
;
;  rega        : register with address loaded
;  regb        : Tmp register
;  regc        : Tmp register
;  mask1       : Mask applied to rega
;  mask2       : Mask applied to regb if cacheop use VA address
;  ndx         : Define wich csr_cacheop instruction is using
pb_start set_cacheopCsrLockSw(rega, regb, regc, mask1, mask2, regdestlevel, ndx, jmptofail)

  ; for this evict_sw no error is expected
  li regc, 0x3C0         ; mask for SET bits	
  and regb, regc, rega
  slli regb, regb, 8     ; SET start in bit 14
  addi regc, x0, 1
  slli regc, regc, 58
  or regb, regc, regb    ; add dest_level l2

  csrw evict_sw, regb    ; evict way_0   
  wait_cacheops()
  ; No tensor error expected at this point check for assurance
  csrrs regc, tensor_error, x0
  bne regc, x0, jmptofail

  add regc, regb, x0    ; cpy with way0
  ori regc, regc, 0x40
  csrw evict_sw, regc    ; evict way_1 
  wait_cacheops()        
  ; No tensor error expected at this point check for assurance
  csrrs regc, tensor_error, x0
  bne regc, x0, jmptofail

  add regc, regb, x0     ; cpy with way0
  ori regc, regc, 0x80
  csrw evict_sw, regc     ; evict way_2  
  wait_cacheops()         
  ; No tensor error expected at this point check for assurance
  csrrs regc, tensor_error, x0
  bne regc, x0, jmptofail

  add regc, regb, x0      ; cpy with way0
  ori regc, regc, 0xC0
  csrw evict_sw, regc     ; evict way_3      
  wait_cacheops()        
  ; No tensor error expected at this point check for assurance
  csrrs regc, tensor_error, x0
  bne regc, x0, jmptofail

  cacheop_inst = ndx

  get_destlevel(r_destlevel = regdestlevel, r_addr = rega)

  li regc, mask1
  add regb, x0, rega  
  and regb, regb, regc

  li regc, mask2
  and regc, rega, regc

  or regb, regb, regc
  
  csrw lock_sw, regb  

pb_end