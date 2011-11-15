#include "proc.h"
#include "common.h"
#include "tables.h"
#include "text.h"
#include <stdlib.h>

/* no operand */
static uint8_t proc_n(const char *args)
{
	if(*args)
		error("this instruction has no operands");
	return 0;
}

/* index to .const */
static uint8_t proc_c(const char *args)
{
	int idx = const_find(args);
	if(idx == -1)
		error("unknown constant '%s'", args);
	return idx;
}

/* index to .stack */
static uint8_t proc_s(const char *args)
{
	int idx = var_find(args);
	if(idx == -1)
		error("unknown variable '%s'", args);
	return idx;
}

/* index to .data */
static uint8_t proc_d(const char *args)
{
	int idx = data_find(args);
	if(idx == -1)
		error("unknown data '%s'", args);
	return idx;
}

static long read_val(const char *str)
{
	char *end;
	long val = strtol(str, &end, 0);
	if(end == str)
		error("number expected");
	return val;
}

/* signed 8-bit number */
static uint8_t proc_i(const char *args)
{
	long val = read_val(args);
	if(val > 127 || val < -128)
		error("number out of range");
	return val;
}

/* unsigned 8-bit number */
static uint8_t proc_u(const char *args)
{
	long val = read_val(args);
	if(val > 255 || val < 0)
		error("number out of range");
	return val;
}

/* text offset */
static uint8_t proc_o(const char *args)
{
	uint32_t addr;
	int ofs;
	if(label_find(args, &addr)) {
		/* back reference */
		ofs = offset - addr;
		if(ofs > 127 || ofs < -127)
			error("offset out of range");
		return ofs;
	}
	forward_ref_near(args);
	/* return stub */
	return 0;
}

/* index to .addr */
static uint8_t proc_a(const char *args)
{
	uint32_t addr;
	if(label_find(args, &addr))
		/* back reference */
		return addr_add(args, addr);
	forward_ref_far(args);
	/* return stub */
	return 0;
}

/* index to .ptbl */
static uint8_t proc_p(const char *args)
{
	int idx = ptbl_find(args);
	if(idx == -1)
		error("unknown procedure '%s'", args);
	return idx;
}

/* index to .imp */
static uint8_t proc_f(const char *args)
{
	int idx = imp_find(args);
	if(idx == -1)
		error("unknown procedure '%s'", args);
	return idx;
}

/* index to .mtbl */
static uint8_t proc_m(const char *args)
{
	int idx = module_find(args);
	if(idx == -1)
		error("unknown module '%s'", args);
	return idx;
}

static uint8_t (*const proc_parse[])(const char *) = {
	proc_n,
	proc_c,
	proc_s,
	proc_d,
	proc_o,
	proc_i,
	proc_u,
	proc_a,
	proc_p,
	proc_f,
	proc_m,
};

uint8_t proc_read(int type, const char *arg)
{
	return proc_parse[type](arg);
}

void proc_finish(void)
{
	ref_resolve();
	if(verbose >= DL_PERLINE) {
		label_print();
		proc_print();
	}
	label_clear();
	var_clear();
}

