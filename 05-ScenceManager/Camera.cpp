﻿#include "Camera.h"
#include "Utils.h"

CCamera::CCamera(CMario* m, int id) {
	this->player = m;
	this->sceneID = id;
	cameraX = 0.0f;
	cameraY = 267.0f;
	playerX = 0.0f;
	playerY = 0.0f;
}

void CCamera::UpdateCamera() {
	
	switch (sceneID)
	{
	case 1:
		UpdateCameraScence1();
		break;
	case 2:
		UpdateCameraScence1();
		break;
	case 3:
		UpdateCameraScence3();
		break;
	case 4:
		UpdateCameraScence1();
		break;
	}
}

void CCamera::UpdateCameraScence1() {

	player->GetPosition(playerX, playerY);
	CGame* game = CGame::GetInstance();

	/*if (playerY < 100) {
		player->SetPosition(playerX, 0);
		SetPositionCamera(playerX, 0);
	}*/

	// Mario khi đi đến sát biên bên trái
	if (playerX < WIDTH_MIN_MAP_2) {
		player->SetPosition(CAM_X_MIN, playerY);
		SetPositionCamera(0, CAM_Y_MIN); 
	}

	// Mario khi đi đến sát biên bên phải 
	if (playerX > NEAR_WIDTH_LIMIT_MAP_1)
	{
		if (playerX > WIDTH_MAX_MAP_1) player->SetPosition(WIDTH_MAX_MAP_1, playerY);
		playerX = CAM_X_MAX;
		SetPositionCamera(playerX, CAM_Y_MIN); // CAM_Y_MIN
	}

	// mario khi đi đến giữa và ở dưới cống 
	else if (playerX > (game->GetScreenWidth() / 2) && playerY > UNDERGROUND_Y) {
		playerX -= game->GetScreenWidth() / 2;
		playerY -= game->GetScreenHeight() / 2;
		CGame::GetInstance()->SetCamPos(round(playerX), CAM_Y_UNDERGROUND);
		SetPositionCamera(round(playerX), CAM_Y_UNDERGROUND);
	}

	// Mario khi đi đến giữa màn hình game
	else if (playerX > (game->GetScreenWidth()/2)) {

		if (player->GetMarioLevel() == LEVEL_MARIO_TAIL)
		{
			playerX -= game->GetScreenWidth() / 2;
			playerY -= game->GetScreenHeight() / 2 + CAM_Y_PLUS;
			if (playerY < 150.0f) {
				SetPositionCamera(round(playerX), round(playerY));
				CGame::GetInstance()->SetCamPos(round(playerX), round(playerY));
			}
			else {
				SetPositionCamera(round(playerX), round(CAM_Y_MIN));
				CGame::GetInstance()->SetCamPos(round(playerX), round(CAM_Y_MIN));
			}
		}
		else {
			playerX -= game->GetScreenWidth() / 2;
			playerY -= game->GetScreenHeight() / 2;
			// thay đổi round(playerY) thành CAM_Y_MIN là fix cứng cameraY
			CGame::GetInstance()->SetCamPos(round(playerX), round(CAM_Y_MIN));
			SetPositionCamera(round(playerX), round(CAM_Y_MIN));
		}

	}

	// Mario khi mới bắt mở game
	else {
		if (player->GetMarioLevel() == LEVEL_MARIO_TAIL && playerY < game->GetScreenHeight() / 2)
		{
			playerY -= game->GetScreenHeight() / 2;
			CGame::GetInstance()->SetCamPos(0.0f, round(playerY));
			SetPositionCamera(0.0f, round(playerY));
		}
		else {
		// playerY -= game->GetScreenHeight() / 2 + 16;
			CGame::GetInstance()->SetCamPos(0.0f, round(CAM_Y_MIN));
			SetPositionCamera(0.0f, round(CAM_Y_MIN));
		}
	}

	playerY -= game->GetScreenHeight() / 2;
}

void CCamera::UpdateCameraScence2() {

}

void CCamera::UpdateCameraScence3() {
//	SetPositionCamera(100, 700);


	player->GetPosition(playerX, playerY);
	CGame* game = CGame::GetInstance();

	/*if (playerY < 100) {
		player->SetPosition(playerX, 0);
		SetPositionCamera(playerX, 0);
	}*/

	// Mario khi đi đến sát biên bên trái
	if (playerX < WIDTH_MIN_MAP_2) {
		player->SetPosition(CAM_X_MIN, playerY);
		SetPositionCamera(0, CAM_Y_MIN_SCENE_3);
	}

	// Mario khi đi đến sát biên bên phải 
	if (playerX > NEAR_WIDTH_LIMIT_MAP_1)
	{
		if (playerX > WIDTH_MAX_MAP_1) player->SetPosition(WIDTH_MAX_MAP_1, playerY);
		playerX = CAM_X_MAX;
		SetPositionCamera(playerX, CAM_Y_MIN_SCENE_3); // CAM_Y_MIN
	}

	// mario khi đi đến giữa và ở dưới cống 
	else if (playerX > (game->GetScreenWidth() / 2) && playerY > UNDERGROUND_Y) {
		playerX -= game->GetScreenWidth() / 2;
		playerY -= game->GetScreenHeight() / 2;
		CGame::GetInstance()->SetCamPos(round(playerX), CAM_Y_UNDERGROUND);
		SetPositionCamera(round(playerX), CAM_Y_UNDERGROUND);
	}

	// Mario khi đi đến giữa màn hình game
	else if (playerX > (game->GetScreenWidth() / 2)) {

		if (player->GetMarioLevel() == LEVEL_MARIO_TAIL)
		{
			playerX -= game->GetScreenWidth() / 2;
			playerY -= game->GetScreenHeight() / 2 + CAM_Y_PLUS;
			if (playerY < 150.0f) {
				SetPositionCamera(round(playerX), round(playerY));
				CGame::GetInstance()->SetCamPos(round(playerX), round(playerY));
			}
			else {
				SetPositionCamera(round(playerX), round(CAM_Y_MIN_SCENE_3));
				CGame::GetInstance()->SetCamPos(round(playerX), round(CAM_Y_MIN_SCENE_3));
			}
		}
		else {
			playerX -= game->GetScreenWidth() / 2;
			playerY -= game->GetScreenHeight() / 2;
			// thay đổi round(playerY) thành CAM_Y_MIN là fix cứng cameraY
			CGame::GetInstance()->SetCamPos(round(playerX), round(CAM_Y_MIN_SCENE_3));
			SetPositionCamera(round(playerX), round(CAM_Y_MIN_SCENE_3));
		}

	}

	// Mario khi mới bắt mở game
	else {
		if (player->GetMarioLevel() == LEVEL_MARIO_TAIL && playerY < game->GetScreenHeight() / 2)
		{
			playerY -= game->GetScreenHeight() / 2;
			CGame::GetInstance()->SetCamPos(0.0f, round(playerY));
			SetPositionCamera(0.0f, round(playerY));
		}
		else {
			float testX = 0;
			float testY = 0;

			// playerY -= game->GetScreenHeight() / 2 + 16;
			CGame::GetInstance()->SetCamPos(testX, round(testY));
			SetPositionCamera(testX, round(testY));
		}
	}

	playerY -= game->GetScreenHeight() / 2;
}

void CCamera::UpdateCameraScence4() {

}