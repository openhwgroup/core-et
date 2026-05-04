   // Main Regions
   static int pk_io_region          = evl_param_xref::get_param_key("IO Region");
   static int pk_systemreg_region   = evl_param_xref::get_param_key("SystemReg Region");
   static int pk_mramreg_region     = evl_param_xref::get_param_key("MRAM Registers Region");
   static int pk_esr_region         = evl_param_xref::get_param_key("ESR Region");
   static int pk_hyperbus_region    = evl_param_xref::get_param_key("HyperBus Region");
   static int pk_bootrom_region     = evl_param_xref::get_param_key("BootROM Region");
   static int pk_mram_region        = evl_param_xref::get_param_key("MRAM Region");
   static int pk_otp_region         = evl_param_xref::get_param_key("OTP Region");
   static int pk_reserved0_region   = evl_param_xref::get_param_key("RESERVED0 Region");
   static int pk_sram_region        = evl_param_xref::get_param_key("SRAM Region");
   static int pk_plic_region        = evl_param_xref::get_param_key("PLIC Region");
   static int pk_scp_region         = evl_param_xref::get_param_key("SCP Region");
   static int pk_reserved_region    = evl_param_xref::get_param_key("RESERVED Region");

   // DRAM: cached subregions
   //static int pk_c_mcode_region    = evl_param_xref::get_param_key("Cached Mcode Region");                // when "secure" disabled
   //static int pk_c_os_region       = evl_param_xref::get_param_key("Cached OS Region");                   // when "secure" disabled
   //static int pk_c_mem_region      = evl_param_xref::get_param_key("Cached Mem Region");
//
   //// DRAM: cached subregions: new PMA: RTLMIN-5656: when mprot.enable_secure_mem=1:
   //static int pk_secure_c_m_code_region    = evl_param_xref::get_param_key("(Secure) Cached M code Region");
   //static int pk_secure_c_m_data_region    = evl_param_xref::get_param_key("(Secure) Cached M data Region");
   //static int pk_secure_c_s_code_region    = evl_param_xref::get_param_key("(Secure) Cached S code Region");
   //static int pk_secure_c_s_data_region    = evl_param_xref::get_param_key("(Secure) Cached S data Region");
   //static int pk_secure_c_os_region        = evl_param_xref::get_param_key("(Secure) Cached OS Region");
//
   //// DRAM: uncached subregions
   //static int pk_uc_mcode_region = evl_param_xref::get_param_key("Uncached Mcode Region");
   //static int pk_uc_os_region    = evl_param_xref::get_param_key("Uncached OS Region");
   //static int pk_uc_mem_region   = evl_param_xref::get_param_key("Uncached Mem Region");
//
   //// IO: subregions
   //static int pk_maxion_region   = evl_param_xref::get_param_key("Maxion Region");
   //static int pk_periph_region   = evl_param_xref::get_param_key("Peripheral Region");
   //static int pk_mailbox_region  = evl_param_xref::get_param_key("Mailbox Region");
//
   //// Some useful SP Region subregions
   //static int pk_sp_rom_region  = evl_param_xref::get_param_key("SP ROM Region");
   //static int pk_sp_ram_region  = evl_param_xref::get_param_key("SP RAM Region");
//
   //// RTLMIN-5656: new PMA: two SP_SRam subregions: lower & upper:
   //static int pk_sp_ram_low_region  = evl_param_xref::get_param_key("SP RAM Low Region");
   //static int pk_sp_ram_upp_region  = evl_param_xref::get_param_key("SP RAM Upp Region");


