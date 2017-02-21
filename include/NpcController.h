#ifndef NPCCONTROLLER_H
#define NPCCONTROLLER_H

#include "UnitController.h"
#include "ActiveItem.h"

class NpcController : public UnitController {
    Q_OBJECT

public:
    explicit NpcController(QObject* parent = 0) : UnitController(parent) { }

    virtual void msgTick (ActiveItem*);
};

#endif // NPCCONTROLLER_H
