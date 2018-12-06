#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Tile.h"
#include <random>
#include <ctime>

mt19937 random_mt;

int Random(int min, int max)
{
	uniform_int_distribution<int> dist(min, max);
	return dist(random_mt);
}
void CreateMines(Tile** &gameBoard) {
	unsigned int mineCount = 50;
	unsigned int x = 0;
	unsigned int y = 0;
	while (mineCount > 0) {
		x = Random(0, 24);
		y = Random(0, 15);
		gameBoard[x][y].isMine = true;
		mineCount--;
	}
}
//void RevealMines(Tile** &gameBoard, sf::RenderWindow &board) {
//	for (unsigned int y = 0; y < 512; y++) {
//		for (unsigned int x = 0; x < 800; x++) {
//			if (gameBoard[x / 32][y / 32].isMine) {
//				board.draw(gameBoard[x / 32][y / 32].reavealedTile);
//				board.draw(gameBoard[x / 32][y / 32].mine);
//			}
//		}
//	}
//}
void Cascade(Tile** &gameBoard, unsigned int x, unsigned int y, sf::RenderWindow &board) {
	if (x == 0 && y == 0) {
		gameBoard[x][y].right = &gameBoard[x + 1][y];
		gameBoard[x][y].bottomright = &gameBoard[x + 1][y + 1];
		gameBoard[x][y].bottom = &gameBoard[x][y + 1];

	}
	else if (x == 25 && y == 0) {
		gameBoard[x][y].left = &gameBoard[x - 1][y];
		gameBoard[x][y].bottomLeft = &gameBoard[x - 1][y + 1];
		gameBoard[x][y].bottom = &gameBoard[x][y + 1];
	}
	else if (x == 0 && y == 16) {
		gameBoard[x][y].top = &gameBoard[x][y - 1];
		gameBoard[x][y].topRight = &gameBoard[x + 1][y - 1];
		gameBoard[x][y].right = &gameBoard[x + 1][y];
	}
	else if (x == 25 && y == 16) {
		gameBoard[x][y].top = &gameBoard[x][y - 1];
		gameBoard[x][y].topleft = &gameBoard[x - 1][y - 1];
		gameBoard[x][y].left = &gameBoard[x - 1][y];
	}
	else if (x == 0 && y != 0) {
		gameBoard[x][y].top = &gameBoard[x][y - 1];
		gameBoard[x][y].topRight = &gameBoard[x + 1][y - 1];

	}
	else if (x == 25 && y != 0) {
		gameBoard[x][y].top = &gameBoard[x][y - 1];
		gameBoard[x][y].topleft = &gameBoard[x - 1][y - 1];
	}
	else if (x == 25 && y != 16) {

	}
}

void LoadingTextures(string imageName, unordered_map<string, sf::Texture> &textures) {
	sf::Texture texture;
	if (!texture.loadFromFile("images\\" + imageName + ".png")) {
		throw "error";
	}
	textures.emplace(imageName, texture);
}


// TODO: put lose face
void GameOver(Tile** &gameBoard, sf::RenderWindow &board, unordered_map<string, sf::Texture> &textures) {
	for (unsigned int y = 0; y < 16; y++) {
		for (unsigned int x = 0; x < 25; x++) {

			if (gameBoard[x][y].isMine) {
				gameBoard[x][y].background.setTexture(textures["tile_revealed"]);
				gameBoard[x][y].layer.setTexture(textures["mine"]);
			}

		}
	}
}

void LeftClickEvent(Tile** &gameBoard, Tile &tile, unordered_map<string, sf::Texture> &textures, sf::RenderWindow &board) {
	if (!tile.isClicked) {
		if (tile.isMine) {
			GameOver(gameBoard, board, textures);
			return;
		}
		tile.background.setTexture(textures["tile_revealed"]);
	}
}

void DrawBoard(Tile** &gameBoard, unordered_map<string, sf::Texture> &textures, sf::RenderWindow &board) {
	for (unsigned int y = 0; y < 16; y++) {
			for (unsigned int x = 0; x < 25; x++) {
	
				if (gameBoard[x][y].isClicked) {
					gameBoard[x][y].background.setTexture(textures["tile_revealed"]);
					if (gameBoard[x][y].mineCount > 0) {
						gameBoard[x][y].background.setTexture(textures["number_"+to_string(gameBoard[x][y].mineCount)]);
					}
					
				}
				else {
					gameBoard[x][y].background.setTexture(textures["tile_hidden"]);
					if (gameBoard[x][y].isFlag) {
						gameBoard[x][y].background.setTexture(textures["flag"]);
					}
				}
				
			
				/*if (gameBoard[x][y].isFlag) {
					board.draw(gameBoard[x][y].flag);

				}
				if (gameBoard[x][y].isClicked){
					if (!gameBoard[x][y].isMine) {
						board.draw(gameBoard[x][y].reavealedTile);
					}
					else {
						RevealMines(gameBoard, board);
					}
				}*/
			}
		}
}

int main()
{
	unordered_map<string, sf::Texture> textures;
	LoadingTextures("tile_hidden", textures);
	LoadingTextures("debug", textures);
	LoadingTextures("face_happy", textures);
	LoadingTextures("face_lose", textures);
	LoadingTextures("face_win", textures);
	LoadingTextures("flag", textures);
	LoadingTextures("mine", textures);
	for (unsigned int i = 1; i <= 8; i++) {
		LoadingTextures("number_" + to_string(i), textures);
	}
	LoadingTextures("test_1", textures);
	LoadingTextures("test_2", textures);
	LoadingTextures("tile_revealed", textures);

	for (unsigned int i = 0; i < 10; i++) {
		sf::Texture texture;
		if (!texture.loadFromFile("images\\digits.png", sf::IntRect(i * 21, 0, 21, 32))) {
			throw "error";
		}
		textures.emplace("digit_" + to_string(i), texture);
	}


	
	sf::Sprite face_happy;
	face_happy.setTexture(textures["face_happy"]);
	face_happy.setPosition(sf::Vector2f(400, 512));

	sf::Sprite face_lose;
	face_lose.setTexture(textures["face_lose"]);
	face_lose.setPosition(sf::Vector2f(400, 512));

	sf::Sprite face_win;
	face_win.setTexture(textures["face_win"]);
	face_win.setPosition(sf::Vector2f(400, 512));

	sf::Sprite test_1;
	test_1.setTexture(textures["test_1"]);
	test_1.setPosition(sf::Vector2f(664, 512));

	sf::Sprite test_2;
	test_2.setTexture(textures["test_2"]);
	test_2.setPosition(sf::Vector2f(728, 512));

	sf::Sprite debug;
	debug.setTexture(textures["debug"]);
	debug.setPosition(sf::Vector2f(600, 512));

	sf::Sprite digit_0;
	sf::Sprite digit_2;
	sf::Sprite digit_3;
	sf::Sprite digit_1;
	sf::Sprite digit_4;
	sf::Sprite digit_5;
	sf::Sprite digit_6;
	sf::Sprite digit_7;
	sf::Sprite digit_8;
	sf::Sprite digit_9;

	
	
	//vector<vector<Tile*>> gameBoard;
	Tile** gameBoard = new Tile*[25];
	for (unsigned int i = 0; i < 25; i++) {
		gameBoard[i] = new Tile[16];
	}

	for (unsigned int y = 0; y < 16; y++) {
		for (unsigned int x = 0; x < 25; x++) {
			sf::Texture texture;
			Tile tile;
			tile.background.setTexture(textures["tile_hidden"]);
			tile.background.setPosition(sf::Vector2f(x * 32, y * 32));
			tile.layer.setPosition(sf::Vector2f(x * 32, y * 32));
			/*tile.reavealedTile.setTexture(textures["tile_revealed"]);
			tile.reavealedTile.setPosition(sf::Vector2f(x * 32, y * 32));
			tile.flag.setTexture(textures["flag"]);
			tile.flag.setPosition(sf::Vector2f(x * 32, y * 32));
			tile.mine.setTexture(textures["mine"]);
			tile.mine.setPosition(sf::Vector2f(x * 32, y * 32));*/
			gameBoard[x][y] = tile;

		}
	}
	
	CreateMines(gameBoard);
	sf::RenderWindow board(sf::VideoMode(800, 600), "Minesweeper");
	while (board.isOpen())
	{
		sf::Event event;
		board.clear(sf::Color::White);
		sf::Texture test;
		
		
		DrawBoard(gameBoard, textures,board);
		
		board.draw(face_happy);
		board.draw(debug);
		board.draw(test_1);
		board.draw(test_2);
		
		while (board.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				board.close();
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Right) {
					if (gameBoard[event.mouseButton.x / 32][event.mouseButton.y / 32].isFlag) {
						gameBoard[event.mouseButton.x / 32][event.mouseButton.y / 32].isFlag = false;

						//increase the mine count
					}
					else {
						gameBoard[event.mouseButton.x / 32][event.mouseButton.y / 32].isFlag = true;
						gameBoard[event.mouseButton.x / 32][event.mouseButton.y / 32].background.setTexture(textures["flag"]);
						//Reduce the mineCount
					}					
				}
				if (event.mouseButton.button == sf::Mouse::Left) {
					//gameBoard[event.mouseButton.x / 32][event.mouseButton.y / 32].isClicked = true;
					LeftClickEvent(gameBoard, gameBoard[event.mouseButton.x / 32][event.mouseButton.y / 32], textures, board);
					
				}
			}
		}

		
		
		//board.draw(temp.sprite);

		board.display();
	}


	
	
	



	

	return 0;
}