#pragma once
#include "CellResource.h"
#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Game.h"
#include "PlayScenceConfig.h"
#include "Textures.h"
#include "GridConfig.h"
#include "Goomba.h"
#include "Koopas.h"

class CGridResource {
	CCellResource** cellResource;


	void _ParseSection_Grid_ENEMIES(string line);
	void _ParseSection_Grid_ITEMS(string line);

public:
	CGridResource(LPCWSTR pathFileGrid);
	void GridLoadResource(LPCWSTR pathFileGrid);
	void GirdRemoveResource();
	void GirdPushResource(vector<LPGAMEOBJECT>& listResource, int Xcamera, int YCamera);
};
