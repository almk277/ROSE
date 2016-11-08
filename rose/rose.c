#include "module.h"
#include "thread.h"
#include "symbol.h"
#include "loader.h"
#include "heap.h"

int main(int argc, char *argv[])
{
	Module *m;
	Thread t;
	const char *file = argc > 1 ? argv[1] : "out.rmd";
	loader_add_dir_from_file(file);
	loader_add_dirs_from_env();
	heap_create();

   	m = module_load_obligatory(file);
	if(thread_init(&t, m, "main") < 0)
		return 1;
	thread_run(&t);

	return 0;
}

