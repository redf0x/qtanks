#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "common.h"
#include "Block.h"
#include "ActiveItem.h"
#include "UnitController.h"
#include "NpcController.h"
#include "ProjectileController.h"
#include "KeyAssignments.h"
#include "RTree.h"
#include "WorkQueue.h"

class GameScene : public QObject {
    Q_OBJECT

    static const int columns = 26;
    static const int rows = 26;

    Q_PROPERTY(int columns READ getColumns CONSTANT)
    Q_PROPERTY(int rows READ getRows CONSTANT)
    Q_PROPERTY(bool frozen READ getFrozen WRITE setFrozen NOTIFY frozenChanged)

    Q_PROPERTY(int enemyCounter MEMBER _enemyCounter NOTIFY enemyCounterChanged)
    Q_PROPERTY(int stage MEMBER _stage NOTIFY stageChanged)
    Q_PROPERTY(int playerTanks MEMBER _playerCounter NOTIFY playerCounterChanged)

    Q_PROPERTY(QQmlListProperty<Block> bmap READ getBmap NOTIFY bmapChanged)
    Q_PROPERTY(QQmlListProperty<ActiveItem> playableItems READ getPlayableItems NOTIFY playableItemsChanged)
    Q_PROPERTY(QQmlListProperty<ActiveItem> npcItems READ getNpcItems NOTIFY npcItemsChanged)
    Q_PROPERTY(QQmlListProperty<ActiveItem> projectiles READ getProjectiles NOTIFY projectilesChanged)

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

    explicit GameScene(QObject* parent = 0) : QObject(parent), _frozen(true), _stage(1), _playerCounter(1) {
        _keyConfig = new KeyAssignments(/* QString("keys.conf"), */ this);
        _keyConfig->dump ();
        _playerCtl = new UnitController(this);
        _botCtl = new NpcController(this);
        _projectileCtl = new ProjectileController(this);
        wq = new WorkQueue(this);
    }

    ~GameScene();

    int getColumns () const;
    int getRows () const;
    bool getFrozen () const;
    QQmlListProperty<Block> getBmap ();
    QQmlListProperty<ActiveItem> getPlayableItems ();
    QQmlListProperty<ActiveItem> getNpcItems ();
    QQmlListProperty<ActiveItem> getProjectiles ();

    void initialize (QString level);
    void reset ();
    void spawnPlayableItem (QPoint, QString = "");
    void spawnNpcItem (QPoint, QString = "");
    void fireProjectile (ActiveItem*);
    void buildObjectsRTree ();
    Block* scanDirection (QRect&, ActiveItem::Direction);
    QList<ActiveItem*> checkImmediateCollisions (ActiveItem*);
    QList<Entity*> checkImmediateCollisions (ActiveItem*, QList<Block*>&);

    KeyAssignments* getControllerConfig () const;
    int getBattleFieldWidth () const;
    int getBattleFieldHeight () const;
    int getBlocksCount () const;
    void start ();
    void setFrozen (bool);
    void removeProjectile (ActiveItem*);
    void removeBlock (Block*);
    void respawn (ActiveItem* a);
    WorkQueue* getwq ();
    void finalize ();

signals:
    void bmapChanged (QQmlListProperty<Block>);
    void playableItemsChanged (QQmlListProperty<ActiveItem>);
    void npcItemsChanged (QQmlListProperty<ActiveItem>);
    void projectilesChanged (QQmlListProperty<ActiveItem>);
    void frozenChanged (bool);
    void enemyCounterChanged (int);
    void stageChanged (int);
    void playerCounterChanged (int);
    void winCondition (int);

private:
    QList<ActiveItem*> getIntersectionsList (ActiveItem*, QList<ActiveItem*> &);
    void freeze (QList<ActiveItem*> &);
    void thaw (QList<ActiveItem*>& l);
    void setFrozenState (QList<ActiveItem*>& l, bool s);
    void *stow_away_npcs ();
    void restore_npcs (void*);

    QList<Block*> _bmap;
    QList<ActiveItem*> _playableItems;
    QList<ActiveItem*> _npcItems;
    QList<ActiveItem*> _projectiles;
    UnitController* _playerCtl;
    NpcController* _botCtl;
    ProjectileController* _projectileCtl;
    KeyAssignments* _keyConfig;
    ObjectRTree* _tree;
    QPoint _fieldSize;
    QTimer _timer;
    bool _frozen;
    int _enemyCounter, _stage, _playerCounter;
    QMap<ActiveItem*, QPoint> _spawners;
    WorkQueue* wq;
};

#endif // GAMESCENE_H
