#include "ProjectileController.h"
#include "GameScene.h"

void ProjectileController::msgTick (ActiveItem *a)
{
    QQuickItem* target = a->getLinkedObject ();
    QRectF location(target->x (), target->y (), target->width (), target->height ());
    QRect region(location.x (), location.y (), location.width (), location.height ());
    Block* block = nullptr;
    QList<Block*> l;
    QList<Entity*> targets;

    if (a->getFrozen ())
        return;

    if ((block = getScene ()->scanDirection (region, a->getDirection ())))
        l << block;

    targets = getScene ()->checkImmediateCollisions (a, l);

    if (!targets.empty ()) {
        ActiveItem* x;

        for (QList<Entity*>::iterator t = targets.begin (); t != targets.end (); t++) {
            (*t)->setArmor ((*t)->getArmor () - 1);

            if ((*t)->getArmor () <= 0) {
                block = qobject_cast<Block*>((*t));

                if (block != nullptr)
                    getScene ()->removeBlock (block);
                else {
                    x = qobject_cast<ActiveItem*>((*t));

                    if (x != nullptr)
                        getScene ()->respawn (x);
                }
            }
        }

        destroyProjectile (a);
        return;
    }

    switch (a->getDirection ()) {
        case ActiveItem::Direction::NORTH:
            location.translate (0, -Globals::projectileSpeed);
            break;

        case ActiveItem::Direction::SOUTH:
            location.translate (0, Globals::projectileSpeed);
            break;

        case ActiveItem::Direction::WEST:
            location.translate (-Globals::projectileSpeed, 0);
            break;

        case ActiveItem::Direction::EAST:
            location.translate (Globals::projectileSpeed, 0);
            break;
    }

    if (location.x () < 0 || location.y () < 0 || location.x () > getScene ()->getBattleFieldWidth () ||
            location.y () > getScene ()->getBattleFieldHeight ()) {
        destroyProjectile (a);
        return;
    }

    target->setPosition (QPointF(location.x (), location.y ()));
}

void ProjectileController::destroyProjectile (ActiveItem* a)
{
    if (a->getFrozen ())
        return;

    if (getScene ())
        getScene ()->removeProjectile (a);
}

void ProjectileController::msgDirectionChanged (ActiveItem* a)
{
    Q_UNUSED(a)
}
