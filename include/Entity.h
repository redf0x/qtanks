#ifndef ENTITY_H
#define ENTITY_H

#include "common.h"

#define set_if_changed(variable)    \
    if (_ ## variable != variable) { _ ## variable = variable; emit variable ## Changed(_ ## variable); }

#define ATTRIBUTE_GENERIC "generic"
#define ATTRIBUTE_UNKNOWN "unknown"
#define EXPIRY_NEVER    std::numeric_limits<long long>::max()

class Attribute {
public:
    Attribute() : family(ATTRIBUTE_UNKNOWN), type(ATTRIBUTE_GENERIC), value(QVariant::fromValue(0)), expiry(0) { }
    Attribute(QString f, QVariant v, long long exp = EXPIRY_NEVER, QString t = ATTRIBUTE_GENERIC) : family(f), type(t),
        value(v), expiry(exp) { }

    QVariant getValue () const { return value; }
    void setValue (QVariant v) { value = v; }
    void tickOff () { if (expiry - 1) expiry--; }
    bool expired () { return !expiry; }
    void extend (long long nexp) { expiry += nexp; }
    QString getFamily () const { return family; }
    QString getType () const { return type; }

protected:
    QString family, type;
    QVariant value;
    long long expiry;
};

typedef QList<Attribute> AttrList;

class Entity : public QObject {
    Q_OBJECT

public:
    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int rotation READ getRotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(bool solid READ isSolid CONSTANT)
    Q_PROPERTY(bool spawned READ isSpawned WRITE setSpawned NOTIFY spawnedChanged)
    Q_PROPERTY(QString objectId READ getObjectId CONSTANT)
    Q_PROPERTY(QString texture READ getTextureSource WRITE setTextureSource NOTIFY textureChanged)
    Q_PROPERTY(int z READ z CONSTANT)
    Q_PROPERTY(int armor READ getArmor WRITE setArmor NOTIFY armorChanged)

    explicit Entity(QObject* parent = 0) : QObject(parent),  _solid(true), _spawned(true),
        _x(0), _y(0), _width(1), _height(1), _rotation(0), _z(1) { }
    virtual ~Entity() { }

    virtual QString getObjectId () const;
    virtual int x () const;
    virtual int y () const;
    virtual int width () const;
    virtual int height () const;
    virtual QString getTextureSource () const;
    virtual int getRotation () const;
    virtual int z () const;
    virtual bool isSolid () const;
    virtual bool isSpawned () const;
    virtual int getArmor () const;

    virtual void setObjectId (QString id);
    virtual void setX (int);
    virtual void setY (int);
    virtual void setWidth (int);
    virtual void setHeight (int);
    virtual void setTextureSource (const QString&);
    virtual void setRotation (int);
    virtual void setSolid (bool);
    virtual void setSpawned (bool);
    virtual void setArmor (int);

    virtual Entity* create (QObject* parent, char sign, QPoint pos);

    virtual void addAttribute (Attribute attr);
    virtual Attribute& getAttribute (QString family, QString type = ATTRIBUTE_GENERIC);
    virtual void removeAttribute (QString family, QString type = ATTRIBUTE_GENERIC);

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
    void spawnedChanged (bool spawned);

private:
    QString _id;
    QString _texture;
    bool _solid, _spawned;
    int _x, _y, _width, _height;
    int _rotation;
    int _armor;
    QMap<QString, Attribute> attributeSlots;

protected:
    int _z;
};

#endif // ENTITY_H
