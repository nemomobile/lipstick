QMAKE_STRIP = echo
OBJECTS_DIR = .obj
MOC_DIR = .moc
DESTDIR = ../../lib

CONFIG += qt warn_on depend_includepath
CONFIG += qmake_cache target_qt 

#contains(QT_CONFIG, reduce_exports):CONFIG += hide_symbols
#unix:contains(QT_CONFIG, reduce_relocations):CONFIG += bsymbolic_functions

include(../../m_targets.pri)
