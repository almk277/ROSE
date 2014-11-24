# This is ISA formal description. It is used for automatic code generation.

# name     operands  code  description
arr.get    oai       80    $1 = $2[$3]
arr.len    ia        85    $1 = length($2)
arr.new    ai        84    $1 = new array[$2]
arr.put    oai       82    $2[$3] = $1
barr.getb  vai       90    $1 = $2[$3]
barr.getw  vai       91    $1 = $2[$3]
barr.len   ia        95    $1 = length($2)
barr.load  aA        96    $1 = new array; arr.load($2)
barr.new   ai        94    $1 = new array[$2]
barr.putb  vai       92    $2[$3] = $1
barr.putw  vai       93    $2[$3] = $1
call       P         40    call $1
dbg.byte   c         01    prints $1 & 0xFF
dbg.flt    f         04    prints $1
dbg.int    i         03    prints $1
dbg.str    a         02    prints $1
dispatch   oS        45    call $1.$2
exit       -         05    stops the program
flt.add    fff       A2    $1 = $2 + $3
flt.div    fff       A5    $1 = $2 / $3
flt.mul    fff       A4    $1 = $2 * $3
flt.new    fw        A0    $1 = $2
flt.sub    fff       A3    $1 = $2 - $3
int.add    iii       12    $1I = $2I + $3I
int.inc    ic        16    $1I = $1I + $2
int.new    iw        10    $1 = $2I
int.byte   ic        11    $1 = $2I
int.sub    iii       13    $1 = $2 - $3
invoke     I         43    call $1::$2
jmp        r         20    jumps for $1 bytes
jmp.flt.l  ffr       25    jumps for $3 bytes if $1 < $2
jmp.int.eq iir       22    jumps for $3 bytes if $1 == $2
jmp.int.l  iir       23    jumps for $3 bytes if $1 < $2
jmp.int.le iir       24    jumps for $3 bytes if $1 <= $2
jmp.int.z  ir        21    jumps for $3 bytes if $1 == 0
jmp.ref.eq oor       26    jumps for $3 bytes if $1 == $2
load       sD        34    $1 = this.$2
move       oo        30    $1 = $2
new        oM        50    $1O = new $2
nop        -         00    does nothing
return     -         47    returns from current method
store      sD        35    this.$2 = $1
