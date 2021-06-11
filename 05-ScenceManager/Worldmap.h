#pragma once
#include "GameObject.h"
#include "Scence.h"
#include "Game.h"
#include "MarioWorldmap.h"
#include "Map1.h"


class CWorldMap : public CScene
{
	CMarioWorldmap* player;
	vector<LPGAMEOBJECT> objects;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
public:
	CWorldMap(int id, LPCWSTR filePath);
	CMarioWorldmap * GetPlayers() { return player; }	
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	friend class CWorldMapKeyHandler;
};

class CWorldMapKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CWorldMapKeyHandler(CScene* s) : CScenceKeyHandler(s) {};
};


#define SCENE_SECTION_UNKNOWN					-1
#define SCENE_SECTION_TEXTURES					2
#define SCENE_SECTION_SPRITES					3
#define SCENE_SECTION_ANIMATIONS				4
#define SCENE_SECTION_ANIMATION_SETS			5
#define SCENE_SECTION_OBJECTS					6
#define SCENE_SECTION_NODES						7
#define MAX_SCENE_LINE						1024



#define OBJECT_TYPE_WORLDMAP1				1
#define OBJECT_TYPE_SCOREBOARD				2
#define OBJECT_TYPE_BACKGROUND				3
#define OBJECT_TYPE_MARIOOVERWORLD			4
#define	OBJECT_TYPE_TURTLE					5

#define OBJECT_TYPE_SCORE_BOARD				21
#define OBJECT_TYPE_TIME					23
#define OBJECT_TYPE_NUMBER					24
#define OBJECT_TYPE_SCORE					26
#define OBJECT_TYPE_ARROW					27
#define OBJECT_TYPE_ARROWS					28
#define OBJECT_TYPE_COINPLAY				29
#define OBJECT_TYPE_LIVES					30
#define OBJECT_TYPE_CARD					34