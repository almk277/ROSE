/* Works with #text, #ptbl and #addr */

#ifndef TEXT_H
#define TEXT_H

#include <stdio.h>
#include <stdint.h>
struct HashEntry;

/* adds procedure name to #ptbl and returns long-live procedure name */
const struct HashEntry *proc_add(const char *name);

/* returns procedure index or -1 if not found */
int proc_find(const char *name);

/* debug ouput for #ptbl */
void proc_print(void);

/* dumps #ptbl into file */
void proc_write(FILE *file);

/* #ptbl length */
size_t proc_length(void);

/* adds new instruction to #text */
void text_add(uint8_t opcode, uint8_t oper);

/* debug output for #text */
void text_print(void);

/* dumps #text to file */
void text_write(FILE *file);

/* #text length */
size_t text_length(void);

/* stores label with given name and address */
void label_add(const char *name, uint32_t addr);

/* writes to *addr address of label 'name' and returns if it is found */
int label_find(const char *name, uint32_t *addr);

/* clears all labels */
void label_clear(void);

/* debug output for labels */
void label_print(void);

/* adds address addr of label to #addr and returns it's index */
int addr_add(const char *label, uint32_t addr);

/* debug output for #addr */
void addr_print(void);

/* dumps #addr to file */
void addr_write(FILE *file);

/* #addr length */
size_t addr_length(void);

/* stores reference to unknown label for near jump */
void forward_ref_near(const char *label);

/* stores reference to unknown label for far jump */
void forward_ref_far(const char *label);

/* for all stored references, looks for labels and corrects instructions */
void ref_resolve(void);

/* adds procedure proc to #exp */
void exp_add(const struct HashEntry *proc);

/* debug output for #exp */
void exp_print(void);

/* dumps #exp to file */
void exp_write(FILE *file);

#endif

