Instructions
============

Every instruction starts with one-byte opcode,
possibly followed by several operands. There are formal [operand](operand.dsc) and [instruction](isa.dsc) specifications.

## Data types

ROSE supports some primitive types. Every type has constructor(s) and other instructions to work with. There are [value types](memory.md#vstack) and [reference types](memory.md#heap).

* **Integers** are 32-bits signed numbers with usual arithmetic. Value type.
Instructions: `int.*`.

* **Floats** are IEEE-754 single precision floating-point numbers with usual arithmetic. Value type.
Instructions: `flt.*`.

* **Arrays** are containers of (references to) objects with random access. Reference type.
Instructions: `arr.*`.

* **Byte arrays** are containers of bytes/words with random access. Reference type.
Instructions: `barr.*`.

* **Files** are bytestreams used for input-output. Reference type.
Instructions: `file.*`.

* **Objects** are module instances. Reference type.
Instructions: `obj.*`.

## Execution flow

* Conditional and unconditional jumps: `jmp`, `jmp.*`;
* procedure calls:
  * internal call `call`;
  * external call `invoke`;
* procedure return: `return`;
* program termination: `exit`.
