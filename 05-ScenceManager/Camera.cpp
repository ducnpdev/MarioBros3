#include "Camera.h"

CCamera::CCamera(CMario* m,int id) {
	this->player = m;
	this->sceneID = id;
}

void CCamera::UpdateCamera() {
	switch (sceneID)
	{
	default:
		break;
	}
}