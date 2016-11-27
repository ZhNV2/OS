#include "slab.h"


void initSlabAllocator() {
}

uint64_t initSlab(uint64_t varSize) {
	lock(&INIT_SLAB_LOCK);
	if (varSize >= PHYS_PAGE_SIZE || varSize < 2) {
		printlnStr("Error: size is invalid");
		unlock(&SLAB_LOCK);	
		return 0;
	}
	varSize = varSize < 8 ? 8 : varSize;
	uint64_t slabSize = (varSize * 8 / PHYS_PAGE_SIZE + 1) * PHYS_PAGE_SIZE;
	uint64_t slab = allocPhys(slabSize);
	*(uint64_t *)(slab + 8) = varSize;
	*(uint64_t *)(slab + 16) = 0;
	*(uint64_t *)(slab + 24) = slabSize;
	uint64_t varBegin = align(slab + 32, varSize);
	*(uint64_t *)slab = varBegin;
	uint64_t curVar = varBegin;
	while (curVar + varSize <= slab + slabSize) {
		if (curVar + 2 * varSize <= slab + slabSize) {
			*(uint64_t *)curVar = (curVar + varSize);
		} else {
			*(uint64_t *)curVar = 0;
		}
		curVar += varSize;
	} 
	printlnInt(slab);
	unlock(&INIT_SLAB_LOCK);	
	return slab;
}

uint64_t allocLogical(uint64_t slab) {
	lock(&SLAB_LOCK);
	for (;;) {
		uint64_t nextSlab = *(uint64_t *)(slab + 16);
		uint64_t freeVar = *(uint64_t *)slab;
		if (freeVar) break;
		if (!nextSlab) break;
		slab = nextSlab;
	}
	uint64_t freeVar = *(uint64_t *)slab;
	uint64_t varSize = *(uint64_t *)(slab + 8);
	if (freeVar == 0) {
		
		uint64_t newSlab = initSlab(varSize);
		*(uint64_t *)(slab + 16) = newSlab;
		slab = newSlab;
		freeVar = *(uint64_t *)slab;
	} 
	*(uint64_t *)slab = *(uint64_t *)freeVar;
	unlock(&SLAB_LOCK);
	return freeVar;
}

void freeLogical(uint64_t slab, uint64_t toFree) {
	lock(&SLAB_LOCK);
	for (;;) {
		uint64_t slabSize = *(uint64_t *)(slab + 24);
		uint64_t nextSlab = *(uint64_t *)(slab + 16);
		
		if (slab <= toFree && toFree < slab + slabSize) break;
		slab = nextSlab;
	}
	uint64_t freeVar = *(uint64_t *)slab;
	*(uint64_t *)toFree = freeVar;
	*(uint64_t *)slab = toFree;
	unlock(&SLAB_LOCK);
}

void freeSlab(uint64_t slab) {
	lock(&SLAB_LOCK);
	for (;;) {
		uint64_t nextSlab = *(uint64_t *)(slab + 16);
		freePhys(slab);
		if (!nextSlab) break;
		slab = nextSlab;
	}
	unlock(&SLAB_LOCK);
}