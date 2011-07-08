#ifndef MODULE_H
#define MODULE_H

#include "rmd.h"
#include "seg_addr.h"
#include "seg_const.h"
#include "seg_exp.h"
#include "seg_imp.h"
#include "seg_mtbl.h"
#include "seg_ptbl.h"
#include "seg_sym.h"
#include "seg_text.h"
#include <string>

class Module {
	public:
		Module();
		class LoadException {
			public:
				enum Error {
					OPEN,
					READ,
					MEMORY,
				};
				LoadException(enum Error err): _err(err) {}
			private:
				enum Error _err;
		};
		void load(const std::string& filename) throw(LoadException);
	private:
		std::string _filename;
		RMDHeader _header;
		uint32_t _size;
		char *_body;
		SegExp _exp;
		SegPtbl _ptbl;
		SegMtbl _mtbl;
		SegImp _imp;
		SegAddr _addr;
		SegConst _const;
		SegText _text;
		SegSym _sym;
};

#endif

