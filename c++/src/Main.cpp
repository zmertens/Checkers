/*
 *    This is a checkers game programmed in C++ using SFML.
 *	By Zach Mertens @link:https://github.com/zmertens @mailto:zach.mm035@gmail.com
 *	Uses sound effects from WitchBlast (see NOTICE in resource folder)
 *	Free software! GPL license.
*/

/*	The checkerboard class represents an 8x8 checkerboard. It extends the FloatRect class in order to use the intersects function.
*	The checkerboard is created by using loop and initializing Square objects (Square class extends sf::RectangleShape class) in an 8x8 2D array.
*
*	The squares have their occupied bool variable initialize in the initSquareArray() function which is called by the constructor.
*	Black squares on the left side of the board (0, y) can cause a "kitty corner jump." If an adjacent checker clicks a checker at (0, y) they will complete an illegal jump.
*	A temporary fix has been added in the findSquare(bool, Square*,...,int) overload function in the 225 and 45 generalDirection cases to avoid the "kitty corner jump". 
*/


#include <iostream>
#include "Checkers.hpp"

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

static constexpr unsigned int DEBUG_CHECKERS = true; 


// Global.hpp
/*

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

// Checkers.hpp
class Checkers
{

Checkers::Checkers() {}

/* Start the Checkers game by creating a window, menu options, and performing a graphical animation on the window 
void Checkers::start()
{

}

/* load the fastest 10 scores from external file storage
void Checkers::loadTimes(sf::RenderWindow& window, sf::Event& event)
{
	const string RESOURCE_ERROR_SAV = "ERROR - cannot open \"game_times.sav\"";
	ifstream file("game_times.sav"); // open the file for reading
	if(!file)
		cerr << RESOURCE_ERROR_SAV << endl;
	else
	{
		int i = 0;
		string temp;
		vector<string> times;
		times.push_back(temp);
		while(i < 10 && file >> temp)
		{
			++i;
			temp += "\n"; // append a newline to the time
			times.push_back(temp);
			// cout << temp << endl;
		}			

		// load the text font
		sf::Font font;
		if(!font.loadFromFile("resources/ENGR.TTF"))
			cerr << "ERROR - cannot find resource file ENGR.TTF" << endl;
		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(WINDOW_WIDTH / 40);
		text.setPosition(0, WINDOW_HEIGHT / 40); // draw slightly below the title
		text.setColor(sf::Color::Black);
		// now sort the times in non-decreasing order
		sort(times.begin(), times.end());
		// reset temp
		temp = "Press ESC to go back to Main Menu!\n\n\n"
			"Top 10 Fastest Times:\n\n";
		vector<string>::const_iterator it;
		for(it = times.begin(); it != times.end(); ++it)
			temp += *it;
		// set the text to temp
		text.setString(temp);

		// this loop keeps the top 10 best scores on the window
		bool view = true;
		while(view)
		{
			while(window.pollEvent(event))
			{
				// go back to the main menu
				if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				{
					cout << endl << "Closing Fastest Times View. Returning to Main Menu." << endl;
					view = false;
				}
		
				// SFML drawing sequence
				window.clear(sf::Color::White);
				window.draw(text); // draw the times and header
				window.display();
			}
		}
	}
}

// create a fun little animation on the main menu (sort of like showing a game in progress) 

private:
	void loadTimes(sf::RenderWindow&, sf::Event&);
	void animation(sf::RenderWindow&, const int&, const int&)
 {
		// The animation is of a mock-checker game. It takes up roughly 3/4 the window 
	// reset startingX, startingY, and k
	int startingX = 0, startingY = WINDOW_HEIGHT / 4;
	sf::CircleShape circle (WINDOW_WIDTH / (2 * SQUARES_HORIZONTAL));
	circle.setPosition(WINDOW_WIDTH, 0); // get rid of the 0,0 default constructed circle
	sf::RectangleShape square (sf::Vector2f(static_cast<float>(XOFFSET), static_cast<float>(YOFFSET)));
	for(int i = 0; i < SQUARES_VERTICAL - 2; ++i)
	{	
		for(int j = 0; j < SQUARES_HORIZONTAL; ++j)
		{	
			// position the square
			square.setPosition(static_cast<float>(startingX), static_cast<float>(startingY));
			sf::RectangleShape temp (sf::Vector2f(static_cast<float>(XOFFSET / 3), static_cast<float>(YOFFSET / 3)));
			temp.setPosition(static_cast<float>(mouseOverX), static_cast<float>(mouseOverY));
			if(square.getGlobalBounds().intersects(temp.getGlobalBounds()))
			{
				// give the move to square a nice green highlight
				square.setOutlineThickness(-(SQUARES_VERTICAL / 2));
				square.setOutlineColor(sf::Color::Green);	
			}
			// create some random checkerpieces
			if(i <= 1 && !((j % 2 == 0 && i % 2 == 0) || (j % 2 != 0 && i % 2 != 0)))
			{
				circle.setFillColor(sf::Color::Blue);
				circle.setPosition(static_cast<float>(startingX), static_cast<float>(startingY));
			}
			else if(i >= 4 && !((j % 2 == 0 && i % 2 == 0) || (j % 2 != 0 && i % 2 != 0)))
			{
				circle.setFillColor(sf::Color::Red);
				circle.setPosition(static_cast<float>(startingX), static_cast<float>(startingY));
			}
			// this crazy looking if statement gives the checkered B/W pattern
			if((j % 2 == 0 && i % 2 == 0) || (j % 2 != 0 && i % 2 != 0))
				square.setFillColor(sf::Color::White); 
			else
				square.setFillColor(sf::Color::Black);
			window.draw(square);
			window.draw(circle);
			startingX += XOFFSET;
			// erase any previous color highlights from the
			square.setOutlineThickness(0);
			square.setOutlineColor(sf::Color(0, 0, 0, 255));
		}
		startingY += YOFFSET;
		startingX = 0;
	}

}
public:
	Checkers();

void start(  ) { // start the Main Menu window
}

};

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


*/


auto start_checkers = [] () -> void {
    using namespace std;	
    const string RESOURCE_ERROR_IMAGE = "ERROR - cannot open \"resources/Checkerboard_8x8_125px.png\"";
    const string RESOURCE_ERROR_FONT = "ERROR - cannot open \"resources/ENGR.TTF\"";
    const string TITLE = "_CHECKERS_";
    const string MAIN_MENU = "Press 'P' for Player verse Player\n"
		"Press 'C' for Players verses Computer\n"
		"Press 'R' for the Rules\nPress 'T' to View Top 10 Fastest Times";
    const string RULES = "See the Checker_Rules.txt files in resources.";
    const string VIEW_TIMES = "View Fastest Times (in minutes) selected, Main Screen";


auto menu_animation = [&] (sf::RenderWindow& window,
    const int& mouseOverX, const int& mouseOverY) {
    cout << "Animation Func, Line: " << __LINE__ << endl;
};

	// setup the window (give dimensions, followed by a window title, and the default format)
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, sf::Style::Default);
	window.setVerticalSyncEnabled(true);
	// the creation coordinates of the window context on the monitor
	window.setPosition(sf::Vector2i(WINDOW_POSITION_X, WINDOW_POSITION_Y));
	
	// load an image to use as an icon on the titlebar
	sf::Image image;
	if(!image.loadFromFile("resources/Checkerboard_8x8_125px.png"))
		cerr << RESOURCE_ERROR_IMAGE << endl;
	window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
	
	// load the text font
	sf::Font font;
	if(!font.loadFromFile("resources/ENGR.TTF"))
		cerr << RESOURCE_ERROR_FONT << endl;
	
	// initialize menu fonts
	sf::Text menuMessage, menuTitle;
	menuTitle.setFont(font);
	menuTitle.setCharacterSize(WINDOW_WIDTH / 40);
	menuTitle.setPosition(0, 0); // draw @ top-left corner of window
	menuTitle.setColor(sf::Color::Red);
	menuTitle.setString(TITLE);
	menuMessage.setFont(font);
	menuMessage.setCharacterSize(WINDOW_WIDTH / 40);
	menuMessage.setPosition(0, WINDOW_HEIGHT / 40); // draw slightly below the title
	menuMessage.setColor(sf::Color::Black);
	menuMessage.setString(MAIN_MENU);
	
	int mouseOverX = 0, mouseOverY = 0; // used to gather information about user's mouse coords on the window
	
	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if((event.type == sf::Event::Closed) 
				|| ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) // close SFML window
			{
				window.close();
			}
			else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::P)) // human verses human
			{
	//			CheckerGame checkerGame(window, true, true); // both players are human
	//			checkerGame.startCheckers(window, event);
			}
			else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::C)) // human verses computer
			{
	//			CheckerGame checkerGame(window, true, false); // only player 1 is human
	//			checkerGame.startCheckers(window, event);
			}
			else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::R)) // Rules is selected
			{
				cout << RULES << endl;
			}
			else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::T)) // view best scores is selected
			{
				cout << VIEW_TIMES << endl;
	//			loadTimes(window, event);
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				// saves these coordinates and draws a nice green or purple box around the closest moused over square on the checkerboard.
				mouseOverX = event.mouseMove.x;
				mouseOverY = event.mouseMove.y;
			}
		}

		// SFML window drawing sequence
		window.clear(sf::Color::White);
		window.draw(menuTitle);
		window.draw(menuMessage);
		menu_animation(window, mouseOverX, mouseOverY);
		window.display();
	} // window.isOpen()
}; // start_checkers



int main()
{
	using namespace std;


	Checkers checkers;


	cout << "Hello Checkers" << endl;

	return 0;
}
