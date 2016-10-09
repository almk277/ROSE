Modules
=======

Module is a main program unit contained in a separate file. It contains program code and data

Module interface is a list of it's public procedures. Other entities always are private.

At least one program module should be **main** module: it contains a public procedure `main`, program entry point.

Naming
------

A module has a **name** in format of a relative file path, for example: `http/get`.

Also, a module has a **version** in format of two numbers: major and minor, for example: `1.0`.

When some module refers to another module, it uses both module name and version.

Loading
-------

A module is loaded either by user (main module), or on the first call to it's procedure.

A module's filename is formed as module name, then `_`, then major version number, and finally an extension `.rmd`. For example, module `http/get` with version 1.0 is contained in a file `http/get_1.rmd` (where "http" is a subdirectory).

A module is searched for in following directories, in this order:
* a directory containing a main module;
* directories read from environment variable `ROSEPATH` as a colon-separated list;
* system directories as specified in config file.

When a file with appropriate name is found, VM tries to load it. At this moment, module minor version number is checked too, and it must be not less than required, otherwise module is rejected.

On successful loading, module is shallow-verified (not bytecode, only some tables).

Binding
-------

ROSE uses late binding: every procedure binds on first call to it.

Procedure is identified by it's name and type. On binding, VM:
* [loads](#Loading) procedure's module, if not loaded yet;
* searches for public procedure with a given name and type;
* verifies procedure;
* caches procedure address.

On subsequent calls to the procedure, all this steps are no longer needed.

Structure
---------

A module includes following sections:

* [38] #head - general module information and other sections sizes:
	* [4] signature "RMD\x1F";
	* [2] rmdver  - RMD format version;
	* [2] name    - #sym string with module name;
	* [2] modver  - module version;
	* [14] section sizes in section units:
		* [1] #exp;
		* [1] #ptbl;
		* [1] #mtbl;
		* [1] #imp;
		* [4] #text;
		* [2] #sym;
		* [4] #str.
	* [1] datac   - number of module variables;
	* [1] flags   - various flags;
	* [4] size    - module size (without header);
	* [8] padding.
* [5] #exp  - exported procedures:
	* [2] name - #sym string with procedure export name;
	* [2] type - #sym string with type code.
	* [1] idx  - index into #ptbl.
* [10] #ptbl - every procedure location and description:
	* [4] addr - offset in #text;
	* [4] size - procedure size;
	* [1] argc - number of procedure arguments;
	* [1] varc - number of procedure variables.
* [4] #mtbl - imported modules:
	* [2] name    - #sym string with module full name;
	* [2] version - required module version.
* [5] #imp  - imported procedures:
	* [2] name   - #sym string with procedure name;
	* [2] type   - #sym string with procedure type;
	* [1] module - #mtbl index for module with this procedure;
* #text - all module instructions;
* #sym  - all loadable symbols;
* #str  - constant array pool.

Numbers are stored as little endian.
