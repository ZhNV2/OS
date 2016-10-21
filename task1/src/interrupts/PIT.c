#include "../general.h"

const uint8_t PITCom = 0x43;
const uint8_t PITData = 0x40;

void PITsetup() {
	uint8_t command = (2 << 1) + bit(4) + bit(5); // mode 2, bits 4,5 for init
	out8(PITCom, command);
	uint16_t INIT = 500;
	out8(PITData, INIT & ((1 << 8) - 1));
	out8(PITData, INIT >> 8);	
}
