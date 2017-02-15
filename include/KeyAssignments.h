#ifndef KEYASSIGNMENTS_H
#define KEYASSIGNMENTS_H

#include "common.h"

#define K_NORTH "key.north"
#define K_SOUTH "key.south"
#define K_EAST  "key.east"
#define K_WEST  "key.west"
#define K_PAUSE "key.pause"
#define K_FIRE  "key.file"

#define keyNorth_default    Qt::Key_Up
#define keySouth_default    Qt::Key_Down
#define keyEast_default     Qt::Key_Right
#define keyWest_default     Qt::Key_Left
#define keyFire_default     Qt::Key_Control
#define keyPause_default    Qt::Key_Alt

class KeyAssignments : public QObject {
    Q_OBJECT

    Q_PROPERTY(Qt::Key keyNorth READ keyNorth WRITE setKeyNorth)
    Q_PROPERTY(Qt::Key keySouth READ keySouth WRITE setKeySouth)
    Q_PROPERTY(Qt::Key keyEast READ keyEast WRITE setKeyEast)
    Q_PROPERTY(Qt::Key keyWest READ keyWest WRITE setKeyWest)
    Q_PROPERTY(Qt::Key keyFire READ keyFire WRITE setKeyFire)
    Q_PROPERTY(Qt::Key keyPause READ keyPause WRITE setKeyPause)

public:
    explicit KeyAssignments(QObject* parent = 0) : QObject(parent), _keyNorth(Qt::Key_Up), _keySouth(Qt::Key_Down),
        _keyEast(Qt::Key_Right), _keyWest(Qt::Key_Left), _keyFire(Qt::Key_Control), _keyPause(Qt::Key_Space),
        _file("") { }
    KeyAssignments(const QString& fileName, QObject* parent = 0);

    void load (const QString& fileName);
    void store (const QString& fileName);
    void store ();
    void setConfigFile (const QString& fileName);
    void dump ();
    void check ();

    Qt::Key keyNorth () const;
    Qt::Key keySouth () const;
    Qt::Key keyEast () const;
    Qt::Key keyWest () const;
    Qt::Key keyFire () const;
    Qt::Key keyPause () const;

    void setKeyNorth (Qt::Key key);
    void setKeySouth (Qt::Key key);
    void setKeyEast (Qt::Key key);
    void setKeyWest (Qt::Key key);
    void setKeyFire (Qt::Key key);
    void setKeyPause (Qt::Key key);

private:
    QString fromKey (Qt::Key code);
    Qt::Key toKey (QString const& str);

    Qt::Key _keyNorth;
    Qt::Key _keySouth;
    Qt::Key _keyEast;
    Qt::Key _keyWest;
    Qt::Key _keyFire;
    Qt::Key _keyPause;
    QString _file;
};

#endif // KEYASSIGNMENTS_H
