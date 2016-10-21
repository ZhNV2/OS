#ifndef __GENERAL_H__
#define __GENERAL_H__

#include <desc.h>
#include <ints.h>
#include <stdint.h>
#include <ioport.h>
#include <memory.h>

#define ADDR_SHIFT ((((uint64_t)1 << 17) - 1) << 47)
#define MASTER_BEGIN 32
#define bit(x) (1 << (x))
#define PHYS_PAGE_SIZE (1 << 12)

//serial port
void setupPort();
void printChar(char);
void printStr(char *);
void printInt(uint64_t);
void printlnInt(uint64_t);
void debug(char *, uint64_t);
void printlnStr();
void printIntBin(uint64_t);
void printIntHex(uint64_t);

//IDT
void IDTsetup();

//PIC
void PICsetup();
void disablePICInts();
void enablePICInts();
void EOI(short);

//PIT
void PITsetup();

//handlerMemMap
#define NULL 0
#define MAX_NUM_OF_REGIONS 10
struct memRegion { uint32_t begin, end; };
void initMemRegions(uint32_t *);


//general
uint64_t align(uint64_t addr, uint64_t need); 

#endif  /* __GENERAL_H__ */

