include(../../duiconfig.pri)
TEMPLATE=subdirs

suites.path=$$TEST_SUITE_INSTALL_LOCATION/DuiApplicationExtensionArea
suites.files=ts_duiapplicationextensionarea.rb \

cases.path=$$TEST_SUITE_INSTALL_LOCATION/DuiApplicationExtensionArea/cases
cases.files=cases/tc_duiapplicationextensionarea.rb \

INSTALLS += \
    cases \
    suites \

include(../check.pri)
