TEMPLATE = app
TARGET = qtanks
DESTDIR = build
OBJECTS_DIR = build
INCLUDEPATH += include
QT += qml quick quickwidgets widgets
CONFIG += qt debug warn_on c++14

SOURCES += \
    src/main.cpp \
    src/Entity.cpp \
    src/Block.cpp \
    src/GameScene.cpp \
    src/MapBuilder.cpp \
    src/ActiveItem.cpp \
    src/NpcController.cpp \
    src/Loader.cpp \
    src/ProjectileController.cpp \
    src/PlayerController.cpp \
    src/UnitController.cpp \
    src/KeyAssignments.cpp

HEADERS += \
    include/common.h \
    include/Entity.h \
    include/Globals.h \
    include/Block.h \
    include/GameScene.h \
    include/MapBuilder.h \
    include/ActiveItem.h \
    include/UnitController.h \
    include/Exceptions.h \
    include/KeyAssignments.h \
    include/RTree.h \
    include/NpcController.h \
    include/Loader.h \
    include/ProjectileController.h \
    include/PlayerController.h

RESOURCES += \
    qtanks.qrc

DISTFILES +=
