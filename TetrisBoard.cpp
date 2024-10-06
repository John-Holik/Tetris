// File: TetrisBoard.cpp
//   By: John Holik
// Desc:

#include <iostream>
#include "TetrisBoard.h"
#include "ShapeI.h"
#include "ShapeJ.h"
#include "ShapeL.h"
#include "ShapeO.h"
#include "ShapeS.h"
#include "ShapeT.h"
#include "ShapeZ.h"
#include "tetris.h"

// local functions
bool isKeyPressed(KeyPressedState input[], sf::Keyboard::Key);

/**
 * Default constructor sets up the board
 */
TetrisBoard::TetrisBoard() {

    //initialize frame counters
    _counters = {FRAMES_NEW_SHAPE, 0,
                  FRAMES_AUTO_MOVE, 0};

    // set up the game board grid
    // --------------------------------------------------

    // position and size of a grid block
    sf::Vector2f position{GRID_LEFT, GRID_TOP}; // screen position
    sf::Vector2f size{BLOCK_SIZE, BLOCK_SIZE}; // screen size (pixels)

    // create a temp block shape to update the grids cells
    sf::RectangleShape block{size};
    block.setFillColor(BACKGROUND_COLOR);
    block.setOutlineColor(GRID_COLOR);
    block.setOutlineThickness(-1.f);

    // populate the game board grid with block shapes
    for(int row = GAME_ROWS - 1; row >= 0; -- row){

        for(int col = 0; col < GAME_COLUMNS; ++col){
            block.setPosition(position); // set screen position of block

            _cells[row][col] = {false, block}; // copy shape to grid

            position.x += size.x; // move block right 1 cell
        } // columns left to right

        position.x = GRID_LEFT; // rest block left to left side of grid
        position.y += size.y;  // move block down to next row of grid
    } // rows from top down to bottom
    _randGenerator = std::mt19937 (_randDevice()); // see the generator engine
    _randDistribution = std::uniform_int_distribution<>(Tetromino::ShapeType::SHAPE_NONE + 1,
                                                        Tetromino::ShapeType::SHAPE_COUNT -1);

    _currentShape = nullptr;
    _nextShape = nullptr;
    _currentCell = sf::Vector2i{0,0};

    // get the next shape
    nextShape();

} // default

/**
 * Destructor cleans up gameboard objects
 */
TetrisBoard::~TetrisBoard() {
    delete _currentShape;
    _currentShape = nullptr;

    delete _nextShape;
    _nextShape = nullptr;
}
/**
 * Update shape objects on the Tetris board
 * @param input - user keypress
 * @param board - Tetris game board
 * @return true if game should end
 */
bool TetrisBoard::Update(KeyPressedState *input) {
    bool endGame = false;

    // Check if spacebar was pressed to rotate the shape
    if(_currentShape) {
        if (isKeyPressed(input, sf::Keyboard::Key::Space)) {
            if (canRotateShape()) {
                _currentShape->rotate();
            }
        }

        if (isKeyPressed(input, sf::Keyboard::Key::A)) {
            if(canMove(Tetromino::Movement::MoveLeft)){
                _currentShape->move(Tetromino::Movement::MoveLeft);
                _currentCell.x -= 1;
            }
        }
        else if (isKeyPressed(input, sf::Keyboard::Key::D)) {
            if(canMove(Tetromino::Movement::MoveRight)){
                _currentShape->move(Tetromino::Movement::MoveRight);
                _currentCell.x += 1;
            }
        }

        bool moveDown = false; // true if we are move the shape down
        //checks if the user input to move shape down
        if (isKeyPressed(input, sf::Keyboard::Key::S)) {
            moveDown = true;
        } // user move down

        //if user requests or if it's time to auto move shape
        if(moveDown || _counters.autoMove >= _counters.autoMoveRate){
            // see if we can move it down first
            if(canMove(Tetromino::Movement::MoveDown)){
                // if yes move down
                _currentShape->move(Tetromino::Movement::MoveDown);
                _currentCell.y -= 1;
            }
            // reset auto move counter
            _counters.autoMove = 0;
        } // auto or user move down
        else{
            // otherwise increment frame counter
            _counters.autoMove++;
        }

        if(!canMove(Tetromino::Movement::MoveDown)){
            lockShape();
            delete _currentShape;
            _currentShape = nullptr;
        }

        if (isKeyPressed(input, sf::Keyboard::Key::LShift)) {
            std::cout << _currentShape->toString() << std::endl;
        }
    }
    else{// no current shape
        // count frames until time to show next shape
        if(_counters.newShape < _counters.newShapeRate){
            _counters.newShape++; // increase frame counter
        } else {// show next shape
            _counters.newShape = 0; // reset counter
            _currentShape = _nextShape; // copy next to current
            nextShape(); // get the next shape
        }
    } // current shape
    return endGame;
} // boardUpdate


/**
 * draw game objects on the window
 * @param window - main game window
 */
void TetrisBoard::render(sf::RenderWindow &window) {
    // draw the grid
    for(int row = 0; row < GAME_ROWS; ++row){
        for(int col = 0; col < GAME_COLUMNS; ++col){
            window.draw(_cells[row][col].block);
        } // each column
    } // each row

    // draw current shape if we have one
    if(_currentShape) {
        _currentShape->draw(window);
    }
} // render


// Private methods
// ---------------------------------------------

/**
* Randomly selects the next shape to show
*/
void TetrisBoard::nextShape() {
    // reset current shape cell to top center
    _currentCell = sf::Vector2i (START_CELL_COLUMN, START_CELL_ROW);

    // calc starting screen position
    float x = (_currentCell.x + 1) * BLOCK_SIZE;
    float y = (GAME_ROWS - _currentCell.y) * BLOCK_SIZE;
    sf::Vector2f position{x,y};

    // generate random shape type
    auto nextType = static_cast<Tetromino::ShapeType>(_randDistribution(_randGenerator));


    // generate a shape for nextShape
    switch (nextType) {

        case Tetromino::ShapeType::SHAPE_I:
            _nextShape = new ShapeI(position);
            break;
        case Tetromino::ShapeType::SHAPE_J:
            _nextShape = new ShapeJ(position);
            break;
        case Tetromino::ShapeType::SHAPE_L:
            _nextShape = new ShapeL(position);
            break;
        case Tetromino::ShapeType::SHAPE_O:
            _nextShape = new ShapeO(position);
            break;
        case Tetromino::ShapeType::SHAPE_S:
            _nextShape = new ShapeS(position);
            break;
        case Tetromino::ShapeType::SHAPE_T:
            _nextShape = new ShapeT(position);
            break;
        case Tetromino::ShapeType::SHAPE_Z:
            _nextShape = new ShapeZ(position);
            break;
    }
}


/**
 * process key input for update frames
 * @param input - current key states
 * @param key - check state of particular key
 * @return true if key is in a pressed state
 */
bool isKeyPressed(KeyPressedState input[], sf::Keyboard::Key key){
    bool pressed = false;
    // check if key was pressed to rotate the shape
    if(input[key].prior){
        // this is a current key press for this frame
        if(input[key].current){
            pressed =  true;
            // turn off the current flag to indicate we processed it
            input[key].current = false;
        }
        else{// we were skipping a frame
            // turn off the prior flag, so it can be picked up again
            input[key].prior = false;
        }
    }
    return pressed;
}// is key pressed


bool TetrisBoard::canMove(Tetromino::Movement direction) {
    bool canMove = true;

    // make a copy of the current cell
    sf::Vector2i tempCell = _currentCell;

    // move temp location
    switch(direction) {
        case Tetromino::MoveLeft:
            if (_currentCell.x < -1)
                canMove = false;
            else
                tempCell.x -= 1;
            break;
        case Tetromino::MoveRight:
            if(_currentCell.x + _currentShape->getColumns() > GAME_COLUMNS + 1)
                canMove = false;
            else
                tempCell.x += 1;
            break;
        case Tetromino::MoveDown:
            if(_currentCell.y < 0)
                canMove = false;
            else
                tempCell.y -= 1;
    }// direction

    if(canMove){
        // declare temp shape so default constructor is called to
        Tetromino tempShape;

        // make a copy of the current shape to temp shape
        tempShape = *_currentShape;

        canMove = !hasCollision(tempShape, tempCell);
    }
    return canMove;
} // canMove


/**
 * Determine if a shape can rotate without colliding with any existing shapes
 * Shape may have to perform a wall-kick to put it back inside the gameboard.
 * @return true if it can rotate
 */
bool TetrisBoard::canRotateShape(){
    bool canRotate = true;

    //make a copy of the current cell
    sf::Vector2i tempCell = _currentCell;

    //make sure location and shape are inside gameboard
    wallKick(tempCell);

    Tetromino tempShape;
    tempShape = *_currentShape;
    tempShape.rotate();

    canRotate = !hasCollision(tempShape, tempCell);

    if(canRotate){
        int diffX = std::abs(tempCell.x - _currentCell.x);
        if(diffX > 0){
            // assume on left wall (x is -) so move right
            auto dir = Tetromino::Movement::MoveRight;
            if(_currentCell.x > 0)// if the x was + then on right side
                dir = Tetromino::Movement::MoveLeft;
            _currentShape->move(dir,diffX);
            _currentCell = tempCell;
        }
    }

    
    return canRotate;
}// canRotate


/**
 * Move the location of a shape back inside the gameboard
 * @param location - of the current shape
 */
void TetrisBoard::wallKick(sf::Vector2i location) {
    if(location.x <0){
        location.x = 0;
    }
    else if (location.x + _currentShape->getColumns() >= GAME_COLUMNS){
        int diff = location.x + _currentShape->getColumns() - GAME_COLUMNS;
        location.x -= diff;
    }
}// wallKick



/**
 * See if any of the blocks in a shape overlay a block in the grid or are outside of the walls
 * @param shape - reference to the shape to check
 * @param location - location of the shape
 * @return true if there is a collision
 */
bool TetrisBoard::hasCollision(Tetromino& shape, sf::Vector2i location){
    bool collision = false;

    // used to indicate if current block is in grid or outside walls
    bool filled{};

    //check if temp shape at new temp cell location has any collisions
    for(int row = 0; row < shape.getRows() && !collision; ++row){

        for(int column = 0; column < shape.getColumns() && !collision; ++column){
            // check if grid cell is filled
            if(location.x + column < 0 ||
               location.x + column >= GAME_COLUMNS ||
               location.y - row < 0){
                filled = true;
            } else{
                filled = _cells[location.y - row][location.x + column].filled;
            }

            // check for collision between temp shape and grid cell
            if(filled and shape.hasBlock(row, column)){
                collision = true;
            }
        }// each column
    }// each row
    return collision;
} // hasCollision



/**
* Lock the current shape into the current position on gameboard
*/
void TetrisBoard::lockShape() {
    for(int row = 0; row < _currentShape->getRows(); ++row){
        for(int column = 0; column < _currentShape->getColumns(); ++column){
            if(_currentCell.x + column >= 0 ||
               _currentCell.x + column <= (GAME_COLUMNS - 1) ||
               _currentCell.y - row >= 0){
                if(_currentShape->hasBlock(row,column)){
                    _cells[_currentCell.y - row][_currentCell.x + column].block.setFillColor(_currentShape->getFillColor());
                    _cells[_currentCell.y - row][_currentCell.x + column].filled = true;
                }
            }
            
        }// each column
    }// each row
}// lockShape