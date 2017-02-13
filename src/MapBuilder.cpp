#include "MapBuilder.h"

QList<Block *> MapBuilder::spawnObjects(QObject *parent)
{
    QList<Block*> objects;
    char blk;
    QFile data(_file);

    if (!data.open (QIODevice::ReadOnly | QIODevice::Text))
        return QList<Block*>();

    QTextStream stream(&data);

    for (int y = 0; y < _scene->getRows () + 1; y++)
        for (int x = 0; x < _scene->getColumns () + 1; x++) {
            Block* entity;

            if (stream.atEnd ())
                break;

            stream >> blk;

            if (!isalpha (blk))
                continue;

            entity = Block::create (parent, blk, QPoint(x, y));

            if (entity) {
                entity->setObjectId (QString("%1x%2y%3").arg (blk).arg (x).arg (y));
                objects << entity;
            }
        }

    data.close ();

    return objects;
}
