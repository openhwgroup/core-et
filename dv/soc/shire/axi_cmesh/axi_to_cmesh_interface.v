module axi_to_cmesh_interface (
      // system			       
      input logic 		     clock,
      input logic 		     reset,
      input logic [`NUM_SHIRE_IDS_R] shire_id,

      ////////////////////////////////////////
      // Shire Cacheable Interface
      ////////////////////////////////////////
      					   
      // Write address channel
      input logic 		     to_l3_mesh_master_axi_AWVALID,
      input 			     sc_master_axi_AW_channel_t to_l3_mesh_master_axi_AW,
      output logic 		     to_l3_mesh_master_axi_AWREADY,
			       
      // Write data channel
      input logic 		     to_l3_mesh_master_axi_WVALID,
      input 			     sc_master_axi_W_channel_t to_l3_mesh_master_axi_W,
      output logic 		     to_l3_mesh_master_axi_WREADY,
			       
      // Read address channel
      input logic 		     to_l3_mesh_master_axi_ARVALID,
      input 			     sc_master_axi_AR_channel_t to_l3_mesh_master_axi_AR,
      output logic 		     to_l3_mesh_master_axi_ARREADY,
			       
      // Read data channel
      output logic 		     to_l3_mesh_master_axi_RVALID,
      output 			     sc_master_axi_R_channel_t to_l3_mesh_master_axi_R,
      input logic 		     to_l3_mesh_master_axi_RREADY,

      // Write response channel
      output logic 		     to_l3_mesh_master_axi_BVALID,
      output 			     sc_master_axi_B_channel_t to_l3_mesh_master_axi_B,
      input logic 		     to_l3_mesh_master_axi_BREADY,

      ////////////////////////////////////////
      // Shire Non-cacheable Interface (sys)
      ////////////////////////////////////////
      					   
      // Write address channel
      input logic 		     uc_to_sys_mesh_master_axi_AWVALID,
      input 			     sc_master_axi_AW_channel_t uc_to_sys_mesh_master_axi_AW,
      output logic 		     uc_to_sys_mesh_master_axi_AWREADY,
			       
      // Write data channel
      input logic 		     uc_to_sys_mesh_master_axi_WVALID,
      input 			     sc_master_axi_W_channel_t uc_to_sys_mesh_master_axi_W,
      output logic 		     uc_to_sys_mesh_master_axi_WREADY,
			       
      // Read address channel
      input logic 		     uc_to_sys_mesh_master_axi_ARVALID,
      input 			     sc_master_axi_AR_channel_t uc_to_sys_mesh_master_axi_AR,
      output logic 		     uc_to_sys_mesh_master_axi_ARREADY,
			       
      // Read data channel
      output logic 		     uc_to_sys_mesh_master_axi_RVALID,
      output 			     sc_master_axi_R_channel_t uc_to_sys_mesh_master_axi_R,
      input logic 		     uc_to_sys_mesh_master_axi_RREADY,

      // Write response channel
      output logic 		     uc_to_sys_mesh_master_axi_BVALID,
      output 			     sc_master_axi_B_channel_t uc_to_sys_mesh_master_axi_B,
      input logic 		     uc_to_sys_mesh_master_axi_BREADY,

      ////////////////////////////////////////
      // Shire Non-cacheable Interface (l3)
      ////////////////////////////////////////
      					   
      // Write address channel
      input logic 		     uc_to_l3_mesh_master_axi_AWVALID,
      input 			     sc_master_axi_AW_channel_t uc_to_l3_mesh_master_axi_AW,
      output logic 		     uc_to_l3_mesh_master_axi_AWREADY,
			       
      // Write data channel
      input logic 		     uc_to_l3_mesh_master_axi_WVALID,
      input 			     sc_master_axi_W_channel_t uc_to_l3_mesh_master_axi_W,
      output logic 		     uc_to_l3_mesh_master_axi_WREADY,
			       
      // Read address channel
      input logic 		     uc_to_l3_mesh_master_axi_ARVALID,
      input 			     sc_master_axi_AR_channel_t uc_to_l3_mesh_master_axi_AR,
      output logic 		     uc_to_l3_mesh_master_axi_ARREADY,
			       
      // Read data channel
      output logic 		     uc_to_l3_mesh_master_axi_RVALID,
      output 			     sc_master_axi_R_channel_t uc_to_l3_mesh_master_axi_R,
      input logic 		     uc_to_l3_mesh_master_axi_RREADY,

      // Write response channel
      output logic 		     uc_to_l3_mesh_master_axi_BVALID,
      output 			     sc_master_axi_B_channel_t uc_to_l3_mesh_master_axi_B,
      input logic 		     uc_to_l3_mesh_master_axi_BREADY,

      ////////////////////////////////////////
      // Shire Sys
      ////////////////////////////////////////
      					   
      // Write address channel
      input logic 		     to_sys_mesh_master_axi_AWVALID,
      input 			     sc_master_axi_AW_channel_t to_sys_mesh_master_axi_AW,
      output logic 		     to_sys_mesh_master_axi_AWREADY,
			       
      // Write data channel
      input logic 		     to_sys_mesh_master_axi_WVALID,
      input 			     sc_master_axi_W_channel_t to_sys_mesh_master_axi_W,
      output logic 		     to_sys_mesh_master_axi_WREADY,
			       
      // Read address channel
      input logic 		     to_sys_mesh_master_axi_ARVALID,
      input 			     sc_master_axi_AR_channel_t to_sys_mesh_master_axi_AR,
      output logic 		     to_sys_mesh_master_axi_ARREADY,
			       
      // Read data channel
      output logic 		     to_sys_mesh_master_axi_RVALID,
      output 			     sc_master_axi_R_channel_t to_sys_mesh_master_axi_R,
      input logic 		     to_sys_mesh_master_axi_RREADY,

      // Write response channel
      output logic 		     to_sys_mesh_master_axi_BVALID,
      output 			     sc_master_axi_B_channel_t to_sys_mesh_master_axi_B,
      input logic 		     to_sys_mesh_master_axi_BREADY,

      ////////////////////////////////////////
      // Minion-sim C++ NoC Interface			       
      ////////////////////////////////////////
			       
      // Cacheable requests
      output logic 		     mesh_req_valid,
      output 			     mesh_request_t mesh_req,
      input logic 		     mesh_req_ready,

      // Replies to cacheable requests					   
      input logic 		     mesh_rep_valid,
      input 			     mesh_reply_t mesh_rep,
      output logic 		     mesh_rep_ready,
			       
      // Non-cacheable requests
      output logic 		     uc_mesh_req_valid,
      output 			     mesh_request_t uc_mesh_req,
      input logic 		     uc_mesh_req_ready,

      // Replies to non-cacheable requests					   
      input logic 		     uc_mesh_rep_valid,
      input 			     mesh_reply_t uc_mesh_rep,
      output logic 		     uc_mesh_rep_ready,
			       
      // Non-cacheable requests
      output logic 		     sys_mesh_req_valid,
      output 			     mesh_request_t sys_mesh_req,
      input logic 		     sys_mesh_req_ready,

      // Replies to non-cacheable requests					   
      input logic 		     sys_mesh_rep_valid,
      input 			     mesh_reply_t sys_mesh_rep,
      output logic 		     sys_mesh_rep_ready

//////////			       
      // Non-cacheable requests
      			       
      //output logic 		     mesh_uc_req_valid,
      //input 			     mesh_request_t ext_uc_mesh_req,
      //input logic 		     mesh_uc_req_ready,
      //			       
      //input logic 		     ext_uc_mesh_req_valid,
      //output 			     et_link_req_info_t mesh_uc_req_info,
      //output logic 		     ext_uc_mesh_req_ready,
   );

   // mux mesh replies to either L2 or local uc blocks
   logic          mesh_rep_l2_en;
   logic          mesh_req_l2_en;

   // l2 mesh reqs
   mesh_request_t l2_mesh_req;
   logic          l2_mesh_req_valid;
   logic          l2_mesh_req_ready;

   // and replies
   logic          mesh_rep_l2_valid;
   logic          mesh_rep_l2_ready;

   //mesh_request_t uc_mesh_req;
   //logic          uc_mesh_req_valid;
   //logic          uc_mesh_req_ready;

   // and replies
   //logic          uc_mesh_rep_valid;
   //logic          uc_mesh_rep_ready;

   axi_to_cmesh axi_to_cmesh (
	 // system		       
         .clock				( clock				),
         .reset				( reset				),
         .shire_id			( shire_id			),
	
	 ////////////////////////////////////////
	 // Shire Cacheable Interface
	 ////////////////////////////////////////
		
         // Write address channel			      
         .mesh_master_axi_AWVALID	( to_l3_mesh_master_axi_AWVALID ),
         .mesh_master_axi_AW		( to_l3_mesh_master_axi_AW	),
         .mesh_master_axi_AWREADY	( to_l3_mesh_master_axi_AWREADY ),
			      
         // Write data channel			      
         .mesh_master_axi_WVALID	( to_l3_mesh_master_axi_WVALID	),
         .mesh_master_axi_W		( to_l3_mesh_master_axi_W	),
         .mesh_master_axi_WREADY	( to_l3_mesh_master_axi_WREADY	),

         // Read address channel			      
         .mesh_master_axi_ARVALID	( to_l3_mesh_master_axi_ARVALID ),
         .mesh_master_axi_AR		( to_l3_mesh_master_axi_AR	),
         .mesh_master_axi_ARREADY	( to_l3_mesh_master_axi_ARREADY ),
			      
         // Read data channel			      
         .mesh_master_axi_RVALID	( to_l3_mesh_master_axi_RVALID	),
         .mesh_master_axi_R		( to_l3_mesh_master_axi_R	),
         .mesh_master_axi_RREADY	( to_l3_mesh_master_axi_RREADY	),
			      
         // Write response channel
         .mesh_master_axi_BVALID	( to_l3_mesh_master_axi_BVALID	),
         .mesh_master_axi_B		( to_l3_mesh_master_axi_B	),
         .mesh_master_axi_BREADY	( to_l3_mesh_master_axi_BREADY	),
			      
	 ////////////////////////////////////////
	 // Shire Non-cacheable Interface (sys)
	 ////////////////////////////////////////
		
         // Write address channel			      
         .uc_to_sys_mesh_master_axi_AWVALID	( uc_to_sys_mesh_master_axi_AWVALID ),
         .uc_to_sys_mesh_master_axi_AW		( uc_to_sys_mesh_master_axi_AW	),
         .uc_to_sys_mesh_master_axi_AWREADY	( uc_to_sys_mesh_master_axi_AWREADY ),
			      
         // Write data channel			      
         .uc_to_sys_mesh_master_axi_WVALID	( uc_to_sys_mesh_master_axi_WVALID	),
         .uc_to_sys_mesh_master_axi_W		( uc_to_sys_mesh_master_axi_W	),
         .uc_to_sys_mesh_master_axi_WREADY	( uc_to_sys_mesh_master_axi_WREADY	),

         // Read address channel			      
         .uc_to_sys_mesh_master_axi_ARVALID	( uc_to_sys_mesh_master_axi_ARVALID ),
         .uc_to_sys_mesh_master_axi_AR		( uc_to_sys_mesh_master_axi_AR	),
         .uc_to_sys_mesh_master_axi_ARREADY	( uc_to_sys_mesh_master_axi_ARREADY ),
			      		  
         // Read data channel		  
         .uc_to_sys_mesh_master_axi_RVALID	( uc_to_sys_mesh_master_axi_RVALID	),
         .uc_to_sys_mesh_master_axi_R		( uc_to_sys_mesh_master_axi_R	),
         .uc_to_sys_mesh_master_axi_RREADY	( uc_to_sys_mesh_master_axi_RREADY	),
			      		  
         // Write response channel	  
         .uc_to_sys_mesh_master_axi_BVALID	( uc_to_sys_mesh_master_axi_BVALID	),
         .uc_to_sys_mesh_master_axi_B		( uc_to_sys_mesh_master_axi_B	),
         .uc_to_sys_mesh_master_axi_BREADY	( uc_to_sys_mesh_master_axi_BREADY	),
			      
	 ////////////////////////////////////////
	 // Shire Non-cacheable Interface (l3)
	 ////////////////////////////////////////
		
         // Write address channel			      
         .uc_to_l3_mesh_master_axi_AWVALID	( uc_to_l3_mesh_master_axi_AWVALID	),
         .uc_to_l3_mesh_master_axi_AW		( uc_to_l3_mesh_master_axi_AW		),
         .uc_to_l3_mesh_master_axi_AWREADY	( uc_to_l3_mesh_master_axi_AWREADY	),
			      
         // Write data channel			      
         .uc_to_l3_mesh_master_axi_WVALID	( uc_to_l3_mesh_master_axi_WVALID	),
         .uc_to_l3_mesh_master_axi_W		( uc_to_l3_mesh_master_axi_W		),
         .uc_to_l3_mesh_master_axi_WREADY	( uc_to_l3_mesh_master_axi_WREADY	),

         // Read address channel			      
         .uc_to_l3_mesh_master_axi_ARVALID	( uc_to_l3_mesh_master_axi_ARVALID	),
         .uc_to_l3_mesh_master_axi_AR		( uc_to_l3_mesh_master_axi_AR		),
         .uc_to_l3_mesh_master_axi_ARREADY	( uc_to_l3_mesh_master_axi_ARREADY	),
			      		  
         // Read data channel		  
         .uc_to_l3_mesh_master_axi_RVALID	( uc_to_l3_mesh_master_axi_RVALID	),
         .uc_to_l3_mesh_master_axi_R		( uc_to_l3_mesh_master_axi_R		),
         .uc_to_l3_mesh_master_axi_RREADY	( uc_to_l3_mesh_master_axi_RREADY	),
			      		  
         // Write response channel	  
         .uc_to_l3_mesh_master_axi_BVALID	( uc_to_l3_mesh_master_axi_BVALID	),
         .uc_to_l3_mesh_master_axi_B		( uc_to_l3_mesh_master_axi_B		),
         .uc_to_l3_mesh_master_axi_BREADY	( uc_to_l3_mesh_master_axi_BREADY	),
			      
	 ////////////////////////////////////////
	 // Shire Sys Interface
	 ////////////////////////////////////////
		
         // Write address channel			      
         .to_sys_mesh_master_axi_AWVALID	( to_sys_mesh_master_axi_AWVALID	),
         .to_sys_mesh_master_axi_AW		( to_sys_mesh_master_axi_AW		),
         .to_sys_mesh_master_axi_AWREADY	( to_sys_mesh_master_axi_AWREADY	),
			      
         // Write data channel			      
         .to_sys_mesh_master_axi_WVALID		( to_sys_mesh_master_axi_WVALID		),
         .to_sys_mesh_master_axi_W		( to_sys_mesh_master_axi_W		),
         .to_sys_mesh_master_axi_WREADY		( to_sys_mesh_master_axi_WREADY		),

         // Read address channel			      
         .to_sys_mesh_master_axi_ARVALID	( to_sys_mesh_master_axi_ARVALID	),
         .to_sys_mesh_master_axi_AR		( to_sys_mesh_master_axi_AR		),
         .to_sys_mesh_master_axi_ARREADY	( to_sys_mesh_master_axi_ARREADY	),
			      		  
         // Read data channel		  
         .to_sys_mesh_master_axi_RVALID		( to_sys_mesh_master_axi_RVALID		),
         .to_sys_mesh_master_axi_R		( to_sys_mesh_master_axi_R		),
         .to_sys_mesh_master_axi_RREADY		( to_sys_mesh_master_axi_RREADY		),
			      		  
         // Write response channel	  
         .to_sys_mesh_master_axi_BVALID		( to_sys_mesh_master_axi_BVALID		),
         .to_sys_mesh_master_axi_B		( to_sys_mesh_master_axi_B		),
         .to_sys_mesh_master_axi_BREADY		( to_sys_mesh_master_axi_BREADY		),
			      
	 ////////////////////////////////////////
	 // Minion-sim C++ NoC Interface
	 ////////////////////////////////////////
		
         // mesh cacheable		      
         .mesh_req_valid		( l2_mesh_req_valid		),
         .mesh_req			( l2_mesh_req			),
         .mesh_req_ready		( l2_mesh_req_ready		),
			      
         .mesh_rep_valid		( mesh_rep_l2_valid		),
         .mesh_rep			( mesh_rep			),
         .mesh_rep_ready		( mesh_rep_l2_ready		),
			      
         // mesh non-cacheable		      
         .uc_mesh_req_valid		( uc_mesh_req_valid		),
         .uc_mesh_req			( uc_mesh_req			),
         .uc_mesh_req_ready		( uc_mesh_req_ready		),
			      
         .uc_mesh_rep_valid		( uc_mesh_rep_valid		),
         .uc_mesh_rep			( uc_mesh_rep			),
         .uc_mesh_rep_ready		( uc_mesh_rep_ready		),
			      
         // mesh non-cacheable		      
         .sys_mesh_req_valid		( sys_mesh_req_valid		),
         .sys_mesh_req			( sys_mesh_req			),
         .sys_mesh_req_ready		( sys_mesh_req_ready		),
			      
         .sys_mesh_rep_valid		( sys_mesh_rep_valid		),
         .sys_mesh_rep			( sys_mesh_rep			),
         .sys_mesh_rep_ready		( sys_mesh_rep_ready		)
      );

   // request IF
    assign mesh_req_valid    = l2_mesh_req_valid;
    assign mesh_req          = l2_mesh_req;
   
    assign mesh_req_l2_en    = 1'b1;
    assign l2_mesh_req_ready = mesh_req_ready & mesh_req_l2_en;

   // Response IF
   
   `ifndef NETSPEED_MESH
     //MRN//assign mesh_rep_l2_en = ~mesh_rep.uc;
     assign mesh_rep_l2_en = 1;
   `else
      assign mesh_rep_l2_en = 1;
   `endif
    assign mesh_rep_l2_valid = mesh_rep_valid & mesh_rep_l2_en;
    assign mesh_rep_ready    = mesh_rep_l2_en ? mesh_rep_l2_ready : 1'b0;

endmodule
