/* Draft for new memory pool management API */

#ifndef ROSE_MEMPOOL_H
#define ROSE_MEMPOOL_H

#include <stddef.h> /* size_t */

struct MempoolArray;

struct MempoolHandle {

	/* public fields: fill them before first use */
	size_t size;
	size_t count;
	unsigned int flags;
	void *(*alloc)(size_t size);
	void (*free)(void *addr);
	void (*init)(void *addr);
	void *(*err_handler)(void);

	/* private fields: do not touch */
	struct MempoolArray *_memory;
	size_t _allocated;
	size_t _capacity;

};

#define MEMPOOL_INIT

void mempool_init(struct MempoolHandle *handle);

void *mempool_alloc(struct MempoolHandle *handle);

void mempool_free(struct MempoolHandle *handle, void *addr);

void mempool_destroy(struct MempoolHandle *handle);

size_t mempool_get_allocated(const struct MempoolHandle *handle);

size_t mempool_get_capacity(const struct MempoolHandle *handle);

void mempool_set_error_handler(void *(*err_handler)(void));


#define MEMPOOL_NAME(type)

#define MEMPOOL_INIT(type)

#define MEMPOOL_ALLOC(type)

#define MEMPOOL_FREE(type, addr)

#define MEMPOOL_DESTROY(type)


void *mempool_chain_alloc(struct MempoolHandle handles[], size_t size);

void mempool_chain_free(struct MempoolHandle handles[], void *addr);

#endif

