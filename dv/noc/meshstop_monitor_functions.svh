   // IN/OUT-PORT FIFOs in all the ports in all the routers (that belongs to a shire) in all the layers
   localparam  int NUM_PORT_ = 8;
   localparam string port_names[7:0] = '{"P_E","P_W","P_S", "P_N", "P_K", "P_J", "P_I", "P_H"};
   localparam  int MAX_CREDIT_WIDTH_ = 8;
   function void initialize_histogram(
				      output int   input_fifo_histogram [NUM_NOC_LAYERS-1:0][NUM_PORT_-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH_-1:0] ,
                                      output int  output_fifo_histogram [NUM_NOC_LAYERS-1:0][NUM_PORT_-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH_-1:0] 
				);
      int layer_num, port_num, vc_num, num_occupancy, occupancy;
      for (layer_num = 0; layer_num < NUM_NOC_LAYERS; layer_num = layer_num+1) begin
        for (port_num =0; port_num< NUM_PORT_; port_num = port_num+1) begin
          for (vc_num = 0; vc_num < NUM_VIRT_CHANNELS; vc_num = vc_num+1) begin
		for(num_occupancy =0; num_occupancy<MAX_CREDIT_WIDTH_; num_occupancy = num_occupancy + 1 ) begin
      		    //`evl_log(UVM_DEBUG, VMOD_MESH_MON, inst_name, $sformatf("INIT HISTi %0d %0d ",input_fifo_histogram[layer_num][port_num][vc_num][num_occupancy],output_fifo_histogram[layer_num][port_num][vc_num][num_occupancy] ));
		    input_fifo_histogram[layer_num][port_num][vc_num][num_occupancy]=0; 
		    output_fifo_histogram[layer_num][port_num][vc_num][num_occupancy]=0; 
      		    //`evl_log(UVM_DEBUG, VMOD_MESH_MON, inst_name, $sformatf("INIT HISTi %0d %0d ",input_fifo_histogram[layer_num][port_num][vc_num][num_occupancy],output_fifo_histogram[layer_num][port_num][vc_num][num_occupancy] ));
		    occupancy = 0 ; 
	        end 
          end
        end
      end
   endfunction 
   function void print_histogram(
                                 input logic [NUM_NOC_LAYERS-1:0][NUM_PORT_-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH_-1:0] inport_fifo_depth ,
                                 input logic [NUM_NOC_LAYERS-1:0][NUM_PORT_-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH_-1:0] outport_fifo_depth ,
			         input int   input_fifo_histogram [NUM_NOC_LAYERS-1:0][NUM_PORT_-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH_-1:0] ,
                                 input int  output_fifo_histogram [NUM_NOC_LAYERS-1:0][NUM_PORT_-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH_-1:0] , 
				 input int msg_verbosity = UVM_DEBUG
                                );
      int layer_num, port_num, vc_num, num_occupancy, occupancy, depth;
      string port_type ;

      string outputs ;
      string code_str ;

      // disable the prints for now - 10012019
      return;
      
      if ((`evl_log_check_verbosity(UVM_DEBUG, VMOD_MESH_MON) == 0) && (`evl_log_check_verbosity(msg_verbosity, VMOD_MESH_HIST) == 0)) begin
         return;
      end
      for (layer_num = 0; layer_num < NUM_NOC_LAYERS; layer_num = layer_num+1) begin
        for (port_num =0; port_num< NUM_PORT_; port_num = port_num+1) begin
          for (vc_num = 0; vc_num < NUM_VIRT_CHANNELS; vc_num = vc_num+1) begin
	 	depth = inport_fifo_depth[layer_num][port_num][vc_num];	
                if (  depth != 0 ) begin
		    port_type = "_IN"; 
		    outputs = "" ; 
                    for(num_occupancy =0; num_occupancy< (depth+1); num_occupancy = num_occupancy + 1 ) begin
			code_str = $sformatf("%0d:%0d",num_occupancy,input_fifo_histogram[layer_num][port_num][vc_num][num_occupancy]);
			if (num_occupancy == 0 ) begin
			   outputs = $sformatf("%s",code_str);
			end
			else begin	
			   outputs = $sformatf("%s,%s",outputs,code_str);
			end 
                    end
                    `evl_log(UVM_NONE, VMOD_MESH_HIST, inst_name, $sformatf("%0d/%0s/%0s/%0d - %0s", layer_num, port_type, port_names[port_num], vc_num, outputs ));
		end 
	 	depth = outport_fifo_depth[layer_num][port_num][vc_num];	
                if (  depth != 0 ) begin
		    port_type = "OUT"; 
		    outputs = "" ; 
                    for(num_occupancy =0; num_occupancy<(depth+1); num_occupancy = num_occupancy + 1 ) begin
			code_str = $sformatf("%0d:%0d",num_occupancy,output_fifo_histogram[layer_num][port_num][vc_num][num_occupancy]);
			if (num_occupancy == 0 ) begin
			   outputs = $sformatf("%s",code_str);
			end
			else begin	
			   outputs = $sformatf("%s,%s",outputs,code_str);
			end 
                    end
                    //`evl_log(UVM_DEBUG, VMOD_MESH_MON, inst_name, $sformatf("Layer %0d PortDirection %0s PortName %0s VC %0d Depth %0d - %0s", layer_num, port_type, port_names[port_num], vc_num, depth, outputs ));
                    `evl_log(msg_verbosity, VMOD_MESH_MON, inst_name, $sformatf("%0d/%0s/%0s/%0d - %0s", layer_num, port_type, port_names[port_num], vc_num, outputs ));
		end 
          end
        end
      end
   endfunction 

	
   function void watch_fifo_interface(input logic [NUM_NOC_LAYERS-1:0][NUM_PORT_-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH_-1:0] inport_rd_pntr_bin_q,
                                      input logic [NUM_NOC_LAYERS-1:0][NUM_PORT_-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH_-1:0] inport_wr_pntr_bin_q,
                                      input logic [NUM_NOC_LAYERS-1:0][NUM_PORT_-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH_-1:0] inport_fifo_depth,
                                      input logic [NUM_NOC_LAYERS-1:0][NUM_PORT_-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH_-1:0] outport_credit_cntr_q,
                                      input logic [NUM_NOC_LAYERS-1:0][NUM_PORT_-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH_-1:0] outport_fifo_depth, 
				      output int   input_fifo_histogram [NUM_NOC_LAYERS-1:0][NUM_PORT_-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH_-1:0] , 
				      output int  output_fifo_histogram [NUM_NOC_LAYERS-1:0][NUM_PORT_-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH_-1:0] 
					);
      int layer_num, port_num, vc_num;
      int occupancy; 
      string outputs ;
      string code_str ;
      outputs = "";
      code_str = "";
      // disable mesh monitor for now - 10022019
      return;
      
      for (layer_num = 0; layer_num < NUM_NOC_LAYERS; layer_num = layer_num+1) begin
        for (port_num =0; port_num< NUM_PORT_; port_num = port_num+1) begin
          for (vc_num = 0; vc_num < NUM_VIRT_CHANNELS; vc_num = vc_num+1) begin
		occupancy = 0 ; 
                if (  outport_fifo_depth[layer_num][port_num][vc_num] != 0 ) begin
                      code_str = process_outport_port(layer_num, port_num,vc_num, outport_credit_cntr_q[layer_num][port_num][vc_num],outport_fifo_depth[layer_num][port_num][vc_num], occupancy);
                      outputs = $sformatf("%s%s",outputs,code_str);
		      output_fifo_histogram[layer_num][port_num][vc_num][occupancy]= output_fifo_histogram[layer_num][port_num][vc_num][occupancy] + 1; 
                end
                if (  inport_fifo_depth[layer_num][port_num][vc_num] != 0 ) begin
                      code_str = process_inport_port(layer_num, port_num,vc_num, inport_rd_pntr_bin_q[layer_num][port_num][vc_num],inport_wr_pntr_bin_q[layer_num][port_num][vc_num],inport_fifo_depth[layer_num][port_num][vc_num],occupancy);
                      outputs = $sformatf("%s%s",outputs,code_str);
		      input_fifo_histogram[layer_num][port_num][vc_num][occupancy]= input_fifo_histogram[layer_num][port_num][vc_num][occupancy] + 1; 
                end

          end
        end
      end
      `evl_log(UVM_DEBUG, VMOD_MESH_MON, inst_name, $sformatf("%s", outputs ));
   endfunction

   function string process_outport_port(input int layer_num, input int port_num, input int vc_num, input int credit_cntr_q, input int outport_fifo_depth, output int occupancy);
       string port_type ;
       int port_occupancy ;
       int vc_full  ;
       int vc_empty  ;
       int code  ;
       string code_str;
       port_type = "OUT";
       vc_full = 0 ;
       vc_empty = 0;
       port_occupancy = outport_fifo_depth - credit_cntr_q;
       if (port_occupancy == 0 ) begin
           vc_empty = 1;
       end
       else if (port_occupancy == outport_fifo_depth) begin
           vc_full = 1;
       end
       code = 0 ;
       code = code | ( layer_num          << (0+3+3+2+3+1)  ) ;
       code = code | ( 0                  << (0+3+3+2+3)  ) ;
       code = code | ( port_num           << (0+3+3+2)  ) ;
       code = code | ( vc_num             << (0+3+3)  ) ;
       code = code | ( outport_fifo_depth << (0+3)  ) ;
       code = code | ( port_occupancy     << (0)  ) ;
       code_str = $sformatf("%04X",code);
       occupancy = port_occupancy; 


       //`evl_log(UVM_DEBUG, VMOD_MESH_MON, inst_name, $sformatf("Layer %0d PortDirection %0s PortName %0s VC %0d Depth %0d Occupancy %0d Full %0d Empty %0d - %0d %0s", layer_num, port_type, port_names[port_num], vc_num, outport_fifo_depth, port_occupancy, vc_full, vc_empty,credit_cntr_q, code_str ));
       return code_str;
   endfunction

   function string process_inport_port(input int layer_num, input int port_num, input int vc_num, input int rd_pntr_bin_q, input int wr_pntr_bin_q, input int fifo_depth, output int occupancy);
       string port_type ;
       int port_empty ;
       int port_size ;
       int P_FIFO_A_WIDTH ;
       int P_FIFO_A_WIDTH_1s ; 
       int is_full  ;
       int code ;
       string code_str;

       port_type = " IN";
       port_empty = 0;
       port_size = 0;
       P_FIFO_A_WIDTH = 1;
       P_FIFO_A_WIDTH_1s = 1'b1; 

       if ( fifo_depth <= 1 ) begin
            P_FIFO_A_WIDTH = 1;
	    P_FIFO_A_WIDTH_1s = 1'b1;  
       end
       else if (fifo_depth <= 2 ) begin
            P_FIFO_A_WIDTH = 1;
	    P_FIFO_A_WIDTH_1s = 1'b1;  
       end
       else if (fifo_depth <= 4 ) begin
            P_FIFO_A_WIDTH = 2;
	    P_FIFO_A_WIDTH_1s = 2'b1;  
       end
       else if (fifo_depth <= 8 ) begin
            P_FIFO_A_WIDTH = 3;
	    P_FIFO_A_WIDTH_1s = 3'b1;  
       end
       else if (fifo_depth <= 16 ) begin
            P_FIFO_A_WIDTH = 4;
	    P_FIFO_A_WIDTH_1s = 4'b1;  
       end
       is_full =(wr_pntr_bin_q & ( P_FIFO_A_WIDTH_1s+1) ) ^ (rd_pntr_bin_q & (P_FIFO_A_WIDTH_1s+1) );
       if ( is_full != 0 ) begin
            is_full =  ( (wr_pntr_bin_q & P_FIFO_A_WIDTH_1s) == (rd_pntr_bin_q & P_FIFO_A_WIDTH_1s));
       end
       if ( is_full == 0) begin
            if ( (wr_pntr_bin_q & P_FIFO_A_WIDTH_1s) == (rd_pntr_bin_q & P_FIFO_A_WIDTH_1s)) begin
                port_size =  1'b0;
                port_empty = 1'b1;
            end
            else begin
                port_empty = 1'b0;
                if ((wr_pntr_bin_q &P_FIFO_A_WIDTH_1s) > (rd_pntr_bin_q &P_FIFO_A_WIDTH_1s)) begin
                    port_size =  (wr_pntr_bin_q &P_FIFO_A_WIDTH_1s) - (rd_pntr_bin_q &P_FIFO_A_WIDTH_1s);
                end
                else begin
                    port_size = fifo_depth + (wr_pntr_bin_q &P_FIFO_A_WIDTH_1s) - (rd_pntr_bin_q &P_FIFO_A_WIDTH_1s) ;
                end
            end
       end
       else begin
            is_full = 1'b1 ;
            port_empty = 1'b0;
            port_size = fifo_depth;
       end
       code = 0 ;
       code = code | ( layer_num          << (0+3+3+2+3+1)  ) ;
       code = code | ( 1                  << (0+3+3+2+3)  ) ;
       code = code | ( port_num           << (0+3+3+2)  ) ;
       code = code | ( vc_num             << (0+3+3)  ) ;
       code = code | ( fifo_depth         << (0+3)  ) ;
       code = code | ( port_size          << (0)  ) ;
       occupancy = port_size;
       code_str = $sformatf("%04X",code);
       //`evl_log(UVM_DEBUG, VMOD_MESH_MON, inst_name, $sformatf("Layer %0d PortDirection %0s PortName %0s VC %0d Depth %0d Occupancy %0d Full %0d Empty %0d - %0d %0d %0s", layer_num, port_type, port_names[port_num], vc_num, fifo_depth, port_size, is_full, port_empty, rd_pntr_bin_q, wr_pntr_bin_q, code_str ));
       return code_str;
   endfunction

   function void watch_interface (input logic [NUM_NOC_LAYERS-1:0] [NUM_VIRT_CHANNELS-1:0] valid,
                                    input evl_paddr_t [NUM_NOC_LAYERS-1:0] addr,
                                    input evl_bus_req_id_t [NUM_NOC_LAYERS-1:0] req_id,
                                    input evl_bus_req_id_t [NUM_NOC_LAYERS-1:0] rsp_id,
                                    input evl_noc_flit_dir_t direction,
                                    input bit is_input);
      int layer_num, vc_num;

      for (layer_num = 0; layer_num < NUM_NOC_LAYERS; layer_num = layer_num+1) begin
         for (vc_num = 0; vc_num < NUM_VIRT_CHANNELS; vc_num = vc_num+1) begin
            if (valid[layer_num][vc_num] == 1'b1) begin
               casez ({layer_num, vc_num})
                  {int'(0), int'(0)}, {int'(2), int'(0)}, {int'(4), int'(0)}, {int'(6), int'(0)} : process_observed_req(addr[layer_num], req_id[layer_num], layer_num, vc_num, direction, AXI_REQ_READ, is_input); // TOL3 AR
                  {int'(1), int'(0)}, {int'(3), int'(0)}, {int'(5), int'(0)}, {int'(7), int'(0)} : process_observed_req(addr[layer_num], req_id[layer_num], layer_num, vc_num, direction, AXI_REQ_WRITE, is_input); // TOL3 AWW
                  {int'(0), int'(3)}, {int'(2), int'(3)}, {int'(4), int'(3)}, {int'(6), int'(3)} : process_observed_rsp(rsp_id[layer_num], layer_num, vc_num, direction, AXI_REQ_WRITE, is_input); // BRESP
                  {int'(1), int'(3)}, {int'(3), int'(3)}, {int'(5), int'(3)}, {int'(7), int'(3)} : process_observed_rsp(rsp_id[layer_num], layer_num, vc_num, direction, AXI_REQ_READ, is_input); // RRESP
                  {int'(0), int'(2)}                                                             : process_observed_req(addr[layer_num], req_id[layer_num], layer_num, vc_num, direction, AXI_REQ_READ, is_input); // IO/Messages AR
                  {int'(1), int'(2)}                                                             : process_observed_req(addr[layer_num], req_id[layer_num], layer_num, vc_num, direction, AXI_REQ_WRITE, is_input); // IO/Messages AWW
                  {int'(2), int'(1)}                                                             : process_observed_rsp(rsp_id[layer_num], layer_num, vc_num, direction, AXI_REQ_WRITE, is_input); // ESR R/B
                  {int'(2), int'(2)}                                                             : process_observed_req(addr[layer_num], req_id[layer_num], layer_num, vc_num, direction, AXI_REQ_READ, is_input); // ESR AR
                  {int'(3), int'(2)}                                                             : process_observed_req(addr[layer_num], req_id[layer_num], layer_num, vc_num, direction, AXI_REQ_WRITE, is_input); // ESR AWW
                  {int'(4), int'(1)}                                                             : process_observed_req(addr[layer_num], req_id[layer_num], layer_num, vc_num, direction, AXI_REQ_READ, is_input); // TOSYS AR
                  {int'(5), int'(1)}                                                             : process_observed_req(addr[layer_num], req_id[layer_num], layer_num, vc_num, direction, AXI_REQ_WRITE, is_input); // TOSYS AWW
                  {int'(5), int'(2)}                                                             : process_observed_rsp(rsp_id[layer_num], layer_num, vc_num, direction, AXI_REQ_WRITE, is_input); // MC Atomic Resp B
                  {int'(6), int'(2)}                                                             : process_observed_req(addr[layer_num], req_id[layer_num], layer_num, vc_num, direction, AXI_REQ_READ, is_input); // MC Atomic Resp AR
                  {int'(7), int'(1)}                                                             : process_observed_req(addr[layer_num], req_id[layer_num], layer_num, vc_num, direction, AXI_REQ_WRITE, is_input); // MC Atomic Resp AWW
                  {int'(7), int'(2)}                                                             : process_observed_rsp(rsp_id[layer_num], layer_num, vc_num, direction, AXI_REQ_READ, is_input); // MC Atomic Resp R
                  default        : begin
                                    `evl_log(UVM_HIGH, VMOD_MESH_MON, inst_name, $sformatf("Unexpected transaction on Layer %0d, VC %0d", layer_num, vc_num));
                                   end
               endcase
            end
         end
      end
   endfunction

   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_observed_req
   //
   function void process_observed_req (input evl_paddr_t req_addr, input evl_bus_req_id_t req_id, input int layer_num, input int vc_num, input evl_noc_flit_dir_t direction, input evl_axi_req_type_t req_type, input bit is_input);
      string                  txn_type;
      string                  txn_dir;
      evl_shire_id_t          source_shire;
      evl_shire_id_t          target_shire;
      evl_noc_master_type_t   master_type;

      if (is_input == 1'b1) begin
         txn_dir = "entering";
      end
      else begin
         txn_dir = "leaving";
      end

      decode_master_id(req_id, source_shire, master_type);
      `evl_log(UVM_HIGH, VMOD_MESH_MON, inst_name, $sformatf("%0s req ID 0x%0x Addr 0x%0x, %0s meshstop (S%0d %p, req ID 0x%0x, addr 0x%0x, Layer %0d, VC %0d, %s)", req_type, req_id[8:0], req_addr, txn_dir, source_shire, master_type, req_id, req_addr, layer_num, vc_num, direction));

   endfunction: process_observed_req

   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_observed_rsp
   //
   function void process_observed_rsp (input evl_bus_req_id_t req_id, input int layer_num, input int vc_num, input evl_noc_flit_dir_t direction, input evl_axi_req_type_t req_type, input bit is_input);
      string                  txn_type;
      string                  txn_dir;
      evl_shire_id_t          source_shire;
      evl_shire_id_t          target_shire;
      evl_noc_master_type_t   master_type;

      if (is_input == 1'b1) begin
         txn_dir = "entering";
      end
      else begin
         txn_dir = "leaving";
      end


      `evl_log(UVM_HIGH, VMOD_MESH_MON, inst_name, $sformatf("%0s RESP, req ID 0x%0x, %0s meshstop (req ID 0x%0x, Layer %0d, VC %0d, %s)", req_type, req_id, txn_dir, req_id, layer_num, vc_num, direction));

   endfunction: process_observed_rsp

   //-----------------------------------------------------------------------------------------------
   //
   // Function: decode_master_id
   //
   function void decode_master_id (input evl_bus_req_id_t req_id, output evl_shire_id_t shire, output evl_noc_master_type_t master_type);
      int            master_id;
      bit            sib_bit;
      evl_shire_id_t phys_shire_id;
      bit            match_found;

      master_id = req_id[19:10];
      sib_bit   = req_id[9];
      match_found = 1'b0;
      if (master_id <= 13) begin
         if ((master_id >= 4) && (master_id <= 11)) begin
            foreach (memshire_master_ids[ii]) begin
               if (memshire_master_ids[ii] == master_id) begin
                  shire = get_memshire_virt_id_from_index(ii);
                  master_type = MCTOSYS_M;
                  break;
               end
            end
         end
         else begin
            case (master_id)
               `IOSHIRE_PM_MASTER_ID    : begin
                                           shire = 'd254;
                                           master_type = IO_PM;
                                          end
               `IOSHIRE_TOL3_MASTER_ID  : begin
                                           shire = 'd254;
                                           master_type = IO_TOL3_M;
                                          end
               `IOSHIRE_TOL3B_MASTER_ID : begin
                                           shire = 'd254;
                                           master_type = IO_TOL3_M;
                                          end
               `IOSHIRE_TOSYS_MASTER_ID : begin
                                           shire = 'd254;
                                           master_type = IO_TOSYS_M;
                                          end
               `PSHIRE_TOL3_MASTER_ID   : begin
                                           shire = 'd253;
                                           master_type = PSHIRE_TOL3_M;
                                          end
               `PSHIRE_TOSYS_MASTER_ID  : begin
                                           shire = 'd253;
                                           master_type = PSHIRE_TOSYS_M;
                                          end
            endcase
         end
      end
      else begin
         if ((master_id >= 252) && (master_id <= 285)) begin
            foreach (tol3_sib_master_ids[ii]) begin
               if (tol3_sib_master_ids[ii] == master_id) begin
                  phys_shire_id = ii;
                  master_type = (sib_bit == 1) ? UCTOL3_M : L2TOL3_M;
                  break;
               end
            end
         end
         else if ((master_id >= 'd286) && (master_id <= 'd319)) begin
            foreach (tosys_sib_master_ids[ii]) begin
               if (tosys_sib_master_ids[ii] == master_id) begin
                  phys_shire_id = ii;
                  master_type = (sib_bit == 1) ? UCTOSYS_M : L3TOSYS_M;
                  break;
               end
            end
         end
         else begin
            foreach (l2tol3_master_ids[ii]) begin
               foreach (l2tol3_master_ids[ii][jj]) begin
                  if (l2tol3_master_ids[ii][jj] == master_id) begin
                     phys_shire_id = jj;
                     master_type = L2TOL3_M;
                     match_found = 1'b1;
                     break;
                  end
               end
               if (match_found == 1'b1) begin
                  break;
               end
            end
         end
         shire = get_minion_shire_virt_id(phys_shire_id);
      end

   endfunction : decode_master_id

