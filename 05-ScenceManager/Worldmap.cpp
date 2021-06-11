#include "Worldmap.h"
#include "Game.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"
#include "GameObject.h"
#include "BackgroundWorldmap.h"

CWorldMap::CWorldMap(int id, LPCWSTR filePath) : CScene(id, filePath)
{
	player = NULL;
	/*time = NULL;
	score = NULL;
	arrows = NULL;
	coinPlay = NULL;
	lives = NULL;
	sb = NULL;*/
	key_handler = new CWorldMapKeyHandler(this);
}

void CWorldMapKeyHandler::OnKeyDown(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	//CMarioWorldmap* marioWorldmap = ((CMarioWorldmap*)scence)->

		//((CMarioWorldmap*)scence)->GetPlayer();
}



void CWorldMap::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);

	}
}

void CWorldMap::Render()
{
	for (int i = objects.size() - 1; i >= 0; i--)
	{
		objects[i]->Render();
	}
}

void CWorldMap::Unload()
{
	for (unsigned int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();	
}

void CWorldMap::Load()
{
	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		/*if (line == "[NODES]") {
			section = SCENE_SECTION_NODES; continue;
		}*/
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			// case SCENE_SECTION_NODES: _ParseSection_NODES(line); break;
		}
	}
	f.close();
	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
}

void CWorldMap::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 5) return; // skip invalid lines
	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());
	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CWorldMap::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CWorldMap::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (unsigned int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CWorldMap::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (unsigned int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void CWorldMap::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;
	switch (object_type)
	{
	case OBJECT_TYPE_MARIOOVERWORLD:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMarioWorldmap();
		player = (CMarioWorldmap*)obj;
		break;
	case OBJECT_TYPE_WORLDMAP1:
		obj = new CMap1();
		break;
	case OBJECT_TYPE_SCOREBOARD:
		break;
	case OBJECT_TYPE_BACKGROUND:
		obj = new CBackgroundWorldmap();
		break;
	case OBJECT_TYPE_TURTLE:
		break;
	case OBJECT_TYPE_CARD:
		

		break;
	case OBJECT_TYPE_SCORE_BOARD:
	
		break;
	case OBJECT_TYPE_NUMBER:
	{
		
		break;
	}
	case OBJECT_TYPE_TIME:
		
		break;
	case OBJECT_TYPE_SCORE:
	
		break;
	case OBJECT_TYPE_ARROW:
	{
		break;
	}
	break;
	case OBJECT_TYPE_ARROWS:
		
		break;
	case OBJECT_TYPE_COINPLAY:
		
		break;
	case OBJECT_TYPE_LIVES:
	
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
	obj->SetPosition(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

	

void CWorldMapKeyHandler::OnKeyUp(int KeyCode)
{
}
void CWorldMapKeyHandler::KeyState(BYTE* states)
{
}