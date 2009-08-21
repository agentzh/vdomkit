TEMPLATE = app
#DEFINES += ENABLE_TIMER
DEFINES += ENABLE_FASTCGI
LIBS += -lfcgi
QT+=xml network webkit

HEADERS += vdomkit_version.h vdomkit.h vdomwebpage.h fcgi_util.h vdomkitloop.h
SOURCES += vdomkit.cpp vdomwebpage.cpp fcgi_util.cpp
SOURCES += main.cpp

isEmpty(OUTPUT_DIR) {
    CONFIG(release):OUTPUT_DIR=$$PWD
    CONFIG(debug):OUTPUT_DIR=$$PWD
}

isEmpty(FCGI_DIR) {
} else {
    INCLUDEPATH = $$FCGI_DIR/include $$INCLUDEPATH
    QMAKE_LIBDIR_FLAGS += -L$$FCGI_DIR/lib
}

INCLUDEPATH += $$QTDIR/include/QtWebKit

CONFIG -= app_bundle
CONFIG += release uitools
#DESTDIR = ../../../bin

#OUTPUT_DIR = /home/xunxin/vdom-webkit/webkit/WebKit/WebKitBuild/Release/
#include(../../../WebKit.pri)
QT += network testlib
macx:QT+=xml network
#LIBS += -L$$OUT_DIR/lib
#QMAKE_RPATHDIR = $$OUTPUT_DIR/lib $$QMAKE_RPATHDIR

QMAKE_RPATHDIR = $$QMAKE_RPATHDIR
target.path = $$OUTPUT_DIR/bin
INSTALLS += target

