#ifndef ACTIVABLE_LIST_H
#define ACTIVABLE_LIST_H

#include <process.h>
#include <stdbool.h>

typedef struct {
	process_data* head;
	process_data* tail;
	int32_t size;
} process_list;

extern process_list* ACTIVABLE_LIST;
process_data* ACTIVE_PROCESS;

void ordonancer(void);

void push(process_list* list,process_data* process);

process_data* pop(process_list* list);

void display_list(process_list* list);

bool is_empty(process_list* list);

#endif
