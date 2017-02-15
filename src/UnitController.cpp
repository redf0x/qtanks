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
