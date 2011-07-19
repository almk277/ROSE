#ifndef ROSE_DATA_H
#define ROSE_DATA_H

#include <stdint.h>

typedef struct Data Data;

struct Data {
	int32_t *start;
};

static inline int32_t data_at(const Data *data, int idx)
{
	return *data->start[idx];
}

#endif

