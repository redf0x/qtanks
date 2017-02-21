#ifndef UNITCONTROLLER_H
#define UNITCONTROLLER_H

#include "common.h"

class GameScene;
class ActiveItem;

class UnitController : public QObject {
    Q_OBJECT

public:
    explicit UnitController(QObject* parent = 0);

    virtual void msgDirectionChanged (ActiveItem*);
    virtual void msgTick (ActiveItem*);

protected:
    virtual GameScene* getScene () const;

private:
    GameScene* scene;
};

#endif // UNITCONTROLLER_H
