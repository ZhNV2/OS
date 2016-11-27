#include "../general.h"
#include "../threads/scheduler.h"

void c_handler(uint64_t *rsp) {
	uint64_t no = *rsp;
	uint64_t er = *(rsp + 1);
	static uint8_t cnt = 0;
	if (cnt == 29) {
		printStr("Interrupt number:");
		printInt(no);
		printChar(' ');
		printInt(er);
		printChar('\n');
		
		Resp toSwitch = toRun(0);
		if (MASTER_BEGIN <= no && no < MASTER_BEGIN + 8) EOI(no - MASTER_BEGIN);
		switch_threads((uint64_t)toSwitch.prev, toSwitch.next);
	} else {
		printStr("Skipau Interrupt number:");
		printInt(no);
		printChar(' ');
		printInt(er);
		printChar('\n');

	}
	cnt = (cnt + 1) % 30;
	//enable_ints();
	if (MASTER_BEGIN <= no && no < MASTER_BEGIN + 8) EOI(no - MASTER_BEGIN);		
	
	if (er == 0) return;  // in order to use errorCode
}

