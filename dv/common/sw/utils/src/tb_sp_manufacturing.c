/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/**
* @file $Id$
* @version $Release$
* @date $Date$
* @author
*
* @brief tb.c Test Bench interface
*
* SW for various SW <-> SV synchronization
*/



/* =============================================================================
 * STANDARD INCLUDE FILES
 * =============================================================================
 */
#include "cpu.h"
#include "soc.h"

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "tb.h"
#include "tb_sp_manufacturing.h"

#include "macros.h"

//#include "serial.h"


//#include "api.h"
//#include "spy.h"


/* =============================================================================
 * GLOBAL VARIABLES DECLARATIONS
 * =============================================================================
 */

TB_t *pTB;



/* =============================================================================
 * LOCAL TYPES AND DEFINITIONS
 * =============================================================================
 */

#define PRINT_TYPE(DATA_TYPE, SN_STR)                                   \
    do {                                                                \
        DATA_TYPE num = va_arg(a_list, DATA_TYPE);                      \
        if (!num) {                                                     \
            putchar('0');                                               \
        } else {                                                        \
            length = snprintf( NULL, 0, SN_STR, num );                  \
            /* Skip on error */                                         \
            if (length > 0 && length <= STR_LEN) {                      \
                snprintf( str, (size_t) length + 1, SN_STR, num );      \
                for( int i = 0; i < length; i++ )                       \
                    putchar(str[i]);                                    \
            }                                                           \
        }                                                               \
    } while(0);                                                         


/* =============================================================================
 * LOCAL VARIABLES DECLARATIONS
 * =============================================================================
 */

uint64_t *pTb_Id;
uint64_t *pTb_Param;

uint64_t noPrintf;

uint64_t *printArgs;



/* =============================================================================
 * LOCAL FUNCTIONS DECLARATIONS
 * =============================================================================
 */

void tb_write( uint64_t Id, uint64_t Param );



/* =============================================================================
 * EXTERNAL VARIABLES DECLARATIONS
 * =============================================================================
 */

// tb Area

/* From Scatter File  */

extern uint64_t __tb_Id;
extern uint64_t __tb_Param;
extern uint64_t __tb_base_address;
extern uint64_t __spy_base_address;
extern uint64_t __tcParam_base_address;
extern uint64_t __tb_end_address;
extern uint64_t __spy_end_address;
extern uint64_t __soc_end_address;


// From Soc.h
extern uint32_t thisCpu;


//from tcParam.h
void* tc_param_random_ptr;
void* tc_param_default_ptr;
int tc_param_seed;

 /* =============================================================================
 * LOCAL FUNCTIONS PROTOTYPES
 * =============================================================================
 */



/* =============================================================================
 * FUNCTIONS
 * =============================================================================
 */



/*==================== Function Separator =============================*/
void tb_init(void)
{

    uint64_t addr;

    /* Set Global Pointers */
    pTB = (TB_t *)&__tb_base_address;
    /* Init tbId/Param  pointers */
    pTb_Id    = &__tb_Id;

    pTb_Id[0]    = 0;
    pTb_Id[1]    = 0;
    pTb_Id[2]    = 0xCCCC;


    /* Set CPU Id */
    pTB->cpuId = CPU;

    /* Configure tb from scatter file */
    pTB->msgQueueAddress = 0;
    pTB->msgQueueSize    = 0;

    pTB->spyAddress      = (uint64_t)&__spy_base_address;
    pTB->spySize         = (uint64_t)(&__spy_end_address) - (uint64_t)(&__spy_base_address);

    pTB->printfAddress   = 0;
    pTB->printfSize      = 0;
    pTB->printModeSelect = 0;   // FUTURE: add Enums. UVM printf

    /* Set TC param area */
    pTB->tcParamAddress  = (uint64_t)&__tcParam_base_address;
    pTB->tcParamSelect   = TB_TC_PARAM_RANDOM;

    addr = (uint64_t)&__soc_end_address;
    addr += 0x10;
    addr &= 0xFFFFFFFFFFFFFFF0ul;

    printArgs = (uint64_t *)addr;

    /* Set TC status as started */
    pTB->tcStatus = TB_TC_STARTED;
    __asm__ __volatile__ ("fence\n");

    /* Set master TC status as started */
    pTB->masterTcStatus = TB_TC_STARTED;
    __asm__ __volatile__ ("fence\n");

    noPrintf  = 0;

    printDbg( "Completed tb_init()\n" );
} /* tb_init() */



/*==================== Function Separator =============================*/
void tb_finish( uint32_t status )
{

    uint32_t i;
    uint32_t error;

    pTB->tcStatus = TB_TC_FINISH | ( status & 0x0000FFFF );
    __asm__ __volatile__ ("fence\n");

    error = ( status & 0xFFFF ) ? 1 : 0;

    /* All Slaves Completed, Stop Simulation */
    i = ( error ) ? TB_TC_FAIL : TB_TC_PASS;
    pTB->masterTcStatus = TB_TC_FINISH | i;


   /* Send Error status to stop Simulation */
   tb_write( TB_STOP , status & 0xFFFF );

   /* Stop BEMU */
   if( error ) {
      C_TEST_FAIL;
   }
   else {
      C_TEST_PASS;
   }


    __asm__ __volatile__ ("fence\n");

    while( 1 ) {
        WFI;
        printDbg( "Unexpected Wakeup while TC is finished !!!\n" );
    }

} /* tb_finish() */



/*==================== Function Separator =============================*/
void tb_fatalError( uint32_t error )
{


    pTB->tcStatus = TB_TC_FATAL_ERROR | ( error & 0x0000FFFF );
    __asm__ __volatile__ ("fence\n");

    /* At least one CPU detected Fatal Error */
    /* So stop simulation immediatelly */
    /*FUTURE: Use some msg  or irq to master CPU */

        while( 1 ) {
//        __WFI();
//        printDbg( "Unexpected Wakeup while TC has Fatal Error !!!\n" );
    }

} /* tb_fatalError() */



/*==================== Function Separator =============================*/
void tb_setTCStatus( TB_TC_STATUS_t status )
{

    pTB->tcStatus = status;
    __asm__ __volatile__ ("fence\n");

} /* tb_setTCStatus() */


/*==================== Function Separator =============================*/
int tb_printf(char *format, ...)
{

  va_list a_list;

  char ch;
  char *p;
  int length;
  const int STR_LEN = 32;
  char str[STR_LEN]; // Fits the largest 64 bits number str represtantation. That's should be enough

  va_start( a_list, format);

  while (*format){
      ch = *(format);
      if (ch != '%') {
          putchar(*format++);
      } else  {
          ch = *(++format);
          if (ch == '0') {
              ch = *(++format);
          }
          
          switch (ch) {
          case 'i':
          case 'd': {
              PRINT_TYPE(int64_t, "%ld")
              break;
          }
          case 'u': {
              PRINT_TYPE(uint64_t, "%lu");
              break;
          }
          case 'h':
          case 'X':
          case 'x': {
              PRINT_TYPE(uint64_t, "%lx");
              break;
          }
          case 'c': {
              putchar(ch);
              break;
          }
          case 's': {
              p = va_arg(a_list, char*);
              while ((ch = *p++))
                  putchar(ch);
              break;
          }
          default: break;
          }
          format++;
      }
  }
  putchar('\n');
  va_end( a_list );

  return 0;

} /* tb_printf() */



/*==================== Function Separator =============================*/
uint64_t tb_getRandom( char *format, ... )
{

    va_list a_list;


    printArgs[0] = (uint64_t)format;
    va_start( a_list, format);

    char word[64];
    int rand_mode = 0;
    int64_t val[16];
    int64_t res;
    int cnt = 0;
    char x, next_x;
    int word_index = 0;

    while (*format)
      { 
        x = *format;
        format++; 
        next_x = *format;
        
        if (x == ' ') 
	{ 
	    if (strncmp(word, "RANGE",5) == 0){
	      rand_mode = 0;
	    } else if (strncmp(word, "MEMBER",6) == 0){
	      rand_mode = 1;
	    } else if (strncmp(word,"%0d",3) == 0){
	      val[cnt] = va_arg(a_list, int64_t);
	      cnt++;
	    } else{
	      val[cnt] = strtol(word, NULL, 10); //atoi(word);
	      cnt++;
	    } 

            word_index = 0;
            word[0] = '\0';
	} 
        else
	  { 
	    word[word_index] = x; 
	    if(next_x == '\0'){
	      if(strncmp(word,"%0d",3) == 0){
                val[cnt] = va_arg(a_list, int64_t);
                cnt++;
	      }else{
		val[cnt] = strtol(word, NULL, 10); //atoi(word);
		cnt++;
	      }  
	    }
	
	    word_index++;
	  }

      }  

    srand((unsigned int) tc_param_seed);

    if(rand_mode == 0){
      res = (rand() % (val[1] - val[0] + 1)) + val[0];
    }else{
      res = rand() % (cnt + 1);
      res = val[res];
    }

    va_end( a_list );


    tb_printf("DEBUG SEED == %d", tc_param_seed);
    tb_printf("DEBUG tb_getRandom(): RESULT == %d" , res);

    return (uint64_t) res;


} /* tb_getRandom() */



/*==================== Function Separator =============================*/
void* tb_getTcParamPtr( uint64_t tcParamType )
{

    void  *tcParamPtr = (void*) pTB->tcParamAddress;
    
    switch( tcParamType ) {
       case  TB_TC_PARAM_DEFAULT: { buildTcParamDefault(tcParamPtr); break; }
       case  TB_TC_PARAM_RANDOM : { buildTcParamRandom(tcParamPtr); break; }
       default: { break; /* Nothing to do*/ }
    }
    
    return tcParamPtr;

} /* tb_getTcParamPtr() */



/*==================== Function Separator =============================*/
void tb_runningCPUs( uint64_t running_CPUs )
{

    tb_write( TB_RUNNING_CPUS, running_CPUs );

} /* tb_runningCPUs() */



/*==================== Function Separator =============================*/
void tb_spy( uint64_t spyVal )
{

    tb_write( TB_SPY, spyVal );

} /* tb_spy() */



/*==================== Function Separator =============================*/
void tb_sync(uint64_t syncVal)
{

    tb_write( TB_SYNC, syncVal );

} /* tb_Sync() */



/*==================== Function Separator =============================*/
void tb_buff(uint64_t *buff)
{

  tb_write( TB_BUFF, (uint64_t) buff );

} /* tb_buff() */


/*==================== Function Separator =============================*/
void tb_measureClock( uint64_t clockID, uint64_t address )
{

    uint64_t clockMeasureDataStruct[2];
    clockMeasureDataStruct[0] = clockID;
    clockMeasureDataStruct[1] = address;

    tb_write(64, (uint64_t) &clockMeasureDataStruct);

} /* tb_measureClock() */


/*==================== Function Separator =============================*/
void tb_genuine_boot(uint64_t action )
{

    tb_write(66, action);

} /* tb_genuine_boot() */


/*==================== Function Separator =============================*/
void tb_external_clk_sel(uint64_t strap_in )
{

    tb_write(69, strap_in);

} /* tb_external_clk_sel() */



/*==================== Function Separator =============================*/
void tb_sendSoCaddr( uint64_t SoCbaseAddr )
{

    tb_write(67, SoCbaseAddr ); /* fm_clockConfig_handler.sv */

} /* tb_sendSoCaddr() */

/*==================== Function Separator =============================*/
void tb_sendPadsAddr( uint64_t padsBaseAddr )
{

    tb_write(70, padsBaseAddr ); /* fm_forcePads_handler.sv */

} /* tb_sendPadsAddr() */

/*==================== Function Separator =============================*/
void tb_write( uint64_t Id, uint64_t Param )
{

#if 0
    pTb_Id[1] = Param;
    __asm__ __volatile__ ("fence\n");
    pTb_Id[0] = Id;
    __asm__ __volatile__ ("fence\n");

   /* Clear Id to enable next one  */
    pTb_Id[0] = 0;
    __asm__ __volatile__ ("fence\n");
#else
    (void)Id;
    (void)Param;
#endif

} /* tb_write() */



/*****     <EOF>     *****/
