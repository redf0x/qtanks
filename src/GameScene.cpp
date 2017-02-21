#include <cmath>

#include "GameScene.h"
#include "MapBuilder.h"

GameScene::~GameScene()
{
    qDeleteAll (_bmap);
    _bmap.clear ();
    qDeleteAll (_playableItems);
    _playableItems.clear ();
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

void GameScene::spawnPlayableItem (QPoint pos)
{
    ActiveItem* p = ActiveItem::create (this, ActiveItem::ActiveItemType::PLAYER, pos);

    p->setObjectId(QString("player"));
    p->setObjectName (p->getObjectId ());
    p->setUnitController (_playerCtl);
    _playableItems << p;
}

void GameScene::reset ()
{
    QList<Block*> tmp0(_bmap);

    _bmap.clear ();
    qDeleteAll (tmp0);

    QList<ActiveItem*> tmp1(_playableItems);

    _playableItems.clear ();
    qDeleteAll (tmp1);
}

void GameScene::initialize (QString level)
{
    MapBuilder map(this, level, this);

    reset ();
    _bmap = map.spawnObjects (this);
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

    qDebug() << "Visiting object @ (" << lo->x () << "," << lo->y () << ")";

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

    qDebug() << "player location" << target;

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

    qDebug() << "Scan zone" << scanZone;
    box = ObjectRTreeBox(scanZone.x (), scanZone.y (), scanZone.width (), scanZone.height ());
    ctx.nearestObject = std::numeric_limits<int>::max ();
    ctx.target = target.center ();
    ctx.object = nullptr;
    _tree->Search (box.min, box.max, scanArea, &ctx);

    return qobject_cast<Block*>(ctx.object);
}
