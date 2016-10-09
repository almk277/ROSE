Memory
======

Main memory unit is **word**, and word is 32 bits. Concrete byte is not addressable (though, a byte in a bytearray can be accessed).

Segments
--------

There is segmented memory model, and it lets us use short addresses in many situations.

| Segment            | Address width |
|--------------------|---------------|
| [.heap](#heap)     | 32            |
| [.vstack](#vstack) | 8             |
| [.pstack](#pstack) | -             |
| [.ptbl](#ptbl)     | 8             |
| [.mtbl](#mtbl)     | 8             |
| [.imp](#imp)       | 8             |
| [.exp](#exp)       | 8             |
| [.text](#text)     | 32            |
| [.sym](#sym)       | 16            |
| [.str](#str)       | 32            |

### Global segments

#### heap

Area for dynamic memory allocation. **Reference types** are allocated here and garbage collected later.

### Thread-local segments

#### vstack

Contains local **variables**, including procedure arguments. Every variable is a word: either value itself, for **value types**, or **reference** to value, for reference types. Stack frame size varies: it depends on number of arguments and variables.

`| arg0 | ... | argM | var0 | ... | varN |`

A procedure has an access only to current stack frame, not entire .vstack. So, more accurately, the segment is current stack frame.

#### pstack

Internal procedure stack, contains procedure activation records -- information about previous procedure. Stack frame size is fixed. This stack is not accessible by program directly.

`| .text address | .vstack frame address | procedure descriptor | module |`

### Module-local segments

This segments are loaded from module.

#### ptbl

Procedure table contains metadata for all module procedures.

`| start address | size | argument count | variable count |`

#### mtbl

Module table contains information about all modules referred from this one.

`| name | version |`

#### imp

Import table contains information about all external procedures invoked from this module.

`| name | signature | module |`

#### exp

Export table contains information about all public procedures in this module.

`| name | .ptbl slot |`

#### text

Program instructions.

`int.byte 0 41 inc 0 1 return`

#### sym

Runtime symbol table is a collection of zero-terminated strings.

`"main\0println\0aux\0"`

#### str

Data constant pool as a collection of data arrays.

`0 0 0 5 'H' 'e' 'l' 'l' 'o'`

### Module-instance-local segments

#### data

Module instance variables, aka object fields.
