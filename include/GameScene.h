#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "common.h"
#include "Block.h"
#include "ActiveItem.h"
#include "UnitController.h"
#include "NpcController.h"
#include "KeyAssignments.h"
#include "RTree.h"

class GameScene : public QObject {
    Q_OBJECT

    static const int columns = 26;
    static const int rows = 26;

    Q_PROPERTY(int columns READ getColumns CONSTANT)
    Q_PROPERTY(int rows READ getRows CONSTANT)
    Q_PROPERTY(QQmlListProperty<Block> bmap READ getBmap NOTIFY bmapChanged)
    Q_PROPERTY(QQmlListProperty<ActiveItem> playableItems READ getPlayableItems NOTIFY playableItemsChanged)
    Q_PROPERTY(QQmlListProperty<ActiveItem> npcItems READ getNpcItems NOTIFY npcItemsChanged)

    typedef struct ObjectRTreeBox {
        ObjectRTreeBox() { }
        ObjectRTreeBox(int x, int y, int width, int height) {
            this->min [0] = x; this->max [0] = x + width;
            this->min [1] = y; this->max [1] = y + height;
        }

        int min [2];
        int max [2];
    } ObjectRTreeBox;

    typedef struct ObjectRTreeCtx {
        int nearestObject;
        QPoint target;
        Entity* object;
    } ObjectRTreeCtx;

    friend bool scanArea (Entity*, void*);

public:
    typedef RTree<Entity*, int, 2, float, 512> ObjectRTree;

    explicit GameScene(QObject* parent = 0) : QObject(parent) {
        _keyConfig = new KeyAssignments(/* QString("keys.conf"), */ this);
        _keyConfig->dump ();
        _playerCtl = new UnitController(this);
        _botCtl = new NpcController(this);
    }

    ~GameScene();

    int getColumns () const;
    int getRows () const;
    QQmlListProperty<Block> getBmap ();
    QQmlListProperty<ActiveItem> getPlayableItems ();
    QQmlListProperty<ActiveItem> getNpcItems ();
    void initialize (QString level);
    void reset ();
    void spawnPlayableItem (QPoint, QString = "");
    void spawnNpcItem (QPoint, QString = "");
    KeyAssignments* getControllerConfig () const;
    void buildObjectsRTree ();
    Block* scanDirection (QRect&, ActiveItem::Direction);
    int getBattleFieldWidth () const;
    int getBattleFieldHeight () const;
    void start ();

signals:
    void bmapChanged (QQmlListProperty<Block>);
    void playableItemsChanged (QQmlListProperty<ActiveItem>);
    void npcItemsChanged (QQmlListProperty<ActiveItem>);

private:
    QList<Block*> _bmap;
    QList<ActiveItem*> _playableItems;
    QList<ActiveItem*> _npcItems;
    UnitController* _playerCtl;
    NpcController* _botCtl;
    KeyAssignments* _keyConfig;
    ObjectRTree* _tree;
    QPoint _fieldSize;
    QTimer _timer;
};

#endif // GAMESCENE_H
