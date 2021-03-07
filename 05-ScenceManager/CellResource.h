#pragma once
#include "GameObject.h"

class CCellResource {
	vector<LPGAMEOBJECT> listResourceInCell;
public:
	void PushObjectToCellResource(CGameObject* obj);
	vector<LPGAMEOBJECT> GetListResourceInCell();
	void RemoveResource();
};

