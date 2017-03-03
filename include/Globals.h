#ifndef GLOBALS_H
#define GLOBALS_H

#include "common.h"

#define FIRE_AT_LEAST 500
#define FIRE_AT_MOST 15
#define RESPAWN_TIMEOUT 30
#define CLEANUP_TIMEOUT 3

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
};

#endif // GLOBALS_H
