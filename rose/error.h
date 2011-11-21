#ifndef ROSE_ERROR_H
#define ROSE_ERROR_H

enum Error {
	ERR_ILLEGAL_INSTR,
	ERR_REF_FAULT,
	ERR_REF_OVER,
	ERR_OUT_OF_BAND,
	ERR_OUT_OF_MEMORY,
	ERR_NO_MODULE,
	ERR_NO_PROC,
};

void error(int code);

static inline void check_ptr(void *ptr)
{
	if(!ptr)
		error(ERR_OUT_OF_MEMORY);
}

#endif

