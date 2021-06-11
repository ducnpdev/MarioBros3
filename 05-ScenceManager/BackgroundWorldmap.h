#pragma once

#include "GameObject.h"

#define DARK_BACKGROUND_POSITION_X			2496
#define DARK_BACKGROUND_POSITION_Y			133

#define BACKGROUND_OBJECTS_WIDTH_AMOUNT		21
#define BACKGROUND_OBJECTS_HEIGHT_AMOUNT	17

#define BACKGROUND_OBJECT_SIZE				16


using namespace std;

class CBackgroundWorldmap : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

