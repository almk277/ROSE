/* We serialize numbers in little endian. Support big endian as well. */

#ifndef ENDIAN_H
#define ENDIAN_H

#define ENDIAN_BIG (1L >> 1)

/* Only stubs for now */

#define serial_8(x)  (x)
#define serial_16(x) (x)
#define serial_32(x) (x)

#define deserial_8(x)  (x)
#define deserial_16(x) (x)
#define deserial_32(x) (x)

/* Generic macro. In general, they should be preferred */

#define serial(x) \
	(sizeof(x) == 1 ? serial_8(x) : \
	 sizeof(x) == 2 ? serial_16(x) : \
	 sizeof(x) == 4 ? serial_32(x) : \
	 printf("error: %s:%d\n", __FILE__, __LINE__))

#define deserial(x) \
	(sizeof(x) == 1 ? deserial_8(x) : \
	 sizeof(x) == 2 ? deserial_16(x) : \
	 sizeof(x) == 4 ? deserial_32(x) : \
	 printf("error: %s:%d\n", __FILE__, __LINE__))

#endif

