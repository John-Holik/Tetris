// File: ShapeT.h
//   By: John Holik
// Desc:

#ifndef TETRIS3_SHAPET_H
#define TETRIS3_SHAPET_H
#include "tetris.h"
#include "Tetromino.h"
#include <SFML/Graphics.hpp>


class ShapeT : public Tetromino {
private:

public:
    explicit ShapeT(sf::Vector2f position={0.f, 0.f}): Tetromino(3,3){
    // set the shape type
    _shapeType  = SHAPE_T;

        // define the matrix values
        int blocks[9] = {0, 0, 0,
                         1, 1, 1,
                         0, 1, 0};
        // set the internal matrix
            setMatrix(blocks);

        // set the screen size, position, and color
        _size = sf::Vector2f(_rows * BLOCK_SIZE, _columns * BLOCK_SIZE);
        _position = position;
        _fillColor = sf::Color(MAGENTA);
    }
};


#endif //TETRIS3_SHAPET_H
