#include "module.h"
#include <fstream>

Module::Module()
{
}

//inline void *addr(uint32_t a)
//{
	//return &_body[sizeof _header];
//}

void Module::load(const std::string& filename) throw(LoadException)
{
	std::ifstream file(filename.c_str(),
			std::ifstream::in | std::ifstream::binary);
	if(!file.is_open())
		throw LoadException(LoadException::OPEN);
	_filename = filename;
	file.read(&_header, sizeof _header);
	if(!file.good())
		throw LoadException(LoadException::READ);
	_size = _header.str - sizeof _header;
	try {
		_body = new char[_size];
	} catch(std::bad_alloc& exc) {
		throw LoadException(LoadException::MEMORY);
	}
	file.read(&_body, _size);
	if(!file.good())
		throw LoadException(LoadException::READ);

	_exp.place(&_body[0], _header.ptbl);
	_ptbl.place(&_body[_header.ptbl], _header.mtbl - _header.ptbl);
	_mtbl.place(&_body[_header.mtbl], _header.imp - _header.ptbl);
}

