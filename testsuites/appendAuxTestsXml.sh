#!/bin/sh

grep -v "</testdefinition>" meta/tests.xml > meta/tests1.xml
cat acceptancetests/features/aux_tests.xml >> meta/tests1.xml
echo "</testdefinition>" >> meta/tests1.xml
mv meta/tests1.xml meta/tests.xml
