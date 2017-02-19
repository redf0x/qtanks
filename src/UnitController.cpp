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
    QQuickItem* o = a->getLinkedObject ();

    if (o)
        qDebug() << a->getObjectId () << " now @ (" << a->getLinkedObject ()->x () << ", " \
                 << a->getLinkedObject()->y () << ")";
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
    else {
        Block* block = scene->getNearestObstacle (newLoc);

        if (block)
            result = false;
    }

    return result;
}
