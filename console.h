#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <inttypes.h>

typedef struct {
	int32_t line;
	int32_t column;
} cursor;

void clear_screen(void);

void write_time();

void console_putbytes(const char *s, int len);

#endif
