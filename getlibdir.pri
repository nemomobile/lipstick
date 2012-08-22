
LIBDIR = /usr/lib

unix {
    exists("/usr/bin/rpm") {
        LIBDIR = $$system(/usr/bin/rpm --eval %{_libdir})
        message("We have rpm on this system, using libdir from there for INSTALLS")
    }
    else {
        linux-g++ {
            HARDWARE_PLATFORM = $$system(uname -i)
            contains(HARDWARE_PLATFORM, x86_64) {
                # Use /usr/lib64 on 64-bit Linux
                LIBDIR = /usr/lib64
                message("This is a 64-bit Linux build! Using /usr/lib64 for INSTALLS")
            }
        }
    }
}
