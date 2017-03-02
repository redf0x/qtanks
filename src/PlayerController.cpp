#include "PlayerController.h"
#include "GameScene.h"
#include "Utility.h"
#include "Globals.h"

PlayerController::PlayerController(QObject* parent) : UnitController(parent)
{
        blkcnt = getScene ()->getBlocksCount ();
}

void PlayerController::msgDirectionChanged (ActiveItem* a)
{
    QRect location(a->x (), a->y (), a->width (), a->height ());
    Block* block = nullptr;
    int distance = 0;

    if (a->getFrozen ())
        return;

    block = getScene ()->scanDirection (location, a->getDirection ());

    switch (a->getDirection ()) {
        case ActiveItem::SOUTH:
            if (block)
                distance = block->y () - (a->y () + a->height ());
            else
                distance = getScene ()->getBattleFieldHeight () - (a->y () + a->height ());
            break;

        case ActiveItem::NORTH:
            if (block)
                distance = a->y () - (block->y () + block->height ());
            else
                distance = a->y ();
            break;

        case ActiveItem::EAST:
            if (block)
                distance = block->x () - (a->x () + a->width ());
            else
                distance = getScene ()->getBattleFieldWidth () - (a->x () + a->width ());
            break;

        case ActiveItem::WEST:
            if (block)
                distance = a->x () - (block->x () + block->width ());
            else
                distance = a->x ();
    }

    a->setDistance (distance);
}

void PlayerController::msgTick (ActiveItem* a)
{
    if (a->getFrozen ()) {
        if (a->isSpawned ()) {
            a->setArmor (1);
            a->setRotation (0);
            a->setDirection (ActiveItem::Direction::SOUTH);
            a->setDistance (0);
            a->setAlive (true);
            a->setFrozen (false);
        }

        return;
    }

    if (getScene ()->getBlocksCount () != blkcnt) {
        blkcnt = getScene ()->getBlocksCount ();
        msgDirectionChanged (a);
    }
}

void PlayerController::msgFired (ActiveItem* a)
{
    getScene ()->fireProjectile (a);
}
