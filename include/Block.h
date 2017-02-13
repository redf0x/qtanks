#ifndef BLOCK_H
#define BLOCK_H

#include "common.h"
#include "Entity.h"

class Block : public Entity {
    Q_OBJECT

public:
    enum BlockType { VOID, BRICK, STEEL, FOREST, IRON };

    Q_PROPERTY(BlockType blockType READ getBlockType CONSTANT)
    Q_PROPERTY(bool solid READ isSolid CONSTANT)

    Block(QObject* parent = 0, int rotation = 0, bool solid = false, BlockType type = VOID);

    virtual QString getTextureSource () const;
    BlockType getBlockType () const;

    static Block* create (QObject* parent, char sign, QPoint pos);

private:
    BlockType _type;
};

#endif // BLOCK_H
