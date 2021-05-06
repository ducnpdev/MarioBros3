#pragma once
#include "GameObject.h"
#include "Collision.h"

#define PIRANHAPLANT_SPEECH_Y					0.035f
#define PIRANHAPLANT_STATE_HIDE					1
#define PIRANHAPLANT_STATE_APPEARANCE			2
#define PIRANHAPLANT_STATE_MOVE_UP				3
#define PIRANHAPLANT_STATE_MOVE_DOWN			4

#define PIRANHAPLANT_STATE_DESTROY				5

#define PIRANHAPLANT_RANGE_ATTACK_PLAYER_X		150

#define PIRANHAPLANT_MOVE_UP_TIME				920		
#define PIRANHAPLANT_MOVE_DOWN_TIME				2500
#define PIRANHAPLANT_APPEARANCE_TIME			2000
#define PIRANHAPLANT_RESET_TIME					3420

#define PIRANHAPLANT_BBOX_WIDTH					16
#define PIRANHAPLANT_BBOX_HEIGHT				22

class CPiranhaPlant : public CCollision
{
	float sx, sy, tan;
	float y_limit;

	int move_up_state;
	int move_down_state;
	int appearance_state;
	int hide_state;
	bool check_y_limit;
	bool found_player;

	DWORD move_up_time;
	CGameObject* player;

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
	CPiranhaPlant(CGameObject* mario);
	virtual void SetState(int state);
	virtual int GetState();
};

