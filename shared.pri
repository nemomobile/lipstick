defineReplace(findFile) {
    FILE=$$1

    # LIST=$$(PATH)
    # for a reason beyond my knowledge, if i get path list from $$(PATH) and replace all :'s with space,
    # i cant iterate the entries with qmake's for loop.
    # SO, here's a list of predefined places where to look for executable files..
    # Also, i cant rely to find doxygen via system() call since doxygen returns error every time its called
    # if it cant find Doxyfile =(

    LIST  = /scratchbox/devkits/cputransp/bin /scratchbox/devkits/maemo3-tools/bin
    LIST += /scratchbox/devkits/debian-etch/bin /scratchbox/devkits/perl/bin /scratchbox/tools/bin
    LIST += /targets/links/arch_tools/bin /host_usr/bin /scratchbox/compilers/bin /bin /usr/bin
    LIST += /usr/local/bin /usr/X11R6/bin ~/bin .

    for(path,LIST) {
        exists($${path}/$${FILE}):LOCATED = $${path}/$${FILE}
    }

    return($${LOCATED})
}

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
