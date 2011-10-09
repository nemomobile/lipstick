# ####################################################################
# Main projectfile
# ####################################################################

defineTest(addSubDirs)
{
	for(subdirs, 1) {
		entries = $$files($$subdirs)
		for(entry, entries) {
			name = $$replace(entry, [/\\\\], _)
			SUBDIRS += $$name
			eval ($${name}.subdir = $$entry)
			for(dep, 2):eval ($${name}.depends += $$replace(dep, [/\\\\], _))
				export ($${name}.subdir)
				export ($${name}.depends)
		}
	}
	export (SUBDIRS)
}


TEMPLATE = subdirs

addSubDirs(src)

QMAKE_CLEAN += \
    build-stamp \
    configure-stamp \
    artifacts/*.deb \
    *.log.xml \
    *.log

QMAKE_DISTCLEAN += \
    build-stamp \
    configure-stamp \
    *.log.xml \
    *.log

check.target = check
check.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check

check-xml.target = check-xml
check-xml.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check-xml

