resource
cpu_t   x

num_t   csr_val = snp_randnum()
num_t   tmask = snp_urandrange(0,65536)
num_t   repeat = snp_urandrange(0,2)
num_t   tensor_id = snp_urandrange(0, 2)

axreg_t csr_enc
axreg_t mask_reg
axreg_t saved_x31
axreg_t va

base_t  rb1

snip_setup
    ; Empty

snip_init
    ; Empty

snip_run
@cpu: x

    ; Save x31
    addi saved_x31, x31, 0

    ;
    ; csr_enc is the register used to write to CSR
    ; mask_reg is used for masking
    ; rb1 contains a VA
    ; All equally random, except VA and bottom 4 bits (repeat count)
    ; to avoid destroying too much cache state too often
    ;
    li  csr_enc, csr_val
    li  mask_reg, 0xffff000000000030
    and csr_enc, csr_enc, mask_reg

    ; add VA
    li   mask_reg, 0xffffffffffc0
    and  va, rb1, mask_reg
    or   csr_enc, csr_enc, va
    ;

    li x31, tensor_id
if repeat > 0:
    ; Select the repeat count, with a bias on smaller values
    SELECT 1 {
     16 : ori csr_enc, csr_enc, 1
     12 : ori csr_enc, csr_enc, 2
     12 : ori csr_enc, csr_enc, 3
     10 : ori csr_enc, csr_enc, 4
     10 : ori csr_enc, csr_enc, 5
     10 : ori csr_enc, csr_enc, 6
     10 : ori csr_enc, csr_enc, 7
     8  : ori csr_enc, csr_enc, 8
     8  : ori csr_enc, csr_enc, 9
     4  : ori csr_enc, csr_enc, 10
     4  : ori csr_enc, csr_enc, 11
     2  : ori csr_enc, csr_enc, 12
     2  : ori csr_enc, csr_enc, 13
     1  : ori csr_enc, csr_enc, 14
     1  : ori csr_enc, csr_enc, 15
    }

    ; Randomize stride to some cache line multiples
    SELECT 1 {
     1 : ori x31, x31, 64
     1 : ori x31, x31, 128
     1 : ori x31, x31, 256
     1 : ori x31, x31, 512
    }
endif


; Load the Tensor Mask CSR if using it
if csr_val >= 0x8000000000000000:
    li mask_reg, tmask
    csrw tensor_mask, mask_reg
endif

    csrw prefetch_va, csr_enc

    ; Restore x31
    addi x31, saved_x31, 0

snip_check
    ; Empty

