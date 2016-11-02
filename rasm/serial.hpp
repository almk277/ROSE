#pragma once
#include <type_traits>

template<typename OutputIterator, typename T>
std::enable_if_t<std::is_integral<T>::value>
serial(OutputIterator it, T val)
{
	auto v = static_cast<typename std::make_unsigned_t<T>>(val);
	if (sizeof v == 1)
		*it = v;
	else
		for (int i = 0; i != sizeof v; ++i) {
			*it++ = v & 0xFF;
			v >>= 8;
		}
}

template<typename OutputIterator>
void serial(OutputIterator it, float val)
{
	static_assert(sizeof(float) == 4, "unusual float");
	union {
		float f;
		uint32_t i;
	} u = { val };
	serial(it, u.i); // UB?
}

template<typename OutputIterator>
void serial(OutputIterator, double) = delete;
