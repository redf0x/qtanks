#ifndef PROJECTILECONTROLLER_H
#define PROJECTILECONTROLLER_H

#include "UnitController.h"
#include "Entity.h"
#include "ActiveItem.h"

class ProjectileController : public UnitController {
    Q_OBJECT

public:
    explicit ProjectileController(QObject* parent) : UnitController(parent) { }

    virtual void msgTick (ActiveItem*);

private:
    void destroyProjectile (ActiveItem*);
    void handleImpact (Entity* entity);
};

#endif // PROJECTILECONTROLLER_H
