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
    Q_PROPERTY(bool fired READ getFired WRITE setFired NOTIFY firedChanged)
    Q_PROPERTY(bool alive READ isAlive WRITE setAlive NOTIFY aliveChanged)

    explicit ActiveItem(QObject* parent = nullptr, int rotation = 0,
                        ActiveItemType type = NONE, UnitController* u = nullptr);

    virtual QString getTextureSource () const;
    Direction getDirection () const;
    virtual int getWidth () const;
    virtual int getHeight () const;
    int getDistance () const;
    bool getFrozen () const;
    bool getFired () const;
    bool isAlive () const;

    void setDirection (Direction);
    void setUnitController (UnitController*);
    void setDistance (int);
    void overrideTexture (bool);
    void setFrozen (bool);
    void setFired (bool);
    void setAlive (bool);

protected:
    virtual Entity* createObject (QObject *parent, char type, QPoint pos);

signals:
    void distanceChanged (int distance);
    void directionChanged (Direction direction);
    void frozenChanged (bool frozen);
    void firedChanged (bool fired);
    void aliveChanged (bool alive);

public slots:
    void tick ();

private:
    ActiveItemType _type;
    Direction _direction;
    UnitController* _uc;
    int _distance;
    bool _frozen, texOverriden, _fired, _alive;
};

#endif // ACTIVEITEM_H
