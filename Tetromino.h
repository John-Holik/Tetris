//  File: Tetromino.h
// Class: COP 3003 Programming II
//    By: John Holik
//  Desc: Header file for a shape representing the shapes
//        found in a game of Tetris. Each shape is made up
//        of multiple rectangles used to create the shape.
//        Internally they Teromino maintains a Matrix of 0's
//        and 1's to represent where graphical shapes should
//        be drawn, based on the lop-left corner of the shape.
// ------------------------------------------------------------

#ifndef TETRIS1_TETROMINO_H
#define TETRIS1_TETROMINO_H

#include <SFML/Graphics.hpp>
#include "Matrix.h"
#include <string>

class Tetromino : public Matrix{
public:
    // color templetes for each of the shapes
    static const unsigned int LIGHT_BLUE = 0XADD8E6FF;
    static const unsigned int DARK_BLUE = 0X00008BFF;
    static const unsigned int ORANGE = 0XFFA500FF;
    static const unsigned int YELLOW = 0XFFFF00FF;
    static const unsigned int GREEN = 0X00FF00FF;
    static const unsigned int RED = 0XFF0000FF;
    static const unsigned int MAGENTA = 0XFF00FFFF;

    // known Tetromino shape types
    enum ShapeType{
        SHAPE_NONE = -1,
        SHAPE_I,
        SHAPE_J,
        SHAPE_L,
        SHAPE_O,
        SHAPE_S,
        SHAPE_T,
        SHAPE_Z,
        SHAPE_COUNT
    };


    // shape movement constants
    enum Movement{
        MoveNone,
        MoveLeft,
        MoveRight,
        MoveDown
    };

    // Constructors
    // --------------------------------------------------------
    Tetromino(); // Deafult
    Tetromino(int rows, int columns);
    Tetromino(int rows, int columns, int *blocks);

    // Accessors
    // --------------------------------------------------------
    ShapeType getShapeType() {return _shapeType;}
    
    sf::Vector2f getSize(){return _size;}
    void setSize(sf::Vector2f size) {_size = size;}

    sf::Vector2f getPosition(){return _position;}
    void setPosition(sf::Vector2f position){_position = position;}

    sf::Color getFillColor(){return _fillColor;}
    void setFillColor(sf::Color fillColor){_fillColor = fillColor;}

    // Methods
    // --------------------------------------------------------
    void draw(sf::RenderWindow & window);

    void rotate();

    void move(Movement direction, int blocks = 1);

    std::string toString();

protected:
    ShapeType _shapeType;

    sf::Vector2f _size;      // (width, height)
    sf::Vector2f _position;  // (left, top)
    sf::Color _fillColor;
};


#endif //TETRIS1_TETROMINO_H
