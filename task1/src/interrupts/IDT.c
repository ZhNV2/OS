#include "../general.h"

#define ITER_AMOUNT 41


struct IDTdesc {
	uint16_t offset1; // offset 0..15
	uint16_t segSel; // segment selector
	
	uint8_t zero1;
	uint8_t P_type; // P zero type
	uint16_t offset2; //16..31
	
	uint32_t offset3;  // 32..63
	
	uint32_t zero2;
	
}__attribute__((packed));

struct IDTdesc IDT[ITER_AMOUNT];

extern uint64_t tbl[ITER_AMOUNT];

// I took it from internet

static inline void lidt(uint64_t base, uint16_t limit)
{   
    struct {
        uint16_t limit;
        uint64_t base;
    } __attribute__((packed)) IDTR = { limit, base };
 
    __asm__ ( "lidt %0" : : "m"(IDTR) ); 
}

//

void IDTsetup() {
	for (int i = 0; i < ITER_AMOUNT; i++) {
		IDT[i].zero1 = 0;
		IDT[i].zero2 = 0;
		IDT[i].segSel = KERNEL_CS;
		IDT[i].offset1 = tbl[i] & ((1 << 16) - 1);
		IDT[i].offset2 = (tbl[i] >> 16) & ((1 << 16) - 1);
		IDT[i].offset3 = (tbl[i] >> 32);
		IDT[i].P_type = bit(7) +bit(3)	 + bit(2) + bit(1) + bit(0); // P = 1 && type == 0b1110
	}
	lidt((uint64_t)IDT, sizeof(IDT) - 1);					
}
