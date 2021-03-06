#ifndef __SLAB_H__
#define __SLAB_H__

#include "../general.h"
#include "../buddy/buddy.h"
#include "../threads/thread.h"

struct SlabS {
	uint64_t freeVar;
	uint64_t varSize;
	uint64_t nextSlab;
	uint64_t slabSize;
	spinlock lock;
	
}__attribute__((packed));

typedef struct SlabS Slab;

/* Every Slab has this structure in memory:

uint64_t freeVar; 0
uint64_t varSize; 8
uint64_t nextSlab; 16
uint64_t slabSize; 24

memory for variables

*/

uint64_t initSlab(uint64_t varSize);

uint64_t allocLogical(uint64_t slab);

void freeLogical(uint64_t slab, uint64_t toFree);

void freeSlab(uint64_t slab);

void initSlabAllocator();	


#endif // __SLAB_H__