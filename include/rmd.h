#ifndef ROSE_RMD_H
#define ROSE_RMD_H

#include <stdint.h>

typedef struct RMDVersion {
	unsigned char maj;
	unsigned char min;
} RMDVersion;

typedef struct RMDHeader {
	unsigned char ident[4];    /* signature                 */
	RMDVersion rmd_version;    /* ROSE version              */
	uint16_t name;             /* module name               */
	uint16_t parent;           /* parent module name        */
	RMDVersion version;        /* module version            */
	uint8_t flags;             /* various flags             */
	uint8_t datac;             /* data count                */
	uint8_t exp;               /* export table size         */
	uint8_t ptbl;              /* procedure table size      */
	uint8_t mtbl;              /* module table size         */
	uint8_t imp;               /* import table size         */
	uint32_t text;             /* text sector size          */
	uint16_t sym;              /* symbol sector size        */
	uint32_t str;              /* string sector size        */
	uint32_t size;             /* size of (module - header) */
	uint32_t debug;            /* debug section size        */
	char pad[16];              /* for future expanding      */
} RMDHeader;

/* RMD signature */
#define RMD_H_IDENT1		'R'
#define RMD_H_IDENT2		'M'
#define RMD_H_IDENT3		'D'
#define RMD_H_IDENT4		0x1F

/* #ptbl entry */
typedef struct RMDProcedure {
	uint32_t addr;
	uint8_t argc;
	uint8_t varc;
} RMDProcedure;

/* #exp entry */
typedef struct RMDExport {
	uint16_t name;
	uint16_t proto;
	uint8_t idx;
} RMDExport;

/* #mtbl entry */
typedef struct RMDModule {
	uint16_t name;
	RMDVersion version;
} RMDModule;

/* #imp entry */
typedef struct RMDImport {
	uint16_t name;
	uint8_t module;
	uint8_t slot;
} RMDImport;

/* #str entry */
typedef struct RMDString {
	uint32_t len;
	char str[1];
} RMDString;

#endif

