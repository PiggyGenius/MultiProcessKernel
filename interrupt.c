#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <console.h>
#include <segment.h>
#include <cpu.h>
#include <stdbool.h>
#include <interrupt.h>
#include <process.h>

#define INTERRUPT_PORT 0x20
#define INTERRUPT_DATA 0x21
#define ACK_INT 0x20
#define INTERRUPT_TABLE 0x1000
#define INTERRUPT_CONST 0x00008E00

#define CLOCK_COMMAND 0x43
#define CLOCK_DATA 0x40
#define NEW_FREQUENCY 0x34
#define QUARTZ 0x1234DD
#define FREQUENCY 50


uint32_t TICK_COUNT = 0;
time CLOCK = {0,0,0};

uint32_t get_time(void){
	uint32_t current_time = CLOCK.sec;
	current_time += CLOCK.min * 60;
	current_time += CLOCK.hour * 3600;
	return current_time;
}

void update_time(void){
	CLOCK.sec += 1;
	if(CLOCK.sec == 60){
		CLOCK.sec = 0;
		CLOCK.min += 1;
		if(CLOCK.min == 60){
			CLOCK.min = 0;
			CLOCK.hour += 1;
			if(CLOCK.hour == 24)
				CLOCK.hour = 0;
		}
	}
	char time[9];
	sprintf(time,"%02u",CLOCK.hour);
	time[2] = ':';
	sprintf(time+3,"%02u",CLOCK.min);
	time[5] = ':';
	sprintf(time+6,"%02u",CLOCK.sec);
	write_time(time);
}

void clock_tick(void){
	outb(ACK_INT,INTERRUPT_PORT);
	TICK_COUNT += 1;
	if(TICK_COUNT == FREQUENCY){
		TICK_COUNT = 0;
		update_time();
	}
	scheduler();
}

void set_interruption_mask(uint32_t number, bool mask){
	uint8_t table = inb(INTERRUPT_DATA);
	if(mask)
		table |= 1 << number;
	else
		table &= ~(1 << number);
	outb(table,INTERRUPT_DATA);
}

void init_interrupt_table(uint32_t number,void (*fct)(void)){
	uint32_t* interruption = (uint32_t*) (INTERRUPT_TABLE + (2 * 4 * number));
	*interruption = (((uint32_t) KERNEL_CS) << 16) | (((uint32_t) fct) & 0x0000FFFF);
	*(interruption+1) = (((uint32_t) fct) & 0xFFFF0000) | INTERRUPT_CONST;
}

void set_clock(void){
	outb(NEW_FREQUENCY,CLOCK_COMMAND);
	outb((uint8_t) (QUARTZ / FREQUENCY),CLOCK_DATA);
	outb((QUARTZ / FREQUENCY) >> 8,CLOCK_DATA);
}
