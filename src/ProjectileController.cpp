#include "ProjectileController.h"
#include "GameScene.h"

void ProjectileController::msgTick (ActiveItem *a)
{
    QRectF location(a->x (), a->y (), a->width (), a->height ());
    QRect region(location.x (), location.y (), location.width (), location.height ());
    Block* block = nullptr;
    BlockList l;
    ObjectList targets;

    if (a->getFrozen () || !a->isAlive ())
        return;

    if ((block = getScene ()->scanDirection (region, a->getDirection ())))
        l << block;

    targets = getScene ()->checkImmediateCollisions (a, l);

    if (!targets.empty ()) {
        for_all_objects(targets, handleImpact);
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

    a->setX (location.x ()); a->setY (location.y ());
}

void ProjectileController::destroyProjectile (ActiveItem* a)
{
    a->setAlive (false);
}

void ProjectileController::handleImpact (Entity* entity)
{
    entity->setArmor (entity->getArmor () - 1);

    if (entity->getArmor () <= 0) {
        Block* block = qobject_cast<Block*>(entity);

        if (block != nullptr)
            getScene ()->removeBlock (block);
        else {
            ActiveItem* x = qobject_cast<ActiveItem*>(entity);

            if (x != nullptr) {
                if (!x->isAlive ())     /* can't simply shoot a zombie */
                    return;
                else {
                    x->setAlive (false);
                    getScene ()->respawn (x);
                }
            }
        }
    }
}
