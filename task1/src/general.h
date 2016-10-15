#ifndef __GENERAL_H__
#define __GENERAL_H__

#include <desc.h>
#include <ints.h>
#include <stdint.h>
#include <ioport.h>
#include <memory.h>

#define MASTER_BEGIN 32
#define bit(x) (1 << (x))

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
void initAllocators(uint32_t *);

#endif  /* __GENERAL_H__ */

