
class shire_cvg extends evl_void;

   covergroup lvdpll_mode_cg with function sample (bit [31:0] shire_id, bit[7:0]  lvdpll_mode, evl_etlink_req_opcode_t etlink_req_opcode, bit[3:0] clock_sel); 
      
      option.name = "lvdpll_mode_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;
      
      ccp_shire_id : coverpoint shire_id {
         bins shire_id[] = {[0:33]};
         bins others = default;
      }

      ccp_clock_sel : coverpoint clock_sel[2:0] {
         bins clock_sel[] = {[0:7]};
         bins others = default;
      }

      ccp_lvdpll_mode : coverpoint lvdpll_mode {
         bins mode1 = {1};
         bins mode2 = {2};
         bins mode3 = {3};
         bins mode4 = {4};
         bins mode5 = {5};
         bins mode16 = {16};
         bins mode17 = {17};
         bins mode18 = {18};
         bins mode19 = {19};
         bins mode20 = {20};
         bins mode21 = {21};
         bins mode22 = {22};
         bins mode23 = {23};
         bins others = default;
      }

      ccp_etlink_req_opcode: coverpoint etlink_req_opcode {
         bins        write         = { ET_LINK_REQ_Write };
         bins        read          = { ET_LINK_REQ_Read };
         bins        writearound   = { ET_LINK_REQ_WriteAround };
         bins        readcoop      = { ET_LINK_REQ_ReadCoop };
         bins        msgsend       = { ET_LINK_REQ_MsgSendData };
         bins        atomic        = { ET_LINK_REQ_Atomic };
         bins        flush         = { ET_LINK_REQ_Flush };
         bins        evict         = { ET_LINK_REQ_Evict };
         bins        lock          = { ET_LINK_REQ_Lock };
         bins        unlock        = { ET_LINK_REQ_Unlock };
         bins        scpfill       = { ET_LINK_REQ_ScpFill };
         bins        prefetch      = { ET_LINK_REQ_Prefetch };
         ignore_bins ignore_values = { ET_LINK_REQ_ScpRead,
                                       ET_LINK_REQ_ScpWrite,
                                       ET_LINK_REQ_FlushToMem,
                                       ET_LINK_REQ_EvictToMem,
                                       ET_LINK_REQ_AtomicRsp };
      }

      ccp_shire_id_x_lvdpll_mode: cross ccp_shire_id, ccp_lvdpll_mode;
      ccp_lvdpll_mode_x_etlink_req_opcode:   cross ccp_lvdpll_mode, ccp_etlink_req_opcode;

   endgroup : lvdpll_mode_cg

   covergroup dll_feedback_delay_offset_cg with function sample (int dll_feedback_delay_offset); 
      
      option.name = "dll_feedback_delay_offset";
      option.per_instance         = 0;
      type_option.merge_instances = 1;
      
      ccp_dll_feedback_delay_offset : coverpoint dll_feedback_delay_offset {
         bins dll_feedback_delay_offset[30] = {[300:1800]};
         bins others = default;
      }

   endgroup : dll_feedback_delay_offset_cg

   covergroup dll_clk_delay_cg with function sample (bit[5:0] sel_taps_feedback_shire , bit[5:0]  sel_taps_feedback_neigh, bit[23:0] sel_taps_neigh); 
      
      option.name = "dll_clk_delay_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;
      
      ccp_sel_taps_feedback_shire : coverpoint sel_taps_feedback_shire {
         bins taps_feedback_shire[] = {[0:63]};
         bins others = default;
      }

      ccp_sel_taps_feedback_neigh : coverpoint sel_taps_feedback_neigh {
         bins taps_feedback_neigh[] = {[0:63]};
         bins others = default;
      }

      ccp_sel_taps_neigh_0 : coverpoint sel_taps_neigh[5:0] {
         bins taps_neigh_0[] = {[0:63]};
         bins others = default;
      }

      ccp_sel_taps_neigh_1 : coverpoint sel_taps_neigh[11:6] {
         bins taps_neigh_1[] = {[0:63]};
         bins others = default;
      }

      ccp_sel_taps_neigh_2 : coverpoint sel_taps_neigh[17:12] {
         bins taps_neigh_2[] = {[0:63]};
         bins others = default;
      }

      ccp_sel_taps_neigh_3 : coverpoint sel_taps_neigh[23:18] {
         bins taps_neigh_3[] = {[0:63]};
         bins others = default;
      }


      ccp_taps_feedback_shire_x_taps_feedback_neigh_x_sel_taps_neigh: cross ccp_sel_taps_feedback_shire, ccp_sel_taps_feedback_neigh, ccp_sel_taps_neigh_0, ccp_sel_taps_neigh_1, ccp_sel_taps_neigh_2, ccp_sel_taps_neigh_3;

   endgroup : dll_clk_delay_cg

   covergroup noc_clock_freq_cg with function sample (int noc_clock_freq, int intf_num, int rw); 
      
      option.name = "noc_clock_freq";
      option.per_instance         = 0;
      type_option.merge_instances = 1;
      
      ccp_noc_clock_freq : coverpoint noc_clock_freq {
         bins f375MHz = {[370:380]};
         bins f400MHz = {[395:405]};
         bins f500MHz = {[495:505]};
         bins others = default;
      }

      ccp_noc_intf_num : coverpoint intf_num {
         bins sysy_mesh_slave = {0};
         bins to_l3_mesh_master = {1};
         bins to_sys_mesh_master = {2};
         bins l3_mesh_slave = {3};
         bins uc_to_l3_mesh_master = {4};
         bins uc_to_sys_mesh_master = {5};
         bins others = default;
      }

      ccp_rw : coverpoint rw {
         bins RD_op = {0};
         bins WR_op = {1};
         bins others = default;
      }

      ccp_noc_clock_freq_x_noc_intf_num_x_rw: cross ccp_noc_clock_freq, ccp_noc_intf_num, rw;

   endgroup : noc_clock_freq_cg
       
   function new();
      super.new();
      lvdpll_mode_cg = new();
      dll_feedback_delay_offset_cg = new();
      dll_clk_delay_cg = new();
      noc_clock_freq_cg = new();
   endfunction

endclass
