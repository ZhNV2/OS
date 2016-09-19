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
void printIntBin(uint64_t);
//IDT
void IDTsetup();

//PIC
void PICsetup();
void disablePICInts();
void enablePICInts();
void EOI(short);

//PIT
void PITsetup();
#endif  /* __GENERAL_H__ */

