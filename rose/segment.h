#ifndef ROSE_SEGMENT_H
#define ROSE_SEGMENT_H

#include <fstream>
#include <string>

// Common memory segment
class Segment {
	public:
		Segment(const std::string& type): _start(0), _size(0), name(type)  {}
		virtual ~Segment() { free(); }
		void allocate(int size) throw(std::bad_alloc);
		void free();
		virtual int entsize() const = 0;
		void set_size(int val) { _size = val; }
		int size() const { return _size; }
	protected:
		char *_start;
	private:
		Segment(const Segment&);	// Not allowed
		int length;
		int _size;
		const std::string name;

	friend std::ifstream& operator>>(std::ifstream& stream, Segment&) throw(std::string);
};

std::ifstream& operator>>(std::ifstream& stream, Segment&) throw(std::string);

#endif

