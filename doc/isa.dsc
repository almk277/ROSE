# This is ISA formal description. It is used for automatic code generation.

# Operand types:
# n	no operand
# c	index to .const (constant)
# s	index to .stack (local variable)
# d	index to .data (module variable)
# i	signed 8-bit number
# u	unsigned 8-bit number
# o	text offset
# a	index to .addr (code address)
# p	index to .ptbl (procedure)
# f	index to .imp (import procedure)
# m	index to .mtbl (module)

# Various signs
# $top    stack top word
# $base   stack base word
# $ip     current instruction address
# $op     current instruction operand

# Instructions:
# name    operand  opcode  description
add       n        0x60    pops two stack words and pushes their sum
arrdel    n        0x84    deletes array $top points to
arrget    s        0x80    loads $top-th element from array $op
arrnew    n        0x83    creates new array of size $top; pops; pushes ref
arrput    s        0x81    stores $top to $top[-1]-th element of array $op
call      p        0x40    makes near procedure call
dec       u        0x66    substracts $op from $top
delete    n        0x51    deletes object referenced by $top and pops $top
dup       n        0x17    pushes $top on stack again
exit      n        0x05    stops the program
farcall   f        0x42    makes far procedure call
farjump   a        0x21    jumps to given address
getc      c        0x10    pushes constant on stack
getd      d        0x11    pushes module variable on stack
gets      s        0x13    pushes local variable on stack
inc       u        0x67    adds $op to $top
jump      o        0x20    jumps to ($ip + $op)
jumpeq    o        0x27    pops two stack words and jumps to ($ip + $op) if \
                           they are equal
jumpl     o        0x23    pops $top and jumps to ($ip + $op) if $top is \
                           less than zero
jumple    o        0x25    pops $top and jumps to ($ip + $op) if $top is \
                           less or equal to zero
jumpz     o        0x22    pops $top and jumps to ($ip + $op) if $top is zero
loadstr   n        0x57    creates array, initialises it from .str addr $top
new       m        0x50    creates new module object and pushes reference to \
                           it on stack
nop       n        0x01    nop does nothing
out       u        0x70    pops stack word and outputs it in given file
pushc     i        0x15    pushes constant on stack
pushcsh   i        0x16    makes $top = ($top << 8) | $op
putd      d        0x12    pops stack word into module variable
puts      s        0x14    pops stack word into local variable
return    n        0x45    returns execution from current procedure with \
                           return value
retp      n        0x46    returns execution from current procedure without \
                           return value
strlen    n        0x58    gets array length: $top = length($top)
sub       n        0x61    pops two stack words and pushes their difference
write     u        0x72    writes to given file array $top bytes from array \
                           $top[-1]
