/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "tbox_test.h"
#include <stdio.h>
#include <algorithm>
#include "minion.h"

volatile uint64_t *imgInfoTableBasePtr  = (uint64_t*)TBOX_IMG_TABLE_BASE_PTR;
volatile uint64_t *statusEsrPtr         = (uint64_t*)TBOX_STATUS_ESR_PTR;
volatile uint64_t *controlEsrPtr        = (uint64_t*)TBOX_CONTROL_ESR_PTR;  
volatile uint64_t *gfxTrapEsrPtr        = (uint64_t*)GFX_TRAP_ESR_PTR;  

ImageInfo imgInfoTable[] __attribute__((aligned(32))) = {
  {
    .address      = 0,
    .type         = 1,    // 2D
    .format       = 37,   // RGBA8
    .width        = 128,  // 129 x 65
    .height       = 64,
    .depth        = 0,
    .reserved0    = 0,
    .arraybase    = 0,
    .arraycount   = 0,
    .basemip      = 0,
    .mipcount     = 0,
    .swizzler     = 1,
    .swizzleg     = 1,
    .swizzleb     = 1,
    .swizzlea     = 1,
    .reserved1    = 0,
    .rowpitch     = (128*4*8/512),
    .mippitchl0   = 0,
    .mippitchl1   = 0,
    .elementpitch = 0,
    .tiled        = 1,
    .packedlayout = 0,
    .packedmip    = 0,
    .packedlevel  = 0,
    .reserved2    = 0
  },
  {
    .address      = 0,
    .type         = 1,    // 2D
    .format       = 37,   // RGBA8
    .width        = 32,   // 33 x 65
    .height       = 64,
    .depth        = 0,
    .reserved0    = 0,
    .arraybase    = 0,
    .arraycount   = 0,
    .basemip      = 0,
    .mipcount     = 0,
    .swizzler     = 1,
    .swizzleg     = 1,
    .swizzleb     = 1,
    .swizzlea     = 1,
    .reserved1    = 0,
    .rowpitch     = (128*4*8/512),
    .mippitchl0   = 0,
    .mippitchl1   = 0,
    .elementpitch = 0,
    .tiled        = 0,
    .packedlayout = 0,
    .packedmip    = 0,
    .packedlevel  = 0,
    .reserved2    = 0
   }
};

extern uint32_t texture[128*64];

uint8_t dummy(void* x){
  return *(volatile uint8_t*) x;
}

#define DCACHE_WAYS 4
#define DCACHE_SETS 64
#define DCACHE_ROW_BYTES 64
#define DCACHE_SET_TAG_OFFSET 12 // log2(SETS) + log2(offset)

void flush(void* ad, uint32_t size){
  uint8_t* end = ((uint8_t*) ad) + size;
  uint8_t* current = (uint8_t*)(uintptr_t(ad) & ~0x3F);
  //read 4 next tags to force eviction
  while(current<end) {
    for(int i=1;i<=DCACHE_WAYS;i++) dummy(current + (i<<DCACHE_SET_TAG_OFFSET));
    current+=0x40;
  }
}

void setup_rx_port( unsigned port_idx, bool user_mode, unsigned log2size, unsigned maxMsgs, void *buf, unsigned way) {
  unsigned nr_lines = DIV_CEIL((1<<log2size)*maxMsgs, 64);
  for ( unsigned i = 0; i < nr_lines; i++) {
    // evict line from L1 before locking
    uint64_t evict_conf = (1ULL << 58) | // evict to L2
      ((((intptr_t) buf) + i*64) & (~0x3FULL));
    __asm__ __volatile__ ( "csrw 0x89f, %[evict_conf]\n" : : [evict_conf] "r" (evict_conf) : );

    // FUTURE: Emmanuel Marie check cacheop error
    // lock scratchpad
    intptr_t scp_conf = ((intptr_t) buf) + 64*i;
    scp_conf &= ~0x3F; //remove offset
    scp_conf |= ((uint64_t) way) << 55;
    __asm__ __volatile__ ( "csrw 0x7fd, %[scp_conf]\n" : : [scp_conf] "r" (scp_conf) : );
  }

  unsigned scp_set = (( (intptr_t) buf) >> 6) & 0xF;
  // FUTURE: Emmanuel Marie check cacheop error

  // configure port
  unsigned port_conf =
    0x1                | // enable
    (1 << 1)           | // enable oob
    (user_mode << 4)   |
    (log2size << 5)    |
    ((maxMsgs-1) << 8) |
    (1 << 15)          | // don't care (hardwired)
    (scp_set << 16)    |
    (way << 24);

  switch(port_idx){
  case 0:
    __asm__ __volatile__ ( "csrw %[csr], %[port_conf]\n" : : [port_conf] "r" (port_conf), [csr] "i" (PORT0_CTRL + 0) );
    break;
  case 1:
    __asm__ __volatile__ ( "csrw %[csr], %[port_conf]\n" : : [port_conf] "r" (port_conf), [csr] "i" (PORT0_CTRL + 1) );
    break;
  case 2:
    __asm__ __volatile__ ( "csrw %[csr], %[port_conf]\n" : : [port_conf] "r" (port_conf), [csr] "i" (PORT0_CTRL + 2) );
    break;
  default:
    __asm__ __volatile__ ( "csrw %[csr], %[port_conf]\n" : : [port_conf] "r" (port_conf), [csr] "i" (PORT0_CTRL + 3) );
    break;
  }

  // FUTURE: Emmanuel Marie check msg error
}

#define UNKNOWN             0
#define NUMBER_OF_TESTS     2
#define BUFFER_SIZE         (32*4*4)/4 // 32 (msg size) * 4 (msgs) * 4 (ports) * 16 (harts). Divided by 4 because is UNSIGNED

sample_request_t sample_request[NUMBER_OF_TESTS] = {
    // Test 1
    {1, {/*imageSamplingOperation*/IMAGE_SAMPLING_OPERATION_SAMPLE_L,
        /*componentSelection    */UNKNOWN,
        /*texelOffsetK          */0,
        /*texelOffsetJ          */0,
        /*texelOffsetI          */0,
        /*samplerRequestPackets */2,
        /*reserved0             */0,

        /*pixelMask             */0xff,
        /*reserved1             */0,
        /*imageId               */1,
        /*reserved2             */0,
        /*borderDescId          */UNKNOWN,
        /*reserved3             */0,

        /*minFilter             */LINEAR_FILTER,
        /*magFilter             */LINEAR_FILTER,
        /*mipmapFilter          */LINEAR_FILTER, // trilinear!!!
        /*anisoFilter           */ANISO_DISABLED,
        /*compareOp             */COMP_NEVER,
        /*addressModeU          */ADDR_MODE_REPEAT,
        /*addressModeV          */ADDR_MODE_REPEAT,
        /*addressModeW          */ADDR_MODE_REPEAT,
        /*borderColorMode       */BORDER_COLOR_TRANSPARENT_BLACK,
        /*swizzler              */SWIZZLE_IDENTIY,
        /*swizzleg              */SWIZZLE_IDENTIY,
        /*swizzleb              */SWIZZLE_IDENTIY,
        /*swizzlea              */SWIZZLE_IDENTIY,
        /*reserved4             */0,

        /*lodaniso.lod_array[0] */{1,
        /*lodaniso.lod_array[1] */1,
        /*lodaniso.lod_array[2] */1,
        /*lodaniso.lod_array[3] */1,
        /*lodaniso.lod_array[4] */1,
        /*lodaniso.lod_array[5] */1,
        /*lodaniso.lod_array[6] */1,
        /*lodaniso.lod_array[7] */1}
    }
   },
   // Test 2
    {2, {/*imageSamplingOperation*/IMAGE_SAMPLING_OPERATION_SAMPLE_L,
        /*componentSelection    */UNKNOWN,
        /*texelOffsetK          */0,
        /*texelOffsetJ          */0,
        /*texelOffsetI          */0,
        /*samplerRequestPackets */2,
        /*reserved0             */0,

        /*pixelMask             */0xff,
        /*reserved1             */0,
        /*imageId               */1,
        /*reserved2             */0,
        /*borderDescId          */UNKNOWN,
        /*reserved3             */0,

        /*minFilter             */LINEAR_FILTER,
        /*magFilter             */LINEAR_FILTER,
        /*mipmapFilter          */LINEAR_FILTER, // trilinear!!!
        /*anisoFilter           */ANISO_DISABLED,
        /*compareOp             */COMP_NEVER,
        /*addressModeU          */ADDR_MODE_REPEAT,
        /*addressModeV          */ADDR_MODE_REPEAT,
        /*addressModeW          */ADDR_MODE_REPEAT,
        /*borderColorMode       */BORDER_COLOR_TRANSPARENT_BLACK,
        /*swizzler              */SWIZZLE_IDENTIY,
        /*swizzleg              */SWIZZLE_IDENTIY,
        /*swizzleb              */SWIZZLE_IDENTIY,
        /*swizzlea              */SWIZZLE_IDENTIY,
        /*reserved4             */0,

        /*lodaniso.lod_array[0] */{1,
        /*lodaniso.lod_array[1] */1,
        /*lodaniso.lod_array[2] */1,
        /*lodaniso.lod_array[3] */1,
        /*lodaniso.lod_array[4] */1,
        /*lodaniso.lod_array[5] */1,
        /*lodaniso.lod_array[6] */1,
        /*lodaniso.lod_array[7] */1}
    }
   }
};// end array

int main()
{

	uint64_t mid, tid;
	mid = get_minion_id();
	tid = get_thread_id();

   if(mid == 0 && tid == 0)
   {
    C_TEST_START;

    // set mask to all 1's
    __asm__ __volatile__ (
        "li a7, 0xff\n"
        "mova.m.x a7\n"
        :
        :
        : "a7");

    CTC(sizeof(SampleRequest)==256/8);
    CTC(sizeof(ImageInfo)==256/8);

    SampleRequest header        __attribute__((aligned(32)));
    tboxSendCoord_t u_coord     __attribute__((aligned(32))) = {0.2, 0.4, 0.6, 0.8, 0.3, 0.5, 0.7, 0.9};
    tboxSendCoord_t v_coord     __attribute__((aligned(32))) = {0.8, 0.6, 0.4, 0.2, 0.9, 0.7, 0.5, 0.3};

    // configure pointers
    *imgInfoTableBasePtr        =  (uintptr_t) imgInfoTable; // Store image table base address in (*imgInfoTableBasePtr) => (*imgInfoTableBasePtr)[0] == imgInfoTable[0]
    *controlEsrPtr              =  (uintptr_t) (0x0001);
    *gfxTrapEsrPtr              =  (uintptr_t) (0);
     imgInfoTable[0].address    =  (uint64_t) texture;
     imgInfoTable[1].address    =  (uint64_t) texture;

    // set value for header to use 2nd entry of the table
    bzero(&header, sizeof(SampleRequest));

    flush(imgInfoTable, sizeof(ImageInfo)*2);

    volatile unsigned  buffer_sample_request[BUFFER_SIZE] __attribute__((aligned(64))); // The buffer that will feed the Tbox must be aligned to line size (64B)
    volatile unsigned* buffer_sample_request_ptr = buffer_sample_request;

    for(int i = 0; i < BUFFER_SIZE; i++)
    {
        buffer_sample_request[i] = 0xdeadbeef;
    }

    for(unsigned test_idx = 0; test_idx < 1; test_idx++)
    {
        for(unsigned hart_idx = 0; hart_idx < 1; hart_idx++) // FUTURE: Emmanuel Marie Distribute Sample Request among different harts, right now all enabled harts execute all requests
        {
            for (unsigned port_idx = 0; port_idx < 1; port_idx++)
            {

                test_config_t test_conf;

                header.info = sample_request[test_idx].info;

                test_conf.log2size[port_idx]     = 5; // log2 of message size in B
                test_conf.way[port_idx]          = 0;
                test_conf.enable_oob[port_idx]   = 1;
                test_conf.maxMsgs[port_idx]      = 4; // reserve 4 messages of 256, because the TBOX returns 1024b

                /* Esperanto PRM: Texture Extension: Texture Protocol */

                /* Step 1: Setup a communication port
                    CSRRW  x0, 0x8df, x1            // LockVA to reserve cache lines
                    CSRRSI x0, PORTCTRL<portid>, 0  // PENABLE to associate cache lines to hart's port
                */
                bool        user_mode       = false;

                setup_rx_port(port_idx, user_mode, test_conf.log2size[port_idx], test_conf.maxMsgs[port_idx], (void *) buffer_sample_request_ptr, test_conf.way[port_idx]/*, test_conf.enable_oob[port_idx]*/);    // Step 1


                /* Step 2: Prepare arguments in local cache
                   Software creates a texture request (see the ?Texture Request? section) and stores it at offset 0 of the storage associated to the port set up in step #1.
                   This is done using regular RISCV store instructions and does not involve any communication with the texture unit.
                */
                /* DCACHE will send three packets of 256 bits containing the parameter data */

                char* ptr = (char*) buffer_sample_request_ptr;
                std::copy(&header, (&header) +1, (SampleRequest*) ptr);
                ptr += sizeof(SampleRequest);
                std::copy(&u_coord, &(u_coord)+1, (tboxSendCoord_t*) ptr);
                ptr += sizeof(tboxSendCoord_t);
                std::copy(&v_coord, (&v_coord) +1,   (tboxSendCoord_t*) ptr);

                /* Step 3: Notify texture unit that request is ready using TEXSEND
                    CSRRW  xd, xs, 0x807
                        xs[63:8] = WARL (0)
                        xs[7:4] = Texture request size in number of 128b packets. Minimum valid value is 2, maximum is 6.
                        xs[3:0] = portid holding arguments and where results will be written. Maximum valid value is 3.
                */
                int n1, n2;
                int number_packets = 3; // number of packets for the sample request
                int csr_op = port_idx | (number_packets<<4);

                __asm__ __volatile__ ("csrrw %[n1], %[csr], %[rs]\n"
                                    : [n1] "=r" (n1)
                                    : [csr] "i" (0x807), [rs] "r" (csr_op) );

                /* Step 4: Wait for texture unit response
                   CSRRS rd, PORTHEAD<portid>, x0 Blocking PGET (Port Get)
                */
                unsigned num_messages = test_conf.maxMsgs[port_idx];

                for(unsigned i = 0; i < num_messages; i++)
                {
                    // First PGET
                    switch(port_idx)
                    {
                      case 0:
                        __asm__ __volatile__ ("csrrs %[n1], %[port], x0\n"
                                        : [n1] "=r" (n1)
                                        : [port] "i" (PORT0_HEAD + 0));
                        break;
                      case 1:
                        __asm__ __volatile__ ("csrrs %[n1], %[port], x0\n"
                                        : [n1] "=r" (n1)
                                        : [port] "i" (PORT0_HEAD + 1));
                        break;
                      case 2:
                        __asm__ __volatile__ ("csrrs %[n1], %[port], x0\n"
                                        : [n1] "=r" (n1)
                                        : [port] "i" (PORT0_HEAD + 2));
                        break;
                      default:
                        __asm__ __volatile__ ("csrrs %[n1], %[port], x0\n"
                                        : [n1] "=r" (n1)
                                        : [port] "i" (PORT0_HEAD + 3));
                        break;
                    }
                }

                /* The result of the Sample Request should be:
                    0x3f65789b // r0
                    0x3f209717
                    0x3f29e7e8
                    0x3ecb6d6e
                    0x3e5d5d5e
                    0x3f2ceced
                    0x3edcdcdd
                    0x3f2ceced // r7

                    0x3f65789b // g
                    0x3f209717
                    0x3f29e7e8
                    0x3ecb6d6e
                    0x3e5d5d5e
                    0x3f2ceced
                    0x3edcdcdd
                    0x3f2ceced

                    0x3f65789b // b
                    0x3f209717
                    0x3f29e7e8
                    0x3ecb6d6e
                    0x3e5d5d5e
                    0x3f2ceced
                    0x3edcdcdd
                    0x3f2ceced

                    0x3f65789b // a
                    0x3f209717
                    0x3f29e7e8
                    0x3ecb6d6e
                    0x3e5d5d5e
                    0x3f2ceced
                    0x3edcdcdd
                    0x3f2ceced

                */

                unsigned aux = n1 + n2;
                if (aux > 0)
                    aux = aux + 1;
            } // end for port
        }// end for hart_idx
    }// end for test number
    C_TEST_PASS;
    return 0;
   }
   return 0;
}
