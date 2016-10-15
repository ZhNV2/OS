#include "general.h"
#include "buddy.h"


void initSerialPort() {
	setupPort();
}
void initInterrupts() {
	printStr("Testing serial port: ");
	printInt(10);
	printChar('\n');
	IDTsetup();
	PICsetup();
	enablePICInts();	
	enable_ints();
	printStr("PIT\n");
	PITsetup();
}

void testBuddyAllocator_1() {
	printlnStr("\ntestBuddyAllocator_1:\n");
	
	uint32_t addr1 = allocPhys(SMALLEST_BLOCK_SIZE * 1);
	debug("addr1", addr1 / SMALLEST_BLOCK_SIZE);
	uint32_t addr2 = allocPhys(SMALLEST_BLOCK_SIZE * 2);
	debug("addr2", addr2 / SMALLEST_BLOCK_SIZE);
	
	uint32_t addr3 = allocPhys(SMALLEST_BLOCK_SIZE * 4);
	debug("addr3", addr3 / SMALLEST_BLOCK_SIZE);
	uint32_t addr4 = allocPhys(SMALLEST_BLOCK_SIZE * 1);
	debug("addr4", addr4 / SMALLEST_BLOCK_SIZE);

	printlnStr("free addr4");
	freePhys(addr4);
	uint32_t addr5 = allocPhys(SMALLEST_BLOCK_SIZE * 1);
	debug("addr5", addr5 / SMALLEST_BLOCK_SIZE);
	printlnStr("free addr5");
	freePhys(addr5);
	printlnStr("free addr1");
	freePhys(addr1);
	printlnStr("free addr2");
	freePhys(addr2);
	uint32_t addr6 = allocPhys(SMALLEST_BLOCK_SIZE * 4);
	debug("addr6", addr6 / SMALLEST_BLOCK_SIZE);
	printlnStr("free addr3");
	freePhys(addr3);
	printlnStr("free addr6");
	freePhys(addr6);
	uint32_t addr7 = allocPhys(SMALLEST_BLOCK_SIZE * 8);
	debug("addr7", addr7 / SMALLEST_BLOCK_SIZE);
	printlnStr("free addr7");
	freePhys(addr7);
}

void testBuddyAllocator_2() {
	printlnStr("\ntestBuddyAllocator_2:\n");
	uint32_t addr[8];
	for (int it = 0; it < 1; it++) {
		addr[it] = allocPhys(SMALLEST_BLOCK_SIZE * 1);
	 	printlnInt(addr[it] / SMALLEST_BLOCK_SIZE);
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
	uint32_t addre = allocPhys(SMALLEST_BLOCK_SIZE * 8);
	printlnInt(addre);
}
void main(uint64_t ebx)
{

	struct desc_table_ptr ptr = {0, 0};
	write_idtr(&ptr);


	initSerialPort();
	
	initAllocators((uint32_t *)ebx);
	
	initBuddyAllocator();

	testBuddyAllocator_1();
	testBuddyAllocator_1();
	testBuddyAllocator_2();
	
	
	

	
	
	//initInterrupts();
	while(1);
}
