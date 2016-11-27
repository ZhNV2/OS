#ifndef __TEST_H__
#define __TESH_H__

#include "../general.h"
#include "../buddy/buddy.h"
#include "../slab/slab.h"
#include "../threads/scheduler.h"

void testBuddyAllocator_1();
void testBuddyAllocator_2();
void testSlab_1(int varSize, int it);
void testSlab_2(int varSize);
void testScheduler();


#endif // __TEST_H__
