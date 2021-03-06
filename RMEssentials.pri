
CONFIG += warn_on c++11
win32: CONFIG += skip_target_version_ext

VERSION = 9.2.15
RMEVERSION = 20190215
DEFINES += "RMEVERSION=\\\"$$RMEVERSION\\\"" "RMEVERSIONNUMBER=\\\"$$VERSION\\\""

DEFINES += QT_NO_CAST_FROM_ASCII

android || ios {
    CONFIG += staticlib
    DEFINES += MOBILE_DEVICES
} else {
    CONFIG += use_quazip
}

staticlib: DEFINES += LIBRMESSENTIALS_STATIC

!isEmpty(PREFIX) {
    CONFIG += install_build
}

#load(configure)

#qtCompileTest(quazip)

!win32-msvc* {
    # we use gcc/clang on unix-like systems and mingw
    QMAKE_CFLAGS += -Wpointer-to-int-cast
    QMAKE_CXXFLAGS += -Wc++11-compat 
    *-g++: QMAKE_CXXFLAGS += -Wzero-as-null-pointer-constant
    mac:QMAKE_LFLAGS += -Wl,-undefined -Wl,error
    else:QMAKE_LFLAGS += -Wl,--no-undefined
}

LIBS += -L$$OUT_PWD/../dist/lib
INCLUDEPATH += $$OUT_PWD/../dist/include

use_quazip {
    DEFINES += "RME_USE_QUAZIP=\\\"0.7.6\\\""
}
