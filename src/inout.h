#ifndef INOUT_H
#define INOUT_H
#include <stdint.h>
#include <stdio.h>

void init_display();
void draw(uint8_t* display);
void stop_display();

int findFileSize(char filename[]);
extern FILE *chkExist;
extern long int res;

#endif