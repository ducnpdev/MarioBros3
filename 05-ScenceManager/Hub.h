#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "Mario.h"
#include "Utils.h"

class CHub : public CGameObject
{
	CCamera* camera;

public:
	CHub();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetCameraInHub(CCamera* c) { camera = c; }
};

