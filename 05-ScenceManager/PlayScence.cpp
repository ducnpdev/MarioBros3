#include <iostream>
#include <fstream>

#include "PlayScenceConfig.h"
#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Road.h"



using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

bool CPlayScene::ObjectInUsing(float x, float y) {
	float CamX, CamY;
	CamX = camera->GetPositionCameraX();
	CamY = camera->GetPositionCameraX();

	/*if (((CamX - 10 < x) && (x < CamX + 320)) && ((CamY < y) && (y < CamY + 320)))
		return true;
	return false;*/

	if ((x > CamX - 200) && (x < CamY + 200) && (y < CamY + 200) && (y > CamY - 200)){
		return true;
	}

	return false;
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

	if (tokens.size() < 3) return; 

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)
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

	if (tokens.size() < 2) return;

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

	if (tokens.size() < 3) return; 

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
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  
		DebugOut(L"load class mario \n");
		break;
	case OBJECT_TYPE_BRICK: 
		obj = new CBrick(); 
		break;
	
	case OBJECT_TYPE_NUMBER: {
		int typetmp = atoi(tokens[4].c_str());
		obj = new CNumber();
		if (typetmp == 0) {
			// num is time
			num.push_back((CNumber*)obj);
		}
		if (typetmp == 1) {
			numScore.push_back((CNumber*)obj);
		}
		if (typetmp == 2) {
			numCoin.push_back((CNumber*)obj);
		}
		if (typetmp == 3) {
			numLives.push_back((CNumber*)obj);
		}
		break;
	}
	case OBJECT_TYPE_HUB: {

		obj = new CHub();
		hub = (CHub*)obj;
		hub->SetTimeHub(time);
	//	hub->SetCoinHub(coinPlay);
		//hub->SetArrowHub(arrows);
		//hub->SetCardHub(cards);
		break;
	}
	case OBJECT_TYPE_HUB_TIME: {
		obj = new CTime(num);
		time = (CTime*)obj;
		break;
	}
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
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring file_path = ToWSTR(tokens[0]);
	gridResource = new CGridResource(file_path.c_str());
}

void CPlayScene::Load()
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

		if (line == "[MAPBACKGROUND]")  { 
			section = SCENE_SECTION_MAPBACKGROUND; continue; }

		if (line == "[GRIDRESOURCE]") { 
			section = SCENE_SECTION_GRID_RESOURCE; continue; }

		if (line == "[TEXTURES]") { 
			section = SCENE_SECTION_TEXTURES; continue; }
		
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
	
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
	
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue; }
	
		if (line[0] == '[') { 
			section = SCENE_SECTION_UNKNOWN; continue; }	

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

}

void CPlayScene::initCamera() {
	if (camera != NULL) {
		return;
	}
	camera = new CCamera(player, id);

	// hub = new CHub();
	if (hub != NULL) {

		hub->SetCameraHub(camera);
	}
	else {
		DebugOut(L"123123 \n");
	}
}

void CPlayScene::Update(DWORD dt)
{
	initCamera();
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	for (size_t i = 1; i < objects.size(); i++)
	{
		if (dynamic_cast<CKoopas*>(objects[i])) {
			float tempX, tempY;
			objects[i]->GetPosition(tempX, tempY);
			if (ObjectInUsing(tempX, tempY)) {
				objects[i]->SetVisible(false);
				objects.erase(objects.begin() + i);
			}
		}
		
	}

	objects[0]->Update(dt, &objects);
	camera->UpdateCamera();

	int Xcam = (int)camera->GetPositionCameraX();
	int Ycam = (int)camera->GetPositionCameraY();

	gridResource->GirdPushResource(objects, Xcam, Ycam);
	 for (size_t i = 0; i < objects.size(); i++)
	 {
		 objects[i]->Update(dt, &coObjects);
	 }
	
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

}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetMarioIsDead()) return;

	switch (KeyCode)
	{
	case DIK_1:
		mario->SetLevel(LEVEL_MARIO_SMAIL);
		break;
	case DIK_2:
		mario->SetLevel(LEVEL_MARIO_BIG);
		break;
	case DIK_3:
		mario->SetLevel(LEVEL_MARIO_TAIL);
		break;
	case DIK_4:
		mario->SetLevel(LEVEL_MARIO_FIRE);
		break;
	case DIK_5:
		break;
	case DIK_6:
		mario->SetPosition(50, 384);
		break;
	case DIK_7:
		mario->SetPosition(1152, 384);
		break;
	case DIK_8:
		mario->SetPosition(2357, 80);
		break;
	case DIK_S:
		// if(mario->GetMarioIsStandingFloor()) mario->SetState(STATE_MARIO_JUMP);
		if (mario->GetMarioIsJump() == 0) {
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
	case DIK_DOWN: {

		if(mario->GetMarioIsStateSitDown()){
			if (mario->GetMarioLevel() != LEVEL_MARIO_SMAIL) {
				mario->y -= NUMBER_AFTER_MARIO_SIT_DOWN;
				mario->SetMarioIsStateSitDown(false);
			}
		}
		break;
	}
	case DIK_S: {
		mario->SetMarioIsJump(-1);
		break;

	}
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

	if (game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_DOWN)) {
		//DebugOut(L"DIK_RIGHT \n");
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
	else if (game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_DOWN) ) {
		//DebugOut(L"DIK_LEFT \n");

		mario->SetTimeWalkingLeft(GetTickCount());
		if (GetTickCount() - mario->GetTimeWalkingRight() > 200) {
			if(game->IsKeyDown(DIK_A) || game->IsKeyDown(DIK_A) && game->IsKeyDown(DIK_S)){
				mario->SetState(STATE_MARIO_RUNNING_LEFT);
			}else {
				mario->SetState(STATE_MARIO_WALKING_LEFT);
			}
		}
		else {
			mario->SetState( STATE_MARIO_TURN_RIGHT);
		}
	}
	else {
	//	DebugOut(L"STATE_MARIO_IDLE \n");

		mario->SetState(STATE_MARIO_IDLE);
	}
	
	if (game->IsKeyDown(DIK_S)) {
		if (GetTickCount() - mario->GetTimeJumpStart() < 250 )
		{

			// && mario->vy <= 0
			if (mario->GetMarioSpeechJump() < 0.2) {
				mario->SetMarioSpeechJump();
			}
		//	DebugOut(L"mario->vy 22 %f \n", mario->GetMarioSpeechJump());

			if (mario->GetMarioIsJump() != -1 && mario->vy <= 0.02) {
				mario->SetState(STATE_MARIO_JUMP);
			}
		}
	}

}