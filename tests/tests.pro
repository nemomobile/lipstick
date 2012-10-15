SRCDIR = ../src
QMLDIR = $$SRCDIR/qml
TEMPLATE = subdirs
INSTALLS += tests test-definition

tests.files = tst_BatteryStatusIndicator.qml $$QMLDIR/BatteryStatusIndicator.qml stubs
tests.path = /opt/tests/lipstick-jolla-home-tests
test-definition.files = tests.xml
test-definition.path = /opt/tests/lipstick-jolla-home-tests/test-definition
