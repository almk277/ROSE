#include "error.h"
#include <stdio.h>
#include <stdlib.h>

void error(int code)
{
	fprintf(stderr, "Critical error %d\n", code);
	exit(1);
}

