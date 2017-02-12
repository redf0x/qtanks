TEMPLATE = app
TARGET = tankz
DESTDIR = build
OBJECTS_DIR = build
INCLUDEPATH += include
QT += qml quick quickwidgets widgets
CONFIG += qt debug warn_on

SOURCES += \
    src/main.cpp \
    src/Entity.cpp \
    src/Block.cpp \
    src/GameScene.cpp \
    src/MapBuilder.cpp

HEADERS += \
    include/common.h \
    include/Entity.h \
    include/Globals.h \
    include/Block.h \
    include/GameScene.h \
    include/MapBuilder.h

RESOURCES += \
    tankz.qrc

DISTFILES +=