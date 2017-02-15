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
    game->spawnPlayableItem (QPoint(6, 24));
    engine.rootContext ()->setContextProperty ("battleField", game);
    engine.load (QUrl("qrc:/qml/ui/game.qml"));

    app.exec ();

    return 0;
}
