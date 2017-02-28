#include "UnitController.h"
#include "GameScene.h"
#include "Exceptions.h"
#include "Utility.h"
#include "Globals.h"

UnitController::UnitController(QObject* parent) : QObject(parent)
{
        if (!dynamic_cast<GameScene*>(parent))
            throw InvalidOwnership(parent);

        scene = qobject_cast<GameScene*>(parent);
        blkcnt = scene->getBlocksCount ();
}

void UnitController::msgDirectionChanged (ActiveItem* a)
{
    QQuickItem* target = a->getLinkedObject ();
    QRect location(target->x (), target->y (), target->width (), target->height ());
    Block* block = nullptr;
    int distance = 0;

    if (a->getFrozen ())
        return;

    block = scene->scanDirection (location, a->getDirection ());

    switch (a->getDirection ()) {
        case ActiveItem::SOUTH:
            if (block)
                distance = block->getLinkedObject ()->y () - (target->y () + target->height ());
            else
                distance = scene->getBattleFieldHeight () - (target->y () + target->height ());
            break;

        case ActiveItem::NORTH:
            if (block)
                distance = target->y () - (block->getLinkedObject ()->y () + block->getLinkedObject ()->height ());
            else
                distance = target->y ();
            break;

        case ActiveItem::EAST:
            if (block)
                distance = block->getLinkedObject ()->x () - (target->x () + target->width ());
            else
                distance = scene->getBattleFieldWidth () - (target->x () + target->width ());
            break;

        case ActiveItem::WEST:
            if (block)
                distance = target->x () - (block->getLinkedObject ()->x () + block->getLinkedObject ()->width ());
            else
                distance = target->x ();
    }

    a->setDistance (distance);
}

void UnitController::msgTick (ActiveItem* a)
{
    if (a->getFrozen ()) {
        if (a->isSpawned ()) {
            qDebug() << "run actions for" << a->getObjectId ();
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
        qDebug() << "rebuild routes";
        blkcnt = getScene ()->getBlocksCount ();
        msgDirectionChanged (a);
    }
}

GameScene* UnitController::getScene () const
{
    return scene;
}

void UnitController::msgFired (ActiveItem* a)
{
    getScene ()->fireProjectile (a);
}
