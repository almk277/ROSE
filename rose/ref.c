#include "ref.h"
#include "addrcat.h"
#include "compiler.h"
#include "error.h"
#include <stddef.h>

static AddrCat addr_cat = {
	{ &addrtbl_first }, /* 1st address table is created statically */
	0
};

#define REF_CAT_IDX(ref)       (ref >> REF_TBL_BITS)
#define REF_TBL_IDX(ref)       (ref & (ADDRTBL_SIZE - 1))
#define REF_FROM(cat, tbl)     ((cat << REF_TBL_BITS) | tbl)


Ref ref_new(void *ptr)
{
	Ref ref;
	uint32_t tbl_idx;
   	uint32_t cat_idx = addrcat_top(&addr_cat);
	AddrTbl *tbl = addrcat_get(&addr_cat, cat_idx);
	if(likely(!addrtbl_is_full(tbl)))
		tbl_idx = addrtbl_add_ptr(tbl, ptr);
	else {
		if(unlikely(addrcat_is_full(&addr_cat)))
			error();
	   	tbl = addrtbl_new();
		cat_idx = addrcat_top(&addr_cat);
		addrcat_add_tbl(&addr_cat, tbl);
		tbl_idx = addrtbl_add_ptr(tbl, ptr);
	}
	ref._ref = REF_FROM(cat_idx, tbl_idx);
	return ref;
}

void ref_delete(Ref ref)
{
}

void *ref_get(const Ref ref)
{
	void *ptr;
	uint32_t cat_idx = REF_CAT_IDX(ref._ref);
	uint32_t tbl_idx = REF_TBL_IDX(ref._ref);
	AddrTbl *tbl = addrcat_get(&addr_cat, cat_idx);
	if(unlikely(tbl == NULL))
		error();
	ptr = addrtbl_get(tbl, tbl_idx);
	if(unlikely(ptr == NULL))
		error();
	return ptr;
}

