#include <stdio.h>
#include <stdlib.h>
#include <cpu.h>
#include <malloc.c.h>
#include <stddef.h>
#include <interrupt.h>
#include <activable_list.h>
#include <sleeping_list.h>
#include <process.h>

#define MAX_PROCESS 9
#define STACK_SIZE 512

activable_list* ACTIVABLE_LIST = NULL;
sleeping_list* SLEEPING_LIST = NULL;
uint32_t PROCESS_COUNT = 0;

void scheduler(void){
	while(is_ready(SLEEPING_LIST))
		push(ACTIVABLE_LIST,remv(SLEEPING_LIST));

	push(ACTIVABLE_LIST,ACTIVE_PROCESS);
	process_data* next_activable = pop(ACTIVABLE_LIST);

	process_data* old_activable = ACTIVE_PROCESS;
	ACTIVE_PROCESS = next_activable;
	ctx_sw(old_activable->registers,next_activable->registers);
}

void sleep(uint32_t sleep_count){
	ACTIVE_PROCESS->sleep_count = get_time() + sleep_count;
	add(SLEEPING_LIST,ACTIVE_PROCESS);
	process_data* next_activable = pop(ACTIVABLE_LIST);

	process_data* old_activable = ACTIVE_PROCESS;
	ACTIVE_PROCESS = next_activable;
	ctx_sw(old_activable->registers,next_activable->registers);
}

activable_list* create_stack(void){
	activable_list* list = malloc(sizeof(activable_list));
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return list;
}

sleeping_list* create_list(void){
	sleeping_list* list = malloc(sizeof(sleeping_list));
	list->head = NULL;
	list->size = 0;
	return list;
}

void print_process(void){
	for(;;){
		printf("[%s] pid = %i\n",ACTIVE_PROCESS->name,ACTIVE_PROCESS->pid);
		sti();
		hlt();
		cli();
	}
}

void print_process2(void){
	for(;;){
		printf("[%s] pid = %i\n",ACTIVE_PROCESS->name,ACTIVE_PROCESS->pid);
		sleep(2);
	}
}

void print_process3(void){
	for(;;){
		printf("[%s] pid = %i\n",ACTIVE_PROCESS->name,ACTIVE_PROCESS->pid);
		sleep(3);
	}
}

void print_process5(void){
	for(;;){
		printf("[%s] pid = %i\n",ACTIVE_PROCESS->name,ACTIVE_PROCESS->pid);
		sleep(5);
	}
}

void idle(void){
	for(;;){
		printf("[%s] pid = %i\n",ACTIVE_PROCESS->name,ACTIVE_PROCESS->pid);
		sti();
		hlt();
		cli();
	}
}

void create_idle(void){
	ACTIVABLE_LIST = create_stack();
	SLEEPING_LIST = create_list();
	process_data* proc = malloc(sizeof(process_data));
	proc->pid = 0;
	proc->name = "Process 0";
	proc->state = ELECTED;
	ACTIVE_PROCESS = proc;
	PROCESS_COUNT += 1;
}

uint32_t create_process(void (*fct)(void),char* name){
	if(PROCESS_COUNT == MAX_PROCESS || name == NULL){
		printf("Bad arguments.\n");
		return -1;
	}

	process_data* proc = malloc(sizeof(process_data));
	proc->stack = malloc(STACK_SIZE*sizeof(int32_t));
	proc->name = malloc((strlen(name)+1)*sizeof(char));
	strcpy(proc->name,name);

	proc->pid = PROCESS_COUNT;
	proc->registers[1] = (int32_t) &(proc->stack[STACK_SIZE-1]);
	proc->stack[STACK_SIZE-1] = (int32_t) fct;
	proc->next = NULL;
	push(ACTIVABLE_LIST,proc);
	return PROCESS_COUNT++;
}
