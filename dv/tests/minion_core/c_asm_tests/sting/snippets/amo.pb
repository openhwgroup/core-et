
pb_start amo_storeg_d(addr, store_value, regx1, regx2)
   li regx1, store_value
   la regx2, addr
   amoswapg.d x0, regx2, regx1
pb_end

pb_start amoaddg_d(addr, inc_value, regx1, regx2)
   li regx1, inc_value
   la regx2, addr
   amoaddg.d regx1, regx2, regx1
pb_end

pb_start amo_loadg_d(addr, regx1)
   la regx1, addr
   amoorg.d regx1, regx1, x0
pb_end

pb_start cpu_relax(regx1)
   div regx1, regx1, x0
pb_end

pb_start amo_barrier_init(addr, init_value, regx1, regx2)
   amo_storeg_d(addr = addr, store_value = init_value, regx1 = regx1, regx2 = regx2)
pb_end

pb_start amo_barrier_wait(addr, regx1)
amo_load:
   amo_loadg_d(addr = addr, regx1 = regx1) 
   beq x0, regx1, done
   cpu_relax(regx1 = regx1)
   j amo_load
done:
pb_end

pb_start amo_barrier(num_thr, addr, regx1, regx2)
start_amo_barrier:
   amoaddg_d(addr = addr, inc_value = -1, regx1 = regx1, regx2 = regx2)
   li regx2, 1
   bge regx2, regx1, done_amo_barrier
   amo_barrier_wait(addr = addr, regx1 = regx1)
   li regx2, 0
done_amo_barrier:
pb_end

pb_start copy_fs1_to_fd(fd, fs1)
   fcmovm.ps fd, fs1, fs1
pb_end
