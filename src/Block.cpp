#include "Block.h"

Block::Block(QObject* parent, int rotation, bool solid, BlockType type) : Entity(parent), _type(type)
{
    setSolid (solid);
    setRotation (rotation);
}

Block::BlockType Block::getBlockType () const
{
    return _type;
}

QString Block::getTextureSource () const
{
    QString name("");

    switch (_type) {
        case BRICK:
            name = "brick";
            break;

        case STEEL:
            name = "steel";
            break;

        case FOREST:
            name = "forest";
            break;

        case IRON:
            name = "iron";

        case VOID:
            return name;
    }

    return "qrc:/image/ui/res/small_" + name + ".png";
}

Block* Block::create(QObject* parent, char sign, QPoint pos)
{
    Block* e;

    switch (_tolower(sign)) {
        case 'b':
            e = new Block(parent, 0, true, BRICK);
            break;

        case 's':
            e = new Block(parent, 0, true, STEEL);
            break;

        case 'f':
            e = new Block(parent, 0, false, FOREST);
            break;

        case 'i':
            e = new Block(parent, 0, false, IRON);
            break;

        case 'x':
            return nullptr;
    }

    if (e) {
        e->setX (pos.x ());
        e->setY (pos.y ());
    }

    return e;
}
