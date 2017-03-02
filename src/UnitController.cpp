#include "UnitController.h"
#include "GameScene.h"
#include "ActiveItem.h"
#include "Exceptions.h"

UnitController::UnitController(QObject* parent) : QObject(parent)
{
        if (!dynamic_cast<GameScene*>(parent))
            throw InvalidOwnership(parent);

        scene = qobject_cast<GameScene*>(parent);
}

void UnitController::msgDirectionChanged (ActiveItem* target)
{
    Q_UNUSED(target)
}

void UnitController::msgTick (ActiveItem* target)
{
    Q_UNUSED(target)
}

void UnitController::msgFired (ActiveItem* target)
{
    Q_UNUSED(target)
}

GameScene* UnitController::getScene () const
{
    return scene;
}
