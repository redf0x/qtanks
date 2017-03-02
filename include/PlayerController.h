#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "common.h"
#include "UnitController.h"

class PlayerController : public UnitController {
    Q_OBJECT

public:
    explicit PlayerController(QObject* parent);

    virtual void msgDirectionChanged (ActiveItem*);
    virtual void msgTick (ActiveItem*);
    virtual void msgFired (ActiveItem*);

private:
    int blkcnt;
};

#endif // PLAYERCONTROLLER_H
