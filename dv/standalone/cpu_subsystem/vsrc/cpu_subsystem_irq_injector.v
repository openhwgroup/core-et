`include "soc.vh"

`define REG_VALIDATION1               12'h8d1

module cpu_subsystem_irq_injector
  (  
    //CSR monitor signal
   input logic                              clock,
   input logic                              reset_warm,
   input logic [11:0]                       mon_csr_mem_addr,
   input logic [11:0]                       mon_csr_wb_addr,
   input logic [2:0]                        mon_csr_wb_cmd,
   input logic [63:0]                       mon_csr_wb_wdata,
   input logic                              mon_csr_wb_thread_id,
   output logic [`PLIC_NEXTINT_SOURCES-1:0] plic_irq
  );

        logic is_interrupt_command;
        logic [`PLIC_NEXTINT_SOURCES-1:0] payload;

        always @ (posedge clock) begin
            if (reset_warm == 1'b1) begin
                plic_irq <= 0;
            end
            else begin
                if ((mon_csr_wb_cmd === 1) && (mon_csr_wb_addr == `REG_VALIDATION1) && is_interrupt_command)
                        plic_irq <= payload;
                    else
                        plic_irq <= 0;
            end
        end
        assign is_interrupt_command = (mon_csr_wb_wdata[63:56] === 5);
        assign payload = mon_csr_wb_wdata[`PLIC_NEXTINT_SOURCES-1:0];
    // assign plic_irq = 0;
endmodule
