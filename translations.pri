# This include file makes generation and installation of .qm files
# from .ts files and updating .ts files from the sources easier.
#
# An application which has translations can use this include file as
# in the following example for “widgetsgallery”:
#
# Create a subdirectory “translations” in the source folder of the
# application which contains a translations.po file and the .ts
# files, for example if the source dir is “libm/demos/widgetsgallery”:
#
#    libm/demos/widgetsgallery/translations.pro
#    libm/demos/widgetsgallery/widgetsgallery_ar.ts
#    libm/demos/widgetsgallery/widgetsgallery_de.ts
#    libm/demos/widgetsgallery/widgetsgallery_en.ts
#    libm/demos/widgetsgallery/widgetsgallery_fi.ts
#
# The translations.pro file should have contents like this:
#
#    LANGUAGES = ar de en fi
#    CATALOGNAME = widgetsgallery
#    SOURCEDIR = $$PWD/..
#    TRANSLATIONDIR = $$PWD
#    MROOT = ../../..
#    include($$MROOT/mconfig.pri)
#    include($$MROOT/translations.pri)
#
# It is better to have an extra translations.pro file in the directory
# where the translations are instead of adding the stuff for the translations
# to the .pro file of the application (“widgetsgallery.pro”).
# It is possible to add similar stuff to update the .qm files
# directly to the application profile but the disadvantage is that
# changing the .ts files then causes the application to rebuild
# (because of qmake limitations).
#
# Introducing the extra translations/translations.pro file makes it
# necessary to add the widgetsgallery/translations directory to the
# list of sub-directories in a .pro file higher up in the directory
# hierarchy. For example the libm/demos/demos.pro file contains
# something like the following:
#
#    SUBDIRS     =  \
#        servicefw \
#        widgetsgallery \
#        widgetsgallery/translations \
#        animatedlayout \
#        animatedlayout/translations \
#        launcher \
#        qtstyleexample
#

# get around lack of sleep on Windows:
unix:SLEEP = sleep 1
else:SLEEP = ping -n 5 localhost > NUL
# null device:
unix:NULL = /dev/null
else:NULL = NUL

# var, prepend, append
defineReplace(prependAll) {
    for(a,$$1):result += $$2$${a}$$3
    return($$result)
}

isEmpty(LRELEASE_OPTIONS) {
    LRELEASE_OPTIONS = -idbased
}
isEmpty(LUPDATE_OPTIONS) {
    LUPDATE_OPTIONS = -locations absolute -no-ui-lines -no-sort
}
defineReplace(targetPath) {
    return($$replace(1, /, $$QMAKE_DIR_SEP))
}
LRELEASE = $$targetPath($$[QT_INSTALL_BINS]/lrelease) $$LRELEASE_OPTIONS
LUPDATE = $$targetPath($$[QT_INSTALL_BINS]/lupdate) $$LUPDATE_OPTIONS
# add “-markuntranslated '!!\ '” if available (needs new Qt 4.6):
out = $$system($$LRELEASE -help)
# the second parameter of contains can be a regexp!
contains(out, .*markuntranslated.*) {
    LRELEASE += -markuntranslated '!!\ '
}

TRANSLATIONS = $$prependAll(LANGUAGES, $$TRANSLATIONDIR/$${CATALOGNAME}_,.ts)

isEqual(TEMPLATE_PREFIX, vc):vcproj=1
isEmpty(vcproj) {
    QMAKE_LINK = @: IGNORE THIS LINE
    OBJECTS_DIR =
    win32:CONFIG -= embed_manifest_exe
} else {
    CONFIG += console
    PHONY_DEPS = .
    phony_src.input = PHONY_DEPS
    phony_src.output = phony.c
    phony_src.variable_out = GENERATED_SOURCES
    phony_src.commands = echo int main() { return 0; } > phony.c
    phony_src.name = CREATE phony.c
    phony_src.CONFIG += combine
    QMAKE_EXTRA_COMPILERS += phony_src
}


!isEqual(DISABLE_QTTRID_ENGINEERING_ENGLISH, "yes") {
    ENGINEERING_ENGLISH = $$OUT_PWD/$${CATALOGNAME}.ts
    # updating the engineering English .ts file should be done *always*
    # because it is fully auto-generated, no manual editing is needed:

    DUMMY_DEP = $$OUT_PWD/dummy-dependency-this-file-will-never-be-created
    updateeets.input = DUMMY_DEP
    updateeets.output = $$ENGINEERING_ENGLISH
    !isEmpty(vcproj):updateeets.variable_out = GENERATED_SOURCES
    updateeets.commands = $$LUPDATE $$SOURCEDIR -ts $$ENGINEERING_ENGLISH
    updateeets.name = LUPDATE $$ENGINEERING_ENGLISH
    updateeets.CONFIG += no_link target_predeps
    QMAKE_EXTRA_COMPILERS += updateeets

    FORCED_DEP = .
    dummy.input = FORCED_DEP
    dummy.output = $$DUMMY_DEP
    !isEmpty(vcproj):dummy.variable_out = GENERATED_SOURCES
    # we do not really create $$DUMMY_DEP here, make does not check the
    # output of the build steps, it assumes that the build command either
    # fails or that the output is created:
    dummy.commands = echo > $$NULL
    dummy.CONFIG += no_link target_predeps
    QMAKE_EXTRA_COMPILERS += dummy
}

# the stuff below works as well on Linux, but does not work with vcproj
# because PRE_TARGETDEPS (just like .CONFIG += target_predeps) does
# not work for vcproj output.
#
# !isEqual(DISABLE_QTTRID_ENGINEERING_ENGLISH, "yes") {
#     ENGINEERING_ENGLISH = $$OUT_PWD/$${CATALOGNAME}.ts
#     # updating the engineering English .ts file should be done *always*
#     # because it is fully auto-generated, no manual editing is needed:
#
#     updateeets.commands = $$LUPDATE $$SOURCEDIR -ts $$ENGINEERING_ENGLISH
#     QMAKE_EXTRA_TARGETS += updateeets
#     QMAKE_CLEAN += $$ENGINEERING_ENGLISH
#     PRE_TARGETDEPS = updateeets
# }

# other .ts files besides the engineering English one should only be updated
# on request, i.e. only when calling “make updatets” manually:
updatets.commands = $$LUPDATE $$SOURCEDIR -ts $$TRANSLATIONS
updatets.target = updatets
QMAKE_EXTRA_TARGETS += updatets

updateqm.input = TRANSLATIONS
!isEqual(DISABLE_QTTRID_ENGINEERING_ENGLISH, "yes") {
    updateqm.input += ENGINEERING_ENGLISH
}
updateqm.output = $$OUT_PWD/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$LRELEASE ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_OUT};
updateqm.name = LRELEASE ${QMAKE_FILE_IN}
updateqm.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += updateqm

isEqual(INSTALL_TRANSLATIONS, "no") {
    # do not install the .qm files
}
else {
    isEmpty(TRANSLATION_INSTALLDIR) {
        TRANSLATION_INSTALLDIR = $${M_TRANSLATION_DIR}
    }
    qmfiles.files = $$prependAll(LANGUAGES, $$OUT_PWD/$${CATALOGNAME}_,.qm)
    !isEqual(DISABLE_QTTRID_ENGINEERING_ENGLISH, "yes") {
        qmfiles.files += $$OUT_PWD/$${CATALOGNAME}.qm
    }
    qmfiles.CONFIG += no_check_exist
    qmfiles.path = $${TRANSLATION_INSTALLDIR}

    INSTALLS += qmfiles
}
