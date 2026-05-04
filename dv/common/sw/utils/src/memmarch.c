/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/**
* @file $Id$
* @version $Release$
* @date $Date$
* @author 
*
* @brief memmarch.c Standard memmarch tests
*
*/

#include "cpu.h"
#include "soc.h"

//#include "tb.h"
//#include "spy.h"

#include "memmarch.h"

#define bool int

int nrErrorsToPrint = 5;
uint8_t masks8[] = {0x0,0x55,0x33,0x0F};
uint16_t masks16[] = {0x0,0x5555,0x3333,0x0F0F,0x00FF};
uint32_t masks32[] = {0x0,0x55555555,0x33333333,0x0F0F0F0F,0x00FF00FF,0x0000FFFF};
uint64_t masks64[] = {0x0,0x5555555555555555,0x3333333333333333,0x0F0F0F0F0F0F0F0F,0x00FF00FF00FF00FF,0x0000FFFF0000FFFF};
int nrMasks = 6;

/*==================== Function Separator =============================*/
/*void mm_printError( const char* str, uint64_t addr, uint64_t val, uint32_t mask)
{

   if (nrErrorsToPrint) {
      //printDbg("%s on addr 0x%X with val 0x%X and expected 0x%X\n",str,addr,val,mask);
      nrErrorsToPrint--;
   }

} */ /* mm_printError() */



/*==================== Function Separator =============================*/
uint32_t mm_march9seq8( uint64_t address, uint64_t size, uint8_t mask )
{

    uint32_t   errors = 0;
    uint8_t*   base;
    uint8_t*   p;
    uint8_t    invMask = mask ^ 0xFF;
    uint8_t    v;
    uint32_t   i;

    base = (uint8_t*) address;

    p = base;
    for( i = 0; i < size; i++ ) {
        *p++ = invMask;
    }

    p = base;
    for( i = 0; i < size; i++ ) {
        v = *p;
        if( v != invMask ) {
            errors++;
            //mm_printError("step 2",(uint64_t)p,v,invMask);
        }
        *p++ = mask;
    }

    p = base;
    for( i = 0; i <size; i++ ) {
        v = *p;
        if( v != mask ) {
           errors++;
           //mm_printError("step 4",(uint64_t)p,v,mask);
        }
        *p++ = invMask;
    }

    p = base+size-1;
    for( i = 0; i < size; i++ ) {
        v = *p;
        if( v != invMask ) {
            errors++;
            //mm_printError("step 6",(uint64_t)p,v,invMask);
        }
        *p-- = mask;
    }

    p = base+size-1;
    for( i = 0; i < size; i++ ) {
        v = *p;
        if( v != mask ) {
            errors++;
            //mm_printError("step 8",(uint64_t)p,v,mask);
         }
         *p-- = invMask;
    }

    p = base+size-1;
    for( i = 0; i < size; i++ ) {
        v = *p;
        if( v != invMask ) {
            errors++;
            //mm_printError("step 10",(uint64_t)p,v,invMask);
        }
        p--;
    }

    return errors;

} /* mm_march9seq8() */



/*==================== Function Separator =============================*/
uint32_t mm_march9seq16(  uint64_t address, uint32_t size, uint16_t mask )
{

   uint32_t errors = 0;
   uint16_t* base;
   uint16_t* p;
   uint16_t invMask = mask ^ 0xFFFF;
   uint16_t v;

   base = (uint16_t*) address;
   p = base;

   for (uint32_t i =0;i<size;++i) {
      *p++ = invMask;
   }
   p = base;
   for (uint32_t i =0;i<size;++i) {
      v = *p;
      if (v != invMask) {
         errors++;
         //mm_printError("step 2",(uint64_t)p,v,invMask);
      }
      *p++ = mask;
   }
   p = base;
   for (uint32_t i =0;i<size;++i) {
      v = *p;
      if (v != mask) {
         errors++;
         //mm_printError("step 4",(uint64_t)p,v,mask);
      }
      *p++ = invMask;
   }
   p = base+size-1;
   for (uint32_t i =0;i<size;++i) {
     v = *p;
     if (v != invMask) {
         errors++;
         //mm_printError("step 6",(uint64_t)p,v,invMask);
      }
      *p-- = mask;
   }
   p = base+size-1;
   for (uint32_t i =0;i<size;++i) {
      v = *p;
      if (v != mask) {
         errors++;
         //mm_printError("step 8",(uint64_t)p,v,mask);
      }
      *p-- = invMask;
   }
   p = base+size-1;
   for (uint32_t i =0;i<size;++i) {
      v = *p;
      if (v != invMask) {
         errors++;
         //mm_printError("step 10",(uint64_t)p,v,invMask);
      }
      p--;
   }
   return errors;

} /* mm_march9seq16() */



/*==================== Function Separator =============================*/
uint32_t mm_march9seq32(  uint64_t address, uint32_t size, uint32_t mask )
{

   uint32_t errors = 0;
   uint32_t* base;
   uint32_t* p;
   uint32_t invMask = ~mask;
   uint32_t v;

   base = (uint32_t*) address;
   p = base;
   for (uint32_t i =0;i<size;++i) {
      *p++ = invMask;
   }
   p = base;
   for (uint32_t i =0;i<size;++i) {
      v = *p;
      if (v != invMask) {
         errors++;
         //mm_printError("step 2",(uint64_t)p,v,invMask);
      }
      *p++ = mask;
   }
   p = base;
   for (uint32_t i =0;i<size;++i) {
      v = *p;
      if (v != mask) {
         errors++;
         //mm_printError("step 4",(uint64_t)p,v,mask);
      }
      *p++ = invMask;
   }
   p = base+size-1;
   for (uint32_t i =0;i<size;++i) {
     v = *p;
     if (v != invMask) {
         errors++;
         //mm_printError("step 6",(uint64_t)p,v,invMask);
      }
      *p-- = mask;
   }
   p = base+size-1;
   for (uint32_t i =0;i<size;++i) {
      v = *p;
      if (v != mask) {
         errors++;
         //mm_printError("step 8",(uint64_t)p,v,mask);
      }
      *p-- = invMask;
   }
   p = base+size-1;
   for (uint32_t i =0;i<size;++i) {
      v = *p;
      if (v != invMask) {
         errors++;
         //mm_printError("step 10",(uint64_t)p,v,invMask);
      }
      p--;
   }

   return errors;

} /* mm_march9seq32() */



/*==================== Function Separator =============================*/
uint32_t mm_march9seq64(  uint64_t address, uint32_t size, uint64_t mask )
{
   uint32_t errors = 0;
   uint64_t* base;
   uint64_t* p;
   uint64_t invMask = ~mask;
   uint64_t v;
   base = (uint64_t*) address;
   p = base;

   for (uint32_t i =0;i<size;++i) {
      *p++ = invMask;
   }
   p = base;
   for (uint32_t i =0;i<size;++i) {
      v = *p;
      if (v != invMask) {
         errors++;
         //mm_printError("step 2",(uint64_t)p,v,invMask);
      }
      *p++ = mask;
   }
   p = base;
   for (uint32_t i =0;i<size;++i) {
      v = *p;
      if (v != mask) {
         errors++;
         //mm_printError("step 4",(uint64_t)p,v,mask);
      }
      *p++ = invMask;
   }
   p = base+size-1;
   for (uint32_t i =0;i<size;++i) {
     v = *p;
     if (v != invMask) {
         errors++;
         //mm_printError("step 6",(uint64_t)p,v,invMask);
      }
      *p-- = mask;
   }
   p = base+size-1;
   for (uint32_t i =0;i<size;++i) {
      v = *p;
      if (v != mask) {
         errors++;
         //mm_printError("step 8",(uint64_t)p,v,mask);
      }
      *p-- = invMask;
   }
   p = base+size-1;
   for (uint32_t i =0;i<size;++i) {
      v = *p;
      if (v != invMask) {
         errors++;
         //mm_printError("step 10",(uint64_t)p,v,invMask);
      }
      p--;
   }
  
   return errors;

} /* mm_march9seq64() */



/*==================== Function Separator =============================*/
uint32_t mm_march9seq( uint64_t address, uint32_t sizeInBytes, uint8_t logDataWidth)
{

   uint32_t errors = 0;
   uint32_t size = sizeInBytes >> logDataWidth;

   switch(logDataWidth){

     case 0:

      for (int i =0;i<(nrMasks-2);i++) {
         errors += mm_march9seq8(address,size,masks8[i]);
      }

     break;

     case 1:

      for (int i =0;i<(nrMasks-1);i++) {
         errors += mm_march9seq16(address,size,masks16[i] );
      }

     break;

     case 2:

      for (int i =0;i<nrMasks;i++) {
         errors += mm_march9seq32(address,size,masks32[i] );
      }

     break;

     case 3:

      for (int i =0;i<nrMasks;i++) {
         errors += mm_march9seq64(address,size,masks64[i]);
      }

     break;

   }

   return errors;

} /* mm_march9seq() */



/*==================== Function Separator =============================*/
uint32_t mm_matsPlusseq8( uint64_t address, uint32_t size )
{

   uint32_t i = 0;
   uint32_t errors = 0;
   uint8_t* base;
   uint8_t* p;
   uint8_t mask = 0xFF;
   //uint8_t invMask = 0x00;
   uint8_t v;

   base = (uint8_t*) address;
   p = base;

   for(i=0;i<size;i++) *p++ = 0;//write 0 in m1

   p = base;

   for(i=0;i<size;i++) {//read 0 in m2, write 1
      v = *p;
      if( v == 0 ) *p++ = mask;
      else {
         errors++;p++;
         //mm_printError("step m2",(uint64_t)p,v,mask);
      }
   }

   p = base;

   for(i=0;i<size;i++) {//read 1 in m3, write 0
      v = *p;
      if( v == mask ) *p++ = 0;
      else {
         errors++;p++;
         //mm_printError("step m3",(uint64_t)p,v,invMask);
      }
   }

   return errors;

} /* mm_matsPlusseq8() */



/*==================== Function Separator =============================*/
uint32_t mm_matsPlusseq16( uint64_t address, uint32_t size )
{

   uint32_t i = 0;
   uint32_t errors = 0;
   uint16_t* base;
   uint16_t* p;
   uint16_t mask = 0xFFFF;
   //uint16_t invMask = 0x0000;
   uint16_t v;

   base = (uint16_t*) address;
   p = base;

   for(i=0;i<size;i++) *p++ = 0;//write 0 in m1

   p = base;

   for(i=0;i<size;i++) {//read 0 in m2, write 1
      v = *p;
      if( v == 0 ) *p++ = mask;
      else {
         errors++;p++;
         //mm_printError("step m2",(uint64_t)p,v,mask);
      }
   }

   p = base;

   for(i=0;i<size;i++) {//read 1 in m3, write 0
      v = *p;
      if( v == mask ) *p++ = 0;
      else {
         errors++;p++;
         //mm_printError("step m3",(uint64_t)p,v,invMask);
      }
   }

   return errors;

} /* mm_matsPlusseq16() */



/*==================== Function Separator =============================*/
uint32_t mm_matsPlusseq32( uint64_t address, uint32_t size )
{

   uint32_t i = 0;
   uint32_t errors = 0;
   uint32_t* base;
   uint32_t* p;
   uint32_t mask = 0xFFFFFFFF;
   //uint32_t invMask = ~mask;
   uint32_t v;

   base = (uint32_t*) address;
   p = base;

   for(i=0;i<size;i++) *p++ = 0;//write 0 in m1

   p = base;

   for(i=0;i<size;i++) {//read 0 in m2, write 1
     v = *p;
     if( v == 0 )*p++ = mask; 
     else {
        errors++;p++;
        //mm_printError("step m2",(uint64_t)p,v,mask);
     }
   }

   p = base;

   for(i=0;i<size;i++) {//read 1 in m3, write 0
      v = *p;
      if( v == mask ) *p++ = 0;
      else {
         errors++;p++;
         //mm_printError("step m3",(uint64_t)p,v,invMask);
      }
   }

   return errors;

} /* mm_matsPlusseq32() */



/*==================== Function Separator =============================*/
uint32_t mm_matsPlusseq64( uint64_t address, uint32_t size )
{

   uint32_t i = 0;
   uint32_t errors = 0;
   uint64_t* base;
   uint64_t* p;
   uint64_t mask = 0xFFFFFFFFFFFFFFFF;
   //uint64_t invMask = ~mask;
   uint64_t v;

   base = (uint64_t*) address;
   p = base;

   for(i=0;i<size;i++) *p++ = 0;//write 0 in m1

   p = base;

   for(i=0;i<size;i++) {//read 0 in m2, write 1
      v = *p;
      if( v == 0 ) *p++ = mask;
      else {
         errors++;p++;
         //mm_printError("step m2",(uint64_t)p,v,mask);
      }
   }

   p = base;

   for(i=0;i<size;i++) {//read 1 in m3, write 0
      v = *p;
      if( v == mask ) *p++ = 0;
      else {
         errors++;p++;
         //mm_printError("step m3",(uint64_t)p,v,invMask);
      }
   }

   return errors;

} /* mm_matsPlusseq64() */




/*==================== Function Separator =============================*/
uint32_t mm_matsPlusseq( uint64_t address, uint32_t sizeInBytes, uint8_t logDataWidth)
{

   uint32_t errors = 0;
   uint32_t size = sizeInBytes >> logDataWidth;

   switch(logDataWidth){

     case 0:

        errors = mm_matsPlusseq8( address, size );

     break;

     case 1:

        errors = mm_matsPlusseq16( address, size );

     break;

     case 2:

        errors = mm_matsPlusseq32( address, size );

     break;

     case 3:

        errors = mm_matsPlusseq64( address, size ); 

     break;

   }

   return errors;

} /*mm_matsPlusseq() */



/*==================== Function Separator =============================*/
uint32_t mm_marchCseq8( uint64_t address, uint32_t size )
{

   uint32_t i = 0;
   uint32_t errors = 0;
   uint8_t* base;
   uint8_t* p;
   uint8_t mask = 0xFF;
   //uint8_t invMask = 0x00;
   uint8_t v;

   base = (uint8_t*) address;
   p = base;

   for(i=0;i<size;i++) *p++ = 0;//write 0 in m1

   p = base;

   for(i=0;i<size;i++) {//read 0 in m2, write 1
      v = *p;
      if( v == 0 )*p++ = mask;
      else {
         errors++;p++;
         //mm_printError("step m2",(uint64_t)p,v,mask);
      }
   }

   p = base;

   for(i=0;i<size;i++) {//read 1 in m3, write 0
      v = *p;
      if( v == mask ) *p++ = 0;
      else {
         errors++;p++;
         //mm_printError("step m3",(uint64_t)p,v,invMask);
      }
   }

   p = base;

   for(i=0;i<size;i++) {//read 0 in m4
      v = *p;
      if( v != 0 ) {
         errors++;
         //mm_printError("step m4",(uint64_t)p,v,0xFFFFFFFF);
      }
      p++;
   }

   return errors;

} /* mm_marchCseq8() */



/*==================== Function Separator =============================*/
uint32_t mm_marchCseq16( uint64_t address, uint32_t size )
{

   uint32_t i = 0;
   uint32_t errors = 0;
   uint16_t* base;
   uint16_t* p;
   uint16_t mask = 0xFFFF;
   //uint16_t invMask = 0x0000;
   uint16_t v;

   base = (uint16_t*) address;
   p = base;

   for(i=0;i<size;i++) *p++ = 0;//write 0 in m1

   p = base;

   for(i=0;i<size;i++) {//read 0 in m2, write 1
      v = *p;
      if( v == 0 ) *p++ = mask;
      else {
         errors++;p++;
         //mm_printError("step m2",(uint64_t)p,v,mask);
      }
   }

   p = base;

   for(i=0;i<size;i++) {//read 1 in m3, write 0
      v = *p;
      if( v == mask ) *p++ = 0;
      else {
         errors++;p++;
         //mm_printError("step m3",(uint64_t)p,v,invMask);
      }
   }

   p = base;

   for(i=0;i<size;i++) {//read 0 in m4
      v = *p;
      if( v != 0 ) {
         errors++;
         //mm_printError("step m4",(uint64_t)p,v,0xFFFFFFFF);
      }
      p++;
   }

   return errors;

} /* mm_marchCseq16() */



/*==================== Function Separator =============================*/
uint32_t mm_marchCseq32( uint64_t address, uint32_t size )
{

   uint32_t i = 0;
   uint32_t errors = 0;
   uint32_t* base;
   uint32_t* p;
   uint32_t mask = 0xFFFFFFFF;
   //uint32_t invMask = ~mask;
   uint32_t v;

   base = (uint32_t*) address;
   p = base;

   for(i=0;i<size;i++) *p++ = 0;//write 0 in m1

   p = base;

   for(i=0;i<size;i++) {//read 0 in m2, write 1
      v = *p;
      if( v == 0 ) *p++ = mask;
      else {
         errors++;p++;
         //mm_printError("step m2",(uint64_t)p,v,mask);
      }
   }

   p = base;

   for(i=0;i<size;i++) {//read 1 in m3, write 0
      v = *p;
      if( v == mask ) *p++ = 0;
      else {
         errors++;p++;
         //mm_printError("step m3",(uint64_t)p,v,invMask);
      }
   }

   p = base;

   for(i=0;i<size;i++) {//read 0 in m4
      v = *p;
      if( v != 0 ) {
         errors++;
         //mm_printError("step m4",(uint64_t)p,v,0xFFFFFFFF);
      }
      p++;
   }

   return errors;

} /* mm_marchCseq32() */



/*==================== Function Separator =============================*/
uint32_t mm_marchCseq64( uint64_t address, uint32_t size )
{

   uint32_t i = 0;
   uint32_t errors = 0;
   uint64_t* base;
   uint64_t* p;
   uint64_t mask = 0xFFFFFFFFFFFFFFFF;
   //uint64_t invMask = ~mask;
   uint64_t v;

   base = (uint64_t*) address;
   p = base;

   for(i=0;i<size;i++) *p++ = 0;//write 0 in m1

   p = base;

   for(i=0;i<size;i++) {//read 0 in m2, write 1
      v = *p;
      if( v == 0 ) *p++ = mask;
      else {
        errors++;p++;
        //mm_printError("step m2",(uint64_t)p,v,mask);
      }
   }

   p = base;

   for(i=0;i<size;i++) {//read 1 in m3, write 0
      v = *p;
      if( v == mask ) *p++ = 0;
      else {
        errors++;p++;
        //mm_printError("step m3",(uint64_t)p,v,invMask);
      }
   }

   p = base;

   for(i=0;i<size;i++) {//read 0 in m4
      v = *p;
      if( v != 0 ) {
        errors++;
        //mm_printError("step m4",(uint64_t)p,v,0xFFFFFFFF);
      }
      p++;
   }

   return errors;

} /* mm_marchCseq64() */



/*==================== Function Separator =============================*/
uint32_t mm_marchCseq( uint64_t address, uint32_t sizeInBytes, uint8_t logDataWidth)
{

   uint32_t errors = 0;
   uint32_t size = sizeInBytes >> logDataWidth;

   switch(logDataWidth){

     case 0:
        errors = mm_marchCseq8( address, size );
        break;

     case 1:
        errors = mm_marchCseq16( address, size );
        break;

     case 2:
        errors = mm_marchCseq32( address, size );
        break;

     case 3:
        errors = mm_marchCseq64( address, size ); 
        break;

   }

   return errors;

} /* mm_marchCseq() */



/*=====================================================================*/
/*==================== Functions ======================================*/
/*=====================================================================*/



/*==================== Function Separator =============================*/
uint32_t memmarch_marchC( uint64_t startAddress, uint32_t sizeInBytes, uint32_t dataWidth )
{

    uint32_t errors;
    uint8_t logDataWidth;

    switch( dataWidth ) {
        case 1:
            logDataWidth = 0;
            break;

        case 2:
            logDataWidth = 1;
            break;

        case 4:
            logDataWidth = 2;
            break;

        case 8:
            logDataWidth = 3;
            break;

        default:
            errors = 0x8000000;
            return errors;
    }

    errors = mm_marchCseq( startAddress, sizeInBytes, logDataWidth );

    return errors;

} /* memmarch_marchC() */



/*==================== Function Separator =============================*/
uint32_t memmarch_matsPlus( uint64_t startAddress, uint32_t sizeInBytes, uint32_t dataWidth )
{

    uint32_t errors;
    uint8_t logDataWidth;

    switch(dataWidth){
        case 1:
            logDataWidth = 0;
            break;

        case 2:
            logDataWidth = 1;
            break;

        case 4:
            logDataWidth = 2;
            break;

        case 8:
            logDataWidth = 3;
            break;

        default:
            errors = 0x80000000;
            return errors;
    }

    errors = mm_matsPlusseq( startAddress, sizeInBytes, logDataWidth);

    return errors;

} /* memmarch_matsPlus() */



/*==================== Function Separator =============================*/
uint32_t memmarch_march9( uint64_t startAddress, uint32_t sizeInBytes,int log2Banks, uint32_t dataWidth )
{

   uint32_t errors = 0;
   uint8_t logDataWidth = 0;
   uint64_t a = startAddress;
   uint32_t stride = sizeInBytes >> log2Banks;
   uint32_t sz = 32;

   switch(dataWidth){
     case 1:
        logDataWidth = 0;
     break;

     case 2:
        logDataWidth = 1;
     break;

     case 4:
        logDataWidth = 2;
     break;

     case 8:
        logDataWidth = 3;
     break;
   }

   for (int i=0;i<(1<<log2Banks);++i) {
      errors += mm_march9seq(a,sz,logDataWidth);
      errors += mm_march9seq(a + stride-sz,sz,logDataWidth );
      a += stride;
   }

  return errors;

}  /* memmarch_march9() */


/*     < EOF >     */
