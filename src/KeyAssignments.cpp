#include "KeyAssignments.h"

class KeyHelper : private QObject {
public:
    static QString fromKey (Qt::Key code) {
        static int idx = staticQtMetaObject.indexOfEnumerator ("Key");
        QString key = staticQtMetaObject.enumerator (idx).valueToKey (code);

        return (key.isEmpty ()) ? key : key.mid (4);
    }
};

KeyAssignments::KeyAssignments(const QString& fileName, QObject* parent) :
    QObject(parent), _file(fileName)
{
    load (fileName);
}

void KeyAssignments::load (const QString& fileName)
{
    QSettings* q = new QSettings(fileName, QSettings::NativeFormat);

    setKeyNorth (toKey (q->value (K_NORTH).toString ()));
    setKeySouth (toKey (q->value (K_SOUTH).toString ()));
    setKeyEast (toKey (q->value (K_EAST).toString ()));
    setKeyWest (toKey (q->value (K_WEST).toString ()));
    setKeyFire (toKey (q->value (K_FIRE).toString ()));
    setKeyPause (toKey (q->value (K_PAUSE).toString ()));

    delete q;
}

void KeyAssignments::dump ()
{
    qDebug() << K_NORTH << fromKey (_keyNorth);
    qDebug() << K_SOUTH << fromKey (_keySouth);
    qDebug() << K_WEST << fromKey (_keyWest);
    qDebug() << K_EAST << fromKey (_keyEast);
    qDebug() << K_FIRE << fromKey (_keyFire);
    qDebug() << K_PAUSE << fromKey (_keyPause);
}

void KeyAssignments::store (const QString& fileName)
{
    QSettings* q = new QSettings(fileName, QSettings::NativeFormat);

    q->setValue (K_NORTH, QVariant(fromKey (_keyNorth)));
    q->setValue (K_SOUTH, QVariant(fromKey (_keySouth)));
    q->setValue (K_WEST, QVariant(fromKey (_keyWest)));
    q->setValue (K_EAST, QVariant(fromKey (_keyEast)));
    q->setValue (K_FIRE, QVariant(fromKey (_keyFire)));
    q->setValue (K_PAUSE, QVariant(fromKey (_keyPause)));

    delete q;
}

void KeyAssignments::store ()
{
    if (!_file.isEmpty ())
        store (_file);
}

void KeyAssignments::setConfigFile (const QString &fileName)
{
    _file = QString(fileName);
}

QString KeyAssignments::fromKey (Qt::Key code)
{
    return KeyHelper::fromKey (code);
}

Qt::Key unconventionalKey (QString const& s)
{
    QMap<QString, Qt::Key> keymap(std::map<QString, Qt::Key> {
        { "Control", Qt::Key_Control },
        { "Alt", Qt::Key_Alt },
        { "AltGr", Qt::Key_AltGr },
        { "Ctrl", Qt::Key_Control },
        { "Shift", Qt::Key_Shift }
    });
    Qt::Key key = Qt::Key_unknown;

    if (keymap.contains(s))
        key = keymap [s];

    return key;
}

Qt::Key KeyAssignments::toKey (QString const& str)
{
    QString s(str);
    QKeySequence seq;
    Qt::Key key = Qt::Key_unknown;

    s.remove (" ");
    seq = QKeySequence(s);

    if (seq.count () != 0)
        switch (static_cast<Qt::Key>(seq [0])) {
            case Qt::Key_unknown:
                key = unconventionalKey (s);
                break;

            default:
                key = static_cast<Qt::Key>(seq [0]);
        }

    return key;
}

Qt::Key KeyAssignments::keyNorth () const
{
    return _keyNorth;
}

Qt::Key KeyAssignments::keySouth () const
{
    return _keySouth;
}

Qt::Key KeyAssignments::keyEast () const
{
    return _keyEast;
}

Qt::Key KeyAssignments::keyWest () const
{
    return _keyWest;
}

Qt::Key KeyAssignments::keyFire () const
{
    return _keyFire;
}

Qt::Key KeyAssignments::keyPause () const
{
    return _keyPause;
}

void KeyAssignments::setKeyNorth (Qt::Key key)
{
    _keyNorth = key;
}

void KeyAssignments::setKeySouth (Qt::Key key)
{
    _keySouth = key;
}

void KeyAssignments::setKeyEast (Qt::Key key)
{
    _keyEast = key;
}

void KeyAssignments::setKeyWest (Qt::Key key)
{
    _keyWest = key;
}

void KeyAssignments::setKeyFire (Qt::Key key)
{
    _keyFire = key;
}

void KeyAssignments::setKeyPause (Qt::Key key)
{
    _keyPause = key;
}

void KeyAssignments::check ()
{
    if (_keyNorth == Qt::Key_unknown)
        setKeyNorth (keyNorth_default);

    if (_keySouth == Qt::Key_unknown)
        setKeySouth (keySouth_default);

    if (_keyEast == Qt::Key_unknown)
        setKeyEast (keyEast_default);

    if (_keyWest == Qt::Key_unknown)
        setKeyWest (keyWest_default);

    if (_keyFire == Qt::Key_unknown)
        setKeyFire (keyFire_default);

    if (_keyPause == Qt::Key_unknown)
        setKeyPause (keyPause_default);
}
