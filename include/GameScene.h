#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "common.h"
#include "Block.h"
#include "ActiveItem.h"
#include "UnitController.h"
#include "KeyAssignments.h"

class GameScene : public QObject {
    Q_OBJECT

    static const int columns = 26;
    static const int rows = 26;

    Q_PROPERTY(int columns READ getColumns CONSTANT)
    Q_PROPERTY(int rows READ getRows CONSTANT)
    Q_PROPERTY(QQmlListProperty<Block> bmap READ getBmap NOTIFY bmapChanged)
    Q_PROPERTY(QQmlListProperty<ActiveItem> playableItems READ getPlayableItems NOTIFY playableItemsChanged)

public:
    explicit GameScene(QObject* parent = 0) : QObject(parent) {
        _keyConfig = new KeyAssignments(/* QString("keys.conf"), */ this);
        _keyConfig->dump ();
        _playerCtl = new UnitController(this);
    }

    ~GameScene();

    int getColumns () const;
    int getRows () const;
    QQmlListProperty<Block> getBmap ();
    QQmlListProperty<ActiveItem> getPlayableItems ();
    void initialize (QString level);
    void reset ();
    void spawnPlayableItem (QPoint);
    KeyAssignments* getControllerConfig () const;

signals:
    void bmapChanged (QQmlListProperty<Block>);
    void playableItemsChanged (QQmlListProperty<ActiveItem>);

private:
    QList<Block*> _bmap;
    QList<ActiveItem*> _playableItems;
    UnitController* _playerCtl;
    KeyAssignments* _keyConfig;
};

#endif // GAMESCENE_H
