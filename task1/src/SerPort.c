#include "general.h"

void setupPort() {
	unsigned short basePort = 0x3f8;
	
	out8(basePort + 3, bit(7));   // to setup Div
	
	uint16_t Div = 12;
	out8(basePort + 0, Div & (bit(8) - 1)); // smaller bits
	out8(basePort + 1, Div >> 8);           // bigger bits
	
	uint8_t setup = bit(0) + bit(1);
	out8(basePort + 3, setup);   // setup cadr's format

	out8(basePort + 1, 0);        // turn off interrups
}

void printChar(char c) {
	unsigned short basePort = 0x3f8;
	out8(basePort + 0, c);
	while (1) {
		uint8_t port5 = in8(basePort + 5);
		if ((port5 >> 5) & 1) break;
	}	
}

void printStr(char *str) {
	while (*str) {
		printChar(*str);
		str++;
	}
}

void printInt(uint64_t x) {
	int a[20];
	int r = 0;
	while (x) {
		a[r++] = x % 10;
		x /= 10;
	}
	for (int i = r - 1; i >= 0; i--)
		printChar('0' + a[i]);
	if (r == 0) printChar('0');
}

void debug(char *str, uint64_t x) {
	printStr(str);
	printStr(":");
	printlnInt(x);
}

void printlnInt(uint64_t x) {
	printInt(x);
	printChar('\n');
}
void printlnStr(char *str) {
	printStr(str);
	printChar('\n');
}

void printIntHex(uint64_t x) {
	int a[16];
	for (int i = 0; i < 16; i++) {
		a[i] = x % 16;
		x /= 16;
	}
	printStr("0x");
	for (int i = 16 - 1; i >= 0; i--)
		printChar(a[i] < 10 ? '0' + a[i] : 'a' + (a[i] - 10));
}
void printIntBin(uint64_t x) {
	int a[64];
	for (int i = 0; i < 64; i++) {
		a[i] = x % 2;
		x /= 2;
	}
	printStr("0b");
	for (int i = 64 - 1; i >= 0; i--)
		printChar('0' + a[i]);
}

