#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <string> // for strings

const int MAX_CHECKERS_PER_TEAM = 12;
const int KING_ROW_0 = 0; // used for checkerpieces, each checkerpiece has a row to advanced to become a king
const int KING_ROW_7 = 7;

const int SQUARES_VERTICAL = 8;
const int SQUARES_HORIZONTAL = 8;
const int TOTAL_SQUARES = 64;

// note that the only way to accurately resize the window is here (for now, until I learn how to use sf::View class)
const int WINDOW_WIDTH = 600; // the width of the window
const int WINDOW_HEIGHT = 600; // the height of the window
const int XOFFSET = (WINDOW_WIDTH / SQUARES_VERTICAL); // the horizontal offset, this is used to measure square and checker width
const int YOFFSET = (WINDOW_HEIGHT / SQUARES_HORIZONTAL); // the vertical offset, this is used to measure square and checker width
const int WINDOW_POSITION_X = 0; // the vertical position of the window when it is created
const int WINDOW_POSITION_Y = 0; // the horizontal position of the window when it is created
const std::string WINDOW_TITLE = "Checkers";

enum sound_resources 
{
	SOUND_MOVE,
	SOUND_JUMP_CHECKER,
	SOUND_JUMP_SQUARE,
	SOUND_KING
};

#endif