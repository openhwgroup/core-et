/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

/*

Skip to content
Using Esperanto Technologies Mail with screen readers

1 of 237
File
Inbox
x

nikola.gacic@esperantotech.com
Attachments
11:09 AM (0 minutes ago)
to me

 

Attachments area
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// file: cc_boot_seq.tcl
//
// This file is used to initialize the Synopsys Memory Controller IP
//
//  ===>>> this version does not include DDR interface training <<<===
//
// The general flow is:
// 1) Turn off the APB reset and set Power OK by writing to the the ddrc_reset_ctl (a memshire/DDRC ESR)
// 2) Configure the memory controller main blocks by writing to various registers in both controllers
// 3) Turn off the core and AXI resets by writing to the ddrc_reset_ctl 
// 4) Turn off the PUB reset by writing to the ddrc_reset_ctl 
// 5) Configure the PUB (PHY) by writing to various PUB registers 
// 6) Wait for PHY init to complete by polling a memory controller register
// 7) finalize initialzation  by writing a few memory controller registers
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Key:
//
// write_esr  <register_name> <value> : write an esr in the memshire
// read_esr  <register_name>          : read an esr in the memshire
//
// write_both_ddrc_reg <reg> <value>  : write a value to a memory controller register in both memory controllers
// write_ddrc_reg <blk> <reg> <value> : write a value to particular memory controller block:  
//                                    : 0 = mem controller 0, 1 = mem controller 1, 2 = PUB (PHY)
// read_ddrc_reg <blk> <reg>          : read a value to particular memory controller block:  
//                                    : 0 = mem controller 0, 1 = mem controller 1, 2 = PUB (PHY)
//
// poll_ddrc_reg <blk> <reg> <value> <mask> <timeout> : continues to issue read_ddrc_reg <blk> <reg> until
//                                                    :   (returned value & <mask>) == <value>.
//                                                    : If this match is not acheived after <timeout> tries, a dut_error is flagged
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint64_t set_address(uint32_t memshire_id, uint64_t address)
{
   if (address & 0x80000000)
      return (address | ((0xe8 + memshire_id) << 22));      
   else 
      return (address | (memshire_id << 26) );
}
void write_reg(uint32_t memshire_id, uint64_t address, uint32_t value){
   uint64_t *addr64;
   uint32_t *addr32;

   if (address & 0x80000000){
      addr64 =(uint64_t*)set_address(memshire_id, address);
      *addr64 = value;
   }
   else{
      addr32 =(uint32_t*)set_address(memshire_id, address);
      *addr32 = value;
   }
}
uint32_t read_reg(uint32_t memshire_id, uint64_t address){
   uint64_t *addr64;
   uint32_t *addr32;
   
   if (address & 0x80000000){
      addr64 =(uint64_t*)set_address(memshire_id, address);
      return (*addr64);
   }
   else{ 
      addr32 =(uint32_t*)set_address(memshire_id, address);
      return (*addr32);
   }
}

void ddr_init(uint32_t memshire_id ){
//////////////////////////////////////////////////////////////////////////
// set power-ok and turn off apb reset
// keep other resets asserted
/////////////////////////////////////////////////////////////////////
uint32_t value_read = 0;

write_reg( memshire_id, 0x80800200, 0x0000000d);
do{value_read = read_reg( memshire_id, 0x80800200);}
  while(!((value_read & 0xffffffff) == 0x00000000d));

//////////////////////////////////////////////////////////////////////
// start to initialze the memory controllers
//////////////////////////////////////////////////////////////////////
write_reg( memshire_id, 0x60000304, 0x00000001);
write_reg( memshire_id, 0x60001304, 0x00000001);
write_reg( memshire_id, 0x60000030, 0x00000001);
write_reg( memshire_id, 0x60001030, 0x00000001);

// make sure operating mode is Init 
do{value_read = read_reg( memshire_id, 0x60000004);}
  while(!((value_read & 0x7) == 0));
do{value_read = read_reg( memshire_id, 0x60001004);}
  while(!((value_read & 0x7) == 0));

//////////////////////////////////////////////////////////////////////
// configure the memory controllers
//////////////////////////////////////////////////////////////////////
write_reg( memshire_id, 0x60000000, 0x80080020);
write_reg( memshire_id, 0x60001000, 0x80080020);
write_reg( memshire_id, 0x60000010, 0x0000a010);
write_reg( memshire_id, 0x60001010, 0x0000a010);
write_reg( memshire_id, 0x60000014, 0x0001008c);
write_reg( memshire_id, 0x60001014, 0x0001008c);
write_reg( memshire_id, 0x60000020, 0x00000404);
write_reg( memshire_id, 0x60001020, 0x00000404);
write_reg( memshire_id, 0x60000024, 0xc0c188dd);
write_reg( memshire_id, 0x60001024, 0xc0c188dd);
write_reg( memshire_id, 0x6000002c, 0x00000000);
write_reg( memshire_id, 0x6000102c, 0x00000000);
write_reg( memshire_id, 0x60000030, 0x00000000);
write_reg( memshire_id, 0x60001030, 0x00000000);
write_reg( memshire_id, 0x60000034, 0x0040d104);
write_reg( memshire_id, 0x60001034, 0x0040d104);
write_reg( memshire_id, 0x60000038, 0x00af0002);
write_reg( memshire_id, 0x60001038, 0x00af0002);
write_reg( memshire_id, 0x60000050, 0x00210000);
write_reg( memshire_id, 0x60001050, 0x00210000);
write_reg( memshire_id, 0x60000060, 0x00000001);
write_reg( memshire_id, 0x60001060, 0x00000001);
write_reg( memshire_id, 0x60000064, 0x0082008c);
write_reg( memshire_id, 0x60001064, 0x0082008c);
write_reg( memshire_id, 0x60000068, 0x00410000);
write_reg( memshire_id, 0x60001068, 0x00410000);
write_reg( memshire_id, 0x60000070, 0x003f7f10);
write_reg( memshire_id, 0x60001070, 0x003f7f10);
write_reg( memshire_id, 0x60000074, 0x000007b2);
write_reg( memshire_id, 0x60001074, 0x000007b2);
write_reg( memshire_id, 0x6000007c, 0x00000300);
write_reg( memshire_id, 0x6000107c, 0x00000300);
write_reg( memshire_id, 0x600000b8, 0x000002b4);
write_reg( memshire_id, 0x600010b8, 0x000002b4);
write_reg( memshire_id, 0x600000bc, 0x0001df60);
write_reg( memshire_id, 0x600010bc, 0x0001df60);
write_reg( memshire_id, 0x600000c0, 0x00000000);
write_reg( memshire_id, 0x600010c0, 0x00000000);
write_reg( memshire_id, 0x600000d0, 0x00020002);
write_reg( memshire_id, 0x600010d0, 0x00020002);
write_reg( memshire_id, 0x600000d4, 0x00020002);
write_reg( memshire_id, 0x600010d4, 0x00020002);
write_reg( memshire_id, 0x600000d8, 0x0000f405);
write_reg( memshire_id, 0x600010d8, 0x0000f405);
write_reg( memshire_id, 0x600000dc, 0x0074007f);
write_reg( memshire_id, 0x600010dc, 0x0074007f);
write_reg( memshire_id, 0x600000e0, 0x00330000);
write_reg( memshire_id, 0x600010e0, 0x00330000);
write_reg( memshire_id, 0x600000e4, 0x0005000c);
write_reg( memshire_id, 0x600010e4, 0x0005000c);
write_reg( memshire_id, 0x600000e8, 0x0000004d);
write_reg( memshire_id, 0x600010e8, 0x0000004d);
write_reg( memshire_id, 0x600000ec, 0x0000004d);
write_reg( memshire_id, 0x600010ec, 0x0000004d);
write_reg( memshire_id, 0x600000f0, 0x00000000);
write_reg( memshire_id, 0x600010f0, 0x00000000);
write_reg( memshire_id, 0x60000100, 0x2921242d);
write_reg( memshire_id, 0x60001100, 0x2921242d);
write_reg( memshire_id, 0x60000104, 0x00090901);
write_reg( memshire_id, 0x60001104, 0x00090901);
write_reg( memshire_id, 0x60000108, 0x11120a21);
write_reg( memshire_id, 0x60001108, 0x11120a21);
write_reg( memshire_id, 0x6000010c, 0x00f0f000);
write_reg( memshire_id, 0x6000110c, 0x00f0f000);
write_reg( memshire_id, 0x60000110, 0x14040914);
write_reg( memshire_id, 0x60001110, 0x14040914);
write_reg( memshire_id, 0x60000114, 0x02061111);
write_reg( memshire_id, 0x60001114, 0x02061111);
write_reg( memshire_id, 0x60000118, 0x0101000a);
write_reg( memshire_id, 0x60001118, 0x0101000a);
write_reg( memshire_id, 0x6000011c, 0x00000602);
write_reg( memshire_id, 0x6000111c, 0x00000602);
write_reg( memshire_id, 0x60000120, 0x00000101);
write_reg( memshire_id, 0x60001120, 0x00000101);
write_reg( memshire_id, 0x60000130, 0x00020000);
write_reg( memshire_id, 0x60001130, 0x00020000);
write_reg( memshire_id, 0x60000134, 0x16100002);
write_reg( memshire_id, 0x60001134, 0x16100002);
write_reg( memshire_id, 0x60000138, 0x00000093);
write_reg( memshire_id, 0x60001138, 0x00000093);
write_reg( memshire_id, 0x60000180, 0xc42f0021);
write_reg( memshire_id, 0x60001180, 0xc42f0021);
write_reg( memshire_id, 0x60000184, 0x036a3055);
write_reg( memshire_id, 0x60001184, 0x036a3055);
write_reg( memshire_id, 0x60000188, 0x00000000);
write_reg( memshire_id, 0x60001188, 0x00000000);
write_reg( memshire_id, 0x60000190, 0x049f821e);
write_reg( memshire_id, 0x60001190, 0x049f821e);
write_reg( memshire_id, 0x60000194, 0x00090303);
write_reg( memshire_id, 0x60001194, 0x00090303);
write_reg( memshire_id, 0x60000198, 0x0321b010);
write_reg( memshire_id, 0x60001198, 0x0321b010);
write_reg( memshire_id, 0x600001a0, 0x80400018);
write_reg( memshire_id, 0x600011a0, 0x80400018);
write_reg( memshire_id, 0x600001a4, 0x003c00b1);
write_reg( memshire_id, 0x600011a4, 0x003c00b1);
write_reg( memshire_id, 0x600001a8, 0x80000000);
write_reg( memshire_id, 0x600011a8, 0x80000000);
write_reg( memshire_id, 0x600001b0, 0x00000081);
write_reg( memshire_id, 0x600011b0, 0x00000081);
write_reg( memshire_id, 0x600001b4, 0x00001f1e);
write_reg( memshire_id, 0x600011b4, 0x00001f1e);
write_reg( memshire_id, 0x600001c0, 0x00000001);
write_reg( memshire_id, 0x600011c0, 0x00000001);
write_reg( memshire_id, 0x600001c4, 0x00000000);
write_reg( memshire_id, 0x600011c4, 0x00000000);
write_reg( memshire_id, 0x60000204, 0x00030303);
write_reg( memshire_id, 0x60001204, 0x00030303);
write_reg( memshire_id, 0x60000208, 0x03000000);
write_reg( memshire_id, 0x60001208, 0x03000000);
write_reg( memshire_id, 0x60000210, 0x00001f1f);
write_reg( memshire_id, 0x60001210, 0x00001f1f);
write_reg( memshire_id, 0x6000020c, 0x03030303);
write_reg( memshire_id, 0x6000120c, 0x03030303);
write_reg( memshire_id, 0x60000214, 0x07070707);
write_reg( memshire_id, 0x60001214, 0x07070707);
write_reg( memshire_id, 0x60000218, 0x07070707);
write_reg( memshire_id, 0x60001218, 0x07070707);
write_reg( memshire_id, 0x6000021c, 0x00000f07);
write_reg( memshire_id, 0x6000121c, 0x00000f07);
//write_reg( memshire_id, 0x60000220, 0x00003f3f);
//write_reg( memshire_id, 0x60001220, 0x00003f3f);
write_reg( memshire_id, 0x60000224, 0x07070707);
write_reg( memshire_id, 0x60001224, 0x07070707);
write_reg( memshire_id, 0x60000228, 0x07070707);
write_reg( memshire_id, 0x60001228, 0x07070707);
write_reg( memshire_id, 0x6000022c, 0x001f1f07);
write_reg( memshire_id, 0x6000122c, 0x001f1f07);
write_reg( memshire_id, 0x60000240, 0x061a0c1c);
write_reg( memshire_id, 0x60001240, 0x061a0c1c);
write_reg( memshire_id, 0x60000244, 0x00000000);
write_reg( memshire_id, 0x60001244, 0x00000000);
write_reg( memshire_id, 0x60000250, 0x00a01f01);
write_reg( memshire_id, 0x60001250, 0x00a01f01);
write_reg( memshire_id, 0x60000254, 0x00000000);
write_reg( memshire_id, 0x60001254, 0x00000000);
write_reg( memshire_id, 0x6000025c, 0x0f000001);
write_reg( memshire_id, 0x6000125c, 0x0f000001);
write_reg( memshire_id, 0x60000264, 0x0f00007f);
write_reg( memshire_id, 0x60001264, 0x0f00007f);
write_reg( memshire_id, 0x6000026c, 0x0f00007f);
write_reg( memshire_id, 0x6000126c, 0x0f00007f);
write_reg( memshire_id, 0x60000300, 0x00000000);
write_reg( memshire_id, 0x60001300, 0x00000000);
write_reg( memshire_id, 0x60000304, 0x00000000);
write_reg( memshire_id, 0x60001304, 0x00000000);
write_reg( memshire_id, 0x6000030c, 0x00000000);
write_reg( memshire_id, 0x6000130c, 0x00000000);
write_reg( memshire_id, 0x60000320, 0x00000001);
write_reg( memshire_id, 0x60001320, 0x00000001);
write_reg( memshire_id, 0x6000036c, 0x00000010);
write_reg( memshire_id, 0x6000136c, 0x00000010);
write_reg( memshire_id, 0x60000374, 0x000001d2);
write_reg( memshire_id, 0x60001374, 0x000001d2);
write_reg( memshire_id, 0x6000037c, 0x00000000);
write_reg( memshire_id, 0x6000137c, 0x00000000);
write_reg( memshire_id, 0x60000384, 0x00000000);
write_reg( memshire_id, 0x60001384, 0x00000000);
write_reg( memshire_id, 0x60000490, 0x00000001);
write_reg( memshire_id, 0x60001490, 0x00000001);
write_reg( memshire_id, 0x60000540, 0x00000001);
write_reg( memshire_id, 0x60001540, 0x00000001);

//////////////////////////////////////////////////////////////////////
// make sure STAT == 0
//////////////////////////////////////////////////////////////////////
do{value_read = read_reg( memshire_id, 0x60000004);}
  while(!((value_read & 0xffffffff) == 0));
do{value_read = read_reg( memshire_id, 0x60001004);}
  while(!((value_read & 0xffffffff) == 0));

write_reg( memshire_id, 0x60000400, 0x00000000);
write_reg( memshire_id, 0x60001400, 0x00000000);
write_reg( memshire_id, 0x60000404, 0x0000400f);
write_reg( memshire_id, 0x60001404, 0x0000400f);
write_reg( memshire_id, 0x600004b4, 0x0000400f);
write_reg( memshire_id, 0x600014b4, 0x0000400f);
write_reg( memshire_id, 0x60000404, 0x0000500f);
write_reg( memshire_id, 0x60001404, 0x0000500f);
write_reg( memshire_id, 0x600004b4, 0x0000500f);
write_reg( memshire_id, 0x600014b4, 0x0000500f);
write_reg( memshire_id, 0x60000404, 0x0000500f);
write_reg( memshire_id, 0x60001404, 0x0000500f);
write_reg( memshire_id, 0x600004b4, 0x0000500f);
write_reg( memshire_id, 0x600014b4, 0x0000500f);
write_reg( memshire_id, 0x60000404, 0x0000100f);
write_reg( memshire_id, 0x60001404, 0x0000100f);
write_reg( memshire_id, 0x600004b4, 0x0000100f);
write_reg( memshire_id, 0x600014b4, 0x0000100f);
write_reg( memshire_id, 0x60000408, 0x0000400f);
write_reg( memshire_id, 0x60001408, 0x0000400f);
write_reg( memshire_id, 0x600004b8, 0x0000400f);
write_reg( memshire_id, 0x600014b8, 0x0000400f);
write_reg( memshire_id, 0x60000408, 0x0000500f);
write_reg( memshire_id, 0x60001408, 0x0000500f);
write_reg( memshire_id, 0x600004b8, 0x0000500f);
write_reg( memshire_id, 0x600014b8, 0x0000500f);
write_reg( memshire_id, 0x60000408, 0x0000500f);
write_reg( memshire_id, 0x60001408, 0x0000500f);
write_reg( memshire_id, 0x600004b8, 0x0000500f);
write_reg( memshire_id, 0x600014b8, 0x0000500f);
write_reg( memshire_id, 0x60000408, 0x0000100f);
write_reg( memshire_id, 0x60001408, 0x0000100f);
write_reg( memshire_id, 0x600004b8, 0x0000100f);
write_reg( memshire_id, 0x600014b8, 0x0000100f);
write_reg( memshire_id, 0x60000304, 0x00000000);
write_reg( memshire_id, 0x60001304, 0x00000000);

////////////////////////////////////////////////////////////////////////////////
// make power control reg can be written
// FIXME tberg: is this needed?
////////////////////////////////////////////////////////////////////////////////
do{value_read = read_reg( memshire_id, 0x60000030);}
  while(!((value_read & 0x1ff) == 0));
do{value_read = read_reg( memshire_id, 0x60000030);}
  while(!((value_read & 0x1ff) == 0));

write_reg( memshire_id, 0x60000030, 0x00000000);
write_reg( memshire_id, 0x60001030, 0x00000000);
do{value_read = read_reg( memshire_id, 0x60000030);}
  while(!((value_read & 0x1ff) == 0));
do{value_read = read_reg( memshire_id, 0x60000030);}
  while(!((value_read & 0x1ff) == 0));

write_reg( memshire_id, 0x60000030, 0x00000000);
write_reg( memshire_id, 0x60001030, 0x00000000);
write_reg( memshire_id, 0x60000320, 0x00000000);
write_reg( memshire_id, 0x60001320, 0x00000000);
write_reg( memshire_id, 0x600001b0, 0x00000080);
write_reg( memshire_id, 0x600011b0, 0x00000080);
write_reg( memshire_id, 0x600001b0, 0x00001080);
write_reg( memshire_id, 0x600011b0, 0x00001080);

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// These are probably optional, but good to make sure 
// the memory controllers are in the expected state
////////////////////////////////////////////////////////////////////////////////////////////////////////////
do{value_read = read_reg( memshire_id, 0x600000d0);}
  while(!((value_read & 0xffffffff) == 0x00020002));
do{value_read = read_reg( memshire_id, 0x600001c0);}
  while(!((value_read & 0xffffffff) == 0x00000001));
do{value_read = read_reg( memshire_id, 0x60000000);}
  while(!((value_read & 0xffffffff) == 0x00080020));
do{value_read = read_reg( memshire_id, 0x600000dc);}
  while(!((value_read & 0xffffffff) == 0x0074007f));
do{value_read = read_reg( memshire_id, 0x600000e0);}
  while(!((value_read & 0xffffffff) == 0x00330000));
do{value_read = read_reg( memshire_id, 0x600000e8);}
  while(!((value_read & 0xffffffff) == 0x0000004d));

do{value_read = read_reg( memshire_id, 0x600010d0);}
  while(!((value_read & 0xffffffff) == 0x00020002));
do{value_read = read_reg( memshire_id, 0x600011c0);}
  while(!((value_read & 0xffffffff) == 0x00000001));
do{value_read = read_reg( memshire_id, 0x60001000);}
  while(!((value_read & 0xffffffff) == 0x00080020));
do{value_read = read_reg( memshire_id, 0x600010dc);}
  while(!((value_read & 0xffffffff) == 0x0074007f));
do{value_read = read_reg( memshire_id, 0x600010e0);}
  while(!((value_read & 0xffffffff) == 0x00330000));
do{value_read = read_reg( memshire_id, 0x600010e8);}
  while(!((value_read & 0xffffffff) == 0x0000004d));

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// turn off core and axi resets
////////////////////////////////////////////////////////////////////////////////////////////////////////////
write_reg( memshire_id, 0x80800200, 0x0000000c);
do{value_read = read_reg( memshire_id, 0x80800200);}
  while(!((value_read & 0xffffffff) == 0x0000000c));

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// turn off pub reset
////////////////////////////////////////////////////////////////////////////////////////////////////////////
write_reg( memshire_id, 0x80800200, 0x00000008);
do{value_read = read_reg( memshire_id, 0x80800200);}
  while(!((value_read & 0xffffffff) == 0x00000008));

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Execute routine to initialize the phy 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//set my_repo_root [get_env REPOROOT]
//set procs_file "${my_repo_root}/dv/tests/memshire/tcl_tests/tcl_scripts/lib/phy_init.tcl"
write_reg( memshire_id, 0x6204017c, 0x000005ff);
write_reg( memshire_id, 0x6204057c, 0x000005ff);
write_reg( memshire_id, 0x6204417c, 0x000005ff);
write_reg( memshire_id, 0x6204457c, 0x000005ff);
write_reg( memshire_id, 0x6204817c, 0x000005ff);
write_reg( memshire_id, 0x6204857c, 0x000005ff);
write_reg( memshire_id, 0x6204c17c, 0x000005ff);
write_reg( memshire_id, 0x6204c57c, 0x000005ff);
write_reg( memshire_id, 0x62000154, 0x000001ff);
write_reg( memshire_id, 0x62004154, 0x000001ff);
write_reg( memshire_id, 0x62008154, 0x000001ff);
write_reg( memshire_id, 0x6200c154, 0x000001ff);
write_reg( memshire_id, 0x62010154, 0x000001ff);
write_reg( memshire_id, 0x62014154, 0x000001ff);
write_reg( memshire_id, 0x62080314, 0x00000019);
write_reg( memshire_id, 0x620800b8, 0x00000002);
write_reg( memshire_id, 0x62240810, 0x00000000);
write_reg( memshire_id, 0x62080090, 0x000001e3);
write_reg( memshire_id, 0x620800e8, 0x00000002);
write_reg( memshire_id, 0x620801f4, 0x00000212);
write_reg( memshire_id, 0x620801f0, 0x00000061);
write_reg( memshire_id, 0x62080158, 0x00000003);
write_reg( memshire_id, 0x62040134, 0x00000600);
write_reg( memshire_id, 0x62040534, 0x00000600);
write_reg( memshire_id, 0x62044134, 0x00000600);
write_reg( memshire_id, 0x62044534, 0x00000600);
write_reg( memshire_id, 0x62048134, 0x00000600);
write_reg( memshire_id, 0x62048534, 0x00000600);
write_reg( memshire_id, 0x6204c134, 0x00000600);
write_reg( memshire_id, 0x6204c534, 0x00000600);
write_reg( memshire_id, 0x62040124, 0x00000618);
write_reg( memshire_id, 0x62040524, 0x00000618);
write_reg( memshire_id, 0x62044124, 0x00000618);
write_reg( memshire_id, 0x62044524, 0x00000618);
write_reg( memshire_id, 0x62048124, 0x00000618);
write_reg( memshire_id, 0x62048524, 0x00000618);
write_reg( memshire_id, 0x6204c124, 0x00000618);
write_reg( memshire_id, 0x6204c524, 0x00000618);
write_reg( memshire_id, 0x6200010c, 0x000003ff);
write_reg( memshire_id, 0x6200410c, 0x000003ff);
write_reg( memshire_id, 0x6200810c, 0x000003ff);
write_reg( memshire_id, 0x6200c10c, 0x000003ff);
write_reg( memshire_id, 0x6201010c, 0x000003ff);
write_reg( memshire_id, 0x6201410c, 0x000003ff);
write_reg( memshire_id, 0x62080060, 0x00000003);
write_reg( memshire_id, 0x620801d4, 0x00000004);
write_reg( memshire_id, 0x62080140, 0x00000000);
write_reg( memshire_id, 0x62080020, 0x0000042b);
write_reg( memshire_id, 0x62080220, 0x00000009);
write_reg( memshire_id, 0x620802c8, 0x00000104);
write_reg( memshire_id, 0x6204010c, 0x000005a1);
write_reg( memshire_id, 0x6204050c, 0x000005a1);
write_reg( memshire_id, 0x6204410c, 0x000005a1);
write_reg( memshire_id, 0x6204450c, 0x000005a1);
write_reg( memshire_id, 0x6204810c, 0x000005a1);
write_reg( memshire_id, 0x6204850c, 0x000005a1);
write_reg( memshire_id, 0x6204c10c, 0x000005a1);
write_reg( memshire_id, 0x6204c50c, 0x000005a1);
write_reg( memshire_id, 0x620803e8, 0x00000001);
write_reg( memshire_id, 0x62080064, 0x00000001);
write_reg( memshire_id, 0x620803c0, 0x00000000);
write_reg( memshire_id, 0x620803c4, 0x00000000);
write_reg( memshire_id, 0x620803c8, 0x00004444);
write_reg( memshire_id, 0x620803cc, 0x00008888);
write_reg( memshire_id, 0x620803d0, 0x00005555);
write_reg( memshire_id, 0x620803d4, 0x00000000);
write_reg( memshire_id, 0x620803d8, 0x00000000);
write_reg( memshire_id, 0x620803dc, 0x0000f000);
write_reg( memshire_id, 0x62040128, 0x00000500);
write_reg( memshire_id, 0x62044128, 0x00000500);
write_reg( memshire_id, 0x62048128, 0x00000500);
write_reg( memshire_id, 0x6204c128, 0x00000500);
write_reg( memshire_id, 0x62080094, 0x00000000);
write_reg( memshire_id, 0x620800b4, 0x00000000);
write_reg( memshire_id, 0x620800b0, 0x00000000);
write_reg( memshire_id, 0x62040080, 0x00000008);
write_reg( memshire_id, 0x62044080, 0x00000008);
write_reg( memshire_id, 0x62048080, 0x00000008);
write_reg( memshire_id, 0x6204c080, 0x00000008);
write_reg( memshire_id, 0x62080080, 0x00000008);
write_reg( memshire_id, 0x62040340, 0x00000100);
write_reg( memshire_id, 0x62040740, 0x00000100);
write_reg( memshire_id, 0x62044340, 0x00000100);
write_reg( memshire_id, 0x62044740, 0x00000100);
write_reg( memshire_id, 0x62048340, 0x00000100);
write_reg( memshire_id, 0x62048740, 0x00000100);
write_reg( memshire_id, 0x6204c340, 0x00000100);
write_reg( memshire_id, 0x6204c740, 0x00000100);
write_reg( memshire_id, 0x62040300, 0x00000087);
write_reg( memshire_id, 0x62040700, 0x00000087);
write_reg( memshire_id, 0x62040b00, 0x00000087);
write_reg( memshire_id, 0x62040f00, 0x00000087);
write_reg( memshire_id, 0x62041300, 0x00000087);
write_reg( memshire_id, 0x62041700, 0x00000087);
write_reg( memshire_id, 0x62041b00, 0x00000087);
write_reg( memshire_id, 0x62041f00, 0x00000087);
write_reg( memshire_id, 0x62042300, 0x00000087);
write_reg( memshire_id, 0x62044300, 0x00000087);
write_reg( memshire_id, 0x62044700, 0x00000087);
write_reg( memshire_id, 0x62044b00, 0x00000087);
write_reg( memshire_id, 0x62044f00, 0x00000087);
write_reg( memshire_id, 0x62045300, 0x00000087);
write_reg( memshire_id, 0x62045700, 0x00000087);
write_reg( memshire_id, 0x62045b00, 0x00000087);
write_reg( memshire_id, 0x62045f00, 0x00000087);
write_reg( memshire_id, 0x62046300, 0x00000087);
write_reg( memshire_id, 0x62048300, 0x00000087);
write_reg( memshire_id, 0x62048700, 0x00000087);
write_reg( memshire_id, 0x62048b00, 0x00000087);
write_reg( memshire_id, 0x62048f00, 0x00000087);
write_reg( memshire_id, 0x62049300, 0x00000087);
write_reg( memshire_id, 0x62049700, 0x00000087);
write_reg( memshire_id, 0x62049b00, 0x00000087);
write_reg( memshire_id, 0x62049f00, 0x00000087);
write_reg( memshire_id, 0x6204a300, 0x00000087);
write_reg( memshire_id, 0x6204c300, 0x00000087);
write_reg( memshire_id, 0x6204c700, 0x00000087);
write_reg( memshire_id, 0x6204cb00, 0x00000087);
write_reg( memshire_id, 0x6204cf00, 0x00000087);
write_reg( memshire_id, 0x6204d300, 0x00000087);
write_reg( memshire_id, 0x6204d700, 0x00000087);
write_reg( memshire_id, 0x6204db00, 0x00000087);
write_reg( memshire_id, 0x6204df00, 0x00000087);
write_reg( memshire_id, 0x6204e300, 0x00000087);
write_reg( memshire_id, 0x62040200, 0x000004d6);
write_reg( memshire_id, 0x62040600, 0x000004d6);
write_reg( memshire_id, 0x62044200, 0x000004d6);
write_reg( memshire_id, 0x62044600, 0x000004d6);
write_reg( memshire_id, 0x62048200, 0x000004d6);
write_reg( memshire_id, 0x62048600, 0x000004d6);
write_reg( memshire_id, 0x6204c200, 0x000004d6);
write_reg( memshire_id, 0x6204c600, 0x000004d6);
write_reg( memshire_id, 0x62240804, 0x00002200);
write_reg( memshire_id, 0x62240808, 0x00000000);
write_reg( memshire_id, 0x6224080c, 0x00002e00);
write_reg( memshire_id, 0x620801c8, 0x00000001);
write_reg( memshire_id, 0x620801cc, 0x00000001);
write_reg( memshire_id, 0x620402b8, 0x00000037);
write_reg( memshire_id, 0x620442b8, 0x00000037);
write_reg( memshire_id, 0x620482b8, 0x00000037);
write_reg( memshire_id, 0x6204c2b8, 0x00000037);
write_reg( memshire_id, 0x620402bc, 0x00000037);
write_reg( memshire_id, 0x620442bc, 0x00000037);
write_reg( memshire_id, 0x620482bc, 0x00000037);
write_reg( memshire_id, 0x6204c2bc, 0x00000037);
write_reg( memshire_id, 0x620402a8, 0x00000501);
write_reg( memshire_id, 0x620442a8, 0x0000050d);
write_reg( memshire_id, 0x620482a8, 0x00000501);
write_reg( memshire_id, 0x6204c2a8, 0x0000050d);
write_reg( memshire_id, 0x620801dc, 0x00000034);
write_reg( memshire_id, 0x620801f0, 0x00000054);
write_reg( memshire_id, 0x620801f4, 0x000002f2);
write_reg( memshire_id, 0x62100300, 0x0000010f);
write_reg( memshire_id, 0x6208032c, 0x000061f0);
write_reg( memshire_id, 0x622400a0, 0x00000000);
write_reg( memshire_id, 0x62340000, 0x00000000);
write_reg( memshire_id, 0x62240000, 0x00000010);
write_reg( memshire_id, 0x62240004, 0x00000400);
write_reg( memshire_id, 0x62240008, 0x0000010e);
write_reg( memshire_id, 0x6224000c, 0x00000000);
write_reg( memshire_id, 0x62240010, 0x00000000);
write_reg( memshire_id, 0x62240014, 0x00000008);
write_reg( memshire_id, 0x622400a4, 0x0000000b);
write_reg( memshire_id, 0x622400a8, 0x00000480);
write_reg( memshire_id, 0x622400ac, 0x00000109);
write_reg( memshire_id, 0x622400b0, 0x00000008);
write_reg( memshire_id, 0x622400b4, 0x00000448);
write_reg( memshire_id, 0x622400b8, 0x00000139);
write_reg( memshire_id, 0x622400bc, 0x00000008);
write_reg( memshire_id, 0x622400c0, 0x00000478);
write_reg( memshire_id, 0x622400c4, 0x00000109);
write_reg( memshire_id, 0x622400c8, 0x00000000);
write_reg( memshire_id, 0x622400cc, 0x000000e8);
write_reg( memshire_id, 0x622400d0, 0x00000109);
write_reg( memshire_id, 0x622400d4, 0x00000002);
write_reg( memshire_id, 0x622400d8, 0x00000010);
write_reg( memshire_id, 0x622400dc, 0x00000139);
write_reg( memshire_id, 0x622400e0, 0x0000000b);
write_reg( memshire_id, 0x622400e4, 0x000007c0);
write_reg( memshire_id, 0x622400e8, 0x00000139);
write_reg( memshire_id, 0x622400ec, 0x00000044);
write_reg( memshire_id, 0x622400f0, 0x00000633);
write_reg( memshire_id, 0x622400f4, 0x00000159);
write_reg( memshire_id, 0x622400f8, 0x0000014f);
write_reg( memshire_id, 0x622400fc, 0x00000630);
write_reg( memshire_id, 0x62240100, 0x00000159);
write_reg( memshire_id, 0x62240104, 0x00000047);
write_reg( memshire_id, 0x62240108, 0x00000633);
write_reg( memshire_id, 0x6224010c, 0x00000149);
write_reg( memshire_id, 0x62240110, 0x0000004f);
write_reg( memshire_id, 0x62240114, 0x00000633);
write_reg( memshire_id, 0x62240118, 0x00000179);
write_reg( memshire_id, 0x6224011c, 0x00000008);
write_reg( memshire_id, 0x62240120, 0x000000e0);
write_reg( memshire_id, 0x62240124, 0x00000109);
write_reg( memshire_id, 0x62240128, 0x00000000);
write_reg( memshire_id, 0x6224012c, 0x000007c8);
write_reg( memshire_id, 0x62240130, 0x00000109);
write_reg( memshire_id, 0x62240134, 0x00000000);
write_reg( memshire_id, 0x62240138, 0x00000001);
write_reg( memshire_id, 0x6224013c, 0x00000008);
write_reg( memshire_id, 0x62240140, 0x00000000);
write_reg( memshire_id, 0x62240144, 0x0000045a);
write_reg( memshire_id, 0x62240148, 0x00000009);
write_reg( memshire_id, 0x6224014c, 0x00000000);
write_reg( memshire_id, 0x62240150, 0x00000448);
write_reg( memshire_id, 0x62240154, 0x00000109);
write_reg( memshire_id, 0x62240158, 0x00000040);
write_reg( memshire_id, 0x6224015c, 0x00000633);
write_reg( memshire_id, 0x62240160, 0x00000179);
write_reg( memshire_id, 0x62240164, 0x00000001);
write_reg( memshire_id, 0x62240168, 0x00000618);
write_reg( memshire_id, 0x6224016c, 0x00000109);
write_reg( memshire_id, 0x62240170, 0x000040c0);
write_reg( memshire_id, 0x62240174, 0x00000633);
write_reg( memshire_id, 0x62240178, 0x00000149);
write_reg( memshire_id, 0x6224017c, 0x00000008);
write_reg( memshire_id, 0x62240180, 0x00000004);
write_reg( memshire_id, 0x62240184, 0x00000048);
write_reg( memshire_id, 0x62240188, 0x00004040);
write_reg( memshire_id, 0x6224018c, 0x00000633);
write_reg( memshire_id, 0x62240190, 0x00000149);
write_reg( memshire_id, 0x62240194, 0x00000000);
write_reg( memshire_id, 0x62240198, 0x00000004);
write_reg( memshire_id, 0x6224019c, 0x00000048);
write_reg( memshire_id, 0x622401a0, 0x00000040);
write_reg( memshire_id, 0x622401a4, 0x00000633);
write_reg( memshire_id, 0x622401a8, 0x00000149);
write_reg( memshire_id, 0x622401ac, 0x00000010);
write_reg( memshire_id, 0x622401b0, 0x00000004);
write_reg( memshire_id, 0x622401b4, 0x00000018);
write_reg( memshire_id, 0x622401b8, 0x00000000);
write_reg( memshire_id, 0x622401bc, 0x00000004);
write_reg( memshire_id, 0x622401c0, 0x00000078);
write_reg( memshire_id, 0x622401c4, 0x00000549);
write_reg( memshire_id, 0x622401c8, 0x00000633);
write_reg( memshire_id, 0x622401cc, 0x00000159);
write_reg( memshire_id, 0x622401d0, 0x00000d49);
write_reg( memshire_id, 0x622401d4, 0x00000633);
write_reg( memshire_id, 0x622401d8, 0x00000159);
write_reg( memshire_id, 0x622401dc, 0x0000094a);
write_reg( memshire_id, 0x622401e0, 0x00000633);
write_reg( memshire_id, 0x622401e4, 0x00000159);
write_reg( memshire_id, 0x622401e8, 0x00000441);
write_reg( memshire_id, 0x622401ec, 0x00000633);
write_reg( memshire_id, 0x622401f0, 0x00000149);
write_reg( memshire_id, 0x622401f4, 0x00000042);
write_reg( memshire_id, 0x622401f8, 0x00000633);
write_reg( memshire_id, 0x622401fc, 0x00000149);
write_reg( memshire_id, 0x62240200, 0x00000001);
write_reg( memshire_id, 0x62240204, 0x00000633);
write_reg( memshire_id, 0x62240208, 0x00000149);
write_reg( memshire_id, 0x6224020c, 0x00000000);
write_reg( memshire_id, 0x62240210, 0x000000e0);
write_reg( memshire_id, 0x62240214, 0x00000109);
write_reg( memshire_id, 0x62240218, 0x0000000a);
write_reg( memshire_id, 0x6224021c, 0x00000010);
write_reg( memshire_id, 0x62240220, 0x00000109);
write_reg( memshire_id, 0x62240224, 0x00000009);
write_reg( memshire_id, 0x62240228, 0x000003c0);
write_reg( memshire_id, 0x6224022c, 0x00000149);
write_reg( memshire_id, 0x62240230, 0x00000009);
write_reg( memshire_id, 0x62240234, 0x000003c0);
write_reg( memshire_id, 0x62240238, 0x00000159);
write_reg( memshire_id, 0x6224023c, 0x00000018);
write_reg( memshire_id, 0x62240240, 0x00000010);
write_reg( memshire_id, 0x62240244, 0x00000109);
write_reg( memshire_id, 0x62240248, 0x00000000);
write_reg( memshire_id, 0x6224024c, 0x000003c0);
write_reg( memshire_id, 0x62240250, 0x00000109);
write_reg( memshire_id, 0x62240254, 0x00000018);
write_reg( memshire_id, 0x62240258, 0x00000004);
write_reg( memshire_id, 0x6224025c, 0x00000048);
write_reg( memshire_id, 0x62240260, 0x00000018);
write_reg( memshire_id, 0x62240264, 0x00000004);
write_reg( memshire_id, 0x62240268, 0x00000058);
write_reg( memshire_id, 0x6224026c, 0x0000000b);
write_reg( memshire_id, 0x62240270, 0x00000010);
write_reg( memshire_id, 0x62240274, 0x00000109);
write_reg( memshire_id, 0x62240278, 0x00000001);
write_reg( memshire_id, 0x6224027c, 0x00000010);
write_reg( memshire_id, 0x62240280, 0x00000109);
write_reg( memshire_id, 0x62240284, 0x00000005);
write_reg( memshire_id, 0x62240288, 0x000007c0);
write_reg( memshire_id, 0x6224028c, 0x00000109);
write_reg( memshire_id, 0x62100000, 0x00000811);
write_reg( memshire_id, 0x62100080, 0x00000880);
write_reg( memshire_id, 0x62100100, 0x00000000);
write_reg( memshire_id, 0x62100180, 0x00000000);
write_reg( memshire_id, 0x62100004, 0x00004008);
write_reg( memshire_id, 0x62100084, 0x00000083);
write_reg( memshire_id, 0x62100104, 0x0000004f);
write_reg( memshire_id, 0x62100184, 0x00000000);
write_reg( memshire_id, 0x62100008, 0x00004040);
write_reg( memshire_id, 0x62100088, 0x00000083);
write_reg( memshire_id, 0x62100108, 0x00000051);
write_reg( memshire_id, 0x62100188, 0x00000000);
write_reg( memshire_id, 0x6210000c, 0x00000811);
write_reg( memshire_id, 0x6210008c, 0x00000880);
write_reg( memshire_id, 0x6210010c, 0x00000000);
write_reg( memshire_id, 0x6210018c, 0x00000000);
write_reg( memshire_id, 0x62100010, 0x00000720);
write_reg( memshire_id, 0x62100090, 0x0000000f);
write_reg( memshire_id, 0x62100110, 0x00001740);
write_reg( memshire_id, 0x62100190, 0x00000000);
write_reg( memshire_id, 0x62100014, 0x00000016);
write_reg( memshire_id, 0x62100094, 0x00000083);
write_reg( memshire_id, 0x62100114, 0x0000004b);
write_reg( memshire_id, 0x62100194, 0x00000000);
write_reg( memshire_id, 0x62100018, 0x00000716);
write_reg( memshire_id, 0x62100098, 0x0000000f);
write_reg( memshire_id, 0x62100118, 0x00002001);
write_reg( memshire_id, 0x62100198, 0x00000000);
write_reg( memshire_id, 0x6210001c, 0x00000716);
write_reg( memshire_id, 0x6210009c, 0x0000000f);
write_reg( memshire_id, 0x6210011c, 0x00002800);
write_reg( memshire_id, 0x6210019c, 0x00000000);
write_reg( memshire_id, 0x62100020, 0x00000716);
write_reg( memshire_id, 0x621000a0, 0x0000000f);
write_reg( memshire_id, 0x62100120, 0x00000f00);
write_reg( memshire_id, 0x621001a0, 0x00000000);
write_reg( memshire_id, 0x62100024, 0x00000720);
write_reg( memshire_id, 0x621000a4, 0x0000000f);
write_reg( memshire_id, 0x62100124, 0x00001400);
write_reg( memshire_id, 0x621001a4, 0x00000000);
write_reg( memshire_id, 0x62100028, 0x00000e08);
write_reg( memshire_id, 0x621000a8, 0x00000c15);
write_reg( memshire_id, 0x62100128, 0x00000000);
write_reg( memshire_id, 0x621001a8, 0x00000000);
write_reg( memshire_id, 0x6210002c, 0x00000625);
write_reg( memshire_id, 0x621000ac, 0x00000015);
write_reg( memshire_id, 0x6210012c, 0x00000000);
write_reg( memshire_id, 0x621001ac, 0x00000000);
write_reg( memshire_id, 0x62100030, 0x00004028);
write_reg( memshire_id, 0x621000b0, 0x00000080);
write_reg( memshire_id, 0x62100130, 0x00000000);
write_reg( memshire_id, 0x621001b0, 0x00000000);
write_reg( memshire_id, 0x62100034, 0x00000e08);
write_reg( memshire_id, 0x621000b4, 0x00000c1a);
write_reg( memshire_id, 0x62100134, 0x00000000);
write_reg( memshire_id, 0x621001b4, 0x00000000);
write_reg( memshire_id, 0x62100038, 0x00000625);
write_reg( memshire_id, 0x621000b8, 0x0000001a);
write_reg( memshire_id, 0x62100138, 0x00000000);
write_reg( memshire_id, 0x621001b8, 0x00000000);
write_reg( memshire_id, 0x6210003c, 0x00004040);
write_reg( memshire_id, 0x621000bc, 0x00000080);
write_reg( memshire_id, 0x6210013c, 0x00000000);
write_reg( memshire_id, 0x621001bc, 0x00000000);
write_reg( memshire_id, 0x62100040, 0x00002604);
write_reg( memshire_id, 0x621000c0, 0x00000015);
write_reg( memshire_id, 0x62100140, 0x00000000);
write_reg( memshire_id, 0x621001c0, 0x00000000);
write_reg( memshire_id, 0x62100044, 0x00000708);
write_reg( memshire_id, 0x621000c4, 0x00000005);
write_reg( memshire_id, 0x62100144, 0x00000000);
write_reg( memshire_id, 0x621001c4, 0x00002002);
write_reg( memshire_id, 0x62100048, 0x00000008);
write_reg( memshire_id, 0x621000c8, 0x00000080);
write_reg( memshire_id, 0x62100148, 0x00000000);
write_reg( memshire_id, 0x621001c8, 0x00000000);
write_reg( memshire_id, 0x6210004c, 0x00002604);
write_reg( memshire_id, 0x621000cc, 0x0000001a);
write_reg( memshire_id, 0x6210014c, 0x00000000);
write_reg( memshire_id, 0x621001cc, 0x00000000);
write_reg( memshire_id, 0x62100050, 0x00000708);
write_reg( memshire_id, 0x621000d0, 0x0000000a);
write_reg( memshire_id, 0x62100150, 0x00000000);
write_reg( memshire_id, 0x621001d0, 0x00002002);
write_reg( memshire_id, 0x62100054, 0x00004040);
write_reg( memshire_id, 0x621000d4, 0x00000080);
write_reg( memshire_id, 0x62100154, 0x00000000);
write_reg( memshire_id, 0x621001d4, 0x00000000);
write_reg( memshire_id, 0x62100058, 0x0000060a);
write_reg( memshire_id, 0x621000d8, 0x00000015);
write_reg( memshire_id, 0x62100158, 0x00001200);
write_reg( memshire_id, 0x621001d8, 0x00000000);
write_reg( memshire_id, 0x6210005c, 0x0000061a);
write_reg( memshire_id, 0x621000dc, 0x00000015);
write_reg( memshire_id, 0x6210015c, 0x00001300);
write_reg( memshire_id, 0x621001dc, 0x00000000);
write_reg( memshire_id, 0x62100060, 0x0000060a);
write_reg( memshire_id, 0x621000e0, 0x0000001a);
write_reg( memshire_id, 0x62100160, 0x00001200);
write_reg( memshire_id, 0x621001e0, 0x00000000);
write_reg( memshire_id, 0x62100064, 0x00000642);
write_reg( memshire_id, 0x621000e4, 0x0000001a);
write_reg( memshire_id, 0x62100164, 0x00001300);
write_reg( memshire_id, 0x621001e4, 0x00000000);
write_reg( memshire_id, 0x62100068, 0x00004808);
write_reg( memshire_id, 0x621000e8, 0x00000880);
write_reg( memshire_id, 0x62100168, 0x00000000);
write_reg( memshire_id, 0x621001e8, 0x00000000);
write_reg( memshire_id, 0x62240290, 0x00000000);
write_reg( memshire_id, 0x62240294, 0x00000790);
write_reg( memshire_id, 0x62240298, 0x0000011a);
write_reg( memshire_id, 0x6224029c, 0x00000008);
write_reg( memshire_id, 0x622402a0, 0x000007aa);
write_reg( memshire_id, 0x622402a4, 0x0000002a);
write_reg( memshire_id, 0x622402a8, 0x00000010);
write_reg( memshire_id, 0x622402ac, 0x000007b2);
write_reg( memshire_id, 0x622402b0, 0x0000002a);
write_reg( memshire_id, 0x622402b4, 0x00000000);
write_reg( memshire_id, 0x622402b8, 0x000007c8);
write_reg( memshire_id, 0x622402bc, 0x00000109);
write_reg( memshire_id, 0x622402c0, 0x00000010);
write_reg( memshire_id, 0x622402c4, 0x00000010);
write_reg( memshire_id, 0x622402c8, 0x00000109);
write_reg( memshire_id, 0x622402cc, 0x00000010);
write_reg( memshire_id, 0x622402d0, 0x000002a8);
write_reg( memshire_id, 0x622402d4, 0x00000129);
write_reg( memshire_id, 0x622402d8, 0x00000008);
write_reg( memshire_id, 0x622402dc, 0x00000370);
write_reg( memshire_id, 0x622402e0, 0x00000129);
write_reg( memshire_id, 0x622402e4, 0x0000000a);
write_reg( memshire_id, 0x622402e8, 0x000003c8);
write_reg( memshire_id, 0x622402ec, 0x000001a9);
write_reg( memshire_id, 0x622402f0, 0x0000000c);
write_reg( memshire_id, 0x622402f4, 0x00000408);
write_reg( memshire_id, 0x622402f8, 0x00000199);
write_reg( memshire_id, 0x622402fc, 0x00000014);
write_reg( memshire_id, 0x62240300, 0x00000790);
write_reg( memshire_id, 0x62240304, 0x0000011a);
write_reg( memshire_id, 0x62240308, 0x00000008);
write_reg( memshire_id, 0x6224030c, 0x00000004);
write_reg( memshire_id, 0x62240310, 0x00000018);
write_reg( memshire_id, 0x62240314, 0x0000000e);
write_reg( memshire_id, 0x62240318, 0x00000408);
write_reg( memshire_id, 0x6224031c, 0x00000199);
write_reg( memshire_id, 0x62240320, 0x00000008);
write_reg( memshire_id, 0x62240324, 0x00008568);
write_reg( memshire_id, 0x62240328, 0x00000108);
write_reg( memshire_id, 0x6224032c, 0x00000018);
write_reg( memshire_id, 0x62240330, 0x00000790);
write_reg( memshire_id, 0x62240334, 0x0000016a);
write_reg( memshire_id, 0x62240338, 0x00000008);
write_reg( memshire_id, 0x6224033c, 0x000001d8);
write_reg( memshire_id, 0x62240340, 0x00000169);
write_reg( memshire_id, 0x62240344, 0x00000010);
write_reg( memshire_id, 0x62240348, 0x00008558);
write_reg( memshire_id, 0x6224034c, 0x00000168);
write_reg( memshire_id, 0x62240350, 0x00000070);
write_reg( memshire_id, 0x62240354, 0x00000788);
write_reg( memshire_id, 0x62240358, 0x0000016a);
write_reg( memshire_id, 0x6224035c, 0x00001ff8);
write_reg( memshire_id, 0x62240360, 0x000085a8);
write_reg( memshire_id, 0x62240364, 0x000001e8);
write_reg( memshire_id, 0x62240368, 0x00000050);
write_reg( memshire_id, 0x6224036c, 0x00000798);
write_reg( memshire_id, 0x62240370, 0x0000016a);
write_reg( memshire_id, 0x62240374, 0x00000060);
write_reg( memshire_id, 0x62240378, 0x000007a0);
write_reg( memshire_id, 0x6224037c, 0x0000016a);
write_reg( memshire_id, 0x62240380, 0x00000008);
write_reg( memshire_id, 0x62240384, 0x00008310);
write_reg( memshire_id, 0x62240388, 0x00000168);
write_reg( memshire_id, 0x6224038c, 0x00000008);
write_reg( memshire_id, 0x62240390, 0x0000a310);
write_reg( memshire_id, 0x62240394, 0x00000168);
write_reg( memshire_id, 0x62240398, 0x0000000a);
write_reg( memshire_id, 0x6224039c, 0x00000408);
write_reg( memshire_id, 0x622403a0, 0x00000169);
write_reg( memshire_id, 0x622403a4, 0x0000006e);
write_reg( memshire_id, 0x622403a8, 0x00000000);
write_reg( memshire_id, 0x622403ac, 0x00000068);
write_reg( memshire_id, 0x622403b0, 0x00000000);
write_reg( memshire_id, 0x622403b4, 0x00000408);
write_reg( memshire_id, 0x622403b8, 0x00000169);
write_reg( memshire_id, 0x622403bc, 0x00000000);
write_reg( memshire_id, 0x622403c0, 0x00008310);
write_reg( memshire_id, 0x622403c4, 0x00000168);
write_reg( memshire_id, 0x622403c8, 0x00000000);
write_reg( memshire_id, 0x622403cc, 0x0000a310);
write_reg( memshire_id, 0x622403d0, 0x00000168);
write_reg( memshire_id, 0x622403d4, 0x00001ff8);
write_reg( memshire_id, 0x622403d8, 0x000085a8);
write_reg( memshire_id, 0x622403dc, 0x000001e8);
write_reg( memshire_id, 0x622403e0, 0x00000068);
write_reg( memshire_id, 0x622403e4, 0x00000798);
write_reg( memshire_id, 0x622403e8, 0x0000016a);
write_reg( memshire_id, 0x622403ec, 0x00000078);
write_reg( memshire_id, 0x622403f0, 0x000007a0);
write_reg( memshire_id, 0x622403f4, 0x0000016a);
write_reg( memshire_id, 0x622403f8, 0x00000068);
write_reg( memshire_id, 0x622403fc, 0x00000790);
write_reg( memshire_id, 0x62240400, 0x0000016a);
write_reg( memshire_id, 0x62240404, 0x00000008);
write_reg( memshire_id, 0x62240408, 0x00008b10);
write_reg( memshire_id, 0x6224040c, 0x00000168);
write_reg( memshire_id, 0x62240410, 0x00000008);
write_reg( memshire_id, 0x62240414, 0x0000ab10);
write_reg( memshire_id, 0x62240418, 0x00000168);
write_reg( memshire_id, 0x6224041c, 0x0000000a);
write_reg( memshire_id, 0x62240420, 0x00000408);
write_reg( memshire_id, 0x62240424, 0x00000169);
write_reg( memshire_id, 0x62240428, 0x00000058);
write_reg( memshire_id, 0x6224042c, 0x00000000);
write_reg( memshire_id, 0x62240430, 0x00000068);
write_reg( memshire_id, 0x62240434, 0x00000000);
write_reg( memshire_id, 0x62240438, 0x00000408);
write_reg( memshire_id, 0x6224043c, 0x00000169);
write_reg( memshire_id, 0x62240440, 0x00000000);
write_reg( memshire_id, 0x62240444, 0x00008b10);
write_reg( memshire_id, 0x62240448, 0x00000168);
write_reg( memshire_id, 0x6224044c, 0x00000001);
write_reg( memshire_id, 0x62240450, 0x0000ab10);
write_reg( memshire_id, 0x62240454, 0x00000168);
write_reg( memshire_id, 0x62240458, 0x00000000);
write_reg( memshire_id, 0x6224045c, 0x000001d8);
write_reg( memshire_id, 0x62240460, 0x00000169);
write_reg( memshire_id, 0x62240464, 0x00000080);
write_reg( memshire_id, 0x62240468, 0x00000790);
write_reg( memshire_id, 0x6224046c, 0x0000016a);
write_reg( memshire_id, 0x62240470, 0x00000018);
write_reg( memshire_id, 0x62240474, 0x000007aa);
write_reg( memshire_id, 0x62240478, 0x0000006a);
write_reg( memshire_id, 0x6224047c, 0x0000000a);
write_reg( memshire_id, 0x62240480, 0x00000000);
write_reg( memshire_id, 0x62240484, 0x000001e9);
write_reg( memshire_id, 0x62240488, 0x00000008);
write_reg( memshire_id, 0x6224048c, 0x00008080);
write_reg( memshire_id, 0x62240490, 0x00000108);
write_reg( memshire_id, 0x62240494, 0x0000000f);
write_reg( memshire_id, 0x62240498, 0x00000408);
write_reg( memshire_id, 0x6224049c, 0x00000169);
write_reg( memshire_id, 0x622404a0, 0x0000000c);
write_reg( memshire_id, 0x622404a4, 0x00000000);
write_reg( memshire_id, 0x622404a8, 0x00000068);
write_reg( memshire_id, 0x622404ac, 0x00000009);
write_reg( memshire_id, 0x622404b0, 0x00000000);
write_reg( memshire_id, 0x622404b4, 0x000001a9);
write_reg( memshire_id, 0x622404b8, 0x00000000);
write_reg( memshire_id, 0x622404bc, 0x00000408);
write_reg( memshire_id, 0x622404c0, 0x00000169);
write_reg( memshire_id, 0x622404c4, 0x00000000);
write_reg( memshire_id, 0x622404c8, 0x00008080);
write_reg( memshire_id, 0x622404cc, 0x00000108);
write_reg( memshire_id, 0x622404d0, 0x00000008);
write_reg( memshire_id, 0x622404d4, 0x000007aa);
write_reg( memshire_id, 0x622404d8, 0x0000006a);
write_reg( memshire_id, 0x622404dc, 0x00000000);
write_reg( memshire_id, 0x622404e0, 0x00008568);
write_reg( memshire_id, 0x622404e4, 0x00000108);
write_reg( memshire_id, 0x622404e8, 0x000000b7);
write_reg( memshire_id, 0x622404ec, 0x00000790);
write_reg( memshire_id, 0x622404f0, 0x0000016a);
write_reg( memshire_id, 0x622404f4, 0x0000001f);
write_reg( memshire_id, 0x622404f8, 0x00000000);
write_reg( memshire_id, 0x622404fc, 0x00000068);
write_reg( memshire_id, 0x62240500, 0x00000008);
write_reg( memshire_id, 0x62240504, 0x00008558);
write_reg( memshire_id, 0x62240508, 0x00000168);
write_reg( memshire_id, 0x6224050c, 0x0000000f);
write_reg( memshire_id, 0x62240510, 0x00000408);
write_reg( memshire_id, 0x62240514, 0x00000169);
write_reg( memshire_id, 0x62240518, 0x0000000d);
write_reg( memshire_id, 0x6224051c, 0x00000000);
write_reg( memshire_id, 0x62240520, 0x00000068);
write_reg( memshire_id, 0x62240524, 0x00000000);
write_reg( memshire_id, 0x62240528, 0x00000408);
write_reg( memshire_id, 0x6224052c, 0x00000169);
write_reg( memshire_id, 0x62240530, 0x00000000);
write_reg( memshire_id, 0x62240534, 0x00008558);
write_reg( memshire_id, 0x62240538, 0x00000168);
write_reg( memshire_id, 0x6224053c, 0x00000008);
write_reg( memshire_id, 0x62240540, 0x000003c8);
write_reg( memshire_id, 0x62240544, 0x000001a9);
write_reg( memshire_id, 0x62240548, 0x00000003);
write_reg( memshire_id, 0x6224054c, 0x00000370);
write_reg( memshire_id, 0x62240550, 0x00000129);
write_reg( memshire_id, 0x62240554, 0x00000020);
write_reg( memshire_id, 0x62240558, 0x000002aa);
write_reg( memshire_id, 0x6224055c, 0x00000009);
write_reg( memshire_id, 0x62240560, 0x00000000);
write_reg( memshire_id, 0x62240564, 0x00000400);
write_reg( memshire_id, 0x62240568, 0x0000010e);
write_reg( memshire_id, 0x6224056c, 0x00000008);
write_reg( memshire_id, 0x62240570, 0x000000e8);
write_reg( memshire_id, 0x62240574, 0x00000109);
write_reg( memshire_id, 0x62240578, 0x00000000);
write_reg( memshire_id, 0x6224057c, 0x00008140);
write_reg( memshire_id, 0x62240580, 0x0000010c);
write_reg( memshire_id, 0x62240584, 0x00000010);
write_reg( memshire_id, 0x62240588, 0x00008138);
write_reg( memshire_id, 0x6224058c, 0x0000010c);
write_reg( memshire_id, 0x62240590, 0x00000008);
write_reg( memshire_id, 0x62240594, 0x000007c8);
write_reg( memshire_id, 0x62240598, 0x00000101);
write_reg( memshire_id, 0x6224059c, 0x00000008);
write_reg( memshire_id, 0x622405a0, 0x00000448);
write_reg( memshire_id, 0x622405a4, 0x00000109);
write_reg( memshire_id, 0x622405a8, 0x0000000f);
write_reg( memshire_id, 0x622405ac, 0x000007c0);
write_reg( memshire_id, 0x622405b0, 0x00000109);
write_reg( memshire_id, 0x622405b4, 0x00000000);
write_reg( memshire_id, 0x622405b8, 0x000000e8);
write_reg( memshire_id, 0x622405bc, 0x00000109);
write_reg( memshire_id, 0x622405c0, 0x00000047);
write_reg( memshire_id, 0x622405c4, 0x00000630);
write_reg( memshire_id, 0x622405c8, 0x00000109);
write_reg( memshire_id, 0x622405cc, 0x00000008);
write_reg( memshire_id, 0x622405d0, 0x00000618);
write_reg( memshire_id, 0x622405d4, 0x00000109);
write_reg( memshire_id, 0x622405d8, 0x00000008);
write_reg( memshire_id, 0x622405dc, 0x000000e0);
write_reg( memshire_id, 0x622405e0, 0x00000109);
write_reg( memshire_id, 0x622405e4, 0x00000000);
write_reg( memshire_id, 0x622405e8, 0x000007c8);
write_reg( memshire_id, 0x622405ec, 0x00000109);
write_reg( memshire_id, 0x622405f0, 0x00000008);
write_reg( memshire_id, 0x622405f4, 0x00008140);
write_reg( memshire_id, 0x622405f8, 0x0000010c);
write_reg( memshire_id, 0x622405fc, 0x00000000);
write_reg( memshire_id, 0x62240600, 0x00000478);
write_reg( memshire_id, 0x62240604, 0x00000109);
write_reg( memshire_id, 0x62240608, 0x00000000);
write_reg( memshire_id, 0x6224060c, 0x00000001);
write_reg( memshire_id, 0x62240610, 0x00000008);
write_reg( memshire_id, 0x62240614, 0x00000008);
write_reg( memshire_id, 0x62240618, 0x00000004);
write_reg( memshire_id, 0x6224061c, 0x00000008);
write_reg( memshire_id, 0x62240620, 0x00000008);
write_reg( memshire_id, 0x62240624, 0x000007c8);
write_reg( memshire_id, 0x62240628, 0x00000101);
write_reg( memshire_id, 0x62240018, 0x00000000);
write_reg( memshire_id, 0x6224001c, 0x00000000);
write_reg( memshire_id, 0x62240020, 0x00000008);
write_reg( memshire_id, 0x62240024, 0x00000000);
write_reg( memshire_id, 0x62240028, 0x00000000);
write_reg( memshire_id, 0x6224002c, 0x00000000);
write_reg( memshire_id, 0x6234039c, 0x00000400);
write_reg( memshire_id, 0x6224005c, 0x00000000);
write_reg( memshire_id, 0x6224007c, 0x00000029);
write_reg( memshire_id, 0x62240098, 0x0000006a);
write_reg( memshire_id, 0x62100340, 0x00000000);
write_reg( memshire_id, 0x62100344, 0x00000101);
write_reg( memshire_id, 0x62100348, 0x00000105);
write_reg( memshire_id, 0x6210034c, 0x00000107);
write_reg( memshire_id, 0x62100350, 0x0000010f);
write_reg( memshire_id, 0x62100354, 0x00000202);
write_reg( memshire_id, 0x62100358, 0x0000020a);
write_reg( memshire_id, 0x6210035c, 0x0000020b);
write_reg( memshire_id, 0x620800e8, 0x00000002);
write_reg( memshire_id, 0x6208002c, 0x00000085);
write_reg( memshire_id, 0x62080030, 0x0000010a);
write_reg( memshire_id, 0x62080034, 0x00000a6b);
write_reg( memshire_id, 0x62080038, 0x0000002c);
write_reg( memshire_id, 0x62240030, 0x00000000);
write_reg( memshire_id, 0x62240034, 0x00000173);
write_reg( memshire_id, 0x62240038, 0x00000060);
write_reg( memshire_id, 0x6224003c, 0x00006110);
write_reg( memshire_id, 0x62240040, 0x00002152);
write_reg( memshire_id, 0x62240044, 0x0000dfbd);
write_reg( memshire_id, 0x62240048, 0x0000ffff);
write_reg( memshire_id, 0x6224004c, 0x00006152);
write_reg( memshire_id, 0x62100200, 0x000000e0);
write_reg( memshire_id, 0x62100204, 0x00000012);
write_reg( memshire_id, 0x62100208, 0x000000e0);
write_reg( memshire_id, 0x6210020c, 0x00000012);
write_reg( memshire_id, 0x62100210, 0x000000e0);
write_reg( memshire_id, 0x62100214, 0x00000012);
write_reg( memshire_id, 0x621003f4, 0x0000000f);
write_reg( memshire_id, 0x62040044, 0x00000001);
write_reg( memshire_id, 0x62040048, 0x00000001);
write_reg( memshire_id, 0x6204004c, 0x00000180);
write_reg( memshire_id, 0x62040060, 0x00000001);
write_reg( memshire_id, 0x62040008, 0x00006209);
write_reg( memshire_id, 0x620402c8, 0x00000001);
write_reg( memshire_id, 0x620406d0, 0x00000001);
write_reg( memshire_id, 0x62040ad0, 0x00000001);
write_reg( memshire_id, 0x62040ed0, 0x00000001);
write_reg( memshire_id, 0x620412d0, 0x00000001);
write_reg( memshire_id, 0x620416d0, 0x00000001);
write_reg( memshire_id, 0x62041ad0, 0x00000001);
write_reg( memshire_id, 0x62041ed0, 0x00000001);
write_reg( memshire_id, 0x620422d0, 0x00000001);
write_reg( memshire_id, 0x62044044, 0x00000001);
write_reg( memshire_id, 0x62044048, 0x00000001);
write_reg( memshire_id, 0x6204404c, 0x00000180);
write_reg( memshire_id, 0x62044060, 0x00000001);
write_reg( memshire_id, 0x62044008, 0x00006209);
write_reg( memshire_id, 0x620442c8, 0x00000001);
write_reg( memshire_id, 0x620446d0, 0x00000001);
write_reg( memshire_id, 0x62044ad0, 0x00000001);
write_reg( memshire_id, 0x62044ed0, 0x00000001);
write_reg( memshire_id, 0x620452d0, 0x00000001);
write_reg( memshire_id, 0x620456d0, 0x00000001);
write_reg( memshire_id, 0x62045ad0, 0x00000001);
write_reg( memshire_id, 0x62045ed0, 0x00000001);
write_reg( memshire_id, 0x620462d0, 0x00000001);
write_reg( memshire_id, 0x62048044, 0x00000001);
write_reg( memshire_id, 0x62048048, 0x00000001);
write_reg( memshire_id, 0x6204804c, 0x00000180);
write_reg( memshire_id, 0x62048060, 0x00000001);
write_reg( memshire_id, 0x62048008, 0x00006209);
write_reg( memshire_id, 0x620482c8, 0x00000001);
write_reg( memshire_id, 0x620486d0, 0x00000001);
write_reg( memshire_id, 0x62048ad0, 0x00000001);
write_reg( memshire_id, 0x62048ed0, 0x00000001);
write_reg( memshire_id, 0x620492d0, 0x00000001);
write_reg( memshire_id, 0x620496d0, 0x00000001);
write_reg( memshire_id, 0x62049ad0, 0x00000001);
write_reg( memshire_id, 0x62049ed0, 0x00000001);
write_reg( memshire_id, 0x6204a2d0, 0x00000001);
write_reg( memshire_id, 0x6204c044, 0x00000001);
write_reg( memshire_id, 0x6204c048, 0x00000001);
write_reg( memshire_id, 0x6204c04c, 0x00000180);
write_reg( memshire_id, 0x6204c060, 0x00000001);
write_reg( memshire_id, 0x6204c008, 0x00006209);
write_reg( memshire_id, 0x6204c2c8, 0x00000001);
write_reg( memshire_id, 0x6204c6d0, 0x00000001);
write_reg( memshire_id, 0x6204cad0, 0x00000001);
write_reg( memshire_id, 0x6204ced0, 0x00000001);
write_reg( memshire_id, 0x6204d2d0, 0x00000001);
write_reg( memshire_id, 0x6204d6d0, 0x00000001);
write_reg( memshire_id, 0x6204dad0, 0x00000001);
write_reg( memshire_id, 0x6204ded0, 0x00000001);
write_reg( memshire_id, 0x6204e2d0, 0x00000001);
write_reg( memshire_id, 0x62080224, 0x00000001);
write_reg( memshire_id, 0x62080220, 0x00000019);
write_reg( memshire_id, 0x62300200, 0x00000002);
write_reg( memshire_id, 0x62340000, 0x00000001);

write_reg( memshire_id, 0x600001b0, 0x000010a0);
write_reg( memshire_id, 0x600011b0, 0x000010a0);

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// wait for init complete
////////////////////////////////////////////////////////////////////////////////////////////////////////////
do{value_read = read_reg( memshire_id, 0x600001bc);}
  while(!((value_read & 0x1) == 1));
do{value_read = read_reg( memshire_id, 0x600011bc);}
  while(!((value_read & 0x1) == 1));

write_reg( memshire_id, 0x600001b0, 0x00001080);
write_reg( memshire_id, 0x600011b0, 0x00001080);
write_reg( memshire_id, 0x600001b0, 0x00001081);
write_reg( memshire_id, 0x600011b0, 0x00001081);
write_reg( memshire_id, 0x600001b0, 0x00001081);
write_reg( memshire_id, 0x600011b0, 0x00001081);
write_reg( memshire_id, 0x60000320, 0x00000001);
write_reg( memshire_id, 0x60001320, 0x00000001);

do{value_read = read_reg( memshire_id, 0x60000324);}
  while(!((value_read & 0x1) == 1));
do{value_read = read_reg( memshire_id, 0x60001324);}
  while(!((value_read & 0x1) == 1));
do{value_read = read_reg( memshire_id, 0x60000004);}
  while(!((value_read & 0x1) == 1));
do{value_read = read_reg( memshire_id, 0x60001004);}
  while(!((value_read & 0x1) == 1));

write_reg( memshire_id, 0x60000030, 0x00000000);
write_reg( memshire_id, 0x60001030, 0x00000000);
write_reg( memshire_id, 0x60000490, 0x00000001);
write_reg( memshire_id, 0x60001490, 0x00000001);
write_reg( memshire_id, 0x60000540, 0x00000001);
write_reg( memshire_id, 0x60001540, 0x00000001);

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// That's All folks...
////////////////////////////////////////////////////////////////////////////////////////////////////////////
};
//ddrc_boot_seq.txt
//Displaying ddrc_boot_seq.txt.
