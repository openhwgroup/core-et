assign reset_n_system               = reset_n_in ;
assign reset_n_system_debug         = reset_n_in ;
assign resetn_ios_noc               = reset_n_in ;

assign resetn_cold_clk              = reset_n_in;
assign resetn_ios_apb               = reset_n_in;

assign cru_sms_resetn               = reset_n_in;
assign resetn_sms_10mhz             = reset_n_in;
assign resetn_main_wrck             = reset_n_in;

assign reset_cold_minion            = ~reset_n_in;
assign reset_warm_minion            = {`SOCTOP_MINION_SHIRES{1'b0}};
assign reset_cold_memshire_east     = ~reset_n_in;
assign reset_cold_memshire_west     = ~reset_n_in;
assign reset_warm_memshire_east     = {`SOCTOP_MEMSHIRES_PER_SIDE{1'b0}};
assign reset_warm_memshire_west     = {`SOCTOP_MEMSHIRES_PER_SIDE{1'b0}};
assign reset_cold_pshire            = ~reset_n_in;
assign reset_warm_pshire            = ~reset_n_in;
assign reset_debug_pshire           = ~reset_n_in;
assign msg_lock_enable_esperanto    = 1'b1;
assign msg_lock_enable_riscv_minion = 1'b1;
assign msg_lock_enable_pcie         = 1'b1;

assign resetn_cold_clk_ios_apb      = reset_n_in;
assign resetn_cold_clk_ios_ahb      = reset_n_in;
assign resetn_cold_clk_ios          = reset_n_in;
assign resetn_cold_clk_24mhz        = reset_n_in;
assign resetn_cold_clk_10mhz        = reset_n_in;

assign resetn_boot_clk_ios_apb      = reset_n_in;
assign resetn_cold_clk              = reset_n_in;
assign resetn_cold_clk_200mhz       = reset_n_in;
assign resetn_cold_clk_25mhz        = reset_n_in;
assign resetn_cold_clk_500mhz       = reset_n_in;
assign resetn_emmc_aclk             = reset_n_in;
assign resetn_emmc_bclk             = reset_n_in;
assign resetn_emmc_cclk             = reset_n_in;
assign resetn_emmc_tclk             = reset_n_in;

assign resetn_ust                         = reset_n_in;
assign resetn_ios                         = reset_n_in;
assign resetn_ios_ahb                     = reset_n_in;
assign resetn_ios_usb20_ahb2axi_ahb_clk   = reset_n_in;
assign resetn_ios_usb20_ctrl_ahb_clk      = reset_n_in;
assign resetn_ios_usb20_ctrl_phy_clk      = reset_n_in;
assign resetn_ios_usb20_dbg_utmi_clk      = reset_n_in;
assign resetn_ios_usb20_esr_apb_clk       = reset_n_in;
assign resetn_ios_usb20_phy_por           = reset_n_in;
assign resetn_ios_usb20_phy_port0         = reset_n_in;
assign resetn_ios_usb20_reloc_apb_clk     = reset_n_in;
assign resetn_ios_usb21_ahb2axi_ahb_clk   = reset_n_in;
assign resetn_ios_usb21_ctrl_ahb_clk      = reset_n_in;
assign resetn_ios_usb21_ctrl_phy_clk      = reset_n_in;
assign resetn_ios_usb21_esr_apb_clk       = reset_n_in;
assign resetn_ios_usb21_phy_por           = reset_n_in;
assign resetn_ios_usb21_phy_port0         = reset_n_in;
assign resetn_ios_usb21_reloc_apb_clk     = reset_n_in;
assign resetn_ios_usb_24mhz               = reset_n_in;

assign resetn_max_pll_core    = reset_n_in;
assign resetn_max_pll_uncore  = reset_n_in;
assign resetn_pu_gpio         = reset_n_in;
assign resetn_pu_i2c          = reset_n_in;
assign resetn_pu_i3c          = reset_n_in;
assign resetn_pu_spi          = reset_n_in;
assign resetn_pu_timer0       = reset_n_in;
assign resetn_pu_timer1       = reset_n_in;
assign resetn_pu_timer2       = reset_n_in;
assign resetn_pu_timer3       = reset_n_in;
assign resetn_pu_timer4       = reset_n_in;
assign resetn_pu_timer5       = reset_n_in;
assign resetn_pu_timer6       = reset_n_in;
assign resetn_pu_timer7       = reset_n_in;
assign resetn_pu_uart0        = reset_n_in;
assign resetn_pu_uart1        = reset_n_in;
assign resetn_pu_wdt          = reset_n_in;
assign resetn_sms             = reset_n_in;
assign resetn_spio_dma        = reset_n_in;
assign resetn_spio_gpio       = reset_n_in;
assign resetn_spio_i2c0       = reset_n_in;
assign resetn_spio_i2c1       = reset_n_in;
assign resetn_spio_pll0       = reset_n_in;
assign resetn_spio_pll1       = reset_n_in;
assign resetn_spio_pll2       = reset_n_in;
assign resetn_spio_pll3       = reset_n_in;
assign resetn_spio_pll4       = reset_n_in;
assign resetn_spio_sp         = reset_n_in;
assign resetn_spio_spi0       = reset_n_in;
assign resetn_spio_spi1       = reset_n_in;
assign resetn_spio_timer0     = reset_n_in;
assign resetn_spio_timer1     = reset_n_in;
assign resetn_spio_timer2     = reset_n_in;
assign resetn_spio_timer3     = reset_n_in;
assign resetn_spio_timer4     = reset_n_in;
assign resetn_spio_timer5     = reset_n_in;
assign resetn_spio_timer6     = reset_n_in;
assign resetn_spio_timer7     = reset_n_in;
assign resetn_spio_uart0      = reset_n_in;
assign resetn_spio_uart1      = reset_n_in;
assign resetn_spio_vault_por  = reset_n_in;
assign resetn_spio_wdt        = reset_n_in;

assign cru_sys_reset                = 1'b0;
assign boot_status_error_sms_udr    = 1'b0;
assign boot_status_error_sms_bist   = 1'b0;
assign boot_status_error_sms_bihr   = 1'b0;
assign boot_status_error_vault      = 1'b0;
assign boot_status_state0           = 1'b0;
assign boot_status_state1           = 1'b0;
assign boot_status_state2           = 1'b0;
assign boot_status_state3           = 1'b0;
assign boot_status_state4           = 1'b0;
assign boot_status_state5           = 1'b0;
assign boot_status_finish           = 1'b0;
assign boot_status_sp_hold          = 1'b0;
assign debug_ready                  = 1'b0;
assign spio_vault_fatal_error       = 1'b0;

assign spio_gpio_out                = 16'b0;
assign spio_gpio_oe                 = 16'b0;

assign spio_uart0_tx                = 1'b0;
assign spio_uart1_tx                = 1'b0;

assign spio_i2c0_clk_oe             = 1'b0;
assign spio_i2c0_data_oe            = 1'b0;
assign spio_i2c1_clk_oe             = 1'b0;
assign spio_i2c1_data_oe            = 1'b0;

assign spio_spi0_txd                = 1'b0;
assign spio_spi0_ssi_oe             = 1'b0;
assign spio_spi0_ss_0_n             = 1'b0;
assign spio_spi0_sclk_out           = 1'b0;

assign spio_spi1_txd                = 1'b0;
assign spio_spi1_ssi_oe             = 1'b0;
assign spio_spi1_ss_0_n             = 1'b0;
assign spio_spi1_ss_1_n             = 1'b0;
assign spio_spi1_sclk_out           = 1'b0;

assign clk_ios_apb_origin           = clkroot_100mhz;
assign clk_24mhz                    = clkroot_24mhz;
assign clk_ref_hpdpll               = clkroot_100mhz;
assign clk_ref_lvdpll               = clkroot_100mhz;
assign clk_ios_ahb                  = clkroot_100mhz;
assign clk_step_min                 = clkroot_ext;

beh_pll_module beh_pll ();

//
assign clk__noc_origin              = beh_pll.clk_beh_500Mhz; // meshstop noc clock
assign clk_ios_origin               = beh_pll.clk_beh_500Mhz_90deg;
//
//assign clk__noc_origin              = beh_pll.clk_beh_500Mhz; // meshstop noc clock
//assign clk_ios_origin               = beh_pll.clk_beh_500Mhz;
//
//assign clk__noc_origin              = clkroot_100mhz;
//assign clk_ios_origin               = clkroot_100mhz;
//
assign clk_10mhz                    = beh_pll.clk_beh_10Mhz;
assign clk_main_wrck                = clk_10mhz;
assign clk_ios_origin_int           = clk_ios_origin;
assign clk_ios_apb_origin_int       = clk_ios_apb_origin;

assign clk_200mhz                   = beh_pll.clk_beh_200Mhz;
assign clk_25mhz                    = beh_pll.clk_beh_25Mhz;
assign clk_3mhz                     = beh_pll.clk_beh_3Mhz;
assign clk_500mhz                   = beh_pll.clk_beh_500Mhz;

assign spio_noc_spio_i2c1_apb_pready         = 1'b1;
assign spio_noc_spio_i2c1_apb_pslverr        = 1'b0;
assign spio_noc_spio_i2c1_apb_prdata         = 32'b0;
assign spio_noc_spio_spi1_apb_pready         = 1'b1;
assign spio_noc_spio_spi1_apb_pslverr        = 1'b0;
assign spio_noc_spio_spi1_apb_prdata         = 32'b0;
assign spio_noc_spio_uart1_apb_pready        = 1'b1;
assign spio_noc_spio_uart1_apb_pslverr       = 1'b0;
assign spio_noc_spio_uart1_apb_prdata        = 32'b0;
assign spio_noc_spio_pll0_apb_pready         = 1'b1;
assign spio_noc_spio_pll0_apb_pslverr        = 1'b0;
assign spio_noc_spio_pll0_apb_prdata         = 32'b0;
assign spio_noc_spio_pll1_apb_pready         = 1'b1;
assign spio_noc_spio_pll1_apb_pslverr        = 1'b0;
assign spio_noc_spio_pll1_apb_prdata         = 32'b0;
assign spio_noc_spio_pll2_apb_pready         = 1'b1;
assign spio_noc_spio_pll2_apb_pslverr        = 1'b0;
assign spio_noc_spio_pll2_apb_prdata         = 32'b0;
assign spio_noc_spio_pll3_apb_pready         = 1'b1;
assign spio_noc_spio_pll3_apb_pslverr        = 1'b0;
assign spio_noc_spio_pll3_apb_prdata         = 32'b0;
assign spio_noc_spio_pll4_apb_pready         = 1'b1;
assign spio_noc_spio_pll4_apb_pslverr        = 1'b0;
assign spio_noc_spio_pll4_apb_prdata         = 32'b0;
assign spio_noc_spio_plic_apb_pready         = 1'b1;
assign spio_noc_spio_plic_apb_pslverr        = 1'b0;
assign spio_noc_spio_plic_apb_prdata         = 32'b0;
assign spio_noc_spio_i2c0_apb_pready         = 1'b1;
assign spio_noc_spio_i2c0_apb_pslverr        = 1'b0;
assign spio_noc_spio_i2c0_apb_prdata         = 32'b0;
assign spio_noc_spio_spi0_apb_pready         = 1'b1;
assign spio_noc_spio_spi0_apb_pslverr        = 1'b0;
assign spio_noc_spio_spi0_apb_prdata         = 32'b0;
assign spio_noc_spio_uart0_apb_pready        = 1'b1;
assign spio_noc_spio_uart0_apb_pslverr       = 1'b0;
assign spio_noc_spio_uart0_apb_prdata        = 32'b0;
assign spio_noc_spio_wdt_apb_pready          = 1'b1;
assign spio_noc_spio_wdt_apb_pslverr         = 1'b0;
assign spio_noc_spio_wdt_apb_prdata          = 32'b0;
assign spio_noc_spio_timers_apb_pready       = 1'b1;
assign spio_noc_spio_timers_apb_pslverr      = 1'b0;
assign spio_noc_spio_timers_apb_prdata       = 32'b0;
assign spio_noc_spio_cru_apb_pready          = 1'b1;
assign spio_noc_spio_cru_apb_pslverr         = 1'b0;
assign spio_noc_spio_cru_apb_prdata          = 32'b0;
assign spio_noc_spio_misc_esr_apb_pready     = 1'b1;
assign spio_noc_spio_misc_esr_apb_pslverr    = 1'b0;
assign spio_noc_spio_misc_esr_apb_prdata     = 32'b0;
assign spio_noc_spio_vault_apb_pready        = 1'b1;
assign spio_noc_spio_vault_apb_pslverr       = 1'b0;
assign spio_noc_spio_vault_apb_prdata        = 32'b0;
assign spio_noc_spio_dma_reloc_apb_pready    = 1'b1;
assign spio_noc_spio_dma_reloc_apb_pslverr   = 1'b0;
assign spio_noc_spio_dma_reloc_apb_prdata    = 32'b0;
assign spio_noc_spio_gpio_apb_prdata         = 32'b0;

assign spio_noc_spio_dma_ctrl_slave_ahb_hreadyout  = 1'b1;
assign spio_noc_spio_rvtimer_slave_ahb_hreadyout   = 1'b1;

assign spio_noc_spio_dma_ctrl_slave_ahb_hrdata  = 'b0;
assign spio_noc_spio_dma_ctrl_slave_ahb_hresp   = 'b0;
assign spio_noc_spio_rvtimer_slave_ahb_hrdata   = 'b0;
assign spio_noc_spio_rvtimer_slave_ahb_hresp    = 'b0;

assign efuse_lockbox_sadcl_ram_bc            = 'b0;
assign efuse_lockbox_sadcl_ram_ra            = 'b0;
assign efuse_lockbox_sadcl_ram_rm            = 'b0;
assign efuse_lockbox_sadcl_ram_rme           = 'b0;
assign efuse_lockbox_sadcl_ram_wa            = 'b0;
assign efuse_lockbox_sadcl_ram_wpulse        = 'b0;

assign msg_lock_enable_esperanto             = 1'b1;
assign msg_lock_enable_riscv_minion          = 1'b1;
assign msg_lock_enable_riscv_maxion          = 1'b1;
assign msg_lock_enable_jtag_dft              = 1'b1;
assign msg_lock_enable_pcie                  = 1'b1;

assign esr_ms_dmctrl.dmactive                = 1'b0;

assign root_me_spio_me_us_msg_ready_meN      = 1'b0;
assign root_me_spio_me_ds_msg_valid_meN      = 1'b0;
assign root_me_spio_me_ds_msg_event_meN      = 1'b0;
assign root_me_spio_me_ds_msg_data_meN       = 'b0;
assign root_me_spio_axicomm_us_msg_valid_meN = 1'b0;
assign root_me_spio_axicomm_us_msg_event_meN = 1'b0;
assign root_me_spio_axicomm_us_msg_data_meN  =  'b0;
assign root_me_spio_axicomm_ds_msg_ready_meN = 1'b0;

assign dft__usb_test_clk = 1'b0;

// Signals that where Z
assign async_resetn_ios_usb20_ahb2axi_ahb_clk   = reset_n_in;
assign async_resetn_ios_usb20_ctrl_ahb_clk      = reset_n_in;
assign async_resetn_ios_usb20_ctrl_phy_clk      = reset_n_in;
assign async_resetn_ios_usb20_dbg_utmi_clk      = reset_n_in;
assign async_resetn_ios_usb20_esr_apb_clk       = reset_n_in;
assign async_resetn_ios_usb20_phy_por           = reset_n_in;
assign async_resetn_ios_usb20_phy_port0         = reset_n_in;
assign async_resetn_ios_usb20_reloc_apb_clk     = reset_n_in;
assign async_resetn_ios_usb21_ahb2axi_ahb_clk   = reset_n_in;
assign async_resetn_ios_usb21_ctrl_ahb_clk      = reset_n_in;
assign async_resetn_ios_usb21_ctrl_phy_clk      = reset_n_in;
assign async_resetn_ios_usb21_esr_apb_clk       = reset_n_in;
assign async_resetn_ios_usb21_phy_por           = reset_n_in;
assign async_resetn_ios_usb21_phy_port0         = reset_n_in;
assign async_resetn_ios_usb21_reloc_apb_clk     = reset_n_in;

assign spio_pll_debug_yin_out    = spio_pll_debug_yin_in;
assign spio_pll_debug_yang_out   = spio_pll_debug_yang_in;
//assign spio_tdr_so               = spio_tdr_si;
assign spio_tdr_so               = 1'b0;

assign pu_usb20_ust_en           = 1'b1;
