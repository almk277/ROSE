#ifndef ROSE_OBJ_H
#define ROSE_OBJ_H

#include <stdint.h>

typedef struct RMDHeader {
	unsigned char ident[4];    /* signature                 */
	unsigned char rmd_ver[2];  /* ROSE version              */
	uint16_t name;             /* module name               */
	unsigned char version[2];  /* module version            */
	uint8_t flags;             /* various flags             */
	uint8_t data_cnt;          /* data count                */
	uint8_t exp;               /* export table offset       */
	uint8_t ptbl;              /* procedure table offset    */
	uint8_t mtbl;              /* module table offset       */
	uint8_t imp;               /* import table offset       */
	uint8_t cnst;              /* constant table offset     */
	uint8_t addr;              /* address table offset      */
	uint32_t text;             /* text segment offset       */
	uint16_t sym;              /* symbol segment offset     */
	uint32_t str;              /* string segment offset     */
	uint32_t size;             /* size of (module - header) */
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
	const char str[1];
} RMDString;

#endif

