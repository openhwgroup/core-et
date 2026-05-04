/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include <stdio.h>
//#include <algorithm>
#include "minion.h"
#include "et_test_common.h"
#include "tbox_test.h"
#include "utils.hpp"

constexpr int NUM_HARTS_PER_SHIRE = (NUM_MINIONS_PER_NEIGH*NUM_NEIGH_PER_SHIRE*NUM_HARTS_PER_MINION);

extern uint32_t texture[TEXTURE_SIZE];
extern volatile ImageInfo imgInfoTable[] __attribute__((aligned(32)));
extern tboxSendCoord_t texture_coordinates[];
extern unsigned first_coord_of_test[];
extern sample_request_t sample_request[];

extern unsigned getNumberOfSampleRequest();

volatile dump_storage_t dump_storage[NUM_HARTS_PER_SHIRE];

const uint64_t HARTS_WORKING = TBOX_HARTS; //Up to 16 (1 neigh)

/*
 * Run test
 */
bool run_test(uint64_t test_id, uint8_t port_idx, volatile cl_data_t* l1_data) {
  uint64_t tid;    
  SampleRequest header;
  tboxSendCoord_t* uvw_coords;
  uint64_t set;
  uint8_t sampler_request_packets;
  
  // Setup
  tid = get_thread_id();
  header.info = sample_request[test_id].info;
  
  uvw_coords  = &texture_coordinates[first_coord_of_test[test_id]];
  set = SETS_PER_PORT*port_idx+NR_PORTS*SETS_PER_PORT*tid;
  sampler_request_packets = sample_request->info.samplerRequestPackets;
  
  /* Step 2: Prepare arguments in local cache
     Software creates a texture request (see the ?Texture Request? section) and stores it at offset 0 of the storage associated to the port set up in step #1. 
     This is done using regular RISCV store instructions and does not involve any communication with the texture unit.
  */      
  /* DCACHE will send three packets of 256 bitts containing the parameter data */
  
  volatile unsigned char* ptr = l1_data->data[set];

  my_memcpy((uint8_t*) ptr,(uint8_t*) &header, sizeof(SampleRequest));
  ptr += sizeof(SampleRequest);
  my_memcpy((uint8_t*)ptr, (uint8_t*) uvw_coords, sizeof(tboxSendCoord_t)*sampler_request_packets);
  
  /* Step 3: Notify texture unit that request is ready using TEXSEND
     CSRRW  xd, xs, 0x807
     xs[63:8] = WARL (0)
     xs[7:4] = Texture request size in number of 256b packets. Minimum valid value is 2, maximum is 6.
     xs[3:0] = portid holding arguments and where results will be written. Maximum valid value is 3.
  */

  const int nr_packages_header = 1;
  const int nr_packages_payload = sample_request->info.samplerRequestPackets;
  int number_packets = nr_packages_header + nr_packages_payload; // number of packets for the sample request

  bool error = texsend(number_packets, port_idx);
  return error; 
}

/*
 * Initial setup before all the tests
 */
void setup_hart(volatile cl_data_t* l1_data) {
  
  uint64_t tid = get_thread_id();
  
  for(int port_idx = 0 ; port_idx < NR_PORTS; ++port_idx) {
    test_config_t test_conf;

    test_conf.log2size[port_idx]     = 5; // log2 of message size in B
    test_conf.way[port_idx]          = tid;
    test_conf.enable_oob[port_idx]   = 1;
    test_conf.maxMsgs[port_idx]      = 4; // reserve 4 messages of 256, because the TBOX returns 1024b

    /* Esperanto PRM: Texture Extension: Texture Protocol */

    /* Step 1: Setup a communication port
       CSRRW  x0, 0x8df, x1            // LockVA to reserve cache lines
       CSRRSI x0, PORTCTRL<portid>, 0  // PENABLE to associate cache lines to hart's port
    */
    constexpr bool user_mode  = false;
    uint64_t set = SETS_PER_PORT*port_idx+NR_PORTS*SETS_PER_PORT*tid;
    setup_rx_port(port_idx, 
		  user_mode,
		  test_conf.log2size[port_idx], 
		  test_conf.maxMsgs[port_idx], 
		  (void *) l1_data->data[set], 
		  test_conf.way[port_idx],
		  test_conf.enable_oob[port_idx]);    // Step 1
  }

}


void store_sample(uint64_t test_idx, volatile uint8_t* sample_addr){
  dump_storage[test_idx].success = true;
  for(size_t i = 0; TBOX_RSP_TOTAL_SIZE; ++i) {
    dump_storage[test_idx].rsp_data[i] = sample_addr[i];
  }
}

void dump_samples() {
  
  clear_l1d(); // flush all l1

  const uint64_t NUMBER_OF_TESTS = getNumberOfSampleRequest();
  if (get_hart_id() == 0) {
    /*
     * wait the other minions dump the samples to L2
     */
    for(uint64_t i = 0; i < HARTS_WORKING-1; ++i)
      wait_for_credit(0);   
    
    for(uint64_t test = 0; test < NUMBER_OF_TESTS; ++test) {
      char test_num[sizeof(uint64_t)+1];
      itoa(test, test_num, 10);
      et_printf("TBOX_SAMPLE: ");
      et_printf(test_num);
      et_printf(" data: ");
      for(uint64_t i = 0 ; i < TBOX_RSP_TOTAL_SIZE; ++i) {
	et_putchar(dump_storage[test].rsp_data[i]);
      }
      et_printf("\n\n");
    } 
  } else {
    give_credit(0,0,0,0);
  }
  
}

void wait_tests(port_used_t* port_used, int* nr_port_used, volatile cl_data_t* data) {
  uint64_t tid = get_thread_id();
  for(uint8_t port_idx = 0; port_idx < NR_PORTS; ++port_idx) {
    if (port_used[port_idx].used) {
      uint64_t offset0;
      offset0 = read_port_status(port_idx);
      read_port_status(port_idx);
      read_port_status(port_idx);
      read_port_status(port_idx);
      
      port_used[port_idx].used = false;
      *nr_port_used=(*nr_port_used)-1;
      
      /*
       * clear oob from the offset
       * ignore the value. The RTL  always sends 1
       */
      offset0 = offset0 & ~((uint64_t)0x1);

      
      uint64_t set = SETS_PER_PORT*port_idx+NR_PORTS*SETS_PER_PORT*tid;
      volatile uint8_t* sample_addr = (volatile uint8_t*) data->data[set]+offset0;
      store_sample(port_used[port_idx].test, sample_addr);

    }
  }
}

void clear_data(volatile cl_data* l1_data) {
  for(uint64_t i = 0; i < L1_SETS; ++i) {
    volatile uint32_t* data = (volatile uint32_t*) l1_data->data[i];
    for(uint64_t j = 0 ; j < L1_CL_SIZE/sizeof(uint32_t); ++j)
      data[i] = 0xdeadbeaf;    
  }
}

int8_t get_free_port(port_used_t* port_used) {
  for(uint8_t i = 0; i < NR_PORTS; ++i) {
    if(!port_used[i].used)
      return i;
  }
  return -1;
}

void flush_texture() {
  
  if (get_hart_id() == 0) {
    volatile uint64_t *imgInfoTableBasePtr = (uint64_t*) TBOX_IMG_TABLE_BASE_PTR;
    imgInfoTable[0].address    =  (uint64_t) texture;
    *imgInfoTableBasePtr       =  (uintptr_t) imgInfoTable;
    flush((void*)imgInfoTable, sizeof(ImageInfo));    
    give_credit_local(-1, 0, 0); 
    give_credit_local(-1, 1, 0);
    FENCE
    wait_for_credits(0);
  } else {
    wait_for_credits(0);
  }

}


volatile cl_data_t ports_buffer[NUM_HARTS_PER_SHIRE] __attribute__((aligned(L1_CL_SIZE)));

int main() {
  
  C_TEST_START;
  
  
  bool error;    
  const uint64_t NUMBER_OF_TESTS = getNumberOfSampleRequest();

  volatile cl_data_t* l1_data;
  
  uint64_t hid = get_hart_id();
  l1_data = &ports_buffer[hid];
  
  if (hid >= HARTS_WORKING)
    WFI
  
  clear_data(l1_data);
  
  /*
   * Flush the texture to L2
   */ 
  flush_texture();
  
  setup_hart(l1_data);
  
  uint64_t start, end;
  start = hid;
  end = NUMBER_OF_TESTS;

  int nr_port_used = 0;
  port_used_t port_used[NR_PORTS] = {{-1,false}};
  
  uint64_t test_idx;
  for(test_idx = start; test_idx < end; test_idx+=HARTS_WORKING) 
  {
    int8_t port = get_free_port(port_used);
    
    error = run_test(test_idx,port, l1_data);
    if(error)
      C_TEST_FAIL;
	
    ++nr_port_used;
    port_used[port].used = true;
    port_used[port].test = test_idx;
    
    /*
     * Wait until the tbox response when we have all the ports busy
     */
    if(nr_port_used == NR_PORTS)
      wait_tests(port_used, &nr_port_used, l1_data);      
  }
  
  /*
   * Wait pending messages
   */
  wait_tests(port_used, &nr_port_used, l1_data);
  
  dump_samples();
  
  C_TEST_PASS;      
  
}


