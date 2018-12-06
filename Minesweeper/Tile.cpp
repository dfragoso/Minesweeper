#include "pch.h"
#include "Tile.h"


Tile::Tile()
{
	top = nullptr;
	topleft = nullptr;
	topRight = nullptr;
	left = nullptr;
	right = nullptr;
	bottom = nullptr;
	bottomLeft = nullptr;
	bottomright = nullptr;
	isFlag = false;
	isMine = false;
	isClicked = false;
}

