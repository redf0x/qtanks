#ifndef NPCCONTROLLER_H
#define NPCCONTROLLER_H

#include "PlayerController.h"
#include "ActiveItem.h"

class NpcController : public PlayerController {
    Q_OBJECT

public:
    explicit NpcController(QObject* parent) : PlayerController(parent) { }

    virtual void msgTick (ActiveItem*);
};

#endif // NPCCONTROLLER_H
