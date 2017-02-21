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

void Entity::linkObject (QVariant obj)
{
    QObject* object = qvariant_cast<QObject*>(obj);
    QQuickItem* q = qobject_cast<QQuickItem*>(object);

    if (q != NULL)
        _linkedObject = q;
}

QQuickItem* Entity::getLinkedObject () const
{
    return _linkedObject;

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

int Entity::getZLevel () const
{
    return _z;
}
