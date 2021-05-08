#pragma once
#include "GameObject.h"

#define LEAF_BBOX_WIDTH				16
#define LEAF_BBOX_HEIGHT			16

#define LEAF_SPEED_Y				0.1f
#define LEAF_SPEED_X				0.08f
#define LEAF_GRAVITY_Y				0.03f

#define LEAF_STATE_HIDEN			0
#define EFFECT_STATE				1
#define LEAF_STATE_NORMAL			2

#define LEAF_EFFECT_LEFT_ANI		0
#define LEAF_EFFECT_RIGHT_ANI		1

#define LEAF_EFFECT_RIGHT_TIME		300

#define EFFECT_LIMIT_TIME			500


class CLeaf : public CGameObject
{
	int effect_state;
	DWORD effect_time;
	DWORD effect_time_start;
	int hiden_state;
	int intro_state;

	DWORD effect_right_time;
	DWORD effect_left_time;
	DWORD effect_right_time_start;
	DWORD effect_left_time_start;
	DWORD create_time;
	int isRight;
public:
	CLeaf();

	void SetState(int state);
	void SetIntroState(int s) { intro_state = s; }
	int IsHidenState() { return hiden_state; }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
//	void CreateIntroAnimationLeaf();
};
