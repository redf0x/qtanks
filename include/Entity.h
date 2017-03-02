#ifndef ENTITY_H
#define ENTITY_H

#include "common.h"

#define set_if_changed(variable)    \
    if (_ ## variable != variable) { _ ## variable = variable; emit variable ## Changed(_ ## variable); }

class Entity : public QObject {
    Q_OBJECT

public:
    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int rotation READ getRotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(bool solid READ isSolid CONSTANT)
    Q_PROPERTY(QString objectId READ getObjectId CONSTANT)
    Q_PROPERTY(QString texture READ getTextureSource WRITE setTextureSource NOTIFY textureChanged)
    Q_PROPERTY(int z READ z CONSTANT)
    Q_PROPERTY(int armor READ getArmor WRITE setArmor NOTIFY armorChanged)

    explicit Entity(QObject* parent = 0) : QObject(parent),  _solid(true), _x(0), _y(0), _width(1), _height(1),
        _rotation(0), _z(1) { }

    virtual QString getObjectId () const;
    virtual int x () const;
    virtual int y () const;
    virtual int width () const;
    virtual int height () const;
    virtual QString getTextureSource () const;
    virtual int getRotation () const;
    virtual int z () const;
    virtual bool isSolid () const;
    virtual int getArmor () const;

    virtual void setObjectId (QString id);
    virtual void setX (int);
    virtual void setY (int);
    virtual void setWidth (int);
    virtual void setHeight (int);
    virtual void setTextureSource (const QString&);
    virtual void setRotation (int);
    virtual void setSolid (bool);
    virtual void setArmor (int);
    virtual Entity* create (QObject* parent, char sign, QPoint pos);

protected:
    virtual Entity* createObject (QObject* parent, char sign, QPoint pos);

signals:
    void xChanged (int);
    void yChanged (int);
    void widthChanged (int);
    void heightChanged (int);
    void rotationChanged (int);
    void textureChanged (QString);
    void armorChanged (int);

private:
    QString _id;
    QString _texture;
    bool _solid;
    int _x, _y, _width, _height;
    int _rotation;
    int _armor;

protected:
    int _z;
};

#endif // ENTITY_H
