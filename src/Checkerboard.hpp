/*
*	The checkerboard class represents an 8x8 checkerboard. It extends the FloatRect class in order to use the intersects function.
*	The checkerboard is created by using loop and initializing Square objects (Square class extends sf::RectangleShape class) in an 8x8 2D array.
*
*	The squares have their occupied bool variable initialize in the initSquareArray() function which is called by the constructor.
*	Black squares on the left side of the board (0, y) can cause a "kitty corner jump." If an adjacent checker clicks a checker at (0, y) they will complete an illegal jump.
*	A temporary fix has been added in the findSquare(bool, Square*,...,int) overload function in the 225 and 45 generalDirection cases to avoid the "kitty corner jump". 
*/

#ifndef CHECKERBOARD_HPP
#define CHECKERBOARD_HPP

#include <SFML/Graphics.hpp> // allows window drawing
#include <SFML/Window.hpp> // allows window instantation
#include <SFML/System.hpp> // needed for window instantion (and sf::Time)
#include <iostream>
#include "Square.hpp"

class Checkerboard : public sf::FloatRect
{
private:
	Square* squareArray[SQUARES_HORIZONTAL][SQUARES_VERTICAL];

public:
	Checkerboard(sf::RenderWindow&);
	~Checkerboard();

	Square* findSquare(int, int);
	Square* findSquare(int);
	Square* findJumpOntoSquare(Square*, int);
	Square* findIntermSquare(Square*, int);
	void drawGrid(sf::RenderWindow&, int, int);
	void drawGrid(sf::RenderWindow&, int, int, int, int, bool);
	void printTheSquares();
	void initSquareArray();
	bool intersects(const sf::RectangleShape, const sf::RectangleShape);

	// setters

	// getters
	Square** getSquareArray();
};
#endif 