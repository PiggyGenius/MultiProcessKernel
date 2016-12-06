#include <stdio.h>
#include <cpu.h>
#include <inttypes.h>
#include <console.h>
#include <interrupt.h>
#include <process.h>
#include <sleeping_list.h>

#define CLOCK_INTERRUPTION 32

void kernel_start(void){
	init_interrupt_table(CLOCK_INTERRUPTION,manager);
	set_interruption_mask(0,false);
	set_clock();

	create_idle();
	create_process(print_process2,"Process 1");
	create_process(print_process3,"Process 2");
	create_process(print_process5,"Process 3");
	idle();
    while (1) {
        // cette fonction arrete le processeur
		hlt();
    }
}
