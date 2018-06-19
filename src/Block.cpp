#include "Block.h"

Block::Block(QObject* parent, int rotation, bool solid, BlockType type) :
    Entity(parent), _type(type)
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
            break;

        case VOID:
            return name;
    }

    return "qrc:/image/ui/res/small_" + name + ".png";
}

Entity* Block::createObject (QObject* parent, char sign, QPoint pos)
{
    Block* e{};

    switch (_tolower(sign)) {
        case 'b':
            e = new Block(parent, 0, true, BRICK);
            e->setArmor (1);
            break;

        case 's':
            e = new Block(parent, 0, true, STEEL);
            e->setArmor (std::numeric_limits<int>::max ());
            break;

        case 'f':
            e = new Block(parent, 0, false, FOREST);
            e->_z = 2;
            break;

        case 'i':
            e = new Block(parent, 0, false, IRON);
            e->_z = 0;
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
