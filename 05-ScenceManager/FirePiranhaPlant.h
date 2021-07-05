#pragma once
#include "GameObject.h"
#include "Collision.h"

#define FIREPIRANHAPLANT_SPEECH_Y			0.035f

#define FIREPIRANHAPLANT_STATE_HIDE					1
#define FIREPIRANHAPLANT_STATE_APPEARANCE			2
#define FIREPIRANHAPLANT_STATE_MOVE_UP				3
#define FIREPIRANHAPLANT_STATE_MOVE_DOWN			4
#define FIREPIRANHAPLANT_STATE_DESTROY				5

#define FIREPIRANHAPLANT_ANI_DOWN_LEFT				0
#define FIREPIRANHAPLANT_ANI_TOP_LEFT				1
#define FIREPIRANHAPLANT_ANI_DOWN_RIGHT				2
#define FIREPIRANHAPLANT_ANI_TOP_RIGHT				3

#define FIREPIRANHAPLANT_RANGE_ATTACK_PLAYER_X		150

#define FIREPIRANHAPLANT_MOVE_UP_TIME				920
#define FIREPIRANHAPLANT_SHOOT_TIME					2000			
#define FIREPIRANHAPLANT_MOVE_DOWN_TIME				3500
#define FIREPIRANHAPLANT_APPEARANCE_TIME			3000
#define FIREPIRANHAPLANT_RESET_TIME					4420

#define FIREPIRANHAPLANT_POSITION_RIGHT_X			20

#define FIREPIRANHAPLANT_TAN_ANGLE_ATTACK			1.7f

#define FIREPIRANHAPLANT_BBOX_WIDTH			16
#define FIREPIRANHAPLANT_BBOX_HEIGHT		33
#define FIREPIRANHAPLANT_Y_LIMIT 336.0f
#define FIREPIRANHAPLANT_RANGE_EFFECT_MARIO 150

class CFirePiranhaPlant : public CCollision
{
	float sx, sy, tan;
	float y_limit;

	 // int move_up_state;
	bool stateMoveUp;
	DWORD timeMoveUp;

	int move_down_state;
	int appearance_state;
	int stateHide;
	bool is_shoot;
	bool check_y_limit;
	bool found_player;

	int down_left_state;
	int down_right_state;
	int top_right_state;
	int top_left_state;
	
	DWORD shoot_start;
	CGameObject* player;
	CGameObject* firePlantBullet;


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);
public:
	CFirePiranhaPlant(CGameObject* player);
	void CreateFirePlantBullet(CGameObject* fireplantbullet);
	void ShootFirePlantBullet();
	virtual void SetState(int state);
	virtual int GetState();
};