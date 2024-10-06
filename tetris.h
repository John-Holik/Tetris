// File: tetris.h
//   By: John Holik
// Desc:

#ifndef TETRIS2_TETRIS_H
#define TETRIS2_TETRIS_H
#include <SFML/Graphics.hpp>

const int FPS = 30; // how many update frames per second
const int FRAME_RATE_MS = (1.f / float(FPS) * 1000.f); // per millisecond

const int FRAMES_NEW_SHAPE = 45; // default rate to show next shape
const int FRAMES_AUTO_MOVE = 30; // default rate to show next shape

// Num of rows and columns that make up the grid in the tetris game
const int GAME_ROWS = 21;
const int GAME_COLUMNS = 10;

// Size of a square block(width x height) of each block in the grid &
// the individual blocks of a tetromino shape
const int BLOCK_SIZE = 30; //

// calc required window size to fit rows and columns with one block size of margin
const int WIN_WIDTH = GAME_COLUMNS * BLOCK_SIZE + (2 * BLOCK_SIZE); // 1 block on the left and right of board
const int WIN_HEIGHT = GAME_ROWS * BLOCK_SIZE + (2 * BLOCK_SIZE); // 1 block on the top and bottom of board

const int GRID_TOP = BLOCK_SIZE;
const int GRID_LEFT = BLOCK_SIZE;

const int START_CELL_COLUMN = 3; // column 4, array index 3
const int START_CELL_ROW = 20; // top row 21, array index 20

const sf::Color BACKGROUND_COLOR = sf::Color::Black;
const sf::Color GRID_COLOR = sf::Color(0xD3, 0xD3, 0xD3, 50); // light gray 50/255 ~20% Opacity

struct KeyPressedState{   // maintain state of each input key across frames
    bool prior;           // state of key in prior frame: Pressed = true
    bool current;         // state of key for this frame: Pressed = true
};



#endif //TETRIS2_TETRIS_H
