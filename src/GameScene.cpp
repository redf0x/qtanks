#include <cmath>

#include "GameScene.h"
#include "MapBuilder.h"

GameScene::~GameScene()
{
    qDeleteAll (_bmap);
    _bmap.clear ();
    qDeleteAll (_playableItems);
    _playableItems.clear ();
    qDeleteAll (_npcItems);
    _npcItems.clear ();
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

void GameScene::spawnPlayableItem (QPoint pos, QString texOverride)
{
    ActiveItem s, *p;

    p = dynamic_cast<ActiveItem*>(s.create (this, ActiveItem::ActiveItemType::PLAYER, pos));
    p->setObjectId(QString("player"));
    p->setObjectName (p->getObjectId ());
    p->setUnitController (_playerCtl);

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
    _frozen = false;
    _timer.start ();
}

bool GameScene::getFrozen () const
{
    return _frozen;
}

void GameScene::setFrozen (bool p)
{
    if (p != _frozen) {
        QList<ActiveItem*>::iterator i;

        _frozen = p;

        if (_frozen)
            _timer.stop ();
        else
            _timer.start ();

        for (i = _playableItems.begin (); i != _playableItems.end (); i++)
            (*i)->setFrozen (_frozen);

        for (i = _npcItems.begin (); i != _npcItems.end (); i++)
            (*i)->setFrozen (_frozen);

        emit frozenChanged(_frozen);
    }
}

int GameScene::getEnemyCounter () const
{
    return _enemyCounter;
}

void GameScene::setEnemyCounter (int c)
{
    if (c != _enemyCounter) {
        _enemyCounter = c;
        emit enemyCounterChanged(_enemyCounter);
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
