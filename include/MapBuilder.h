#ifndef MAPBUILDER_H
#define MAPBUILDER_H

#include "Entity.h"
#include "Block.h"
#include "GameScene.h"

class MapBuilder : public QObject {
    Q_OBJECT

public:
    MapBuilder(QObject* parent, QString file, GameScene* scene) : QObject(parent), _file(file), _scene(scene) { }
    QList<Block*> spawnObjects (QObject* parent);

private:
    QString _file;
    GameScene* _scene;
};

#endif // MAPBUILDER_H
