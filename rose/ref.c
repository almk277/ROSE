#include "ref.h"
#include "addrcat.h"
#include "compiler.h"
#include "error.h"
#include <stddef.h>

/* Ref <=> SplittedRef <=> PointerCell <=> Pointer */

typedef Pointer *PointerCell;

typedef struct SplittedRef {
	uint16_t cat;
	uint16_t tbl;
} SplittedRef;

static AddrCat addr_cat = {
	{ &addrtbl_first }, /* 1st address table is created statically */
	1                   /* $0.0 is forbidden */
};

static Ref ref_free_list = { 0 };

#define REF_CAT_IDX(ref)       (ref._ref >> REF_TBL_BITS)
#define REF_TBL_IDX(ref)       (ref._ref & (ADDRTBL_SIZE - 1))
#define REF_SPLIT(j_ref)       { REF_CAT_IDX(j_ref), REF_TBL_IDX(j_ref) }
#define REF_JOIN(sp_ref)       ((sp_ref.cat << REF_TBL_BITS) | sp_ref.tbl)

static PointerCell ref_cell(Ref ref)
{
	PointerCell cell;
	SplittedRef sp_ref = REF_SPLIT(ref);
	AddrTbl *tbl = addrcat_get(&addr_cat, sp_ref.cat);
	if(unlikely(tbl == NULL))
		error(ERR_REF_FAULT);
	cell = addrtbl_addr(tbl, sp_ref.tbl);
	if(unlikely(cell == NULL))
		error(ERR_REF_FAULT);
	return cell;
}

void *ref_get(Ref ref)
{
	return *ref_cell(ref);
}

Ref ref_new(void *ptr)
{
	Ref ref;
	SplittedRef sp_ref;
	AddrTbl *tbl;
	if(!ref_is_invalid(ref_free_list)) {
		/* There is freed reference, reuse it */
		PointerCell ret_cell = ref_cell(ref_free_list);
		uint32_t next = (uint32_t)*(intptr_t *)*ret_cell;
		sp_ref.cat = REF_CAT_IDX(ref_free_list);
		sp_ref.tbl = REF_TBL_IDX(ref_free_list);
		ref_free_list._ref = next;
	} else {
		/* Table has an empty cell */
		sp_ref.cat = addrcat_top(&addr_cat);
		tbl = addrcat_get(&addr_cat, sp_ref.cat);
		if(likely(!addrtbl_is_full(tbl)))
			sp_ref.tbl = addrtbl_add_ptr(tbl, ptr);
		else {
			/* We need new table */
			if(unlikely(addrcat_is_full(&addr_cat)))
				error(ERR_REF_OVER);
			tbl = addrtbl_new();
			sp_ref.cat = addrcat_top(&addr_cat);
			addrcat_add_tbl(&addr_cat, tbl);
			sp_ref.tbl = addrtbl_add_ptr(tbl, ptr);
		}
	}
	ref._ref = REF_JOIN(sp_ref);
	return ref;
}

void ref_delete(Ref ref)
{
	PointerCell cell = ref_cell(ref);
	Ref head = ref_free_list;
	ref_free_list = ref;
	*(intptr_t *)*cell = head._ref;
}

