include(../duiconfig.pri)
TEMPLATE=subdirs
TEST_SUITES = \
    HomeLauncher \
    HomeMashupCategories \
    HomeSwitcher \
    HomeStartup \
    DuiAppletInstallationThroughAppletLibrary \
    HomeAppletTests \
    DuiApplicationExtensionArea

ACCEPTANCE_TESTS = \
    acceptancetests

TEST_APPS = \
    ta_homeswitcher \
    ta_callui \
    ta_activeapp \
    fixture_mashupcanvas \
    fakesystemnotificationplugin

SUBDIRS = \
    lib \
    helpers \
    $$TEST_SUITES \
    $$TEST_APPS \
    $$ACCEPTANCE_TESTS

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
test_xml.commands=buildTestXml.rb -tmeta/tests.xml -i $$TEST_SUITE_INSTALL_LOCATION -f \"$$TEST_SUITES\" && ./appendAuxTestsXml.sh && $$XMLLINT --noout --schema testdefinition.xsd meta/tests.xml
test_xml.depends=ridoc FORCE
test_xml.path=$$TEST_SUITE_INSTALL_LOCATION
test_xml.CONFIG+=no_check_exist

sudoers.path=$$SUDOERS_DIR
sudoers.files=duifw-home-tests.sudoers

INSTALLS+= \
    test_xml \
    sudoers

check.target = check
check.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check
