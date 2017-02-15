#include "GameScene.h"
#include "MapBuilder.h"

GameScene::~GameScene()
{
    qDeleteAll (_bmap);
    _bmap.clear ();
    qDeleteAll (_playableItems);
    _playableItems.clear ();
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
