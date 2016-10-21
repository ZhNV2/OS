#ifndef __PAGING_H__
#define __PAGING_H__

#include "general.h"
#include "buddy.h"

#define TABLE_ENTRY_SIZE 8
#define GB (1LL << 30)
	

uint64_t buildEntry(uint64_t physAddres, int ps);

void putEntry(uint64_t entry, int num, uint64_t table);

extern void get_cr3();
uint64_t cr3;
void save_cr3();


void initNewPaging();

#endif // __PAGING_H__