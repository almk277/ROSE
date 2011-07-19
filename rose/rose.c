#include "module.h"
#include <stdio.h>

int main(void)
{
	int err;
	int p;
	Module *m = module_load("../sources/out.rmd", &err);
	if(!m) {
		printf("error = %d\n", err);
		perror("module_load");
		return 1;
	}
	printf("module '%s' with version %hhu.%hhu\n", m->name,
			m->version[0], m->version[1]);
	p = module_find_proc(m, "main");
	if(p == -1)
		puts("main not found");
	else
		puts("main");
	module_unload(m);
	return 0;
}

