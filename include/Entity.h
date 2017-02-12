#ifndef ENTITY_H
#define ENTITY_H

#include "common.h"

class Entity : public QObject {
    Q_OBJECT

    Q_PROPERTY(int width READ getWidth CONSTANT)
    Q_PROPERTY(int height READ getHeight CONSTANT)
    Q_PROPERTY(int x READ getX WRITE setX NOTIFY xChanged)
    Q_PROPERTY(int y READ getY WRITE setY NOTIFY yChanged)
    Q_PROPERTY(int orientation READ getOrientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(QString objectId READ getObjectId CONSTANT)
    Q_PROPERTY(QString texture READ getTextureSource CONSTANT)

public:
    explicit Entity(QObject* parent = 0) : QObject(parent), _orientation(Globals::SOUTH), _visible(true) { }

    virtual QString getObjectId () const;
    virtual int getX () const;
    virtual int getY () const;
    virtual int getWidth () const;
    virtual int getHeight () const;
    virtual QString getTextureSource () const;
    virtual int getOrientation () const;

    virtual void setObjectId (QString id);
    virtual void setX (int);
    virtual void setY (int);
    virtual void setTextureSource (const QString&);
    virtual void setOrientation (int);

signals:
    void xChanged (int);
    void yChanged (int);
    void orientationChanged (int);

private:
    QString _id;
    QString _texture;
    int _x;
    int _y;
    int _orientation;
    bool _visible;
};

#endif // ENTITY_H
