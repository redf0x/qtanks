#include "MapBuilder.h"
#include "Exceptions.h"

QList<Block *> MapBuilder::spawnObjects(QObject *parent)
{
    QList<Block*> objects;
    char blk;
    QFile data(_file);

    if (!data.open (QIODevice::ReadOnly | QIODevice::Text))
        return QList<Block*>();

    QTextStream stream(&data);

    for (int y = 0; y < Globals::fieldCellRows + 1; y++)
        for (int x = 0; x < Globals::fieldCellColumns + 1; x++) {
            Block b, *entity;

            if (stream.atEnd ()) {
                if (y < Globals::fieldCellRows)
                    throw InvalidLevelData(_file, InvalidLevelData::UNEXPECTED_EOF);
                else
                    break;
            }

            stream >> blk;

            if (blk == '\n')
                continue;

            if (!isalpha (blk))
                throw InvalidLevelData(_file);

            entity = dynamic_cast<Block*>(b.create (parent, blk, QPoint(x, y)));

            if (entity) {
                entity->setObjectId (QString("%1x%2y%3").arg (blk).arg (x).arg (y));
                objects << entity;
            }
        }

    data.close ();

    return objects;
}
