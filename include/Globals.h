#ifndef GLOBALS_H
#define GLOBALS_H

#include "common.h"

class Globals : public QObject {
    Q_OBJECT

public:
    static const int defaultEntityWidth = 32;
    static const int defaultEntityHeight = 32;

    enum { EXIT, START, SETTINGS, HISCORE };
};

#endif // GLOBALS_H
