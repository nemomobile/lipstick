#!/bin/bash

DOMAIN="Application Framework"
FEATURE="Direct UI Core"
TYPE="Functional"
LEVEL="Component"

UT_TESTCASES=""

for TEST in `ls -d ?t_*`; do
	if [ -x $TEST/$TEST ]; then

TESTCASE_TEMPLATE="<case name=\"$TEST\" description=\"$TEST\" requirement=\"\" timeout=\"60\">
        <step expected_result=\"0\">/usr/lib/meegotouchhome-tests/$TEST</step>
      </case>
      "

		if [ -n "`echo $TEST | egrep '^u'`" ]; then
			UT_TESTCASES="${UT_TESTCASES}${TESTCASE_TEMPLATE}"
		else
			FT_TESTCASES="${FT_TESTCASES}${TESTCASE_TEMPLATE}"
		fi
	fi
done

TESTSUITE_TEMPLATE="<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>
<testdefinition version=\"0.1\">
  <suite name=\"meegotouchhome-tests\" domain=\"$DOMAIN\" type=\"$TYPE\" level=\"$LEVEL\">
    <set name=\"unit_tests\" description=\"Unit Tests\" feature=\"$FEATURE\">

      $UT_TESTCASES

      <environments>
        <scratchbox>false</scratchbox>
        <hardware>true</hardware>    
      </environments> 

    </set>

  </suite>
</testdefinition>"

echo "$TESTSUITE_TEMPLATE"

