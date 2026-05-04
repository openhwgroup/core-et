//
// This module is a wrapper for the east-side gate-level memory shire.  It is used to get the
// auto-init module to work on gate netlists.
//
`include "soc.vh"

`ifdef MEMSHIRE_AUTO_INIT_ENABLE
   `ifdef DV_MEMSHIRE_EAST_SIDE_USE_GATE
      `define ET_MS_AI_EAST_TOP_PATH       memshire_east_top_memshire_top_memshire_top
      `define ET_MS_AI_EAST_DEBUG_PATH     `ET_MS_AI_EAST_TOP_PATH.ms_debug_g_ms_debug
      `define ET_MS_AI_EAST_MEM_LOGIC_PATH `ET_MS_AI_EAST_TOP_PATH.mem_logic
   `endif // ifdef DV_MEMSHIRE_EAST_SIDE_USE_GATE
`endif // ifdef MEMSHIRE_AUTO_INIT_ENABLE

`ifndef _ET_MS_AUTO_INIT_EAST_WRAPPER
   `define _ET_MS_AUTO_INIT_EAST_WRAPPER

   `ifdef ET_MS_AI_EAST_TOP_PATH

      module ms_auto_init_east_wrapper (
            input wire       clk_ms_pll,
            input wire       reset_cold,
            input wire       reset_warm,
            input wire [7:0] memshire_id_num
         );

         wire                                       reset;
         wire [`MEMSHIRE_BPAM_APB_ADDR_WIDTH - 1:0] ai_apb_paddr;
         wire                                       ai_apb_penable;
         wire [`MEMSHIRE_BPAM_APB_DATA_WIDTH - 1:0] ai_apb_prdata;
         wire                                       ai_apb_pready;
         wire                                       ai_apb_psel;
         wire [`MEMSHIRE_BPAM_APB_DATA_WIDTH - 1:0] ai_apb_pwdata;
         wire                                       ai_apb_pwrite;
         wire                                       ms_auto_init_enable;
         wire [`MS_AUTO_CONFIG_WIDTH-1:0]           ai_auto_init_config;
         wire                                       ms_auto_init_inprog;
         wire                                       dv_ms_auto_init_done;
         bit                                        ai_auto_init_enable;

         assign reset                    = reset_cold | reset_warm;
         assign ai_apb_prdata            = (ms_auto_init_inprog     === 1'b1)  ? `ET_MS_AI_EAST_TOP_PATH.bpam_apb_prdata :
                                                                                 {(`MEMSHIRE_BPAM_APB_DATA_WIDTH){1'b0}};
         assign ai_apb_pready            = (ms_auto_init_inprog     === 1'b1)  ? `ET_MS_AI_EAST_TOP_PATH.bpam_apb_pready : 1'b0;
         assign ms_auto_init_enable      = (top.ms_auto_init_enable === 1'b1)  ? 1'b1 : 1'b0;
         assign ai_auto_init_config      = (memshire_id_num[1:0]    === 2'b00) ? { 1'b1, top.ms_auto_init_config[14:0] } :
                                                                                 { 1'b0, top.ms_auto_init_config[14:0] };
         assign top.ms_auto_init_inprog  = ms_auto_init_inprog;
         assign top.dv_ms_auto_init_done = dv_ms_auto_init_done;

         initial begin
            ai_auto_init_enable = 1'b0;
            wait (ms_auto_init_enable === 1'b1);
            repeat (3) @(posedge clk_ms_pll);
            ai_auto_init_enable <= 1'b1;
            repeat (2) @(negedge clk_ms_pll);
            force `ET_MS_AI_EAST_TOP_PATH.bpam_apb_paddr[30:2]                     = ai_apb_paddr[30:2];
            force `ET_MS_AI_EAST_TOP_PATH.bpam_apb_psel                            = ai_apb_psel;
            force `ET_MS_AI_EAST_TOP_PATH.bpam_apb_pwdata                          = ai_apb_pwdata;
            force `ET_MS_AI_EAST_TOP_PATH.bpam_apb_pwrite                          = ai_apb_pwrite;
            force `ET_MS_AI_EAST_DEBUG_PATH.reset                                  = 1'b1;
            force `ET_MS_AI_EAST_MEM_LOGIC_PATH.mem_mesh_slave_AR_fifo_valid_fifo  = 1'b0;
            force `ET_MS_AI_EAST_MEM_LOGIC_PATH.mem_mesh_slave_AWW_fifo_valid_fifo = 1'b0;
            wait (ms_auto_init_inprog === 1'b1);
            @(posedge clk_ms_pll);
            ai_auto_init_enable <= 1'b0;
            wait (dv_ms_auto_init_done === 1'b1);
            @(posedge clk_ms_pll);
            @(negedge clk_ms_pll);
            release `ET_MS_AI_EAST_TOP_PATH.bpam_apb_paddr;
            release `ET_MS_AI_EAST_TOP_PATH.bpam_apb_psel;
            release `ET_MS_AI_EAST_TOP_PATH.bpam_apb_pwdata;
            release `ET_MS_AI_EAST_TOP_PATH.bpam_apb_pwrite;
            release `ET_MS_AI_EAST_DEBUG_PATH.reset;
            release `ET_MS_AI_EAST_MEM_LOGIC_PATH.mem_mesh_slave_AR_fifo_valid_fifo;
            release `ET_MS_AI_EAST_MEM_LOGIC_PATH.mem_mesh_slave_AWW_fifo_valid_fifo;
         end

         `ifdef EVL_SIMULATION
            import uvm_pkg::*;
            import evl_base_pkg::*;

            string abstract_name;

            initial begin
               wait (ai_auto_init_enable === 1'b1);
               abstract_name = "";
               forever @(posedge clk_ms_pll) begin
                  if (dv_ms_auto_init_done === 1'b1) begin
                     break;
                  end
                  if ((ai_apb_penable === 1'b1) && (ai_apb_psel === 1'b1)) begin
                     wait (ai_apb_pready === 1'b1);
                     @(posedge clk_ms_pll);
                     if (abstract_name == "") begin
                        abstract_name = $sformatf("S%0d:MS_ESR(APB)", memshire_id_num + 232);
                     end
                     if (ai_apb_pwrite === 1'b1) begin
                        `evl_log_addr(UVM_MEDIUM, VMOD_GLOBAL, abstract_name, ai_apb_paddr, $sformatf("APB_REQ_WRITE 0x%x <- 0x%x", ai_apb_paddr, ai_apb_pwdata))
                     end
                     else begin
                        `evl_log_addr(UVM_MEDIUM, VMOD_GLOBAL, abstract_name, ai_apb_paddr, $sformatf("APB_REQ_READ 0x%x <- 0x%x", ai_apb_paddr, ai_apb_prdata))
                     end
                  end
               end
            end
         `endif // ifdef EVL_SIMULATION

         ms_auto_init ms_auto_init (
               .clock                ( clk_ms_pll           ),
               .reset                ( reset                ),
               .ai_apb_paddr         ( ai_apb_paddr         ),
               .ai_apb_penable       ( ai_apb_penable       ),
               .ai_apb_prdata        ( ai_apb_prdata        ),
               .ai_apb_pready        ( ai_apb_pready        ),
               .ai_apb_psel          ( ai_apb_psel          ),
               .ai_apb_pwdata        ( ai_apb_pwdata        ),
               .ai_apb_pwrite        ( ai_apb_pwrite        ),
               .ms_auto_init_enable  ( ai_auto_init_enable  ),
               .ms_auto_init_config  ( ai_auto_init_config  ),
               .ms_auto_init_inprog  ( ms_auto_init_inprog  ),
               .dv_ms_auto_init_done ( dv_ms_auto_init_done )
            );
      endmodule

   `endif // ifdef ET_MS_AI_EAST_TOP_PATH
`endif // ifndef _ET_MS_AUTO_INIT_EAST_WRAPPER
