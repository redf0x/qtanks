TEMPLATE = app
TARGET = qtanks
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
    src/MapBuilder.cpp \
    src/ActiveItem.cpp \
    src/UnitController.cpp \
    src/Utility.cpp \
    src/KeyAssignmets.cpp \
    src/NpcController.cpp

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
    include/Utility.h \
    include/KeyAssignments.h \
    include/RTree.h \
    include/NpcController.h

RESOURCES += \
    qtanks.qrc

DISTFILES +=