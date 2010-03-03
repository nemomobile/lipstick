include(../../duiconfig.pri)
TEMPLATE=subdirs

suites.path=$$TEST_BENCHMARKS_INSTALL_LOCATION/HomeSwitcher
suites.files=*.rb

cases.path=$$TEST_BENCHMARKS_INSTALL_LOCATION/HomeSwitcher/cases
cases.files=cases/*.rb

INSTALLS += \
    cases \
    suites \
