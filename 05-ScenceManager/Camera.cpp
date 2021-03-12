#include "Camera.h"

CCamera::CCamera(CMario* m,int id) {
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
	DebugOut(L"playX %f \n", playerX);
	DebugOut(L"playY %f \n", playerY);

	CGame* game = CGame::GetInstance();
	
	//Mario chạm biên map bên phải
	if (playerX < 0) {
		player->SetPosition(CAM_X_MIN, playerY);
		SetPositionCamera(0, 267.0f);
	}

	if (playerX > 2656)
	{
		if (playerX > 2816) player->SetPosition(2816, playerY);
		playerX = 2504;
		SetPositionCamera(playerX, 2504);
	}

	else if (playerX > (game->GetScreenHeight()/2+48)) {
		playerX -= game->GetScreenWidth() / 2;
		playerY -= game->GetScreenHeight() / 2;
		CGame::GetInstance()->SetCamPos(playerX, playerY);
		SetPositionCamera(playerX, playerY);

	}
	else {
		CGame::GetInstance()->SetCamPos(0.0f, CAM_Y_MIN);
		SetPositionCamera(0.0f, 267.0f);
	}
	playerY -= game->GetScreenHeight() / 2;

	//if (playerX > 5000) {
	//	if (playerX > 5000)  player->SetPosition(5000, playerY);
	//	SetPositionCamera(300, 267);
	//}
	//else if (playerX > game->GetScreenWidth() / 2 && playerY > 507.0f)
	//{
	//	playerX -= game->GetScreenWidth() / 2;
	//	playerY -= game->GetScreenHeight() / 2;
	//	CGame::GetInstance()->SetCamPos(round(playerX), 507.0f);
	//	SetPositionCamera(round(playerX), 507.0f);
	//}
	//else if (playerX > game->GetScreenHeight() / 2) {
	//	if (player->GetMarioLevel() == LEVEL_MARIO_TAIL)
	//	{
	//		playerX -= game->GetScreenWidth() / 2;
	//		playerY -= game->GetScreenHeight() / 2 + 10;
	//		SetPositionCamera(round(playerX), round(playerY));
	//		CGame::GetInstance()->SetCamPos((playerX), (playerY));
	//	}
	//	else
	//	{
	//		playerX -= game->GetScreenWidth() / 2;
	//		SetPositionCamera(round(playerX), 10);
	//		CGame::GetInstance()->SetCamPos(round(playerX), 267.0f);
	//	}
	//}


	//else {
	//	if(player->GetMarioLevel() == LEVEL_MARIO_TAIL && playerY < game->GetScreenHeight() / 2 ){
	//		playerY -= game->GetScreenHeight() / 2;
	//		CGame::GetInstance()->SetCamPos(0.0f, round(playerY));
	//		SetPositionCamera(0.0f, round(playerY));
	//	}
	//	else
	//	{
	//		CGame::GetInstance()->SetCamPos(0.0f, 267.0f);
	//		SetPositionCamera(0.0f, 267.0f);
	//	}
	//}
	//
}

void CCamera::UpdateCameraScence2() {

}

void CCamera::UpdateCameraScence3() {

}

void CCamera::UpdateCameraScence4() {

}