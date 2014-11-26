#ifndef CMDOPT_H
#define CMDOPT_H

struct cmdopt {
	const char *option;
	int *flag;
	void (*handler)(const char *value);
	const char *description;
};

int cmdopt_parse(int argc, char *const argv[], const struct cmdopt *opts);

void cmdopt_print(const struct cmdopt *opts);

#endif

