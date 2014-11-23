#include "cmdopt.h"
#include <stdio.h>
#include <string.h>

static void handle(const struct cmdopt *opt, const char *param)
{
	if(opt->flag)
		*opt->flag = 1;
	if(opt->handler)
		opt->handler(param);
}

int cmdopt_parse(int argc, char *const argv[], const struct cmdopt *opts)
{
	int arg;
	const struct cmdopt *param_opt = opts;
	while(param_opt->option)
		++param_opt;
	for(arg = 1; arg != argc; ++arg) {
		const char *s = argv[arg];
		if(s[0] != '-') /* non-option parameter */
			handle(param_opt, s);
		else {
			const char *eq = strchr(++s, '='); /* drop out '-' */
			if(eq) { /* parameter with value */
				size_t param_len = eq - s;
				const struct cmdopt *opt;
				for(opt = opts; opt->option; ++opt)
					if(strncmp(s, opt->option, param_len) == 0
							&& opt->option[param_len] == '\0') {
						handle(opt, eq + 1);
						goto loop;
					}
				return arg;
			} else { /* parameter without value */
				const struct cmdopt *opt;
				for(opt = opts; opt->option; ++opt)
					if(strcmp(s, opt->option) == 0) {
						handle(opt, NULL);
						goto loop;
					}
				return arg;
			}
		}
loop: ;
	}
	return 0;
}

void cmdopt_print(const struct cmdopt *opts)
{
	const struct cmdopt *opt;
	int maxlen = 0;
	/* calculate maximum option length */
	for(opt = opts; opt->option; ++opt) {
		int len = strlen(opt->option);
		if(len > maxlen)
			maxlen = len;
	}
	for(opt = opts; opt->option; ++opt)
		printf("  -%-*s%s  %s\n", maxlen, opt->option,
				opt->handler ? "=v" : "  ", opt->description);
}

