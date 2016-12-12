#include "scheduler.h"

int currentThread = -1;

int threadQueue[MAX_NUM_THREADS];
int threadQueueBeg, threadQueueEnd;
int freeThreads[MAX_NUM_THREADS];
int freeThread ;


void (*threadCode[MAX_NUM_THREADS]) (void);
int threadStatus[MAX_NUM_THREADS];
uint64_t stackPointer[MAX_NUM_THREADS];
uint64_t mainStackPointer;
uint64_t stackSlab;

void safe_switch_threads(uint64_t prev, uint64_t next) {
	if (*(uint64_t *)prev == next) return;
	switch_threads(prev, next);

}
int inc(int pointer) {
	return pointer + 1 == MAX_NUM_THREADS ? 0 : pointer + 1;
}

void push(int thread_id) {
	threadQueue[threadQueueEnd] = thread_id;
	threadQueueEnd = inc(threadQueueEnd);
}

int pop() {
	if (threadQueueBeg == threadQueueEnd) return -1;
	int thread_id = threadQueue[threadQueueBeg];
	threadQueueBeg = inc(threadQueueBeg);
	return thread_id;
}

void initScheduler() {
	stackSlab = initSlab(STACK_SIZE);
	for (int i = 0; i < MAX_NUM_THREADS; i++) {
		freeThreads[i] = i;
	}
	freeThread = MAX_NUM_THREADS;
}


int addThread(void (*runThread)(void)) {
	if (freeThread == 0) {
		return -1;
	}
	int curThread = freeThreads[freeThread - 1];
	freeThread--;
	
	push(curThread);
	threadCode[curThread] = runThread;
	threadStatus[curThread] = ALIVE;


	stackPointer[curThread] = allocLogical(stackSlab) + STACK_SIZE - 8 * 8;
	for (int i = 0; i < 7; i++)
		*(uint64_t*)(stackPointer[curThread] + i * 8) = 0;
	*(uint64_t*)(stackPointer[curThread] + 7 * 8) = (uint64_t)runThread;

	return curThread;
}

void closeThread(int thread_id) {
	threadStatus[thread_id] = CLOSED;
	freeThreads[freeThread++] = thread_id;
	freeLogical(stackSlab, stackPointer[thread_id] - stackPointer[thread_id] % STACK_SIZE);
	Resp toSwitch = toRun(0);
	safe_switch_threads((uint64_t)toSwitch.prev, toSwitch.next);
}

void join(int thread_id) {
	while (threadStatus[thread_id] != CLOSED) {
		Resp toSwitch = toRun(0);
		safe_switch_threads((uint64_t)toSwitch.prev, toSwitch.next);
	}
}


void runScheduler() {
	Resp toSwitch;
	toSwitch = toRun(1);
	safe_switch_threads((uint64_t)toSwitch.prev, toSwitch.next);
	freeSlab(stackSlab);
	if (freeThread != MAX_NUM_THREADS) {
		printlnStr("kek");
	}
}


Resp toRun(int isBegging) {

	Resp res;
	res.prev = isBegging ? &mainStackPointer : &stackPointer[currentThread];
	if (currentThread != -1) {
		push(currentThread);
	}
	while (1) {
		int thread_id = pop();
		if (thread_id == -1) break;
		if (threadStatus[thread_id] == CLOSED) continue;
		res.next = stackPointer[thread_id];
		currentThread = thread_id;
		return res;
	}	

	safe_switch_threads((uint64_t)&stackPointer[currentThread], mainStackPointer);
	return res;
}

