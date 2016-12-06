#ifndef ACTIVABLE_LIST_H
#define ACTIVABLE_LIST_H

#include <process.h>

typedef struct {
	process_data* head;
	process_data* tail;
	int32_t size;
} activable_list;

extern activable_list* ACTIVABLE_LIST;
process_data* ACTIVE_PROCESS;

void ordonancer(void);

void push(activable_list* list,process_data* process);

process_data* pop(activable_list* list);

void display_list(activable_list* list);

#endif
