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
#include "Road.h"
#include "QuestionBrick.h"
#include "FirePiranhaPlant.h"
#include "FirePlantBullet.h"
#include "Brick.h"
#include "BoomerangConfig.h"
#include "Boomerang.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "GiftItem.h"
#include "UpVip.h"

class CGridResource {

	int numRow; 
	int numCol;
	int cellWidth;
	int cellHeight;
	CCellResource** cellResource;
	CBoomerang* boomerang[BOOMERANG_AMOUNT];
	CGameObject* listItemQuestionBrick[11];
	CQuestionBrick* questionBrick[11];
	
	CFirePlantBullet* firePlantBullet[2];
	CFirePiranhaPlant* firePiranhaPlant[2];
	CGameObject* itemBrick[ITEM_IN_BRICK_AMOUNT];
	// CBrick* brickcoli[BRICK_AMOUNT];
	CBrick* brick[BRICK_AMOUNT];

	void _ParseSection_Grid_ENEMIES(string line);
	void _ParseSection_Grid_ITEMS(string line);
	void _ParseSection_Grid_INITIAL(string line);
	void _ParseSection_ITEMS_QUESTION(string line);
	void _ParseSection_ITEMS_BRICK(string line);


	CMushroom* mushroomOfBrick;
	CLeaf* leafOfBrick;
	CGoombaMini* goombaMini[NUMBER_GOOMBA_MINI];

	CGiftItem* giftItem;
	CUpVip* upVip;

public:
	CGridResource(LPCWSTR pathFileGrid);
	void GridLoadResource(LPCWSTR pathFileGrid);
	void GirdRemoveResource();
	void GirdPushResource(vector<LPGAMEOBJECT>& listResource, int Xcamera, int YCamera);
};
