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
    explicit Loader(QObject* parent = 0);
    ~Loader();

    void titleScreen ();
    void gameScreen ();
    void gameOverScreen ();

signals:
    void contentRequest (QVariant);

public slots:
    void userAction (int c);
    void userWin (int stage);
    void userLost ();

private:
    void terminateGame ();

    GameScene* game;
};

#endif // LOADER_H
