/* describes working with asm directives */

#ifndef DIRECT_H
#define DIRECT_H

/* describes asm directive */
struct Directive {
	const char *name;          /* directive name */
	void (*func)(char *args);  /* function to call on directive */
};

/* processes directive direct with arguments args */
void read_direct(const char *direct, char *args);

/* directive processing functions */
void dir_const(char *);
void dir_data(char *);
void dir_sub(char *);
void dir_arg(char *);
void dir_var(char *);
void dir_import(char *);
void dir_module(char *);
void dir_export(char *);
void dir_extern(char *);
void dir_str(char *);

#endif

