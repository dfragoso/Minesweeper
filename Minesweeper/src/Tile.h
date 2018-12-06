#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include<string>
#include <vector>
using namespace std;

struct Tile
{
	vector<Tile*> neighbors;
	

	unsigned int mineCount;
	bool isMine;
	bool isClicked;
	bool isFlag;
	bool visited;

	sf::Sprite background;
	sf::Sprite layer;
	/*sf::Sprite reavealedTile;
	sf::Sprite flag;
	sf::Sprite mine;*/

	//sf::Vector2f topLeft;
	//sf::Vector2f bottomRight;
	
	Tile();
};

