#ifndef __SLAB_H__
#define __SLAB_H__

#include "../general.h"
#include "../buddy/buddy.h"


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


#endif // __SLAB_H__