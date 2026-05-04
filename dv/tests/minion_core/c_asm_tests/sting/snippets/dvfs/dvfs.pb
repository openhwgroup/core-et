
import "$MINION_DIAGS/sting/snippets/silicon_mode/sm_et_abi.pb"

pb_start jump_to_dvfs_api(pb_c_api_addr)
      ; Sting ABI != GCC ABI
      ; So we have to backup the registers
      rv_save_abi_xregs()

      jump_to_c_funct(c_arg_addr = pb_c_api_addr, c_funct = pb_c_api_addr)

      ; Restore back the registers
      rv_restore_abi_xregs()
pb_end

pb_start jump_to_dvfs_api_with_arg(pb_c_api_addr, pb_c_arg_addr)
      ; Sting ABI != GCC ABI
      ; So we have to backup the registers
      rv_save_abi_xregs()

      jump_to_c_funct(c_arg_addr = pb_c_arg_addr, c_funct = pb_c_api_addr)

      ; Restore back the registers
      rv_restore_abi_xregs()
pb_end
