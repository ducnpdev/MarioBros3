#include <iostream>
#include <fstream>

#include "PlayScenceConfig.h"
#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"


using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}


void CPlayScene::_ParseSection_TEXTURES(string line)
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

void CPlayScene::_ParseSection_SPRITES(string line)
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

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  
		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void CPlayScene::_ParseSection_MAPBACKGROUND(string line) {
	DebugOut(L"_ParseSection_MAPBACKGROUND \n");
	vector<string> tokens = split(line);

	if (tokens.size() < 7) return; // skip invalid lines

	int idTileSet = atoi(tokens[0].c_str());
	int totalRowsTileSet = atoi(tokens[1].c_str());
	int totalColumnsTileSet = atoi(tokens[2].c_str());
	int totalRowsMap = atoi(tokens[3].c_str());
	int totalColumnsMap = atoi(tokens[4].c_str());
	int totalTiles = atoi(tokens[5].c_str());
	wstring file_path = ToWSTR(tokens[6]);

	mapBackground = new CMap(idTileSet, totalRowsTileSet, totalColumnsTileSet, totalRowsMap, totalColumnsMap, totalTiles);
	mapBackground->LoadResourceTilesMap(file_path.c_str());
	mapBackground->LoadTilesSet();
}

void CPlayScene::_ParseSection_GRID_RESOURCE(string line)
{
	DebugOut(L"_ParseSection_GRID_RESOURCE: %S \n", line);
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring file_path = ToWSTR(tokens[0]);

	gridResource = new CGridResource(file_path.c_str());
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[MAPBACKGROUND]")  { section = SCENE_SECTION_MAPBACKGROUND; continue; }

		if (line == "[GRIDRESOURCE]") { section = SCENE_SECTION_GRID_RESOURCE; continue; }

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
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
			case SCENE_SECTION_MAPBACKGROUND: _ParseSection_MAPBACKGROUND(line); break;
			case SCENE_SECTION_GRID_RESOURCE: _ParseSection_GRID_RESOURCE(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	gridResource->GirdPushResource(objects,);
	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame *game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	CGame::GetInstance()->SetCamPos(cx, 250 /*cy*/);
}

void CPlayScene::Render()
{
	// render tilemap 
	mapBackground->RenderMap();

	// render all object in game
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetMarioIsDead()) return;

	switch (KeyCode)
	{
	case DIK_S:
		// if(mario->GetMarioIsStandingFloor()) mario->SetState(STATE_MARIO_JUMP);
		if (mario->GetMarioIsJump() == false) {
			DebugOut(L"start jump 111\n");
			mario->SetTimeJumpStart(GetTickCount64());
		//	mario->SetState(STATE_MARIO_JUMP);
		}
		break;
	case DIK_Q: 
		mario->Reset();
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode) 
{
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetMarioIsDead()) return;
	switch (KeyCode)
	{
	case DIK_DOWN:
		if(mario->GetMarioIsStateSitDown()){
			if (mario->GetMarioLevel() != LEVEL_MARIO_SMAIL) {
				mario->y -= NUMBER_AFTER_MARIO_SIT_DOWN;
				mario->SetMarioIsStateSitDown(false);
			}
		}
		break;
	
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	
	if(mario->GetMarioIsDead()) return; 

	// disable control key when Mario die 
	if (mario->GetState() == STATE_MARIO_DIE) return;

	if (game->IsKeyDown(DIK_DOWN)) {
		if (mario->GetState() == STATE_MARIO_IDLE)
			mario->SetState(STATE_MARIO_SITDOWN);
	}

	if (game->IsKeyDown(DIK_RIGHT)) {

		mario->SetTimeWalkingRight(GetTickCount());
		if (GetTickCount() - mario->GetTimeWalkingLeft() > 200) {

			if (game->IsKeyDown(DIK_A) || game->IsKeyDown(DIK_A) && game->IsKeyDown(DIK_S)) {
				if (mario->GetMarioPower()) mario->SetState(STATE_MARIO_RUNNING_FAST_RIGHT);
				else mario->SetState(STATE_MARIO_RUNNING_RIGHT);
			}
			else {
				mario->SetState(STATE_MARIO_WALKING_RIGHT);
			}

		}
		else {
			mario->SetState(STATE_MARIO_TURN_LEFT);
		}
		
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		mario->SetTimeWalkingLeft(GetTickCount());
		if (GetTickCount() - mario->GetTimeWalkingRight() > 200) {
			if(game->IsKeyDown(DIK_A)){

				mario->SetState(STATE_MARIO_RUNNING_LEFT);
			}else {
				mario->SetState(STATE_MARIO_WALKING_LEFT);
			}
		}
		else {
			mario->SetState( STATE_MARIO_TURN_RIGHT);
		}
	}
	else
		mario->SetState(STATE_MARIO_IDLE);
	
	if (game->IsKeyDown(DIK_S)) {
		if (GetTickCount() - mario->GetTimeJumpStart() < 380 )
		{
			if (mario->GetMarioIsJump() != 1)
				mario->SetState(STATE_MARIO_JUMP);
		}
	}

}