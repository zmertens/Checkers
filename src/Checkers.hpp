/*
*	This class handles creation of the application. 
*	An SFML window is created. The window has a main menu and an animation.
*	Main menu options include (by pressing keyboard):
*	P - Player verses Player
*	C - Player verses Computer
*	R - View rules
*	T - View fastest game times.
*	Menu options are selected by pressing the appropriate key on the keyboard.
*	There is a "game in progress" animation below the main menu options on the window.
*	All graphics are drawn using SFML primitives (circles, rectangles, and basic polygons).
*/

#ifndef CHECKERS_HPP
#define CHECKERS_HPP

#include <SFML/Graphics.hpp> // allows window drawing
#include <SFML/Window.hpp> // allows window instantation
#include <SFML/System.hpp> // needed for window instantion (and sf::Time)
#include <iostream>
#include <string>
#include <fstream> // for file IO
#include <string> // for strings
#include <vector> // for vector
#include <algorithm> // for sort
#include "CheckerGame.hpp"

class Checkers
{
private:
	void loadTimes(sf::RenderWindow&, sf::Event&);
	void animation(sf::RenderWindow&, const int&, const int&);
public:
	Checkers();
	void start(); // start the Main Menu window
};

#endif 
