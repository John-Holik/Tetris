// File: TetrisBoard.h
//   By: John Holik
// Desc:

#ifndef TETRIS2_TETRISBOARD_H
#define TETRIS2_TETRISBOARD_H
#include "tetris.h"
#include "Tetromino.h"
#include <SFML/Graphics.hpp>
#include <random>


class TetrisBoard {
public:
    // Constructors
    // --------------------------------------------------------
    TetrisBoard(); // default

    ~TetrisBoard(); // destructor

    // Methods
    // --------------------------------------------------------
    bool Update(KeyPressedState input[]);

    void render(sf::RenderWindow(&window));


// Private
// ------------------------------------------------------------
private:
    struct FrameCounters{
        int newShapeRate;
        int newShape;
        int autoMoveRate;
        int autoMove;
    };

    FrameCounters _counters;

    struct GridCell{
        bool filled;
        sf::RectangleShape block;
    };

    // grid of cells by row and column
    GridCell _cells[GAME_ROWS][GAME_COLUMNS];

    // current and next shape
    Tetromino* _currentShape;
    Tetromino* _nextShape;

    // top/left row/column of grid for current shape
    sf::Vector2i _currentCell;

    // properties for generation random numbers
    // to select the next Tetromino at random
    // uses a merseene_twister generator engine
    // to produce a uniform integer distribution
    std::random_device _randDevice;
    std::mt19937 _randGenerator;
    std::uniform_int_distribution<> _randDistribution;

    void nextShape(); // calc next random shape

    bool canMove(Tetromino::Movement direction);

    void lockShape(); // locks the current shape in gameboard
    bool canRotateShape();
    void wallKick(sf::Vector2i location);
    bool hasCollision(Tetromino& shape, sf::Vector2i location);
};


#endif //TETRIS2_TETRISBOARD_H
