#ifndef ROSE_OBJ_H
#define ROSE_OBJ_H

#include <stdint.h>

typedef struct RMDHeader {
	unsigned char ident[4];    /* signature               */
	unsigned char rmd_ver[2];  /* ROSE version            */
	uint16_t name;             /* module name             */
	unsigned char version[2];  /* module version          */
	uint8_t flags;             /* various flags           */
	uint8_t data_cnt;          /* data count              */
	uint32_t exp;              /* export table offset     */
	uint32_t ptbl;             /* procedure table offset  */
	uint32_t mtbl;             /* module table offset     */
	uint32_t imp;              /* import table offset     */
	uint32_t addr;             /* address table offset    */
	uint32_t consts;           /* constant table offset   */
	uint32_t text;             /* text segment offset     */
	uint32_t sym;              /* symbol segment offset   */
	uint32_t str;              /* string segment offset   */
} RMDHeader;

/* RMD signature */
#define RMD_H_IDENT1		'R'
#define RMD_H_IDENT2		'M'
#define RMD_H_IDENT3		'D'
#define RMD_H_IDENT4		0x1F

#define RMD_H_RMDVER_MAJ    0
#define RMD_H_RMDVER_MIN    1

#define RMD_H_VER_MAJ       0
#define RMD_H_VER_MIN       1

/* #exp entry */
typedef struct RMDExport {
	uint16_t name;
	uint16_t proto;
	uint32_t descr;
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

#endif

