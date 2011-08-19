#ifndef ROSE_ERROR_H
#define ROSE_ERROR_H

enum Error {
	ERR_ILLEGAL_INSTR,
	ERR_REF_FAULT,
	ERR_REF_OVER,
};

void error(int code);

#endif

