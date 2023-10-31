#ifndef INOUT_H
#define INOUT_H
#include <stdint.h>
#include <stdio.h>

int init_display(void);
void draw(uint8_t* display);
void stop_display(void);

int findFileSize(char filename[]);
extern FILE *chkExist;
extern long int res;

extern uint16_t x;
extern uint16_t y;

#endif