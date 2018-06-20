#include "Loader.h"

Loader::Loader(QObject* parent) : QObject(parent)
{
    if (!dynamic_cast<QQmlApplicationEngine*>(parent))
        throw InvalidOwnership(parent);
    game = 0;
}

Loader::~Loader()
{
}

void Loader::titleScreen ()
{
    emit contentRequest(QString("qrc:/qml/ui/start.qml"));
}

void Loader::gameOverScreen ()
{
    emit contentRequest(QString("qrc:/qml/ui/gameover.qml"));
}

void Loader::gameScreen ()
{
    game = new GameScene((QQmlApplicationEngine*)parent (), this->parent ());
    emit contentRequest(QString("qrc:/qml/ui/game.qml"));
    game->initialize (":/data/level.dat");
    game->spawnPlayerItem (QPoint(24, 24));
    game->spawnNpcItem (QPoint(0, 0), "qrc:/image/ui/res/tank_fast.png");
    game->spawnNpcItem (QPoint(13, 0));
    game->spawnNpcItem (QPoint(24, 0), "qrc:/image/ui/res/tank_armored.png");
    game->spawnBase (QPoint(12, 24));

    game->buildObjectsRTree ();
    QObject::connect(game, SIGNAL(winCondition(int)), this, SLOT(userWin(int)));
    QObject::connect(game, SIGNAL(loseCondition()), this, SLOT(userLost()));
    game->start ();
}

void Loader::userWin (int stage)
{
    Q_UNUSED(stage)
    userAction (Globals::GAMEOVER);
}

void Loader::userLost ()
{
    userAction (Globals::GAMEOVER);
}

void Loader::userAction (int c)
{
    switch (c) {
        case Globals::EXIT:
            QApplication::quit ();
            break;

        case Globals::START:
            gameScreen ();
            break;

        case Globals::GAMEOVER:
            terminateGame ();
            gameOverScreen ();
            break;

        case Globals::CANCEL_GAME:
            terminateGame ();
            titleScreen ();
    }
}

void Loader::terminateGame()
{
    game->reset ();
    game->deleteLater ();
}
