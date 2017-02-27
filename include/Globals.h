#ifndef GLOBALS_H
#define GLOBALS_H

#include "common.h"

class Globals : public QObject {
    Q_OBJECT

public:
    static const int defaultEntityWidth = 32;
    static const int defaultEntityHeight = 32;
    static const int enemyCount = 18;

    enum UserAction { EXIT, START, SETTINGS, HISCORE, CANCEL_GAME };
    Q_ENUMS(UserAction)
};

#endif // GLOBALS_H
