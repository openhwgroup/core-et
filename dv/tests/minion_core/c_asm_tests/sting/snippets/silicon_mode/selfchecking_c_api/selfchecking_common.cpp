
#include "selfchecking_common.h"

int compare_vpurf(uint64_t hid, uint64_t* vpurf_a, uint64_t* vpurf_b) {
   const unsigned int SIZE = LANES_PER_VPU_REG / (sizeof(uint64_t)/BYTES_PER_LANE);
   uint64_t (*cmp_vpurf_a)[SIZE] = (uint64_t (*)[SIZE]) vpurf_a;
   uint64_t (*cmp_vpurf_b)[SIZE] = (uint64_t (*)[SIZE]) vpurf_b;
   for (int i = 0; i < VPURF_NUM_REGS; i++) {
      for (int j = 0; j < SIZE; j++) {
         if (cmp_vpurf_a[i][j] != cmp_vpurf_b[i][j]) {
            et_printf("[dbg] H%d error: vpu mismatch\n", hid);
            et_printf("[dbg] H%d   vpu_a(chk) : f%u[%u/%u] = 0x%lx (addr: %lx)\n", hid, i, j / 2, j / 2 + 1, cmp_vpurf_a[i][j], &cmp_vpurf_a[i][j]);
            et_printf("[dbg] H%d   vpu_b(dut) : f%u[%u/%u] = 0x%lx (addr: %lx)\n", hid, i, j / 2, j / 2 + 1, cmp_vpurf_b[i][j], &cmp_vpurf_b[i][j]);
            C_TEST_FAIL
            return NOT_PASS;
         }
      }
   }
   return PASS;
}

void print_vpurf_fs1(uint64_t hid, uint64_t* vpurf_a, uint64_t fs1) {
   const unsigned int SIZE = LANES_PER_VPU_REG / (sizeof(uint32_t)/BYTES_PER_LANE);
   uint32_t (*cmp_vpurf_a)[SIZE] = (uint32_t (*)[SIZE]) vpurf_a;
   et_printf("[dbg] H%d f%d : { 0:0x%x 1:0x%x 2:0x%x 3:0x%x 4:0x%x 5:0x%x 6:0x%x 7:0x%x }\n", hid, fs1, \
              cmp_vpurf_a[fs1][0], cmp_vpurf_a[fs1][1], cmp_vpurf_a[fs1][2], cmp_vpurf_a[fs1][3], \
              cmp_vpurf_a[fs1][4], cmp_vpurf_a[fs1][5], cmp_vpurf_a[fs1][6], cmp_vpurf_a[fs1][7]);
}

void print_vpurf_fd(uint64_t hid, uint64_t* vpurf_a, uint64_t fd) {
   const unsigned int SIZE = LANES_PER_VPU_REG / (sizeof(uint32_t)/BYTES_PER_LANE);
   uint32_t (*cmp_vpurf_a)[SIZE] = (uint32_t (*)[SIZE]) vpurf_a;
   et_printf("[dbg] H%d f%d = { 0:0x%x 1:0x%x 2:0x%x 3:0x%x 4:0x%x 5:0x%x 6:0x%x 7:0x%x }\n", hid, fd, \
              cmp_vpurf_a[fd][0], cmp_vpurf_a[fd][1], cmp_vpurf_a[fd][2], cmp_vpurf_a[fd][3], \
              cmp_vpurf_a[fd][4], cmp_vpurf_a[fd][5], cmp_vpurf_a[fd][6], cmp_vpurf_a[fd][7]);
}

void print_tquant_l1scp(uint64_t hid, uint64_t* l1scp, uint64_t line, uint64_t start) {
   const uint32_t elems_per_scp_line = L1SCP_LINE_SIZE_BYTES / sizeof(uint32_t);
   uint32_t (*l1scp_a)[elems_per_scp_line] = (uint32_t (*)[elems_per_scp_line]) l1scp;

   et_printf("[dbg] H%d SCP[%d] = { 0:0x%x 1:0x%x 2:0x%x 3:0x%x 4:0x%x 5:0x%x 6:0x%x 7:0x%x }\n", hid, line, \
              l1scp_a[line][start+0], l1scp_a[line][start+1], l1scp_a[line][start+2], l1scp_a[line][start+3], \
              l1scp_a[line][start+4], l1scp_a[line][start+5], l1scp_a[line][start+6], l1scp_a[line][start+7]);
}

void print_mem_w(uint64_t hid, uint64_t addr) {
   int nelems = 128 / 32;
   uint32_t *mem = (uint32_t*) addr;
   for (int i = 0; i < nelems; i++) {
      et_printf("[dbg] H%d MEM128[0x%lx] = { 0:0x%x 1:0x%x 2:0x%x 3:0x%x }\n", hid, addr, \
                 mem[0], mem[1], mem[2], mem[3]);
   }
}
