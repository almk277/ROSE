#include "module.h"
#include <stdio.h>

int main(void)
{
	int err;
	Module *m = module_load("../sources/out.rmd", &err);
	if(!m) {
		printf("error = %d\n", err);
		perror("module_load");
		return 1;
	}
	printf("module '%s' with version %hhu.%hhu\n", m->name,
			m->version[0], m->version[1]);
	module_unload(m);
	return 0;
}

