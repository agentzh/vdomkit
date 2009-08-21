TEMPLATE = app
#DEFINES += ENABLE_TIMER
#DEFINES += ENABLE_FASTCGI
LIBS += -lfcgi

HEADERS += vdomkit_version.h vdomkit.h vdomwebpage.h fcgi_util.h vdomkitloop.h
SOURCES += vdomkit.cpp vdomwebpage.cpp fcgi_util.cpp
SOURCES += main.cpp

CONFIG -= app_bundle
CONFIG += release uitools
DESTDIR = ../../../bin

#OUTPUT_DIR = /home/xunxin/vdom-webkit/webkit/WebKit/WebKitBuild/Release/
include(../../../WebKit.pri)
QT += network testlib
macx:QT+=xml network
#LIBS += -L$$OUT_DIR/lib
QMAKE_RPATHDIR = $$OUTPUT_DIR/lib $$QMAKE_RPATHDIR

