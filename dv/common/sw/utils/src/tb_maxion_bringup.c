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



//#define STUB_RANDOMS // FIXME
/* =============================================================================
 * STANDARD INCLUDE FILES
 * =============================================================================
 */
#include "cpu.h"
#include "soc.h"

#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "tb.h"
#include "tb_maxion_bringup.h"

#include "macros.h"
#ifdef MAXION_UART_PRINT
#include "serial.h"
#endif


#ifdef MAXION_UART_PRINT
#pragma message "MAXION UART PRINT is defined, hooray!"
#else
#pragma error "MAXION UART PRINT is undefined!"
#endif


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
//extern uint64_t __msg_base_address;
extern uint64_t __spy_base_address;
extern uint64_t __tcParam_base_address;
extern uint64_t __tb_end_address;
//extern uint64_t __msg_end_address;
extern uint64_t __spy_end_address;
//extern uint64_t __tcParam_end_address;
extern uint64_t __soc_end_address;


// From Soc.h
extern uint32_t thisCpu;

#ifdef STUB_RANDOMS
//from tcParam.h
void*    tc_param_random_ptr;
void*    tc_param_default_ptr;
uint64_t tc_param_seed;
#endif

 /* =============================================================================
 * LOCAL FUNCTIONS PROTOTYPES
 * =============================================================================
 */



/* =============================================================================
 * FUNCTIONS
 * =============================================================================
 */

#define MX_SP_MSG_TEST_NAME    0x1
#define MX_SP_MSG_TEST_PRINT   0x2
#define MX_SP_MSG_TEST_RESULT  0x3
#define MX_SP_MSG_TESTS_DONE   0x4
#define MX_SP_MSG_PRINT        0x5

#define MX_SP_MSG_TYPE_OFSET           0x0
#define MX_SP_MSG_MAX_ID_OFSET         0x1
#define MX_SP_MSG_TEST_ID_OFSET        0x2
#define MX_SP_MSG_TEST_RESULT_OFSET    0x4
#define MX_SP_MSG_TEXT_OFSET           0x5

#define MX_TEST_PASS  0x0
#define MX_TEST_FAIL  0x1

#ifndef SLT
   #define UART_DEVICE PU_UART0
#else
   #define UART_DEVICE PU_UART1
#endif

/*==================== Function Separator =============================*/
void tb_init(void)
{

    uint64_t addr;

    /* Set Global Pointers */
    pTB = (TB_t *)&__tb_base_address;
    /* Init tbId/Param  pointers */
    pTb_Id    = &__tb_Id;
//    pTb_Param = &__tb_Param;

    pTb_Id[0]    = 0;
    pTb_Id[1]    = 0;
    pTb_Id[2]    = 0xCCCC;


    /* Set CPU Id */
    pTB->cpuId = CPU;

    /* Configure tb from scatter file */
/*
    pTB->msgQueueAddress = (uint64_t)&__msg_base_address;
    pTB->msgQueueSize    = (uint64_t)(&__msg_end_address) - (uint64_t)(&__msg_base_address);
*/
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

    uint32_t error;

    error = ( status & 0xFFFF ) ? 1 : 0;

#ifndef MAXION_BRINGUP_TESTS
    uint32_t i;

    pTB->tcStatus = TB_TC_FINISH | ( status & 0x0000FFFF );
    __asm__ __volatile__ ("fence\n");

    /* All Slaves Completed, Stop Simulation */
    i = ( error ) ? TB_TC_FAIL : TB_TC_PASS;
    pTB->masterTcStatus = TB_TC_FINISH | i;


   /* Send Error status to stop Simulation */
   tb_write( TB_STOP , status & 0xFFFF );
#endif

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
    /* FUTURE: Use some msg  or irq to master CPU */
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

void tb_mailbox_putchar(char ch, uint8_t* currentCharIdx) {

   volatile uint8_t* pMboxAddr = (volatile uint8_t*)(R_PU_MBOX_MX_SP_BASEADDR + MX_SP_MSG_TEXT_OFSET);
   pMboxAddr[*currentCharIdx] = ch;
   asm volatile ("fence\n");

}

/*-------------------------------------------------------------------------*//**
 * @DEFINITION   CSRregRead
 *
 * @BRIEF        Read CSR register
 *
 * @DESCRIPTION  This macro is used to read CSR register.
 *               reg is name from Minion  CSR Register table
 *
 *//*------------------------------------------------------------------------ */
#define REGNAME(x) #x
#define CSR(x) REGNAME(x)
#define CSRregRead( reg ) (        \
{                                  \
    uint64_t ret;                  \
    __asm__ __volatile(            \
        "csrr %0, " CSR(reg) "\n"  \
        :"=r"(ret)                 \
        :                          \
        :);                        \
    ret;                           \
}                                  \
)



/*==================== Function Separator =============================*/
int tb_printf(char *format, ...)
{

  va_list a_list;

  char ch;
  char *p;
  int length;
  const int STR_LEN = 64;
  char str[STR_LEN]; // Fits the largest 64 bits number str represtantation. That's should be enough

  uint8_t currentCharIdx = 0;
  va_start( a_list, format);
  #ifndef MAXION_UART_PRINT
  //Wait on mailbox channel free
  #include "mbox_intr.h"
  volatile uint8_t* pMboxAddr = (volatile uint8_t*)R_PU_MBOX_MX_SP_BASEADDR;
  while (*pMboxAddr !=0) asm volatile ("nop\n");
  #endif
  
  while (*format){
    ch = *(format);
    if (ch != '%') {
      #ifndef MAXION_UART_PRINT
      tb_mailbox_putchar(*format++, &currentCharIdx);
      #else
      SERIAL_putc(UART_DEVICE, *format++);
      #endif
      currentCharIdx++;
    } else  {
      ch = *(++format);

      if (ch == '0') {
	ch = *(++format);
      }

      switch (ch) {
      case 'u':
      case 'd': {
	long num = va_arg(a_list, long);

	if (!num) {
	  #ifndef MAXION_UART_PRINT
          tb_mailbox_putchar('0', &currentCharIdx);
          #else
          SERIAL_putc(UART_DEVICE, '0');
          #endif
          currentCharIdx++;
	} else {
	  length = snprintf( NULL, 0, "%ld", num );
	  // Skip on error
	  if (length > 0 && length <= STR_LEN) {
	    snprintf( str, (size_t) length + 1, "%ld", num );
	    for( int i = 0; i < length; i++ ) {
	      #ifndef MAXION_UART_PRINT
              tb_mailbox_putchar(str[i], &currentCharIdx);
              #else 
              SERIAL_putc(UART_DEVICE, str[i]);
              #endif 
              currentCharIdx++;
            }
	  }

	}
	break;
      }
      case 'X':
      case 'x': {
	long num = va_arg(a_list, long);
	if (!num) {
           #ifndef MAXION_UART_PRINT
           tb_mailbox_putchar('0', &currentCharIdx);
           #else
           SERIAL_putc(UART_DEVICE, '0');
           #endif   
           currentCharIdx++;
	} else {
	  length = snprintf( NULL, 0, "%lx", num );
	  // Skip on error
	  if (length > 0 && length <= STR_LEN) {
	    snprintf( str, (size_t) length + 1, "%lx", num );
	    for( int i = 0; i < length; i++ ) {
	       #ifndef MAXION_UART_PRINT
               tb_mailbox_putchar(str[i], &currentCharIdx);
               #else
               SERIAL_putc(UART_DEVICE, str[i]);
               #endif   
               currentCharIdx++;
            }
	  }
	}
	break;
      }
      case 'h': {
	long num = va_arg(a_list, long);
	if (!num) {
	  #ifndef MAXION_UART_PRINT
          tb_mailbox_putchar('0', &currentCharIdx);
          #else
          SERIAL_putc(UART_DEVICE, '0');
          #endif  
          currentCharIdx++;
	} else {
	  length = snprintf( NULL, 0, "%lx", num );
	  // Skip on error
	  if (length > 0 && length <= STR_LEN) {
	    snprintf( str, (size_t) length + 1, "%lx", num );
	    for( int i = 0; i < length; i++ ) {
	      #ifndef MAXION_UART_PRINT
              tb_mailbox_putchar(str[i], &currentCharIdx);
              #else
              SERIAL_putc(UART_DEVICE, str[i]);
              #endif   
              currentCharIdx++;
            }
	  }
	}
	break;
      }
      case 'c': {
        #ifndef MAXION_UART_PRINT
        tb_mailbox_putchar(ch, &currentCharIdx);
        #else
        SERIAL_putc(UART_DEVICE, ch);
        #endif
        currentCharIdx++;
	break;
      }
      case 's': {
	p = va_arg(a_list, char*);
	while ((ch = *p++)) {
	  #ifndef MAXION_UART_PRINT
          tb_mailbox_putchar(ch, &currentCharIdx);
          #else
          SERIAL_putc(UART_DEVICE, ch);
          #endif  
          currentCharIdx++;
        }
	break;
      }
      default: break;
      }

      format++;

    }

  }
 #ifndef MAXION_UART_PRINT
   tb_mailbox_putchar('\0', &currentCharIdx);
   pMboxAddr[MX_SP_MSG_MAX_ID_OFSET] = (uint8_t) CSRregRead( mhartid );
   pMboxAddr[MX_SP_MSG_TYPE_OFSET]   = MX_SP_MSG_PRINT;
   asm volatile ("fence\n");



  PTR_Mbox_intr mbox_int_regs;
  mbox_int_regs = ( Mbox_intr* ) R_PU_TRG_MAX_BASEADDR;
  mbox_int_regs->ipi_trigger_set=0xF;
  #else
  SERIAL_putc(UART_DEVICE, '\n');
  SERIAL_putc(UART_DEVICE, '\r');
  #endif
  va_end( a_list );

  return 0;

} /* tb_printf() */



/*==================== Function Separator =============================*/
uint64_t tb_getRandom( char *format, ... )
{

    va_list a_list;

#ifndef STUB_RANDOMS
    printArgs[0] = (uint64_t)format;
#endif
    va_start( a_list, format);

#ifndef STUB_RANDOMS
    for( int i = 1; i < 16; i++ ) {
         printArgs[i] = va_arg( a_list, uint64_t );
    }

    tb_write( TB_RANDOM, (uint64_t)printArgs );

#else
    char word[64];
    uint64_t rand_mode = 0;
    uint64_t val[16];
    uint64_t res;
    uint64_t cnt = 0;
    char x, next_x;
    uint64_t word_index = 0;

    while (*format)
    {
        x = *format;
        format++;
        next_x = *format;

        if (x == ' ') {
            if (strncmp(word, "RANGE",5) == 0) {
                rand_mode = 0;
            }
            else if(strncmp(word, "MEMBER",6) == 0) {
                rand_mode = 1;
            }
            else if(strncmp(word,"%0d",3) == 0) {
                val[cnt] = va_arg(a_list, uint64_t);
                cnt++;
            }
            else {
                val[cnt] = (uint64_t) strtol(word, NULL, 10);
                cnt++;
            }

            word_index = 0;
            word[0] = '\0';
        }
        else {
            word[word_index] = x;
            if(next_x == '\0') {
                if(strncmp(word,"%0d",3) == 0) {
                    val[cnt] = (uint64_t) va_arg(a_list, int64_t);
                    cnt++;
                }
                else {
                    val[cnt] = (uint64_t) strtol(word, NULL, 10);
                    cnt++;
                }
            }

            word_index++;
        }

    }

    tb_printf("tb_getRandom(): SEED %d", tc_param_seed);
    srand( (uint32_t) tc_param_seed); // dirty fix

    if(rand_mode == 0){
        res = ((uint64_t)rand() % (val[1] - val[0] + 1)) + val[0];
        tb_printf("tb_getRandom(): rand_mode - RANGE  result %d", res);
    }else{
        res = (uint64_t)rand() % (cnt + 1);
        res = val[res];
        tb_printf("tb_getRandom(): rand_mode - MEMBER result %d", res);
    }

    for (uint64_t i = 0; i < cnt; i++)
    {
        tb_printf("tb_getRandom(): val[%d] %d" , i,  val[i]);
    }
#endif

    va_end( a_list );

#ifdef STUB_RANDOMS
    return res;
#else
    return printArgs[0];
#endif

} /* tb_getRandom() */



/*==================== Function Separator =============================*/
void *tb_getTcParamPtr( uint64_t tcParamType )
{

    void  *tcParamPtr;


    switch( tcParamType ) {
        case  TB_TC_PARAM_DEFAULT :
#ifndef STUB_RANDOMS
            tcParamPtr = (void*) pTB->tcParamAddress;
            tb_write( ((uint64_t)thisCpu << 32 ) + TB_TC_PARAM_DEFAULT, (uint64_t) tcParamPtr );
#else
            return tc_param_default_ptr;
#endif
            break;
        case  TB_TC_PARAM_RANDOM :
#ifndef STUB_RANDOMS
            tcParamPtr = (void*) pTB->tcParamAddress;
            tb_write( ((uint64_t)thisCpu << 32 ) + TB_TC_PARAM_RANDOM, (uint64_t) tcParamPtr );
#else
            return tc_param_random_ptr;
#endif
            break;
        default:
            tcParamPtr = NULL;
            break;
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

/*
    uint64_t pData[3];

    switch( Id ) {
        case TB_STOP:
            pData[0] = Param;
            pTb_Id[1] = (uint64_t)pData;
            break;
        case TB_TBCFG:
            pData[0] = 0;
            pData[1] = pTB;
            pData[2] = 0xABBA;
            pTb_Id[1] = (uint64_t)pData;
            break;
        case TB_SPY:
            pData[0] = Param;
            pTb_Id[1] = (uint64_t)pData;
            break;
        case TB_SYNC:
            pData[0] = Param;
            pTb_Id[1] = (uint64_t)pData;
            break;
        case TB_BUFF:
            pData[0] = Param;
            pTb_Id[1] = (uint64_t)pData;
            break;
        default:
            pTb_Id[1] = Param;

    }
*/

    pTb_Id[1] = Param;
    __asm__ __volatile__ ("fence\n");
    pTb_Id[0] = Id;
    __asm__ __volatile__ ("fence\n");

   /* Clear Id to enable next one  */
    pTb_Id[0] = 0;
    __asm__ __volatile__ ("fence\n");


} /* tb_write() */



/*****     <EOF>     *****/
