# Load more defines from the m_defines...
load(meegotouch_defines)
# duihome specific directories
unix {
     MEEGOHOME_DIR = $$M_INSTALL_DATA/duihome
     MEEGOHOME_THEMES_DIR = /usr/share/themes/base/meegotouch/duihome

     TEST_SUITE_INSTALL_LOCATION=/usr/share/mfw-home-tests
     TEST_APP_INSTALL_LOCATION=/usr/lib/mfw-home-tests
     TEST_THEME_ROOT_LOCATION=/usr/share/themes/base/m
     TEST_BENCHMARKS_INSTALL_LOCATION=/usr/share/mfw-home-benchmark-tests
}
mac {
     # Do mac stuff here
}
win32 {
     # Do win32 stuff here
}
