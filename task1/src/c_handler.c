#include "general.h"

void c_handler(uint64_t *rsp) {
	uint64_t no = *rsp;
	uint64_t er = *(rsp + 1);
	static uint8_t cnt = 0;
	if (cnt == 0) {
		printStr("Interrupt number:");
		printInt(no);
		printChar(' ');
		printInt(er);
		printChar('\n');
	}
	cnt = (cnt + 1) % 30;
	if (MASTER_BEGIN <= no && no < MASTER_BEGIN + 8) EOI(no - MASTER_BEGIN);		
	if (er == 0) return;  // in order to use errorCode
}
