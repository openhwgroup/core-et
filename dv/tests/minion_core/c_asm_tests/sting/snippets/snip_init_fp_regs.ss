
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"

resource

unum_t  index = 0
unum_t  rand_value = 0
axreg_t rx1, rx2
base_t  mem_reg[64]@64

unum_t     cpuid = snp_cpu_id()
gmemory_t  mem_fp[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION

snip_setup
    ; Empty
    ; For current Sting version, this is a placeholder


snip_init
   ; Empty

snip_run
@cpu: *

   ; The goal of this snippet is to load into the FP registers
   ; random values (typically different than 0)

   backup_fp_regs(memory = mem_fp[cpuid], rega = rx1)

   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f0)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f1)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f2)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f3)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f4)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f5)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f6)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f7)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f8)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f9)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f10)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f11)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f12)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f13)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f14)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f15)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f16)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f17)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f18)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f19)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f20)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f21)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f22)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f23)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f24)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f25)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f26)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f27)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f28)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f29)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f30)
   rand_value = snp_randnum()
   load_rand_fp(loop_index = index, num_var = rand_value, reg_mem = mem_reg, rega = rx1, regb = rx2, reg_fp = f31)

   restore_fp_regs(memory = mem_fp[cpuid], rega = rx1)

   fence iorw, iorw 

snip_check
   ; Empty
