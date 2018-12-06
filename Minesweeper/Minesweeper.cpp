#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Tile.h"
#include <random>
#include <ctime>

mt19937 random_mt;

unsigned int mineCount = 10;
unsigned int flagsLeft = mineCount;

int Random(int min, int max)
{
	uniform_int_distribution<int> dist(min, max);
	return dist(random_mt);
}
void CreateMines(Tile** &gameBoard) {
	
	unsigned int x = 0;
	unsigned int y = 0;
	for (unsigned int i = 0; i < mineCount; i++) {
		x = Random(0, 24);
		y = Random(0, 15);
		gameBoard[x][y].isMine = true;
	}
}

void InitTileNeighbors(Tile** &gameBoard) {
	for (unsigned int y = 0; y < 16; y++) {
		for (unsigned int x = 0; x < 25; x++) {
			Tile tile = gameBoard[x][y];
			int back = x - 1;
			int forward = x + 1;
			int up = y - 1;
			int down = y + 1;

			if (back >= 0) {
				gameBoard[x][y].neighbors.push_back(&gameBoard[back][y]);
				if (up >= 0) {
					
					gameBoard[x][y].neighbors.push_back(&gameBoard[back][up]);
				}

				if (down < 16) {
					gameBoard[x][y].neighbors.push_back(&gameBoard[back][down]);
				}
			}

			if (up >= 0) {
				gameBoard[x][y].neighbors.push_back(&gameBoard[x][up]);
			}

			if (down < 16) {
				gameBoard[x][y].neighbors.push_back(&gameBoard[x][down]);
			}

			if (forward < 25) {
				gameBoard[x][y].neighbors.push_back(&gameBoard[forward][y]);
				if (down < 16) {
					
					gameBoard[x][y].neighbors.push_back(&gameBoard[forward][down]);
				}
				if (up >= 0) {
					gameBoard[x][y].neighbors.push_back(&gameBoard[forward][up]);
				}

			}
		}
	}
}

void Cascade(Tile &tile ) {
	tile.isClicked = true;
	tile.visited = true;
	for (unsigned int i = 0; i < tile.neighbors.size(); i++) {
		if (tile.neighbors[i]->isMine) {
			tile.mineCount++;
		}
	}
	for (unsigned int i = 0; i < tile.neighbors.size(); i++) {
		if (tile.mineCount == 0 && !tile.neighbors[i]->visited && !tile.neighbors[i]->isMine && !tile.neighbors[i]->isFlag) {
			Cascade(*tile.neighbors[i]);
		}
	}
	


}

void LoadingTextures(string imageName, unordered_map<string, sf::Texture> &textures) {
	sf::Texture texture;
	if (!texture.loadFromFile("images\\" + imageName + ".png")) {
		throw "error";
	}
	textures.emplace(imageName, texture);
}



bool LeftClickEvent( Tile &tile) {
	if (!tile.isClicked) {
		if (tile.isMine) {
			
			return true;
		}
		Cascade(tile);
	}
	return false;
}

void DrawBoard(Tile** &gameBoard, unordered_map<string, sf::Texture> &textures, sf::RenderWindow &board, bool gameOver, bool debug, bool gameWon, sf::Sprite &face) {

	if(gameWon){
		face.setTexture(textures["face_win"]);
		board.draw(face);
	}
	else if (gameOver) {
		face.setTexture(textures["face_lose"]);
		board.draw(face);
	}
	else{
		face.setTexture(textures["face_happy"]);
		board.draw(face);
	}

	for (unsigned int y = 0; y < 16; y++) {
			for (unsigned int x = 0; x < 25; x++) {
				Tile tile = gameBoard[x][y];
				
				if(tile.isClicked) {
					tile.background.setTexture(textures["tile_revealed"]);
					board.draw(tile.background);
					if (tile.mineCount > 0) {
						tile.background.setTexture(textures["number_" + to_string(tile.mineCount)]);
						board.draw(tile.background);
					}

				}
				else {
					tile.background.setTexture(textures["tile_hidden"]);
					board.draw(tile.background);
					if (tile.isFlag) {
						tile.background.setTexture(textures["flag"]);
						board.draw(tile.background);
					}
				}
				if (gameOver || debug || gameWon) {
					if (tile.isMine) {
						tile.background.setTexture(textures["tile_revealed"]);
						board.draw(tile.background);
						tile.background.setTexture(textures["mine"]);
						board.draw(tile.background);

						
					}

				
				}		
				
			}
		}
}

void ResetGame(Tile** gameBoard) {
	for (unsigned int y = 0; y < 16; y++) {
		for (unsigned int x = 0; x < 25; x++) {
			sf::Texture texture;
			Tile tile;
			tile.background.setPosition(sf::Vector2f(x * 32, y * 32));
			gameBoard[x][y] = tile;

		}
	}
	CreateMines(gameBoard);
	InitTileNeighbors(gameBoard);
	mineCount = 10;
	flagsLeft = mineCount;

}

void DisplayCounter(sf::Sprite &digit_0, sf::Sprite &digit_1, sf::Sprite &digit_2, unordered_map<string, sf::Texture > &textures,sf::RenderWindow &board) {
	string countValue = to_string(flagsLeft);
	string digit0;
	string digit1;
	string digit2;

	if (countValue.size() > 2) {
		digit0.push_back(countValue[0]);
		digit1.push_back(countValue[1]);
		digit2.push_back(countValue[2]);

		digit_0.setTexture(textures["digit_" + digit0]);
		digit_1.setTexture(textures["digit_" + digit1]);
		digit_2.setTexture(textures["digit_" + digit2]);
		
		
	}
	else if (countValue.size() == 2) {
		digit1.push_back(countValue[0]);
		digit2.push_back(countValue[1]);

		digit_0.setTexture(textures["digit_0"]);
		digit_1.setTexture(textures["digit_" + digit1]);
		digit_2.setTexture(textures["digit_" + digit2]);
	
	}
	else if (countValue.size() == 1) {
		digit2.push_back(countValue[0]);

		digit_0.setTexture(textures["digit_0"]);
		digit_1.setTexture(textures["digit_0"]);
		digit_2.setTexture(textures["digit_" + digit2]);
	}

	board.draw(digit_0);
	board.draw(digit_1);
	board.draw(digit_2);
}

bool GameWon(Tile** &gameBoard) {
	unsigned int remaningSpaces = 0;
	unsigned int mines = 0;

	for (unsigned int y = 0; y < 16; y++) {
		for (unsigned int x = 0; x < 25; x++) {
			if (!gameBoard[x][y].isClicked) {
				remaningSpaces++;
			}
			if (gameBoard[x][y].isMine) {
				mines++;
			}
		}
	}

	
	if (remaningSpaces == mines) {
		return true;
	}
	else {
		return false;
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


	
	sf::Sprite face;
	face.setTexture(textures["face_happy"]);
	face.setPosition(sf::Vector2f(400, 512));

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
	sf::Sprite digit_1;
	sf::Sprite digit_2;
	digit_0.setTexture(textures["digit_0"]);
	digit_1.setTexture(textures["digit_5"]);
	digit_2.setTexture(textures["digit_0"]);

	digit_0.setPosition(sf::Vector2f(0, 512));
	digit_1.setPosition(sf::Vector2f(21, 512));
	digit_2.setPosition(sf::Vector2f(42, 512));
	
	
	
	//vector<vector<Tile*>> gameBoard;
	Tile** gameBoard = new Tile*[25];
	for (unsigned int i = 0; i < 25; i++) {
		gameBoard[i] = new Tile[16];
	}

	ResetGame(gameBoard);
	
	sf::RenderWindow board(sf::VideoMode(800, 600), "Minesweeper");
	bool gameOver = false;
	bool debugMode = false;
	bool gameWon = false;
	while (board.isOpen())
	{
		sf::Event event;
		board.clear(sf::Color::White);
		sf::Texture test;
		
		DisplayCounter(digit_0, digit_1, digit_2, textures, board);
		board.draw(debug);
		board.draw(test_1);
		board.draw(test_2);

		DrawBoard(gameBoard, textures, board, gameOver, debugMode, gameWon, face);
		
		
	
		
		while (board.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				board.close();
				gameOver = true;
			}
				
			if (event.type == sf::Event::MouseButtonPressed) {
				if (!gameOver) {
					if (event.mouseButton.x >= 0 && event.mouseButton.x <= 800 && event.mouseButton.y >= 0 && event.mouseButton.y <= 512) {
						if (event.mouseButton.button == sf::Mouse::Right) {
							if (gameBoard[event.mouseButton.x / 32][event.mouseButton.y / 32].isFlag) {
								gameBoard[event.mouseButton.x / 32][event.mouseButton.y / 32].isFlag = false;
								flagsLeft++;

								//increase the mine count
							}
							else {
								if (flagsLeft > 0) {
									gameBoard[event.mouseButton.x / 32][event.mouseButton.y / 32].isFlag = true;
									//Reduce the mineCount
									flagsLeft--;
								}
								
							
							}
						}
						if (event.mouseButton.button == sf::Mouse::Left) {
							if (!gameBoard[event.mouseButton.x / 32][event.mouseButton.y / 32].isFlag) {
								gameOver = LeftClickEvent(gameBoard[event.mouseButton.x / 32][event.mouseButton.y / 32]);
							}

						}
						gameWon = GameWon(gameBoard);
					}

					if (event.mouseButton.x >= 600 && event.mouseButton.x <= 664 && event.mouseButton.y >= 512 && event.mouseButton.y <= 576) {
						debugMode = !debugMode;
					}
				}

				if (event.mouseButton.button == sf::Mouse::Left) {
					if (event.mouseButton.x >= 400 && event.mouseButton.x <= 464 && event.mouseButton.y >= 512 && event.mouseButton.y <= 576) {
						ResetGame(gameBoard);
						gameOver = false;
						gameWon = false;
					}
					
				}

				
			}
		}


		board.display();
	}


	
	
	



	

	return 0;
}