#include "pch.h"
#include "Board.h"



Board::Board() {
	gameBoard = new Tile*[25];
	for (unsigned int i = 0; i < 25; i++) {
		gameBoard[i] = new Tile[16];
	}

	// initialize all tiles
	for (unsigned int y = 0; y < 16; y++) {
		for (unsigned int x = 0; x < 25; x++) {
			sf::Texture texture;
			Tile tile;
			tile.background.setPosition(sf::Vector2f(x * 32, y * 32));
			gameBoard[x][y] = tile;

		}
	}

}


Tile Board::GetTile(int x, int y) {
	return gameBoard[x][y];
}
