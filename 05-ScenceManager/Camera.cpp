#include "Camera.h"
#include "Utils.h"
#include "CameraConfig.h"

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
	case SCENE_1:
		UpdateCameraScence1();
		break;
	/*case 2:
		UpdateCameraScence1();
		break;*/
	case SCENE_3:
		UpdateCameraScence3();
		break;
	/*case 4:
		UpdateCameraScence1();
		break;*/
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

	// mario khi đi đến giữa và ở dưới cống 
	else if (playerX > game->GetScreenWidth() / 2 && playerY > UNDERGROUND_Y) {
		playerX -= game->GetScreenWidth() / 2;
		playerY -= game->GetScreenHeight() / 2;
		CGame::GetInstance()->SetCamPos(round(playerX), CAM_Y_UNDERGROUND);
		SetPositionCamera(round(playerX), CAM_Y_UNDERGROUND);
	}

	// Mario khi đi đến giữa màn hình game
	else if (playerX > game->GetScreenWidth() / 2 ) {
		float speedX; float speedY;
		player->GetSpeed(speedX, speedY);
		if (player->GetMarioLevel() == LEVEL_MARIO_TAIL 
			&& player->GetMarioAcceptFlyCamera()
			)
		{
			if (playerY < game->GetScreenHeight() / 2 + 30) {
				playerX -= game->GetScreenWidth() / 2;
				SetPositionCamera(round(playerX), 0);
				CGame::GetInstance()->SetCamPos(round(playerX), 0);
			}
			else {

				playerX -= game->GetScreenWidth() / 2;
				playerY -= game->GetScreenHeight() / 2 + CAM_Y_PLUS;
				SetPositionCamera(round(playerX), round(playerY));
				CGame::GetInstance()->SetCamPos(round(playerX), round(playerY));
			}
		}
		/*else if (player->GetMarioLevel() == LEVEL_MARIO_TAIL) {
			if (playerY < game->GetScreenHeight() / 2 + 30) {
				playerX -= game->GetScreenWidth() / 2;
				SetPositionCamera(round(playerX), 0);
				CGame::GetInstance()->SetCamPos(round(playerX), 0);
			}
			else {
				playerX -= game->GetScreenWidth() / 2;
				playerY -= game->GetScreenHeight() / 2 + CAM_Y_PLUS;
				SetPositionCamera(round(playerX), round(playerY));
				CGame::GetInstance()->SetCamPos(round(playerX), round(playerY));
			}
		}*/

		else {
			playerX -= game->GetScreenWidth() / 2;
			playerY -= game->GetScreenHeight() / 2;
			// thay đổi round(playerY) thành CAM_Y_MIN là fix cứng cameraY
			CGame::GetInstance()->SetCamPos(round(playerX), round(CAM_Y_MIN));
			// SetPositionCamera(round(playerX), round(CAM_Y_MIN));
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


void CCamera::UpdateCameraScence3Right() {
	if (!player->GetMarioScene3Right()) {
		player->SetPosition(CAM_3_X_SCENE_RIGHT_POS_MARIO, playerY);
		player->SetMarioScene3Right(true);
	}
	else {
		player->SetPosition(playerX, playerY);
	}
	SetPositionCamera(CAM_3_X_SCENE_RIGHT_DEFAULT, CAM_3_Y_DEFAULT);
	CGame::GetInstance()->SetCamPos(round(CAM_3_X_SCENE_RIGHT_DEFAULT), round(CAM_3_Y_DEFAULT));
}

void CCamera::UpdateCameraScence3Top() {
	float ydefault = 20.0f;
	player->GetPosition(playerX, playerY);
	CGame* game = CGame::GetInstance();
	if (playerX < CAM_3_TOP_X_MIN_CAM) {
		if (playerX < CAM_3_TOP_X_MARIO) player->SetPosition(CAM_3_TOP_X_MARIO, playerY);
	//
		if (player->GetMarioLevel() == LEVEL_MARIO_TAIL && player->GetMarioAcceptFlyCamera())
		{
		//	DebugOut(L"2222222 \n");
			if (playerY < game->GetScreenHeight() / 2 + 30) {
				playerX -= game->GetScreenWidth() / 2;
				SetPositionCamera(round(CAM_3_TOP_X_MARIO), ydefault);
				CGame::GetInstance()->SetCamPos(round(CAM_3_TOP_X_MARIO), ydefault);
			}
			else {
				playerX -= game->GetScreenWidth() / 2;
				playerY -= game->GetScreenHeight() / 2 + CAM_Y_PLUS;
				SetPositionCamera(round(CAM_3_TOP_X_MARIO), round(playerY));
				CGame::GetInstance()->SetCamPos(round(CAM_3_TOP_X_MARIO), round(playerY));
			}
		} else {
			CGame::GetInstance()->SetCamPos(round(CAM_3_TOP_X_MARIO), round(CAM_Y_MIN));
		}
	}

	else if (playerX > CAM_3_TOP_Y_MAX_MARIO)
	{
		if (playerX > CAM_3_TOP_Y_MAX_MARIO_1) player->SetPosition(2200, playerY);

		if (player->GetMarioLevel() == LEVEL_MARIO_TAIL && player->GetMarioAcceptFlyCamera())
		{
			//
			if (playerY < game->GetScreenHeight() / 2 + 30) {
				DebugOut(L"Right 111 \n");
				playerX -= game->GetScreenWidth() / 2;

				SetPositionCamera(round(CAM_3_TOP_Y_MAX_CAM), ydefault);
				CGame::GetInstance()->SetCamPos(round(CAM_3_TOP_Y_MAX_CAM), ydefault);
			}
			else {
				DebugOut(L"Right 222 \n");
				playerX -= game->GetScreenWidth() / 2;
				playerY -= game->GetScreenHeight() / 2 + CAM_Y_PLUS;
				SetPositionCamera(round(CAM_3_TOP_Y_MAX_CAM), round(playerY));
				CGame::GetInstance()->SetCamPos(round(CAM_3_TOP_Y_MAX_CAM), round(playerY));
			}
		}
		else {
			SetPositionCamera(CAM_3_TOP_Y_MAX_CAM, CAM_Y_MIN); // CAM_Y_MIN
		}
	}
	else {
	//	DebugOut(L"1111111 \n");
		if (player->GetMarioLevel() == LEVEL_MARIO_TAIL && player->GetMarioAcceptFlyCamera())
		{
		//	DebugOut(L"2222222 \n");
			if (playerY < game->GetScreenHeight() / 2 + 30) {
				playerX -= game->GetScreenWidth() / 2;
				SetPositionCamera(round(playerX), ydefault);
				CGame::GetInstance()->SetCamPos(round(playerX), ydefault);
			}
			else {
				playerX -= game->GetScreenWidth() / 2;
				playerY -= game->GetScreenHeight() / 2 + CAM_Y_PLUS;
				SetPositionCamera(round(playerX), round(playerY));
				CGame::GetInstance()->SetCamPos(round(playerX), round(playerY));
			}
		}
		else {
			// DebugOut(L"3333333 \n");
			playerX -= game->GetScreenWidth() / 2;
			playerY -= game->GetScreenHeight() / 2;
			CGame::GetInstance()->SetCamPos(round(playerX), round(CAM_Y_MIN));
			SetPositionCamera(round(playerX), round(CAM_Y_MIN));
		}
	}
}

void CCamera::UpdateCameraScence3TopNew() {
	player->GetPosition(playerX, playerY);
	CGame* game = CGame::GetInstance();
	float defaultY = CAM_Y_MIN + 16.0f;
	float defaultX = CAM_3_TOP_X_MARIO;
	if (scene3TOP_X < CAM_3_TOP_X_INIT) {
		scene3TOP_X = defaultX;
	}
	if (playerX < CAM_3_TOP_X_MARIO) player->SetPosition(CAM_3_TOP_X_MARIO, playerY);
	if (playerX > CAM_3_TOP_Y_MAX_MARIO_1) player->SetPosition(CAM_3_TOP_Y_MAX_MARIO_1, playerY);
	if (playerX > (scene3TOP_X + CAM_3_TOP_X_OVER_CAMERA)) {
		player->SetPosition(scene3TOP_X + CAM_3_TOP_X_OVER_CAMERA, playerY);
	}
	if (!checkLeft) {
		scene3TOP_X = scene3TOP_X + 0.9f;
	}

	if (scene3TOP_X > CAM_3_TOP_Y_MAX_CAM) {
		checkLeft = true;
	}
	if (playerX < scene3TOP_X) {
		player->SetState(STATE_MARIO_SCENE_3_TOP_LEFT);
		player->SetPosition(scene3TOP_X, playerY);
	}

	if (playerX > CAM_3_TOP_Y_MAX_MARIO)
	{
		//if (player->GetMarioLevel() == LEVEL_MARIO_TAIL && player->GetMarioAcceptFlyCamera())
		//{
		//	if (playerY < game->GetScreenHeight() / 2 + 30) {
		//		playerX -= game->GetScreenWidth() / 2;
		//		SetPositionCamera(round(CAM_3_TOP_Y_MAX_CAM), defaultY);
		//		CGame::GetInstance()->SetCamPos(round(CAM_3_TOP_Y_MAX_CAM), defaultY);
		//	}
		//	else {
		//	//	DebugOut(L"Right 222 \n");
		//		playerX -= game->GetScreenWidth() / 2;
		//		playerY -= game->GetScreenHeight() / 2 + CAM_Y_PLUS;
		//		SetPositionCamera(round(CAM_3_TOP_Y_MAX_CAM), round(playerY));
		//		CGame::GetInstance()->SetCamPos(round(CAM_3_TOP_Y_MAX_CAM), round(playerY));
		//	}
		//}
		//else {
			CGame::GetInstance()->SetCamPos(round(CAM_3_TOP_Y_MAX_CAM), round(defaultY));
			SetPositionCamera(CAM_3_TOP_Y_MAX_CAM, defaultY); // CAM_Y_MIN
		// }
	}
	else {
		SetPositionCamera(round(scene3TOP_X), round(defaultY));
		CGame::GetInstance()->SetCamPos(round(scene3TOP_X), round(defaultY));

	}

}

void CCamera::UpdateCameraScence3() {
	player->GetPosition(playerX, playerY);
	CGame* game = CGame::GetInstance();
	
	if (player->GetMarioIsAcceptPortal()) {
		UpdateCameraScence3Right();
		return;
	}

	if (player->GetMarioScene3Top()) {
		// UpdateCameraScence3Top();
		UpdateCameraScence3TopNew();
		return;
	}

	// Mario khi đi đến sát biên bên trái
	if (playerX < WIDTH_MIN_MAP_2) {
		player->SetPosition(CAM_X_MIN, playerY);
		SetPositionCamera(0, CAM_3_Y_DEFAULT);
	}

	// Mario khi đi đến sát biên bên phải 
	if (playerX > CAM_3_X_SCENE_RIGHT_MAX_MARIO)
	{
		if (playerX > CAM_3_X_SCENE_RIGHT_3_MARIO) player->SetPosition(CAM_3_X_SCENE_RIGHT_3_MARIO, playerY);
		playerX = CAM_3_X_SCENE_RIGHT_1_MARIO;
		SetPositionCamera(playerX, CAM_3_Y_DEFAULT); // CAM_Y_MIN
	}
	else if (playerX > (game->GetScreenWidth() / 2)) {
		if (player->GetMarioLevel() == LEVEL_MARIO_TAIL && player->GetMarioAcceptFlyCamera())
		{
			if (playerY < game->GetScreenHeight() / 2 + 30) {
				playerX -= game->GetScreenWidth() / 2;
				SetPositionCamera(round(playerX), 0);
				CGame::GetInstance()->SetCamPos(round(playerX), 0);
			}
			else {

				playerX -= game->GetScreenWidth() / 2;
				playerY -= game->GetScreenHeight() / 2 + CAM_Y_PLUS;
				SetPositionCamera(round(playerX), round(playerY));
				CGame::GetInstance()->SetCamPos(round(playerX), round(playerY));
			}
		}
		else {
			playerX -= game->GetScreenWidth() / 2;
			playerY -= game->GetScreenHeight() / 2;
			// thay đổi round(playerY) thành CAM_Y_MIN là fix cứng cameraY
			CGame::GetInstance()->SetCamPos(round(playerX), round(CAM_3_Y_DEFAULT));
			SetPositionCamera(round(playerX), round(CAM_3_Y_DEFAULT));
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
			CGame::GetInstance()->SetCamPos(CAM_3_X_MIN, round(CAM_3_Y_DEFAULT));
			SetPositionCamera(CAM_3_X_MIN, round(CAM_3_Y_DEFAULT));
		}
	}
	playerY -= game->GetScreenHeight() / 2;
}

void CCamera::UpdateCameraScence2() {
}

void CCamera::UpdateCameraScence4() {
}