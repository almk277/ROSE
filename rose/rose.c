#include "module.h"
#include "thread.h"
#include <stdio.h>

static int file_error(const char *name, int error)
{
	const char *msg;
	switch(error) {
		case RMD_FILE:
			msg = "can not open file"; break;
		case RMD_READ:
			msg = "can not read file"; break;
		case RMD_NO_MEMORY:
			msg = "out of memory"; break;
		case RMD_BAD_IDENT:
			msg = "file has no ROSE signature"; break;
		case RMD_BAD_VERSION:
			msg = "unsupported module version"; break;
		default:
			msg = "unknown error";
	}
	fprintf(stderr, "%s: %s\n", name, msg);
	return 1;
}

static void init(void)
{
	module_init();
}

int main(int argc, char *argv[])
{
	int err;
	const char *fname = (argc == 1) ? "out.rmd" : argv[1];
	Module *m;
	Thread t;

	init();
	thread_init(&t);
   	m = module_load_file(fname, &err);
	if(!m)
		return file_error(fname, err);
	printf("module '%s' with version %hhu.%hhu\n", m->name,
			m->version[0], m->version[1]);

	if(thread_start(&t, m) != THS_EXIT)
		puts("thread interrupt");
	module_unload(m);
	return 0;
}

