#ifndef ACTIVEITEM_H
#define ACTIVEITEM_H

#include "common.h"
#include "Entity.h"
#include "UnitController.h"

class ActiveItem : public Entity {
    Q_OBJECT

public:
    enum ActiveItemType { NONE, BASE, PLAYER, ENEMY, PROJECTILE };
    enum Direction { NORTH, EAST, SOUTH, WEST };

    Q_ENUMS(ActiveItemType Direction)
    Q_PROPERTY(Direction direction READ getDirection WRITE setDirection)
    Q_PROPERTY(int distance READ getDistance WRITE setDistance NOTIFY distanceChanged)

    explicit ActiveItem(QObject* parent = 0, int rotation = 0, ActiveItemType type = NONE, UnitController* u = nullptr);

    virtual QString getTextureSource () const;
    Direction getDirection () const;
    virtual int getWidth () const;
    virtual int getHeight () const;
    int getDistance () const;

    void setDirection (Direction);
    void setUnitController (UnitController*);
    void setDistance (int);

protected:
    virtual Entity* createObject (QObject *parent, char type, QPoint pos);

signals:
    void distanceChanged (int);

private:
    ActiveItemType _type;
    Direction _direction;
    UnitController* _uc;
    bool _frozen;
    int _distance;
};

#endif // ACTIVEITEM_H
