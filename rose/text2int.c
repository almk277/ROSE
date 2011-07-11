#include <stdio.h>
#include <unistd.h>

int main()
{
	unsigned long input;
	scanf("%lu", &input);
	write(1, &input, sizeof input);
	return 0;
}

