#include "disas.h"
#include "module.h"
#include <inttypes.h>

#define STRBUF_SIZE  64

enum OperType {
	OPER_none,
	OPER_n,
	OPER_c,
	OPER_s,
	OPER_d,
	OPER_i,
	OPER_u,
	OPER_o,
	OPER_a,
	OPER_p,
	OPER_f,
	OPER_m,
};

#define DIS_NO_INSTR "<no instruction>"

static const struct Disas {
	const char *name;
	enum OperType type;
} disas_tbl[] = {
#include "disas_tbl.c"
};

static const char *oper_none(const Module *module, uint8_t operand)
{
	return "";
}

static const char *oper_n(const Module *module, uint8_t operand)
{
	return "";
}

static const char *oper_c(const Module *module, uint8_t operand)
{
	static char buf[12];
	snprintf(buf, sizeof buf, ".const[%d] = %" PRId32, operand,
			cnst_get(&module->seg.cnst, operand));
	return buf;
}

static const char *oper_s(const Module *module, uint8_t operand)
{
	return "";
}

static const char *oper_d(const Module *module, uint8_t operand)
{
	return "";
}

static const char *oper_i(const Module *module, uint8_t operand)
{
	return "";
}

static const char *oper_u(const Module *module, uint8_t operand)
{
	return "";
}

static const char *oper_o(const Module *module, uint8_t operand)
{
	return "";
}

static const char *oper_a(const Module *module, uint8_t operand)
{
	return "";
}

static const char *oper_p(const Module *module, uint8_t operand)
{
	return "";
}

static const char *oper_f(const Module *module, uint8_t operand)
{
	return "";
}

static const char *oper_m(const Module *module, uint8_t operand)
{
	return "";
}

static const char *(*oper_present[])(const Module *,uint8_t) = {
	oper_none,
	oper_n,
	oper_c,
	oper_s,
	oper_d,
	oper_i,
	oper_u,
	oper_o,
	oper_a,
	oper_p,
	oper_f,
	oper_m,
};

const char *disas_str(const Module *module, uint32_t addr)
{
	static char buf[STRBUF_SIZE];

	uint8_t *p = text_addr(&module->seg.text, addr);
	uint8_t opcode = *p++;
	uint8_t operand = *p;
	const struct Disas *desc = &disas_tbl[opcode];
	snprintf(buf, sizeof buf, "%s %s", desc->name,
			oper_present[desc->type](module, operand));
	return buf;
}

void disas_module(const Module *module, FILE *file)
{
}

