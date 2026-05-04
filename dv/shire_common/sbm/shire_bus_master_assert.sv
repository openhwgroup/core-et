// assertion usage example:
// `ASSERT_ALWAYS_MSG(ERROR_nameOfAssertion, clk, reset_cold, (assert always expression), $sformatf("your strings with %stuff", $sampled(var) ) )
// PORTS
`ifndef SBM_IFC
 `define SBM_IFC shire_top.shire_channel_wrap_inst.shire_control_gen.shire_control_status_inst.sbm_top
 `define SBM_NOC_IFC `SBM_IFC.sbm_nocs
`endif

// Defines from PRM-19 - Minion Shire ESR Map 
`define MSB_ESR_ADDR_R      39:32
`define SHIRE_ID_ADDR_R     29:22
`define ESR_REGION_ADDR_R   21:20
`define SHIRE_SUBREG_ADDR_R 19:17
`define NEIGH_ADDR_R        19:16 
`define SC_BANK_ADDR_R      16:13 
`define MINION_ADDR_R       19:13 
localparam minion_esr       = 0;
localparam neigh_esr        = 1;
localparam shire_esr        = 3;
localparam sc_bank_esr      = 0;
localparam rbox_esr         = 1;
localparam shire_other_esr  = 2;
localparam decode_bdc_req   = 4'hf; 
localparam sbm_addr_size    = `SC_MESH_MASTER_AXI_ADDR_SIZE;
localparam sbm_req_id_size  = `SC_MESH_SLAVE_AXI_ID_SIZE;
localparam sbm_data_size    = `ESR_APB_D_WIDTH;
localparam fifo_depth       = `SBM_WRITE_CREDITS;
localparam NUM_MINIONS      = `NUM_NEIGH*`MIN_PER_N;


// -------- Auxiliary signals -------
// Aux decode request
logic[sbm_addr_size-1:0] m_sbm_req_addr;
logic[sbm_addr_size-1:0] m_sbm_wr_req_addr;
logic[sbm_addr_size-1:0] m_sbm_rd_req_addr;
logic                    m_sbm_wr_req_valid;
logic                    m_sbm_rd_req_valid;
logic                    m_sbm_wr_req_ready;
logic                    m_sbm_rd_req_ready;
logic m_neigh_wr_req, m_neigh_rd_req, m_neigh_req;
logic m_minion_wr_req, m_minion_rd_req, m_minion_req;
logic m_sc_wr_req;
logic m_sc_rd_req;
logic m_sc_req;
logic m_rbox_wr_req;
logic m_rbox_rd_req;
logic m_rbox_req;
logic m_shire_wr_req, m_shire_rd_req, m_shire_req;
logic m_simult_same_endpoint;
// Aux decode error 
logic m_dec_err_msb, m_dec_err_msb_wr_req, m_dec_err_msb_rd_req;
logic m_dec_err_shire, m_dec_err_shire_wr_req, m_dec_err_shire_rd_req;
logic m_dec_err_neigh, m_dec_err_neigh_wr_req, m_dec_err_neigh_rd_req;
logic m_dec_err_minion, m_dec_err_minion_wr_req, m_dec_err_minion_rd_req;
logic m_dec_err_sc_bank, m_dec_err_sc_bank_wr_req, m_dec_err_sc_bank_rd_req;
logic m_dec_err_rbox, m_dec_err_rbox_wr_req, m_dec_err_rbox_rd_req;
logic m_dec_err, m_dec_err_wr_req, m_dec_err_rd_req;
 //Aux BDC
logic m_neigh_bdc_req;
logic m_sc_bdc_req;
logic m_bdc_req;
logic[`NUM_NEIGH-1:0] bdc_neigh_sync_start, bdc_neigh_err_rsp;
logic[`SC_BANKS-1:0] bdc_sc_sync_start, bdc_sc_err_rsp;
//Aux endpoint in reset
logic                      m_wr_req_to_neigh_in_reset, m_rd_req_to_neigh_in_reset;
logic[sbm_req_id_size-1:0] m_shire_wr_req_id_to_neigh_in_reset, m_shire_rd_req_id_to_neigh_in_reset;
logic                      m_wr_req_to_sc_in_reset, m_rd_req_to_sc_in_reset;
logic[sbm_req_id_size-1:0] m_shire_wr_req_id_to_sc_in_reset, m_shire_rd_req_id_to_sc_in_reset;
logic                      m_wr_req_to_rbox_in_reset, m_rd_req_to_rbox_in_reset;
logic[sbm_req_id_size-1:0] m_shire_wr_req_id_to_rbox_in_reset, m_shire_rd_req_id_to_rbox_in_reset;
//     Aux AXI to APB conversion 
//Aux simultaneous read and write req
logic[`SBM_APB_INTERFACES-1:0]                      m_wr_req_to_endpoint, m_rd_req_to_endpoint;
logic[`SBM_APB_INTERFACES-1:0][sbm_req_id_size-1:0] m_wr_req_to_endpoint_id, m_rd_req_to_endpoint_id;
logic[`SBM_APB_INTERFACES-1:0]                      m_new_simult_req, m_apb_rd_req_seen, m_apb_wr_req_seen, m_wr_req_was_first;
//Aux correct data chunk extracted
logic[2:0]               m_data_chunk;
logic[sbm_data_size-1:0] m_apb_data;
reg[`SBM_APB_INTERFACES-1:0] [sbm_addr_size-1:0] m_wr_req_to_endpoint_addr [0:fifo_depth-1];
reg[`SBM_APB_INTERFACES-1:0] [sbm_data_size-1:0] m_wr_req_to_endpoint_data [0:fifo_depth-1];
int m_req_idx[`SBM_APB_INTERFACES-1:0];

if (SC_STUB==0) begin
   assign m_sbm_wr_req_addr  = `SBM_NOC_IFC.sbm_mesh_slave_axi_AW.ADDR;
   assign m_sbm_rd_req_addr  = `SBM_NOC_IFC.sbm_mesh_slave_axi_AR.ADDR;
   assign m_sbm_wr_req_valid = `SBM_NOC_IFC.sbm_mesh_slave_axi_AWVALID & `SBM_NOC_IFC.sbm_mesh_slave_axi_AWREADY;
   assign m_sbm_rd_req_valid = `SBM_NOC_IFC.sbm_mesh_slave_axi_ARVALID;
   assign m_sbm_wr_req_ready = `SBM_NOC_IFC.sbm_mesh_slave_axi_AWREADY;
   assign m_sbm_rd_req_ready = `SBM_NOC_IFC.sbm_mesh_slave_axi_ARREADY;

   // Decode error
   assign m_neigh_wr_req  = (m_sbm_wr_req_addr[`ESR_REGION_ADDR_R] == neigh_esr);
   assign m_neigh_rd_req  = (m_sbm_rd_req_addr[`ESR_REGION_ADDR_R] == neigh_esr);
   assign m_neigh_req     = |{m_neigh_wr_req,m_neigh_rd_req};
   assign m_minion_wr_req = (m_sbm_wr_req_addr[`ESR_REGION_ADDR_R] == minion_esr);
   assign m_minion_rd_req = (m_sbm_rd_req_addr[`ESR_REGION_ADDR_R] == minion_esr);
   assign m_minion_req    = |{m_minion_wr_req,m_minion_rd_req};
   assign m_sc_wr_req     = ((m_sbm_wr_req_addr[`ESR_REGION_ADDR_R] == shire_esr) && (m_sbm_wr_req_addr[`SHIRE_SUBREG_ADDR_R] == sc_bank_esr));
   assign m_sc_rd_req     = ((m_sbm_rd_req_addr[`ESR_REGION_ADDR_R] == shire_esr) && (m_sbm_rd_req_addr[`SHIRE_SUBREG_ADDR_R] == sc_bank_esr));
   assign m_sc_req        = |{m_sc_wr_req,m_sc_rd_req};
   assign m_rbox_wr_req   = ((m_sbm_wr_req_addr[`ESR_REGION_ADDR_R] == shire_esr) && (m_sbm_wr_req_addr[`SHIRE_SUBREG_ADDR_R] == rbox_esr));
   assign m_rbox_rd_req   = ((m_sbm_rd_req_addr[`ESR_REGION_ADDR_R] == shire_esr) && (m_sbm_rd_req_addr[`SHIRE_SUBREG_ADDR_R] == rbox_esr));
   assign m_rbox_req      = |{m_rbox_wr_req,m_rbox_rd_req};
   assign m_shire_wr_req  = ((m_sbm_wr_req_addr[`ESR_REGION_ADDR_R] == shire_esr) && (m_sbm_wr_req_addr[`SHIRE_SUBREG_ADDR_R] == shire_other_esr));
   assign m_shire_rd_req  = ((m_sbm_rd_req_addr[`ESR_REGION_ADDR_R] == shire_esr) && (m_sbm_rd_req_addr[`SHIRE_SUBREG_ADDR_R] == shire_other_esr));
   assign m_shire_req     = |{m_shire_wr_req,m_shire_rd_req};

   // Simultaneous read and write requests to the same endpoint
   assign m_simult_same_endpoint = (m_dec_err != 1) && (m_sbm_wr_req_valid && m_sbm_wr_req_ready && m_sbm_rd_req_valid && m_sbm_rd_req_ready) && (      // valid, accepted requests that did not cause dec err 
                                   (m_neigh_wr_req && m_neigh_rd_req && (m_sbm_wr_req_addr[`NEIGH_ADDR_R] == m_sbm_rd_req_addr[`NEIGH_ADDR_R]))  |       // same neigh
                                   (m_minion_wr_req && m_minion_rd_req && (m_sbm_wr_req_addr[`MINION_ADDR_R] == m_sbm_rd_req_addr[`MINION_ADDR_R]))  |   // same minion
                                   (m_sc_wr_req && m_sc_rd_req && (m_sbm_wr_req_addr[`SC_BANK_ADDR_R] == m_sbm_rd_req_addr[`SC_BANK_ADDR_R]))  |         // same sc bank
                                   (m_shire_wr_req && m_shire_rd_req)  |                                                                                // shire other
                                   (m_rbox_wr_req && m_rbox_rd_req) );                                                                                  // rbox
                            

   // Decode errors
   assign m_dec_err_msb_wr_req = (m_sbm_wr_req_addr[`MSB_ESR_ADDR_R] != 8'h01) && m_sbm_wr_req_valid && m_sbm_wr_req_ready ? 1'b1 : 1'b0;
   assign m_dec_err_msb_rd_req = (m_sbm_rd_req_addr[`MSB_ESR_ADDR_R] != 8'h01) && m_sbm_rd_req_valid && m_sbm_rd_req_ready ? 1'b1 : 1'b0;
   assign m_dec_err_msb        = |{m_dec_err_msb_wr_req,m_dec_err_msb_rd_req};
   assign m_dec_err_shire_wr_req = (m_sbm_wr_req_addr[`SHIRE_ID_ADDR_R] != `SBM_NOC_IFC.shire_virtual_id) && m_sbm_wr_req_valid && m_sbm_wr_req_ready ? 1'b1 : 1'b0;
   assign m_dec_err_shire_rd_req = (m_sbm_rd_req_addr[`SHIRE_ID_ADDR_R] != `SBM_NOC_IFC.shire_virtual_id) && m_sbm_rd_req_valid && m_sbm_rd_req_ready ? 1'b1 : 1'b0;
   assign m_dec_err_shire        = |{m_dec_err_shire_wr_req,m_dec_err_shire_rd_req};
   assign m_dec_err_neigh_wr_req = m_neigh_wr_req && m_sbm_wr_req_valid && m_sbm_wr_req_ready ?        // Request targets neigh esrs
                                   ((m_sbm_wr_req_addr[`NEIGH_ADDR_R] >= `NUM_NEIGH) && (m_sbm_wr_req_addr[`NEIGH_ADDR_R] != decode_bdc_req)) // Request targets non-existing neigh and is not bdc
                                   | ((m_sbm_wr_req_addr[`NEIGH_ADDR_R] < `NUM_NEIGH) && (`SBM_IFC.reset_neigh[m_sbm_wr_req_addr[`NEIGH_ADDR_R]])) // Request targets neigh in reset
                                   | ((m_sbm_wr_req_addr[`NEIGH_ADDR_R] == decode_bdc_req) && (`SBM_IFC.reset_neigh > 0)) // Bcast request when any of the neighs is in reset
                                   ? 1'b1 : 1'b0 : 1'b0;
   assign m_dec_err_neigh_rd_req = m_neigh_rd_req && m_sbm_rd_req_valid && m_sbm_rd_req_ready ?        // Request targets neigh esrs
                                   (m_sbm_rd_req_addr[`NEIGH_ADDR_R] >= `NUM_NEIGH) // Request targets non-existing neigh
                                   | ((m_sbm_rd_req_addr[`NEIGH_ADDR_R] < `NUM_NEIGH) && (`SBM_IFC.reset_neigh[m_sbm_rd_req_addr[`NEIGH_ADDR_R]])) // Request targets neigh in reset
                                   ? 1'b1 : 1'b0 : 1'b0;
   assign m_dec_err_neigh        = |{m_dec_err_neigh_wr_req,m_dec_err_neigh_rd_req};
   assign m_dec_err_minion_wr_req = m_minion_wr_req && m_sbm_wr_req_valid && m_sbm_wr_req_ready ?        // Request targets minion esrs
                                     (m_sbm_wr_req_addr[`MINION_ADDR_R] >= NUM_MINIONS) | (`SBM_IFC.reset_neigh[m_sbm_wr_req_addr[`NEIGH_ADDR_R]]) ? 1'b1 : 1'b0 : 1'b0; // Request targets non-existing minion or minion belongs to neigh in reset
   assign m_dec_err_minion_rd_req = m_minion_rd_req && m_sbm_rd_req_valid && m_sbm_rd_req_ready ?        // Request targets minion esrs
                                     (m_sbm_rd_req_addr[`MINION_ADDR_R] >= NUM_MINIONS) | (`SBM_IFC.reset_neigh[m_sbm_rd_req_addr[`NEIGH_ADDR_R]]) ? 1'b1 : 1'b0 : 1'b0; // Request targets non-existing minion or minion belongs to neigh in reset
   assign m_dec_err_minion        = |{m_dec_err_minion_wr_req,m_dec_err_minion_rd_req};
   assign m_dec_err_sc_bank_wr_req = m_sc_wr_req && m_sbm_wr_req_valid && m_sbm_wr_req_ready ?        // Request targets sc esrs
                                      (((m_sbm_wr_req_addr[`SC_BANK_ADDR_R] >= `SC_BANKS) && (m_sbm_wr_req_addr[`SC_BANK_ADDR_R] != decode_bdc_req)) | `SBM_IFC.reset_sc) // Request targets non-existing sc and is not bdc or cache is in reset
                                     ? 1'b1 : 1'b0 : 1'b0; 
   assign m_dec_err_sc_bank_rd_req = m_sc_rd_req && m_sbm_rd_req_valid && m_sbm_rd_req_ready ?        // Request targets sc esrs
                                      (m_sbm_rd_req_addr[`SC_BANK_ADDR_R] >= `SC_BANKS) | `SBM_IFC.reset_sc ? 1'b1 : 1'b0 : 1'b0;  // Request targets non-existing sc or cache is disabled
   assign m_dec_err_sc_bank        = |{m_dec_err_sc_bank_wr_req,m_dec_err_sc_bank_rd_req};
   assign m_dec_err_rbox_wr_req = m_rbox_wr_req && m_sbm_wr_req_valid && m_sbm_wr_req_ready ?        // Request targets rbox esrs
                                     `SBM_IFC.reset_rbox // Request targets rbox in reset
                                     ? 1'b1 : 1'b0 : 1'b0; 
   assign m_dec_err_rbox_rd_req = m_rbox_rd_req && m_sbm_rd_req_valid && m_sbm_rd_req_ready ?        // Request targets rbox esrs
                                      `SBM_IFC.reset_rbox ? 1'b1 : 1'b0 : 1'b0;  // Request targets rbox in reset
   assign m_dec_err_rbox        = |{m_dec_err_rbox_wr_req,m_dec_err_rbox_rd_req};
   assign m_dec_err_wr_req  = |{m_dec_err_msb_wr_req,m_dec_err_shire_wr_req,m_dec_err_neigh_wr_req,m_dec_err_minion_wr_req,m_dec_err_sc_bank_wr_req,m_dec_err_rbox_wr_req};
   assign m_dec_err_rd_req  = |{m_dec_err_msb_rd_req,m_dec_err_shire_rd_req,m_dec_err_neigh_rd_req,m_dec_err_minion_rd_req,m_dec_err_sc_bank_rd_req,m_dec_err_rbox_rd_req};
   assign m_dec_err         = |{m_dec_err_wr_req,m_dec_err_rd_req};

   assign m_neigh_bdc_req = (m_sbm_wr_req_addr[`ESR_REGION_ADDR_R] == neigh_esr) && (m_sbm_wr_req_addr[`NEIGH_ADDR_R] == decode_bdc_req) && (m_dec_err_wr_req==0); //neigh bdc req and no decode error
   assign m_sc_bdc_req    = (m_sbm_wr_req_addr[`ESR_REGION_ADDR_R] == shire_esr) && (m_sbm_wr_req_addr[`SHIRE_SUBREG_ADDR_R] == sc_bank_esr) && (m_sbm_wr_req_addr[`SC_BANK_ADDR_R] == decode_bdc_req) && (m_dec_err_wr_req==0); //sc bdc req and no decode error
   assign m_bdc_req       = |{m_neigh_bdc_req,m_sc_bdc_req};

   assign m_wr_req_to_neigh_in_reset = ((m_sbm_wr_req_addr[`ESR_REGION_ADDR_R] <= neigh_esr) && (m_sbm_wr_req_addr[`NEIGH_ADDR_R]<`NUM_NEIGH)) ? `SBM_IFC.reset_neigh[m_sbm_wr_req_addr[`NEIGH_ADDR_R]] : 1'b0; //BDC requests not included
   assign m_shire_wr_req_id_to_neigh_in_reset = m_wr_req_to_neigh_in_reset ? `SBM_NOC_IFC.sbm_mesh_slave_axi_AW.ID : m_shire_wr_req_id_to_neigh_in_reset; 
   assign m_rd_req_to_neigh_in_reset = ((m_sbm_rd_req_addr[`ESR_REGION_ADDR_R] <= neigh_esr) && (m_sbm_rd_req_addr[`NEIGH_ADDR_R]<`NUM_NEIGH)) ? `SBM_IFC.reset_neigh[m_sbm_rd_req_addr[`NEIGH_ADDR_R]] : 1'b0;
   assign m_shire_rd_req_id_to_neigh_in_reset = m_rd_req_to_neigh_in_reset ? `SBM_NOC_IFC.sbm_mesh_slave_axi_AR.ID : m_shire_rd_req_id_to_neigh_in_reset;
   assign m_wr_req_to_sc_in_reset = (m_sc_wr_req && `SBM_IFC.reset_sc) ? 1'b1 : 1'b0;
   assign m_shire_wr_req_id_to_sc_in_reset = m_wr_req_to_sc_in_reset ? `SBM_NOC_IFC.sbm_mesh_slave_axi_AW.ID : m_shire_wr_req_id_to_sc_in_reset; 
   assign m_rd_req_to_sc_in_reset = (m_sc_rd_req && `SBM_IFC.reset_sc) ? 1'b1 : 1'b0;
   assign m_shire_rd_req_id_to_sc_in_reset = m_rd_req_to_sc_in_reset ? `SBM_NOC_IFC.sbm_mesh_slave_axi_AR.ID : m_shire_rd_req_id_to_sc_in_reset; 
   assign m_wr_req_to_rbox_in_reset = (m_rbox_wr_req && `SBM_IFC.reset_rbox) ? 1'b1 : 1'b0;
   assign m_shire_wr_req_id_to_rbox_in_reset = m_wr_req_to_rbox_in_reset ? `SBM_NOC_IFC.sbm_mesh_slave_axi_AW.ID : m_shire_wr_req_id_to_rbox_in_reset; 
   assign m_rd_req_to_rbox_in_reset = (m_rbox_rd_req && `SBM_IFC.reset_rbox) ? 1'b1 : 1'b0;
   assign m_shire_rd_req_id_to_rbox_in_reset = m_rd_req_to_rbox_in_reset ? `SBM_NOC_IFC.sbm_mesh_slave_axi_AR.ID : m_shire_rd_req_id_to_rbox_in_reset; 

   assign m_data_chunk = {1'b0, m_sbm_wr_req_addr[4:3]}; // 0, 1, 2, 3 64bits to extract from data[255:0] 
   assign m_apb_data   = `SBM_NOC_IFC.sbm_mesh_slave_axi_W.DATA[m_data_chunk*sbm_data_size+:sbm_data_size];
end //if (SC_STUB==0)

genvar sbm_port;
generate
if (SC_STUB==0) begin

//BROADCAST
//if (NEIGH_STUB == 0) begin
 for (genvar neigh_bdc=0; neigh_bdc<`NUM_NEIGH; neigh_bdc++) begin : sva_neigh_bdc
    always_comb begin
     bdc_neigh_sync_start[neigh_bdc] = (`SBM_NOC_IFC.gen_sbm_axi_to_apb[neigh_bdc+`NEIGHS_RANGE_LO].axi_to_apb.apb_paddr ==`SBM_NOC_IFC.gen_sbm_axi_to_apb[`NEIGHS_RANGE_LO].axi_to_apb.apb_paddr) && `SBM_NOC_IFC.gen_sbm_axi_to_apb[neigh_bdc+`NEIGHS_RANGE_LO].axi_to_apb.apb_pwrite && `SBM_NOC_IFC.gen_sbm_axi_to_apb[neigh_bdc+`NEIGHS_RANGE_LO].axi_to_apb.apb_psel && `SBM_NOC_IFC.gen_sbm_axi_to_apb[neigh_bdc+`NEIGHS_RANGE_LO].axi_to_apb.apb_penable;

     if (`SBM_NOC_IFC.bcast_active[`NEIGHS_RANGE] && (`SBM_NOC_IFC.bcast_neighs_valid_mask[neigh_bdc+`NEIGHS_RANGE_LO]==0) && `SBM_NOC_IFC.gen_sbm_axi_to_apb[neigh_bdc+`NEIGHS_RANGE_LO].axi_to_apb.apb_pready)
        bdc_neigh_err_rsp[neigh_bdc] = `SBM_NOC_IFC.gen_sbm_axi_to_apb[neigh_bdc+`NEIGHS_RANGE_LO].axi_to_apb.apb_pslverr;
     else if (`SBM_NOC_IFC.broadcast_response[0]) 
        bdc_neigh_err_rsp[neigh_bdc] = 0;

    end //always_comb
 end //for
//end // NEIGH_STUB
 for (genvar sc_bdc=0; sc_bdc<`SC_BANKS; sc_bdc++) begin : sva_sc_bdc
    always_comb begin
     bdc_sc_sync_start[sc_bdc] = (`SBM_NOC_IFC.gen_sbm_axi_to_apb[sc_bdc+`BANKS_RANGE_LO].axi_to_apb.apb_paddr ==`SBM_NOC_IFC.gen_sbm_axi_to_apb[`BANKS_RANGE_LO].axi_to_apb.apb_paddr) && `SBM_NOC_IFC.gen_sbm_axi_to_apb[sc_bdc+`BANKS_RANGE_LO].axi_to_apb.apb_pwrite && `SBM_NOC_IFC.gen_sbm_axi_to_apb[sc_bdc+`BANKS_RANGE_LO].axi_to_apb.apb_psel && `SBM_NOC_IFC.gen_sbm_axi_to_apb[sc_bdc+`BANKS_RANGE_LO].axi_to_apb.apb_penable;
     
     if (`SBM_NOC_IFC.bcast_active[`BANKS_RANGE] && (`SBM_NOC_IFC.bcast_banks_valid_mask[sc_bdc+`BANKS_RANGE_LO]==0) && `SBM_NOC_IFC.gen_sbm_axi_to_apb[sc_bdc+`BANKS_RANGE_LO].axi_to_apb.apb_pready) begin

      bdc_sc_err_rsp[sc_bdc] = `SBM_NOC_IFC.gen_sbm_axi_to_apb[sc_bdc+`BANKS_RANGE_LO].axi_to_apb.apb_pslverr;
      end
     else if (`SBM_NOC_IFC.broadcast_response==2'h2)
        bdc_sc_err_rsp[sc_bdc] = 0;

    end //always_comb
 end //for

// ----- SVA BDC op should start at the same time for all destinations -----
//if (NEIGH_STUB == 0) begin
`ASSERT_ALWAYS_MSG(sva_bdc_neigh_sync_start, shire_clock, reset_cold,
     ((m_neigh_bdc_req && m_sbm_wr_req_valid) |-> s_eventually (&(bdc_neigh_sync_start))), "Broadcast operation did not start synchronously for all neighs");
//end
`ASSERT_ALWAYS_MSG(sva_bdc_sc_banks_sync_start, shire_clock, reset_cold,
     ((m_sc_bdc_req && m_sbm_wr_req_valid) |-> s_eventually (&(bdc_sc_sync_start))), "Broadcast operation did not start synchronously for all shire cache banks");

// ----- SVA If at least one BDC rsp is error, axi response is error ----
`ASSERT_ALWAYS_MSG(sva_one_bdc_neigh_err_axi_rsp_err, shire_clock, reset_cold,
     ((|bdc_neigh_err_rsp && $fell(|(`SBM_NOC_IFC.bcast_active[`NEIGHS_RANGE]))) |-> s_eventually ((`SBM_NOC_IFC.broadcast_response==2'b10) && (`SBM_NOC_IFC.bcast_response_B.RESP == 2'b10))), "Broadcast operation did not send AXI error response");
`ASSERT_ALWAYS_MSG(sva_one_bdc_sc_err_axi_rsp_err, shire_clock, reset_cold,
     (((|bdc_sc_err_rsp) && $fell(|(`SBM_NOC_IFC.bcast_active[`BANKS_RANGE])) ) |-> s_eventually (`SBM_NOC_IFC.broadcast_response[0] && (`SBM_NOC_IFC.bcast_response_B.RESP == 2'b10))), "Broadcast operation did not send AXI error response");

// ----- SVA BDC op returns only one axi response -----

`ASSERT_ALWAYS_MSG(sva_neigh_bdc_wait_for_all_rsp_to_send_axi_rsp, shire_clock, reset_cold,
     (`SBM_NOC_IFC.bcast_rsp_sent && $past(`SBM_NOC_IFC.broadcast_response[0] && (m_dec_err!=1), 1) |-> $past(`SBM_NOC_IFC.bcast_control[0].counter == `NUM_NEIGH, 1)), "SBM did not wait for all Neighs to perform broadcast request to send AXI response");
`ASSERT_ALWAYS_MSG(sva_sc_bdc_wait_for_all_rsp_to_send_axi_rsp, shire_clock, reset_cold,
     (`SBM_NOC_IFC.bcast_rsp_sent && $past((`SBM_NOC_IFC.broadcast_response==2'h2) && (m_dec_err!=1), 1) |-> $past(`SBM_NOC_IFC.bcast_control[1].counter == `SC_BANKS, 1)), "SBM did not wait for all SC Banks to perform broadcast request to send AXI response"); //Neigh BDC response has priority if both are ready at the same time

//
 for (genvar i=0; i < `SBM_APB_INTERFACES; i++) begin : sva_sbm
    always_comb begin

    if ((i >= `NEIGHS_RANGE_LO) && (i < `NEIGHS_RANGE_HI)) begin
    m_wr_req_to_endpoint[i] = (m_sbm_wr_req_addr[`ESR_REGION_ADDR_R] <= neigh_esr) && (m_sbm_wr_req_addr[`NEIGH_ADDR_R]==(i-`NEIGHS_RANGE_LO));
    m_rd_req_to_endpoint[i] = (m_sbm_rd_req_addr[`ESR_REGION_ADDR_R] <= neigh_esr) && (m_sbm_rd_req_addr[`NEIGH_ADDR_R]==(i-`NEIGHS_RANGE_LO));
    end
    else if ((i >=`BANKS_RANGE_LO && i < `BANKS_RANGE_HI)) begin
    m_wr_req_to_endpoint[i] = m_sc_wr_req && (m_sbm_wr_req_addr[`SC_BANK_ADDR_R]==(i-`BANKS_RANGE_LO));
    m_rd_req_to_endpoint[i] = m_sc_rd_req && (m_sbm_rd_req_addr[`SC_BANK_ADDR_R]==(i-`BANKS_RANGE_LO));
    end
    else if (i==`BANKS_RANGE_HI) begin     //Shire other
    m_wr_req_to_endpoint[i] = m_shire_wr_req;
    m_rd_req_to_endpoint[i] = m_shire_rd_req;
    end
    else begin                              //RBOX
    m_wr_req_to_endpoint[i] = m_rbox_wr_req;
    m_rd_req_to_endpoint[i] = m_rbox_rd_req;
    end
   
    end //always_comb
    always @(posedge shire_clock) begin
    // Correct data chunk extraction
    if(m_wr_req_to_endpoint[i] && m_sbm_wr_req_valid && (m_dec_err_wr_req==0)) begin  
       for(int j = fifo_depth - 1; j > 0; j=j-1) begin
          m_wr_req_to_endpoint_addr[j][i] = m_wr_req_to_endpoint_addr[j-1][i];
          m_wr_req_to_endpoint_data[j][i] = m_wr_req_to_endpoint_data[j-1][i];
       end
       m_wr_req_to_endpoint_addr[0][i] = m_sbm_wr_req_addr;
       m_wr_req_to_endpoint_data[0][i] = m_apb_data;
       m_req_idx[i] = m_req_idx[i] + 1;
    end
    if ((m_wr_req_to_endpoint_addr[m_req_idx[i]-1][i]==`SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.apb_paddr) && `SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.apb_psel && `SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.apb_pwrite && (`SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.state==3'h1)) m_req_idx[i]=m_req_idx[i]-1;

    // Simultaneous read and write requests for neigh 
    if (m_wr_req_to_endpoint[i] && m_sbm_wr_req_valid && m_rd_req_to_endpoint[i] && m_sbm_rd_req_valid && (m_dec_err!=1) && (m_new_simult_req[i]==0)) begin
       m_wr_req_was_first[i] = 0;
       m_wr_req_to_endpoint_id[i] = `SBM_NOC_IFC.sbm_mesh_slave_axi_AW.ID;
       m_rd_req_to_endpoint_id[i] = `SBM_NOC_IFC.sbm_mesh_slave_axi_AR.ID;
       m_new_simult_req[i]  = 1;
       m_apb_rd_req_seen[i] = 0;
       m_apb_wr_req_seen[i] = 0;
    end
    if (m_new_simult_req[i] && (`SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.sbm_mesh_slave_axi_ARVALID || `SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.sbm_mesh_slave_axi_AWVALID)) begin
         if  ((m_apb_rd_req_seen[i]!=1) && m_apb_wr_req_seen[i]) begin
           m_apb_rd_req_seen[i] = ((m_rd_req_to_endpoint_id[i] == `SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.sbm_mesh_slave_axi_AR.ID) && `SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.sbm_mesh_slave_axi_ARVALID);
           if (m_apb_rd_req_seen[i]) begin
           m_new_simult_req[i] = 0;
           m_wr_req_was_first[i] = 1;
           end
         end
         if (m_apb_wr_req_seen[i]!=1) 
           m_apb_wr_req_seen[i] = ((m_wr_req_to_endpoint_id[i] == `SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.sbm_mesh_slave_axi_AW.ID) && `SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.sbm_mesh_slave_axi_AWVALID);
    end
    end //always
    // ----- SVA Simultaneous read and write requests to the same neigh -----
    `ASSERT_ALWAYS_MSG(sva_sbm_write_priority_over_read, shire_clock, reset_cold,
     ((m_new_simult_req[i] && m_wr_req_to_endpoint[i] && m_sbm_wr_req_valid && m_rd_req_to_endpoint[i] && m_sbm_rd_req_valid && (m_dec_err!=1)) |-> s_eventually m_wr_req_was_first[i]),$sformatf("Write request did not have priority over simultaneous read request targeting apb slave %d ", $sampled(i)));
    // SVA AXI to APB - Correct data chunk extraction
//if (NEIGH_STUB==0) begin
  `ASSERT_ALWAYS_MSG(sva_axi_and_apb_data_match, shire_clock, reset_cold,
      ( (m_wr_req_to_endpoint[i] && m_sbm_wr_req_valid && (m_dec_err_wr_req==0)) |-> s_eventually (`SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.apb_psel && `SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.apb_pwrite && (m_wr_req_to_endpoint_addr[m_req_idx[i]-1][i]==`SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.apb_paddr) && (m_wr_req_to_endpoint_data[m_req_idx[i]-1][i]==`SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.apb_pwdata) && (`SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.state==3'h1))),$sformatf("AXI to APB conversion - data mismatch for request targeting apb slave %d ", $sampled(i)));
//end
//else if ((NEIGH_STUB == 1) && (i>=`BANKS_RANGE_LO)) begin
//  `ASSERT_ALWAYS_MSG(sva_axi_and_apb_data_match, shire_clock, reset_cold,
//      ( (m_wr_req_to_endpoint[i] && m_sbm_wr_req_valid && (m_dec_err_wr_req==0)) |-> s_eventually (`SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.apb_psel && `SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.apb_pwrite && (m_wr_req_to_endpoint_addr[m_req_idx[i]-1][i]==`SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.apb_paddr) && (m_wr_req_to_endpoint_data[m_req_idx[i]-1][i]==`SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.apb_pwdata) && (`SBM_NOC_IFC.gen_sbm_axi_to_apb[i].axi_to_apb.state==3'h1))),$sformatf("AXI to APB conversion - data mismatch for request targeting apb slave %d ", $sampled(i)));
//end

end //for

// *************** Assertions that don't need for loop  *************

// ----- SVA Req to endpoint in reset should return an error -----
`ASSERT_ALWAYS_MSG(sva_neigh_in_reset_wr_req_cause_error, shire_clock, reset_cold,
     (m_wr_req_to_neigh_in_reset && m_sbm_wr_req_valid && m_sbm_wr_req_valid |=> `SBM_NOC_IFC.error_return[1]), "Write request to neigh in reset did not return decode error response"); 
`ASSERT_ALWAYS_MSG(sva_neigh_in_reset_rd_req_cause_error, shire_clock, reset_cold,
     (m_rd_req_to_neigh_in_reset && m_sbm_rd_req_valid && m_sbm_rd_req_valid |=> `SBM_NOC_IFC.error_return[0]), "Read request to neigh in reset did not return decode error response");
`ASSERT_ALWAYS_MSG(sva_sc_in_reset_wr_req_cause_error, shire_clock, reset_cold,
     (m_wr_req_to_sc_in_reset && m_sbm_wr_req_valid && m_sbm_wr_req_valid |=> `SBM_NOC_IFC.error_return[1]), "Write request to shire cache in reset did not return decode error response"); 
`ASSERT_ALWAYS_MSG(sva_sc_in_reset_rd_req_cause_error, shire_clock, reset_cold,
     (m_rd_req_to_sc_in_reset && m_sbm_rd_req_valid && m_sbm_rd_req_valid |=> `SBM_NOC_IFC.error_return[0]), "Read request to shire cache in reset did not return decode error response");
`ASSERT_ALWAYS_MSG(sva_rbox_in_reset_wr_req_cause_error, shire_clock, reset_cold,
     (m_wr_req_to_rbox_in_reset && m_sbm_wr_req_valid && m_sbm_wr_req_valid |=> `SBM_NOC_IFC.error_return[1]), "Write request to rbox in reset did not return decode error response"); 
`ASSERT_ALWAYS_MSG(sva_rbox_in_reset_rd_req_cause_error, shire_clock, reset_cold,
     (m_rd_req_to_rbox_in_reset && m_sbm_rd_req_valid && m_sbm_rd_req_valid |=> `SBM_NOC_IFC.error_return[0]), "Read request to rbox in reset did not return decode error response");

// ----- SVA BDC req have priority over other reqests stored in REQ FIFO -----
// BDC req and REQ FIFO is not empty and does not pop, BDC starts at the same time for all destinations
`ASSERT_ALWAYS_MSG(sva_fifo_not_empty_neigh_bdc_req, shire_clock, reset_cold,
     (m_neigh_bdc_req && m_sbm_wr_req_valid && |(`SBM_NOC_IFC.sbm_fifo_req_not_empty[`NEIGHS_RANGE]) |-> (|(`SBM_NOC_IFC.sbm_fifo_req_pop[`NEIGHS_RANGE])==0)), "Neigh broadcase request did not have priority over other requests");

`ASSERT_ALWAYS_MSG(sva_fifo_not_empty_sc_bdc_req, shire_clock, reset_cold,
     (m_sc_bdc_req && m_sbm_wr_req_valid && |(`SBM_NOC_IFC.sbm_fifo_req_not_empty[`BANKS_RANGE]) |-> (|(`SBM_NOC_IFC.sbm_fifo_req_pop[`BANKS_RANGE])==0)), "Shire Cache broadcase request did not have priority over other requests");

// ----- SVA Decode error causes access fault -----
`ASSERT_ALWAYS_MSG(sva_sbm_decode_error_causes_access_fault, shire_clock, reset_cold,
     (m_dec_err |=> |(`SBM_NOC_IFC.error_return)), "Decode error was not caught");

// ----- SVA Decode error rsp has priority over other responses
// It goes around fifo (rsp fifo does not pop)
`ASSERT_ALWAYS_MSG(sva_rd_dec_error_rd_rsp_fifo_doesnot_pop, shire_clock, reset_cold,
     ((`SBM_NOC_IFC.error_return[0]) |-> !(`SBM_NOC_IFC.readRsp_fifo_pop)), "Read request decode error response did not have priority over regular read response");
`ASSERT_ALWAYS_MSG(sva_wr_dec_error_wr_rsp_fifo_doesnot_pop, shire_clock, reset_cold,
     ((`SBM_NOC_IFC.error_return[1]) |-> !(`SBM_NOC_IFC.writeRsp_fifo_pop)), "Write request decode error response did not have priority over regular write response");

// ----- SVA No more than one read and write responses were accepted
`ASSERT_ALWAYS_MSG(sva_only_one_write_resp_can_be_accepted, shire_clock, reset_cold,
     ($countones(`SBM_NOC_IFC.mesh_slave_axi_BREADY) <= 1), "More than one write response accepted at a time");
`ASSERT_ALWAYS_MSG(sva_only_one_read_resp_can_be_accepted, shire_clock, reset_cold,
     ($countones(`SBM_NOC_IFC.mesh_slave_axi_RREADY) <= 1), "More than one read response accepted at a time");
// ------------ STH ELSE

    for (sbm_port = `NEIGHS_RANGE_LO; sbm_port < `NEIGHS_RANGE_HI; sbm_port++) begin : sva_sbm_apb_to_axi_write_error_response_neigh

    `ASSERT_ALWAYS_MSG(sva_sbm_apb_to_axi_write_error_response_neigh, shire_clock, reset_cold,
         ((`SBM_NOC_IFC.gen_sbm_axi_to_apb[sbm_port].axi_to_apb.apb_pslverr && `SBM_NOC_IFC.gen_sbm_axi_to_apb[sbm_port].axi_to_apb.apb_penable && `SBM_NOC_IFC.gen_sbm_axi_to_apb[sbm_port].axi_to_apb.apb_pready && `SBM_NOC_IFC.gen_sbm_axi_to_apb[sbm_port].axi_to_apb.apb_pwrite && (`SBM_NOC_IFC.gen_sbm_axi_to_apb[sbm_port].axi_to_apb.apb_paddr[`NEIGH_ADDR_R] != decode_bdc_req))
         |-> s_eventually ((`SBM_NOC_IFC.sbm_mesh_slave_axi_B.RESP == 2'b10) && `SBM_NOC_IFC.sbm_mesh_slave_axi_BVALID && (`SBM_NOC_IFC.bcast_response != 1))),
         $sformatf("AXI write error response coming from apb slave %d (NEIGH) was not seen", $sampled(sbm_port))); //BDC write requests excluded

    end
    for (sbm_port=`BANKS_RANGE_LO; sbm_port < `BANKS_RANGE_HI; sbm_port++) begin : sva_sbm_apb_to_axi_write_error_response_sc_banks

    `ASSERT_ALWAYS_MSG(sva_sbm_apb_to_axi_write_error_response_scbanks, shire_clock, reset_cold,
         ((`SBM_NOC_IFC.gen_sbm_axi_to_apb[sbm_port].axi_to_apb.apb_pslverr && `SBM_NOC_IFC.gen_sbm_axi_to_apb[sbm_port].axi_to_apb.apb_penable && `SBM_NOC_IFC.gen_sbm_axi_to_apb[sbm_port].axi_to_apb.apb_pready && `SBM_NOC_IFC.gen_sbm_axi_to_apb[sbm_port].axi_to_apb.apb_pwrite && (`SBM_NOC_IFC.gen_sbm_axi_to_apb[sbm_port].axi_to_apb.apb_paddr[`SC_BANK_ADDR_R] != decode_bdc_req))
         |-> s_eventually ((`SBM_NOC_IFC.sbm_mesh_slave_axi_B.RESP == 2'b10) && `SBM_NOC_IFC.sbm_mesh_slave_axi_BVALID && (`SBM_NOC_IFC.bcast_response != 1))),
         $sformatf("AXI write error response coming from apb slave %d (SC_BANK) was not seen", $sampled(sbm_port))); //BDC write requests excluded
    end
    for (sbm_port=`BANKS_RANGE_HI; sbm_port < `SBM_APB_INTERFACES; sbm_port++) begin : sva_sbm_apb_to_axi_write_error_response

    `ASSERT_ALWAYS_MSG(sva_sbm_apb_to_axi_write_error_response, shire_clock, reset_cold,
         ((`SBM_NOC_IFC.gen_sbm_axi_to_apb[sbm_port].axi_to_apb.apb_pslverr && `SBM_NOC_IFC.gen_sbm_axi_to_apb[sbm_port].axi_to_apb.apb_penable && `SBM_NOC_IFC.gen_sbm_axi_to_apb[sbm_port].axi_to_apb.apb_pready && `SBM_NOC_IFC.gen_sbm_axi_to_apb[sbm_port].axi_to_apb.apb_pwrite)
         |-> s_eventually ((`SBM_NOC_IFC.sbm_mesh_slave_axi_B.RESP == 2'b10) && `SBM_NOC_IFC.sbm_mesh_slave_axi_BVALID && (`SBM_NOC_IFC.bcast_response != 1))),
         $sformatf("AXI write error response coming from apb slave %d was not seen", $sampled(sbm_port)));
   end

    for (sbm_port=0; sbm_port < `SBM_APB_INTERFACES; sbm_port++) begin : sva_sbm_apb_to_axi_read_error_response
    `ASSERT_ALWAYS_MSG(sva_sbm_apb_to_axi_read_error_response, shire_clock, reset_cold,
         ((`SBM_NOC_IFC.gen_sbm_axi_to_apb[sbm_port].axi_to_apb.apb_pslverr && `SBM_NOC_IFC.gen_sbm_axi_to_apb[sbm_port].axi_to_apb.apb_penable && `SBM_NOC_IFC.gen_sbm_axi_to_apb[sbm_port].axi_to_apb.apb_pready && (`SBM_NOC_IFC.gen_sbm_axi_to_apb[sbm_port].axi_to_apb.apb_pwrite == 0)) 
          |-> s_eventually ((`SBM_NOC_IFC.sbm_mesh_slave_axi_R.RESP == 2'b10) && `SBM_NOC_IFC.sbm_mesh_slave_axi_RVALID)),
          $sformatf("AXI read error response coming from apb slave %d was not seen", $sampled(sbm_port)));
    end

// ----- SVA Req fifo should never overflow -----
// sbm_buffer_count > 14 and simultaneous rw request and no pop/s
`ASSERT_ALWAYS_MSG(sva_fifo_should_not_overflow_simult_req, shire_clock, reset_cold,
     (  !(  ($past(`SBM_NOC_IFC.sbm_buffer_count,1) > `SBM_CREDIT_NUMBER-2) && (m_sbm_wr_req_valid & m_sbm_wr_req_ready & m_sbm_rd_req_valid & m_sbm_rd_req_ready) && (m_dec_err == 0) && !($countones(`SBM_NOC_IFC.sbm_fifo_req_pop) >= (2-(m_simult_same_endpoint + m_bdc_req + $countones(`SBM_NOC_IFC.write_req_valid) + $countones(`SBM_NOC_IFC.read_req_valid) )+$past(`SBM_NOC_IFC.sbm_buffer_count,1)-`SBM_CREDIT_NUMBER)) )==1  ), "REQ FIFO overflows with simultaneous read and write requests");

// sbm_buffer_count > 15 and read or write request and no pop/s
`ASSERT_ALWAYS_MSG(sva_fifo_should_not_overflow, shire_clock, reset_cold,
     (  !(  ($past(`SBM_NOC_IFC.sbm_buffer_count,1) > `SBM_CREDIT_NUMBER-1) && ((m_sbm_wr_req_valid & m_sbm_wr_req_ready & (m_dec_err_wr_req===0)) | (m_sbm_rd_req_valid & m_sbm_rd_req_ready & (m_dec_err_rd_req===0) )) && !($countones(`SBM_NOC_IFC.sbm_fifo_req_pop) >= (1-(m_bdc_req+$countones(`SBM_NOC_IFC.write_req_valid) + $countones(`SBM_NOC_IFC.read_req_valid) )+$past(`SBM_NOC_IFC.sbm_buffer_count,1)-`SBM_CREDIT_NUMBER)) )==1  ), "REQ FIFO overflows with read or write request");

// ----- SVA Req fifo should never underflow -----
`ASSERT_ALWAYS_MSG(sva_fifo_should_not_underflow, shire_clock, reset_cold,
     ( ($past(`SBM_NOC_IFC.sbm_buffer_count,1) + $countones(`SBM_NOC_IFC.sbm_fifo_req_push)) >= $countones(`SBM_NOC_IFC.sbm_fifo_req_pop) ), "REQ FIFO underflow");


end //if
endgenerate




