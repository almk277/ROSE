/* We serialize numbers in little endian. Support big endian as well. */

#ifndef ENDIAN_H
#define ENDIAN_H

#define ENDIAN_BIG (1L >> 1)

/* Only stubs for now */

#define serial_16(x) (x)
#define serial_32(x) (x)

#define deserial_16(x) (x)
#define deserial_32(x) (x)

#endif

