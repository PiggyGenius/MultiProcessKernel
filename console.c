#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <cpu.h>
#include <console.h>

#define MEMORY 0xB8000
#define COLUMN_COUNT 80
#define LINE_COUNT 25
#define DEFAULT_FORMAT 0x0F

#define COMMAND_PORT 0x3D4
#define DATA_PORT 0x3D5
#define LOWER_VALUE 0x0F
#define UPPER_VALUE 0x0E


uint16_t* memory_pointer(uint32_t line, uint32_t column){
	return (uint16_t*) (MEMORY + 2 * (line * COLUMN_COUNT + column));
}

void write_char(uint32_t line, uint32_t column, char c,uint8_t format){
	uint16_t* word = memory_pointer(line,column);
	*word = (((uint16_t) format) << 8) | c;
}

void write_time(char* s){
	uint32_t len = strlen(s);
	if(len > COLUMN_COUNT)
		len = COLUMN_COUNT;
	for(uint32_t i = 0;i < len;i++)
		write_char(0,COLUMN_COUNT-len+i,s[i],DEFAULT_FORMAT);
}

void clear_line(uint32_t line){
	for(uint32_t i = 0;i < COLUMN_COUNT; i++)
		write_char(line,i,' ',DEFAULT_FORMAT);
}

void scroll_screen(){
	size_t size = 2 * (LINE_COUNT-1) * COLUMN_COUNT;
	memmove((void*) MEMORY,(void*) (MEMORY + (2 * COLUMN_COUNT)),size);
	clear_line(LINE_COUNT-1);
}

void put_cursor(uint32_t line, uint32_t column){
	if(column == COLUMN_COUNT){
		line += 1;
		column = 0;
	}
	if(line == LINE_COUNT){
		scroll_screen();
		line -= 1;
	}
	uint16_t position = column + line * COLUMN_COUNT;
	outb(LOWER_VALUE,COMMAND_PORT);
	outb((uint8_t) position,DATA_PORT);
	outb(UPPER_VALUE,COMMAND_PORT);
	outb((uint8_t) (position >> 8),DATA_PORT);
}

cursor get_cursor(void){
	uint16_t position;
	cursor screen_cursor;
	
	outb(UPPER_VALUE,COMMAND_PORT);
	position = inb(DATA_PORT);
	position = position << 8;

	outb(LOWER_VALUE,COMMAND_PORT);
	position |= inb(DATA_PORT);

	screen_cursor.line = position / COLUMN_COUNT;
	screen_cursor.column = position % COLUMN_COUNT;
	return screen_cursor;
}

void clear_screen(void){
	for(uint32_t i = 0;i < LINE_COUNT;i++){
		for(uint32_t j = 0;j < COLUMN_COUNT;j++)
			write_char(i,j,' ',DEFAULT_FORMAT);
	}
	put_cursor(0,0);
}

void dispatch_char(char c,uint16_t format){
	if(c >= 0 && c <= 127){
		cursor screen_cursor = get_cursor();
		if(c >= 32 && c <= 126){
			write_char(screen_cursor.line,screen_cursor.column,c,format);
			put_cursor(screen_cursor.line,screen_cursor.column + 1);
		}
		else {
			switch(c){
				case '\b':
					if(screen_cursor.column != 0)
						put_cursor(screen_cursor.line,screen_cursor.column - 1);
					break;
				case '\t':
					if(screen_cursor.column >= COLUMN_COUNT-8)
						put_cursor(screen_cursor.line+1,0);
					else
						put_cursor(screen_cursor.line,screen_cursor.column+(8-(screen_cursor.column%8)));
					break;
				case '\n':
					put_cursor(screen_cursor.line+1,0);
					break;
				case '\f':
					clear_screen();
					break;
				case '\r':
					put_cursor(screen_cursor.line,0);
					break;
			}
		}
	}
}

extern void console_putbytes(const char *s, int len){
	for(uint32_t i = 0;i < len;i++){
		dispatch_char(s[i],DEFAULT_FORMAT);
	}
}
