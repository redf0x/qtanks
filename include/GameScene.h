#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "common.h"
#include "Block.h"
#include "ActiveItem.h"

class GameScene : public QObject {
    Q_OBJECT

    static const int columns = 26;
    static const int rows = 26;

    Q_PROPERTY(int columns READ getColumns CONSTANT)
    Q_PROPERTY(int rows READ getRows CONSTANT)
    Q_PROPERTY(QQmlListProperty<Block> bmap READ getBmap NOTIFY bmapChanged)
    Q_PROPERTY(QQmlListProperty<ActiveItem> playableItems READ getPlayableItems NOTIFY playableItemsChanged)

public:
    explicit GameScene(QObject* parent = 0) : QObject(parent) { }
    ~GameScene();

    int getColumns () const;
    int getRows () const;
    QQmlListProperty<Block> getBmap ();
    QQmlListProperty<ActiveItem> getPlayableItems ();
    void initialize (QString level);
    void reset ();
    void spawnPlayableItem (QPoint);

signals:
    void bmapChanged (QQmlListProperty<Block>);
    void playableItemsChanged (QQmlListProperty<ActiveItem>);

private:
    QList<Block*> _bmap;
    QList<ActiveItem*> _playableItems;
};

#endif // GAMESCENE_H
