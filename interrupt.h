#ifndef __Interrupt_H__
#define __Interrupt_H__

#include <inttypes.h>
#include <stdbool.h>

typedef struct {
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
} time;

uint32_t get_time(void);

void manager(void);

void set_interruption_mask(uint32_t number, bool mask);

void set_clock(void);

void clock_tick(void);

void init_interrupt_table(uint32_t number,void (*fct)(void));

#endif
