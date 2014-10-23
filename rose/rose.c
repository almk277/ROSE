#include "module.h"
#include "symbol.h"
#include "loader.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	Module *m;
	const Symbol *name;
	const RMDVersion *ver;
	const char *err;
	const char *file = argc > 1 ? argv[1] : "out.rmd";
	loader_add_dir_from_file(file);
	loader_add_dirs_from_env();

   	m = module_load(file, &err);
	if(!m) {
		printf("%s: %s\n", file, err);
		return 1;
	}
	name = module_name(m);
	ver = module_version(m);
	symbol_print(module_name(m));
	printf(" %hhu.%hhu\n", ver->maj, ver->min);
	/*module_unload(m);*/
	return 0;
}

