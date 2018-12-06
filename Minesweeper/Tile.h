#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include<string>
using namespace std;

struct Tile
{
	Tile* topleft;
	Tile* top;
	Tile* topRight;
	Tile* left;
	Tile* right;
	Tile* bottomLeft;
	Tile* bottom;
	Tile* bottomright;

	unsigned int mineCount;
	bool isMine;
	bool isClicked;
	bool isFlag;

	sf::Sprite background;
	sf::Sprite layer;
	/*sf::Sprite reavealedTile;
	sf::Sprite flag;
	sf::Sprite mine;*/

	//sf::Vector2f topLeft;
	//sf::Vector2f bottomRight;
	
	Tile();
};

