/* working with asm instructions */

#ifndef PROC_H
#define PROC_H

#include <stdint.h>

/* for instruction with given type and argument list arg,
 * returns operand */
uint8_t proc_read(int type, const char *arg);

/* this must be called after every procedure finish */
void proc_finish(void);

#endif

