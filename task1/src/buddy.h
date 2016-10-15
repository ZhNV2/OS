#ifndef __BUDDY_H__
#define __BUDDY_H__

#include "buddy_node.h"
#include "buddy_desc.h"
#include "general.h"

#define SMALLEST_BLOCK_SIZE (1 << 12) // 4KB
#define MAX_ORDER 20

extern struct memRegion memRegions[];
extern int cntRegions;

typedef struct buddyAllocator {
	int used;
	int maxOrder;
	int memRegionNum;
	uint32_t descriptorsBegin;
	uint32_t listsBegin;
	Node* ordList[MAX_ORDER];
	Node* stackTop;
	uint32_t memBegin;
} BuddyAllocator;




uint32_t align(uint32_t addr, uint32_t need);

BuddyAllocator buddyAllocators[MAX_NUM_OF_REGIONS];

int getDescNum(int i, uint32_t addr);

Descriptor getDesc(int i, int desc);

void nullDesc(int i, int desc);

void setDesc(int i, int desc, Node *node, int ord, int isFree);

void init(int i);

void addToList(int i, int listNum, Node);


void eraseFromList(int i, int listNum, Node *toDelete);


uint32_t allocPhys(uint32_t size);

void freePhys(uint32_t addr);

void initBuddyAllocator();


#endif // __BUDDY_H_