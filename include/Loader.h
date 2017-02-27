#ifndef LOADER_H
#define LOADER_H

#include "common.h"
#include "Entity.h"
#include "Block.h"
#include "GameScene.h"
#include "Exceptions.h"

class Loader : public QObject {
    Q_OBJECT

public:
    explicit Loader(QObject* parent = 0) : QObject(parent) {
        if (!dynamic_cast<QQmlApplicationEngine*>(parent))
            throw InvalidOwnership(parent);
        game = 0;
    }

    ~Loader() {
        if (game)
            delete game;
    }

    inline void titleScreen () { emit contentRequest(QString("qrc:/qml/ui/start.qml")); }

    inline void gameScreen () {
        game = new GameScene(this->parent ());
        game->initialize (":/data/level.dat");
        game->spawnPlayableItem (QPoint(24, 24));
        game->spawnNpcItem (QPoint(0, 0), "qrc:/image/ui/res/tank_fast.png");
        game->spawnNpcItem (QPoint(13, 0));
        game->spawnNpcItem (QPoint(24, 0), "qrc:/image/ui/res/tank_armored.png");
        ((QQmlApplicationEngine*)parent ())->rootContext ()->setContextProperty ("battleField", game);
        ((QQmlApplicationEngine*)parent ())->rootContext ()->setContextProperty ("controller", game->getControllerConfig ());
        emit contentRequest(QString("qrc:/qml/ui/game.qml"));
        game->buildObjectsRTree ();
        game->start ();
    }

signals:
    void contentRequest (QVariant);

public slots:
    void userAction (int c) {
        qDebug() << "user choice" << c;
        switch (c) {
            case Globals::EXIT:
                QApplication::quit ();
                break;

            case Globals::START:
                gameScreen ();
                break;

            case Globals::CANCEL_GAME:
                qDebug() << "User aborted game";
                game->reset ();
                delete game;
                game = 0;
                titleScreen ();
        }
    }

private:
    GameScene* game;
};

#endif // LOADER_H
