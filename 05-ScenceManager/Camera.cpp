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
	// DebugOut(L"playX %f \n", playerX);
	// DebugOut(L"playY %f \n", playerY);

	CGame* game = CGame::GetInstance();
	
	// Mario khi đi đến sát biên bên trái
	if (playerX < 0) {
		player->SetPosition(CAM_X_MIN, playerY);
		SetPositionCamera(0, 267.0f);
	}
	// Mario khi đi đến sát biên bên phải 
	if (playerX > 2656)
	{
		if (playerX > 2816) player->SetPosition(2816, playerY);
		playerX = 2504;
		SetPositionCamera(playerX, 2504);
	}
	// Mario khi đi đến giữa màn hình game
	else if (playerX > (game->GetScreenHeight()/2 + 40)) {
		playerX -= game->GetScreenWidth() / 2;
		playerY -= game->GetScreenHeight() / 2 + 16;
		// thay đổi round(playerY) thành CAM_Y_MIN là fix cứng cameraY
		CGame::GetInstance()->SetCamPos(round(playerX), round(playerY));
		SetPositionCamera(round(playerX), round(playerY));
	}
	// Mario khi mới bắt mở game
	else {
		playerY -= game->GetScreenHeight() / 2 + 16;
		CGame::GetInstance()->SetCamPos(0.0f, round(playerY));
		SetPositionCamera(0.0f, playerY);
	}
	playerY -= game->GetScreenHeight() / 2;
}

void CCamera::UpdateCameraScence2() {

}

void CCamera::UpdateCameraScence3() {

}

void CCamera::UpdateCameraScence4() {

}