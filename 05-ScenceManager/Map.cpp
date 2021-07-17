#include "Map.h"
#include "Textures.h"
#include "Utils.h"	
#include <fstream>
#include <iostream>
#define TILESET 32
#define TILESET16 16


CMap::CMap(int idTture, int colMap, int rMap, int colTileSet, int rTileSet, int maxTSet, int indexTile)
{
	this->idTextureOfMap = CTextures::GetInstance()->Get(idTture);
	this->columnMap = colMap;
	this->rowMap = rMap;
	this->columnTileSet = colTileSet;
	this->rowTileSet = rTileSet;
	this->maxTileSet = maxTSet;
	this->indexTileMap = indexTile;
}

CMap::~CMap()
{
}

void CMap::RenderMap(int sceneID)
{
	int tile = TILESET;
	for (int rowMapIndex = 0; rowMapIndex < rowMap; rowMapIndex++) {
		for (int columnMapIndex = 0; columnMapIndex < columnMap; columnMapIndex++)
		{
			float drawX = columnMapIndex * tile;
			float drawY = rowMapIndex * tile;
			spritesTiles[TiledMap[rowMapIndex][columnMapIndex] - indexTileMap]->Draw((float)drawX, (float)drawY, 255); //(x,y,alpha)
		}
	}
	
}
void CMap::LoadResourceTilesMap(LPCWSTR pathFileTileMap)
{
	ifstream fStream;

	// open file save position map
	fStream.open(pathFileTileMap);
	// new instance TiledMap
	this->TiledMap = new int* [rowMap];
	for (int i = 0; i < rowMap; i++)
	{
		TiledMap[i] = new int[columnMap];
		for (int j = 0; j < columnMap; j++)
		{
			fStream >> TiledMap[i][j];
		}
	}
	fStream.close();
}


void CMap::LoadTilesSet(int sceneID)
{
	int tile = TILESET;
	for (int i = 0; i < maxTileSet; i++) 
	{
		int left = i % columnTileSet * tile;
		int top = i / columnTileSet * tile;
		int right = left + tile;
		int bottom = top + tile;
		LPSPRITE spriteTile = new CSprite(i, left, top, right, bottom, idTextureOfMap);
		this->spritesTiles.push_back(spriteTile);
	}
}