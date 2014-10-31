#include "module.h"
#include "thread.h"
#include "symbol.h"
#include "cpu.h"
#include "loader.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	Module *m;
	Thread t;
	const char *err;
	const char *file = argc > 1 ? argv[1] : "out.rmd";
	loader_add_dir_from_file(file);
	loader_add_dirs_from_env();

   	m = module_load(file, &err);
	if(!m) {
		printf("%s: %s\n", file, err);
		return 1;
	}
	thread_init(&t);
	if(!thread_jump_to(&t, m, symbol_store.main)) {
		printf("%s: procedure 'main' not found\n", file);
		return 1;
	}
	cpu_run(&t);

	return 0;
}

