#include "loader.h"
#include "mm.h"
#include "queue.h"
#include "symbol.h"
#include <string.h>
#include <stdlib.h>

typedef struct StdPath StdPath;
typedef SIMPLEQ_HEAD(PathHead, StdPath) PathHead;
typedef SIMPLEQ_ENTRY(StdPath) PathEntry;

struct StdPath {
	unsigned char length;
	char buffer[256];
	PathEntry pe;
};

static MM_DECL(StdPath, 4);

static PathHead stdpath_list = SIMPLEQ_HEAD_INITIALIZER(stdpath_list);

static int add(const char *path, int length)
{
	StdPath *sp;
	if(length == 0 || length > 255)
		return 0;
	sp = mm_alloc(StdPath);
	sp->length = length;
	memcpy(sp->buffer, path, length);
	if(path[length - 1] != '/')
		sp->buffer[sp->length++] = '/';
	SIMPLEQ_INSERT_TAIL(&stdpath_list, sp, pe);
	return 1;
}

int loader_add_dir(const char *dir)
{
	return add(dir, strlen(dir));
}

void loader_add_dirs_from_env()
{
	const char *rosepath = getenv("ROSEPATH");
	if(rosepath) {
		const char *start, *end;
		for(start = rosepath, end = strchr(rosepath, ':'); end;
				start = end + 1, end = strchr(start, ':')) {
			add(start, end - start);
		}
		add(start, strlen(start));
	}
}

int loader_add_dir_from_file(const char *fname)
{
	const char *slash = strrchr(fname, '/');
	return slash ?
		add(fname, slash - fname + 1) : /* cut out last path component */
		add("./", 2);                   /* use current directory */
}

FILE *loader_find_module(const Symbol *name)
{
	StdPath *ent;
	SIMPLEQ_FOREACH(ent, &stdpath_list, pe) {
		FILE *f;
		static const char suffix[] = ".rmd";
		char *const buf = ent->buffer;
		const int dlen = ent->length;
		const int flen = symbol_length(name);
		const int len = dlen + flen;
		if(len + sizeof suffix > 255)
			continue;
		symbol_copy_to(name, buf + dlen);
		memcpy(buf + len, suffix, sizeof suffix);
		f = fopen(buf, "r");
		if(f)
			return f;
	}
	return NULL;
}

void loader_print_pathes()
{
	StdPath *ent;
	SIMPLEQ_FOREACH(ent, &stdpath_list, pe) {
		fwrite(ent->buffer, ent->length, 1, stdout);
		putchar('\n');
	}
}

