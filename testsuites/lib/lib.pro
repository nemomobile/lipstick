include(../../duiconfig.pri)
TEMPLATE=subdirs

lib.path=$$TEST_SUITE_INSTALL_LOCATION/lib
lib.files=runFixup.rb utils.rb

INSTALLS += \
    lib

QMAKE_EXTRA_TARGETS += check
check.commands=ruby -c $${lib.files}
