#include "Player.hpp"

using std::vector;
using std::cout;
using std::endl;
using std::string;

Player::Player(const int number, const bool isHuman)
	: isHuman(isHuman)
	, number(number)
{
	vector<Checkerpiece*> checkers; 
	// reserve memory space for 12 checkers
	checkers.reserve(MAX_CHECKERS_PER_TEAM);
}

Player::~Player()
{
	// erase the entire checker vector
	checkers.erase(checkers.begin(), checkers.end());
}

/* Display the checkers for this player */
void Player::displayCheckers() 
{
	cout << endl <<"Display Player# " << this->getNumber() << "'s active checkers" << endl;
	vector<Checkerpiece*>::const_iterator it;
	for(it = this->checkers.begin(); it != this->checkers.end(); ++it)
		cout << "Position(x,y): (" << (*it)->getPosition().x << ", " << (*it)->getPosition().y << ") " << 
			"King Row: " << (*it)->getKingRow() << " King: " << (*it)->getKing() << endl;
}

// add a checker to the player's checkers vector
void Player::addChecker(Checkerpiece* checkerpiece)
{
	checkers.push_back(checkerpiece);
}

// returns the index # of a checkerpiece in the checkers vector by (x, y) position
int Player::findCheckerIndex(const int& x, const int& y)
{
	for(unsigned int i = 0; i != this->checkers.size(); ++i)
		if(checkers[i]->getPosition().x == x && checkers[i]->getPosition().y == y)
			return i;
	return -1; // checker not found
}

// finds a checkerpiece in the checkers vector by comparing square position and checker position
int Player::findCheckerIndex(const Square* tempSquare)
{
	for(unsigned int i = 0; i < checkers.size(); ++i)
		if((checkers[i]->getPosition().x == tempSquare->getPosition().x) && (checkers[i]->getPosition().y == tempSquare->getPosition().y))
			return i;
	return -1; // checker not found
}

// delete a checker
void Player::deleteChecker(const int& deleteIndex)
{
	this->checkers.erase(this->checkers.begin() + deleteIndex);
}

// delete all the checkers
void Player::deleteAllCheckers()
{
	this->checkers.erase(this->checkers.begin(), this->checkers.end());
}

// getters
vector<Checkerpiece*>& Player::getCheckersVector()
{
	return checkers;
}

// return the Checkerpiece* at the index
Checkerpiece* Player::getChecker(const int& index)
{
	return this->checkers.at(index);
}

bool Player::getTurn()
{
	return turn;
}

int Player::getCounter()
{
	return checkers.size();
}

const bool Player::getIsHuman()
{
	return isHuman;
}

const int Player::getNumber()
{
	return number;
}

// setters
void Player::setTurn(const bool& turn)
{
	this->turn = turn;
}