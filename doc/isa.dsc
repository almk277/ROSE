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
exit       -         05    stops the program
file.close F         B1    close($1)
file.open  Fac       B0    $1 = open(name=$2, mode=$3)
file.read  Fai       B2    $2 <- read(file=$1, size=$3)
file.write Fai       B3    write(file=$1, data=$2, size=$3)
flt.add    fff       A2    $1 = $2 + $3
flt.div    fff       A5    $1 = $2 / $3
flt.mul    fff       A4    $1 = $2 * $3
flt.new    fw        A0    $1 = $2
flt.sub    fff       A3    $1 = $2 - $3
int.add    iii       12    $1 = $2 + $3
int.inc    ic        16    $1 = $1 + $2
int.new    iw        10    $1 = $2
int.byte   ic        11    $1 = $2
int.sub    iii       13    $1 = $2 - $3
invoke     I         43    call $1
jmp        r         20    jumps for $1 bytes
jmp.flt.l  ffr       25    jumps for $3 bytes if $1 < $2
jmp.int.eq iir       22    jumps for $3 bytes if $1 == $2
jmp.int.l  iir       23    jumps for $3 bytes if $1 < $2
jmp.int.le iir       24    jumps for $3 bytes if $1 <= $2
jmp.int.z  ir        21    jumps for $2 bytes if $1 == 0
jmp.ref.eq oor       27    jumps for $3 bytes if $1 == $2
jmp.ref.z  or        26    jumps for $2 bytes if $1 == 0
move       oo        30    $1 = $2
nop        -         00    does nothing
obj.load   sD        34    $1 = this.$2
obj.new    oM        50    $1O = new $2
obj.store  sD        35    this.$2 = $1
return     -         47    returns from current method
