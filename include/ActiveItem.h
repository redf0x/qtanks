#ifndef ACTIVEITEM_H
#define ACTIVEITEM_H

#include "common.h"
#include "Entity.h"
#include "UnitController.h"

class ActiveItem : public Entity {
    Q_OBJECT

public:
    enum ActiveItemType { NONE, BASE, PLAYER, NPC, PROJECTILE };
    enum Direction { NORTH, EAST, SOUTH, WEST };

    Q_ENUMS(ActiveItemType Direction)
    Q_PROPERTY(Direction direction READ getDirection WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(int distance READ getDistance WRITE setDistance NOTIFY distanceChanged)
    Q_PROPERTY(bool frozen READ getFrozen WRITE setFrozen NOTIFY frozenChanged)

    explicit ActiveItem(QObject* parent = 0, int rotation = 0, ActiveItemType type = NONE, UnitController* u = nullptr);

    virtual QString getTextureSource () const;
    Direction getDirection () const;
    virtual int getWidth () const;
    virtual int getHeight () const;
    int getDistance () const;
    bool getFrozen () const;

    void setDirection (Direction);
    void setUnitController (UnitController*);
    void setDistance (int);
    void overrideTexture (bool);
    void setFrozen (bool);

protected:
    virtual Entity* createObject (QObject *parent, char type, QPoint pos);

signals:
    void distanceChanged (int);
    void directionChanged (Direction);
    void frozenChanged (bool);

public slots:
    void tick ();

private:
    ActiveItemType _type;
    Direction _direction;
    UnitController* _uc;
    bool _frozen;
    int _distance;
    bool texOverriden;
};

#endif // ACTIVEITEM_H
