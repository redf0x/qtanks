#ifndef GLOBALS_H
#define GLOBALS_H

#include "common.h"

class Globals : public QObject {
    Q_OBJECT

public:
    explicit Globals(QObject* parent = 0) : QObject(parent) { }

    static const int defaultEntityWidth = 32;
    static const int defaultEntityHeight = 32;

    enum { NORTH, EAST, SOUTH, WEST };
    enum { EXIT, START, SETTINGS, HISCORE };
};

#endif // GLOBALS_H
