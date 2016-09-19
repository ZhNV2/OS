#include "general.h"

const uint8_t MasterCom = 0x20;
const uint8_t MasterData = 0x21;
const uint8_t SlaveCom = 0xA0;
const uint8_t SlaveData = 0xA1;

void PICsetup() {	
	//setup Master
	out8(MasterCom, 0x11);
	out8(MasterData, MASTER_BEGIN); // 32 - the first desc in IDT
	out8(MasterData, 4);  // slave at 2 leg
	out8(MasterData, 1);  // unuseless data

	//setup Slave
	out8(SlaveCom, 0x11);
	out8(SlaveData, MASTER_BEGIN + 8); // 32 + 8
	out8(SlaveData, 2);  // at 2 leg
	out8(SlaveData, 1);  // unuseless data

	disablePICInts();
	disable_ints();
}	

void disablePICInts() {
	out8(MasterData, (1 << 8) - 1);
	out8(SlaveData, (1 << 8) - 1);
}

void enablePICInts() {
	out8(MasterData, 0);
	out8(SlaveData, 0);
} 

void EOI(short InterruptPort) {
	out8(MasterCom, 0x60 + InterruptPort);
}
