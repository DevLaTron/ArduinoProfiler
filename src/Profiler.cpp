#if (ARDUINO >= 100)
    #include <Arduino.h>
#else
    #include <WProgram.h>
#endif

extern unsigned int __heap_start;
extern void *__brkval;

/*
 * The free list structure as maintained by the 
 * avr-libc memory allocation routines.
 */
struct __freelist {
  size_t sz;
  struct __freelist *nx;
};

/* The head of the free list structure */
extern struct __freelist *__flp;

#include "Profiler.h"

Profiler::Profiler()
{
   _initFreeRAM = getFreeRAM();
   _initFreeBlock = getFreeBlock();
}

// Get difference between available RAM at start and currently.
int Profiler::getRAMUsage()
{
    return _initFreeRAM - getFreeRAM();
}

// Get difference between largest Block size available at start and currently.
int Profiler::getBlockUsage()
{
    return _initFreeBlock - getFreeBlock();
}

// Get RAM available when Object was instantiated.
int Profiler::getInitRAM()
{
    return _initFreeRAM;
}

// Get largest continous Block size available when Object was instantiated.
int Profiler::getInitBlock()
{
    return _initFreeBlock;
}

// Calculates the size of the free list
int Profiler::freeListSize() {
  struct __freelist* current;
  int total = 0;
  for (current = __flp; current; current = current->nx) {
    total += 2; // Add two bytes for the memory block's header
    total += (int) current->sz;
  }
  return total;
}


// Get the amount of free RAM available.
int Profiler::getFreeRAM() {
  int free_memory;
  if ((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__heap_start);
  } else {
    free_memory = ((int)&free_memory) - ((int)__brkval);
    free_memory += freeListSize();
  }
  return free_memory;
}

// Get the largest available Block size that can be allocated.
int Profiler::getFreeBlock()
{
    return _getBlockSize(0, RAMEND-RAMSTART + 1);
}

// Recursive routine used internally to get largest available Block size.
int Profiler::_getBlockSize(uint16_t min, uint16_t max) {

  if(min==max-1) return min;

  int size = max;
  int lastSize = size;

  byte *buf;
  while((buf = (byte*) malloc(size)) == NULL) {
      lastSize = size;
      size -=(max-min)/2;
  };

  free(buf); 
  return _getBlockSize(size, lastSize);
}
