include(../../duiconfig.pri)
TEMPLATE=subdirs

 PACKAGE_MANAGER_DBUS_CONFIG_FILE_LOCATION=/etc/dbus-1/system.d

suites.path=$$TEST_SUITE_INSTALL_LOCATION/DuiAppletInstallationThroughAppletLibrary
suites.files=ts_duiappletinstallationthroughappletlibrary.rb \

cases.path=$$TEST_SUITE_INSTALL_LOCATION/DuiAppletInstallationThroughAppletLibrary/cases
cases.files=cases/tc_*.rb \

dbus_config.path=$$PACKAGE_MANAGER_DBUS_CONFIG_FILE_LOCATION
dbus_config.files=*.conf \

INSTALLS += \
    cases \
    suites \
    dbus_config

include(../check.pri)
