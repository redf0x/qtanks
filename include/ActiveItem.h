#ifndef ACTIVEITEM_H
#define ACTIVEITEM_H

#include "common.h"
#include "Entity.h"

class ActiveItem : public Entity {
    Q_OBJECT

public:
    static const int objectWidth = 32;
    static const int objectHeight = 32;

    enum ActiveItemType { NONE, BASE, PLAYER, ENEMY, PROJECTILE };
    enum Direction { NORTH, EAST, SOUTH, WEST };

    Q_ENUMS(ActiveItemType Direction)
    Q_PROPERTY(Direction direction READ getDirection WRITE setDirection)

    explicit ActiveItem(QObject* parent = 0, int rotation = 0, ActiveItemType type = NONE);

    static ActiveItem* create (QObject* parent, ActiveItemType type, QPoint pos);

    virtual QString getTextureSource () const;
    Direction getDirection () const;
    virtual int getWidth () const;
    virtual int getHeight () const;

    void setDirection (Direction);

private:
    ActiveItemType _type;
    Direction _direction;
    bool _frozen;
};

#endif // ACTIVEITEM_H
