#pragma once
#include "GameObject.h"
class CQuestionBlock : public CGameObject
{
public:
	CQuestionBlock();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
};