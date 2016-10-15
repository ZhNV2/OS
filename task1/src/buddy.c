#include "buddy.h"

uint32_t align(uint32_t addr, uint32_t need) {
	return addr % need == 0 ? addr : addr - addr % need + need;
}

BuddyAllocator buddyAllocators[MAX_NUM_OF_REGIONS];


//desc functions
int getDescNum(int i, uint32_t addr) {
	return (addr - buddyAllocators[i].memBegin) / SMALLEST_BLOCK_SIZE;
}

Descriptor getDesc(int i, int desc) {
	Descriptor *tmp = (Descriptor*)(uint64_t)buddyAllocators[i].descriptorsBegin + desc;
	return *tmp;
}


void nullDesc(int i, int desc) {
	Descriptor *tmp = (Descriptor*)(uint64_t)buddyAllocators[i].descriptorsBegin + desc;
	*tmp = nullDescriptor();
}

void setDesc(int i, int desc, Node *node, int ord, int isFree) {
	Descriptor tmp;
	tmp.node = node;
	tmp.ord = ord;
	tmp.isFree = isFree;
	*((Descriptor*)(uint64_t)buddyAllocators[i].descriptorsBegin + desc) = tmp;
}
// fin desc functions


void init(int i) {
	int ord = -1;
	while (20 + (1 << (ord + 1)) * sizeof(Descriptor) + (1 << (ord + 2)) * sizeof(Node) 
				+ ((1 << ord) + 1)  * SMALLEST_BLOCK_SIZE <= memRegions[i].end - memRegions[i].begin)
		ord++;
	ord = 3;
	printStr("ord=");
	printlnInt(ord);
	printStr("size of Node:");
	printlnInt(sizeof(Node));
	printStr("size of Descriptor");
	printlnInt(sizeof(Descriptor));
	buddyAllocators[i].used = ord != -1;
	if (ord == -1) return;
	buddyAllocators[i].maxOrder = ord;
	
	buddyAllocators[i].descriptorsBegin = align(memRegions[i].begin, sizeof(Descriptor));

	buddyAllocators[i].listsBegin = buddyAllocators[i].descriptorsBegin + 
										(1 << buddyAllocators[i].maxOrder) * sizeof(Descriptor);
	buddyAllocators[i].listsBegin = align(buddyAllocators[i].listsBegin, sizeof(Node));


	
	for (Descriptor *tmp = (Descriptor*)(uint64_t)buddyAllocators[i].descriptorsBegin; (uint64_t)tmp + 1 < buddyAllocators[i].listsBegin; tmp++) {
		*tmp = nullDescriptor();
	}

	for (int i = 0; i <= buddyAllocators[i].maxOrder; i++) {
		buddyAllocators[i].ordList[i] = NULL;
	}

	buddyAllocators[i].stackTop = (Node*)(uint64_t)buddyAllocators[i].listsBegin;
	buddyAllocators[i].memBegin = buddyAllocators[i].listsBegin + sizeof(Node) * (1 << ord);
	buddyAllocators[i].memBegin = align(buddyAllocators[i].memBegin, SMALLEST_BLOCK_SIZE);
	printStr("descriptorsBegin:");
	printlnInt(buddyAllocators[i].descriptorsBegin);
	printStr("listsBegin:");
	printlnInt(buddyAllocators[i].listsBegin);
	printStr("memBegin:");
	printlnInt(buddyAllocators[i].memBegin);
	Node bigBlock = nullNode();
	bigBlock.begin = buddyAllocators[i].memBegin;
	addToList(i, buddyAllocators[i].maxOrder, bigBlock);
}

void addToList(int i, int listNum, Node node) {
	Node *cur = buddyAllocators[i].stackTop++;
	*cur = node;
	if (buddyAllocators[i].ordList[listNum] != NULL) {
		cur->next = buddyAllocators[i].ordList[listNum];
		buddyAllocators[i].ordList[listNum]->prev = cur;
	}
	setDesc(i, getDescNum(i, cur->begin), cur, listNum, 1);
	buddyAllocators[i].ordList[listNum] = cur;
}


void eraseFromList(int i, int listNum, Node *toDelete) {
	Node *cur = (buddyAllocators[i].stackTop - 1);
	if (cur != toDelete) {
		if (cur->next) cur->next->prev = toDelete;
		if (cur->prev) cur->prev->next = toDelete;
	}
	if (toDelete->prev) toDelete->prev->next = toDelete->next;
	if (toDelete->next) toDelete->next->prev = toDelete->prev;
	if (buddyAllocators[i].ordList[listNum] == toDelete) buddyAllocators[i].ordList[listNum] = toDelete->next;
	setDesc(i, getDescNum(i, toDelete->begin), toDelete, listNum, 0);
	*toDelete = *cur;
	buddyAllocators[i].stackTop--;
}
static uint32_t alloc(int i, int ord) {
	for (int lvl = ord; lvl <= buddyAllocators[i].maxOrder; lvl++) {
		if (buddyAllocators[i].ordList[lvl] != NULL) {
			for (int curLvl = lvl; curLvl > ord; curLvl--) {
				Node *cur = buddyAllocators[i].ordList[curLvl];
				uint32_t beg = cur->begin;
				eraseFromList(i, curLvl, cur);
				Node firstBuddy = nullNode(), secondBuddy = nullNode();
				firstBuddy.begin = beg;
				secondBuddy.begin = (beg + SMALLEST_BLOCK_SIZE * (1 << (curLvl - 1)));
				addToList(i, curLvl - 1, firstBuddy);
				addToList(i, curLvl - 1, secondBuddy);
			}
			Node *cur = buddyAllocators[i].ordList[ord];
			uint32_t beg = cur->begin;
			eraseFromList(i, ord, cur);
		
	
			 return beg;
		}
	}
	return NULL;
}

static void free(int i, uint32_t addr, int ord) {
	
	Node cur = nullNode();
	cur.begin = addr;
	for (int lvl = ord; lvl <= buddyAllocators[i].maxOrder; lvl++) {
		uint32_t buddy = (((cur.begin - buddyAllocators[i].memBegin) / SMALLEST_BLOCK_SIZE) ^ (1 << lvl)) 
							* SMALLEST_BLOCK_SIZE + buddyAllocators[i].memBegin;
		Descriptor buddyDesc = getDesc(i, getDescNum(i, buddy));
		
		if (lvl == buddyAllocators[i].maxOrder || buddyDesc.node == NULL 
				|| !buddyDesc.isFree || buddyDesc.ord != lvl) {
			addToList(i, lvl, cur);
			break;
		} else {
			//printlnStr("Joined");
			Node tmp = nullNode();
			tmp.begin = cur.begin < buddyDesc.node->begin ? cur.begin : buddyDesc.node->begin;
			eraseFromList(i, lvl, buddyDesc.node);
			cur = tmp; 
		}
	}	
}

uint32_t allocPhys(uint32_t size) {
	int ord = 0;
	while ((uint32_t)(1 << ord) * SMALLEST_BLOCK_SIZE < size) ord++;
	for (int i = 0; i < cntRegions; i++) {
		if (!buddyAllocators[i].used) continue;
		uint32_t addr = alloc(i, ord);
		if (addr) return addr;
	}
	return NULL;
}

void freePhys(uint32_t addr) {
	for (int i = 0; i < cntRegions; i++) {
		if (memRegions[i].begin <= addr && addr <= memRegions[i].end) {
			Descriptor tmp = getDesc(i, getDescNum(i, addr));
			free(i, addr, tmp.ord);
		}
	}

}

void initBuddyAllocator() {
	cntRegions = 1;
	for (int i = 0; i < cntRegions; i++) {
		buddyAllocators[i].memRegionNum = i;
	}
	for (int i = 0; i < cntRegions; i++) {
		init(i);
	}
}
