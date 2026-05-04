
#include "snp_tensor_reduce_auto.h"
#include "snp_common.h"
#include "et_soc.h"
#include "board_conf.h"
#include "assert.h"

#include <queue>

#define TR_SND   0b00
#define TR_RCV   0b01
#define TR_BCAST 0b10
#define TR_AUTO  0b11

#define pwrtwo(x) (1 << (x))
#define MAX_TREE_LVL 10

unum_t snp_get_tensor_reduce_partner(CpuResource * cpu_resource, unum_t nharts, unum_t cpu_x) {
   cpu_x = cpu_x >> 1;
   unum_t nminions = nharts >> 1;
   unum_t partner;
   if (nminions > 2) {
      partner = (cpu_x + 2)%nminions;
   }
   else {
      partner = (cpu_x + 1)%nminions;
   }
   return (partner << 1);
}

unum_t snp_set_legal_reduce_op(CpuResource * cpu_resource, unum_t rand_val, unum_t gl_rand_val) {
    //unum_t op_to_perform = snp_get_bits(cpu_resource, gl_rand_val, 27, 24);
    // As not all the possible bit combinations are legal, we have to constrain it to the legal values
    //op_to_perform = op_to_perform % 7;
    unum_t op_to_perform = snp_urandrange(cpu_resource, 0, 7);
    switch (op_to_perform) {
        case 0:
            op_to_perform = 0; // fadd
            break;
        case 1:
            op_to_perform = 2; // fmax
            break;
        case 2:
            op_to_perform = 3; // fmin
            break;
        case 3:
            op_to_perform = 4; // iadd
            break;
        case 4:
            op_to_perform = 6; // imax
            break;
        case 5:
            op_to_perform = 7; // imin
            break;
        case 6:
            op_to_perform = 8; // fget
            break;
    }
    // Set the Global Operation to Perform
    rand_val = snp_set_bits(cpu_resource, rand_val, 27, 24, op_to_perform);

    return rand_val;
}

unum_t snp_set_illegal_reduce_op(CpuResource * cpu_resource, unum_t csr_val) {
    unum_t op_to_perform = snp_urandrange(cpu_resource, 0, 9);
    switch (op_to_perform) {
        case 0:
            op_to_perform = 0b0001;
            break;
        case 1:
            op_to_perform = 0b0101;
            break;
        case 2:
            op_to_perform = 0b1001;
            break;
        case 3:
            op_to_perform = 0b1010;
            break;
        case 4:
            op_to_perform = 0b1011;
            break;
        case 5:
            op_to_perform = 0b1100;
            break;
        case 6:
            op_to_perform = 0b1101;
            break;
        case 7:
            op_to_perform = 0b1110;
            break;
        case 8:
            op_to_perform = 0b1111;
            break;
    }
    // Set the Global Operation to Perform
    csr_val = snp_set_bits(cpu_resource, csr_val, 27, 24, op_to_perform);

    return csr_val;
}

//Given a tree level, returns true if it is feasible for the current minion_map
unum_t snp_tree_compatible(CpuResource * cpu_resource, unum_t tree, unum_t* minion_map) {
    unum_t step    = pwrtwo(tree+1);
    for (unum_t i = 0; i < 1024; i += step) {
        unum_t partner = (i + step) - pwrtwo(tree);
        if ((minion_map[i]       == 1) &&
            (minion_map[partner] == 1)) {
            return 1;
        }
    }
    return 0;
}

unum_t glbl_snp_tensor_reduce_set_tree_lvl(CpuResource * cpu_resource, unum_t csr_val) {
    unum_t minion_map[1024] = {0};
    for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
        unum_t hart_id = (unum_t)BOARD_CONF.LCPUID_TO_PCPUID_MAP[i];
        if ((hart_id%2) == 0) {
            unum_t minion_id = hart_id >> 1;
            minion_map[minion_id] = 1;
        }
    }

    unum_t lvls[16] = {0};
    unum_t index = 0;
    // You can specify up to level 15 as Tree Depth (4 bits)
    // But we only have 1024 Minions, therefore max is 9
    for (unum_t i = 0; i < 10; i++) {
        unum_t is_compatible = snp_tree_compatible(cpu_resource, i, minion_map);
        if (is_compatible == 1) {
            lvls[index] = i;
            index++;
        }
    }
    unum_t tree_index = snp_urandrange_glbl(cpu_resource, 0, index);
    unum_t tree_lvl = lvls[tree_index];
    return snp_set_bits(cpu_resource, csr_val, 6, 3, tree_lvl);
}

unum_t snp_get_reduce_partner(CpuResource * cpu_resource, unum_t minion_id, unum_t csr_val) {
   unum_t partner;
   unum_t op = snp_get_bits(cpu_resource, csr_val, 1, 0);

   if (op == TR_SND) {
      partner = snp_get_bits(cpu_resource, csr_val, 15, 3);
   }
   else if (op == TR_RCV) {
      // A receive does not know who is its partner
      // Return its own minion_id
      partner = minion_id;
   }
   else {
      unum_t tree = snp_get_bits(cpu_resource, csr_val, 6, 3);
      unum_t step = pwrtwo(tree+1);
      unum_t mod  = minion_id%step;
      if (mod == 0) partner = minion_id + (step/2);
      else          partner = minion_id - (step/2);
   }
   return partner;
}

unum_t snp_in_reduce(CpuResource * cpu_resource, unum_t csr_val, unum_t cpuid) {
    unum_t tree      = snp_get_bits(cpu_resource, csr_val, 6, 3);
    unum_t hart_id   = BOARD_CONF.LCPUID_TO_PCPUID_MAP[cpuid];
    unum_t minion_id = hart_id >> 1;
    unum_t step      = pwrtwo(tree+1);

    if ((hart_id%2) == 1) {
       unum_t bias = snp_urandrange(cpu_resource, 0, 27);
       if (bias != 0) return 0;
    }

    unum_t partner;
    unum_t mod = minion_id%step;
    if ((mod != 0) && (mod != (step/2))) return 0;
    else {
        if (mod == 0) partner = minion_id + (step/2);
        else          partner = minion_id - (step/2);
        for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
            unum_t tmp = ((unum_t)BOARD_CONF.LCPUID_TO_PCPUID_MAP[i]) >> 1;
            if (tmp == partner) return 1;
        }
    }
    return 0;
}

unum_t glbl_snp_tensor_reduce_auto(CpuResource * cpu_resource, unum_t num_cpus, unum_t rand_val, unum_t gl_rand_val) {
    unum_t starting_fp_reg = snp_get_bits(cpu_resource, rand_val, 61, 57);

    unum_t num_fp_reg    = snp_get_bits(cpu_resource, gl_rand_val, 22, 16);
    unum_t tree_depth    = snp_get_bits(cpu_resource, gl_rand_val,  6,  3);

    // Sting considers HARTs as CPUs so we have to divide
    num_cpus = num_cpus/2;

    rand_val = snp_set_legal_reduce_op(cpu_resource, rand_val, gl_rand_val);

    // Set the number of FP registers
    rand_val = snp_set_bits(cpu_resource, rand_val, 22, 16, num_fp_reg);
    // Set the Tree Depth
    rand_val = glbl_snp_tensor_reduce_set_tree_lvl(cpu_resource, rand_val);
    // set the variant to TensorAuto
    rand_val = snp_set_bits(cpu_resource, rand_val, 1, 0, 3);
    return rand_val;
}

unum_t glbl_snp_tensor_reduce_broadcast(CpuResource * cpu_resource, unum_t num_cpus, unum_t rand_val, unum_t gl_rand_val) {
    unum_t starting_fp_reg = snp_get_bits(cpu_resource, rand_val, 61, 57);

    unum_t num_fp_reg    = snp_get_bits(cpu_resource, gl_rand_val, 22, 16);
    unum_t tree_depth    = snp_get_bits(cpu_resource, gl_rand_val,  6,  3);

    // Sting considers HARTs as CPUs so we have to divide
    num_cpus = num_cpus/2;

    rand_val = snp_set_legal_reduce_op(cpu_resource, rand_val, gl_rand_val);

    // Set the number of FP registers
    rand_val = snp_set_bits(cpu_resource, rand_val, 22, 16, num_fp_reg);
    // Set the Tree Depth
    rand_val = glbl_snp_tensor_reduce_set_tree_lvl(cpu_resource, rand_val);
    // set the variant to TensorAuto
    rand_val = snp_set_bits(cpu_resource, rand_val, 1, 0, 2);
    return rand_val;
} 

unum_t snp_is_tensor_reduce_receiver(CpuResource * cpu_resource, unum_t tree_lvl, unum_t minion_id, unum_t is_auto) {
    unum_t is_broadcast = !is_auto;
    unum_t takes_part = minion_id % pwrtwo(tree_lvl);
    if (takes_part == 0) {
        unum_t is_receiver_auto = ((minion_id % pwrtwo(tree_lvl+1)) == 0);
        unum_t is_receiver_broadcast = !is_receiver_auto;
        if ((is_auto && is_receiver_auto) || (is_broadcast && is_receiver_broadcast)) {
            // Is a tensor reduce receiver
            return 1;
        }
    }
    // Is not a tensor reduce receiver
    return 0;
}

// For tensor reduce error
unum_t snp_get_valid_tensor_reduce_tree_lvl(CpuResource * cpu_resource, unum_t cpuid, unum_t is_auto) {
    unum_t hart_id   = snp_get_phycpu(cpu_resource, cpuid); // HART ID
    unum_t minion_id = hart_id >> 1; // Minion ID
    unum_t ret_tree_lvl = 0x0bad;

    std::queue<int> valid_tree_lvls;
    for (int tree_lvl = 0; tree_lvl <= MAX_TREE_LVL; tree_lvl++) {
        unum_t is_receiver = snp_is_tensor_reduce_receiver(cpu_resource, tree_lvl, minion_id, is_auto);
        if (is_receiver == 1) {
            valid_tree_lvls.push(tree_lvl);
        }
    }

    if (valid_tree_lvls.size() > 0) {
        unum_t random_index = snp_urandrange(cpu_resource, 0, valid_tree_lvls.size());
        for (int i = 0; i < random_index; i++) {
            valid_tree_lvls.pop();
        }
        ret_tree_lvl = valid_tree_lvls.front();
    }

    return ret_tree_lvl;
}

unum_t snp_tensor_reduce_err(CpuResource * cpu_resource, unum_t csr_val) {
    unum_t lcpu = snp_cpu_id(cpu_resource); // Logical CPU ID
    unum_t hart_id = snp_get_phycpu(cpu_resource, lcpu); // HART ID
    unum_t minion_id = hart_id >> 1; // Minion ID
    csr_val = snp_set_bits(cpu_resource, csr_val, 1, 1, 0);
    unum_t ret;

    // Num registers must be greater than 0 in order to force the error
    unum_t num_reg = snp_urandrange(cpu_resource, 1, 0x80);
    ret = snp_set_bits(cpu_resource, csr_val, 22, 16, num_reg);

    unum_t bias = snp_urandrange(cpu_resource, 0, 2);
    switch (bias) {
        case 0: // Tensor Snd/Rcv to itself
            ret = snp_set_bits(cpu_resource, ret, 15, 3, minion_id);
            break;
        case 1: // Illegal Tensor Reduce Operation
            // We set an illegal tensor reduce operation
            ret = snp_set_illegal_reduce_op(cpu_resource, ret);

            bias = snp_urandrange(cpu_resource, 0, 2);
            if (bias == 0) {
                // To avoid trigger the tensor snd/rcv to itself error, we choose a random minion
                minion_id = snp_urandrange(cpu_resource, 0, BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP);
                ret = snp_set_bits(cpu_resource, ret, 15, 3, minion_id);
                // Illegal Tensor Reduce Operation only applies for the TensorRecv operation
                ret = snp_set_bits(cpu_resource, ret, 0, 0, 1);
            }
            else {                        //                                         tree_lvl            is_auto
                unum_t is_auto_receiver = snp_is_tensor_reduce_receiver(cpu_resource,       0, minion_id,      1);
                unum_t tree_lvl = snp_get_valid_tensor_reduce_tree_lvl(cpu_resource, lcpu, is_auto_receiver);
                ret = snp_set_bits(cpu_resource, ret, 6, 3, tree_lvl);

                if (is_auto_receiver) ret = snp_set_bits(cpu_resource, ret, 1, 0, 3);
                else                  ret = snp_set_bits(cpu_resource, ret, 1, 0, 2);
            }

            break;
    }
    return ret;
}

unum_t glbl_snp_get_treduce_auto_bcast_wrapper(CpuResource * cpu_resource) {
   unum_t  tr_val_rand    = snp_randnum(cpu_resource);
   unum_t  tr_val_gl_rand = snp_randnum_glbl(cpu_resource);
   unum_t  tr_csr_val;

   // 50/50 between reduce auto and reduce broadcast
   unum_t  bias = snp_urandrange_glbl(cpu_resource, 0, 2);
   if (bias == 0) {
      tr_csr_val = glbl_snp_tensor_reduce_auto(cpu_resource, BOARD_CONF.NUM_CPUS, tr_val_rand, tr_val_gl_rand);
   }
   else {
      tr_csr_val = glbl_snp_tensor_reduce_broadcast(cpu_resource, BOARD_CONF.NUM_CPUS, tr_val_rand, tr_val_gl_rand);
   }

   // Sometimes, do a tensor reduce auto/broadcast with an ilegal operand
   bias = snp_urandrange_glbl(cpu_resource, 0, 27);
   if (bias == 0) {
      // Force variant to be TensorReduce Recv, as illegal operations only apply to the receiver
      tr_csr_val = snp_set_bits(cpu_resource, tr_csr_val, 1, 0, 1);
      tr_csr_val = snp_set_illegal_reduce_op(cpu_resource, tr_csr_val);
   }

   return tr_csr_val;
}

unum_t get_partner_lmid(CpuResource * cpu_resource, unum_t cpuid, unum_t csr_val) {
   unum_t in_reduce = snp_in_reduce(cpu_resource, csr_val, cpuid);

   if (in_reduce) {
      unum_t mid  = snp_get_phycpu(cpu_resource, cpuid) >> 1;
      unum_t partner_pmid = snp_get_reduce_partner(cpu_resource, mid, csr_val);

      for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
         unum_t tmp = ((unum_t)BOARD_CONF.LCPUID_TO_PCPUID_MAP[i]) >> 1;
         if (tmp == partner_pmid) return (i >> 1);
      }

      // The partner must be in the BOARD_CONF.LCPUID_TO_PCPUID_MAP, therefore
      // we should execute the 'return' and never reach this part of the code
      assert("Reduce partner not in BOARD_CONF.LCPUID_TO_PCPUID_MAP" && 0);
   }

   return cpuid >> 1;
}
