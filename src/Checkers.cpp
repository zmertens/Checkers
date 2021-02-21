/*
*	This is a checkers game programmed in C++ using SFML.
*	By Zach Mertens (https://github.com/zmertens, azzach19@yahoo.com)
*	Free software! GPL license.
*
*	This is the main program file for Checkers. It contains a command loop for the main menu.
*	The Main class handles menu selections such as 1) play, 2) view rules, 3) view best scores.
*	Menu options are selected by pressing the appropriate key on the keyboard.
*	There is a "game in progress" animation below the main menu options.
*	All graphics are drawn using SFML primitives (circles, rectangles, and basic polygons).
*/

#include "Checkers.hpp"
#include "CheckerGame.hpp"

using std::string;
using std::cerr;
using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::vector;
using std::sort;

Checkers::Checkers() {}

/* Start the Checkers game by creating a window, menu options, and performing a graphical animation on the window */
void Checkers::start()
{
	// constant strings
	const string RESOURCE_ERROR_IMAGE = "ERROR - cannot open \"resources/Checkerboard_8x8_125px.png\"";
	const string RESOURCE_ERROR_FONT = "ERROR - cannot open \"resources/ENGR.TTF\"";
	const string TITLE = "_CHECKERS_";
	const string MAIN_MENU = "Press 'P' for Player verse Player\n"
		"Press 'C' for Players verses Computer\n"
		"Press 'R' for the Rules\nPress 'T' to View Top 10 Fastest Times";
	const string RULES = "See the Checker_Rules.txt files in resources.";
	const string VIEW_TIMES = "View Fastest Times (in minutes) selected, Main Screen";

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
				CheckerGame checkerGame(window, true, true); // both players are human
				checkerGame.startCheckers(window, event);
			}
			else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::C)) // human verses computer
			{
				CheckerGame checkerGame(window, true, false); // only player 1 is human
				checkerGame.startCheckers(window, event);
			}
			else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::R)) // Rules is selected
			{
				cout << RULES << endl;
			}
			else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::T)) // view best scores is selected
			{
				cout << VIEW_TIMES << endl;
				loadTimes(window, event);
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
		animation(window, mouseOverX, mouseOverY);
		window.display();
	} // end of window.isOpen()
}

/* load the fastest 10 scores from external file storage */
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

/* create a fun little animation on the main menu (sort of like showing a game in progress) */
void Checkers::animation(sf::RenderWindow& window, const int& mouseOverX, const int& mouseOverY)
{ 
	/* The animation is of a mock-checker game. It takes up roughly 3/4 the window */
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