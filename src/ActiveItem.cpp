#include "ActiveItem.h"

ActiveItem::ActiveItem(QObject* parent, int rotation, ActiveItem::ActiveItemType type, UnitController* u) :
    Entity(parent), _type(type), _uc(u), _frozen(false), _distance(0)
{
    setRotation (rotation);
    setDirection (Direction::SOUTH);
}

QString ActiveItem::getTextureSource () const
{
    QString name("");

    switch (_type) {
        case BASE:
            name = "eagle";
            break;

        case PLAYER:
            name = "tank_player";
            break;

        case NPC:
            name = "tank_basic";
    }

    return "qrc:/image/ui/res/" + name + ".png";
}

ActiveItem::Direction ActiveItem::getDirection () const
{
    return _direction;
}

void ActiveItem::setDirection (ActiveItem::Direction direction)
{
    if (_direction != direction) {
        _direction = direction;

        if (_uc)
            _uc->msgDirectionChanged (this);

        emit directionChanged (_direction);
    }
}

int ActiveItem::getHeight () const
{
    return Globals::defaultEntityHeight;
}

int ActiveItem::getWidth () const
{
    return Globals::defaultEntityWidth;
}

Entity* ActiveItem::createObject (QObject* parent, char type, QPoint pos)
{
    ActiveItem* z = new ActiveItem(parent, 0, (ActiveItemType)type);

    if (z != 0) {
        z->setX (pos.x ());
        z->setY (pos.y ());
    }

    return z;
}

void ActiveItem::setUnitController (UnitController* u)
{
    _uc = u;
}

int ActiveItem::getDistance () const
{
    return _distance;
}

void ActiveItem::setDistance (int d)
{
    if (_distance != d) {
        if (d < 0)
            _distance = 0;
        else
            _distance = d;

        emit distanceChanged (d);
    }
}

void ActiveItem::tick ()
{
    if (_uc)
        _uc->msgTick (this);
}
