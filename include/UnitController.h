#ifndef UNITCONTROLLER_H
#define UNITCONTROLLER_H

#include "common.h"

class GameScene;
class ActiveItem;

class UnitController : public QObject {
    Q_OBJECT

public:
    explicit UnitController(QObject* parent);

    virtual void msgDirectionChanged (ActiveItem* target);
    virtual void msgTick (ActiveItem* target);
    virtual void msgFired (ActiveItem* target);

protected:
    virtual GameScene* getScene () const;

private:
    GameScene* scene;
};

#endif // UNITCONTROLLER_H
