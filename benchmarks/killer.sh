#!/bin/sh

FILE="$1"
# make sure file exist and readable
if [ ! -f $FILE ]; then
   echo "$FILE : does not exists"
   exit 1
elif [ ! -r $FILE ]; then
   echo "$FILE: can not read"
   exit 2
fi

# read $FILE using the file descriptors

# Set loop separator to end of line
exec 0<"$FILE"
while read -r line
do
    F_NAME=$(basename $line)
    pkill $F_NAME
done
exit 0

