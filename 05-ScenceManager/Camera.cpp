#include "Camera.h"
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
		UpdateCameraScence1();
		break;
	case 4:
		UpdateCameraScence1();
		break;
	}
}

void CCamera::UpdateCameraScence1() {

	player->GetPosition(playerX, playerY);
	CGame* game = CGame::GetInstance();
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
	// Mario khi đi đến giữa màn hình game
	else if (playerX > (game->GetScreenHeight()/2 + 40)) {
		playerX -= game->GetScreenWidth() / 2;
		playerY -= game->GetScreenHeight() / 2 + 16;
		// thay đổi round(playerY) thành CAM_Y_MIN là fix cứng cameraY
		CGame::GetInstance()->SetCamPos(round(playerX), round(CAM_Y_MIN));
		SetPositionCamera(round(playerX), round(CAM_Y_MIN));
	}
	// Mario khi mới bắt mở game
	else {
		playerY -= game->GetScreenHeight() / 2 + 16;
		CGame::GetInstance()->SetCamPos(0.0f, round(CAM_Y_MIN));
		SetPositionCamera(0.0f, round(CAM_Y_MIN));
	}
	playerY -= game->GetScreenHeight() / 2;
}

void CCamera::UpdateCameraScence2() {

}

void CCamera::UpdateCameraScence3() {

}

void CCamera::UpdateCameraScence4() {

}