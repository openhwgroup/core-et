// Regbus registers for Sh0 CSR address map
   evl_paddr_t m_noc_sh0_tosys_adbase_reg[evl_shire_id_t];

   //-----------------------------------------------------------------------------------------------
   // initialize_rbm_addr
   //
   function void initialize_rbm_addr();
      m_noc_sh0_tosys_adbase_reg[254] = evl_paddr_t'(31'h0054c060);
      m_noc_sh0_tosys_adbase_reg[232] = evl_paddr_t'(31'h0054c0a0);
      m_noc_sh0_tosys_adbase_reg[236] = evl_paddr_t'(31'h0054c0e0);
      m_noc_sh0_tosys_adbase_reg[233] = evl_paddr_t'(31'h0054c120);
      m_noc_sh0_tosys_adbase_reg[237] = evl_paddr_t'(31'h0054c160);
      m_noc_sh0_tosys_adbase_reg[234] = evl_paddr_t'(31'h0054c1a0);
      m_noc_sh0_tosys_adbase_reg[238] = evl_paddr_t'(31'h0054c1e0);
      m_noc_sh0_tosys_adbase_reg[235] = evl_paddr_t'(31'h0054c220);
      m_noc_sh0_tosys_adbase_reg[239] = evl_paddr_t'(31'h0054c260);
      m_noc_sh0_tosys_adbase_reg[0] = evl_paddr_t'(31'h0054c2a0);
      m_noc_sh0_tosys_adbase_reg[10] = evl_paddr_t'(31'h0054c2e0);
      m_noc_sh0_tosys_adbase_reg[11] = evl_paddr_t'(31'h0054c300);
      m_noc_sh0_tosys_adbase_reg[12] = evl_paddr_t'(31'h0054c320);
      m_noc_sh0_tosys_adbase_reg[13] = evl_paddr_t'(31'h0054c340);
      m_noc_sh0_tosys_adbase_reg[14] = evl_paddr_t'(31'h0054c360);
      m_noc_sh0_tosys_adbase_reg[15] = evl_paddr_t'(31'h0054c380);
      m_noc_sh0_tosys_adbase_reg[16] = evl_paddr_t'(31'h0054c3a0);
      m_noc_sh0_tosys_adbase_reg[17] = evl_paddr_t'(31'h0054c3c0);
      m_noc_sh0_tosys_adbase_reg[18] = evl_paddr_t'(31'h0054c3e0);
      m_noc_sh0_tosys_adbase_reg[19] = evl_paddr_t'(31'h0054c400);
      m_noc_sh0_tosys_adbase_reg[1] = evl_paddr_t'(31'h0054c2c0);
      m_noc_sh0_tosys_adbase_reg[20] = evl_paddr_t'(31'h0054c440);
      m_noc_sh0_tosys_adbase_reg[21] = evl_paddr_t'(31'h0054c460);
      m_noc_sh0_tosys_adbase_reg[22] = evl_paddr_t'(31'h0054c480);
      m_noc_sh0_tosys_adbase_reg[23] = evl_paddr_t'(31'h0054c4a0);
      m_noc_sh0_tosys_adbase_reg[24] = evl_paddr_t'(31'h0054c4c0);
      m_noc_sh0_tosys_adbase_reg[25] = evl_paddr_t'(31'h0054c4e0);
      m_noc_sh0_tosys_adbase_reg[26] = evl_paddr_t'(31'h0054c500);
      m_noc_sh0_tosys_adbase_reg[27] = evl_paddr_t'(31'h0054c520);
      m_noc_sh0_tosys_adbase_reg[28] = evl_paddr_t'(31'h0054c540);
      m_noc_sh0_tosys_adbase_reg[29] = evl_paddr_t'(31'h0054c560);
      m_noc_sh0_tosys_adbase_reg[2] = evl_paddr_t'(31'h0054c420);
      m_noc_sh0_tosys_adbase_reg[30] = evl_paddr_t'(31'h0054c5a0);
      m_noc_sh0_tosys_adbase_reg[31] = evl_paddr_t'(31'h0054c5c0);
      m_noc_sh0_tosys_adbase_reg[32] = evl_paddr_t'(31'h0054c5e0);
      m_noc_sh0_tosys_adbase_reg[33] = evl_paddr_t'(31'h0054c600);
      m_noc_sh0_tosys_adbase_reg[3] = evl_paddr_t'(31'h0054c580);
      m_noc_sh0_tosys_adbase_reg[4] = evl_paddr_t'(31'h0054c620);
      m_noc_sh0_tosys_adbase_reg[5] = evl_paddr_t'(31'h0054c640);
      m_noc_sh0_tosys_adbase_reg[6] = evl_paddr_t'(31'h0054c660);
      m_noc_sh0_tosys_adbase_reg[7] = evl_paddr_t'(31'h0054c680);
      m_noc_sh0_tosys_adbase_reg[8] = evl_paddr_t'(31'h0054c6a0);
      m_noc_sh0_tosys_adbase_reg[9] = evl_paddr_t'(31'h0054c6c0);
   endfunction : initialize_rbm_addr

