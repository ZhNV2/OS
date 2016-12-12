#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "../slab/slab.h"
#include "../general.h"

#define MAX_NUM_THREADS 100

//  queue start


int inc(int pointer);
void push(int thread_id);
int pop();

//  queue finish


#define STACK_SIZE 1000


#define ALIVE 1
#define CLOSED 0

int addThread(void (*runThread)(void));
void closeThread(int thread_id);
void join(int thread_id);

void initScheduler();
void runScheduler();

void interrupt();

void switch_threads(uint64_t prev, uint64_t next);

void safe_switch_threads(uint64_t prev, uint64_t next);

typedef struct resp {
	uint64_t *prev;
	uint64_t next;
} Resp;

Resp toRun(int);


#endif // __SHEDULER_H__