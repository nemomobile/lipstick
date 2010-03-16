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
# The file reading part has been adapted from
# http://bash.cyberciti.biz/

 
check_process_started() {
SERVICE=$1

if ps ax | grep -v grep | grep $SERVICE > /dev/null
then
    return 1
else
    return 0
fi

}

processLine(){
  line="$@" # get all args
  exec $line &
  sleep 5
  check_process_started $line
  started=$?
  if [ "$started" == "0" ]; then
    echo "$line : Could not be started ! Maybe it does not exist ?"
    exit 3; 
  fi
}
 
### Main script stars here ###
# Store file name

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
 
exec 0<"$FILE"
while read -r line
do
	# use $line variable to process line in processLine() function
	processLine $line
done
exit 0
