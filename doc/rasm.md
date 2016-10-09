ROSE Assembler
==============

## Lexems

**Constants**:
* bytes: `0`, `42`, `'a'`, `'\n'`, `0x0F`;
* words: `0L`, `32768L`, `0xBEEFL`;
* floats: `3.14`, `1e-100`;
* byte arrays: `"Hello\n"`.

**Names**: `x`, `_2`, `TheVariable`.

**Directives**: `.sub`, `.var`.

## Syntax

**Comments** starts with `;` and ends at end of line.

**Instructions** are written in one line:
```
	<instruction> <op1> ... <opN>
```

**Labels** can be declared in code as `<label>:`.

**Directives** are either one-line or multiline. Multiline (or section) directive starts a section, and it proceeds until next multiline directive.

`.arg <type> <name>`
Declares procedure argument. Allowed in .text section only.

`.data <type> <name>`
Declares #data variable.

`.export <name>`
Declares procedure `name` as public.

`.extern <name>@<module> <type1> ... <typeN>`
Imports procedure with `name` from `module` (previously imported with `.import`).

`.import <name> <maj> <min>`
Imports module with given name and version.

`.module <name> <maj> <min>`
Declares current module name and version.

`.str <name>`
Declares #str section, begins a string with `name`. Multiline, starts .str section.

`.sub <name>`
Declares #text section with procedure `name`. Multiline, starts .text section.

`.var <name>`
Declares local variable. Allowed in .text section only.

## Example
```
.module just_an_example 0.1

.str myname               ; .str section
	"ROSE"

.sub myname               ; .text section
.export myname            ; public procedure

.arg bytearray str        ; procedure argument

	barr.load str myname  ; instructions
	return
```
