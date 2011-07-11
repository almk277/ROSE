#include <iostream>
#include "module.h"
#include "thread.h"

int main(int argc, char *argv[])
{
	if(argc == 1) {
		std::cerr << "No input\n";
		return 1;
	}

	Module module(argv[1]);
	Thread th;
	th.run(module);
}

