#include "paging.h"

	
void save_cr3(uint64_t cur_cr3) {
	cr3 = cur_cr3;
}

uint64_t buildEntry(uint64_t physAddres, int ps) {
	return bit(0) + bit(1)  + (physAddres) + (ps ? bit(7) : 0);
}


void putEntry(uint64_t entry, int num, uint64_t table) {
	*(uint64_t *)(table + num * TABLE_ENTRY_SIZE) = entry;
}

void printTable(uint64_t table) {
	return;
	for (int i = 0; i < 512; i++) {
		if ((*(uint64_t *)table) != 0) {
			printInt(i);
			printChar(' ');
			printIntBin(*(uint64_t *)table);
			printChar('\n');
		}
		table += 8;
	}
}

void initNewPaging() {
	printlnStr("------PAGING BEGIN-------------");
	get_cr3();
	uint64_t PML4 = cr3;
	//delete first mapping

	// correct mapping after hole

	uint64_t PDPT = (((*(uint64_t *)(PML4 + 256 * TABLE_ENTRY_SIZE)) >> 12) * (1 << 12));
	printTable((uint64_t)PDPT);
	for (int i = 1; i < 500; i++) {
		//printInt(i);
		putEntry(buildEntry((uint64_t)i * GB, 1), i, PDPT);
	}
	//after
	putEntry(0, 0, PML4);

	printTable((uint64_t)PML4);
	printTable((uint64_t)PDPT);



	printlnStr("------PAGING FINISH-------------");
	
}