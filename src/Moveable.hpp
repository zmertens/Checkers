/* 
*	The Moveable class is a static class used for movement validation of checker pieces.
*	We check things like whether or not a checker is moving over a friendly checker, if
*	a distance between current and future squares is within a moveable distance for both
*	king and pawn checkers, and whether or not a jump is valid. 
*	There are 2 types of jumps here: jumpByChecker and jumpBySquare.
*	JumpByChecker works when the player selects the checker they want to jump over.
*	JumpBySquare works when the player selects the square they want to land on during a jump.
*	We also check whether or not a given checker has the capability to perform a jump (hasJump).
*/

#ifndef MOVEABLE_HPP
#define MOVEABLE_HPP

#include "Checkerboard.hpp"
#include "Checkerpiece.hpp"
#include "Player.hpp"
#include <vector> // for vector
#include <cmath> // for absolute function

class Moveable
{
private:
protected:
public:
	static bool moveable(Checkerpiece*, Square*, Square*);
	static bool moveable(std::vector<Checkerpiece*>, Square*, Square*, const int&);
	static bool moveableDistance(const int&, const int&, const int&, const int&);
	static bool friendly(const std::vector<Checkerpiece*>&, Square*);
	static bool friendly(const std::vector<Checkerpiece*>&, Square*, Square*);
	static int findGeneralDirection(Square*, Square*);
	static bool jumpBySquare(std::vector<Checkerpiece*>, Square*, Square*, Square*, const int&);
	static bool jumpByChecker(std::vector<Checkerpiece*>, Square*, Square*, Square*, const int&);
	static bool hasJump(Checkerpiece*, const std::vector<Checkerpiece*>&, Checkerboard*&); // validation of a single checker's ability to jump
	static bool hasMove(Checkerpiece*, const std::vector<Checkerpiece*>&, Checkerboard*&); // when the AI needs to find a move
	static std::vector<int>* findJump(Checkerpiece*, const std::vector<Checkerpiece*>&, Checkerboard*&); // when the AI needs to find a jump
	static std::vector<int>* findMove(Checkerpiece*, Checkerboard*&); // when the AI needs to find a move
};

#endif