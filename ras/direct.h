#ifndef DIRECT_H
#define DIRECT_H

struct Directive {
	const char *name;
	void (*func)(char *args);
};

void read_direct(const char *direct, char *args);

void dir_const(char *);
void dir_data(char *);
void dir_sub(char *);
void dir_entry(char *);
void dir_arg(char *);
void dir_var(char *);
void dir_import(char *);
void dir_module(char *);
void dir_export(char *);
void dir_extern(char *);

#endif

