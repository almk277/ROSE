#! /bin/sh

STACK_SIZE=512
BSS_SIZE=0

FILE=$1
OBJ=${FILE%.c}
TEXT=$OBJ.text

if [ $# -eq 0 ]; then
	echo No file name
	exit 1
fi

if ! [ -x text2int ]; then
	make text2int
fi

gcc -c $FILE -o $OBJ
objcopy -O binary $OBJ

exit 0

TEXTSIZE=$(wc -c $TEXT | awk '{ print $1 }')

# signature
echo -n "ROSE" > $OBJ

# entry
echo -n 0 | ./text2int >> $OBJ

# stack
echo -n $STACK_SIZE | ./text2int >> $OBJ

# text header
echo -n 29 | ./text2int >> $OBJ
echo -n $TEXTSIZE | ./text2int >> $OBJ

# data header
for i in `seq 2`; do
	echo -n 0 | ./text2int >> $OBJ
done

# text
cat $TEXT >> $OBJ

rm -f $TEXT

