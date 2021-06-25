#include <iostream>
#include <fstream>

#include "PlayScenceConfig.h"
#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Road.h"
#include "Score.h"
#include "ListScore.h"
#include "ColorBrick.h"
#include "Hub.h"


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
		break;
	case OBJECT_TYPE_BRICK: {
		obj = new CBrick();
		break;
	}
	case OBJECT_TYPE_HUB: {
		obj = new CHub();
		hub = (CHub*)obj;
		hub->SetTimeHub(time);
		hub->SetCoinHub(coinPlay);
		hub->SetArrowHub(arrows);
		hub->SetLives(lives);
		hub->SetScore(score);
		hub->SetCardHub(cards);
		player->SetListArrow(arrows);
		player->SetCards(cards);
		break;
	}
	case OBJECT_TYPE_NUMBER: {
		int typetmp = atoi(tokens[4].c_str());
		obj = new CNumber();
		if (typetmp == 0) {
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

	case OBJECT_TYPE_HUB_COIN:
		obj = new CCoinPlay(numCoin);
		coinPlay = (CCoinPlay*)obj;
		break;
	case OBJECT_TYPE_HUB_TIME: {
		obj = new CTime(num);
		time = (CTime*)obj;
		break;
	}
	case OBJECT_TYPE_HUB_ARROW: // 
	{
		int type = atoi(tokens[4].c_str());
		obj = new CArrow(type);
		arrow.push_back((CArrow*)obj);
		break;
	}
	case OBJECT_TYPE_HUB_LIVES: 
		obj = new CLives(numLives);
		lives = (CLives*)obj;
		break;
	case OBJECT_TYPE_SCORE:
		obj = new CScore(numScore);
		score = (CScore*)obj;
		break;
	case OBJECT_TYPE_LIST_SCORE: 
		obj = new CListScore();
		for (int i = 0; i < 3; i++)
		{
			if (listScore[i] == NULL)
			{
				listScore[i] = (CListScore*)obj;
				listScore[i]->SetScoresPlay(score);
				player->CreateListScore(listScore[i]);
				break;
			}
		}
		break;
	case OBJECT_TYPE_HUB_CARD:
		obj = new CCard();
		{
			int type = atoi(tokens[4].c_str());
			if (type == 0)
			{
				for (int i = 0; i < MaxShowCards; i++)
				{
					if (cards[i] == NULL)
					{
						cards[i] = (CCard*)obj;
						break;
					}
				}
			}
			else if (type == 1)
			{
				cardT = (CCard*)obj;
			}
		}

		break;
	case OBJECT_TYPE_HUB_CARDTEXT: 
		obj = new CCardText(cardT);
		cardText = (CCardText*)obj;
		break;
	case OBJECT_TYPE_HUB_GOALCARDS:
		obj = new CGoalCard(cardText);
		break;
	case OBJECT_TYPE_HUB_ARROWS: 
		obj = new CListArrow(arrow);
		arrows = (CListArrow*)obj;
		break;
	case OBJECT_TYPE_TAIL:
		obj = new CTail();
		tail = (CTail*)obj;
		player->CreateTail(tail);
		break;
	case OBJECT_PIECE_BRICK:
		obj = new CPieceBrick();
		for (int i = 0; i < 16; i++)
		{
			if (pieceBrick[i] == NULL)
			{
				pieceBrick[i] = (CPieceBrick*)obj;
				break;
			}
		}
		break;
	default:
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
//	if (hub != NULL) {
		hub->SetCameraHub(camera);
//	}
}

void CPlayScene::initCard() {
	if (cards[0] == NULL) {
		return;
	}
	if (player->GetCard() == NULL) {
		player->SetCards(cards);
	}
}

void CPlayScene::Update(DWORD dt)
{
	if (id == 1)
	{
		if (player->GetMarioIsDie())	
		{
			player->SetMarioIsDie(false);
			CGame::GetInstance()->SetCamPos(0, 0);
			CGame::GetInstance()->SwitchScene(0);
		}
	}

	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	if (player == NULL) return;
	initCamera();

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	
	for (size_t i = 1; i < objects.size(); i++)
	{
		/*if (dynamic_cast<CKoopas*>(objects[i])) {
			float tempX, tempY;
			objects[i]->GetPosition(tempX, tempY);
			if (ObjectInUsing(tempX, tempY)) {
				objects[i]->SetVisible(false);
				objects.erase(objects.begin() + i);
			}
		}*/
		/*float Ox, Oy;
		objects[i]->GetPosition(Ox, Oy);
		if (!ObjectInUsing(Ox, Oy))
		{
			objects[i]->SetVisible(false);
			objects.erase(objects.begin() + i);
		}*/
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
	
	// TODO
   // initCard();
}

void CPlayScene::Render()
{
	// render tilemap 
	if (mapBackground) {
		mapBackground->RenderMap();
	}

	for (int i = objects.size() - 1; i >= 0; i--)
	{
		if (dynamic_cast<CColorBrick*>(objects[i])) objects[i]->Render();
	}

	for (int i = objects.size() - 1; i >= 0; i--)
	{
		if (!dynamic_cast<CPipe*>(objects[i]) && (!dynamic_cast<CColorBrick*>(objects[i]))
			&& (!dynamic_cast<CHub*>(objects[i])) && (!dynamic_cast<CNumber*>(objects[i])) && (!dynamic_cast<CCard*>(objects[i])) && (!dynamic_cast<CArrow*>(objects[i]))
			) 
			objects[i]->Render();
	}
	for (int i = objects.size() - 1; i >= 0; i--)
	{
		if (dynamic_cast<CPipe*>(objects[i]))
			objects[i]->Render();
	}
	for (int i = objects.size() - 1; i >= 0; i--)
	{
		if (dynamic_cast<CNumber*>(objects[i]) || dynamic_cast<CHub*>(objects[i]) || dynamic_cast<CCard*>(objects[i]) || dynamic_cast<CArrow*>(objects[i]))
			objects[i]->Render();
	}

}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (unsigned int i = 0; i < objects.size(); i++) {
		delete objects[i];
	}
	objects.clear();
	player = NULL;
	gridResource->GirdRemoveResource();
	gridResource = nullptr;
	delete gridResource;

	camera = NULL;
	num.clear();
	numScore.clear();
	numCoin.clear();
	numLives.clear();
	arrow.clear();

	for (int i = 0; i < MaxShowCards; i++)
		cards[i] = NULL;
	for (int i = 0; i < 16; i++)
		pieceBrick[i] = NULL;
	for (int i = 0; i < 3; i++)
		listScore[i] = NULL;
	
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetMarioIsDead()) return;

	switch (KeyCode)
	{
	case DIK_L:
		CGame::GetInstance()->SetCamPos(0, 0);
		CGame::GetInstance()->SwitchScene(0);
		break;
	case DIK_1:
		mario->marioSetUpDownLevel(LEVEL_MARIO_SMAIL);
		break;
	case DIK_2:
		mario->marioSetUpDownLevel(LEVEL_MARIO_BIG);
		break;
	case DIK_3:
		mario->marioSetUpDownLevel(LEVEL_MARIO_TAIL);
		break;
	case DIK_4:
		mario->marioSetUpDownLevel(LEVEL_MARIO_FIRE);
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
	case DIK_A:
		if (mario->GetMarioLevel() == LEVEL_MARIO_TAIL && !mario->GetMarioIsFight()
		 && GetTickCount64() - mario->GetTimeIsFight() > 380)
		{
			mario->SetTimeIsFight(GetTickCount64());
			mario->SetMarioIsFight(STATE_MARIO_FIGHT);
		}
		break;
	case DIK_S:
		// if(mario->GetMarioIsStandingFloor()) mario->SetState(STATE_MARIO_JUMP);
		if (mario->GetMarioIsJump() == 0) {
			mario->SetTimeJumpStart(GetTickCount64());
		//	mario->SetState(STATE_MARIO_JUMP);
		}

		if (mario->GetMarioLevel() == LEVEL_MARIO_TAIL)
		{
			if (mario->GetMarioPower())
			{
				mario->SetMarioFlyHighStart((DWORD)GetTickCount64());
				if (mario->nx > 0)
					mario->SetState(STATE_MARIO_FLYING_HIGH_RIGHT);
				else mario->SetState(STATE_MARIO_FLYING_HIGH_LEFT);
			}
			else if (mario->vy != 0 &&  mario->GetMarioIsJump() == -1)
			{
				mario->SetTimeJumpStartFlyLow(GetTickCount64());
				if (mario->nx > 0)
					mario->SetState(900);
				else mario->SetState(910);
			}
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
	case DIK_A:
		if (mario->GetMarioIsTortoiseshell())
		{
			mario->SetTimeStartKick(GetTickCount64());
			mario->SetState(STATE_MARIO_KICK);
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

	if (game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_DOWN)) {
		mario->SetTimeWalkingRight(GetTickCount64());
		if (GetTickCount64() - mario->GetTimeWalkingLeft() > 200) {

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

		mario->SetTimeWalkingLeft(GetTickCount64());
		if (GetTickCount64() - mario->GetTimeWalkingRight() > 200) {
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
		if (GetTickCount64() - mario->GetTimeJumpStart() < 250 )
		{

			// && mario->vy <= 0
			if (mario->GetMarioSpeechJump() < 0.2) {
				mario->SetMarioSpeechJump();
			}
		//	DebugOut(L"mario->vy 22 %f \n", mario->GetMarioSpeechJump());

			if (mario->GetMarioIsJump() != -1 && mario->vy <= 0.02 && !mario->GetMarioFallState()) {
				mario->SetState(STATE_MARIO_JUMP);
			}
		}
	}

}

void CPlayScene::CreatePieceBrick(float x, float y, DWORD t)
{
	int count = 0;
	for (int i = 0; i < 16; i++)
	{
		if (pieceBrick[i]->GetState() == 0 && count < 4)
		{
			count++;
			pieceBrick[i]->SetState(1);
			switch (count)
			{
			case 1:
				pieceBrick[i]->SetDisplay(x, y, -PIECE_BRICK_SPEED, -PIECE_BRICK_SPEED, t);
				break;
			case 2:
				pieceBrick[i]->SetDisplay(x + PIECE_BRICK_X_PLUS, y, PIECE_BRICK_SPEED, -PIECE_BRICK_SPEED, t);
				break;
			case 3:
				pieceBrick[i]->SetDisplay(x, y + PIECE_BRICK_X_PLUS, -PIECE_BRICK_SPEED, PIECE_BRICK_SPEED, t);
				break;
			case 4:
				pieceBrick[i]->SetDisplay(x + PIECE_BRICK_X_PLUS, y + PIECE_BRICK_X_PLUS, PIECE_BRICK_SPEED, PIECE_BRICK_SPEED, t);
				break;
			default:
				break;
			}

		}
	}
}