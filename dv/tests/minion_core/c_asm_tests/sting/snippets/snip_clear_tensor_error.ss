
resource
unum_t cpuid = snp_cpu_id()

snip_setup
   ; Empty

snip_init
@cpu: *
   csrrw x0, tensor_error, x0


snip_run
   ; Empty

snip_check
   ; Empty
