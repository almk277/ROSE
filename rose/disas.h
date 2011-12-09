#ifndef ROSE_DISAS_H
#define ROSE_DISAS_H

#include <stdint.h>
#include <stdio.h>
struct Module;
struct RMDHeader;

typedef struct DisasInfo DisasInfo;

const char *disas_str(const struct Module *module, uint32_t addr);

void disas_load_sym(struct Module *m, const struct RMDHeader *h);

#endif

