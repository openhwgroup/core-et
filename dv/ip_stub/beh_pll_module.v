module beh_pll_module ();

   reg                             clk_beh_1Ghz;         // 1 Ghz clock source    
   reg                             clk_beh_650Mhz;       // 650 Mhz clock source    
   reg                             clk_beh_500Mhz;       // 500 MHz clock source
   reg                             clk_beh_500Mhz_90deg; // 500 MHz clock source
   reg                             clk_beh_200Mhz;
   reg                             clk_beh_100Mhz;       // 100 MHz clock source
   reg                             clk_beh_10Mhz;        // 10 MHz clock source
   reg                             clk_beh_25Mhz;
   reg                             clk_beh_3Mhz;

   initial begin
      #0;
      clk_beh_1Ghz = 1'b0;
      forever begin
         #0.5ns clk_beh_1Ghz = 1'b0;
         #0.5ns clk_beh_1Ghz = 1'b1;
      end
   end

   initial begin
      #0;
      clk_beh_500Mhz = 1'b0;
      forever begin
         #1ns clk_beh_500Mhz = 1'b0;
         #1ns clk_beh_500Mhz = 1'b1;
      end
   end

   initial begin
      #0.5ns;
      clk_beh_500Mhz_90deg = 1'b0;
      forever begin
         #1ns clk_beh_500Mhz_90deg = 1'b0;
         #1ns clk_beh_500Mhz_90deg = 1'b1;
      end
   end

   initial begin
      #0;
      clk_beh_200Mhz = 1'b0;
      forever begin
         #2.5ns clk_beh_200Mhz = 1'b0;
         #2.5ns clk_beh_200Mhz = 1'b1;
      end
   end

   initial begin
      #0;
      clk_beh_100Mhz = 1'b0;
      forever begin
         #5ns clk_beh_100Mhz = 1'b0;
         #5ns clk_beh_100Mhz = 1'b1;
      end
   end

   initial begin
      #0;
      clk_beh_10Mhz = 1'b0;
      forever begin
         #50ns clk_beh_10Mhz = 1'b0;
         #50ns clk_beh_10Mhz = 1'b1;
      end
   end

   initial begin
      #0;
      clk_beh_25Mhz = 1'b0;
      forever begin
         #20ns clk_beh_25Mhz = 1'b0;
         #20ns clk_beh_25Mhz = 1'b1;
      end
   end

   initial begin
      #0;
      clk_beh_650Mhz = 1'b0;
      forever begin
         #0.769ns clk_beh_650Mhz = 1'b0;
         #0.769ns clk_beh_650Mhz = 1'b1;
      end
   end


   initial begin
      #0;
      clk_beh_3Mhz = 1'b0;
      forever begin
         #166.67ns clk_beh_3Mhz = 1'b0;
         #166.67ns clk_beh_3Mhz = 1'b1;
      end
   end

endmodule

