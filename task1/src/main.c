#include "general.h"
/*void makeInt0() {
	__asm__ volatile("int $0" : : : "memory", "cc");
}
void makeInt10() {
	__asm__ volatile("int $10" : : : "memory", "cc");
}
void makeInt32() {
	__asm__ volatile("int $32" : : : "memory", "cc");
}*/
void makeInt() {
	__asm__ volatile("int $33" : : : "memory", "cc");
}

void main(void)
{

	struct desc_table_ptr ptr = {0, 0};
	write_idtr(&ptr);



	setupPort();
	printStr("Testing serial port: ");
	printInt(10);
	printChar('\n');
	
	IDTsetup();
	PICsetup();
	
	enablePICInts();	
	enable_ints();
//	makeInt();
	printStr("PIT\n");
	PITsetup();
	while(1);
//	makeInt();
//	makeInt10();
//	makeInt10();	
//	makeInt32();
}
