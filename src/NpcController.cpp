#include "GameScene.h"
#include "NpcController.h"

void NpcController::msgTick (ActiveItem* a)
{
    QList<ActiveItem*> c;

    PlayerController::msgTick (a);

    if (a->getFrozen ())
        return;

    c = getScene()->checkImmediateCollisions (a);

    if (!c.empty ())
        a->setDistance (0);

    if (!a->getDistance ()) {
        int newDir = (rand () % (((int)ActiveItem::Direction::WEST) + 1));
        int oldDir = (int)a->getDirection ();

        if (abs (newDir - oldDir) > 1 || abs (newDir - oldDir) == 0)
            return;     /* too lazy to compute nao, maybe next time we get better rand */

        a->setDirection ((ActiveItem::Direction)newDir);

        if (oldDir - newDir == -1)
            a->setRotation (a->getRotation () + 90);
        else
            a->setRotation (a->getRotation () - 90);
    } else {
        QRectF rect(a->x (), a->y (), a->width (), a->height ());

        switch (a->getDirection ()) {
            case ActiveItem::Direction::NORTH:
                rect.translate (0, -Globals::npcBaseSpeed);
                break;

            case ActiveItem::Direction::SOUTH:
                rect.translate (0, Globals::npcBaseSpeed);
                break;

            case ActiveItem::Direction::WEST:
                rect.translate (-Globals::npcBaseSpeed, 0);
                break;

            case ActiveItem::Direction::EAST:
                rect.translate (Globals::npcBaseSpeed, 0);
                break;
        }

        a->setX (rect.x ()); a->setY (rect.y ());
        a->setDistance (a->getDistance () - Globals::npcBaseSpeed);
    }
}
