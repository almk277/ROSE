/*
Copyright (c) 2016 Alexey Makarov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// ROSE assembler

// Note: some functions are declared as noexcept, even if they allocate.
// It just turns std::bad_alloc exception into std::terminate(), and it's OK.

#include "scanner.hpp"
#include "parser.hpp"
#include "module.hpp"
extern "C" {
#include "cmdopt.h"
}
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <locale>
#include <cstdlib>

using std::string;
using std::cerr;
using std::endl;
using error = std::runtime_error;

string inname;
string outname = "out.rmd";

struct option_error: public std::runtime_error {
	explicit option_error(const string &msg): std::runtime_error(msg) {}
};

int help_opt = 0;
const struct cmdopt opts[] = {
	{ "output", nullptr, [](const char *s) { outname = s; },
		"output file name" },
	{ "help", &help_opt, nullptr,
		"prints help" },
	{ nullptr, nullptr, [](const char *s) { inname = s; },
		"input file name" }
};

void parse_opt(int argc, char *argv[])
{
	int idx = cmdopt_parse(argc, argv, opts);
	if (idx)
		throw option_error(string(argv[idx]) + ": can't parse");
	if (help_opt) {
		cmdopt_print(opts);
		std::exit(0);
	}
	if (inname.empty())
		throw option_error("input file not specified");
	if (outname.empty())
		throw option_error("output file not specified");
}

int main(int argc, char *argv[])
{
	try {
		parse_opt(argc, argv);

		std::ifstream input(inname);
		if (!input)
			throw error(inname + ": can't open");

		module m;
		scanner scn(input);
		parser p(scn, m);
		p.run();

		std::ofstream output(outname, std::ios::out | std::ios::binary);
		if (!output)
			throw error(outname + ": can't create file");
		output.exceptions(std::ios::badbit | std::ios::failbit);
		output.imbue(std::locale::classic());
		output << m;
		output.close();

	} catch(option_error &opterr) {
		cerr << opterr.what() << endl;
		cerr << "usage: " << argv[0] << " [options] <filename>" << endl;
		cmdopt_print(opts);
		return 1;
	} catch(source_error &re) {
		cerr << argv[0] << ": " << inname << ":" << re.lineno
			<< ": " << re.what() << endl;
		return 1;
	} catch(std::exception &e) {
		cerr << argv[0] << ": " << e.what() << endl;
		return 1;
	}
}
