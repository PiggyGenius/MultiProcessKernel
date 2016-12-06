#ifndef PROCESS_H
#define PROCESS_H
#include <inttypes.h>

#define REGISTER_COUNT 5

void ctx_sw(int32_t* registers,int32_t* stack);

typedef enum {ELECTED,ACTIVABLE,SLEEPING} procstate;

typedef struct process_data {
	int32_t pid;
	char* name;
	procstate state;
	int32_t registers[REGISTER_COUNT];
	int32_t* stack;
	uint32_t sleep_count;
	struct process_data* next;
} process_data;

void create_idle(void);

void scheduler(void);

void sleep(uint32_t sleep_count);

uint32_t create_process(void (*fct)(void),char* name);

void print_process(void);
void print_process2(void);
void print_process3(void);
void print_process5(void);

void idle(void);

#endif
