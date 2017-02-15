#ifndef UNITCONTROLLER_H
#define UNITCONTROLLER_H

#include "common.h"

class GameScene;
class ActiveItem;

class UnitController : public QObject {
    Q_OBJECT

public:
    explicit UnitController(QObject* parent = 0);

    void msgDirectionChanged (ActiveItem*);

private:
    GameScene* scene;
};

#endif // UNITCONTROLLER_H
