include(../shared.pri)
TEMPLATE = subdirs

addSubDirs(libmeegotouchhome)
addSubDirs(libmeegotouchhome/translations)
addSubDirs(home, libmeegotouchhome)
addSubDirs(extensions, libmeegotouchhome)
