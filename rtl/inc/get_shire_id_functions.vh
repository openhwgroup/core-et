// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "shire_defines.vh"

   function automatic bit [5:0] get_minion_shire_phys_id(input bit [7:0] virtual_shire_id);
      case (virtual_shire_id)
         8'd0:    get_minion_shire_phys_id = 6'd9;
         8'd1:    get_minion_shire_phys_id = 6'd17;
         8'd2:    get_minion_shire_phys_id = 6'd33;
         8'd3:    get_minion_shire_phys_id = 6'd42;
         8'd4:    get_minion_shire_phys_id = 6'd12;
         8'd5:    get_minion_shire_phys_id = 6'd22;
         8'd6:    get_minion_shire_phys_id = 6'd38;
         8'd7:    get_minion_shire_phys_id = 6'd45;
         8'd8:    get_minion_shire_phys_id = 6'd18;
         8'd9:    get_minion_shire_phys_id = 6'd25;
         8'd10:   get_minion_shire_phys_id = 6'd41;
         8'd11:   get_minion_shire_phys_id = 6'd49;
         8'd12:   get_minion_shire_phys_id = 6'd11;
         8'd13:   get_minion_shire_phys_id = 6'd30;
         8'd14:   get_minion_shire_phys_id = 6'd46;
         8'd15:   get_minion_shire_phys_id = 6'd54;
         8'd16:   get_minion_shire_phys_id = 6'd10;
         8'd17:   get_minion_shire_phys_id = 6'd26;
         8'd18:   get_minion_shire_phys_id = 6'd34;
         8'd19:   get_minion_shire_phys_id = 6'd50;
         8'd20:   get_minion_shire_phys_id = 6'd21;
         8'd21:   get_minion_shire_phys_id = 6'd29;
         8'd22:   get_minion_shire_phys_id = 6'd37;
         8'd23:   get_minion_shire_phys_id = 6'd53;
         8'd24:   get_minion_shire_phys_id = 6'd19;
         8'd25:   get_minion_shire_phys_id = 6'd27;
         8'd26:   get_minion_shire_phys_id = 6'd35;
         8'd27:   get_minion_shire_phys_id = 6'd43;
         8'd28:   get_minion_shire_phys_id = 6'd20;
         8'd29:   get_minion_shire_phys_id = 6'd28;
         8'd30:   get_minion_shire_phys_id = 6'd36;
         8'd31:   get_minion_shire_phys_id = 6'd44;
         8'd32:   get_minion_shire_phys_id = 6'd51;
         8'd33:   get_minion_shire_phys_id = 6'd52;
         default: get_minion_shire_phys_id = 6'd63;
      endcase
   endfunction

   function automatic bit [7:0] get_minion_shire_bridge_id(input bit [7:0] virtual_shire_id);
      case (virtual_shire_id)
         8'd0:    get_minion_shire_bridge_id = 8'd0;
         8'd1:    get_minion_shire_bridge_id = 8'd4;
         8'd2:    get_minion_shire_bridge_id = 8'd16;
         8'd3:    get_minion_shire_bridge_id = 8'd23;
         8'd4:    get_minion_shire_bridge_id = 8'd3;
         8'd5:    get_minion_shire_bridge_id = 8'd9;
         8'd6:    get_minion_shire_bridge_id = 8'd21;
         8'd7:    get_minion_shire_bridge_id = 8'd26;
         8'd8:    get_minion_shire_bridge_id = 8'd5;
         8'd9:    get_minion_shire_bridge_id = 8'd10;
         8'd10:   get_minion_shire_bridge_id = 8'd22;
         8'd11:   get_minion_shire_bridge_id = 8'd28;
         8'd12:   get_minion_shire_bridge_id = 8'd2;
         8'd13:   get_minion_shire_bridge_id = 8'd15;
         8'd14:   get_minion_shire_bridge_id = 8'd27;
         8'd15:   get_minion_shire_bridge_id = 8'd33;
         8'd16:   get_minion_shire_bridge_id = 8'd1;
         8'd17:   get_minion_shire_bridge_id = 8'd11;
         8'd18:   get_minion_shire_bridge_id = 8'd17;
         8'd19:   get_minion_shire_bridge_id = 8'd29;
         8'd20:   get_minion_shire_bridge_id = 8'd8;
         8'd21:   get_minion_shire_bridge_id = 8'd14;
         8'd22:   get_minion_shire_bridge_id = 8'd20;
         8'd23:   get_minion_shire_bridge_id = 8'd32;
         8'd24:   get_minion_shire_bridge_id = 8'd6;
         8'd25:   get_minion_shire_bridge_id = 8'd12;
         8'd26:   get_minion_shire_bridge_id = 8'd18;
         8'd27:   get_minion_shire_bridge_id = 8'd24;
         8'd28:   get_minion_shire_bridge_id = 8'd7;
         8'd29:   get_minion_shire_bridge_id = 8'd13;
         8'd30:   get_minion_shire_bridge_id = 8'd19;
         8'd31:   get_minion_shire_bridge_id = 8'd25;
         8'd32:   get_minion_shire_bridge_id = 8'd30;
         8'd33:   get_minion_shire_bridge_id = 8'd31;
         default: get_minion_shire_bridge_id = 8'd255;
      endcase
   endfunction

   function automatic bit [5:0] get_minion_shire_tdr_id(input bit [7:0] virtual_shire_id);
      case (virtual_shire_id)
         8'd0:    get_minion_shire_tdr_id = 6'd3;
         8'd1:    get_minion_shire_tdr_id = 6'd4;
         8'd2:    get_minion_shire_tdr_id = 6'd16;
         8'd3:    get_minion_shire_tdr_id = 6'd26;
         8'd4:    get_minion_shire_tdr_id = 6'd0;
         8'd5:    get_minion_shire_tdr_id = 6'd9;
         8'd6:    get_minion_shire_tdr_id = 6'd21;
         8'd7:    get_minion_shire_tdr_id = 6'd23;
         8'd8:    get_minion_shire_tdr_id = 6'd5;
         8'd9:    get_minion_shire_tdr_id = 6'd15;
         8'd10:   get_minion_shire_tdr_id = 6'd27;
         8'd11:   get_minion_shire_tdr_id = 6'd28;
         8'd12:   get_minion_shire_tdr_id = 6'd1;
         8'd13:   get_minion_shire_tdr_id = 6'd10;
         8'd14:   get_minion_shire_tdr_id = 6'd22;
         8'd15:   get_minion_shire_tdr_id = 6'd33;
         8'd16:   get_minion_shire_tdr_id = 6'd2;
         8'd17:   get_minion_shire_tdr_id = 6'd14;
         8'd18:   get_minion_shire_tdr_id = 6'd17;
         8'd19:   get_minion_shire_tdr_id = 6'd29;
         8'd20:   get_minion_shire_tdr_id = 6'd8;
         8'd21:   get_minion_shire_tdr_id = 6'd11;
         8'd22:   get_minion_shire_tdr_id = 6'd20;
         8'd23:   get_minion_shire_tdr_id = 6'd32;
         8'd24:   get_minion_shire_tdr_id = 6'd6;
         8'd25:   get_minion_shire_tdr_id = 6'd13;
         8'd26:   get_minion_shire_tdr_id = 6'd18;
         8'd27:   get_minion_shire_tdr_id = 6'd25;
         8'd28:   get_minion_shire_tdr_id = 6'd7;
         8'd29:   get_minion_shire_tdr_id = 6'd12;
         8'd30:   get_minion_shire_tdr_id = 6'd19;
         8'd31:   get_minion_shire_tdr_id = 6'd24;
         8'd32:   get_minion_shire_tdr_id = 6'd30;
         8'd33:   get_minion_shire_tdr_id = 6'd31;
         default: get_minion_shire_tdr_id = 6'd63;
      endcase
   endfunction


   // convert from bridge_id to virtual_id (only used by DV code)
   function automatic bit [7:0] get_minion_shire_virt_id(input bit [7:0] bridge_id);
      case (bridge_id)
         8'd0:    get_minion_shire_virt_id = 8'd0;
         8'd1:    get_minion_shire_virt_id = 8'd16;
         8'd2:    get_minion_shire_virt_id = 8'd12;
         8'd3:    get_minion_shire_virt_id = 8'd4;
         8'd4:    get_minion_shire_virt_id = 8'd1;
         8'd5:    get_minion_shire_virt_id = 8'd8;
         8'd6:    get_minion_shire_virt_id = 8'd24;
         8'd7:    get_minion_shire_virt_id = 8'd28;
         8'd8:    get_minion_shire_virt_id = 8'd20;
         8'd9:    get_minion_shire_virt_id = 8'd5;
         8'd10:   get_minion_shire_virt_id = 8'd9;
         8'd11:   get_minion_shire_virt_id = 8'd17;
         8'd12:   get_minion_shire_virt_id = 8'd25;
         8'd13:   get_minion_shire_virt_id = 8'd29;
         8'd14:   get_minion_shire_virt_id = 8'd21;
         8'd15:   get_minion_shire_virt_id = 8'd13;
         8'd16:   get_minion_shire_virt_id = 8'd2;
         8'd17:   get_minion_shire_virt_id = 8'd18;
         8'd18:   get_minion_shire_virt_id = 8'd26;
         8'd19:   get_minion_shire_virt_id = 8'd30;
         8'd20:   get_minion_shire_virt_id = 8'd22;
         8'd21:   get_minion_shire_virt_id = 8'd6;
         8'd22:   get_minion_shire_virt_id = 8'd10;
         8'd23:   get_minion_shire_virt_id = 8'd3;
         8'd24:   get_minion_shire_virt_id = 8'd27;
         8'd25:   get_minion_shire_virt_id = 8'd31;
         8'd26:   get_minion_shire_virt_id = 8'd7;
         8'd27:   get_minion_shire_virt_id = 8'd14;
         8'd28:   get_minion_shire_virt_id = 8'd11;
         8'd29:   get_minion_shire_virt_id = 8'd19;
         8'd30:   get_minion_shire_virt_id = 8'd32;
         8'd31:   get_minion_shire_virt_id = 8'd33;
         8'd32:   get_minion_shire_virt_id = 8'd23;
         8'd33:   get_minion_shire_virt_id = 8'd15;
         default: get_minion_shire_virt_id = 8'd255;
      endcase
   endfunction


   function automatic bit [5:0] get_memory_shire_phys_id(input bit [7:0] memshire_id);
      case (memshire_id)
         8'd0:    get_memory_shire_phys_id = 6'd16;
         8'd1:    get_memory_shire_phys_id = 6'd24;
         8'd2:    get_memory_shire_phys_id = 6'd32;
         8'd3:    get_memory_shire_phys_id = 6'd40;
         8'd4:    get_memory_shire_phys_id = 6'd23;
         8'd5:    get_memory_shire_phys_id = 6'd31;
         8'd6:    get_memory_shire_phys_id = 6'd39;
         8'd7:    get_memory_shire_phys_id = 6'd47;
         default: get_memory_shire_phys_id = 6'd63;
      endcase
   endfunction

   function automatic bit [7:0] get_memory_shire_virt_id(input bit [7:0] memshire_id);
      case (memshire_id)
         8'd0:    get_memory_shire_virt_id = 8'd232;
         8'd1:    get_memory_shire_virt_id = 8'd233;
         8'd2:    get_memory_shire_virt_id = 8'd234;
         8'd3:    get_memory_shire_virt_id = 8'd235;
         8'd4:    get_memory_shire_virt_id = 8'd236;
         8'd5:    get_memory_shire_virt_id = 8'd237;
         8'd6:    get_memory_shire_virt_id = 8'd238;
         8'd7:    get_memory_shire_virt_id = 8'd239;
         default: get_memory_shire_virt_id = 8'd255;
      endcase
   endfunction

   // This function returns the Netspeed Generated ID NOT the ID used in the RTL
   function automatic bit [7:0] get_memory_shire_from_virt_id(input bit [7:0] memshire_id);
      case (memshire_id)
         8'd232:  get_memory_shire_from_virt_id = 8'd0;
         8'd233:  get_memory_shire_from_virt_id = 8'd2;
         8'd234:  get_memory_shire_from_virt_id = 8'd4;
         8'd235:  get_memory_shire_from_virt_id = 8'd6;
         8'd236:  get_memory_shire_from_virt_id = 8'd1;
         8'd237:  get_memory_shire_from_virt_id = 8'd3;
         8'd238:  get_memory_shire_from_virt_id = 8'd5;
         8'd239:  get_memory_shire_from_virt_id = 8'd7;
         default: get_memory_shire_from_virt_id = 8'd255;
      endcase
   endfunction

   function automatic bit [7:0] get_memshire_virt_id_from_index(input bit [7:0] index);
      case (index)
         8'd0:    get_memshire_virt_id_from_index = 8'd232;
         8'd1:    get_memshire_virt_id_from_index = 8'd236;
         8'd2:    get_memshire_virt_id_from_index = 8'd233;
         8'd3:    get_memshire_virt_id_from_index = 8'd237;
         8'd4:    get_memshire_virt_id_from_index = 8'd234;
         8'd5:    get_memshire_virt_id_from_index = 8'd238;
         8'd6:    get_memshire_virt_id_from_index = 8'd235;
         8'd7:    get_memshire_virt_id_from_index = 8'd239;
         default: get_memshire_virt_id_from_index = 8'd255;
      endcase
   endfunction

   function automatic bit [5:0] get_io_shire_phys_id(input bit [7:0] ioshire_id);
      case (ioshire_id)
         8'd0:    get_io_shire_phys_id = 6'd14; // ioshire
         8'd1:    get_io_shire_phys_id = 6'd13; // pshire
         default: get_io_shire_phys_id = 6'd63;
      endcase
   endfunction

   function automatic bit [7:0] get_io_shire_virt_id(input bit [7:0] ioshire_id);
      case (ioshire_id)
         8'd0:    get_io_shire_virt_id = `IO_SHIRE_ID; // ioshire
         8'd1:    get_io_shire_virt_id = `P_SHIRE_ID; // pshire
         default: get_io_shire_virt_id = 8'd255;
      endcase
   endfunction


   function automatic logic [15:0] get_minion_ustoc_host_id(input bit [7:0] shire_id);
      case (shire_id)
         8'd0:    get_minion_ustoc_host_id = 16'h1;
         8'd1:    get_minion_ustoc_host_id = 16'h5;
         8'd2:    get_minion_ustoc_host_id = 16'h9;
         8'd3:    get_minion_ustoc_host_id = 16'hd;
         8'd4:    get_minion_ustoc_host_id = 16'h11;
         8'd5:    get_minion_ustoc_host_id = 16'h15;
         8'd6:    get_minion_ustoc_host_id = 16'h19;
         8'd7:    get_minion_ustoc_host_id = 16'h1d;
         8'd8:    get_minion_ustoc_host_id = 16'h21;
         8'd9:    get_minion_ustoc_host_id = 16'h25;
         8'd10:   get_minion_ustoc_host_id = 16'h29;
         8'd11:   get_minion_ustoc_host_id = 16'h2d;
         8'd12:   get_minion_ustoc_host_id = 16'h31;
         8'd13:   get_minion_ustoc_host_id = 16'h35;
         8'd14:   get_minion_ustoc_host_id = 16'h39;
         8'd15:   get_minion_ustoc_host_id = 16'h3d;
         8'd16:   get_minion_ustoc_host_id = 16'h41;
         8'd17:   get_minion_ustoc_host_id = 16'h45;
         8'd18:   get_minion_ustoc_host_id = 16'h49;
         8'd19:   get_minion_ustoc_host_id = 16'h4d;
         8'd20:   get_minion_ustoc_host_id = 16'h51;
         8'd21:   get_minion_ustoc_host_id = 16'h55;
         8'd22:   get_minion_ustoc_host_id = 16'h59;
         8'd23:   get_minion_ustoc_host_id = 16'h5d;
         8'd24:   get_minion_ustoc_host_id = 16'h61;
         8'd25:   get_minion_ustoc_host_id = 16'h65;
         8'd26:   get_minion_ustoc_host_id = 16'h69;
         8'd27:   get_minion_ustoc_host_id = 16'h6d;
         8'd28:   get_minion_ustoc_host_id = 16'h71;
         8'd29:   get_minion_ustoc_host_id = 16'h75;
         8'd30:   get_minion_ustoc_host_id = 16'h79;
         8'd31:   get_minion_ustoc_host_id = 16'h7d;
         8'd32:   get_minion_ustoc_host_id = 16'h81;
         8'd33:   get_minion_ustoc_host_id = 16'h85;
         default: get_minion_ustoc_host_id = 16'hff;
      endcase
   endfunction

   function automatic logic [(`SHIRE_NOC_BRIDGE_HP_WIDTH * `ust_psum4(`shire_noc_bridge_event_tiers, 8)) - 1:0] get_shire_debug_event_destinations_from_shire_id(input bit [7:0] shire_id);
      case (shire_id)
         8'd0:    get_shire_debug_event_destinations_from_shire_id = {{8'd17,8'd41},{8'd155,8'd5,8'd9},{8'd153,8'd65,8'd81}};
         8'd4:    get_shire_debug_event_destinations_from_shire_id = {{8'd1,8'd41},{8'd137,8'd21,8'd25},{8'd33,8'd89,8'd105}};
         8'd10:   get_shire_debug_event_destinations_from_shire_id = {{8'd1,8'd17},{8'd141,8'd45,8'd49},{8'd57,8'd113,8'd129}};
         8'd1:    get_shire_debug_event_destinations_from_shire_id = {{8'd21,8'd45},{8'd155,8'd1,8'd9},{8'd0,8'd69,8'd85}};
         8'd5:    get_shire_debug_event_destinations_from_shire_id = {{8'd5,8'd45},{8'd137,8'd17,8'd25},{8'd37,8'd93,8'd109}};
         8'd11:   get_shire_debug_event_destinations_from_shire_id = {{8'd5,8'd21},{8'd141,8'd41,8'd49},{8'd61,8'd117,8'd133}};
         8'd2:    get_shire_debug_event_destinations_from_shire_id = {{8'd25,8'd49},{8'd155,8'd1,8'd5},{8'd154,8'd73,8'd147}};
         8'd6:    get_shire_debug_event_destinations_from_shire_id = {{8'd9,8'd49},{8'd137,8'd17,8'd21},{8'd139,8'd97,8'd151}};
         8'd12:   get_shire_debug_event_destinations_from_shire_id = {{8'd9,8'd25},{8'd141,8'd41,8'd45},{8'd143,8'd121,8'd162}};
         8'd3:    get_shire_debug_event_destinations_from_shire_id = {{8'd29,8'd53},{8'd153,8'd0,8'd154},{8'd155,8'd145,8'd77}};
         8'd7:    get_shire_debug_event_destinations_from_shire_id = {{8'd13,8'd53},{8'd33,8'd37,8'd139},{8'd137,8'd149,8'd101}};
         8'd13:   get_shire_debug_event_destinations_from_shire_id = {{8'd13,8'd29},{8'd57,8'd61,8'd143},{8'd141,8'd161,8'd125}};
         8'd8:    get_shire_debug_event_destinations_from_shire_id = {{8'd153,8'd57},{8'd29,8'd37,8'd139},{8'd17,8'd89,8'd105}};
         8'd14:   get_shire_debug_event_destinations_from_shire_id = {{8'd153,8'd33},{8'd53,8'd61,8'd143},{8'd41,8'd113,8'd129}};
         8'd9:    get_shire_debug_event_destinations_from_shire_id = {{8'd0,8'd61},{8'd29,8'd33,8'd139},{8'd21,8'd93,8'd109}};
         8'd15:   get_shire_debug_event_destinations_from_shire_id = {{8'd0,8'd37},{8'd53,8'd57,8'd143},{8'd45,8'd117,8'd133}};
         8'd16:   get_shire_debug_event_destinations_from_shire_id = {{8'd89,8'd113},{8'd145,8'd69,8'd73},{8'd1,8'd153,8'd81}};
         8'd22:   get_shire_debug_event_destinations_from_shire_id = {{8'd65,8'd113},{8'd149,8'd93,8'd97},{8'd17,8'd33,8'd105}};
         8'd28:   get_shire_debug_event_destinations_from_shire_id = {{8'd65,8'd89},{8'd161,8'd117,8'd121},{8'd41,8'd57,8'd129}};
         8'd17:   get_shire_debug_event_destinations_from_shire_id = {{8'd93,8'd117},{8'd145,8'd65,8'd73},{8'd5,8'd0,8'd85}};
         8'd23:   get_shire_debug_event_destinations_from_shire_id = {{8'd69,8'd117},{8'd149,8'd89,8'd97},{8'd21,8'd37,8'd109}};
         8'd29:   get_shire_debug_event_destinations_from_shire_id = {{8'd69,8'd93},{8'd161,8'd113,8'd121},{8'd45,8'd61,8'd133}};
         8'd18:   get_shire_debug_event_destinations_from_shire_id = {{8'd97,8'd121},{8'd145,8'd65,8'd69},{8'd9,8'd154,8'd147}};
         8'd24:   get_shire_debug_event_destinations_from_shire_id = {{8'd73,8'd121},{8'd149,8'd89,8'd93},{8'd25,8'd139,8'd151}};
         8'd30:   get_shire_debug_event_destinations_from_shire_id = {{8'd73,8'd97},{8'd161,8'd113,8'd117},{8'd49,8'd143,8'd162}};
         8'd19:   get_shire_debug_event_destinations_from_shire_id = {{8'd101,8'd125},{8'd81,8'd85,8'd147},{8'd155,8'd13,8'd145}};
         8'd25:   get_shire_debug_event_destinations_from_shire_id = {{8'd77,8'd125},{8'd105,8'd109,8'd151},{8'd137,8'd29,8'd149}};
         8'd31:   get_shire_debug_event_destinations_from_shire_id = {{8'd77,8'd101},{8'd129,8'd133,8'd162},{8'd141,8'd53,8'd161}};
         8'd20:   get_shire_debug_event_destinations_from_shire_id = {{8'd105,8'd129},{8'd77,8'd85,8'd147},{8'd1,8'd153,8'd65}};
         8'd26:   get_shire_debug_event_destinations_from_shire_id = {{8'd81,8'd129},{8'd101,8'd109,8'd151},{8'd17,8'd33,8'd89}};
         8'd32:   get_shire_debug_event_destinations_from_shire_id = {{8'd81,8'd105},{8'd125,8'd133,8'd162},{8'd41,8'd57,8'd113}}; 
         8'd21:   get_shire_debug_event_destinations_from_shire_id = {{8'd109,8'd133},{8'd77,8'd81,8'd147},{8'd5,8'd0,8'd69}};
         8'd27:   get_shire_debug_event_destinations_from_shire_id = {{8'd85,8'd133},{8'd101,8'd105,8'd151},{8'd21,8'd37,8'd93}};
         8'd33:   get_shire_debug_event_destinations_from_shire_id = {{8'd85,8'd109},{8'd125,8'd129,8'd162},{8'd45,8'd61,8'd117}};
         default: get_shire_debug_event_destinations_from_shire_id = 64'd0;
      endcase
   endfunction

   function automatic logic [(`SHIRE_NOC_BRIDGE_HP_WIDTH * `ust_psum4(`shire_noc_bridge_event_tiers, 8)) - 1:0] get_ioshire_dn_debug_event_destinations_from_shire_id(input bit [7:0] ioshire_id);
      case (ioshire_id)
         8'd0:    get_ioshire_dn_debug_event_destinations_from_shire_id = {{8'd37,8'd61},{8'd13,8'd153,8'd154},{8'd5,8'd69,8'd85}};
         default: get_ioshire_dn_debug_event_destinations_from_shire_id = 64'd0;
      endcase
   endfunction

   function automatic logic [(`SHIRE_NOC_BRIDGE_HP_WIDTH * `ust_psum4(`shire_noc_bridge_event_tiers, 8)) - 1:0] get_ioshire_lf_debug_event_destinations_from_shire_id(input bit [7:0] ioshire_id);
      case (ioshire_id)
         8'd0:    get_ioshire_lf_debug_event_destinations_from_shire_id = {{8'd139,8'd143},{8'd13,8'd153,8'd0},{8'd9,8'd73,8'd147}};
         default: get_ioshire_lf_debug_event_destinations_from_shire_id = 64'd0;
      endcase
   endfunction

   function automatic logic [(`SHIRE_NOC_BRIDGE_HP_WIDTH * `ust_psum4(`shire_noc_bridge_event_tiers, 8)) - 1:0] get_pshire_debug_event_destinations_from_shire_id(input bit [7:0] pshire_id);
      case (pshire_id)
         8'd0:    get_pshire_debug_event_destinations_from_shire_id = {{8'd33,8'd57},{8'd13,8'd0,8'd154},{8'd1,8'd65,8'd81}};
         default: get_pshire_debug_event_destinations_from_shire_id = 64'd0;
      endcase
   endfunction

   function automatic logic [(`SHIRE_NOC_BRIDGE_HP_WIDTH * `ust_psum4(`shire_noc_bridge_event_tiers, 8)) - 1:0] get_memshire_dn_debug_event_destinations_from_shire_id(input bit [7:0] memshire_id);
      case (memshire_id)
         8'd0:    get_memshire_dn_debug_event_destinations_from_shire_id = {{8'd155,8'd141},{8'd17,8'd21,8'd25},{8'd29,8'd149,8'd101}};
         8'd1:    get_memshire_dn_debug_event_destinations_from_shire_id = {{8'd155,8'd137},{8'd41,8'd45,8'd49},{8'd53,8'd161,8'd125}};
         8'd2:    get_memshire_dn_debug_event_destinations_from_shire_id = {{8'd149,8'd161},{8'd65,8'd69,8'd73},{8'd155,8'd13,8'd77}};
         8'd3:    get_memshire_dn_debug_event_destinations_from_shire_id = {{8'd145,8'd161},{8'd89,8'd93,8'd97},{8'd137,8'd29,8'd101}};
         8'd4:    get_memshire_dn_debug_event_destinations_from_shire_id = {{8'd154,8'd143},{8'd29,8'd33,8'd37},{8'd25,8'd97,8'd151}};
         8'd5:    get_memshire_dn_debug_event_destinations_from_shire_id = {{8'd154,8'd139},{8'd53,8'd57,8'd61},{8'd49,8'd121,8'd162}};
         8'd6:    get_memshire_dn_debug_event_destinations_from_shire_id = {{8'd151,8'd162},{8'd77,8'd81,8'd85},{8'd9,8'd154,8'd73}};
         8'd7:    get_memshire_dn_debug_event_destinations_from_shire_id = {{8'd147,8'd162},{8'd101,8'd105,8'd109},{8'd25,8'd139,8'd97}};
         default: get_memshire_dn_debug_event_destinations_from_shire_id = 64'd0;
      endcase
   endfunction

   function automatic logic [(`SHIRE_NOC_BRIDGE_HP_WIDTH * `ust_psum4(`shire_noc_bridge_event_tiers, 8)) - 1:0] get_memshire_lf_debug_event_destinations_from_shire_id(input bit [7:0] memshire_id);
      case (memshire_id)
         8'd0:    get_memshire_lf_debug_event_destinations_from_shire_id = {{8'd137,8'd141},{8'd1,8'd5,8'd9},{8'd13,8'd145,8'd77}};
         8'd1:    get_memshire_lf_debug_event_destinations_from_shire_id = 64'd0;
         8'd2:    get_memshire_lf_debug_event_destinations_from_shire_id = 64'd0;
         8'd3:    get_memshire_lf_debug_event_destinations_from_shire_id = {{8'd145,8'd149},{8'd113,8'd117,8'd121},{8'd141,8'd53,8'd125}};
         8'd4:    get_memshire_lf_debug_event_destinations_from_shire_id = 64'd0;
         8'd5:    get_memshire_lf_debug_event_destinations_from_shire_id = 64'd0;
         8'd6:    get_memshire_lf_debug_event_destinations_from_shire_id = 64'd0;
         8'd7:    get_memshire_lf_debug_event_destinations_from_shire_id = {{8'd147,8'd151},{8'd125,8'd129,8'd133},{8'd49,8'd143,8'd121}};
         default: get_memshire_lf_debug_event_destinations_from_shire_id = 64'd0;
      endcase
   endfunction

   function automatic logic [31:0] get_memshire_lf_debug_event_tiers_from_shire_id(input bit [7:0] memshire_id);
      case (memshire_id)
         8'd0:    get_memshire_lf_debug_event_tiers_from_shire_id = `MEMSHIRE_NORMAL_BRIDGE_EVENT_TIERS;
         8'd1:    get_memshire_lf_debug_event_tiers_from_shire_id = 32'd0;
         8'd2:    get_memshire_lf_debug_event_tiers_from_shire_id = 32'd0;
         8'd3:    get_memshire_lf_debug_event_tiers_from_shire_id = 32'd0;
         8'd4:    get_memshire_lf_debug_event_tiers_from_shire_id = 32'd0;
         8'd5:    get_memshire_lf_debug_event_tiers_from_shire_id = 32'd0;
         8'd6:    get_memshire_lf_debug_event_tiers_from_shire_id = `MEMSHIRE_NORMAL_BRIDGE_EVENT_TIERS;
         8'd7:    get_memshire_lf_debug_event_tiers_from_shire_id = `MEMSHIRE_NORMAL_BRIDGE_EVENT_TIERS;
         default: get_memshire_lf_debug_event_tiers_from_shire_id = 31'd0;
      endcase
   endfunction

   function logic [`US_INDEX_LENGTH-1:0] get_minionshire_ust_index_adjust_ip(input bit [7:0] minion_id);
      case (minion_id)
         8'd0:    get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h0;
         8'd1:    get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h10;
         8'd2:    get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h20;
         8'd3:    get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h30;
         8'd4:    get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h40;
         8'd5:    get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h50;
         8'd6:    get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h60;
         8'd7:    get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h70;
         8'd8:    get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h80;
         8'd9:    get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h90;
         8'd10:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'hA0;
         8'd11:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'hB0;
         8'd12:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'hC0;
         8'd13:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'hD0;
         8'd14:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'hE0;
         8'd15:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'hF0;
         8'd16:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h100;
         8'd17:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h110;
         8'd18:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h120;
         8'd19:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h130;
         8'd20:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h140;
         8'd21:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h150;
         8'd22:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h160;
         8'd23:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h170;
         8'd24:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h180;
         8'd25:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h190;
         8'd26:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h1A0;
         8'd27:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h1B0;
         8'd28:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h1C0;
         8'd29:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h1D0;
         8'd30:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h1E0;
         8'd31:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h1F0;
         8'd32:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h200;
         8'd33:   get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h210;
         default: get_minionshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h01;
      endcase
   endfunction

   function logic [`US_INDEX_LENGTH - 1:0] get_memshire_dn_ust_index_adjust_ip(input bit [7:0] memshire_id);
      case (memshire_id)
         8'd0:    get_memshire_dn_ust_index_adjust_ip = `US_INDEX_LENGTH'h220;
         8'd1:    get_memshire_dn_ust_index_adjust_ip = `US_INDEX_LENGTH'h230;
         8'd2:    get_memshire_dn_ust_index_adjust_ip = `US_INDEX_LENGTH'h240;
         8'd3:    get_memshire_dn_ust_index_adjust_ip = `US_INDEX_LENGTH'h250;
         8'd4:    get_memshire_dn_ust_index_adjust_ip = `US_INDEX_LENGTH'h228;
         8'd5:    get_memshire_dn_ust_index_adjust_ip = `US_INDEX_LENGTH'h238;
         8'd6:    get_memshire_dn_ust_index_adjust_ip = `US_INDEX_LENGTH'h248;
         8'd7:    get_memshire_dn_ust_index_adjust_ip = `US_INDEX_LENGTH'h258;
         default: get_memshire_dn_ust_index_adjust_ip = `US_INDEX_LENGTH'h0;
      endcase
   endfunction

   function logic [`US_INDEX_LENGTH - 1:0] get_memshire_lf_ust_index_adjust_ip(input bit [7:0] memshire_id);
      case (memshire_id)
         8'd0:    get_memshire_lf_ust_index_adjust_ip = `US_INDEX_LENGTH'h222;
         8'd1:    get_memshire_lf_ust_index_adjust_ip = `US_INDEX_LENGTH'h232;
         8'd2:    get_memshire_lf_ust_index_adjust_ip = `US_INDEX_LENGTH'h242;
         8'd3:    get_memshire_lf_ust_index_adjust_ip = `US_INDEX_LENGTH'h252;
         8'd4:    get_memshire_lf_ust_index_adjust_ip = `US_INDEX_LENGTH'h22A;
         8'd5:    get_memshire_lf_ust_index_adjust_ip = `US_INDEX_LENGTH'h23A;
         8'd6:    get_memshire_lf_ust_index_adjust_ip = `US_INDEX_LENGTH'h24A;
         8'd7:    get_memshire_lf_ust_index_adjust_ip = `US_INDEX_LENGTH'h25A;
         default: get_memshire_lf_ust_index_adjust_ip = `US_INDEX_LENGTH'h0;
      endcase
   endfunction

   function logic [`US_INDEX_LENGTH - 1:0] get_pshire_ust_index_adjust_ip(input bit [7:0] pshire_id);
      case (pshire_id)
         8'd0:    get_pshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h260;
         default: get_pshire_ust_index_adjust_ip = `US_INDEX_LENGTH'h0;
      endcase
   endfunction

