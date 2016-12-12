#include "slab.h"



spinlock INIT_SLAB_LOCK;

void initSlabAllocator() {
}

Slab *getSlabStruct(uint64_t slabAddress) { return (Slab*)(uint64_t *)slabAddress; }

uint64_t initSlab(uint64_t varSize) {
	lock(&INIT_SLAB_LOCK);
	if (varSize >= PHYS_PAGE_SIZE || varSize < 2) {
		printlnStr("Error: size is invalid");
		unlock(&INIT_SLAB_LOCK);	
		return 0;
	}
	varSize = varSize < 8 ? 8 : varSize;
	uint64_t slabSize = (varSize * 8 / PHYS_PAGE_SIZE + 1) * PHYS_PAGE_SIZE;
	uint64_t slabAddress = allocPhys(slabSize);
	Slab slab;
	slab.varSize = varSize;
	slab.nextSlab = 0;
	slab.slabSize = slabSize;
	spinlock lock;
	slab.lock = lock;
	uint64_t varBegin = align(slabAddress + sizeof(Slab), varSize);
	slab.freeVar = varBegin;
	//*(uint64_t *)slab = varBegin;
	uint64_t curVar = varBegin;
	while (curVar + varSize <= slabAddress + slabSize) {
		if (curVar + 2 * varSize <= slabAddress + slabSize) {
			*(uint64_t *)curVar = (curVar + varSize);
		} else {
			*(uint64_t *)curVar = 0;
		}
		curVar += varSize;
	} 
	*(Slab *)(uint64_t *)slabAddress = slab;
	unlock(&INIT_SLAB_LOCK);	
	return slabAddress;
}


uint64_t allocLogical(uint64_t slabAddress) {
	Slab *slab = getSlabStruct(slabAddress);
	spinlock *lock_ = &slab->lock; 
	lock(lock_);
	for (;;) {
		uint64_t nextSlab = slab->nextSlab;
		uint64_t freeVar = slab->freeVar;
		if (freeVar) break;
		if (!nextSlab) break;
		slabAddress = nextSlab;
		slab = getSlabStruct(slabAddress);
	}
	uint64_t freeVar = slab->freeVar;
	uint64_t varSize = slab->varSize;
	if (freeVar == 0) {
		uint64_t newSlab = initSlab(varSize);
		slab->nextSlab = newSlab;
		slabAddress = newSlab;
		slab = getSlabStruct(slabAddress);
		freeVar = slab->freeVar;
	} 
	slab->freeVar = *(uint64_t *)freeVar;
	unlock(lock_);
	return freeVar;
}

void freeLogical(uint64_t slabAddress, uint64_t toFree) {
	Slab *slab = getSlabStruct(slabAddress);
	spinlock *lock_ = &slab->lock;
	lock(lock_);
	for (;;) {
		uint64_t slabSize = slab->slabSize;
		uint64_t nextSlab = slab->nextSlab;
		
		if (slabAddress <= toFree && toFree < slabAddress + slabSize) break;
		slabAddress = nextSlab;
		slab = getSlabStruct(slabAddress);
	}
	uint64_t freeVar = slab->freeVar;
	*(uint64_t *)toFree = freeVar;
	slab->freeVar = toFree;
	unlock(lock_);
}

void freeSlab(uint64_t slabAddress) {
	Slab *slab = getSlabStruct(slabAddress);
	spinlock *lock_ = &slab->lock;

	lock(lock_);
	for (;;) {
		uint64_t nextSlab = slab->nextSlab;
		freePhys(slabAddress);
		if (!nextSlab) break;
		slabAddress = nextSlab;
		slab = getSlabStruct(slabAddress);
	}
	unlock(lock_);
}