#ifndef ROSE_OBJ_H
#define ROSE_OBJ_H

#include <stdint.h>

typedef struct RMDHeader {
	unsigned char ident[4];    /* signature                 */
	unsigned char rmd_ver[2];  /* ROSE version              */
	uint16_t name;             /* module name               */
	uint16_t parent;           /* parent module name        */
	unsigned char version[2];  /* module version            */
	uint8_t flags;             /* various flags             */
	uint8_t data_cnt;          /* data count                */
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

/* #exp entry */
typedef struct RMDExport {
	uint16_t name;
	uint16_t proto;
	uint8_t idx;
} RMDExport;

/* #ptbl entry */
typedef struct RMDProcedure {
	uint32_t addr;
	uint8_t argc;
	uint8_t varc;
} RMDProcedure;

/* #mtbl entry */
typedef struct RMDModule {
	uint16_t name;
	unsigned char version[2];
	uint32_t sum;
} RMDModule;

/* #imp entry */
typedef struct RMDImport {
	uint16_t name;
	uint8_t module;
	uint8_t slot;
	uint32_t addr;
} RMDImport;

/* #str entry */
typedef struct RMDString {
	uint32_t len;
	char str[1];
} RMDString;

#endif

