/* Works with #text, #ptbl, #addr and #exp */

#ifndef TEXT_H
#define TEXT_H

#include <stdio.h>
#include <stdint.h>
struct HashEntry;

/* adds procedure name to #ptbl and returns long-live procedure name */
const struct HashEntry *ptbl_add(const char *name);

/* returns procedure index or -1 if not found */
int ptbl_find(const char *name);

/* debug ouput for #ptbl */
void ptbl_print(void);

/* dumps #ptbl into file */
void ptbl_write(FILE *file);

/* #ptbl element count */
uint8_t ptbl_count(void);

/* debug output for procedure */
void proc_print(void);

/* adds variable with name */
void var_add(const char *name);

/* returns index of variable, or -1 if not found */
int var_find(const char *name);

/* clears all variables */
void var_clear(void);

/* debug output for variables */
void var_print(void);

/* adds argument with name */
void arg_add(const char *name);

/* returns index of argument, or -1 if not found */
int arg_find(const char *name);

/* clears argument list */
void arg_clear(void);

/* debug output for arguments */
void arg_print(void);

/* adds new instruction to #text */
void text_add(uint8_t opcode, uint8_t oper);

/* debug output for #text */
void text_print(void);

/* dumps #text to file */
void text_write(FILE *file);

/* #text element count */
uint32_t text_count(void);

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

/* #addr element count */
uint8_t addr_count(void);

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

/* #exp element count */
uint8_t exp_count(void);

#endif

