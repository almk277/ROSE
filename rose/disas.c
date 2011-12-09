#include "conf.h"
#ifdef ENABLE_DEBUGGER

#include "disas.h"
#include "module.h"
#include <inttypes.h>
#include <glib.h>

#define SYM_SIZE_CNST 5
#define SYM_SIZE_PTBL 5
#define SYM_SIZE_VAR  5

struct DefaultSym {
	char cnst[256][SYM_SIZE_CNST];
	char ptbl[256][SYM_SIZE_PTBL];
	char var[256][SYM_SIZE_VAR];
};

static struct DefaultSym *default_sym = 0;

struct DisasInfo {
	const char *cnst[256];
	const char *ptbl[256];
};

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

/* index to .const */
static const char *oper_c(const Module *module, uint8_t operand)
{
	static char buf[15];
	int32_t c = cnst_get(&module->seg.cnst, operand);
	snprintf(buf, sizeof buf, "%s = %" PRId32, module->info->cnst[operand], c);
	return buf;
}

/* index to .stack */
static const char *oper_s(const Module *module, uint8_t operand)
{
	/*return module->info->var[operand];*/
	return "";
}

/* index to .data */
static const char *oper_d(const Module *module, uint8_t operand)
{
	return "";
}

/* signed integer */
static const char *oper_i(const Module *module, uint8_t operand)
{
	return "";
}

/* unsigned integer */
static const char *oper_u(const Module *module, uint8_t operand)
{
	return "";
}

/* text offset */
static const char *oper_o(const Module *module, uint8_t operand)
{
	return "";
}

/* index to .addr */
static const char *oper_a(const Module *module, uint8_t operand)
{
	return "";
}

/* index to .ptbl */
static const char *oper_p(const Module *module, uint8_t operand)
{
	return "";
}

/* index to .imp */
static const char *oper_f(const Module *module, uint8_t operand)
{
	return "";
}

/* index to .mtbl */
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

static void create_default_sym(void)
{
	int i;

	default_sym = g_new(struct DefaultSym, 1);

	for(i = 0; i != 256; ++i)
		sprintf(&default_sym->ptbl[i][0], "P%d", i);
	for(i = 0; i != 256; ++i)
		sprintf(&default_sym->cnst[i][0], "C%d", i);
	for(i = 0; i != 256; ++i)
		sprintf(&default_sym->var[i][0], "V%d", i);
}

static void disas_set_default_sym(Module *m, const RMDHeader *h)
{
	int i;

	if(!default_sym)
		create_default_sym();

	m->info = g_new(DisasInfo, 1);

	for(i = 0; i != h->ptbl; ++i)
		m->info->ptbl[i] = &default_sym->ptbl[i][0];
	for(i = 0; i != h->cnst; ++i)
		m->info->cnst[i] = &default_sym->cnst[i][0];
}

void disas_load_sym(Module *m, const RMDHeader *h)
{
	/* Here might be loaded debug symbols.
	 * #debug section is not here yet, only default symbols */
	disas_set_default_sym(m, h);
}

void disas_text(const Module *m)
{
}

#endif /* ENABLE_DEBUGGER */
