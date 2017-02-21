#include <QGuiApplication>

#include "common.h"
#include "Entity.h"
#include "Block.h"
#include "GameScene.h"
#include "Exceptions.h"

int main (int argc, char** argv)
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    GameScene* game;

    qmlRegisterUncreatableType<Globals>("Tanks.Globals", 0, 1, "Globals", "Globals type cannot be instantiated");
    qmlRegisterType<Entity>();
    qmlRegisterType<Block>();
    qmlRegisterType<ActiveItem>("Tanks.ActiveItem", 0, 1, "ActiveItem");

    game = new GameScene(&app);
    game->initialize (":/data/level.dat");
    game->spawnPlayableItem (QPoint(24, 24));
    game->spawnNpcItem (QPoint(0, 0), "qrc:/image/ui/res/tank_fast.png");
    game->spawnNpcItem (QPoint(13, 0));
    game->spawnNpcItem (QPoint(24, 0), "qrc:/image/ui/res/tank_armored.png");
    engine.rootContext ()->setContextProperty ("battleField", game);
    engine.rootContext ()->setContextProperty ("controller", game->getControllerConfig ());
    engine.load (QUrl("qrc:/qml/ui/game.qml"));
    game->buildObjectsRTree ();
    game->start ();

    app.exec ();

    return 0;
}
