#!/bin/bash

DOMAIN="ui"
FEATURE="Lipstick"

UT_TESTCASES=""

for TEST in `ls -d ?t_*`
do
	if [ -x $TEST/$TEST -a ! -e $TEST/disabled ]
	then
		TESTCASE_TEMPLATE="<case name=\"$TEST\" description=\"$TEST\">
        <step expected_result=\"0\">/opt/tests/lipstick-tests/$TEST</step>
      </case>
      "

		if [ -n "`echo $TEST | egrep '^u'`" ]
		then
			UT_TESTCASES="${UT_TESTCASES}${TESTCASE_TEMPLATE}"
		else
			FT_TESTCASES="${FT_TESTCASES}${TESTCASE_TEMPLATE}"
		fi
	fi
done

TESTSUITE_TEMPLATE="<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<testdefinition version=\"1.0\">
  <suite name=\"lipstick-tests\" domain=\"$DOMAIN\">
    <set name=\"unit-tests\" feature=\"$FEATURE\">
      $UT_TESTCASES
    </set>
  </suite>
</testdefinition>"

echo "$TESTSUITE_TEMPLATE"

