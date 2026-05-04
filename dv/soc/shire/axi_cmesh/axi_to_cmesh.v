`include "soc.vh"

// This module contains the Conversion of L2/UC structs to AXI, and the instantiation of the V-cross FIFOs
//
// - req_cmd_fifo and req_data_fifo allows seperate handshake for AW/W and reduced power for non-writes
// - Conversion of structs to AXI signalling
// - Push into correct channels
// - B & R response arbiter/merging.
// - B & R AXI to struct conversion
module axi_to_cmesh 
  (
   // system
   input logic 			  clock,
   input logic 			  reset,
   input logic [`NUM_SHIRE_IDS_R] shire_id,

   // Cacheable
   
   // L2 AXI Write Address (AW) channel
   input 			  sc_master_axi_AW_channel_t mesh_master_axi_AW,
   input logic 			  mesh_master_axi_AWVALID,
   output logic 		  mesh_master_axi_AWREADY,
  
   // L2 AXI Write (W) channel
   input 			  sc_master_axi_W_channel_t mesh_master_axi_W,
   input logic 			  mesh_master_axi_WVALID,
   output logic 		  mesh_master_axi_WREADY,
  
   // L2 AXI Read Address (AR) channel
   input 			  sc_master_axi_AR_channel_t mesh_master_axi_AR,
   input logic 			  mesh_master_axi_ARVALID,
   output logic 		  mesh_master_axi_ARREADY,
  
   // L2 AXI Read Response (R) channel
   output 			  sc_master_axi_R_channel_t mesh_master_axi_R,
   output logic 		  mesh_master_axi_RVALID,
   input logic 			  mesh_master_axi_RREADY,

   // L2 AXI Write Response (B) channel
   output 			  sc_master_axi_B_channel_t mesh_master_axi_B,
   output logic 		  mesh_master_axi_BVALID,
   input logic 			  mesh_master_axi_BREADY,
  
   // Non-cacheable (sys)
   
   // L2 AXI Write Address (AW) channel
   input 			  sc_master_axi_AW_channel_t uc_to_sys_mesh_master_axi_AW,
   input logic 			  uc_to_sys_mesh_master_axi_AWVALID,
   output logic 		  uc_to_sys_mesh_master_axi_AWREADY,
  
   // L2 AXI Write (W) channel
   input 			  sc_master_axi_W_channel_t uc_to_sys_mesh_master_axi_W,
   input logic 			  uc_to_sys_mesh_master_axi_WVALID,
   output logic 		  uc_to_sys_mesh_master_axi_WREADY,
  
   // L2 AXI Read Address (AR) channel
   input 			  sc_master_axi_AR_channel_t uc_to_sys_mesh_master_axi_AR,
   input logic 			  uc_to_sys_mesh_master_axi_ARVALID,
   output logic 		  uc_to_sys_mesh_master_axi_ARREADY,
  
   // L2 AXI Read Response (R) channel
   output 			  sc_master_axi_R_channel_t uc_to_sys_mesh_master_axi_R,
   output logic 		  uc_to_sys_mesh_master_axi_RVALID,
   input logic 			  uc_to_sys_mesh_master_axi_RREADY,

   // L2 AXI Write Response (B) channel
   output 			  sc_master_axi_B_channel_t uc_to_sys_mesh_master_axi_B,
   output logic 		  uc_to_sys_mesh_master_axi_BVALID,
   input logic 			  uc_to_sys_mesh_master_axi_BREADY,
  
   // Non-cacheable (l3)
   
   // L2 AXI Write Address (AW) channel
   input 			  sc_master_axi_AW_channel_t uc_to_l3_mesh_master_axi_AW,
   input logic 			  uc_to_l3_mesh_master_axi_AWVALID,
   output logic 		  uc_to_l3_mesh_master_axi_AWREADY,
  
   // L2 AXI Write (W) channel
   input 			  sc_master_axi_W_channel_t uc_to_l3_mesh_master_axi_W,
   input logic 			  uc_to_l3_mesh_master_axi_WVALID,
   output logic 		  uc_to_l3_mesh_master_axi_WREADY,
  
   // L2 AXI Read Address (AR) channel
   input 			  sc_master_axi_AR_channel_t uc_to_l3_mesh_master_axi_AR,
   input logic 			  uc_to_l3_mesh_master_axi_ARVALID,
   output logic 		  uc_to_l3_mesh_master_axi_ARREADY,
  
   // L2 AXI Read Response (R) channel
   output 			  sc_master_axi_R_channel_t uc_to_l3_mesh_master_axi_R,
   output logic 		  uc_to_l3_mesh_master_axi_RVALID,
   input logic 			  uc_to_l3_mesh_master_axi_RREADY,

   // L2 AXI Write Response (B) channel
   output 			  sc_master_axi_B_channel_t uc_to_l3_mesh_master_axi_B,
   output logic 		  uc_to_l3_mesh_master_axi_BVALID,
   input logic 			  uc_to_l3_mesh_master_axi_BREADY,

   // Sys
   
   // L2 AXI Write Address (AW) channel
   input 			  sc_master_axi_AW_channel_t to_sys_mesh_master_axi_AW,
   input logic 			  to_sys_mesh_master_axi_AWVALID,
   output logic 		  to_sys_mesh_master_axi_AWREADY,
  
   // L2 AXI Write (W) channel
   input 			  sc_master_axi_W_channel_t to_sys_mesh_master_axi_W,
   input logic 			  to_sys_mesh_master_axi_WVALID,
   output logic 		  to_sys_mesh_master_axi_WREADY,
  
   // L2 AXI Read Address (AR) channel
   input 			  sc_master_axi_AR_channel_t to_sys_mesh_master_axi_AR,
   input logic 			  to_sys_mesh_master_axi_ARVALID,
   output logic 		  to_sys_mesh_master_axi_ARREADY,
  
   // L2 AXI Read Response (R) channel
   output 			  sc_master_axi_R_channel_t to_sys_mesh_master_axi_R,
   output logic 		  to_sys_mesh_master_axi_RVALID,
   input logic 			  to_sys_mesh_master_axi_RREADY,

   // L2 AXI Write Response (B) channel
   output 			  sc_master_axi_B_channel_t to_sys_mesh_master_axi_B,
   output logic 		  to_sys_mesh_master_axi_BVALID,
   input logic 			  to_sys_mesh_master_axi_BREADY,

   // MESH
   
   // mesh cacheable request interface
   output 			  mesh_request_t mesh_req,
   output logic 		  mesh_req_valid,
   input logic 			  mesh_req_ready,
  
   // mesh cacheable reply interface
   input 			  mesh_reply_t mesh_rep,
   input logic 			  mesh_rep_valid,
   output logic 		  mesh_rep_ready,
  
   // mesh non-cacheable request interface
   output 			  mesh_request_t uc_mesh_req,
   output logic 		  uc_mesh_req_valid,
   input logic 			  uc_mesh_req_ready,
  
   // mesh non-cacheable reply interface
   input 			  mesh_reply_t uc_mesh_rep,
   input logic 			  uc_mesh_rep_valid,
   output logic 		  uc_mesh_rep_ready,
   
   // mesh non-cacheable request interface
   output 			  mesh_request_t sys_mesh_req,
   output logic 		  sys_mesh_req_valid,
   input logic 			  sys_mesh_req_ready,
  
   // mesh non-cacheable reply interface
   input 			  mesh_reply_t sys_mesh_rep,
   input logic 			  sys_mesh_rep_valid,
   output logic 		  sys_mesh_rep_ready
   );

   assign uc_to_l3_mesh_master_axi_ARREADY = 1'b1;
   assign uc_to_l3_mesh_master_axi_AWREADY = 1'b1;
   assign uc_to_l3_mesh_master_axi_WREADY  = 1'b1;
   assign uc_to_l3_mesh_master_axi_BVALID  = 1'b0;
   assign uc_to_l3_mesh_master_axi_RVALID  = 1'b0;

   assign to_sys_mesh_master_axi_ARREADY = 1'b1;
   assign to_sys_mesh_master_axi_AWREADY = 1'b1;
   assign to_sys_mesh_master_axi_WREADY  = 1'b1;
   assign to_sys_mesh_master_axi_BVALID  = 1'b0;
   assign to_sys_mesh_master_axi_RVALID  = 1'b0;

   `ASSERT_VALID_PORTS(mesh_master_axi_ARID,     clock, reset, mesh_master_axi_ARVALID, mesh_master_axi_AR.ID)
   `ASSERT_VALID_PORTS(mesh_master_axi_ARADDR,   clock, reset, mesh_master_axi_ARVALID, mesh_master_axi_AR.ADDR)
   `ASSERT_VALID_PORTS(mesh_master_axi_ARLEN,    clock, reset, mesh_master_axi_ARVALID, mesh_master_axi_AR.LEN)
   `ASSERT_VALID_PORTS(mesh_master_axi_ARSIZE,   clock, reset, mesh_master_axi_ARVALID, mesh_master_axi_AR.SIZE)
   `ASSERT_VALID_PORTS(mesh_master_axi_ARBURST,  clock, reset, mesh_master_axi_ARVALID, mesh_master_axi_AR.BURST)
   `ASSERT_VALID_PORTS(mesh_master_axi_ARLOCK,   clock, reset, mesh_master_axi_ARVALID, mesh_master_axi_AR.LOCK)
   `ASSERT_VALID_PORTS(mesh_master_axi_ARCACHE,  clock, reset, mesh_master_axi_ARVALID, mesh_master_axi_AR.CACHE)
   `ASSERT_VALID_PORTS(mesh_master_axi_ARPROT,   clock, reset, mesh_master_axi_ARVALID, mesh_master_axi_AR.PROT)
   `ASSERT_VALID_PORTS(mesh_master_axi_ARQOS,    clock, reset, mesh_master_axi_ARVALID, mesh_master_axi_AR.QOS)
   `ASSERT_VALID_PORTS(mesh_master_axi_ARUSER,   clock, reset, mesh_master_axi_ARVALID, mesh_master_axi_AR.USER)

   `ASSERT_VALID_PORTS(mesh_master_axi_AWID,     clock, reset, mesh_master_axi_AWVALID, mesh_master_axi_AW.ID)
   `ASSERT_VALID_PORTS(mesh_master_axi_AWADDR,   clock, reset, mesh_master_axi_AWVALID, mesh_master_axi_AW.ADDR)
   `ASSERT_VALID_PORTS(mesh_master_axi_AWLEN,    clock, reset, mesh_master_axi_AWVALID, mesh_master_axi_AW.LEN)
   `ASSERT_VALID_PORTS(mesh_master_axi_AWSIZE,   clock, reset, mesh_master_axi_AWVALID, mesh_master_axi_AW.SIZE)
   `ASSERT_VALID_PORTS(mesh_master_axi_AWBURST,  clock, reset, mesh_master_axi_AWVALID, mesh_master_axi_AW.BURST)
   `ASSERT_VALID_PORTS(mesh_master_axi_AWLOCK,   clock, reset, mesh_master_axi_AWVALID, mesh_master_axi_AW.LOCK)
   `ASSERT_VALID_PORTS(mesh_master_axi_AWCACHE,  clock, reset, mesh_master_axi_AWVALID, mesh_master_axi_AW.CACHE)
   `ASSERT_VALID_PORTS(mesh_master_axi_AWPROT,   clock, reset, mesh_master_axi_AWVALID, mesh_master_axi_AW.PROT)
   `ASSERT_VALID_PORTS(mesh_master_axi_AWQOS,    clock, reset, mesh_master_axi_AWVALID, mesh_master_axi_AW.QOS)
   `ASSERT_VALID_PORTS(mesh_master_axi_AWUSER,   clock, reset, mesh_master_axi_AWVALID, mesh_master_axi_AW.USER)

   //`ASSERT_VALID_PORTS(mesh_master_axi_WDATA,    clock, reset, mesh_master_axi_WVALID, mesh_master_axi_WDATA)
   `ASSERT_VALID_PORTS(mesh_master_axi_WSTRB,    clock, reset, mesh_master_axi_WVALID, mesh_master_axi_W.STRB)
   `ASSERT_VALID_PORTS(mesh_master_axi_WLAST,    clock, reset, mesh_master_axi_WVALID, mesh_master_axi_W.LAST)

   `ASSERT_VALID_PORTS(mesh_master_axi_BID,    clock, reset, mesh_master_axi_BVALID,  mesh_master_axi_B.ID)
   `ASSERT_VALID_PORTS(mesh_master_axi_BRESP,  clock, reset, mesh_master_axi_BVALID,  mesh_master_axi_B.RESP)

   `ASSERT_VALID_PORTS(mesh_master_axi_RID,    clock, reset, mesh_master_axi_RVALID,  mesh_master_axi_R.ID)
   `ASSERT_VALID_PORTS(mesh_master_axi_RRESP,  clock, reset, mesh_master_axi_RVALID,  mesh_master_axi_R.RESP)
   `ASSERT_VALID_PORTS(mesh_master_axi_RLAST,  clock, reset, mesh_master_axi_RVALID,  mesh_master_axi_R.LAST)
   //`ASSERT_VALID_PORTS(mesh_master_axi_RDATA,  clock, reset, mesh_master_axi_RVALID,    mesh_master_axi_R.DATA)



   l2_mesh_request_t                       l2_mesh_req;
   logic 			  	   sel_ar;
   logic 			  	   sel_r;
   logic [`SC_MESH_MASTER_AXI_ID_SIZE-1:0] axi_rep_id;
   logic [3:0] 				   l2_mesh_qwen;

   l2_mesh_request_t                       uc__mesh_req;
   logic 			           uc_sel_ar;
   logic 			  	   uc_sel_r;
   logic [`SC_MESH_MASTER_AXI_ID_SIZE-1:0] uc_axi_rep_id;
   
   ////////////////////////////////////////
   // Cacheable requests to mesh
   ////////////////////////////////////////

   // Interface to mesh
   assign mesh_req_valid                 = mesh_master_axi_ARVALID | (mesh_master_axi_AWVALID & mesh_master_axi_WVALID);
   assign mesh_req.l2_bank               = mesh_master_axi_AR.ID[`SC_MESH_MASTER_AXI_ID_SIZE-2:`SC_MESH_MASTER_SOURCE_SIZE];
   assign mesh_req.shire_id              = shire_id;
   assign mesh_req.info                  = l2_mesh_req;

   assign mesh_master_axi_ARREADY        = sel_ar & mesh_req_ready;
   assign mesh_master_axi_AWREADY 	 = mesh_master_axi_AWVALID & mesh_master_axi_WVALID & ~sel_ar & mesh_req_ready; // wait for both data and 
   assign mesh_master_axi_WREADY  	 = mesh_master_axi_AWREADY;
   
   // create request info for mesh_req.info above
   assign sel_ar                         = mesh_master_axi_ARVALID;
   assign l2_mesh_req.id                 = sel_ar ? {1'b1, mesh_master_axi_AR.ID} : {1'b0, mesh_master_axi_AW.ID};
   assign l2_mesh_req.request.rnw        = sel_ar;
   assign l2_mesh_req.request.wa         = ~sel_ar & (mesh_master_axi_W.STRB != 64'hffff_ffff_ffff_ffff);

   assign l2_mesh_qwen                   = l2_mesh_req.request.wa  ?
                                           {mesh_master_axi_W.STRB[48],mesh_master_axi_W.STRB[32],mesh_master_axi_W.STRB[16],mesh_master_axi_W.STRB[0]}
                                           : 4'h0;

   assign l2_mesh_req.request.address    = sel_ar ? mesh_master_axi_AR.ADDR : (mesh_master_axi_AW.ADDR | {{`PA_SIZE-4{1'b0}},l2_mesh_qwen});
   assign l2_mesh_req.request.data       = mesh_master_axi_W.DATA;

   ////////////////////////////////////////
   // Cacheable responses from mesh
   ////////////////////////////////////////

   assign {sel_r, axi_rep_id}        = mesh_rep.info.id;

   assign mesh_master_axi_RVALID     = sel_r & mesh_rep_valid; 
   assign mesh_master_axi_R.DATA     = mesh_rep.info.data;
   assign mesh_master_axi_R.ID       = axi_rep_id;
   assign mesh_master_axi_R.RESP     = 2'b00; // OK
   assign mesh_master_axi_R.LAST     = 1'b1; 

   assign mesh_master_axi_BVALID     = !sel_r & mesh_rep_valid; 
   assign mesh_master_axi_B.RESP     = 2'b00; // OK
   assign mesh_master_axi_B.ID       = axi_rep_id;

   assign mesh_rep_ready             =  sel_r ? mesh_master_axi_RREADY : mesh_master_axi_BREADY;

   ////////////////////////////////////////
   // Non-cacheable requests to mesh
   ////////////////////////////////////////

   assign uc_mesh_req_valid              = uc_to_sys_mesh_master_axi_ARVALID | (uc_to_sys_mesh_master_axi_AWVALID & uc_to_sys_mesh_master_axi_WVALID);
   assign uc_mesh_req.l2_bank            = uc_to_sys_mesh_master_axi_AR.ID[`SC_MESH_MASTER_AXI_ID_SIZE-2:`SC_MESH_MASTER_SOURCE_SIZE];
   assign uc_mesh_req.shire_id           = shire_id;
   assign uc_mesh_req.info               = uc__mesh_req;

   assign uc_to_sys_mesh_master_axi_ARREADY = 1'b1; //uc_sel_ar & uc_mesh_req_ready;
   assign uc_to_sys_mesh_master_axi_AWREADY = 1'b1; //uc_to_sys_mesh_master_axi_AWVALID & uc_to_sys_mesh_master_axi_WVALID & ~sel_ar & uc_mesh_req_ready; // wait for data & addr
   assign uc_to_sys_mesh_master_axi_WREADY  = 1'b1; //uc_to_sys_mesh_master_axi_AWREADY;
   
   // create request info for mesh_req.info above
   assign uc_sel_ar                    = uc_to_sys_mesh_master_axi_ARVALID;
   assign uc__mesh_req.id              = uc_sel_ar ? {1'b1, uc_to_sys_mesh_master_axi_AR.ID} : {1'b0, uc_to_sys_mesh_master_axi_AW.ID};
   assign uc__mesh_req.request.rnw     = uc_sel_ar;
   assign uc__mesh_req.request.wa      = ~uc_sel_ar & (uc_to_sys_mesh_master_axi_W.STRB != 64'hffff_ffff_ffff_ffff);

   assign uc__mesh_req.request.address = uc_sel_ar ? uc_to_sys_mesh_master_axi_AR.ADDR : uc_to_sys_mesh_master_axi_AW.ADDR;
   assign uc__mesh_req.request.data    = uc_to_sys_mesh_master_axi_W.DATA;

   ////////////////////////////////////////
   // Non-cacheable responses from mesh
   ////////////////////////////////////////

   assign {uc_sel_r, uc_axi_rep_id}  = uc_mesh_rep.info.id;

   assign uc_to_sys_mesh_master_axi_RVALID  = uc_sel_r & uc_mesh_rep_valid; 
   assign uc_to_sys_mesh_master_axi_R.DATA  = uc_mesh_rep.info.data;
   assign uc_to_sys_mesh_master_axi_R.ID    = uc_axi_rep_id;
   assign uc_to_sys_mesh_master_axi_R.RESP  = 2'b00; // OK
   assign uc_to_sys_mesh_master_axi_R.LAST  = 1'b1; 

   assign uc_to_sys_mesh_master_axi_BVALID  = !uc_sel_r & uc_mesh_rep_valid; 
   assign uc_to_sys_mesh_master_axi_B.RESP  = 2'b00; // OK
   assign uc_to_sys_mesh_master_axi_B.ID    = uc_axi_rep_id;

   assign uc_mesh_rep_ready          = uc_sel_r ? uc_to_sys_mesh_master_axi_RREADY : uc_to_sys_mesh_master_axi_BREADY;

   ////////////////////////////////////////
   // Sys
   ////////////////////////////////////////

   assign sys_mesh_req_valid              = '0;
   assign sys_mesh_req              = '0;
   
endmodule

// Local Variables:
// verilog-typedef-regexp: "\(input\|output\)([:space:]+logic)?[:space:]+[[:alnum:]_]+_t" 
// End:
