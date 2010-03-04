include(../duiconfig.pri)
TEMPLATE=subdirs

TEST_SUITES = \
    SwitcherTest

files.path=$$TEST_BENCHMARKS_INSTALL_LOCATION
files.files= killer.sh \
	programlist.txt \
	programloader.sh \
	toucher.sh \

unix{
  QMAKE_DEL_FILE = rm -rf
}
QMAKE_CLEAN += .ri meta/tests.xml
QMAKE_DISTCLEAN += .ri meta/tests.xml

QMAKE_EXTRA_TARGETS += \
    doc \
    ridoc \
    test_xml

doc.commands = rdoc -d -x ts_*.rb -I png -o ./doc

XMLLINT=xmllint

ridoc.target=./.ri/created.rid
ridoc.commands=rdoc -r --op .ri

test_xml.PRE_TARGETDEPS=./.ri/created.rid
test_xml.target=meta/tests.xml
test_xml.files=meta/tests.xml
test_xml.commands=buildTestXml.rb -tmeta/tests.xml.in -i $$TEST_BENCHMARKS_INSTALL_LOCATION -f \"$$TEST_SUITES\" && ./patch-tests-xml < meta/tests.xml.in > meta/tests.xml
test_xml.depends=ridoc FORCE
test_xml.path=$$TEST_BENCHMARKS_INSTALL_LOCATION
test_xml.CONFIG+=no_check_exist

INSTALLS += \
    files \
    test_xml \
