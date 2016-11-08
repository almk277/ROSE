#ifndef ROSE_CMDOPT_H
#define ROSE_CMDOPT_H

/*
   Parses command string in format:
   % command -opt1 -opt2=val param

   To parse it, make structure as follows:
   struct cmdopt s = {
     { "opt1", &i1, NULL, "1st option" },
     { "opt2", NULL, set_opt2, "2nd option with value" },
     { NULL, NULL, set_param, "parameter" }
   };
 */

struct cmdopt {
	const char *option;
	int *flag;
	void (*handler)(const char *value);
	const char *description;
};

/*
   For the example above, does this:
   i1 = 1;
   set_opt2("val");
   set_param("param");
 */
int cmdopt_parse(int argc, char *const argv[], const struct cmdopt *opts);

/*
   For the example above, prints this:
   "opt1   1st option"
   "opt2=v 2nd option with value"
 */
void cmdopt_print(const struct cmdopt *opts);

#endif

