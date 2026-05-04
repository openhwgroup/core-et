
`ifndef __EVL_BASE_MACROS

   `define __EVL_BASE_MACROS 1

   `ifndef QUIESCENT_FIFO_MASK
      `define QUIESCENT_FIFO_MASK (32'h00000002)
   `endif

   `ifdef ET_SIMULATION // ------------------------------------------------------------------------{

      `ifndef ET_MONITOR_IFCS
         `ifdef EVL_SIMULATION
            `define ET_MONITOR_IFCS 1
         `else
            `ifdef ET_IP_SIMULATION
               `define ET_MONITOR_IFCS 1
            `endif
         `endif
      `endif

      `ifdef EVL_SIMULATION // --------------------------------------------------------------------{

         `ifndef ET_USE_UVM
            `define ET_USE_UVM
         `endif

         `define evl_quiescent_check evl_base_pkg::m_quiescent_check

         `define evl_sim_complete    evl_base_pkg::m_sim_complete

         `define QUIESCENT_SIGNAL `evl_quiescent_check

         `define evl_get_next_trid(SHIRE_ID = ~(evl_base_pkg::evl_shire_id_t'(0)), NEIGHBORHOOD_ID = ~(evl_base_pkg::evl_neighborhood_id_t'(0)), CORE_ID = ~(evl_base_pkg::evl_core_id_t'(0))) \
                    evl_base_pkg::evl_get_next_trid(SHIRE_ID, NEIGHBORHOOD_ID, CORE_ID)

         `define evl_param_key(KEY_NAME) static int KEY_NAME = evl_param_xref::get_param_key(`"KEY_NAME`");

         `define evl_log_set_verbosity(VERBOSITY, VERBOSITY_MOD) \
                    evl_base_pkg::evl_set_verbosity(VERBOSITY, evl_base_pkg::evl_verbosity_mod_t'(VERBOSITY_MOD));

         `define evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) \
                    evl_base_pkg::evl_check_verbosity(VERBOSITY, evl_base_pkg::evl_verbosity_mod_t'(VERBOSITY_MOD))

         `define evl_wait_for_verbosity_change(CUR_VERBOSITY, VERBOSITY, VERBOSITY_MOD) \
                    evl_base_pkg::evl_wait_for_verbosity_change(CUR_VERBOSITY, VERBOSITY, evl_base_pkg::evl_verbosity_mod_t'(VERBOSITY_MOD));

         `define evl_get_verbosity_by_name(VERBOSITY_NAME) \
                    evl_base_pkg::evl_get_verbosity_by_name(VERBOSITY_NAME)

         `define evl_get_verbosity_mod_by_name(VERBOSITY_MOD_NAME) \
                    evl_base_pkg::evl_get_verbosity_mod_by_name(VERBOSITY_MOD_NAME)

         `define evl_log_no_time(VERBOSITY, VERBOSITY_MOD, MESSAGE) \
                    if (`evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) != 1'b0) begin \
                       evl_base_pkg::evl_print_no_time(MESSAGE); \
                    end

         `define evl_log_no_name(VERBOSITY, VERBOSITY_MOD, MESSAGE) \
                    if (`evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) != 1'b0) begin \
                       evl_base_pkg::evl_print_no_name(MESSAGE); \
                    end

         `define evl_log(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, MESSAGE) \
                    if (`evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) != 1'b0) begin \
                       evl_base_pkg::evl_print(ABSTRACT_NAME, MESSAGE, 2'b11); \
                    end

         `define evl_log_addr(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, ADDR, MESSAGE) \
                    if (`evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) != 1'b0) begin \
                       evl_base_pkg::evl_print(ABSTRACT_NAME, MESSAGE, 2'b01, ADDR); \
                    end

         `define evl_log_addr_trid(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, ADDR, TRID, MESSAGE) \
                    if (`evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) != 1'b0) begin \
                       evl_base_pkg::evl_print(ABSTRACT_NAME, MESSAGE, 2'b01, ADDR, TRID); \
                    end

         `define evl_log_addr_trid_id(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, ADDR, TRID, ID, MESSAGE) \
                    if (`evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) != 1'b0) begin \
                       evl_base_pkg::evl_print(ABSTRACT_NAME, MESSAGE, 2'b01, ADDR, TRID, ID); \
                    end

         `define evl_log_mdesc(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, MEM_DESC, MESSAGE) \
                    if (`evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) != 1'b0) begin \
                       if (MEM_DESC == null) begin \
                          evl_base_pkg::evl_print(ABSTRACT_NAME, MESSAGE, 2'b11); \
                       end \
                       else if (MEM_DESC.get_desc_type() == evl_base_pkg::EVL_DESC_REQ) begin \
                          evl_base_pkg::evl_print(ABSTRACT_NAME, MESSAGE, 2'b01, MEM_DESC.paddr_is_valid() ? MEM_DESC.get_paddr() : MEM_DESC.get_vaddr(), MEM_DESC.get_trans_id(), MEM_DESC.get_tag_id()); \
                       end \
                       else begin \
                          evl_base_pkg::evl_print(ABSTRACT_NAME, MESSAGE, 2'b01, MEM_DESC.paddr_is_valid() ? MEM_DESC.get_paddr() : MEM_DESC.get_vaddr(), MEM_DESC.get_trans_id()); \
                       end \
                    end

         `define evl_log_mdesc_pk(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, MEM_DESC, PARAM_KEY, MESSAGE) \
                    if (`evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) != 1'b0) begin \
                       if (MEM_DESC == null) begin \
                          evl_base_pkg::evl_print(ABSTRACT_NAME, MESSAGE, 2'b11); \
                       end \
                       else begin \
                          evl_base_pkg::evl_print(ABSTRACT_NAME, MESSAGE, 2'b01, MEM_DESC.paddr_is_valid() ? MEM_DESC.get_paddr() : MEM_DESC.get_vaddr(), MEM_DESC.get_trans_id(), MEM_DESC.get_int_param(PARAM_KEY, -1)); \
                       end \
                    end

         `define evl_log_no_addr(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, MESSAGE) \
                    if (`evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) != 1'b0) begin \
                       evl_base_pkg::evl_print_no_addr(ABSTRACT_NAME, MESSAGE); \
                    end

         `define evl_set_error_intensity(INTENSITY_KEY, VERBOSITY) \
                    evl_base_pkg::evl_set_error_intensity(INTENSITY_KEY, VERBOSITY);

         `define evl_set_error_intensity_by_name(INTENSITY_NAME, VERBOSITY) \
                    evl_base_pkg::evl_set_error_intensity_by_name(INTENSITY_NAME, VERBOSITY);

         `define evl_get_error_intensity(INTENSITY_KEY) \
                    evl_base_pkg::evl_get_error_intensity(INTENSITY_KEY)

         `define evl_get_error_intensity_by_name(INTENSITY_NAME) \
                    evl_base_pkg::evl_get_error_intensity_by_name(INTENSITY_NAME)

         `define dut_error(ABSTRACT_NAME, MESSAGE, INTENSITY_KEY = 0, VERBOSITY_MODS = {}, RTL_PATH = "", FILENAME = "", LINENUM = 0) \
                    begin \
                        automatic evl_verbosity_mod_t __verbosity_mods_in[] = VERBOSITY_MODS; \
                        if (FILENAME == "") begin \
                           evl_base_pkg::evl_dut_error(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, `__FILE__, `__LINE__), INTENSITY_KEY,__verbosity_mods_in, ~(evl_base_pkg::evl_paddr_t'(0)), ~(evl_base_pkg::evl_trans_id_t'(0)), ~(0), RTL_PATH); \
                        end \
                        else begin \
                           evl_base_pkg::evl_dut_error(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, FILENAME, LINENUM), INTENSITY_KEY, __verbosity_mods_in, ~(evl_base_pkg::evl_paddr_t'(0)), ~(evl_base_pkg::evl_trans_id_t'(0)), ~(0), RTL_PATH); \
                        end \
                    end

         `define dut_error_mdesc(ABSTRACT_NAME, MEM_DESC, MESSAGE, INTENSITY_KEY = 0, VERBOSITY_MODS = {}, RTL_PATH = "", FILENAME = "", LINENUM = 0) \
                    begin \
                        automatic evl_verbosity_mod_t __verbosity_mods_in[] = VERBOSITY_MODS; \
                        if (MEM_DESC == null) begin \
                           if (FILENAME == "") begin \
                              evl_base_pkg::evl_dut_error(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, `__FILE__, `__LINE__), INTENSITY_KEY, __verbosity_mods_in, ~(evl_base_pkg::evl_paddr_t'(0)), ~(evl_base_pkg::evl_trans_id_t'(0)), ~(0), RTL_PATH); \
                           end \
                           else begin \
                              evl_base_pkg::evl_dut_error(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, FILENAME, LINENUM), INTENSITY_KEY, __verbosity_mods_in, ~(evl_base_pkg::evl_paddr_t'(0)), ~(evl_base_pkg::evl_trans_id_t'(0)), ~(0), RTL_PATH); \
                           end \
                        end \
                        else begin \
                           if (FILENAME == "") begin \
                              evl_base_pkg::evl_dut_error(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, `__FILE__, `__LINE__), INTENSITY_KEY, __verbosity_mods_in, MEM_DESC.paddr_is_valid() ? MEM_DESC.get_paddr() : MEM_DESC.get_vaddr(), MEM_DESC.get_trans_id(), ~(0), RTL_PATH); \
                           end \
                           else begin \
                              evl_base_pkg::evl_dut_error(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, FILENAME, LINENUM), INTENSITY_KEY, __verbosity_mods_in, MEM_DESC.paddr_is_valid() ? MEM_DESC.get_paddr() : MEM_DESC.get_vaddr(), MEM_DESC.get_trans_id(), ~(0), RTL_PATH); \
                           end \
                        end \
                    end

         `define dut_warning(ABSTRACT_NAME, MESSAGE, INTENSITY_KEY = 0, VERBOSITY_MODS = {}, RTL_PATH = "", FILENAME = "", LINENUM = 0) \
                    begin \
                        automatic evl_verbosity_mod_t __verbosity_mods_in[] = VERBOSITY_MODS; \
                        if (FILENAME == "") begin \
                           evl_base_pkg::evl_dut_warning(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, `__FILE__, `__LINE__), INTENSITY_KEY, __verbosity_mods_in, ~(evl_base_pkg::evl_paddr_t'(0)), ~(evl_base_pkg::evl_trans_id_t'(0)), ~(0), RTL_PATH); \
                        end \
                        else begin \
                           evl_base_pkg::evl_dut_warning(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, FILENAME, LINENUM), INTENSITY_KEY, __verbosity_mods_in, ~(evl_base_pkg::evl_paddr_t'(0)), ~(evl_base_pkg::evl_trans_id_t'(0)), ~(0), RTL_PATH); \
                        end \
                    end

         `define dut_warning_mdesc(ABSTRACT_NAME, MEM_DESC, MESSAGE, INTENSITY_KEY = 0, VERBOSITY_MODS = {}, RTL_PATH = "", FILENAME = "", LINENUM = 0) \
                    begin \
                        automatic evl_verbosity_mod_t __verbosity_mods_in[] = VERBOSITY_MODS; \
                        if (MEM_DESC == null) begin \
                           if (FILENAME == "") begin \
                              evl_base_pkg::evl_dut_warning(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, `__FILE__, `__LINE__), INTENSITY_KEY, __verbosity_mods_in, ~(evl_base_pkg::evl_paddr_t'(0)), ~(evl_base_pkg::evl_trans_id_t'(0)), ~(0), RTL_PATH); \
                           end \
                           else begin \
                              evl_base_pkg::evl_dut_warning(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, FILENAME, LINENUM), INTENSITY_KEY, __verbosity_mods_in, ~(evl_base_pkg::evl_paddr_t'(0)), ~(evl_base_pkg::evl_trans_id_t'(0)), ~(0), RTL_PATH); \
                           end \
                        end \
                        else begin \
                           if (FILENAME == "") begin \
                              evl_base_pkg::evl_dut_warning(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, `__FILE__, `__LINE__), INTENSITY_KEY, __verbosity_mods_in, MEM_DESC.paddr_is_valid() ? MEM_DESC.get_paddr() : MEM_DESC.get_vaddr(), MEM_DESC.get_trans_id(), ~(0), RTL_PATH); \
                           end \
                           else begin \
                              evl_base_pkg::evl_dut_warning(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, FILENAME, LINENUM), INTENSITY_KEY, __verbosity_mods_in, MEM_DESC.paddr_is_valid() ? MEM_DESC.get_paddr() : MEM_DESC.get_vaddr(), MEM_DESC.get_trans_id(), ~(0), RTL_PATH); \
                           end \
                        end \
                    end


         `define evl_tcl_get_time_of_day \
                    evl_tcl_get_time_of_day()

         `define evl_tcl_get_cur_time \
                    evl_tcl_get_cur_time()

         `define evl_tcl_get_hhmmss \
                    evl_tcl_get_hhmmss()

         `define evl_tcl_get_vpi_time \
                    evl_tcl_get_vpi_time()

         `define evl_tcl_get_env(NAME) \
                    evl_tcl_get_env(NAME)

         `define evl_tcl_set_env(NAME, VALUE) \
                    evl_tcl_set_env(NAME, VALUE)

         `define evl_tcl_find_file(FILE_NAME, USE_INC_PATH) \
                    evl_tcl_find_file(FILE_NAME, USE_INC_PATH)

         `define evl_tcl_system(NAME) \
                    evl_tcl_system(NAME)

         `define evl_tcl_scan_module(HIERARCHY_NAME, DEPTH) \
                    evl_tcl_scan_module(HIERARCHY_NAME, DEPTH);

         `ifdef ENABLE_BEMU
            `define cosim_object_delete cosim_object_delete();
         `else // ifdef ENABLE_BEMU
            `define cosim_object_delete
         `endif // !ifdef ENABLE_BEMU

         `define evl_get_plus_arg_value(PLUS_ARG_NAME) \
                    evl_global_attrs::evl_get_plus_arg_value(PLUS_ARG_NAME)

         `define evl_get_plus_arg_value_as_int(PLUS_ARG_NAME) \
                    evl_global_attrs::evl_get_plus_arg_value_as_int(PLUS_ARG_NAME)

         `define evl_drive_misc_signal(SIGNAL_NAME, INIT_VALUE) \
                    int SIGNAL_NAME``_param_value;\
                    int SIGNAL_NAME``_new_param_value;\
                    assign SIGNAL_NAME = SIGNAL_NAME``_param_value[0:0];\
                    initial begin\
                       #0 m_rtl_port.init_int_param_by_name("pk_``SIGNAL_NAME``_drv", int'(INIT_VALUE));\
                       #0 SIGNAL_NAME``_new_param_value = m_rtl_port.get_int_param_by_name("pk_``SIGNAL_NAME``_drv");\
                       SIGNAL_NAME``_param_value <= SIGNAL_NAME``_new_param_value;\
                       forever begin\
                          #0 wait (SIGNAL_NAME``_param_value === SIGNAL_NAME``_new_param_value);\
                          m_rtl_port.wait_for_int_param_by_name("pk_``SIGNAL_NAME``_drv", SIGNAL_NAME``_param_value, SIGNAL_NAME``_param_value, 1'b1);\
                          #0 SIGNAL_NAME``_new_param_value = m_rtl_port.get_int_param_by_name("pk_``SIGNAL_NAME``_drv");\
                          SIGNAL_NAME``_param_value <= SIGNAL_NAME``_new_param_value;\
                       end\
                    end

         `define evl_drive_misc_bus(SIGNAL_NAME, INIT_VALUE, MSB, LSB) \
                    int SIGNAL_NAME``_param_value;\
                    int SIGNAL_NAME``_new_param_value;\
                    assign SIGNAL_NAME[MSB:LSB] = SIGNAL_NAME``_param_value[(MSB-LSB):0];\
                    initial begin\
                       #0 m_rtl_port.init_int_param_by_name("pk_``SIGNAL_NAME``_drv", int'(INIT_VALUE));\
                       #0 SIGNAL_NAME``_new_param_value = m_rtl_port.get_int_param_by_name("pk_``SIGNAL_NAME``_drv");\
                       SIGNAL_NAME``_param_value <= SIGNAL_NAME``_new_param_value;\
                       forever begin\
                          #0 wait (SIGNAL_NAME``_param_value === SIGNAL_NAME``_new_param_value);\
                          m_rtl_port.wait_for_int_param_by_name("pk_``SIGNAL_NAME``_drv", SIGNAL_NAME``_param_value, SIGNAL_NAME``_param_value, 1'b1);\
                          #0 SIGNAL_NAME``_new_param_value = m_rtl_port.get_int_param_by_name("pk_``SIGNAL_NAME``_drv");\
                          SIGNAL_NAME``_param_value <= SIGNAL_NAME``_new_param_value;\
                       end\
                    end

         `define evl_monitor_misc_signal(SIGNAL_NAME) \
                    reg SIGNAL_NAME``_param_value;\
                    initial begin\
                       SIGNAL_NAME``_param_value = SIGNAL_NAME;\
                       #0 m_rtl_port.init_int_param_by_name("pk_``SIGNAL_NAME``_mon", int'(SIGNAL_NAME``_param_value));\
                       forever begin\
                          #0 wait (SIGNAL_NAME``_param_value !== SIGNAL_NAME);\
                          #0 SIGNAL_NAME``_param_value = SIGNAL_NAME;\
                          m_rtl_port.set_int_param_by_name("pk_``SIGNAL_NAME``_mon", int'(SIGNAL_NAME``_param_value));\
                       end\
                    end

         `define evl_monitor_misc_bus(SIGNAL_NAME, MSB, LSB) \
                    reg [MSB:LSB] SIGNAL_NAME``_param_value;\
                    initial begin\
                       SIGNAL_NAME``_param_value[MSB:LSB] = SIGNAL_NAME[MSB:LSB];\
                       #0 m_rtl_port.init_int_param_by_name("pk_``SIGNAL_NAME``_mon", int'(SIGNAL_NAME``_param_value));\
                       forever begin\
                          #0 wait (SIGNAL_NAME``_param_value[MSB:LSB] !== SIGNAL_NAME[MSB:LSB]);\
                          #0 SIGNAL_NAME``_param_value[MSB:LSB] = SIGNAL_NAME[MSB:LSB];\
                          m_rtl_port.set_int_param_by_name("pk_``SIGNAL_NAME``_mon", int'(SIGNAL_NAME``_param_value[(MSB-LSB):0]));\
                       end\
                    end

         `define evl_write_dpi_event(PACKED_DATA) \
                    et_write_dpi_event(0, PACKED_DATA);

         `define evl_instr_trigger(PC, INSTR, SOURCE) \
                    evl_base_pkg::evl_global_attrs::evl_instr_trigger(evl_base_pkg::evl_vaddr_t'(PC), evl_base_pkg::evl_instr_t'(INSTR), int'(SOURCE));

         `define evl_bus_trigger(ADDR, REQ_ID, SOURCE) \
                    evl_base_pkg::evl_global_attrs::evl_bus_trigger(evl_base_pkg::evl_paddr_t'(ADDR), evl_base_pkg::evl_bus_req_id_t'(INSTR), int'(SOURCE));

         `define evl_incr_objection(PHASE_NAME, SOURCE) \
                    evl_base_pkg::evl_global_attrs::incr_objection(PHASE_NAME, SOURCE);

         `define evl_decr_objection(PHASE_NAME, SOURCE) \
                    evl_base_pkg::evl_global_attrs::decr_objection(PHASE_NAME, SOURCE);

         `ifdef EVL_CERTIFY_PLUSARGS
            `define evl_check_plusargs(PLUS_ARG) evl_base_pkg::evl_check_plusargs(PLUS_ARG, `__FILE__, `__LINE__);
         `else // ifdef EVL_CERTIFY_PLUSARGS
            `define evl_check_plusargs(PLUS_ARG)
         `endif // !ifdef EVL_CERTIFY_PLUSARGS

         `define evl_certify_plusargs \
                    evl_base_pkg::evl_certify_plusargs();

      `else // ifdef EVL_SIMULATION }--------------------------------------------------------------{

         `ifdef ET_IP_SIMULATION // ---------------------------------------------------------------{

            `define evl_quiescent_check evl_base_pkg::m_quiescent_check

            `define evl_sim_complete    evl_base_pkg::m_sim_complete

            `define QUIESCENT_SIGNAL `evl_quiescent_check

            `define evl_get_next_trid(SHIRE_ID = ~(evl_base_pkg::evl_shire_id_t'(0)), NEIGHBORHOOD_ID = ~(evl_base_pkg::evl_neighborhood_id_t'(0)), CORE_ID = ~(evl_base_pkg::evl_core_id_t'(0))) \
                       evl_base_pkg::evl_get_next_trid(SHIRE_ID, NEIGHBORHOOD_ID, CORE_ID)

            `define evl_param_key(KEY_NAME) static int KEY_NAME = evl_param_xref::get_param_key(`"KEY_NAME`");

            `define evl_log_set_verbosity(VERBOSITY, VERBOSITY_MOD) \
                       evl_base_pkg::evl_set_verbosity(VERBOSITY, evl_base_pkg::evl_verbosity_mod_t'(VERBOSITY_MOD));

            `define evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) \
                       evl_base_pkg::evl_check_verbosity(VERBOSITY, evl_base_pkg::evl_verbosity_mod_t'(VERBOSITY_MOD))

            `define evl_wait_for_verbosity_change(CUR_VERBOSITY, VERBOSITY, VERBOSITY_MOD) \
                       evl_base_pkg::evl_wait_for_verbosity_change(CUR_VERBOSITY, VERBOSITY, evl_base_pkg::evl_verbosity_mod_t'(VERBOSITY_MOD));

            `define evl_get_verbosity_by_name(VERBOSITY_NAME) \
                       evl_base_pkg::evl_get_verbosity_by_name(VERBOSITY_NAME)

            `define evl_get_verbosity_mod_by_name(VERBOSITY_MOD_NAME) \
                       evl_base_pkg::evl_get_verbosity_mod_by_name(VERBOSITY_MOD_NAME)

            `define evl_log_no_time(VERBOSITY, VERBOSITY_MOD, MESSAGE) \
                       if (`evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) != 1'b0) begin \
                          evl_base_pkg::evl_print_no_time(MESSAGE); \
                       end

            `define evl_log_no_name(VERBOSITY, VERBOSITY_MOD, MESSAGE) \
                       if (`evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) != 1'b0) begin \
                          evl_base_pkg::evl_print_no_name(MESSAGE); \
                       end

            `define evl_log(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, MESSAGE) \
                       if (`evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) != 1'b0) begin \
                          evl_base_pkg::evl_print_no_addr(ABSTRACT_NAME, MESSAGE); \
                       end

            `define evl_log_addr(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, ADDR, MESSAGE) \
                       if (`evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) != 1'b0) begin \
                          evl_base_pkg::evl_print(ABSTRACT_NAME, MESSAGE, 2'b01, ADDR); \
                       end

            `define evl_log_addr_trid(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, ADDR, TRID, MESSAGE) \
                       if (`evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) != 1'b0) begin \
                          evl_base_pkg::evl_print(ABSTRACT_NAME, MESSAGE, 2'b01, ADDR, TRID); \
                       end

            `define evl_log_addr_trid_id(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, ADDR, TRID, ID, MESSAGE) \
                       if (`evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) != 1'b0) begin \
                          evl_base_pkg::evl_print(ABSTRACT_NAME, MESSAGE, 2'b01, ADDR, TRID, ID); \
                       end

            `define evl_log_mdesc(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, MEM_DESC, MESSAGE) \
                       if (`evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) != 1'b0) begin \
                          evl_base_pkg::evl_print_no_addr(ABSTRACT_NAME, MESSAGE); \
                       end

            `define evl_log_mdesc_pk(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, MEM_DESC, PARAM_KEY, MESSAGE) \
                       if (`evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) != 1'b0) begin \
                          evl_base_pkg::evl_print_no_addr(ABSTRACT_NAME, MESSAGE); \
                       end

            `define evl_log_no_addr(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, MESSAGE) \
                       if (`evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) != 1'b0) begin \
                          evl_base_pkg::evl_print_no_addr(ABSTRACT_NAME, MESSAGE); \
                       end

            `define evl_set_error_intensity(INTENSITY_KEY, VERBOSITY) \
                       evl_base_pkg::evl_set_error_intensity(INTENSITY_KEY, VERBOSITY);

            `define evl_set_error_intensity_by_name(INTENSITY_NAME, VERBOSITY) \
                       evl_base_pkg::evl_set_error_intensity_by_name(INTENSITY_NAME, VERBOSITY);

            `define evl_get_error_intensity(INTENSITY_KEY) \
                       evl_base_pkg::evl_get_error_intensity(INTENSITY_KEY)

            `define evl_get_error_intensity_by_name(INTENSITY_NAME) \
                       evl_base_pkg::evl_get_error_intensity_by_name(INTENSITY_NAME)

            `define dut_error(ABSTRACT_NAME, MESSAGE, INTENSITY_KEY = 0, VERBOSITY_MODS = {}, RTL_PATH = "", FILENAME = "", LINENUM = 0) \
                    begin \
                        automatic evl_verbosity_mod_t __verbosity_mods_in[] = VERBOSITY_MODS; \
                       if (FILENAME == "") begin \
                          evl_base_pkg::evl_dut_error(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, `__FILE__, `__LINE__), INTENSITY_KEY, __verbosity_mods_in, ~(evl_base_pkg::evl_paddr_t'(0)), ~(evl_base_pkg::evl_trans_id_t'(0)), ~(0), RTL_PATH); \
                       end \
                       else begin \
                          evl_base_pkg::evl_dut_error(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, FILENAME, LINENUM), INTENSITY_KEY, __verbosity_mods_in, ~(evl_base_pkg::evl_paddr_t'(0)), ~(evl_base_pkg::evl_trans_id_t'(0)), ~(0), RTL_PATH); \
                       end \
                    end

            `define dut_error_mdesc(ABSTRACT_NAME, MEM_DESC, MESSAGE, INTENSITY_KEY = 0, VERBOSITY_MODS = {}, RTL_PATH = "", FILENAME = "", LINENUM = 0) \
                    begin \
                       automatic evl_verbosity_mod_t __verbosity_mods_in[] = VERBOSITY_MODS; \
                       if (FILENAME == "") begin \
                          evl_base_pkg::evl_dut_error(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, `__FILE__, `__LINE__), INTENSITY_KEY, __verbosity_mods_in, ~(evl_base_pkg::evl_paddr_t'(0)), ~(evl_base_pkg::evl_trans_id_t'(0)), ~(0), RTL_PATH); \
                       end \
                       else begin \
                          evl_base_pkg::evl_dut_error(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, FILENAME, LINENUM), INTENSITY_KEY, __verbosity_mods_in, ~(evl_base_pkg::evl_paddr_t'(0)), ~(evl_base_pkg::evl_trans_id_t'(0)), ~(0), RTL_PATH); \
                       end \
                   end

            `define dut_warning(ABSTRACT_NAME, MESSAGE, INTENSITY_KEY = 0, VERBOSITY_MODS = {}, RTL_PATH = "", FILENAME = "", LINENUM = 0) \
                    begin \
                       automatic evl_verbosity_mod_t __verbosity_mods_in[] = VERBOSITY_MODS; \
                       if (FILENAME == "") begin \
                          evl_base_pkg::evl_dut_warning(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, `__FILE__, `__LINE__), INTENSITY_KEY, __verbosity_mods_in, ~(evl_base_pkg::evl_paddr_t'(0)), ~(evl_base_pkg::evl_trans_id_t'(0)), ~(0), RTL_PATH); \
                       end \
                       else begin \
                          evl_base_pkg::evl_dut_warning(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, FILENAME, LINENUM), INTENSITY_KEY, __verbosity_mods_in, ~(evl_base_pkg::evl_paddr_t'(0)), ~(evl_base_pkg::evl_trans_id_t'(0)), ~(0), RTL_PATH); \
                       end \
                    end

            `define dut_warning_mdesc(ABSTRACT_NAME, MEM_DESC, MESSAGE, INTENSITY_KEY = 0, VERBOSITY_MODS = {}, RTL_PATH = "", FILENAME = "", LINENUM = 0) \
                    begin \
                       automatic evl_verbosity_mod_t __verbosity_mods_in[] = VERBOSITY_MODS; \
                       if (FILENAME == "") begin \
                          evl_base_pkg::evl_dut_warning(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, `__FILE__, `__LINE__), INTENSITY_KEY, __verbosity_mods_in, ~(evl_base_pkg::evl_paddr_t'(0)), ~(evl_base_pkg::evl_trans_id_t'(0)), ~(0), RTL_PATH); \
                       end \
                       else begin \
                          evl_base_pkg::evl_dut_warning(ABSTRACT_NAME, $sformatf("%0s (%0s, line %0d)", MESSAGE, FILENAME, LINENUM), INTENSITY_KEY, __verbosity_mods_in, ~(evl_base_pkg::evl_paddr_t'(0)), ~(evl_base_pkg::evl_trans_id_t'(0)), ~(0), RTL_PATH); \
                       end \
                    end

            `ifdef EVL_CERTIFY_PLUSARGS
               `define evl_check_plusargs(PLUS_ARG) evl_base_pkg::evl_check_plusargs(PLUS_ARG, `__FILE__, `__LINE__);
            `else // ifdef EVL_CERTIFY_PLUSARGS
               `define evl_check_plusargs(PLUS_ARG)
            `endif // !ifdef EVL_CERTIFY_PLUSARGS

            `define evl_certify_plusargs \
                       evl_base_pkg::evl_certify_plusargs();

         `else // ifdef ET_IP_SIMULATION }---------------------------------------------------------{

            `define evl_sim_complete 0

            `define evl_get_next_trid(SHIRE_ID = ~(evl_base_pkg::evl_shire_id_t'(0)), NEIGHBORHOOD_ID = ~(evl_base_pkg::evl_neighborhood_id_t'(0)), CORE_ID = ~(evl_base_pkg::evl_core_id_t'(0))) 0

            `define evl_param_key(KEY_NAME) static int KEY_NAME = 0;

            `define evl_log_set_verbosity(VERBOSITY, VERBOSITY_MOD)

            `define evl_log_check_verbosity(VERBOSITY, VERBOSITY_MOD) 1

            `define evl_wait_for_verbosity_change(CUR_VERBOSITY, VERBOSITY, VERBOSITY_MOD) \
                       CUR_VERBOSITY = 0;\
                       if (VERBOSITY >= 0) begin\
                          wait(0);\
                       end

            `define evl_get_verbosity_by_name(VERBOSITY_NAME)

            `define evl_get_verbosity_mod_by_name(VERBOSITY_MOD_NAME) 1

            `define evl_log_no_time(VERBOSITY, VERBOSITY_MOD, MESSAGE) \
                       $display(MESSAGE);

            `define evl_log_no_name(VERBOSITY, VERBOSITY_MOD, MESSAGE) \
                       $display(MESSAGE);

            `define evl_log(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, MESSAGE) \
                       $display(MESSAGE);

            `define evl_log_addr(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, ADDR, MESSAGE) \
                       $display(MESSAGE);

            `define evl_log_addr_trid(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, ADDR, TRID, MESSAGE) \
                       $display(MESSAGE);

            `define evl_log_addr_trid_id(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, ADDR, TRID, ID, MESSAGE) \
                       $display(MESSAGE);

            `define evl_log_mdesc(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, MEM_DESC, MESSAGE) \
                       $display(MESSAGE);

            `define evl_log_mdesc_pk(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, MEM_DESC, PARAM_KEY, MESSAGE) \
                       $display(MESSAGE);

            `define evl_log_no_addr(VERBOSITY, VERBOSITY_MOD, ABSTRACT_NAME, MESSAGE) \
                       $display(MESSAGE);

            `define evl_set_error_intensity(INTENSITY_KEY, VERBOSITY)

            `define evl_set_error_intensity_by_name(INTENSITY_NAME, VERBOSITY)

            `define evl_get_error_intensity(INTENSITY_KEY) 0

            `define evl_get_error_intensity_by_name(INTENSITY_NAME) 0

            `define dut_error(ABSTRACT_NAME, MESSAGE, INTENSITY_KEY = 0, VERBOSITY_MODS = {}, RTL_PATH = "", FILENAME = "", LINENUM = 0) \
                       $display("DUT Error: %0s", MESSAGE);

            `define dut_error_mdesc(ABSTRACT_NAME, MEM_DESC, MESSAGE, INTENSITY_KEY = 0, VERBOSITY_MODS = {}, RTL_PATH = "", FILENAME = "", LINENUM = 0) \
                       $display("DUT Error: %0s", MESSAGE);

            `define dut_warning(ABSTRACT_NAME, MESSAGE, INTENSITY_KEY = 0, VERBOSITY_MODS = {}, RTL_PATH = "", FILENAME = "", LINENUM = 0) \
                       $display("DUT Warning: %0s", MESSAGE);

            `define dut_warning_mdesc(ABSTRACT_NAME, MEM_DESC, MESSAGE, INTENSITY_KEY = 0, VERBOSITY_MODS = {}, RTL_PATH = "", FILENAME = "", LINENUM = 0) \
                       $display("DUT Warning: %0s", MESSAGE);

            `define evl_check_plusargs(PLUS_ARG)

            `define evl_certify_plusargs

         `endif // !ifdef ET_IP_SIMULATION --------------------------------------------------------}

         `define evl_tcl_get_time_of_day 0

         `define evl_tcl_get_cur_time 0

         `define evl_tcl_get_hhmmss "??:??:??"

         `define evl_tcl_get_vpi_time 0

         `define evl_tcl_get_env(NAME)

         `define evl_tcl_set_env(NAME, VALUE)

         `define evl_tcl_find_file(FILE_NAME, USE_INC_PATH)

         `define evl_tcl_system(NAME)

         `define evl_tcl_scan_module(HIERARCHY_NAME, DEPTH)

         `define cosim_object_delete

         `define evl_get_plus_arg_value(PLUS_ARG_NAME)

         `define evl_get_plus_arg_value_as_int(PLUS_ARG_NAME)

         `define evl_drive_misc_signal(SIGNAL_NAME, INIT_VALUE)

         `define evl_drive_misc_bus(SIGNAL_NAME, INIT_VALUE, MSB, LSB)

         `define evl_monitor_misc_signal(SIGNAL_NAME)

         `define evl_monitor_misc_bus(SIGNAL_NAME, MSB, LSB)

         `define evl_write_dpi_event(PACKED_DATA)

         `define evl_instr_trigger(PC, INSTR, SOURCE)

         `define evl_bus_trigger(ADDR, REQ_ID, SOURCE)

         `define evl_incr_objection(PHASE_NAME, SOURCE)

         `define evl_decr_objection(PHASE_NAME, SOURCE)

      `endif // !ifdef EVL_SIMULATION -------------------------------------------------------------}


      `ifdef ET_IP_SIMULATION // ------------------------------------------------------------------{

         `define et_get_time_of_day 0

         `define et_get_cur_time 0

         `define et_get_vpi_time 0

         `define et_get_env(NAME) ""

         `define et_open_dpi_event_file(FILE_NAME) -1

         `define et_close_dpi_event_files

         `define et_write_dpi_event(FD, PACKED_DATA)

      `else // ifdef ET_IP_SIMULATION }------------------------------------------------------------{

         `define et_get_time_of_day \
                    et_get_time_of_day()

         `define et_get_cur_time \
                    et_get_cur_time()

         `define et_get_vpi_time \
                    et_get_vpi_time()

         `define et_get_env(NAME) \
                    et_get_env(NAME)

         `define et_open_dpi_event_file(FILE_NAME) -1

         `define et_close_dpi_event_files

         `define et_write_dpi_event(FD, PACKED_DATA)

      `endif // !ifdef ET_IP_SIMULATION -----------------------------------------------------------}

   `else // ifdef ET_SIMULATION }------------------------------------------------------------------{

      `define et_get_time_of_day 0

      `define et_get_cur_time 0

      `define et_get_vpi_time 0

      `define et_get_env(NAME) ""

      `define et_open_dpi_event_file(FILE_NAME) -1

      `define et_close_dpi_event_files

      `define et_write_dpi_event(FD, PACKED_DATA)

   `endif // !ifdef ET_SIMULATION -----------------------------------------------------------------}


   `ifndef et_test_plusargs // --------------------------------------------------------------------{

      `ifdef ZEBU
         `define et_test_plusargs(PLUS_ARG) 0
         `define et_test_plusargs_def "0"
      `else // ifdef ZEBU
         `ifdef ET_SIMULATION
            `ifdef EVL_SIMULATION
               `ifdef EVL_CERTIFY_PLUSARGS
                  `define et_test_plusargs(PLUS_ARG) (evl_base_pkg::evl_check_plusargs(PLUS_ARG, `__FILE__, `__LINE__) | $test$plusargs(PLUS_ARG))
                  `define et_test_plusargs_def "(evl_base_pkg::evl_check_plusargs(PLUS_ARG, `__FILE__, `__LINE__) | $test$plusargs(PLUS_ARG))"
               `else // ifdef EVL_CERTIFY_PLUSARGS
                  `define et_test_plusargs(PLUS_ARG) $test$plusargs(PLUS_ARG)
                  `define et_test_plusargs_def "$test$plusargs(PLUS_ARG)"
               `endif // !ifdef EVL_CERTIFY_PLUSARGS
            `else // ifdef EVL_SIMULATION
               `ifdef ET_IP_SIMULATION
                  `ifdef EVL_CERTIFY_PLUSARGS
                     `define et_test_plusargs(PLUS_ARG) (evl_base_pkg::evl_check_plusargs(PLUS_ARG, `__FILE__, `__LINE__) | $test$plusargs(PLUS_ARG))
                     `define et_test_plusargs_def "(evl_base_pkg::evl_check_plusargs(PLUS_ARG, `__FILE__, `__LINE__) | $test$plusargs(PLUS_ARG))"
                  `else // ifdef EVL_CERTIFY_PLUSARGS
                     `define et_test_plusargs(PLUS_ARG) $test$plusargs(PLUS_ARG)
                     `define et_test_plusargs_def "$test$plusargs(PLUS_ARG)"
                  `endif // !ifdef EVL_CERTIFY_PLUSARGS
               `else // ifdef ET_IP_SIMULATION
                  `define et_test_plusargs(PLUS_ARG) $test$plusargs(PLUS_ARG)
                  `define et_test_plusargs_def "$test$plusargs(PLUS_ARG)"
               `endif // !ifdef ET_IP_SIMULATION
            `endif // !ifdef EVL_SIMULATION
         `else // ifdef ET_SIMULATION
            `ifdef SYNOPSYS_SMS
               `define et_test_plusargs(PLUS_ARG) $test$plusargs(PLUS_ARG)
               `define et_test_plusargs_def "$test$plusargs(PLUS_ARG)"
            `else // ifdef SYNOPSYS_SMS
               `define et_test_plusargs(PLUS_ARG) 0
               `define et_test_plusargs_def "0"
            `endif  // !ifdef SYNOPSYS_SMS
         `endif // !ifdef ifdef ET_SIMULATION
      `endif // !ZEBU

   `endif // ifndef et_test_plusargs --------------------------------------------------------------}


   `ifndef et_value_plusargs // -------------------------------------------------------------------{

      `ifdef ZEBU
         `define et_value_plusargs(PLUS_ARG, VALUE) 0
         `define et_value_plusargs_def "0"
      `else // ifdef ZEBU
         `ifdef ET_SIMULATION
            `ifdef EVL_SIMULATION
               `ifdef EVL_CERTIFY_PLUSARGS
                  `define et_value_plusargs(PLUS_ARG, VALUE) (evl_base_pkg::evl_check_plusargs(PLUS_ARG, `__FILE__, `__LINE__) | $value$plusargs(PLUS_ARG, VALUE))
                  `define et_value_plusargs_def "(evl_base_pkg::evl_check_plusargs(PLUS_ARG, `__FILE__, `__LINE__) | $value$plusargs(PLUS_ARG, VALUE))"
               `else // ifdef EVL_CERTIFY_PLUSARGS
                  `define et_value_plusargs(PLUS_ARG, VALUE) $value$plusargs(PLUS_ARG, VALUE)
                  `define et_value_plusargs_def "$value$plusargs(PLUS_ARG, VALUE)"
               `endif // !ifdef EVL_CERTIFY_PLUSARGS
            `else // ifdef EVL_SIMULATION
               `ifdef ET_IP_SIMULATION
                  `define et_value_plusargs(PLUS_ARG) (evl_base_pkg::evl_check_plusargs(PLUS_ARG, `__FILE__, `__LINE__) | $test$plusargs(PLUS_ARG))
                  `define et_value_plusargs_def "(evl_base_pkg::evl_check_plusargs(PLUS_ARG, `__FILE__, `__LINE__) | $test$plusargs(PLUS_ARG))"
                  `ifdef EVL_CERTIFY_PLUSARGS
                     `define et_value_plusargs(PLUS_ARG, VALUE) (evl_base_pkg::evl_check_plusargs(PLUS_ARG, `__FILE__, `__LINE__) | $value$plusargs(PLUS_ARG, VALUE))
                     `define et_value_plusargs_def "(evl_base_pkg::evl_check_plusargs(PLUS_ARG, `__FILE__, `__LINE__) | $value$plusargs(PLUS_ARG, VALUE))"
                  `else // ifdef EVL_CERTIFY_PLUSARGS
                     `define et_value_plusargs(PLUS_ARG, VALUE) $value$plusargs(PLUS_ARG, VALUE)
                     `define et_value_plusargs_def "$value$plusargs(PLUS_ARG, VALUE)"
                  `endif // !ifdef EVL_CERTIFY_PLUSARGS
               `else // ifdef ET_IP_SIMULATION
                  `define et_value_plusargs(PLUS_ARG, VALUE) $value$plusargs(PLUS_ARG, VALUE)
                  `define et_value_plusargs_def "$value$plusargs(PLUS_ARG, VALUE)"
               `endif // !ifdef ET_IP_SIMULATION
            `endif // !ifdef EVL_SIMULATION
         `else // ifdef ET_SIMULATION
            `ifdef SYNOPSYS_SMS
               `define et_value_plusargs(PLUS_ARG, VALUE) $value$plusargs(PLUS_ARG, VALUE)
               `define et_value_plusargs_def "$value$plusargs(PLUS_ARG, VALUE)"
            `else // ifdef SYNOPSYS_SMS
               `define et_value_plusargs(PLUS_ARG, VALUE) 0
               `define et_value_plusargs_def "0"
            `endif  // !SYNOPSYS_SMS
         `endif // !ifdef ET_SIMULATION
      `endif // !ifdef ZEBU

   `endif // ifndef et_value_plusargs -------------------------------------------------------------}

   `ifdef ET_USE_UVM
      `define UVM_NONE   uvm_pkg::UVM_NONE
      `define UVM_LOW    uvm_pkg::UVM_LOW
      `define UVM_MEDIUM uvm_pkg::UVM_MEDIUM
      `define UVM_HIGH   uvm_pkg::UVM_HIGH
      `define UVM_FULL   uvm_pkg::UVM_FULL
      `define UVM_DEBUG  uvm_pkg::UVM_DEBUG
   `else
      `define UVM_NONE   0
      `define UVM_LOW    100
      `define UVM_MEDIUM 200
      `define UVM_HIGH   300
      `define UVM_FULL   400
      `define UVM_DEBUG  500
   `endif

`endif // !__EVL_BASE_MACROS
