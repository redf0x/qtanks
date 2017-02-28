#include <cmath>

#include "GameScene.h"
#include "MapBuilder.h"
#include "Utility.h"

GameScene::~GameScene()
{
    qDeleteAll (_bmap);
    _bmap.clear ();
    qDeleteAll (_playableItems);
    _playableItems.clear ();
    qDeleteAll (_npcItems);
    _npcItems.clear ();
    qDeleteAll (_projectiles);
    _projectiles.clear ();
    delete _tree;
}

int GameScene::getColumns () const
{
    return columns;
}

int GameScene::getRows () const
{
    return rows;
}

QQmlListProperty<Block> GameScene::getBmap ()
{
    return QQmlListProperty<Block>(this, _bmap);
}

QQmlListProperty<ActiveItem> GameScene::getPlayableItems ()
{
    return QQmlListProperty<ActiveItem>(this, _playableItems);
}

QQmlListProperty<ActiveItem> GameScene::getNpcItems ()
{
    return QQmlListProperty<ActiveItem>(this, _npcItems);
}

QQmlListProperty<ActiveItem> GameScene::getProjectiles ()
{
    return QQmlListProperty<ActiveItem>(this, _projectiles);
}

void GameScene::spawnPlayableItem (QPoint pos, QString texOverride)
{
    ActiveItem s, *p;

    p = dynamic_cast<ActiveItem*>(s.create (this, ActiveItem::ActiveItemType::PLAYER, pos));
    p->setObjectId(QString("player"));
    p->setObjectName (p->getObjectId ());
    p->setUnitController (_playerCtl);
    QObject::connect (&_timer, SIGNAL(timeout()), p, SLOT(tick()));

    if (!texOverride.isEmpty ()) {
        p->overrideTexture (true);
        p->setTextureSource (texOverride);
    }

    _playableItems << p;
}

void GameScene::spawnNpcItem (QPoint pos, QString texOverride)
{
    ActiveItem s, *p;

    p = dynamic_cast<ActiveItem*>(s.create (this, ActiveItem::ActiveItemType::NPC, pos));
    p->setObjectId(QString("npc@%1%2").arg (pos.x ()).arg (pos.y ()));
    p->setObjectName (p->getObjectId ());
    QObject::connect (&_timer, SIGNAL(timeout()), p, SLOT(tick()));
    p->setUnitController (_botCtl);

    if (!texOverride.isEmpty ()) {
        p->overrideTexture (true);
        p->setTextureSource (texOverride);
    }

    _npcItems << p;
}

void GameScene::reset ()
{
    QList<Block*> tmp0(_bmap);

    _bmap.clear ();
    qDeleteAll (tmp0);

    QList<ActiveItem*> tmp1(_playableItems);

    _playableItems.clear ();
    qDeleteAll (tmp1);

    QList<ActiveItem*> tmp2(_projectiles);

    _projectiles.clear ();
    qDeleteAll (tmp2);

    _spawners.clear ();
    _enemyCounter = Globals::enemyCount;
}

void GameScene::initialize (QString level)
{
    MapBuilder map(this, level, this);

    reset ();
    _bmap = map.spawnObjects (this);
    _timer.setInterval (100);
}

KeyAssignments* GameScene::getControllerConfig () const
{
    return _keyConfig;
}

void GameScene::buildObjectsRTree ()
{
    QList<Block*>::iterator b;

    _tree = new ObjectRTree();

    if (!_bmap.isEmpty ()) {
        for (b = _bmap.begin (); b != _bmap.end (); b++) {
            QQuickItem* l = (*b)->getLinkedObject ();
            ObjectRTreeBox item(l->x (), l->y (), l->width (), l->height ());

            _tree->Insert (item.min, item.max, *b);
        }

        _fieldSize = QPoint(_bmap.first ()->getLinkedObject ()->parentItem ()->width (),
                            _bmap.first ()->getLinkedObject ()->parentItem ()->height ());
    }
}

int GameScene::getBattleFieldWidth () const
{
    return _fieldSize.x ();
}

int GameScene::getBattleFieldHeight () const
{
    return _fieldSize.y ();
}

bool scanArea (Entity* e, void* arg)
{
    GameScene::ObjectRTreeCtx* ctx = (GameScene::ObjectRTreeCtx*)arg;
    QQuickItem* lo = e->getLinkedObject ();
    QRect r(lo->x (), lo->y (), lo->width (), lo->height ());
    QPoint p = r.center ();
    int distance = 0;

    if (!e->isSolid ())
        return true;

    distance = sqrt (pow (abs (p.x () - ctx->target.x ()), 2) + pow (abs (p.y () - ctx->target.y ()), 2));

    if (distance < ctx->nearestObject) {
        ctx->nearestObject = distance;
        ctx->object = e;
    }

    return true;
}

Block* GameScene::scanDirection (QRect& target, ActiveItem::Direction direction)
{
    QRect scanZone(target);
    ObjectRTreeBox box;
    ObjectRTreeCtx ctx;

    switch (direction) {
        case ActiveItem::Direction::NORTH:
            scanZone.adjust (3, -scanZone.y (), -3, -scanZone.height ());
            break;

        case ActiveItem::Direction::SOUTH:
            scanZone.adjust (3, scanZone.height (), -3, getBattleFieldHeight () - (scanZone.y () + scanZone.height ()));
            break;

        case ActiveItem::Direction::EAST:
            scanZone.adjust (scanZone.height (), 3, getBattleFieldWidth () - (scanZone.x () + scanZone.width ()), -3);
            break;

        case ActiveItem::Direction::WEST:
            scanZone.adjust (-scanZone.x (), 3, -scanZone.width (), -3);
    }

    box = ObjectRTreeBox(scanZone.x (), scanZone.y (), scanZone.width (), scanZone.height ());
    ctx.nearestObject = std::numeric_limits<int>::max ();
    ctx.target = target.center ();
    ctx.object = nullptr;
    _tree->Search (box.min, box.max, scanArea, &ctx);

    return qobject_cast<Block*>(ctx.object);
}

void GameScene::start ()
{
    QQuickItem* o;

    for (QList<ActiveItem*>::iterator i = _npcItems.begin ();
         i != _npcItems.end (); i++) {
        o = (*i)->getLinkedObject ();
        _spawners.insert ((*i), QPoint(o->x (), o->y ()));
    }

    for (QList<ActiveItem*>::iterator i = _playableItems.begin ();
         i != _playableItems.end (); i++) {
        o = (*i)->getLinkedObject ();
        _spawners.insert ((*i), QPoint(o->x (), o->y ()));
    }

    _frozen = false;
    _timer.start ();

    qDebug() << "bounding rect" << _playableItems.first ()->getLinkedObject()->boundingRect();
    qDebug() << "visible" << _playableItems.first ()->getLinkedObject()->isVisible();
    qDebug() << "alive" << _playableItems.first ()->isAlive();
    qDebug() << "spawned" << _playableItems.first ()->isSpawned();
}

bool GameScene::getFrozen () const
{
    return _frozen;
}

void GameScene::freeze (QList<ActiveItem*>& l)
{
    setFrozenState (l, true);
}

void GameScene::thaw (QList<ActiveItem*>& l)
{
    setFrozenState (l, false);
}

void GameScene::setFrozenState (QList<ActiveItem*>& l, bool s)
{
    QList<ActiveItem*>::iterator i;

    for (i = l.begin (); i != l.end (); i++)
        (*i)->setFrozen (s);
}

void GameScene::setFrozen (bool p)
{
    if (p != _frozen) {
        _frozen = p;

        if (_frozen) {
            _timer.stop ();
            freeze (_playableItems);
            freeze (_npcItems);
            freeze (_projectiles);
        } else {
            _timer.start ();
            thaw (_playableItems);
            thaw (_npcItems);
            thaw (_projectiles);
        }

        emit frozenChanged(_frozen);
    }
}

QList<ActiveItem*> GameScene::getIntersectionsList (ActiveItem* a, QList<ActiveItem*>& list)
{
    QList<ActiveItem*> rlist;
    QQuickItem* q = a->getLinkedObject ();
    QRect r1(q->x (), q->y (), q->width (), q->height ()), r2;

    switch (a->getDirection ()) {
        case ActiveItem::NORTH:
            r1.translate (0, -q->height () / 2);
            break;

        case ActiveItem::SOUTH:
            r1.translate (0, q->height () / 2);
            break;

        case ActiveItem::EAST:
            r1.translate (q->width () / 2, 0);
            break;

        case ActiveItem::WEST:
            r1.translate (-q->width () / 2, 0);
    }

    for (QList<ActiveItem*>::iterator i = list.begin (); i != list.end (); i++)
        if (*i != a) {
            q = (*i)->getLinkedObject ();
            r2 = QRect(q->x (), q->y (), q->width (), q->height ());

            if (r1.intersects (r2))
                rlist << (*i);
        }

    return rlist;
}

QList<ActiveItem*> GameScene::checkImmediateCollisions (ActiveItem* a)
{
    QList<ActiveItem*> collisions;

    collisions += getIntersectionsList (a, _playableItems);
    collisions += getIntersectionsList (a, _npcItems);

    return collisions;
}

QList<Entity*> GameScene::checkImmediateCollisions (ActiveItem* a, QList<Block*>& list)
{
    QList<ActiveItem*> c;
    QList<Entity*> collisions;

    c = checkImmediateCollisions (a);

    if (!list.empty ())
        for (QList<Block*>::iterator i = list.begin (); i != list.end (); i++) {
            QQuickItem* q = a->getLinkedObject ();
            QRect r1(q->x (), q->y (), q->width (), q->height ()), r2;

            q = (*i)->getLinkedObject ();
            r2 = QRect(q->x (), q->y (), q->width (), q->height ());

            if (r1.intersects (r2))
                collisions << dynamic_cast<Entity*>(*i);
        }

    if (!c.empty ())
        for (QList<ActiveItem*>::iterator i = c.begin (); i != c.end (); i++)
            collisions << dynamic_cast<Entity*>(*i);

    return collisions;
}

void GameScene::fireProjectile (ActiveItem * a)
{
    ActiveItem s, *p;
    QQuickItem* lo = a->getLinkedObject ();
    QRect r(lo->x (), lo->y (), lo->width (), lo->height ());
    int sx = 0, sy = 0;

    switch (a->getDirection ()) {
        case ActiveItem::Direction::NORTH:
            if (r.y () < 16)
                return;
            r.translate (0, -r.height () / 4);
            sx = r.width () / 2 - 4;
            break;

        case ActiveItem::Direction::SOUTH:
            if (r.y () > getBattleFieldHeight () - 16)
                return;
            r.translate (0, r.height ());
            sx = r.width () / 2 - 4;
            break;

        case ActiveItem::Direction::EAST:
            if (r.x () > getBattleFieldWidth () - 16)
                return;
            r.translate (r.width (), 0);
            sy = r.height () / 2 - 4;
            break;

        case ActiveItem::Direction::WEST:
            if (r.x () < 16)
                return;
            r.translate (-r.width () / 4, 0);
            sy = r.height () / 2 - 4;
    }

    p = dynamic_cast<ActiveItem*>(s.create (this, ActiveItem::ActiveItemType::PROJECTILE,
                                            QPoint(r.x () + sx, r.y () + sy)));
    p->setFrozen (true);
    QObject::connect (&_timer, SIGNAL(timeout()), p, SLOT(tick()));
    p->setUnitController (_projectileCtl);
    p->setDirection (a->getDirection ());
    p->setRotation (a->getRotation ());
    p->setObjectId (QString("projectile%1").arg (QDateTime::currentMSecsSinceEpoch ()));
    p->setObjectName (p->getObjectId ());
    _projectiles << p;
    a->setFired (false);
    p->setFrozen (false);

    emit projectilesChanged(getProjectiles ());
}

void GameScene::removeProjectile (ActiveItem *a)
{
    if (_projectiles.empty ())
        return;

    _projectiles.removeOne (a);
    emit projectilesChanged(getProjectiles ());
}

void GameScene::removeBlock (Block *b)
{
    QQuickItem* l;
    ObjectRTreeBox item;

    if (_bmap.empty ())
        return;

    _bmap.removeOne (b);
    emit bmapChanged(getBmap ());
    l = b->getLinkedObject ();
    item = ObjectRTreeBox(l->x (), l->y (), l->width (), l->height ());
    _tree->Remove (item.min, item.max, b);
    delete b;
}

int GameScene::getBlocksCount () const
{
    return _bmap.size ();
}

class Respawner : public QRunnable {
public:
    Respawner(ActiveItem* item) {
        setAutoDelete(true);
        this->item = item;
    }

    void run () {
        msleep (2000);
        item->setSpawned (true);
    }

private:
    ActiveItem* item;
};

void GameScene::respawn (ActiveItem* a)
{
    Respawner* resp;
    QPoint pt;

    a->setFrozen (true);
    a->setAlive (false);
    a->setSpawned (false);
    pt = _spawners [a];
    a->getLinkedObject ()->setX (pt.x ());
    a->getLinkedObject ()->setY (pt.y ());
    resp = new Respawner(a);
    wq->run_task (resp);
}

WorkQueue* GameScene::getwq ()
{
    return wq;
}
