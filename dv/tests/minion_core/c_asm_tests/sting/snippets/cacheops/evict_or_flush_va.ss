resource
cpu_t   x

unum_t csr_enc   = snp_randnum()
unum_t tmask     = snp_urandrange(0,65536)
unum_t repeat    = snp_urandrange(0, 2)
unum_t tensor_id = snp_urandrange(0, 2)
unum_t do_evict  = snp_urandrange(0, 2)
unum_t x31_enc   = 0

axreg_t csr_reg
axreg_t mask_reg
axreg_t saved_x31
axreg_t va

base_t  base_addr

snip_setup
    ; Empty

snip_init
    ; Empty

snip_run
@cpu: x

    ; Save x31
    addi saved_x31, x31, 0

    ;
    ; csr_reg is the register used to write to CSR
    ; mask_reg is used for masking
    ; base_addr contains a VA
    ; All equally random, except VA and bottom 4 bits (repeat count)
    ; to avoid destroying too much cache state too often
    ;
    csr_enc = snp_and(csr_enc, 0xffff000000000030)

if repeat > 0:
    ; Select the repeat count, with a bias on smaller values
    SELECT 1 {
     16 : csr_enc = snp_or(csr_enc, 1)
     12 : csr_enc = snp_or(csr_enc, 2)
     12 : csr_enc = snp_or(csr_enc, 3)
     10 : csr_enc = snp_or(csr_enc, 4)
     10 : csr_enc = snp_or(csr_enc, 5)
     10 : csr_enc = snp_or(csr_enc, 6)
     10 : csr_enc = snp_or(csr_enc, 7)
     8  : csr_enc = snp_or(csr_enc, 8)
     8  : csr_enc = snp_or(csr_enc, 9)
     4  : csr_enc = snp_or(csr_enc, 10)
     4  : csr_enc = snp_or(csr_enc, 11)
     2  : csr_enc = snp_or(csr_enc, 12)
     2  : csr_enc = snp_or(csr_enc, 13)
     1  : csr_enc = snp_or(csr_enc, 14)
     1  : csr_enc = snp_or(csr_enc, 15)
    }

    ; Randomize stride to some cache line multiples
    SELECT 1 {
     1 : x31_enc = snp_or(x31_enc, 64)
     1 : x31_enc = snp_or(x31_enc, 128)
     1 : x31_enc = snp_or(x31_enc, 256)
     1 : x31_enc = snp_or(x31_enc, 512)
    }
endif
    x31_enc = snp_or(x31_enc, tensor_id)
    li x31, x31_enc
    li csr_reg, csr_enc

    ; add VA
    li  mask_reg, 0xffffffffffc0
    and va, base_addr, mask_reg
    or  csr_reg, csr_reg, va

; Load the Tensor Mask CSR if using it
if csr_enc >= 0x8000000000000000:
    li mask_reg, tmask
    csrw tensor_mask, mask_reg
endif

; Start the cacheop
if do_evict > 0:
    csrw evict_va, csr_reg
else:
    csrw flush_va, csr_reg
endif

    ; Restore x31
    addi x31, saved_x31, 0

snip_check
    ; Empty


