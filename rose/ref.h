/* References */
#ifndef ROSE_REF_H
#define ROSE_REF_H

#include <stdint.h>

typedef struct Ref Ref;

struct Ref {
	uint32_t _ref;
};

/* Makes reference from ptr */
Ref ref_new(void *ptr);

/* Deletes reference */
void ref_delete(Ref ref);

/* Dereferences ref */
void *ref_get(const Ref ref);

/* Converts reference to word */
static inline int32_t ref_to(const Ref ref)
{
	return ref._ref;
}

/* Converts word to reference */
static inline Ref ref_from(int32_t val)
{
	Ref ref = { val };
	return ref;
}

#endif

