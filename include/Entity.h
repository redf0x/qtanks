#ifndef ENTITY_H
#define ENTITY_H

#include "common.h"

class Entity : public QObject {
    Q_OBJECT

public:
    Q_PROPERTY(int width READ getWidth CONSTANT)
    Q_PROPERTY(int height READ getHeight CONSTANT)
    Q_PROPERTY(int x READ getX WRITE setX NOTIFY xChanged)
    Q_PROPERTY(int y READ getY WRITE setY NOTIFY yChanged)
    Q_PROPERTY(int rotation READ getRotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(bool solid READ isSolid CONSTANT)
    Q_PROPERTY(QString objectId READ getObjectId CONSTANT)
    Q_PROPERTY(QString texture READ getTextureSource CONSTANT)

    explicit Entity(QObject* parent = 0) : QObject(parent), _rotation(0), _solid(true) { }

    virtual QString getObjectId () const;
    virtual int getX () const;
    virtual int getY () const;
    virtual int getWidth () const;
    virtual int getHeight () const;
    virtual QString getTextureSource () const;
    virtual int getRotation () const;
    virtual bool isSolid () const;

    virtual void setObjectId (QString id);
    virtual void setX (int);
    virtual void setY (int);
    virtual void setTextureSource (const QString&);
    virtual void setRotation (int);
    virtual void setSolid (bool);

signals:
    void xChanged (int);
    void yChanged (int);
    void rotationChanged (int);

private:
    QString _id;
    QString _texture;
    int _x;
    int _y;
    int _rotation;
    bool _solid;
};

#endif // ENTITY_H
