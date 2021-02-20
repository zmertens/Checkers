/*
*	The CheckerGame class has the checker game loop and keeps track of the state of the game (which player is winning).
*	This class handles manipulations to checkerpieces as well as high level AI operations. For instance, during gameplay a player will click
*	an arbitrary square to select a checker to move (green highlight), and then the player will click another square to move the checker onto (magenta highlight).
*	The computer checks to see if that jump or movement is legal by finding the square and then finding the checker that resides on the square.
*	There are 2 methods for jumping once a checker has been selected, either by selecting the checker to jump over, or by selecting an empty space beyond the checker to jump over.
*
*		***CHECKER RULES***
*	1) Pawns move forward (towards their king row) and diagonal (one square).
*	2) Kings move diagonal forward or backward (one square).
*	3) If a jump presents itself, the checker in play must jump, but it can decide which jump to take and where.
*	4) Double jumps can occur if the jumping checker does not need to make an additional move in order to secure the double jump.
*	5) A pawn can become a king by traversing from its starting row towards the farthest row opposite of the starting row on the starting board.
*	6) Last team standing wins.
*
*	It is assumed that only two players are playing at a time (it could be two humans or even two computers!).
*	At the moment, player one is red, and player two is yellow. In the future, it will be randomized or customizeable.
*	The checkers vector maintains an index of the checker that's in play. It should be consistent throughout the code for each turn.
*/

#ifndef CHECKERGAME_HPP
#define CHECKERGAME_HPP

#include <SFML/Graphics.hpp> // allows window drawing
#include <SFML/Audio.hpp> // allows audio loading and playback
#include <SFML/Window.hpp> // allows window instantation
#include <SFML/System.hpp> // needed for window instantion (and sf::Time)
#include <iostream>
#include <string>
#include <fstream> // for file IO
#include <ios> // for appending to save file
#include <sstream> // for string streams
#include <string> // for strings
#include <vector> // for vectors
#include "AI.hpp"
#include "SoundManager.hpp"

/*
/*
*	The checkerpiece class represents the checkers. 
*	Each team starts with 12 checkerpieces. This class handles drawing to checkerboard (called within CheckerGame.cpp),
*	and construction of checkerpieces. Checkerpieces have a rank (#'s 0-4), the ranking system is as follows:
*			0) Checker can't move at all.
*			1) Checker can only move.
*			2) Checker can move and become a king.
*			3) Checker can jump.
*			4) Checker can double jump.
*	The actual checkerpieces are managed in the Player class (each player has a vector container).


#ifndef CHECKERPIECE_HPP
#define CHECKERPIECE_HPP

#include <SFML/Graphics.hpp> // allows window drawing
#include <SFML/Window.hpp> // allows window instantation
#include <SFML/System.hpp> // needed for window instantion (and sf::Time)
#include "Global.hpp"

class Checkerpiece : public sf::CircleShape
{
private:
protected:
	const int startingRow;
	const int kingRow;
	const int player;
	int rank;
	bool isKing;
public:
	Checkerpiece(); // dummy constructor for drawing object in CheckerGame class
	Checkerpiece(sf::Color, int, int, int, const int, const int, const int);
	
	void drawCheckers(sf::RenderWindow&, const std::vector<Checkerpiece*>&);

	// setters
	void setKing(bool);
	void setRank(int);
	// getters
	bool getKing();
	const int getKingRow();
	const int getStartingRow();
	const int getPlayer();
	int getRank();
};




/*

/*
*	The checkerboard class represents an 8x8 checkerboard. It extends the FloatRect class in order to use the intersects function.
*	The checkerboard is created by using loop and initializing Square objects (Square class extends sf::RectangleShape class) in an 8x8 2D array.
*
*	The squares have their occupied bool variable initialize in the initSquareArray() function which is called by the constructor.
*	Black squares on the left side of the board (0, y) can cause a "kitty corner jump." If an adjacent checker clicks a checker at (0, y) they will complete an illegal jump.
*	A temporary fix has been added in the findSquare(bool, Square*,...,int) overload function in the 225 and 45 generalDirection cases to avoid the "kitty corner jump". 


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
#



#include "AI.hpp"

#include <iostream>
#include <vector> // for vectors
#include "Player.hpp"

class AI
{
private:
	int currentIndex;
	std::vector<int>* coords;
public:
	AI();
	~AI();
	int getCurrentMoveIndex(Player*, Player*, Checkerboard*); // get the current index
	std::vector<int>* AI_Move(Player*, Player*, Checkerboard*);
};



using std::vector;
using std::cout;
using std::endl;

// default constructor
AI::AI() 
{
	
}

AI::~AI() 
{
	delete coords;
}
 Find the current moving checker's index for the active player by using Moveable class 
int AI::getCurrentMoveIndex(Player* active, Player* enemy, Checkerboard* checkerboard)
{
	vector<Checkerpiece*> activeCheckers = active->getCheckersVector();
	vector<Checkerpiece*> enemyCheckers = enemy->getCheckersVector();
	// iterate looking for the first checker with jumping capacity
	for(unsigned int i = 0; i != activeCheckers.size(); ++i)
		if(Moveable::hasJump(activeCheckers.at(i), enemyCheckers, checkerboard))
		{
			cout << "P2 has found a jump @ index " << i << endl;
			currentIndex = i;
			return i;
		}
	// iterate again but only look for a move
	for(unsigned int i = 0; i != activeCheckers.size(); ++i)
		if(Moveable::hasMove(activeCheckers.at(i), enemyCheckers, checkerboard))
		{
			cout << "P2 has found a move @ index " << i << endl;
			currentIndex = i;
			return i;
		}
	return -1; // this implies game over
}

/* 
*	AI_Move is used for when the computer determines a move. 
*	It handles either jump and movement decisions, as well as variable updates and deletions.
*	Here are the steps of the algorithm.
*	1) Iterate through this player's checkers.
*	2) Iterate through the enemy checkers until we find one we can jump. (using jumpByChecker format)
*	3) Jump, and update positions and deletions, and then we're done.
*	4) If there is no jump, then we find a square to move onto.
*	5) Move, and update positions for the moving checker, and we're done.
*	6) If we can't move or jump any of the checkers, then it is game over for this player.
	
vector<int>* AI::AI_Move(Player* active, Player* enemy, Checkerboard* checkerboard) 
{
	vector<Checkerpiece*> activeCheckers = active->getCheckersVector();
	vector<Checkerpiece*> enemyCheckers = enemy->getCheckersVector();
	for(unsigned i = 0; i != activeCheckers.size(); ++i)
	{
		// check if the ith checker can jump	
		coords = Moveable::findJump(activeCheckers.at(i), enemyCheckers, checkerboard); // get coordinates
		if(coords != nullptr)
		{
			cout << "P2 has found a jump->coords[0,1] " << coords->at(0) << " " << coords->at(1) << endl;
			return coords; // stop here and return the coords
		}
	}

	// By this point, if we haven't returned the coords then we know there are no available jumps

	// iterate again, but this time only look for a move since we know there aren't any jumps
	for(int i = 0; i != activeCheckers.size(); ++i)
	{
		coords = Moveable::findMove(activeCheckers.at(i), checkerboard);
		if(coords != nullptr)
		{
			cout << "P2 has found a move->coords[0,1] " << coords->at(0) << " " << coords->at(1) << endl;
			return coords;
		}
	}

	// If we get this far and haven't returned any coords, then we know that the game is over for this player
	return coords;
}
*/

class CheckerGame
{
private:
	bool isPlaying; // used to keep track of the state of the game
	static sf::Time timeElapsed; // time elapsed from most recent checkers game
	static int winner; // winner of the most recent game (0 if there's no winner)
	Checkerboard* checkerboard;
	Checkerpiece cpDrawer;
	Player* p1;
	Player* p2;

	bool playerHasToJump(Player*&, Player*&);
	bool playerCannotMove(Player*&, Player*&, Checkerboard*&);
	void gameLoop(sf::RenderWindow&, sf::Event&);
	void showWinner(sf::RenderWindow&, sf::Event&);
	void createTeams();
	void changeTurn();
	void handleGameState(sf::Clock&);
	bool isActivePlayerHuman();
	void printChecker(Checkerpiece*, const std::string&);
	void deleteCheckerFromGame(Player*, const int&); 
	void ifCheckerKinged(Checkerpiece*, Square*);
	void saveTime(const double&);
public:
	CheckerGame(sf::RenderWindow&, const bool&, const bool&);
	~CheckerGame();
	void startCheckers(sf::RenderWindow&, sf::Event&);
};

#endif
