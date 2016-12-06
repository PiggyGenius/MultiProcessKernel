#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <process.h>
#include <activable_list.h>

void push(activable_list* list,process_data* activable){
	if(list == NULL || activable == NULL){
		printf("The arguments can't be NULL.\n");
		return;
	}
	activable->state = ACTIVABLE;
	activable->next = NULL;
	if(list->head == NULL){
		list->head = activable;
		list->tail = list->head;
	}
	else {
		list->tail->next = activable;
		list->tail = list->tail->next;
	}
	list->size += 1;
}

process_data* pop(activable_list* list){
	if(list == NULL){
		return NULL;
	}
	process_data* activable = list->head;
	activable->state = ELECTED;
	list->head = list->head->next;
	list->size -= 1;
	if(list->head == NULL)
		list->tail = NULL;
	return activable;
}

void display_list(activable_list* list){
	if(list == NULL || list->head == NULL)
		return;

	process_data* activable = list->head;
	printf("head ---> %d\n",list->head->pid);
	printf("tail ---> %d\n",list->tail->pid);
	while(activable != NULL){
		printf("%d ---> ",activable->pid);
		activable = activable->next;
	}
	printf("NULL\n\n");
}
