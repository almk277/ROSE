#include "file.h"
#include <unistd.h>

File::File():
	fd(0)
{
}

File::~File()
{
}

void File::bind(int n)
{
	fd = n;
}

File& operator<<(File& file, int32_t word)
{
	::write(file.fd, &word, sizeof word);
	return file;
}

File& operator>>(File& file, int32_t& word)
{
	::read(file.fd, &word, sizeof word);
	return file;
}

