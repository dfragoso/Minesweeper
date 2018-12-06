#pragma once
#include <vector>
#include "Tile.h"



struct Board
{
	Tile** gameBoard;
	sf::Sprite face;
	sf::Sprite test1;
	sf::Sprite test2;
	sf::Sprite debug;

	unsigned int minesLeft;

	Board();

	Tile GetTile(int x, int y);
	void CreateMines(); //random default
	void CreateMines(string testName);//for the test
	void SetPositions(); //set positions for mines
	
	 
	//bool test1;
	//bool test2; //Which test do you click on the window



}; 


