/* Declarations for RMD - ROSE binary format */

#ifndef ROSE_RMD_H
#define ROSE_RMD_H

#include <stdint.h>

/* Versions */
typedef struct RMDVersion {
	unsigned char maj;
	unsigned char min;
} RMDVersion;

/* Base types */
typedef uint8_t R_Byte;
typedef int32_t R_Word;

/* Address types */
typedef uint16_t RA_Symbol;
typedef uint32_t RA_Array;
typedef uint32_t RA_Text;
typedef int16_t  RA_TextOffset;
typedef uint8_t  RA_Stack;
typedef uint8_t  RA_Data;
typedef uint8_t  RA_Module;
typedef uint8_t  RA_Proc;
typedef uint8_t  RA_Export;
typedef uint8_t  RA_Import;

/* Information about sector sizes */
typedef struct RMDSectorSize {
	RA_Export exp;   /* export table size     */
	RA_Proc ptbl;    /* procedure table size  */
	RA_Module mtbl;  /* module table size     */
	RA_Import imp;   /* import table size     */
	RA_Text text;    /* text sector size      */
	RA_Symbol sym;   /* symbol sector size    */
	RA_Array str;    /* string sector size    */
} RMDSectorSize;

/* RMD header */
typedef struct RMDHeader {
	unsigned char ident[4];    /* signature                 */
	RMDVersion rmd_version;    /* ROSE version              */
	RA_Symbol name;            /* module name               */
	RMDVersion version;        /* module version            */
	RMDSectorSize sizes;       /* sizes of all sectors      */
	RA_Data datac;             /* data count                */
	unsigned char flags;       /* module flags              */
	uint32_t size;             /* size of (module - header) */
	unsigned char pad[8];      /* for future expanding      */
} RMDHeader;

/* RMD signature */
#define RMD_H_IDENT1		'R'
#define RMD_H_IDENT2		'M'
#define RMD_H_IDENT3		'D'
#define RMD_H_IDENT4		0x1F

/* #exp entry */
typedef struct RMDExport {
	RA_Symbol name;
	RA_Proc idx;
} RMDExport;

/* #ptbl entry */
typedef struct RMDProcedure {
	RA_Text addr;
	RA_Text size;
	RA_Symbol type;
	RA_Stack argc;
	RA_Stack varc;
} RMDProcedure;

/* #mtbl entry */
typedef struct RMDModule {
	RA_Symbol name;
	RMDVersion version;
} RMDModule;

/* #imp entry */
typedef struct RMDImport {
	RA_Symbol name;
	RA_Symbol type;
	RA_Export slot; // FIXME
	RA_Module module;
} RMDImport;

/* #str entry */
typedef struct RMDString {
	RA_Array len;
	char str[1];
} RMDString;

#endif

