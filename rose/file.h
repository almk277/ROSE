#ifndef ROSE_FILE_H
#define ROSE_FILE_H

#include <stdint.h>

// File is an input/output absraction
class File {
	public:
		File();
		~File();
		void bind(int n);
	private:
		File(const File&);
		int fd;

		friend File& operator<<(File& file, int32_t word);
		friend File& operator>>(File& file, int32_t& word);
};

// Output to file
File& operator<<(File& file, int32_t word);

// Input from file
File& operator>>(File& file, int32_t& word);

#endif

