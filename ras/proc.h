#ifndef PROC_H
#define PROC_H

#include <stdint.h>

uint8_t proc_read(int type, const char *arg);

void proc_finish(void);

#endif

