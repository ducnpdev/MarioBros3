#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include "Sprites.h"



class CMap {
	int** TiledMap;
	int columnMap;  // 
	int rowMap;
	int columnTileSet; 
	int rowTileSet;
	LPDIRECT3DTEXTURE9 idTextureOfMap; 
	vector<LPSPRITE> spritesTiles;
	int maxTileSet;
public:
	CMap(int idtextture, int columnMap, int rowMap, int columnTileSet, int rowTileSet, int maxTileSet);
	~CMap();
	void RenderMap(int sceneID);
	void LoadResourceTilesMap(LPCWSTR pathFile); // load index of tileset in texture
	void LoadTilesSet(int sceneID);

};