#ifndef __TEST_H__
#define __TESH_H__

#include "general.h"
#include "buddy.h"
#include "slab.h"

void testBuddyAllocator_1();
void testBuddyAllocator_2();
void testSlab_1(int varSize, int it);
void testSlab_2(int varSize);

#endif // __TEST_H__