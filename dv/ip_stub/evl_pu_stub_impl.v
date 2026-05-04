assign pu_gpio_out                  = 8'b0;
assign pu_gpio_oe                   = 8'b0;

assign pu_uart0_tx                  = 1'b0;
assign pu_uart1_tx                  = 1'b0;

assign pu_i2c_clk_oe                = 1'b0;
assign pu_i2c_data_oe               = 1'b0;

assign pu_spi_txd                   = 1'b0;
assign pu_spi_ssi_oe                = 1'b0;
assign pu_spi_ss_0_n                = 1'b0;
assign pu_spi_ss_1_n                = 1'b0;
assign pu_spi_sclk_out              = 1'b0;

assign pu_i3c_sda_oe                = 1'b0;
assign pu_i3c_scl_out               = 1'b0;
assign pu_i3c_sda_out               = 1'b0;
assign pu_i3c_scl_pullup_en         = 1'b0;
assign pu_i3c_sda_pullup_en         = 1'b0;
assign pu_i3c_scl_oe                = 1'b0;

assign pu_dbg_jtag_tdo_op           = 1'b0;
assign pu_dbg_jtag_tdo_en_op        = 1'b0;

assign pu_usb21_drvvbus                      = 1'b0;
assign pu_noc_pu_i2c_apb_pready              = 1'b1;
assign pu_noc_pu_spi_apb_pready              = 1'b1;
assign pu_noc_pu_uart0_apb_pready            = 1'b1;
assign pu_noc_pu_wdt_apb_pready              = 1'b1;
assign pu_noc_pu_timers_apb_pready           = 1'b1;
assign pu_noc_pu_i3c_apb_pready              = 1'b1;
assign pu_noc_pu_uart1_apb_pready            = 1'b1;
assign pu_noc_pu_mbox_apb_pready             = 1'b1;
assign pu_noc_pu_dma_reloc_apb_pready        = 1'b1;
assign pu_noc_pu_plic_apb_pready             = 1'b1;
assign pu_noc_pu_usb20_reloc_apb_pready      = 1'b1;
assign pu_noc_pu_usb21_reloc_apb_pready      = 1'b1;
assign pu_noc_pu_usb20_esr_apb_pready        = 1'b1;
assign pu_noc_pu_usb21_esr_apb_pready        = 1'b1;
assign pu_noc_pu_dma_ctrl_slave_ahb_hreadyout = 1'b1;
assign pu_noc_pu_usb20_ctrl_slave_ahb_hreadyout = 1'b1;
assign pu_noc_pu_usb21_ctrl_slave_ahb_hreadyout = 1'b1;
assign pu_noc_pu_emmc_slave_ahb_hreadyout    = 1'b1;
assign pu_noc_pu_rvtimer_slave_ahb_hreadyout = 1'b1;


assign pu_noc_pu_wdt_apb_prdata           = 'b0;
assign pu_noc_pu_wdt_apb_pslverr          = 'b0;
assign pu_noc_pu_plic_apb_prdata          = 'b0;
assign pu_noc_pu_plic_apb_pslverr         = 'b0;
assign pu_noc_pu_spi_apb_prdata           = 'b0;
assign pu_noc_pu_spi_apb_pslverr          = 'b0;
assign pu_noc_pu_dma_reloc_apb_prdata     = 'b0;
assign pu_noc_pu_dma_reloc_apb_pslverr    = 'b0;
assign pu_noc_pu_gpio_apb_prdata          = 'b0;
assign pu_noc_pu_timers_apb_prdata        = 'b0;
assign pu_noc_pu_timers_apb_pslverr       = 'b0;
assign pu_noc_pu_uart0_apb_prdata         = 'b0;
assign pu_noc_pu_uart0_apb_pslverr        = 'b0;
assign pu_noc_pu_uart1_apb_prdata         = 'b0;
assign pu_noc_pu_uart1_apb_pslverr        = 'b0;
assign pu_noc_pu_i2c_apb_prdata           = 'b0;
assign pu_noc_pu_i2c_apb_pslverr          = 'b0;
assign pu_noc_pu_mbox_apb_prdata          = 'b0;
assign pu_noc_pu_mbox_apb_pslverr         = 'b0;
assign pu_noc_pu_usb21_reloc_apb_prdata   = 'b0;
assign pu_noc_pu_usb21_reloc_apb_pslverr  = 'b0;
assign pu_noc_pu_usb20_reloc_apb_prdata   = 'b0;
assign pu_noc_pu_usb20_reloc_apb_pslverr  = 'b0;
assign pu_noc_pu_i3c_apb_prdata           = 'b0;
assign pu_noc_pu_i3c_apb_pslverr          = 'b0;
assign pu_noc_pu_usb20_esr_apb_prdata     = 'b0;
assign pu_noc_pu_usb20_esr_apb_pslverr    = 'b0;
assign pu_noc_pu_usb21_esr_apb_prdata     = 'b0;
assign pu_noc_pu_usb21_esr_apb_pslverr    = 'b0;

assign pu_noc_pu_dma_ctrl_slave_ahb_hrdata   = 'b0;
assign pu_noc_pu_dma_ctrl_slave_ahb_hresp    = 'b0;
assign pu_noc_pu_usb20_ctrl_slave_ahb_hrdata = 'b0;
assign pu_noc_pu_usb20_ctrl_slave_ahb_hresp  = 'b0;
assign pu_noc_pu_usb21_ctrl_slave_ahb_hrdata = 'b0;
assign pu_noc_pu_usb21_ctrl_slave_ahb_hresp  = 'b0;
assign pu_noc_pu_emmc_slave_ahb_hrdata       = 'b0;
assign pu_noc_pu_emmc_slave_ahb_hresp        = 'b0;
assign pu_noc_pu_rvtimer_slave_ahb_hrdata    = 'b0;
assign pu_noc_pu_rvtimer_slave_ahb_hresp     = 'b0;

assign pu_dbg_me_gpio_out_op = 'b0;

assign root_me_spio_me_us_msg_valid_meN      = 'b0;
assign root_me_spio_me_us_msg_event_meN      = 'b0;
assign root_me_spio_me_us_msg_data_meN       = 'b0;
assign root_me_spio_me_ds_msg_ready_meN      = 'b0;
assign root_me_dbg_noc_leaf_us_msg_ready_op  = 'b0;
assign root_me_dbg_noc_leaf_ds_msg_valid_op  = 'b0;
assign root_me_dbg_noc_leaf_ds_msg_event_op  = 'b0;
assign root_me_dbg_noc_leaf_ds_msg_data_op   = 'b0;
assign root_me_dbg_noc_root_us_msg_valid_op  = 'b0;
assign root_me_dbg_noc_root_us_msg_event_op  = 'b0;
assign root_me_dbg_noc_root_us_msg_data_op   = 'b0;
assign root_me_dbg_noc_root_ds_msg_ready_op  = 'b0;
assign root_me_spio_axicomm_us_msg_ready_meN = 'b0;
assign root_me_spio_axicomm_ds_msg_valid_meN = 'b0;
assign root_me_spio_axicomm_ds_msg_event_meN = 'b0;
assign root_me_spio_axicomm_ds_msg_data_meN  = 'b0;

assign pu_pll_debug_yin_out   = pu_pll_debug_yin_in;
assign pu_pll_debug_yang_out  = pu_pll_debug_yang_in;

assign pu_tdr_so              = pu_tdr_si;
//assign dft_test_in            = feedthru_dft_test_in;
assign dft_test_in            = 'b0;
assign clk_test_in            = feedthru_clk_test_in;
assign dft_test_ip0           = feedthru_dft_test_ip0;
assign soc_top_tck            = feedthru_soc_top_tck;
assign soc_top_trst_n         = feedthru_soc_top_trst_n;

assign dft__test_enable       = 1'b0;

assign feedthru_bsr_shift_dr     = bsr_shift_dr;
assign feedthru_bsr_si           = bsr_si;
assign feedthru_bsr_capture_clk  = bsr_capture_clk;
assign feedthru_bsr_capture_en   = bsr_capture_en;
assign feedthru_bsr_update_clk   = bsr_update_clk;
assign feedthru_bsr_update_en    = bsr_update_en;
assign feedthru_bsr_mode         = bsr_mode;
assign feedthru_bsr_mode1        = bsr_mode1;

//assign bsr_so  = feedthru_bsr_so;
assign bsr_so  = 1'b0;

