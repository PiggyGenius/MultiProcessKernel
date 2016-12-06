#ifndef SLEEPING_LIST_H
#define SLEEPING_LIST_H
#include <process_list.h>

typedef struct {
	process_data* head;
	uint32_t size;
} sleeping_list;


void add(sleeping_list* list,process_data* process);

process_data* remv(sleeping_list* list);

void print_list(sleeping_list* list);

bool is_ready(sleeping_list* list);

#endif
