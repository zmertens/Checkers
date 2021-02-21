#include "CheckerGame.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::ostringstream;

sf::Time CheckerGame::timeElapsed;
int CheckerGame::winner = 0; // no winner at initialization

CheckerGame::CheckerGame(sf::RenderWindow& window, const bool& isHuman1, const bool& isHuman2)
{
	isPlaying = false; // game loop boolean
	checkerboard = new Checkerboard(window); // create a new checkerboard
	SoundManager::getSoundManager()->addSound("../resources/step.ogg");
	SoundManager::getSoundManager()->addSound("../resources/big_wall_impact.ogg");
	SoundManager::getSoundManager()->addSound("../resources/slime_jump.ogg");
	SoundManager::getSoundManager()->addSound("../resources/bonus.ogg");
	p1 = new Player (1, isHuman1); // create player 1
	p2 = new Player (2, isHuman2); // create player 2
}

CheckerGame::~CheckerGame()
{
	// garbage collect pointers
	delete checkerboard;
	checkerboard = nullptr;
	delete p1;
	delete p2;
	p1 = nullptr;
	p2 = nullptr;
}

void CheckerGame::startCheckers(sf::RenderWindow& window, sf::Event& event)
{
	p1->setTurn(true); // p1 starts game 
	p2->setTurn(false);
	createTeams();
	isPlaying = true;
	// start the checker game loop
	gameLoop(window, event);
}

// checker game loop (game state: tied, p1 up, p2 up, p1 wins, p2 wins)
void CheckerGame::gameLoop(sf::RenderWindow& window, sf::Event& event)
{
	sf::Clock clock; // start the clock
	int currentX = 0, currentY = 0; // the current coordinates of a player's selection
	int futureX = 0, futureY = 0; // used after a player selects a checker and wants to move / jump
	int mouseOverX = 0, mouseOverY = 0; // used for creating the green and purple checker square highlighting
	bool selecting = true; // determines whether a player is selecting or moving / jumping a checker.
	bool checkDoubleJump = false; // this is used for determining multiple jump sequences
	int currentIndex = 0; // index of the current check being selected relative to a player's vector
	int deleteIndex = 0; // index of the current check being deleted relative to a player's vector
	int generalDirection = 0; // the general direction, determined by subtracting future from current distances in Moveable class
	Square* currentSquare = nullptr; // the current square selected
	Square* futureSquare = nullptr; // the future square selected
	Square* tempSquare = nullptr; // temp square can serve multiple roles, see the jumpBySquare / jumpByChecker methods in Moveable class.
	bool activePlayerIsHuman = isActivePlayerHuman(); // used to make sure only window events are processed if human player is active

	while(isPlaying) // gameplay continues until there is a winner, tie, or the window is terminated.
	{
		/* At the beginning of the turn, check to see if the player can move any checkers
		 NOT CURRENTLY TESTED */
		//if(playerCannotMove(p1, p2, checkerboard))
			//std::cout << "No moveable checkers." << std::endl;
/************************************************************* AI MOVEMENTS ******************************************/
		if(p2->getTurn() && !p2->getIsHuman()) // player 2 is AI and this is their turn
		{
			// AI for player 2
			AI ai;
			currentIndex = ai.getCurrentMoveIndex(p2, p1, checkerboard);
			if(currentIndex == -1) // we need to do this before we set coords (can't set coords to nullptr)
			{
				// game over for p2, no more moves available
				p2->deleteAllCheckers();
			}
			else // coords will be either of size 4 or 6
			{
				std::vector<int> coords;
				coords = *ai.AI_Move(p2, p1, checkerboard);
				std::cout << "(AI) P2 turn : currentIndex " << currentIndex << std::endl;
				std::cout << "(AI) P2 turn : coord size " << coords.size() << std::endl;
				if(coords.size() == 4) // only a move
				{
					currentX = coords[0];
					currentY = coords[1];
					mouseOverX = coords[2];
					mouseOverY = coords[3];
					// update variables, we use the moveable format
					currentSquare = checkerboard->findSquare(coords[0], coords[1]); 
					futureSquare = checkerboard->findSquare(coords[2], coords[3]); // the square to move to
					if(currentSquare != nullptr && futureSquare != nullptr)
					{
						currentSquare->setOccupied(false); // the square the active checker was sitting on
						futureSquare->setOccupied(true); // the square to move to
						p2->getChecker(currentIndex)->setPosition((float) coords[2], (float) coords[3]); // update current checker's position
						// check if we got kinged
						ifCheckerKinged(p2->getChecker(currentIndex), futureSquare);
						printChecker(p2->getChecker(currentIndex), "MOVING");
						changeTurn(); // change to player 1 turn
						selecting = true;
						SoundManager::getSoundManager()->playSound(SOUND_MOVE);
					}
				}
				else if(coords.size() == 6) // jumpByChecker
				{
					currentX = coords[0];
					currentY = coords[1];
					mouseOverX = coords[2];
					mouseOverY = coords[3];
					// update variables, we use the jumpByChecker format
					currentSquare = checkerboard->findSquare(coords[0], coords[1]); 
					futureSquare = checkerboard->findSquare(coords[2], coords[3]); // the square in the middle
					tempSquare = checkerboard->findSquare(coords[4], coords[5]); // the square to land on
					if(currentSquare != nullptr && futureSquare != nullptr && tempSquare != nullptr)
					{
						currentSquare->setOccupied(false); // the square the active checker was sitting on
						futureSquare->setOccupied(false); // the square to land on
						tempSquare->setOccupied(true); // the square that was jumped over
						p2->getChecker(currentIndex)->setPosition((float) coords[4], (float) coords[5]); // update current checker's position
						// check if we got kinged
						ifCheckerKinged(p2->getChecker(currentIndex), tempSquare);
						printChecker(p2->getChecker(currentIndex), "JUMPING");
						// delete the jumped checker (p2's turn, so delete from p1)
						deleteCheckerFromGame(p1, p1->findCheckerIndex(futureSquare));
						changeTurn(); // change to player 1 turn
						selecting = true;
						SoundManager::getSoundManager()->playSound(SOUND_JUMP_CHECKER);
					}
				}
			}
		}
		else if(!p2->getIsHuman() && !p1->getIsHuman()) // both players are computer
		{
			// both players use AI class
			// variable updates and checker deletions handled in player class 
			changeTurn(); // change to player 2 turn
			selecting = true;
		}
/*********************************************************** END OF AI MOVEMENTS ************************************/
		// for all window events, we assume that the active player is human-controlled.
		while(activePlayerIsHuman && window.pollEvent(event))
		{
			// terminates the application
			if(event.type == sf::Event::Closed)  
			{
				std::cout << std::endl << "Checkers Application Terminated." << std::endl;
				isPlaying = false;
				window.close();
			}
			// go back to the main menu (login screen)
			else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
			{
				std::cout << std::endl << "Terminating the game, returning to Main Menu." << std::endl;
				isPlaying = false;
			}
			else if (event.type == sf::Event::MouseButtonPressed) // handles game movements
			{
				if(event.mouseButton.button == sf::Mouse::Left) // select a checker to move
				{
					if(selecting) // player is selecting a checker to move
					{
						// get the coordinates from where the user clicked
						currentX = event.mouseButton.x;
						currentY = event.mouseButton.y;
						currentSquare = checkerboard->findSquare(currentX, currentY);
						/* The code for both players selection validation is symmetric, even though only player 1 has comments on the code. */
/********************************************************** PLAYER SELECTING ********************************************************************************/
						if(currentSquare != nullptr && currentSquare->getOccupied()) // validates selection
						{ 
							if(p1->getTurn()) // p1's turn 
							{
								currentIndex = p1->findCheckerIndex(currentSquare); 
								if(currentIndex != -1 && currentIndex < p1->getCounter())
								{
									printChecker(p1->getChecker(currentIndex), "SELECTED");
									// first check if current checker is jumping
									if(Moveable::hasJump(p1->getChecker(currentIndex), p2->getCheckersVector(), checkerboard)) 
									{
										// current checker has a jump so we know it's a valid selection
										selecting = false;
									}
									else
									{
										if(playerHasToJump(p1, p2))
											selecting = true; // player has a jump, force the player to select another checker
										else
											selecting = false; // no available jumps, player is ready to move their selected checker
									}
								}
								else
									selecting = true; // keep selecting (checker wasn't found, probably an error)
							}
							else // p2's turn
							{
								currentIndex = p2->findCheckerIndex(currentSquare); 
								if(currentIndex != -1 && currentIndex < p2->getCounter())
								{
									printChecker(p2->getChecker(currentIndex), "SELECTED");
									if(Moveable::hasJump(p2->getChecker(currentIndex), p1->getCheckersVector(), checkerboard)) 
									{
										selecting = false;
									}
									else
									{
										if(playerHasToJump(p2, p1))
											selecting = true; // player has to jump, force the player to select another checker
										else
											selecting = false; // player is ready to move their selected checker
									}
								}
								else
									selecting = true; // keep selecting (checker wasn't found)
							}
						}
						else // keep selecting, user selected an empty square 
							selecting = true;
/**************************************************** END OF PLAYERS SELECTING. *****************************************************************************/	
					}
					else if(!selecting) // a checkerpiece is moving or jumping
					{
						checkDoubleJump = false;
						/* Both players will have their movements analyzed to determine if it is a jump, movement, and so forth.
							The actual code for each player is symmetric, even though only player 1 has comments. */
						futureX = event.mouseButton.x;
						futureY = event.mouseButton.y;
						futureSquare = checkerboard->findSquare(futureX, futureY); // the square that the player wants to move to
						generalDirection = Moveable::findGeneralDirection(currentSquare, futureSquare); // finds the general direction (in degrees) with respect to future square selected
						std::cout << "Moving in the general Direction of: " << generalDirection << std::endl;
						if(generalDirection == -1)
						{
							std::cout << "Moving nowhere (no turn change). Picking another checker to move." << std::endl;
							tempSquare = nullptr;
						}
						else if(futureSquare != nullptr)
						{
							if(futureSquare->getOccupied()) // future square is occupied, so we might be jumping over a checker
								tempSquare = checkerboard->findJumpOntoSquare(futureSquare, generalDirection);
							else // futureSquare unnoccupied, Case 1) could be selecting the square to land on after jumping, or Case 2) simply moving
								tempSquare = checkerboard->findIntermSquare(futureSquare, generalDirection);
						}
						else
							std::cout << "The Future Square was not found." << std::endl;

/************************************************************* CODE FOR BOTH PLAYERS MOVEMENTS. *******************************************************/
						if(tempSquare != nullptr) // the only way tempSquare is null is when generalDirection == -1 or futureSquare isn't found.
						{
							if(futureSquare->getFillColor() == sf::Color::Black) // make sure the future square is moveable
							{
								if(p1->getTurn())
								{
									if(p1->getIsHuman())
									{
										// first check if player is selecting the checker to jump over (must call jumpByChecker() and jumpBySquare() before moveable())
										if(Moveable::jumpByChecker(p1->getCheckersVector(), currentSquare, futureSquare, tempSquare, currentIndex))
										{
											// update variables
											currentSquare->setOccupied(false);
											futureSquare->setOccupied(false); 
											tempSquare->setOccupied(true);
											// update the jumping checker's position
											p1->getChecker(currentIndex)->setPosition(tempSquare->getPosition().x, tempSquare->getPosition().y); 
											// check if the jumping checker became a king
											ifCheckerKinged(p1->getChecker(currentIndex), tempSquare);
											printChecker(p1->getChecker(currentIndex), "JUMPING");
											// delete the jumped checker (p1's turn, so delete from p2)
											deleteCheckerFromGame(p2, p2->findCheckerIndex(futureSquare));
											checkDoubleJump = true; // we just completed a jump, let's see if there's a double jump available
											changeTurn();
											SoundManager::getSoundManager()->playSound(SOUND_JUMP_CHECKER);
										}
										// Check if the player is jumping by selecting the square to land on after a jump (future square in this case), temp square is in the middle
										else if(Moveable::jumpBySquare(p1->getCheckersVector(), currentSquare, futureSquare, tempSquare, currentIndex)) 
										{
											// update variables after jumping
											futureSquare->setOccupied(true);
											currentSquare->setOccupied(false);
											tempSquare->setOccupied(false); // the jumped checker
											// update the jumping checker's position
											p1->getChecker(currentIndex)->setPosition(futureSquare->getPosition().x, futureSquare->getPosition().y); 
											// check if the jumping checker became a king
											ifCheckerKinged(p1->getChecker(currentIndex), futureSquare);
											printChecker(p1->getChecker(currentIndex), "JUMPING");	
											// delete the jumped checker (p1's turn, so delete from p2)
											deleteCheckerFromGame(p2, p2->findCheckerIndex(tempSquare));
											checkDoubleJump = true; // we just completed a jump, let's see if there's a double jump available
											changeTurn();
											SoundManager::getSoundManager()->playSound(SOUND_JUMP_SQUARE);
										}
										// finally check for moving w/o jump
										else if(Moveable::moveable(p1->getCheckersVector(), currentSquare, futureSquare, currentIndex) && !futureSquare->getOccupied()) 
										{
											/* This hasJump call is necessary to prevent a checker that is double jumping 
											from skipping a jump. This is because the boolean variable 'selecting' doesn't 
											go back to true during a double jump, which is where a validation would be done 
											to make sure that there are no pending jumps for the active checker. */
											if(!Moveable::hasJump(p1->getChecker(currentIndex), p2->getCheckersVector(), checkerboard))
											{
												// updates position for all the squares and checker(s) in play
												futureSquare->setOccupied(true);
												currentSquare->setOccupied(false);
												p1->getChecker(currentIndex)->setPosition(futureSquare->getPosition().x, futureSquare->getPosition().y);
												// check if the checker moved onto a king row
												ifCheckerKinged(p1->getChecker(currentIndex), futureSquare);
												printChecker(p1->getChecker(currentIndex), "MOVING");
												checkDoubleJump = false;
												changeTurn();
												SoundManager::getSoundManager()->playSound(SOUND_MOVE);
											}
										}

										// Now we need to figure out if the current check can double jump (this is only checked after a successful jump)
										if(checkDoubleJump && Moveable::hasJump(p1->getChecker(currentIndex), p2->getCheckersVector(), checkerboard))
										{
											// double jump available, we update the current square for the next iteration of the game loop
											currentX = (int) p1->getChecker(currentIndex)->getPosition().x;
											currentY = (int) p1->getChecker(currentIndex)->getPosition().y;
											currentSquare = checkerboard->findSquare(currentX, currentY);
											selecting = false; // technically selecting never changed, but we want to ensure that it's still false.
											/* we need to call changeTurn() again because it was already called in the first successful jump 
											(so effectively canceling the call altogther and thus it is still player 1's turn). */
											changeTurn();
											checkDoubleJump = false; // we just double jumped, so reset this to false.
											tempSquare = nullptr; // reset this to null for the next iteration of game loop
										}
										else
											selecting = true; // No double jump, turn has changed, go back to selecting
									}
								}
								else // p2->getTurn()
								{
									if(p2->getIsHuman())
									{
										if(Moveable::jumpByChecker(p2->getCheckersVector(), currentSquare, futureSquare, tempSquare, currentIndex))
										{
											currentSquare->setOccupied(false);
											futureSquare->setOccupied(false); 
											tempSquare->setOccupied(true);
											p2->getChecker(currentIndex)->setPosition(tempSquare->getPosition().x, tempSquare->getPosition().y); 
											ifCheckerKinged(p2->getChecker(currentIndex), tempSquare);
											printChecker(p2->getChecker(currentIndex), "JUMPING");
											deleteCheckerFromGame(p1, p1->findCheckerIndex(futureSquare));
											checkDoubleJump = true;
											changeTurn();
											SoundManager::getSoundManager()->playSound(SOUND_JUMP_CHECKER);
										}	
										else if(Moveable::jumpBySquare(p2->getCheckersVector(), currentSquare, futureSquare, tempSquare, currentIndex)) 
										{
											futureSquare->setOccupied(true);
											currentSquare->setOccupied(false);
											tempSquare->setOccupied(false); 
											p2->getChecker(currentIndex)->setPosition(futureSquare->getPosition().x, futureSquare->getPosition().y); 
											ifCheckerKinged(p2->getChecker(currentIndex), futureSquare);
											printChecker(p2->getChecker(currentIndex), "JUMPING");
											deleteCheckerFromGame(p1, p1->findCheckerIndex(tempSquare));
											checkDoubleJump = true;
											changeTurn();
											SoundManager::getSoundManager()->playSound(SOUND_JUMP_SQUARE);
										}
										else if(Moveable::moveable(p2->getCheckersVector(), currentSquare, futureSquare, currentIndex) && !futureSquare->getOccupied()) 
										{
											if(!Moveable::hasJump(p2->getChecker(currentIndex), p1->getCheckersVector(), checkerboard))
											{
												futureSquare->setOccupied(true);
												currentSquare->setOccupied(false);
												p2->getChecker(currentIndex)->setPosition(futureSquare->getPosition().x, futureSquare->getPosition().y);
												ifCheckerKinged(p2->getChecker(currentIndex), futureSquare);
												printChecker(p2->getChecker(currentIndex), "MOVING");
												checkDoubleJump = false;
												changeTurn();
												SoundManager::getSoundManager()->playSound(SOUND_MOVE);
											}
										}

										if(checkDoubleJump && Moveable::hasJump(p2->getChecker(currentIndex), p1->getCheckersVector(), checkerboard))
										{
											currentX = (int) p2->getChecker(currentIndex)->getPosition().x;
											currentY = (int) p2->getChecker(currentIndex)->getPosition().y;
											currentSquare = checkerboard->findSquare(currentX, currentY);
											selecting = false;
											changeTurn(); 
											checkDoubleJump = false;
											tempSquare = nullptr;
										}
										else
											selecting = true;
									}
								} // end of p2's turn
/************************************************* END OF MOVEMENT VALIDATION FOR EACH PLAYER *******************************************************/
							} // end of if(futureSquare->getColor() == sf::Color::Black)
						} // end of if(tempSquare != nullptr)
						else // this is equivalent to the player picking a checker to move and then changing their mind
						{
							// don't change the turn, but validate the current player, if they can't move any checkerpieces then they lose the game.
							std::cout << "Changing current checker. Code executed @ line " << __LINE__ << std::endl;
							selecting = true;
						}
					} // end of if(!selecting)
				} // end of if(event.mouseButton.button == sf::Mouse::Left)
			}
			/* Save the current mouse coords for the highlight graphic */
			else if (event.type == sf::Event::MouseMoved)
			{
				mouseOverX = event.mouseMove.x;
				mouseOverY = event.mouseMove.y;
			}
			else if(event.type == sf::Event::Resized)
			{
				std::cout << std::endl << "new window x: " << window.getSize().x << std::endl;
				std::cout << "new window y: " << window.getSize().y << std::endl << std::endl;
			}
		} // end of window events loop

		handleGameState(clock); // check the game state
		activePlayerIsHuman = isActivePlayerHuman(); // determine if the active player is human

		// SFML window drawing sequence
		window.clear();
		checkerboard->drawGrid(window, mouseOverX, mouseOverY, currentX, currentY, selecting);
		cpDrawer.drawCheckers(window, p1->getCheckersVector());
		cpDrawer.drawCheckers(window, p2->getCheckersVector());
		window.display();
	} // end of isPlaying
} // end of gameLoop

// change a player's turn
void CheckerGame::changeTurn()
{
	p1->setTurn(!p1->getTurn());
	p2->setTurn(!p2->getTurn());
}

// create the teams
void CheckerGame::createTeams()
{
	int startingX = 0;
	int startingY = 0;
	int radius = 0;
	int idNumber = 0;

	for(int i = 0; i < SQUARES_VERTICAL; ++i)
	{	
		for(int j = 0; j < SQUARES_HORIZONTAL; ++j)
		{	
			// check if the current square holds a checker
			if(checkerboard->findSquare(idNumber++)->getOccupied())
			{ 
				radius = SQUARES_VERTICAL / (2 * SQUARES_HORIZONTAL);
				if(i <= 2)
					p1->addChecker(new Checkerpiece(sf::Color::Red, radius, startingX, startingY, KING_ROW_7, i, p1->getNumber()));
				else
					p2->addChecker(new Checkerpiece(sf::Color::Blue, radius, startingX, startingY, KING_ROW_0, i, p2->getNumber()));
			}
			startingX += XOFFSET;
		}
		startingY += YOFFSET;
		startingX = 0;
	}
}

// checks to see if the player has other jumps
bool CheckerGame::playerHasToJump(Player*& active, Player*& notActive)
{
	/*	Iterate through every checker in the current player's vector,
	we need to make sure the player doesn't have any other available jumps 
	before they can move their selected checker. */
	for(int i = 0; i < active->getCounter(); ++i)
		if(Moveable::hasJump(active->getChecker(i), notActive->getCheckersVector(), checkerboard))
			return true; // player has to jump
	return false;
}

/* 
*	Determines whether the active player can move any of their checkers.
*	Uses an AI to find a current move for the player (-1 means their is no current move).
*/
bool CheckerGame::playerCannotMove(Player*& p1, Player*& p2, Checkerboard*& checkerboard)
{
	if(p1->getTurn() == true)
	{
		AI ai;
		if(ai.getCurrentMoveIndex(p1, p2, checkerboard) == -1)
		{
			p1->deleteAllCheckers();
			return true;
		}
		else
			return false;
	}
	else
	{
		AI ai;
		if(ai.getCurrentMoveIndex(p2, p1, checkerboard) == -1)
		{
			p2->deleteAllCheckers();
			return true;
		}
		else
			return false;
	}
}

// handles the game state, modifies the class variable 'isPlaying'
void CheckerGame::handleGameState(sf::Clock& clock)
{
	// tied
	if(p1->getCounter() == p2->getCounter()) 
	{
	}
	// player 2 wins
	else if(p1->getCounter() == 0) 
	{
		isPlaying = false;
		std::cout << std::endl << "Player Two Wins " << std::endl;
		winner = p2->getNumber();
		CheckerGame::timeElapsed = clock.restart(); // get the length of the game
		std::cout << "Time Elapsed (in minutes): " << (CheckerGame::timeElapsed.asSeconds() / 60) << std::endl;
		saveTime((CheckerGame::timeElapsed.asSeconds() / 60)); // save the time to external storage
	}
	// player 1 wins
	else if(p2->getCounter() == 0)
	{
		isPlaying = false;
		std::cout << std::endl << "Player One Wins " << std::endl;
		winner = p1->getNumber();
		CheckerGame::timeElapsed = clock.restart(); // get the length of the game
		std::cout << "Time Elapsed (in minutes): " << (CheckerGame::timeElapsed.asSeconds() / 60) << std::endl;
		saveTime((CheckerGame::timeElapsed.asSeconds() / 60)); // save the time to external storage
	}
	// p2 leads
	else if(p1->getCounter() < p2->getCounter()) 
	{
	
	}
	// p1 leads
	else if(p1->getCounter() > p2->getCounter())
	{

	}
}

// determine if the active player is a human
bool CheckerGame::isActivePlayerHuman()
{
	if(p1->getTurn() && p1->getIsHuman())
		return true;
	else if(p1->getTurn() && !p1->getIsHuman())
		return false;
	else if(p2->getTurn() && p2->getIsHuman())
		return true;
	else if(p2->getTurn() && !p2->getIsHuman())
		return false;
	else
		return false; // default false
}

// print the selected checker's credentials
void CheckerGame::printChecker(Checkerpiece* selected, const std::string& action)
{
	std::cout << action << " Checker (x, y): (" << selected->getPosition().x << ", " 
		<< selected->getPosition().y << ")" << " King: " 
			<< selected->getKing() << std::endl; 
}

// delete from the inactive player
void CheckerGame::deleteCheckerFromGame(Player* deleteFrom, const int& deleteIndex)
{
	// delete the jumped checker (deleteFrom is the inactive player who's checker got jumped)
	if(deleteIndex != -1 && deleteIndex < deleteFrom->getCounter())
	{
		std::cout << "DELETING Checker (x, y): (" << deleteFrom->getChecker(deleteIndex)->getPosition().x << ", " 
			<< deleteFrom->getChecker(deleteIndex)->getPosition().y 
				<< ")" << " King: " << deleteFrom->getChecker(deleteIndex)->getKing() << std::endl;
		deleteFrom->deleteChecker(deleteIndex);
	}
}

// check if the active checkerpiece became a king
void CheckerGame::ifCheckerKinged(Checkerpiece* checker, Square* square)
{
	if((square->getRow() == KING_ROW_0 && checker->getKingRow() == KING_ROW_0) 
		|| (square->getRow() == KING_ROW_7 && checker->getKingRow() == KING_ROW_7))
	{
		checker->setKing(true); // update the checker to be a king
		SoundManager::getSoundManager()->playSound(SOUND_KING);
	}
}

// save the time
void CheckerGame::saveTime(const double& time)
{
	std::cout << "Saving the time to game_times.sav" << std::endl;
	std::ofstream file("game_times.sav", std::ios::app); // name the file to save to (make sure to append)
	if(!file)
		std::cerr << "File won't open." << std::endl;
	else
	{
		file << time; // save the time to the file
		file << "\n"; // add a newline
		file.close(); // close the file
	}
}

/* Show the winner on the SFML window after game ends */
void CheckerGame::showWinner(sf::RenderWindow& window, sf::Event& event)
{
	ostringstream ostr; // create ostream
	ostr << "The\nwinner\nis\nPlayer " << winner << "\n"; // ostr takes in the text
	const string winnerText = ostr.str(); // convert ostr to string and save it
	// load the text font
	sf::Font font;
	if(!font.loadFromFile("resources/ENGR.TTF"))
		cerr << "ERROR - cannot load resources" << endl;
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(WINDOW_WIDTH / 8);
	text.setPosition(0, 0); // draw slightly below the title
	text.setColor(sf::Color::Black);
	text.setString(winnerText);

	// this loop keeps the top 10 best scores on the window
	bool view = true;
	while(view)
	{
		while(window.pollEvent(event))
		{
			// go back to the main menu
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
			{
				cout << endl << "Closing Winner View. Returning to Main Menu." << endl;
				view = false;
			}
		
			// SFML drawing sequence
			window.clear(sf::Color::White);
			window.draw(text); // draw the times and header
			window.display();
		}
	}
}