#!/bin/bash

which xmlstarlet > /dev/null
if [ "$?" -ne "0" ] ; then
  echo "xmlstarlet not found. Try installing a package called 'xmlstarlet'"
  exit 1
fi

SOURCE_DIRECTORY=../../home/declsettings

for testable in *.xml ; do
  xmlstarlet val --err --relaxng settings.rng $SOURCE_DIRECTORY/$testable
done

exit 0

