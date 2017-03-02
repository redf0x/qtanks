#ifndef GLOBALS_H
#define GLOBALS_H

#include "common.h"

class Globals : public QObject {
    Q_OBJECT

public:
    static const int defaultEntityWidth = 32;
    static const int defaultEntityHeight = 32;
    static const int enemyCount = 18;
    static const int npcBaseSpeed = 4;
    static const int projectileSpeed = 10;
    static const int fieldCellRows = 26;
    static const int fieldCellColumns = 26;

    enum UserAction { EXIT, START, SETTINGS, HISCORE, CANCEL_GAME, GAMEOVER };
    Q_ENUMS(UserAction)

    Q_PROPERTY(int fieldCellRows MEMBER fieldCellRows CONSTANT)
    Q_PROPERTY(int fieldCellColumns MEMBER fieldCellColumns CONSTANT)
};

#endif // GLOBALS_H
