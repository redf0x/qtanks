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

    Block(QObject* parent = nullptr, int rotation = 0, bool solid = false,
          BlockType type = VOID);

    virtual QString getTextureSource () const;
    BlockType getBlockType () const;

protected:
    virtual Entity* createObject (QObject* parent, char sign, QPoint pos);

private:
    BlockType _type;
};

#endif // BLOCK_H
