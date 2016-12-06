#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <process.h>
#include <process_list.h>

void push(process_list* list,process_data* process){
	if(list == NULL || process == NULL){
		printf("The arguments can't be NULL.\n");
		return;
	}
	process->state = ACTIVABLE;
	process->next = NULL;
	if(list->head == NULL){
		list->head = process;
		list->tail = list->head;
	}
	else {
		list->tail->next = process;
		list->tail = list->tail->next;
	}
	list->size += 1;
}

process_data* pop(process_list* list){
	if(list == NULL){
		return NULL;
	}
	process_data* process = list->head;
	process->state = ELECTED;
	list->head = list->head->next;
	list->size -= 1;
	if(list->head == NULL)
		list->tail = NULL;
	return process;
}

bool is_empty(process_list* list){
	if(list == NULL || list->head == NULL)
		return true;
	else
		return false;
}

void display_list(process_list* list){
	if(list == NULL || list->head == NULL)
		return;

	process_data* process = list->head;
	printf("head ---> %d\n",list->head->pid);
	printf("tail ---> %d\n",list->tail->pid);
	while(process != NULL){
		printf("%d ---> ",process->pid);
		process = process->next;
	}
	printf("NULL\n\n");
}
