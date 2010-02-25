include(../duiconfig.pri)
TEMPLATE=subdirs
TEST_SUITES = \
    NotificationSystem \
    HomeLauncher \
    HomeMashupCategories \
    HomeSwitcher \
    HomeStatusIndicatorMenu \
    HomeStartup \
    DuiAppletInstallationThroughAppletLibrary \
    HomeAppletTests \
    DuiApplicationExtensionArea

TEST_APPS = \
    ta_homeswitcher \
    ta_callui \
    fakesystemnotificationplugin

SUBDIRS = \
    lib \
    $$TEST_SUITES \
    $$TEST_APPS

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
test_xml.commands=buildTestXml.rb -tmeta/tests.xml -i $$TEST_SUITE_INSTALL_LOCATION -f \"$$TEST_SUITES\" && $$XMLLINT --noout --schema testdefinition.xsd meta/tests.xml
test_xml.depends=ridoc FORCE
test_xml.path=$$TEST_SUITE_INSTALL_LOCATION
test_xml.CONFIG+=no_check_exist

INSTALLS+= \
    test_xml

check.target = check
check.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check
