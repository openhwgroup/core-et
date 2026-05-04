
export "DPI-C" function endSimAt_DPI;
import "DPI-C" function void setEnded();
logic endSimEnable = 0;
int endSimTimeout;
int nextEndSimTimeout;
logic nextEndSimTimeout_valid;


function void endSimAt_DPI(input int unsigned t);
   begin
      if (t == 0) begin
         setEnded();
         `ifdef EVL_SIMULATION
            `dut_error("COSIM(DPI)", "cosim error detected")
         `else // ifdef EVL_SIMULATION
            $finish;
         `endif // !ifdef EVL_SIMULATION
      end
      else if ((endSimEnable && (endSimTimeout > t)) || (!endSimEnable)) begin
         nextEndSimTimeout_valid = 1;
         nextEndSimTimeout = t;
         endSimEnable = 1;
      end
   end
endfunction // endSimAt

always_ff@(posedge clock) begin
   if (endSimEnable) begin
      if (nextEndSimTimeout_valid) begin
         nextEndSimTimeout_valid = 0;
         endSimTimeout <= nextEndSimTimeout;
      end
      else begin
         endSimTimeout <= endSimTimeout - 1;
         if (endSimTimeout == 0)  begin
            setEnded();
            `ifdef EVL_SIMULATION
               `dut_error("COSIM(DPI)", "timeout detected")
            `else // ifdef EVL_SIMULATION
               $finish;
            `endif // !ifdef EVL_SIMULATION
         end
      end
   end
end
