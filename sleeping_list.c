#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <interrupt.h>
#include <process_list.h>
#include <sleeping_list.h>

void print_list(sleeping_list* list){
	if(list == NULL || list->head == NULL){
		printf("Empty list.\n");
		return;
	}
	process_data* process = list->head;
	printf("head ---> %d\n",list->head->pid);
	while(process != NULL){
		printf("%d ---> ",process->pid);
		process = process->next;
	}
	printf("NULL\n\n");
}

void add(sleeping_list* list,process_data* process){
	if(list == NULL || process == NULL){
		printf("Arguments are both null.\n");
		return;
	}
	process->state = SLEEPING;
	if(list->head == NULL || process->sleep_count <= list->head->sleep_count){
		process->next = list->head;
		list->head = process;
	}
	else {
		process_data* current = list->head;
		process_data* next = list->head->next;
		while(next != NULL){
			if(process->sleep_count <= next->sleep_count){
				break;
			}
			current = next;
			next = current->next;
		}
		process->next = next;
		current->next = process;
	}
	list->size += 1;
}

bool is_ready(sleeping_list* list){
	if(list == NULL || list->head == NULL)
		return false;
	else {
		if(list->head->sleep_count <= get_time())
			return true;
		else
			return false;
	}
}

process_data* remv(sleeping_list* list){
	if(list == NULL || list->head == NULL){
		return NULL;
	}

	process_data* process = list->head;
	process->state = ACTIVABLE;
	list->head = list->head->next;
	list->size -= 1;
	return process;
}
