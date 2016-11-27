#include "scheduler.h"

int currentThread = -1;

int threadQueue[MAX_NUM_THREADS];
int threadQueueBeg, threadQueueEnd;


void (*threadCode[MAX_NUM_THREADS]) (void);
int threadCnt;
int threadStatus[MAX_NUM_THREADS];
uint64_t stackPointer[MAX_NUM_THREADS];
uint64_t mainStackPointer;

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
}


int addThread(void (*runThread)(void)) {
	push(threadCnt);
	threadCode[threadCnt] = runThread;
	threadStatus[threadCnt] = ALIVE;


	stackPointer[threadCnt] = allocLogical(stackSlab) + 500;
	for (int i = 0; i < 7; i++)
		*(uint64_t*)(stackPointer[threadCnt] + i * 8) = 0;
	*(uint64_t*)(stackPointer[threadCnt] + 7 * 8) = (uint64_t)runThread;

	return threadCnt++;
}

void closeThread(int thread_id) {
	threadStatus[thread_id] = CLOSED;
	Resp toSwitch = toRun(0);
	switch_threads((uint64_t)toSwitch.prev, toSwitch.next);
}

void join(int thread_id) {
	while (threadStatus[thread_id] != CLOSED) {
		Resp toSwitch = toRun(0);
		switch_threads((uint64_t)toSwitch.prev, toSwitch.next);
	}
}


void runScheduler() {
	Resp toSwitch;
	toSwitch = toRun(1);
	switch_threads((uint64_t)toSwitch.prev, toSwitch.next);
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

	switch_threads((uint64_t)&stackPointer[currentThread], mainStackPointer);
	return res;
}

