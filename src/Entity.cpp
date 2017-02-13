#include "Entity.h"

QString Entity::getTextureSource () const
{
    return _texture;
}

void Entity::setTextureSource (const QString& src)
{
    _texture = src;
}

int Entity::getX () const
{
    return _x;
}

int Entity::getY () const
{
    return _y;
}

int Entity::getWidth() const
{
    return Globals::defaultEntityWidth;
}

int Entity::getHeight () const
{
    return Globals::defaultEntityHeight;
}

int Entity::getRotation () const
{
    return _rotation;
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
    _y = y;
}

void Entity::setX (int x)
{
    _x = x;
}

void Entity::setRotation (int rotation)
{
    _rotation = rotation;
}

bool Entity::isSolid () const
{
    return _solid;
}

void Entity::setSolid (bool solid)
{
    _solid = solid;
}
