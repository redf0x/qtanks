#ifndef GLOBALS_H
#define GLOBALS_H

#include "common.h"

#define FIRE_AT_LEAST 500
#define FIRE_AT_MOST 15
#define RESPAWN_TIMEOUT 30
#define CLEANUP_TIMEOUT 3

class Globals : public QObject {
    Q_OBJECT
    Q_PROPERTY(int defaultEntityWidth MEMBER defaultEntityWidth CONSTANT)
    Q_PROPERTY(int defaultEntityHeight MEMBER defaultEntityHeight CONSTANT)
    Q_PROPERTY(int enemyCount MEMBER enemyCount CONSTANT)
    Q_PROPERTY(int npcBaseSpeed MEMBER npcBaseSpeed CONSTANT)
    Q_PROPERTY(int projectileSpeed MEMBER projectileSpeed CONSTANT)
    Q_PROPERTY(int fieldCellRows MEMBER fieldCellRows CONSTANT)
    Q_PROPERTY(int fieldCellColumns MEMBER fieldCellColumns CONSTANT)

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

    explicit Globals(QObject* parent = 0) : QObject(parent) { }
    ~Globals() { }
};

#endif // GLOBALS_H
