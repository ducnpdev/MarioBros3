#pragma once
#include "GameObject.h"

#define PIPE_STATE_NORMAL		0
#define PIPE_STATE_UP_DOWN		1
#define PIPE_STATE_DOWN_UP		2
#define PIPE_STATE_UP			3
#define PIPE_STATE_UP_DOWN_FAST	4

#define PIPE_BBOX_WIDTH 15
#define PIPE_BBOX_HEIGHT 15

#define SIZE 16
class CPipe : public CGameObject
{
	int type;
public:
	CPipe(int t) { type = t; }
	int GetType() { return type; }
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

