#include "general.h"
#include "buddy/buddy.h"
#include "slab/slab.h"
#include "mapping/paging.h"
#include "test/test.h"


uint64_t align(uint64_t addr, uint64_t need) {
	return addr % need == 0 ? addr : addr - addr % need + need;
}

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



void test() {
	testBuddyAllocator_1();
	testBuddyAllocator_2();
	testSlab_1(8, 1000);
	testSlab_1(4000, 100);
	testSlab_2(10);
	testSlab_2(3009);
}

void main(uint32_t ebx)
{

	struct desc_table_ptr ptr = {0, 0};
	write_idtr(&ptr);


	initSerialPort();
	
	initMemRegions((uint32_t *)(uint64_t)ebx);
	
	initBuddyAllocator();
	
	initNewPaging();	

	test();


	
	
	//initInterrupts();
	while(1);
}
