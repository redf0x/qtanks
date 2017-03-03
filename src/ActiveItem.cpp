#include "ActiveItem.h"

ActiveItem::ActiveItem(QObject* parent, int rotation, ActiveItem::ActiveItemType type, UnitController* u) :
    Entity(parent), _type(type), _uc(u), _distance(0), _frozen(false), texOverriden(false), _fired(false),
    _alive(true)
{
    setRotation (rotation);
    setDirection (Direction::SOUTH);
}

void ActiveItem::overrideTexture (bool v)
{
    texOverriden = v;
}

QString ActiveItem::getTextureSource () const
{
    QString name("");

    if (texOverriden)
        return Entity::getTextureSource ();

    switch (_type) {
        case BASE:
            name = "eagle";
            break;

        case PLAYER:
            name = "tank_player";
            break;

        case NPC:
            name = "tank_basic";
            break;

        case PROJECTILE:
            name = "projectile";
            break;

        default:
            break;
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

    z->setArmor (1);

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

        emit distanceChanged(d);
    }
}

void ActiveItem::tick ()
{
    if (_uc)
        _uc->msgTick (this);
}

bool ActiveItem::getFrozen () const
{
    return _frozen;
}

void ActiveItem::setFrozen (bool frozen)
{
    set_if_changed(frozen)
}

void ActiveItem::setFired (bool f)
{
    if (f != _fired) {
        _fired = f;

        if (_uc && _fired)
            _uc->msgFired (this);

        emit firedChanged(_fired);
    }
}

bool ActiveItem::getFired () const
{
    return _fired;
}

void ActiveItem::setAlive (bool alive)
{
    set_if_changed(alive)
}

bool ActiveItem::isAlive () const
{
    return _alive;
}
