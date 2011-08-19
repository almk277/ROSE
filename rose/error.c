#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void error(int code)
{
	fprintf(stderr, "Critical error %d\n", code);
	exit(1);
}

