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
}

void UnitController::msgDirectionChanged (ActiveItem* a)
{
    QQuickItem* target = a->getLinkedObject ();
    QRect location(target->x (), target->y (), target->width (), target->height ());
    Block* block = nullptr;
    int distance = 0;

    if (target)
        qDebug() << a->getObjectId () << " now @ (" << target->x () << ", " \
                 << target->y () << ")";

    block = scene->scanDirection (location, a->getDirection ());

    if (block)
        qDebug() << "Obstacle @ " << block->getLinkedObject ()->x () << "," << block->getLinkedObject ()->y () <<   \
                    block->getLinkedObject ()->width () << "x" << block->getLinkedObject ()->height ();
    else
        qDebug() << "No obstacles found";

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
    qDebug() << "new distance" << a->getDistance ();
}

bool UnitController::msgAdvance (ActiveItem* a, int disp)
{
    bool result = true;
    QQuickItem* target = a->getLinkedObject ();
    QRect globalBounds(target->parentItem ()->x (), target->parentItem ()->y (),
                       target->parentItem ()->width (), target->parentItem ()->height ());
    QRect newLoc (target->x (), target->y (), target->width (), target->height ());

    switch (a->getDirection ()) {
        case ActiveItem::SOUTH:
            newLoc.translate (0, disp);
            break;

        case ActiveItem::NORTH:
            newLoc.translate (0, -disp);
            break;

        case ActiveItem::EAST:
            newLoc.translate (disp, 0);
            break;

        case ActiveItem::WEST:
            newLoc.translate (-disp, 0);
    }

    if (newLoc.x () < 0 || newLoc.y () < 0 || newLoc.x () + newLoc.width () > globalBounds.width () ||
            newLoc.y () + newLoc.height () > globalBounds.height ())
        result = false;

    return result;
}
