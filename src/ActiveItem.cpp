#include "ActiveItem.h"

ActiveItem::ActiveItem(QObject* parent, int rotation, ActiveItem::ActiveItemType type) : Entity(parent), _type(type), _frozen(false)
{
    setRotation (rotation);
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

        case ENEMY:
            name = " tank_basic";
    }

    return "qrc:/image/ui/res/" + name + ".png";
}

ActiveItem::Direction ActiveItem::getDirection () const
{
    return _direction;
}

void ActiveItem::setDirection (ActiveItem::Direction direction)
{
    _direction = direction;
}

int ActiveItem::getHeight () const
{
    return ActiveItem::objectHeight;
}

int ActiveItem::getWidth () const
{
    return ActiveItem::objectWidth;
}

ActiveItem* ActiveItem::create (QObject* parent, ActiveItem::ActiveItemType type, QPoint pos)
{
    ActiveItem* z = new ActiveItem(parent, 0, type);

    if (z != 0) {
        z->setX (pos.x ());
        z->setY (pos.y ());
    }

    return z;
}
