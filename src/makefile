all: checkers

checkers: Main.o Checkers.o CheckerGame.o Checkerboard.o Checkerpiece.o Moveable.o AI.o Player.o Square.o SoundManager.o
	g++ -std=c++0x Main.o Checkers.o CheckerGame.o Checkerboard.o Checkerpiece.o Moveable.o AI.o Player.o Square.o SoundManager.o -o checkers -L /usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

Main.o: Main.cpp
	g++ -std=c++0x -c Main.cpp -I /usr/local/include/SFML-2.1/include

Checkers.o: Checkers.cpp
	g++ -std=c++0x -c Checkers.cpp -I /usr/local/include/SFML-2.1/include

Checkerboard.o: Checkerboard.cpp
	g++ -std=c++0x -c Checkerboard.cpp -I /usr/local/include/SFML-2.1/include
	
Checkerpiece.o: Checkerpiece.cpp
	g++ -std=c++0x -c Checkerpiece.cpp -I /usr/local/include/SFML-2.1/include
	
Moveable.o: Moveable.cpp
	g++ -std=c++0x -c Moveable.cpp -I /usr/local/include/SFML-2.1/include
	
AI.o: AI.cpp
	g++ -std=c++0x -c AI.cpp -I /usr/local/include/SFML-2.1/include
	
Player.o: Player.cpp
	g++ -std=c++0x -c Player.cpp -I /usr/local/include/SFML-2.1/include
	
Square.o: Square.cpp
	g++ -std=c++0x -c Square.cpp -I /usr/local/include/SFML-2.1/include
	
SoundManager.o: SoundManager.cpp
	g++ -std=c++0x -c SoundManager.cpp -I /usr/local/include/SFML-2.1/include
	
CheckerGame.o: CheckerGame.cpp
	g++ -std=c++0x -c CheckerGame.cpp -I /usr/local/include/SFML-2.1/include

clean:
	rm -rf *o checkers
