#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "common.h"
#include "Entity.h"
#include "Block.h"
#include "ActiveItem.h"
#include "UnitController.h"
#include "PlayerController.h"
#include "NpcController.h"
#include "ProjectileController.h"
#include "KeyAssignments.h"
#include "RTree.h"
#include "Globals.h"

typedef QList<ActiveItem*> ActorList;
typedef QList<Block*> BlockList;
typedef QList<Entity*> ObjectList;

#define for_all(type, list, callback) for (QList<type>::iterator ptr = list.begin (); ptr != list.end (); ptr++) callback(*ptr)
#define for_all_actors(list, callback) for_all(ActiveItem*, list, callback)
#define for_all_blocks(list, callback) for_all(Block*, list, callback)
#define for_all_objects(list, callback) for_all(Entity*, list, callback)

class GameScene : public QObject {
    Q_OBJECT

    /* battle field state flags and properties */
    Q_PROPERTY(bool frozen READ getFrozen WRITE setFrozen NOTIFY frozenChanged)

    /* counters/bindings to HUD */
    Q_PROPERTY(int enemyCounter MEMBER _enemyCounter NOTIFY enemyCounterChanged)
    Q_PROPERTY(int stage MEMBER _stage NOTIFY stageChanged)
    Q_PROPERTY(int playerTanks MEMBER _playerCounter NOTIFY playerCounterChanged)

    /* game object lists */
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

    typedef struct Cell {
        int width;
        int height;
    } Cell;

    friend bool scanArea (Entity*, void*);

public:
    typedef RTree<Entity*, int, 2, float, 512> ObjectRTree;

    explicit GameScene(QQmlApplicationEngine* engine, QObject* parent = 0) : QObject(parent), _frozen(true),
        _enemyCounter(Globals::enemyCount), _stage(1), _playerCounter(1) {
        QQmlContext* ctx = engine->rootContext ();

        _keyConfig = new KeyAssignments(/* QString("keys.conf"), */ this);
        _keyConfig->dump ();
        _playerCtl = new PlayerController(this);
        _botCtl = new NpcController(this);
        _projectileCtl = new ProjectileController(this);
        ctx->setContextProperty ("battleField", this);
        ctx->setContextProperty ("controller", _keyConfig);
    }

    ~GameScene();

    bool getFrozen () const;

    QQmlListProperty<Block> getBmap ();
    QQmlListProperty<ActiveItem> getPlayableItems ();
    QQmlListProperty<ActiveItem> getNpcItems ();
    QQmlListProperty<ActiveItem> getProjectiles ();

    void initialize (QString level);
    void reset ();
    ActiveItem* spawnItem (QPoint, ActiveItem::ActiveItemType, ActorList&, QString, UnitController*, QString, AttrList*);
    void spawnPlayerItem (QPoint, QString = "");
    void spawnNpcItem (QPoint, QString = "");
    void spawnBase (QPoint);
    void fireProjectile (ActiveItem*);
    void buildObjectsRTree ();
    Block* scanDirection (QRect&, ActiveItem::Direction);
    ActorList checkImmediateCollisions(ActiveItem*);
    ObjectList checkImmediateCollisions (ActiveItem*, BlockList&);

    KeyAssignments* getControllerConfig () const;
    int getBattleFieldWidth () const;
    int getBattleFieldHeight () const;
    int getBlocksCount () const;
    void start ();
    void setFrozen (bool);
    void removeProjectile (ActiveItem*);
    void removeBlock (Block*);
    void respawn (ActiveItem* a);
    void finalize ();
    enum MappingType { FROM_LOGICAL, FROM_PHYSICAL };
    QRect mapRect (QRect&, MappingType);

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
    void loseCondition (void);

public slots:
    void heightChanged (int newheight);
    void widthChanged (int newwidth);
    void cleanup ();

private:
    QList<ActiveItem*> getIntersectionsList (ActiveItem*, ActorList &);
    void freeze (ActorList&);
    void thaw (ActorList&);
    void setFrozenState (ActorList&, bool);
    void fixupObject (Block*);
    bool checkLoseCondition (ActiveItem*);

    BlockList _bmap;
    ActorList _playableItems;
    ActorList _npcItems;
    ActorList _projectiles;

    UnitController* _playerCtl;
    UnitController* _botCtl;
    UnitController* _projectileCtl;

    KeyAssignments* _keyConfig;
    ObjectRTree* _tree;
    QPoint _fieldSize;
    Cell _cell;
    QTimer _timer;
    bool _frozen;
    int _enemyCounter, _stage, _playerCounter;
    QMap<QString, QPoint> _spawners;
};

#endif // GAMESCENE_H
