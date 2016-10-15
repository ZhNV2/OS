#include "general.h"



struct mem {
	uint32_t size;
	uint64_t base_addr;
	uint64_t length;
	uint64_t type;
}__attribute__((packed));

void print_info(uint32_t start, uint32_t finish, uint32_t type) {
	printStr((char*)("memory range : "));
	printIntHex(start);
	printChar('-');
	printIntHex(finish);
	printStr((char*)(",type="));
	printChar(type == 1 ? '1' : '2');
	printChar('\n');
}

struct memRegion memRegions[MAX_NUM_OF_REGIONS];
int cntRegions;

int handlerMemMap(uint32_t *ebx) {

	if (!(((*ebx) >> 6) & 1)) {
		printStr((char*)("Fail, mmap field is invalid\n"));
		return 0;
	}
	uint32_t mmap_length = *(ebx + 11);
	uint32_t *mmap_addr = (uint32_t*)(uint64_t)*(ebx + 12);
	struct mem* cur_mem = (struct mem*)mmap_addr;
	int i = 0;
	while ((char*)cur_mem - (char*)mmap_addr < mmap_length) {
		print_info(cur_mem->base_addr, cur_mem->base_addr + cur_mem->length - 1, cur_mem->type);
		if ((uint32_t)cur_mem->type == 1) {
			memRegions[i].begin = cur_mem->base_addr;
			memRegions[i].end =  cur_mem->base_addr + cur_mem->length;
			i++;
		} 
		cur_mem = (struct mem*)((char*)cur_mem + 4 + cur_mem->size);
	}
	return i;
}

extern char text_phys_begin[];
extern char bss_phys_end[];

void printMemRegions() {
	for (int i = 0; i < cntRegions; i++) {
		printInt(memRegions[i].begin);
		printChar('-');
		printInt(memRegions[i].end);
		printStr("==");
		printInt((memRegions[i].end - memRegions[i].begin) / 1024 / 1024);
		printStr("MB\n");
	}
}

int eraseCoreCode() {
	uint32_t coreCodeBegin = (uint64_t)(uint64_t *)text_phys_begin;
	uint32_t coreCodeEnd = (uint64_t)(uint64_t *)bss_phys_end + 1;
	printlnInt(coreCodeBegin);
	printlnInt(coreCodeEnd);
	int i = 0;
	while (i < cntRegions) {
		if (memRegions[i].end <= coreCodeBegin || coreCodeEnd <= memRegions[i].begin) {
			i++;
			continue;
		}
		if (coreCodeBegin <= memRegions[i].begin && memRegions[i].end <= coreCodeEnd) {
			for (int j = i + 1; j < cntRegions; j++)
				memRegions[j - 1] = memRegions[j];
			cntRegions--;
			continue;
		}
		if (memRegions[i].begin <= coreCodeBegin && coreCodeEnd <= memRegions[i].end) {
			for (int j = cntRegions; j > i; j--) {
				memRegions[j] = memRegions[j - 1];
			}
			memRegions[i].end = coreCodeBegin;
			memRegions[i + 1].begin = coreCodeEnd;
			cntRegions++;
			break;
		}
		if (coreCodeBegin <= memRegions[i].begin) {
			memRegions[i].begin = coreCodeEnd;
		} else {
			memRegions[i].end = coreCodeBegin;
		}
	}
	return cntRegions;
}

void initAllocators(uint32_t *ebx) {
	cntRegions = handlerMemMap(ebx);
	
	printStr("After memmap's processing:\n");
	printMemRegions();

	cntRegions = eraseCoreCode();

	printStr("After core code erasing\n");
	printMemRegions();



}