//  File: tetris.cpp
// Class: COP 3003 Programming II
//    By: John Holik
//  Desc: Main application file for a tetris game
// ------------------------------------------------------------

#include <SFML/Graphics.hpp>
#include <iostream>
#include "tetris.h"
#include "TetrisBoard.h"

// function declarations (prototypes)
// ------------------------------------------------------------
bool processEvents(sf::RenderWindow & window, KeyPressedState input[]);
bool update(KeyPressedState input[], TetrisBoard & board);
void render(sf::RenderWindow & window, TetrisBoard & gameboard);

// function definitions
// ------------------------------------------------------------
int main() {
    //create the game window with width x height with a title
    sf::RenderWindow window {sf::VideoMode{WIN_WIDTH,
                                           WIN_HEIGHT}, "Tetris"};

    // gameboard grid for the Tetris game
    TetrisBoard gameboard;

    // Keyboard state handling
    KeyPressedState keyStates[sf::Keyboard::KeyCount] = {0};


    // Update frame timing
    // --------------------------------------------------------------------
    sf::Clock frameTimer; // frame rate timer
    int lag{0}; // cumulative lag time each frame


    // main game loop
    // --------------------------------------------------------------------
    bool gameover = false;
    while(!gameover){

        lag += frameTimer.restart().asMilliseconds();

        gameover = processEvents(window, keyStates);

        // Wait until we get to a frame boundary to update
        while (lag >= FRAME_RATE_MS){

            gameover = update(keyStates, gameboard);

            lag -= FRAME_RATE_MS; // Reduce the lag by 1 frame
        }
        render(window, gameboard);

    } // end main game loop

    // clean up the main window
    window.close();

    return 0; // return success on exit
} //end main

/**
 * Process window and keyboard events
 * @param window - reference to the main window
 * @param input - prior and current state of each keyboard key
 * @return true = window closing
 */
bool processEvents(sf::RenderWindow & window, KeyPressedState input[]){
    bool closing = false;

    sf::Event event;
    while(window.pollEvent(event)){
        // Check for close request
        if(event.type == sf::Event::Closed){
            closing = true; // Return closing true
        }
        // Check for keyboard events
        // Only watching for key being released
        else if (event.type == sf::Event::KeyReleased){
            // If Key's prior state is off
            if(!input[event.key.code].prior){
                // Set current and prior state on
                // Will be turned off in update
                input[event.key.code].current = true; // Detected this loop
                input[event.key.code].prior = true; // Ignore the next loop
            }
        }
    }// While events

    return closing;
}


/**
 * Update state of game objects each frame
 * @param input - Keyboard input
 * @return true - If game should end
 */
bool update(KeyPressedState input[], TetrisBoard & board){
    bool endgame = false;

    // update objects on the gameboard
    endgame = board.Update(input);

    return endgame;
}// End update







/**
 * Draw shapes on the main window
 * @param window - Reference to the main window
 * @param shape  - Shape to draw
 */
void render(sf::RenderWindow & window, TetrisBoard & board) {

    window.clear(BACKGROUND_COLOR);

    // draw the gameboard grid
    board.render(window);

    window.display();

} // end render
