# This is ISA formal description. It is used for automatic code generation.

# $NT is N-th operand, and T is type code:
# I - int, F - float, A - array, O - object, S - string (in array)
# $ip - program counter

# Instructions:
# name    operands  opcode  description
arr.get   sss       0x80    $1 = $2A[$3I]
arr.len   ss        0x85    $1I = length($2A)
arr.load  sa        0x86    $1A = new array; arr.load($2)
arr.new   ss        0x83    $1A = new array[$2I]
arr.put   sss       0x81    $2A[$3I] = $1
call      p         0x40    call $1
dec       sb        0x66    $1I = $1I - $2
dispatch  sb        0xAA    call $1.$2
exit      -         0x05    stops the program
flt.add   sss       0x60    $1F = $2F + $3F
flt.div   sss       0xAA    $1F = $2F / $3F
flt.mul   sss       0xAA    $1F = $2F * $3F
flt.new   sw        0xAA    $1 = $2F
flt.sub   sss       0x61    $1F = $2F - $3F
load      ssb       0x11    $1 = $2O.$3
inc       sb        0x67    $1I = $1I + $2
int.add   sss       0x60    $1I = $2I + $3I
int.new   sw        0x11    $1 = $2I
int.snew  sb        0x10    $1 = $2I
int.sub   sss       0x61    $1F = $2F - $3F
invoke    sw        0xAA    call $1::$2S
jump      r         0x20    jumps to ($IP + $2)
jumpeq    ssr       0x27    jumps to ($ip + $3) if $1I == $2I
jumpl     ssr       0x23    jumps to ($ip + $3) if $1I < $2I
jumple    ssr       0x25    jumps to ($ip + $3) if $1I <= $2I
jumplf    ssr       0xAA    jumps to ($ip + $3) if $1F < $2F
jumpz     sr        0x22    jumps to ($ip + $2) if $1I == 0
move      ss        0xAA    $1 = $2
new       sm        0x50    $1O = new $2
nop       -         0x01    nop does nothing
outb      s         0x01    print $1 & 0xFF
outstr    s         0x01    print $1A
return    -         0x45    returns from current method
store     ssb       0x12    $2O.$3 = $1
