#ifndef LOADER_H
#define LOADER_H

struct Symbol;
#include <stdio.h>

/* adds dir to list of module search directories.
 * Returns 1 if success, 0 otherwise (name is too long) */
int loader_add_dir(const char *dir);

/* adds directories from environment variable ROSEPATH.
 * Format is "dir1:dir2:dir3" */
void loader_add_dirs_from_env(void);

/* adds directory, containing file fname, to list of module search directories.
 * Returns 1 if success, 0 otherwise (name is too long) */
int loader_add_dir_from_file(const char *fname);

/* Searches for module 'name' in module search directories;
 * Returns opened file handler on success, or NULL, if not found
 * Not thread-safe! */
FILE *loader_find_module(const struct Symbol *name);

/* Prints out to stdout all search directories known so far */
void loader_print_pathes(void);

#endif

