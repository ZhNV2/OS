#include "test.h"

void testBuddyAllocator_1() {
	printlnStr("\ntestBuddyAllocator_1:\n");
	
	uint64_t addr1 = allocPhys(PHYS_PAGE_SIZE * 1);
	debug("addr1", (addr1 - ADDR_SHIFT) / PHYS_PAGE_SIZE);
	uint64_t addr2 = allocPhys(PHYS_PAGE_SIZE * 2);
	debug("addr2", (addr2 - ADDR_SHIFT) / PHYS_PAGE_SIZE);
	
	uint64_t addr3 = allocPhys(PHYS_PAGE_SIZE * 4);
	debug("addr3", (addr3 - ADDR_SHIFT)/ PHYS_PAGE_SIZE);
	uint64_t addr4 = allocPhys(PHYS_PAGE_SIZE * 1);
	debug("addr4", (addr4 - ADDR_SHIFT)/ PHYS_PAGE_SIZE);
	printlnStr("free addr4");
	

	freePhys(addr4);
	uint64_t addr5 = allocPhys(PHYS_PAGE_SIZE * 1);
	debug("addr5", (addr5 - ADDR_SHIFT) / PHYS_PAGE_SIZE);
	printlnStr("free addr5");
	//return;
	freePhys(addr5);
	printlnStr("free addr1");
	freePhys(addr1);
	printlnStr("free addr2");
	freePhys(addr2);
	uint64_t addr6 = allocPhys(PHYS_PAGE_SIZE * 4);
	debug("addr6", (addr6 - ADDR_SHIFT) / PHYS_PAGE_SIZE);
	printlnStr("free addr3");
	freePhys(addr3);
	printlnStr("free addr6");
	freePhys(addr6);
	uint64_t addr7 = allocPhys(PHYS_PAGE_SIZE * 8);
	debug("addr7", (addr7 - ADDR_SHIFT) / PHYS_PAGE_SIZE);
	printlnStr("free addr7");
	freePhys(addr7);
}

void testBuddyAllocator_2() {
	printlnStr("\ntestBuddyAllocator_2:\n");
	uint64_t addr[8];
	for (int it = 0; it < 1; it++) {
		addr[it] = allocPhys(PHYS_PAGE_SIZE * 1);
	 	printlnInt((addr[it] - ADDR_SHIFT) / PHYS_PAGE_SIZE);
	}
	for (int it = 0; it < 1; it++) {
		freePhys(addr[it]);
	}
	freePhys(addr[3]);
	freePhys(addr[0]);
	freePhys(addr[1]);
	freePhys(addr[7]);
	freePhys(addr[5]);
	freePhys(addr[6]);
	freePhys(addr[2]);
	freePhys(addr[4]);
	uint64_t addre = allocPhys(PHYS_PAGE_SIZE * 8);
	printlnInt(addre - ADDR_SHIFT);
}

void testSlab_1(int varSize, int it) {
	//return;
	printStr("\ntestSlab_1 varSize=");
	printInt(varSize);
	printStr(", it=");
	printInt(it);
	printlnStr("----");
	uint64_t slab = initSlab(varSize);
	printlnInt(slab - ADDR_SHIFT);
	printlnStr("FF");
	for (int i = 0; i < it; i++) {
		uint64_t ne = allocLogical(slab);
		ne = ne + 1;
	}
	printlnStr("FF");
	freeSlab(slab);
	printlnStr("FF");

}

void testSlab_2(int varSize) {
	printStr("\ntestSlab_2 varSize=");
	printInt(varSize);
	printlnStr("----");
	uint64_t slab = initSlab(varSize);
	for (int i = 0; i < 2; i++) {
		uint64_t addr1 = allocLogical(slab);
		debug("addr1", addr1 - slab);
		uint64_t addr2 = allocLogical(slab);
		debug("addr2", addr2 - slab);
		uint64_t addr3 = allocLogical(slab);
		debug("addr3", addr3 - slab);
		freeLogical(slab, addr1);
		freeLogical(slab, addr2);
		freeLogical(slab, addr3);
	}
	return ;
}

void run1(void);
void run2(void);

int run1ThreadId;
int run2ThreadId;
void testScheduler() {
	printlnStr("--------------test Scheduler----------------");
	initScheduler();
	run1ThreadId = addThread(run1);
	run2ThreadId = addThread(run2);
	printlnStr("runScheduler:");
	runScheduler();
	printlnStr("----------finished testScheduler------------");
}


int tmpCnt1;
int tmpCnt2;

#define ITER 50000000


void run1(void) {
	printlnStr("Start run1");
	
	for (;tmpCnt1 < ITER; tmpCnt1++) {
		if (tmpCnt1 % (ITER / 10) == 0) {
			//get();
			printStr("run1 tmpCnt1=");
			printlnInt(tmpCnt1);

			if (tmpCnt1 < ITER / 2) {
				Resp toSwitch = toRun(0);
				safe_switch_threads((uint64_t)toSwitch.prev, toSwitch.next);
			} else {
				join(run2ThreadId);
			}
			
			
		}
	}
	closeThread(run1ThreadId);
}

void run2(void) {
	printlnStr("Start run2");
	for (;tmpCnt2 < ITER; tmpCnt2++) {
		if (tmpCnt2 % (ITER / 10) == 0) {
			printStr("run2 tmpCnt2=");
			printlnInt(tmpCnt2);

			if (tmpCnt2 < ITER / 2) {
				Resp toSwitch = toRun(0);
				safe_switch_threads((uint64_t)toSwitch.prev, toSwitch.next);

			} else {

			}
			
			

		}
	}
	closeThread(run2ThreadId);
}
