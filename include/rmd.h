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
	uint32_t cnst;             /* constant table offset   */
	uint32_t text;             /* text segment offset     */
	uint32_t sym;              /* symbol segment offset   */
	uint32_t str;              /* string segment offset   */
	char pad[16];              /* for future expanding    */
} RMDHeader;

/* RMD signature */
#define RMD_H_IDENT1		'R'
#define RMD_H_IDENT2		'M'
#define RMD_H_IDENT3		'D'
#define RMD_H_IDENT4		0x1F

/* Segment sizes in bytes */
#define RMD_SEG_EXP_SIZE(head)     ((head).ptbl - (head).exp)
#define RMD_SEG_PTBL_SIZE(head)    ((head).mtbl - (head).ptbl)
#define RMD_SEG_MTBL_SIZE(head)    ((head).imp  - (head).mtbl)
#define RMD_SEG_IMP_SIZE(head)     ((head).addr - (head).imp)
#define RMD_SEG_ADDR_SIZE(head)    ((head).cnst - (head).addr)
#define RMD_SEG_CONSTS_SIZE(head)  ((head).text - (head).cnst)
#define RMD_SEG_TEXT_SIZE(head)    ((head).sym  - (head).text)
#define RMD_SEG_SYM_SIZE(head)     ((head).str  - (head).sym)

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

