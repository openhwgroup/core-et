// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"
module neigh_ch_apb_mux (
  // System signals
  input   logic                                               clock               ,
  input   logic                                               reset               ,  
  // APB connection with the shire
  input   APB_to_neigh_t                                      APB_req_from_shire  ,
  output  APB_from_neigh_t                                    APB_rsp_to_shire    ,
       
  // APB connection with the minions
  input   logic [`MIN_PER_N-1:0][`ESR_APB_D_WIDTH - 1:0]      minion_apb_prdata   , 
  input   logic [`MIN_PER_N-1:0]                              minion_apb_pready   ,
  input   logic [`MIN_PER_N-1:0]                              minion_apb_pslverr  ,
  output  logic                                               minion_apb_penable  ,
  output  logic [`MIN_PER_N-1:0]                              minion_apb_psel     ,
  output  logic                                               minion_apb_pwrite   ,
  output  logic [`MIN_DM_APB_ADDR_WIDTH-1:0]                  minion_apb_paddr    ,
  output  logic [`ESR_APB_D_WIDTH  - 1:0]                     minion_apb_pwdata   ,

  // APB connection with neigh_channel
  output  APB_to_neigh_esrs_t                                 APB_req_struct_ESR      ,
  output  APB_to_neigh_icache_t                               APB_req_struct_icache   ,
  output  APB_to_neigh_tbox_t                                 APB_req_struct_tbox     ,
  input   APB_from_neigh_channel_t                            APB_rsp_struct_ESR      , 
  input   APB_from_neigh_channel_t                            APB_rsp_struct_icache   ,
  input   APB_from_neigh_channel_t                            APB_rsp_struct_tbox
);

APB_to_neigh_t      APB_req_to_min;
APB_from_neigh_t    APB_rsp_from_min;

APB_to_neigh_t      APB_req_from_shire_ff  ;
APB_from_neigh_t    APB_rsp_to_shire_ff    ;
APB_to_neigh_tbox_t APB_req_struct_tbox_next;

logic APB_rsp_ESR_valid;
logic APB_rsp_ESR_valid_next;

//       CLK    RST    DATAOUT                 DATAIN                     DEF
`RST_FF (clock, reset, APB_req_from_shire_ff,  APB_req_from_shire      , '0  )
`RST_FF (clock, reset, APB_rsp_to_shire     ,  APB_rsp_to_shire_ff     , '0  )
`RST_FF (clock, reset, APB_req_struct_tbox  ,  APB_req_struct_tbox_next, '0  )
`RST_FF (clock, reset, APB_rsp_ESR_valid    ,  APB_rsp_ESR_valid_next  , '0  )


//Signal used to control the apb response, in order to send it properly, this
//has to be controlled due to the low voltage crossing fifo behaviour
//logic APB_rsp_ready, APB_rsp_ready_next;

//        CLK    RST    DOUT           DIN                 DEF
//`RST_FF ( clock, reset, APB_rsp_ready, APB_rsp_ready_next, 1'b0)


always_comb begin
  APB_rsp_ESR_valid_next          = (APB_req_from_shire_ff.apb_psel && APB_req_from_shire_ff.apb_penable );
  //APB_rsp_ready_next = APB_req_from_shire_ff.apb_psel && APB_req_from_shire_ff.apb_penable;
  //request for minion resources 
  if ( APB_req_from_shire_ff.apb_paddr[`NEIGH_DM_APB_ADDR_WIDTH-1:`NEIGH_DM_APB_ADDR_WIDTH-2] == 2'b00 | //minion esr
       APB_req_from_shire_ff.apb_paddr[`NEIGH_DM_APB_ADDR_WIDTH-1:`NEIGH_DM_APB_ADDR_WIDTH-2] == 2'b10
    ) begin
    APB_req_to_min.apb_paddr    = APB_req_from_shire_ff.apb_paddr; 
    APB_req_to_min.apb_pwrite   = APB_req_from_shire_ff.apb_pwrite;                                         
    APB_req_to_min.apb_psel     = APB_req_from_shire_ff.apb_psel;                                           
    APB_req_to_min.apb_penable  = APB_req_from_shire_ff.apb_penable;                                        
    APB_req_to_min.apb_pwdata   = APB_req_from_shire_ff.apb_pwdata;                                         
    APB_rsp_to_shire_ff         = APB_rsp_from_min;
    APB_req_struct_ESR          = '0;
    APB_req_struct_icache       = '0;
    APB_req_struct_tbox_next    = '0;
  end else begin //request for neighborhood resources
  if ( APB_req_from_shire_ff.apb_paddr[`NEIGH_DM_APB_ADDR_WIDTH-1] ) begin //debug mode enable -> I$/TBOX
    // Disable request for ESR block
    APB_req_struct_ESR = '0;
    if ( APB_req_from_shire_ff.apb_paddr[14] == 1'b1 ) begin //request for tbox 
      APB_req_struct_icache           = '0;
      APB_req_struct_tbox_next.apb_paddr   = APB_req_from_shire_ff.apb_paddr[`TBOX_NEIGH_APB_ADDR_WIDTH-1:0]; 
      APB_req_struct_tbox_next.apb_pwrite  = APB_req_from_shire_ff.apb_pwrite;                                        
      APB_req_struct_tbox_next.apb_psel    = APB_req_from_shire_ff.apb_psel;
      APB_req_struct_tbox_next.apb_penable = APB_req_from_shire_ff.apb_penable;
      APB_req_struct_tbox_next.apb_pwdata  = APB_req_from_shire_ff.apb_pwdata;
      APB_rsp_to_shire_ff                  = APB_rsp_struct_tbox;
    end else begin //request for icache
                
      APB_req_struct_tbox_next          = '0;     
      APB_req_struct_icache.apb_paddr   = APB_req_from_shire_ff.apb_paddr[`ICACHE_NEIGH_APB_ADDR_WIDTH-1:0]; 
      APB_req_struct_icache.apb_pwrite  = APB_req_from_shire_ff.apb_pwrite;                                        
      APB_req_struct_icache.apb_psel    = APB_req_from_shire_ff.apb_psel;
      APB_req_struct_icache.apb_penable = APB_req_from_shire_ff.apb_penable;
      APB_req_struct_icache.apb_pwdata  = APB_req_from_shire_ff.apb_pwdata;
      APB_rsp_to_shire_ff               = APB_rsp_struct_icache;
    end
    end else begin // Send request to ESR block
        
      APB_req_struct_icache           = '0;
      APB_req_struct_tbox_next        = '0;
      APB_req_struct_ESR.apb_paddr    = APB_req_from_shire_ff.apb_paddr[`ESR_NEIGH_APB_AD_PP_WIDTH-1:0]; 
      APB_req_struct_ESR.apb_pwrite   = APB_req_from_shire_ff.apb_pwrite;                                        
      APB_req_struct_ESR.apb_psel     = APB_req_from_shire_ff.apb_psel;
      APB_req_struct_ESR.apb_penable  = APB_req_from_shire_ff.apb_penable;
      APB_req_struct_ESR.apb_pwdata   = APB_req_from_shire_ff.apb_pwdata;
      APB_rsp_to_shire_ff             = (APB_rsp_ESR_valid) ? APB_rsp_struct_ESR : '0;
    end

        /*
        // Needed in case that pready is always asserted to 1
        if ( APB_rsp_ready )
        begin
            if ( APB_req_from_shire_ff.apb_paddr[`NEIGH_DM_APB_ADDR_WIDTH-1] )
            begin
                if( APB_req_from_shire_ff.apb_paddr[14] == 1'b1 ) //response from tbox
                    APB_rsp_to_shire_ff = APB_rsp_struct_tbox;
                else //response from icache
                    APB_rsp_to_shire_ff = APB_rsp_struct_icache;
            end
            else
                APB_rsp_to_shire_ff = APB_rsp_struct_ESR;
        end
        else
            APB_rsp_to_shire_ff = '0;
        */

        //Disable APB request for minions
        APB_req_to_min = '0;
    end
end


// Minion APB mux
logic [`MIN_PER_N-1:0][`ESR_APB_D_WIDTH - 1:0]      minion_apb_prdata_ff  ;
logic [`MIN_PER_N-1:0]                              minion_apb_pready_ff  ;
logic [`MIN_PER_N-1:0]                              minion_apb_pslverr_ff ;

//       CLK    RST    DATAOUT                DATAIN               DEF
`RST_FF(clock, reset, minion_apb_pready_ff,  minion_apb_pready,  '0    )
`RST_FF(clock, reset, minion_apb_prdata_ff,  minion_apb_prdata,  '0    )
`RST_FF(clock, reset, minion_apb_pslverr_ff, minion_apb_pslverr, '0    )

bpam2minions #(
  .AD_IN_WIDTH(`NEIGH_DM_APB_ADDR_WIDTH),
  .AD_OUT_WIDTH(`MIN_DM_APB_ADDR_WIDTH),
  .DATA_WIDTH(`ESR_APB_D_WIDTH),
  .NR_MINIONS(`MIN_PER_N)
) dbg_connection (
  .clock                ( clock ),
  .reset                ( reset ),
  .req_apb_paddr        ( APB_req_to_min.apb_paddr    ),
  .req_apb_pwrite       ( APB_req_to_min.apb_pwrite   ), 
  .req_apb_psel         ( APB_req_to_min.apb_psel     ), 
  .req_apb_penable      ( APB_req_to_min.apb_penable  ),
  .req_apb_pwdata       ( APB_req_to_min.apb_pwdata   ),

  .rsp_apb_pready       ( APB_rsp_from_min.apb_pready   ),
  .rsp_apb_prdata       ( APB_rsp_from_min.apb_prdata   ),
  .rsp_apb_pslverr      ( APB_rsp_from_min.apb_pslverr  ),
  
  .minion_apb_paddr     ( minion_apb_paddr          ),
  .minion_apb_pwrite    ( minion_apb_pwrite         ), 
  .minion_apb_psel      ( minion_apb_psel           ), 
  .minion_apb_penable   ( minion_apb_penable        ),
  .minion_apb_pwdata    ( minion_apb_pwdata         ),
  
  .minion_apb_pready    ( minion_apb_pready_ff      ),
  .minion_apb_prdata    ( minion_apb_prdata_ff      ),
  .minion_apb_pslverr   ( minion_apb_pslverr_ff     )
);

endmodule
