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

void handle_event(void);
extern uint8_t quit;

void wait(uint32_t MS_PER_FRAME);

#endif