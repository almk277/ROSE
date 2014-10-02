#ifndef CMDOPT_H
#define CMDOPT_H

#define CMDOPT_DELIM '-'
#include <string.h>
#include <stdio.h>

/* option value types */
enum CMDOPT_FMT {
	CMDOPT_NONE,
	CMDOPT_LONG, /* long int */
	CMDOPT_LONG1,/* long int with default value: 1 */
	CMDOPT_STR,  /* char* */
};

/* option descriptor */
struct cmdopt {
	const char *option;      /* option name; NULL stands for end of array */
	enum CMDOPT_FMT format;  /* expected value type */
	void *data;              /* pointer to option variable */
};

/*
 * Parses command line
 * argc, argv - main() arguments
 * opts - array of option descriptions as above.
 *   Last element must be filled with 0's.
 * Returns: if negative - an error occuried;
 *          else - index of first non-option argument,
 *          or 0 if no such an argument.
 */
static int cmdopt(int argc, char *argv[], struct cmdopt *opts);

/* Example:
 * ./a.out [-debug] [-verbose[=n]] [-config=name] fname
 *
 * int debug, verb;
 * char *config_name;
 * struct cmdopt opts[] = {
 * 	{ "debug", CMDOPT_NONE, &debug },
 * 	{ "verbose", CMDOPT_LONG1, &verb },
 * 	{ "config", CMDOPT_STR, &config_name },
 * 	{ NULL, 0, NULL }
 * };
 *
 * For: ./a.out -debug -verbose=2 -config=opt.ini input1.txt input2.txt
 *   debug: 1, verb: 2, config_name: "opt.ini",
 *   returns: 4
 */

static int cmd_error(const char *arg, const char *msg)
{
	fprintf(stderr, "%s: %s\n", arg, msg);
	return -1;
}

static int cmdopt(int argc, char *argv[], struct cmdopt *opts)
{
	int i;
	for(i = 1; i < argc; ++i) {
		char *cur_arg = argv[i];
		if(*cur_arg == CMDOPT_DELIM) {
			/* option */
			struct cmdopt *cur_opt;
			char *value;
			int found = 0;
			++cur_arg; /* get rid of '-' */
			value = strchr(cur_arg, '=');
			if(value)
				*value++ = 0;
			for(cur_opt = opts; cur_opt->option; ++cur_opt)
				if(strcmp(cur_arg, cur_opt->option) == 0) {
					if(!value && (cur_opt->format == CMDOPT_LONG
								|| cur_opt->format == CMDOPT_STR))
						return cmd_error(cur_arg, "value expected");
					switch(cur_opt->format) {
						case CMDOPT_NONE:
							if(value)
								return cmd_error(cur_arg, "value not expected.");
							*(int *)cur_opt->data = 1; break;
						case CMDOPT_LONG:
						case CMDOPT_LONG1:
							if(value) {
								if(sscanf(value, "%ld", (long*)cur_opt->data) != 1)
									return cmd_error(cur_arg, "can not parse");
							} else
								*(int *)cur_opt->data = 1;
							break;
						case CMDOPT_STR:
							*(char**)cur_opt->data = value; break;
						default:
							return cmd_error(cur_arg, "wrong format");
					}
					found = 1;
					break;
				}
			if(!found)
				return cmd_error(cur_arg, "unknown option.");
		} else
			return i;
	}
	return 0;
}

#endif

