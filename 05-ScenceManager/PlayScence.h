#pragma once
#include <vector>
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Map.h"
#include "GridResource.h"
#include "Camera.h"
#include "Hub.h"
#include "Time.h"
#include "Arrow.h"
#include "Coin.h"
#include "Lives.h"
#include "CardText.h"
#include "GoldCard.h"


class CPlayScene: public CScene
{
protected: 
	// defination
	CMap* mapBackground;

	CGridResource* gridResource;

	vector<CNumber*> numCoin;
	vector<CNumber*> numScore; // time
	vector<CNumber*> num; // time
	vector<CNumber*> numLives; 
	vector<CArrow*> arrow;

	CHub* hub;
	CTime* time;
	CCoinPlay* coinPlay;
	CListArrow* arrows;
	CLives* lives;

	CCard* cards[3];
	CCard* cardT;
	CCardText* cardText;

	CMario *player;					// A play scene has to have player, right? 
	CCamera* camera;
	vector<LPGAMEOBJECT> objects;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAPBACKGROUND(string line);
	void _ParseSection_GRID_RESOURCE(string line);

	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void initCamera();
	bool ObjectInUsing(float x, float y);
	CMario * GetPlayer() { return player; } 

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

