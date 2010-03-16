include(../../duiconfig.pri)
TEMPLATE=subdirs

suites.path=$$TEST_BENCHMARKS_INSTALL_LOCATION/SwitcherTest
suites.files=*.rb

cases.path=$$TEST_BENCHMARKS_INSTALL_LOCATION/SwitcherTest/cases
cases.files=cases/*.rb

INSTALLS += \
    cases \
    suites \
