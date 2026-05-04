/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#define REGTEST_END_OF_LIST    0xEEEEEEEE


// Struct for IP
typedef struct ip
  {
    uint32_t base_address;
    uint32_t reg_offset;
    uint32_t data_width;
    uint32_t reset_value;
    uint32_t rw_mask;
    char     *ip_name;

  } IPREGS_t;


//                          Base_address,            reg_offset, data_width, reset_value, rw_mask ip_name
//                          -------------            ----------- ----------- ------------ ------- -------
    IPREGS_t arr_ip[] = {
                          { R_PU_I2C_BASEADDR,       0x3c,       0x20,        0x0,         0x03, "PU I2C"      },
                          { R_PU_SPI_BASEADDR,       0x14,       0x20,        0x0,         0x10, "PU SPI"      },
                          { R_PU_GPIO_BASEADDR,      0x00,       0x20,        0x0,         0xff, "PU GPIO"     },
                          { R_PU_TIMER_BASEADDR,     0x14,       0x20,        0x0,         0xff, "PU TIMER"    },
                          { R_PU_UART_BASEADDR,      0x1c,       0x20,        0x0,         0xff, "PU UART"  },
                          { R_PU_WDT_BASEADDR,       0x00,       0x20,        0xa,         0x2a, "PU WDT"   },
                          { R_PU_I3C_BASEADDR,       0x00,       0x20,        0x0,   0x80000000, "PU I3C"   },
                          { R_PU_UART1_BASEADDR,     0x1c,       0x20,        0x0,         0xff, "PU UART1" },
                          { R_PU_PLIC_BASEADDR,      0x04,       0x20,        0x0,         0x03, "PU PLIC"  },  // Accesible but readvalue is wrong
                          { R_PU_USB0_BASEADDR,      0x08,       0x20,        0x0,       0x07, "PU USB0"  },
                          { R_PU_USB1_BASEADDR,      0x08,       0x20,        0x0,       0x07, "PU USB1"  },
                          { R_PU_EMMC_CFG_BASEADDR,  0x34,       0x20,        0x0,         0x0f, "PU EMMC"}, // 2d is the intr status bit and is in hclk domain

                          { REGTEST_END_OF_LIST,     0x00,       0x20,        0x0,         0x0, "DUMMY"}
                        };
