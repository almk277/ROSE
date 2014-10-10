# This is ISA formal description. It is used for automatic code generation.

# name     operands  opcode  description
arr.get    oai       0x80    $1 = $2[$3]
arr.len    ia        0x85    $1 = length($2)
arr.new    ai        0x84    $1 = new array[$2]
arr.put    oai       0x82    $2[$3] = $1
barr.getb  vai       0x90    $1 = $2[$3]
barr.getw  vai       0x91    $1 = $2[$3]
barr.len   ia        0x95    $1 = length($2)
barr.load  aA        0x96    $1 = new array; arr.load($2)
barr.new   ai        0x94    $1 = new array[$2]
barr.putb  vai       0x92    $2[$3] = $1
barr.putw  vai       0x93    $2[$3] = $1
call       P         0x40    call $1
dbg.byte   i         0x01    prints $1 & 0xFF
dbg.str    a         0x02    prints $1
dispatch   oS        0x45    call $1.$2
exit       -         0x05    stops the program
flt.add    fff       0xA2    $1 = $2 + $3
flt.div    fff       0xA5    $1 = $2 / $3
flt.mul    fff       0xA4    $1 = $2 * $3
flt.new    fw        0xA0    $1 = $2
flt.sub    fff       0xA3    $1 = $2 - $3
int.add    iii       0x12    $1I = $2I + $3I
int.dec    ib        0x17    $1I = $1I - $2
int.inc    ib        0x16    $1I = $1I + $2
int.new    iw        0x10    $1 = $2I
int.snew   ic        0x11    $1 = $2I
int.sub    iii       0x13    $1 = $2 - $3
invoke     I         0x43    call $1::$2
jump       r         0x20    jumps for $1 bytes
jumpeq     iir       0x21    jumps for $3 bytes if $1 == $2
jumpl      iir       0x22    jumps for $3 bytes if $1 < $2
jumple     iir       0x23    jumps for $3 bytes if $1 <= $2
jumplf     ffr       0x25    jumps for $3 bytes if $1 < $2
jumpz      ir        0x24    jumps for $3 bytes if $1 == 0
load       sD        0x34    $1 = this.$2
move       oo        0x30    $1 = $2
new        oM        0x50    $1O = new $2
nop        -         0x00    does nothing
return     -         0x47    returns from current method
store      sD        0x35    this.$2 = $1
