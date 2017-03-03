#include "Entity.h"

QString Entity::getTextureSource () const
{
    return _texture;
}

void Entity::setTextureSource (const QString& texture)
{
    set_if_changed(texture)
}

int Entity::x () const
{
    return _x;
}

int Entity::y () const
{
    return _y;
}

int Entity::width () const
{
    return _width;
}

int Entity::height () const
{
    return _height;
}

int Entity::getRotation () const
{
    return _rotation;
}

void Entity::setHeight (int height)
{
    set_if_changed(height)
}

void Entity::setWidth (int width)
{
    set_if_changed(width)
}

QString Entity::getObjectId () const
{
    return _id;
}

void Entity::setObjectId (QString id)
{
    _id = id;
}

void Entity::setY (int y)
{
    set_if_changed(y)
}

void Entity::setX (int x)
{
    set_if_changed(x)
}

void Entity::setRotation (int rotation)
{
    set_if_changed(rotation)
}

bool Entity::isSolid () const
{
    return _solid;
}

void Entity::setSolid (bool solid)
{
    _solid = solid;
}

Entity* Entity::create (QObject* parent, char sign, QPoint pos)
{
    return createObject (parent, sign, pos);
}

/* stub */
Entity* Entity::createObject (QObject* parent, char sign, QPoint pos)
{
    Q_UNUSED(parent)
    Q_UNUSED(sign)
    Q_UNUSED(pos)
    return nullptr;
}

int Entity::z () const
{
    return _z;
}

void Entity::setArmor (int armor)
{
    set_if_changed(armor)
}

int Entity::getArmor () const
{
    return _armor;
}

void Entity::setSpawned (bool spawned)
{
    set_if_changed(spawned)
}

bool Entity::isSpawned () const
{
    return _spawned;
}

void Entity::addAttribute (Attribute attr)
{
    QString attrFCN(attr.getFamily () + "." + attr.getType ());

    if (attributeSlots.contains (attrFCN))
        return;
    else
        attributeSlots.insert (attrFCN, attr);
}

Attribute& Entity::getAttribute (QString family, QString type)
{
    return attributeSlots [QString(family + "." + type)];
}

void Entity::removeAttribute (QString family, QString type)
{
    attributeSlots.remove (QString(family + "." + type));
}
