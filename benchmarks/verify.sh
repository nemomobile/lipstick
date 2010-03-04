#!/bin/sh
#
# Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# Contact: Nokia Corporation (directui@nokia.com)
#
# This file is part of duihome.
#
# If you have questions regarding the use of this file, please contact
# Nokia at directui@nokia.com.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License version 2.1 as published by the Free Software Foundation
# and appearing in the file LICENSE.LGPL included in the packaging
# of this file.
#
FPSTHRESHOLD=20
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
BAKIFS=$IFS
IFS=$(echo -en "\n\b")
exec 3<&0
exec 0<"$FILE"
SUMFPS=0
NUMLINES=0
while read -r line
do
SUMFPS=$[$SUMFPS+$line]
NUMLINES=$[$NUMLINES+1] 
done
averagefps=$SUMFPS/$NUMLINES
echo $[$averagefps]
if [ "$[$averagefps]" -le $FPSTHRESHOLD ]; then
   echo $[$averagefps]
   exit $line; 
fi
exec 0<&3
 
# restore $IFS which was used to determine what the field separators are
IFS=$BAKIFS
exit 0
