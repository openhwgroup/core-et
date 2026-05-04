
module cpu_subsystem_clock_gen
(
 output logic clock_1ghz,
 output logic  clock_200mhz
 );

   clock_parametrizer
   #( .CLOCK_NAME ("CPU_SS_CLK"),
      .CLOCK_MHZ (1000))
   clk_1ghz_gen ( .clk (clock_1ghz));
   clock_parametrizer
   #( .CLOCK_NAME ("REF_CLK"),
      .CLOCK_MHZ (200))
   clk_200mhz_gen ( .clk (clock_200mhz));

endmodule

module clock_parametrizer
#
(
 parameter string CLOCK_NAME,
 parameter longint unsigned CLOCK_MHZ
)
(
 output logic clk
 );
   localparam CLOCK_FREQ     = {CLOCK_NAME, "_FREQ_MHZ=%d"};
   localparam CLOCK_RAND_OFF = {CLOCK_NAME, "_RAND_OFF"};
   localparam CLOCK_JITTER = {CLOCK_NAME,"_JITTER=%f"};
   localparam FREQ_TOLERANCE = {CLOCK_NAME,"_FREQ_TOLERANCE=%d"};
   localparam CLOCK_DUTY_CYCLE_LOW = {CLOCK_NAME,"_CLOCK_DUTY_CYCLE_LOW=%d"};
   localparam CLOCK_DUTY_CYCLE_HIGH = {CLOCK_NAME,"_CLOCK_DUTY_CYCLE_HIGH=%d"};
   localparam longint unsigned US_TO_SEC = 1000*1000; //10^6
   localparam longint unsigned SEC_TO_NS = 1000*1000*1000; //10^9
   localparam longint unsigned NS_TO_MS  = 1000*1000; //10^6      
   localparam real TARGET_CLOCK_PERIOD_SEC = 1.0/(real'(CLOCK_MHZ*US_TO_SEC));
   // Integer instead of real to avoid extra features in VCS due the randomization call
   localparam longint unsigned TARGET_CLOCK_PERIOD = TARGET_CLOCK_PERIOD_SEC*SEC_TO_NS*NS_TO_MS; 
   
   real       clock_jitter;
   int        clock_freq_tolerance;
   int        clock_duty_cycle_low;
   int        clock_duty_cycle_high;
   int        duty_cycle;
   longint unsigned clock_period;
   bit              clock_rand_off;


   initial begin
      #0;
      // Performing conversion here to bypass limitiations in VCS
      clock_rand_off = $test$plusargs(CLOCK_RAND_OFF);
      
      if (!$value$plusargs(CLOCK_JITTER, clock_jitter)) begin
         clock_jitter = 0.0;
      end
      
      if (!$value$plusargs(FREQ_TOLERANCE, clock_freq_tolerance)) begin
         clock_freq_tolerance = 0;
      end
      
      if (!$value$plusargs(CLOCK_DUTY_CYCLE_LOW, clock_duty_cycle_low)) begin
         clock_duty_cycle_low = 0;
      end
      
      if (!$value$plusargs(CLOCK_DUTY_CYCLE_HIGH, clock_duty_cycle_high)) begin
         clock_duty_cycle_high = 0;
      end

      if (!clock_rand_off) begin
         // 0 means default value -> 50%
         if (clock_duty_cycle_high == 0) clock_duty_cycle_high = 50;
         if (clock_duty_cycle_low  == 0) clock_duty_cycle_low  = 50;

         // Legalize duty cycle between 60% and 40%
         // Low can't be greater than high
         clock_duty_cycle_high = clock_duty_cycle_high > 60 ? 60 : clock_duty_cycle_high;
         clock_duty_cycle_low  = clock_duty_cycle_low < 40 ? 40 : clock_duty_cycle_low;

         if (clock_duty_cycle_high < clock_duty_cycle_low)
           clock_duty_cycle_high = clock_duty_cycle_low;

         randomize(duty_cycle) with { 
            duty_cycle >= clock_duty_cycle_low;
            duty_cycle <= clock_duty_cycle_high;
         };

         if (clock_freq_tolerance > 0 && clock_freq_tolerance < 20) begin
            randomize(clock_period) with { 
               clock_period >= ((TARGET_CLOCK_PERIOD*(100-clock_freq_tolerance)) / 100);
               clock_period <= ((TARGET_CLOCK_PERIOD*(100+clock_freq_tolerance)) / 100);
            };
         end else begin
            clock_period = TARGET_CLOCK_PERIOD;
         end
      end else begin
         duty_cycle = 50;
         clock_jitter = 0.0;
         clock_period = TARGET_CLOCK_PERIOD;
      end // else: !if(!clock_rand_off)

      // Drive clock gen parameters
      clock_gen.period = (real'(clock_period))/NS_TO_MS;
      clock_gen.jitter_percent = clock_jitter;
      clock_gen.duty_cycle = duty_cycle;
   end

     clock_gen clock_gen (.clk(clk));

endmodule


module clock_gen
(
 output logic clk
);

   real jitter_percent = 0.0;  // jitter in percents
   real period         = 10;
   int  duty_cycle     = 50; // duty cycle in percent
   int  jitter;
   int  jitter_boundary;
   real real_jitter;
   real clk_first_half_period;
   real clk_second_half_period;
   real clk_half_period;

   //
   // The following code forces a delayed evaluation of jitter_percent (although it still happens at time 0).
   //
   bit [3:0] delay_start_1;
   bit [3:0] delay_start_2;

   initial begin
      #0 delay_start_1 = 4'h0;
      while (delay_start_1 !== 4'hf) begin
         wait (delay_start_1 !== delay_start_2);
         #0 delay_start_1 <= delay_start_2;
         wait (delay_start_1 === delay_start_2);
      end
   end

   initial begin
      delay_start_2 = 4'h0;
      while (delay_start_2 !== 4'hf) begin
         #0 delay_start_2 = delay_start_2 + 1;
         wait (delay_start_1 === delay_start_2);
      end
   end

   initial begin
      #0;
      clk = 1'b0;
      wait (delay_start_1 === 4'hf);
      clk_first_half_period  = real'((real'(duty_cycle) * period) / (real'(100)));
      clk_second_half_period = period - clk_first_half_period;
      clk_half_period        = real'(period / (real'(2)));
      jitter_boundary        = int'(real'(clk_half_period * 100.0 * jitter_percent * 100.0));
      $display("SYSTEM %s", $sformatf("%m: duty cycle:%0d, period:%5.3f(%5.3f,%5.3f), max jitter:%5.5f", duty_cycle, period, clk_first_half_period, clk_second_half_period, real'(real'(jitter_boundary) / real'(1000000))));

      //
      // If jitter_percent is 0, we are not using clock jitter in this simulation ... don't spend
      // time evaluating it.
      //
      fork
         forever begin
            @(period);
            wait (clk === 1'b0);
            wait (clk === 1'b1);
            #0;
            clk_first_half_period  = real'((real'(duty_cycle) * period) / (real'(100)));
            clk_second_half_period = period - clk_first_half_period;
            clk_half_period        = real'(period / (real'(2)));
            jitter_boundary        = int'(real'(clk_half_period * 100.0 * jitter_percent * 100.0));
         end
      join_none
      if (jitter_percent == 0) begin
         #(clk_second_half_period) clk = 1'b1;
         #(clk_first_half_period)  clk = 1'b0;
      end
         forever begin
            randomize(jitter) with { jitter >= -jitter_boundary;
                                     jitter <= jitter_boundary;
                                   };
            real_jitter = real'(real'(jitter) / real'(1000000));
            #(clk_second_half_period + real_jitter) clk = 1'b1;
            #(clk_first_half_period + real_jitter)  clk = 1'b0;
         end
      end
endmodule
