#include "AI.hpp"

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

/* Find the current moving checker's index for the active player by using Moveable class */
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
*/	
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
