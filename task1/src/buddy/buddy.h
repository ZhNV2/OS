#ifndef __BUDDY_H__
#define __BUDDY_H__

#include "buddy_node.h"
#include "buddy_desc.h"
#include "../threads/thread.h"
#include "../general.h"

#define MAX_ORDER 20

extern struct memRegion memRegions[];
extern int cntRegions;

typedef struct buddyAllocator {
	int used;
	int maxOrder;
	int memRegionNum;
	uint64_t descriptorsBegin;
	uint64_t listsBegin;
	Node* ordList[MAX_ORDER];
	Node* stackTop;
	uint64_t memBegin;
} BuddyAllocator;




BuddyAllocator buddyAllocators[MAX_NUM_OF_REGIONS];

int getDescNum(int i, uint64_t addr);

Descriptor getDesc(int i, int desc);

void nullDesc(int i, int desc);

void setDesc(int i, int desc, Node *node, int ord, int isFree);

void init(int i);

void addToList(int i, int listNum, Node);


void eraseFromList(int i, int listNum, Node *toDelete);


uint64_t allocPhys(uint64_t size);

void freePhys(uint64_t addr);

void initBuddyAllocator();


#endif // __BUDDY_H_