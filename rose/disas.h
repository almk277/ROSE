#ifndef ROSE_DISAS_H
#define ROSE_DISAS_H

#include <stdint.h>
#include <stdio.h>
struct Module;

const char *disas_str(const struct Module *module, uint32_t addr);

#endif

