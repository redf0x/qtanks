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

    _tree = new RTree();

    if (!_bmap.isEmpty ())
        for (b = _bmap.begin (); b != _bmap.end (); b++)
            _tree->Insert (*b, bounds ((*b)->getLinkedObject ()->x (), (*b)->getLinkedObject ()->y(),
                                       (*b)->getLinkedObject ()->width (), (*b)->getLinkedObject ()->height ()));
}

GameScene::BoundingBox GameScene::bounds (int x, int y, int w, int h)
{
    GameScene::BoundingBox box;

    box.edges[0].first = x;
    box.edges[0].second = x + w;
    box.edges[1].first = y;
    box.edges[1].second = y + h;

    return box;
}

struct Visitor {
    int count;
    bool ContinueVisiting;
    Block* nearestObstacle;

    Visitor() : count(0), ContinueVisiting(true), nearestObstacle(nullptr) {}

    void operator() (const GameScene::RTree::Leaf* const leaf)
    {
        qDebug() << "#" << count << " visited " << leaf->leaf <<    \
                    " with " << leaf->leaf->getLinkedObject ()->x () << " " <<  \
                    leaf->leaf->getLinkedObject ()->y ();

        if (leaf->leaf->isSolid ())
            nearestObstacle = qobject_cast<Block*>(leaf->leaf);

        ContinueVisiting = false;

        count++;
    }
};

Block* GameScene::getNearestObstacle (QRect region)
{
    GameScene::BoundingBox r = bounds (region.x () + 2, region.y () + 2, region.width () - 2, region.height () - 2);
    Visitor x;
    Block* ob;

    x = _tree->Query (GameScene::RTree::AcceptOverlapping(r), Visitor());
    ob = x.nearestObstacle;
    _tree->RemoveBoundedArea (r);

    return ob;
}
