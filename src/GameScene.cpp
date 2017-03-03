#include <cmath>

#include "GameScene.h"
#include "MapBuilder.h"
#include "Utility.h"
#include "Globals.h"

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

QRect GameScene::mapRect (QRect& source, MappingType mapping)
{
    QRect result;

    switch (mapping) {
        case FROM_LOGICAL:
            result = QRect(source.x () * _cell.width,
                           source.y () * _cell.height,
                           source.width () * _cell.width,
                           source.height () * _cell.height);
            break;

        case FROM_PHYSICAL:
            result.setX (result.x () / _cell.width);
            result.setY (result.y () / _cell.height);
            result.setWidth (result.width () / _cell.width);
            result.setHeight (result.height () / _cell.height);
            break;
    }

    return result;
}

void GameScene::widthChanged (int newwidth)
{
    _fieldSize.setX (newwidth);
    qDebug() << "reported field width" << _fieldSize.x ();
    _cell.width = _fieldSize.x () / columns;
    qDebug() << "logical cell width" << _cell.width;
}

void GameScene::heightChanged (int newheight)
{
    _fieldSize.setY (newheight);
    qDebug() << "reported field height" << _fieldSize.y ();
    _cell.height = _fieldSize.y () / rows;
    qDebug() << "logical cell height" << _cell.height;
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

void GameScene::spawnPlayerItem (QPoint pos, QString texOverride)
{
    ActiveItem s, *p;
    QRect realLoc;

    realLoc = QRect(pos.x (), pos.y (), 2, 2);
    realLoc = mapRect (realLoc, MappingType::FROM_LOGICAL);

    p = dynamic_cast<ActiveItem*>(s.create (this, ActiveItem::ActiveItemType::PLAYER, QPoint(realLoc.x (), realLoc.y ())));
    p->setWidth (realLoc.width ()); p->setHeight (realLoc.height ());
    p->setObjectId(QString("player"));
    p->setObjectName (p->getObjectId ());
    p->setUnitController (_playerCtl);
    QObject::connect (&_timer, SIGNAL(timeout()), p, SLOT(tick()));

    if (!texOverride.isEmpty ()) {
        p->overrideTexture (true);
        p->setTextureSource (texOverride);
    }

    _playableItems << p;
    _spawners.insert (p->getObjectId (), QPoint(realLoc.x (), realLoc.y ()));
    emit playableItemsChanged(getPlayableItems ());
}

void GameScene::spawnNpcItem (QPoint pos, QString texOverride)
{
    ActiveItem s, *p;
    QRect realLoc;
    Attribute attr;
    int initialValue = FIRE_AT_MOST + (int) ((FIRE_AT_LEAST - FIRE_AT_MOST + 1) * (rand () / (RAND_MAX + 1.0)));

    realLoc = QRect(pos.x (), pos.y (), 2, 2);
    realLoc = mapRect (realLoc, MappingType::FROM_LOGICAL);

    p = dynamic_cast<ActiveItem*>(s.create (this, ActiveItem::ActiveItemType::NPC, QPoint(realLoc.x (), realLoc.y ())));
    p->setWidth (realLoc.width ()); p->setHeight (realLoc.height ());
    p->setObjectId(QString("npc@%1%2").arg (pos.x ()).arg (pos.y ()));
    p->setObjectName (p->getObjectId ());
    QObject::connect (&_timer, SIGNAL(timeout()), p, SLOT(tick()));
    p->setUnitController (_botCtl);

    if (!texOverride.isEmpty ()) {
        p->overrideTexture (true);
        p->setTextureSource (texOverride);
    }

    attr = Attribute("counter", QVariant::fromValue (initialValue), EXPIRY_NEVER,"fire");
    p->addAttribute (attr);

    _npcItems << p;
    _spawners.insert (p->getObjectId (), QPoint(realLoc.x (), realLoc.y ()));
    emit npcItemsChanged(getNpcItems ());
}

void GameScene::reset ()
{
    _timer.stop ();

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

void GameScene::fixupObject(Block* block)
{
    QRect rect(block->x (), block->y (), block->width (), block->height ());

    rect = mapRect (rect, MappingType::FROM_LOGICAL);
    block->setX (rect.x ()); block->setY (rect.y ());
    block->setWidth (rect.width ()); block->setHeight (rect.height ());
}

void GameScene::initialize (QString level)
{
    MapBuilder map(this, level, this);

    reset ();
    _bmap = map.spawnObjects (this);
    for_all_blocks(_bmap, fixupObject);
    emit bmapChanged(getBmap ());
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
            ObjectRTreeBox item((*b)->x (), (*b)->y (), (*b)->width (), (*b)->height ());

            _tree->Insert (item.min, item.max, *b);
        }
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
    QRect r(e->x (), e->y (), e->width (), e->height ());
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
    _frozen = false;
    _timer.start ();
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
    QRect r1(a->x (), a->y (), a->width (), a->height ()), r2;

    switch (a->getDirection ()) {
        case ActiveItem::NORTH:
            r1.translate (0, -a->height () / 2);
            break;

        case ActiveItem::SOUTH:
            r1.translate (0, a->height () / 2);
            break;

        case ActiveItem::EAST:
            r1.translate (a->width () / 2, 0);
            break;

        case ActiveItem::WEST:
            r1.translate (-a->width () / 2, 0);
    }

    for (QList<ActiveItem*>::iterator i = list.begin (); i != list.end (); i++)
        if (*i != a) {

            r2 = QRect((*i)->x (), (*i)->y (), (*i)->width (), (*i)->height ());

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
            QRect r1(a->x (), a->y (), a->width (), a->height ()),
                    r2((*i)->x (), (*i)->y (), (*i)->width (), (*i)->height ());

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
    QRect r(a->x (), a->y (), a->width (), a->height ());
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
    p->setWidth (_cell.width / 2); p->setHeight (_cell.height / 2);
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

    a->disconnect ();
    a->setUnitController (0);
    _projectiles.removeOne (a);
    emit projectilesChanged(getProjectiles ());
}

void GameScene::removeBlock (Block *b)
{
    ObjectRTreeBox item;

    if (_bmap.empty ())
        return;

    _bmap.removeOne (b);
    emit bmapChanged(getBmap ());
    item = ObjectRTreeBox(b->x (), b->y (), b->width (), b->height ());
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
        qDebug() << "schedule respawn for" << item->getObjectId ();
    }

    void run () {
        qDebug() << item->getObjectId () << "waiting for respawn";
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

    if (_enemyCounter - 1 >= 0)
        _enemyCounter--;

    emit enemyCounterChanged(_enemyCounter);

    a->setFrozen (true);
    a->setAlive (false);
    a->setSpawned (false);

    /* when enemy counter drops to 3 we start to remove spawners */
    if (_enemyCounter < 3 && !_npcItems.empty ()) {
        ActiveItem* v = nullptr;

        for (int i = 0; i < _npcItems.size (); i++)
            if (!_npcItems [i]->isSpawned () && !_npcItems [i]->isAlive ()) {
                v = _npcItems [i];
                break;
            }

        _npcItems.removeOne (v);
        v->disconnect ();
        emit npcItemsChanged(getNpcItems ());
        _spawners.remove (v->getObjectId ());
        qDebug() << "entity" << v->getObjectId () << "eliminated, not respawning it anymore";

        delete v;
    } else {    /* otherwise schedule respawn in 2 secs */
        pt = _spawners [a->getObjectId ()];
        a->setX (pt.x ());
        a->setY (pt.y ());
        resp = new Respawner(a);
        wq->run_task (resp);
    }

    /* Game over */
    if (!_enemyCounter) {
        finalize ();
        emit winCondition(_stage);
    }
}

WorkQueue* GameScene::getwq ()
{
    return wq;
}

void GameScene::finalize ()
{
    _timer.stop ();

    /* freeze all bullets to prevent them from crushing us with spurious ticks */
    for (QList<ActiveItem*>::iterator p = _projectiles.begin (); p != _projectiles.end (); p++) {
        (*p)->setFrozen (true);
        removeProjectile ((*p));
    }
}
