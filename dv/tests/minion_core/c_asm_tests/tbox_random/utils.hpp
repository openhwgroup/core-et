
#pragma once

inline bool texsend(unsigned number_packets, unsigned port_idx) {

  int error;
  uint64_t csr_data = port_idx | (number_packets<<4);

  __asm__ __volatile__ ("csrrw %[error], %[csr], %[rs]\n"
			: [error] "=r" (error) 
			: [csr] "i" (0x807), [rs] "r" (csr_data) ); 
  return error;
}

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



// STD FUNCTIONS
// the setup for STD is pretty slow so I defined the functios that I need
// to avoid std initialization

void my_memcpy(uint8_t* __restrict__ dest, uint8_t* __restrict__ src, uint64_t size) {
  // uint64_t* dest64 = (uint64_t*) dest;
  // uint64_t* src64 = (uint64_t*) src;
  // uint64_t i;
  // for(i = 0; i < size-(sizeof(uint64_t)-1); i+=sizeof(uint64_t))
  //   dest64[i] = src64[i];
    
  for(size_t i = 0; i < size; ++i)
    dest[i] = src[i];
}

void reverse(char str[], int length) 
{ 
    int start = 0; 
    int end = length -1; 
    while (start < end) 
    { 
      //swap(*(str+start), *(str+end)); 
      char aux = *(str+start);
      *(str+start) = *(str+end);
      *(str+end) = aux;
      start++; 
      end--; 
    } 
} 


// Implementation of itoa() 
char* itoa(int num, char* str, int base) 
{ 
  int i = 0; 
  bool isNegative = false; 
  
  /* Handle 0 explicitely, otherwise empty string is printed for 0 */
  if (num == 0) 
    { 
      str[i++] = '0'; 
      str[i] = '\0'; 
      return str; 
    } 
  
  // In standard itoa(), negative numbers are handled only with  
  // base 10. Otherwise numbers are considered unsigned. 
  if (num < 0 && base == 10) 
    { 
      isNegative = true; 
      num = -num; 
    } 
  
  // Process individual digits 
  while (num != 0) 
    { 
      int rem = num % base; 
      str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0'; 
      num = num/base; 
    } 
  
  // If number is negative, append '-' 
  if (isNegative) 
    str[i++] = '-'; 
  
  str[i] = '\0'; // Append string terminator 
  
  // Reverse the string 
  reverse(str, i); 
  
  return str; 
} 

